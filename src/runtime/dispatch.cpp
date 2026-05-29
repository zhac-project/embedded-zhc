// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "zhc/runtime/dispatch.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace zhc {

namespace {

bool cluster_match(const FzConverter& c, const DecodedMessage& m) {
    if (c.cluster == nullptr) return true;            // any
    if (m.cluster == nullptr) return true;            // id-based dispatch only
    return std::strcmp(c.cluster, m.cluster) == 0;
}

bool selector_match(const FzConverter& c,
                    const DecodedMessage& m,
                    std::uint8_t raw_endpoint) {
    if (c.family != m.family) return false;
    if (!cluster_match(c, m))  return false;

    const std::uint32_t type_bit_m = type_bit(m.type);
    if ((c.type_mask & type_bit_m) == 0) return false;

    if (c.command_id != WILDCARD_CMD_ID && c.command_id != m.command_id) {
        return false;
    }
    if (c.endpoint != WILDCARD_ENDPOINT && c.endpoint != raw_endpoint) {
        return false;
    }
    if (c.direction != m.direction) return false;

    // attr_id + frame_flags: reserved in v1 — documented in definition.hpp.
    return true;
}

// Merge each KeyValue from `src` into `dst`. On key collision the later
// value wins, mirroring z2m converter output precedence.
bool merge_payload(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& src,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& dst) {
    for (std::uint8_t i = 0; i < src.count; ++i) {
        const auto& kv = src.items[i];
        bool replaced = false;
        for (std::uint8_t j = 0; j < dst.count; ++j) {
            const bool same_ptr = dst.items[j].key == kv.key;
            const bool same_str = !same_ptr &&
                std::strcmp(dst.items[j].key, kv.key) == 0;
            if (same_ptr || same_str) {
                dst.items[j].value = kv.value;
                replaced = true;
                break;
            }
        }
        if (!replaced) {
            if (!dst.put(kv.key, kv.value)) return false;  // dst full
        }
    }
    return true;
}

// ── Multi-endpoint suffix rewrite ────────────────────────────────────
//
// Keys in this list are global to the device — they describe the
// device, not a particular endpoint, so they are NEVER suffixed even
// when the device opts in via `def.endpoint_map`. Rationale per key:
//   * battery / battery_low / voltage — single battery per device.
//   * linkquality — meta-attribute, attached by the radio layer.
//   * device_temperature — single MCU chip-temp sensor.
//   * tamper — IAS Zone status bit, one zone per device.
//   * action — button-press strings encode the EP in the value
//     ("press_l1"), not the key. If a future device wants per-EP
//     `action_l1`/`action_l2`, drop it from this list and add a
//     per-def override.
constexpr const char* kAlwaysGlobalKeys[] = {
    "battery", "battery_low", "voltage", "linkquality",
    "device_temperature", "tamper", "action",
};

bool is_always_global_key(const char* key, const PreparedDefinition& def) {
    if (!key) return true;  // null is always global (skip rewrite)
    for (const char* k : kAlwaysGlobalKeys) {
        if (std::strcmp(k, key) == 0) {
            // Per-device opt-in: multi-EP key fobs / scene remotes set
            // `endpoint_action_suffix` so `action` flows through the
            // suffix rewrite (e.g. `action_l1`, `action_top_left`).
            if (def.endpoint_action_suffix &&
                std::strcmp(key, "action") == 0) {
                return false;
            }
            return true;
        }
    }
    return false;
}

const EndpointLabel* lookup_endpoint_label(const PreparedDefinition& def,
                                            std::uint8_t endpoint) {
    if (!def.endpoint_map || def.endpoint_map_count == 0) return nullptr;
    for (std::uint8_t i = 0; i < def.endpoint_map_count; ++i) {
        if (def.endpoint_map[i].endpoint == endpoint) {
            return &def.endpoint_map[i];
        }
    }
    return nullptr;
}

// Allocates a `<base>_<label>` string in `ctx.ep_scratch` and returns
// a stable pointer. Returns nullptr (no rewrite) when the arena would
// overflow — caller leaves the original key intact in that case.
const char* alloc_suffixed_key(RuntimeContext& ctx,
                                const char* base,
                                const char* label) {
    if (!base || !label) return nullptr;
    const std::size_t base_len  = std::strlen(base);
    const std::size_t label_len = std::strlen(label);
    const std::size_t need = base_len + 1 + label_len + 1;  // +'_' +'\0'
    if (ctx.ep_scratch_used + need > sizeof(ctx.ep_scratch)) return nullptr;
    char* p = ctx.ep_scratch + ctx.ep_scratch_used;
    std::memcpy(p, base, base_len);
    p[base_len] = '_';
    std::memcpy(p + base_len + 1, label, label_len);
    p[base_len + 1 + label_len] = '\0';
    ctx.ep_scratch_used = static_cast<std::uint16_t>(ctx.ep_scratch_used + need);
    return p;
}

void rewrite_keys_with_endpoint(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& merged,
                                 const EndpointLabel& lbl,
                                 const PreparedDefinition& def,
                                 RuntimeContext& ctx) {
    for (std::uint8_t i = 0; i < merged.count; ++i) {
        const char* key = merged.items[i].key;
        if (is_always_global_key(key, def)) continue;
        const char* new_key = alloc_suffixed_key(ctx, key, lbl.label);
        if (new_key) merged.items[i].key = new_key;
        // Otherwise the arena was full — preserve original key, drop
        // the suffix this once. Operator-visible: a key like `state`
        // appears unsuffixed instead of vanishing. Safer than the
        // alternative. F41 (FINDINGS.md) KNOWN LIMIT: two endpoints can then
        // collide on the same unsuffixed key (last-writer-wins in the merge).
        // Proper fix = size ep_scratch to ZHC_FIXED_PAYLOAD_CAP × worst-case
        // key length, but RuntimeContext is stack-allocated (zhc_adapter.cpp),
        // so the dispatch task's stack budget must be checked on HW first.
    }
}

// Fallback: surface unmapped Tuya datapoints as `dp_<id>` so a device whose
// matched def carries no DP map (placeholder / un-ported) still shows its raw
// values instead of a silent no-match. Scalar DPs only (Bool/Int/Uint); raw
// byte blobs and strings are skipped in v1. Fires from dispatch_from_zigbee
// ONLY when no converter claimed the frame, so fully-ported devices — which
// emit their mapped keys and set any_matched — are never affected.
bool emit_unmapped_tuya_dps(std::span<const TuyaDpRecord> dps,
                            RuntimeContext& ctx,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool any = false;
    for (const auto& rec : dps) {
        Value v{};
        // Tuya DP type byte: 0x01 Bool, 0x02 Numeric (s32 BE), 0x04 Enum (u8),
        // 0x05 Bitmap (u8/u16/u32). Raw (0x00) / String (0x03) skipped in v1.
        // Decoded inline (not via the zcl decode_tuya_dp) to keep the runtime
        // layer free of a zcl-lib link dependency.
        if (rec.dp_type == 0x01) {
            if (rec.value.empty()) continue;
            v.type = ValueType::Bool;
            v.b = rec.value[rec.value.size() - 1] != 0;
        } else if ((rec.dp_type == 0x02 || rec.dp_type == 0x04 ||
                    rec.dp_type == 0x05) &&
                   !rec.value.empty() && rec.value.size() <= 4) {
            std::int64_t iv = 0;
            for (const auto x : rec.value) iv = (iv << 8) | x;
            v.type = ValueType::Int; v.i = iv;
        } else {
            continue;  // raw blob / string / malformed
        }
        char kb[12];
        const int n = std::snprintf(kb, sizeof(kb), "dp_%u",
                                    static_cast<unsigned>(rec.dp_id));
        if (n <= 0 || static_cast<std::size_t>(n) >= sizeof(kb)) continue;
        const char* key = ctx.alloc_str(kb, static_cast<std::size_t>(n));
        if (!key) continue;
        if (out.put(key, v)) any = true;
    }
    return any;
}

}  // namespace

DispatchResult dispatch_from_zigbee(const DecodedMessage& msg,
                                     std::span<const TuyaDpRecord> tuya_dps,
                                     const PreparedDefinition& def,
                                     const InboundApsFrame& raw_frame,
                                     RuntimeContext& ctx) {
    DispatchResult result{};

    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* cvt = def.from_zigbee[i];
        if (!cvt) continue;
        if (!selector_match(*cvt, msg, raw_frame.src_endpoint)) continue;

        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> scratch{};
        bool emitted = false;
        if (cvt->family == FrameFamily::TuyaDp) {
            if (cvt->fn.tuya_fn) {
                emitted = cvt->fn.tuya_fn(tuya_dps, msg, *cvt, def, ctx, scratch);
            }
        } else {
            if (cvt->fn.zcl_fn) {
                emitted = cvt->fn.zcl_fn(msg, *cvt, def, ctx, scratch);
            }
        }

        if (emitted) {
            result.any_matched = true;
            if (!merge_payload(scratch, result.merged)) {
                // merged full — bail early but don't drop what we have.
                break;
            }
        }
    }

    // No converter claimed this frame. If it carried Tuya datapoints, surface
    // them as `dp_<id>` rather than dropping silently — un-ported / placeholder
    // devices become diagnosable (and usable in rules) instead of invisible.
    if (!result.any_matched && !tuya_dps.empty()) {
        FixedPayload<ZHC_FIXED_PAYLOAD_CAP> scratch{};
        if (emit_unmapped_tuya_dps(tuya_dps, ctx, scratch)) {
            result.any_matched = true;
            merge_payload(scratch, result.merged);
        }
    }

    if (!result.any_matched && ctx.unhandled) {
        ctx.unhandled(raw_frame, &def, ctx);
    }
    // Multi-endpoint key suffixing. When the device opts in via
    // `def.endpoint_map`, rewrite every emitted key in `result.merged`
    // to `<key>_<label>` based on `msg.src_endpoint`. Keys in the
    // global blocklist (`battery`, `linkquality`, etc.) stay as-is —
    // they're per-device, not per-endpoint. Single-EP devices (default
    // null map) are unaffected.
    if (def.endpoint_map && def.endpoint_map_count > 0) {
        if (const EndpointLabel* lbl =
                lookup_endpoint_label(def, msg.src_endpoint)) {
            rewrite_keys_with_endpoint(result.merged, *lbl, def, ctx);
        }
    }
    // Sleep-aware hook: fire `Checkin` after every inbound message so
    // per-device on_event handlers can push pending writes (time
    // sync, reporting reapply, etc.) within the device's wake window.
    // Battery-powered sleepy devices wake → publish → sleep, leaving
    // a narrow window during dispatch where the coordinator can talk
    // back. The handler self-filters by event/condition.
    if (def.on_event) {
        def.on_event(ctx.device_index, EventId::Checkin, ctx);
    }
    return result;
}

bool run_configure(const PreparedDefinition& def, RuntimeContext& ctx) {
    bool ok = true;
    if (ctx.configure_bind) {
        for (std::uint8_t i = 0; i < def.bindings_count; ++i) {
            const auto& b = def.bindings[i];
            if (!ctx.configure_bind(ctx.device_index, b.endpoint,
                                     b.cluster_id)) {
                ok = false;
                break;
            }
        }
    }
    if (ok && ctx.configure_report) {
        for (std::uint8_t i = 0; i < def.reports_count; ++i) {
            const auto& r = def.reports[i];
            if (!ctx.configure_report(ctx.device_index, r.endpoint,
                                       r.cluster_id, r.attr_id,
                                       r.attr_type, r.min_interval,
                                       r.max_interval,
                                       r.reportable_change,
                                       r.manufacturer_code)) {
                // `kReportFlagOptional` lets a per-report failure stay
                // local — the rest of the report set + config_steps
                // still run. Mandatory failures still abort so quirky
                // mains-powered devices don't end up half-configured.
                if (r.flags & kReportFlagOptional) continue;
                ok = false;
                break;
            }
        }
    }
    // Declarative step pipeline. Runs after bindings + reports so any
    // magic-packet / custom-command walk can assume binds landed. First
    // failing step aborts — the retry loop in
    // `zigbee_configure_queue` handles re-attempts.
    if (ok && def.config_steps && def.config_steps_count > 0) {
        for (std::uint8_t i = 0; i < def.config_steps_count; ++i) {
            const ConfigStep& s = def.config_steps[i];
            const std::uint8_t ep = s.endpoint ? s.endpoint : 1;
            // Per-step timeout is NOT implemented in v1. Take a local
            // copy of `wait_ms` so Wait sees the device-supplied value
            // but Read/Cmd/Callback always see zero — keeps the contract
            // unambiguous if future device cpps mis-set the field.
            std::uint16_t step_wait_ms =
                (s.op == ConfigStepOp::Wait) ? s.wait_ms : 0;
            bool step_ok = true;

            switch (s.op) {
                case ConfigStepOp::Read:
                    // `payload_len` holds BYTES (2 per attribute, LE).
                    // The platform hook expects an attribute count, so
                    // halve on the way out.
                    step_ok = ctx.configure_read
                        ? ctx.configure_read(ctx.device_index, ep,
                                              s.cluster_id, s.payload,
                                              static_cast<std::uint8_t>(s.payload_len / 2),
                                              s.manu_code)
                        : false;
                    break;
                case ConfigStepOp::Cmd:
                    step_ok = ctx.configure_cmd
                        ? ctx.configure_cmd(ctx.device_index, ep,
                                             s.cluster_id, s.cmd_id,
                                             s.payload, s.payload_len,
                                             s.flags)
                        : false;
                    break;
                case ConfigStepOp::Callback:
                    if (def.config_callbacks &&
                        s.cmd_id < def.config_callbacks_count) {
                        auto cb = def.config_callbacks[s.cmd_id];
                        step_ok = cb
                            ? cb(ctx.device_nwk, ep, ctx)
                            : true;   // null slot = skip
                    } else {
                        step_ok = false;   // bad index
                    }
                    break;
                case ConfigStepOp::Wait:
                    if (ctx.configure_sleep) ctx.configure_sleep(step_wait_ms);
                    step_ok = true;
                    break;
            }

            if (!step_ok) { ok = false; break; }
        }
    }

    // Legacy `def.configure` only runs when the declarative phases
    // succeeded. Devices in the transition period (config_steps + legacy
    // hook) would otherwise double-configure on every transport failure.
    if (ok && def.configure) def.configure(ctx.device_index, ctx);
    return ok;
}

TzDispatchResult dispatch_to_zigbee(const PreparedDefinition& def,
                                     std::string_view key,
                                     const Value& input,
                                     RuntimeContext& ctx,
                                     std::span<std::uint8_t> out_frame) {
    TzDispatchResult r{};
    for (std::uint8_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* cvt = def.to_zigbee[i];
        if (!cvt || !cvt->fn) continue;
        // Null `cvt->key` means wildcard — the converter's own fn
        // decides which keys to honour. Used by meta-driven encoders
        // like the Tuya DP writer which claims any key in its DP map.
        if (cvt->key && key != cvt->key) continue;
        std::size_t n = 0;
        if (!cvt->fn(key, input, *cvt, def, ctx, out_frame, n)) continue;
        r.ok         = true;
        r.cluster_id = cvt->cluster_id;
        r.command_id = cvt->command_id;
        r.frame_size = n;
        return r;
    }
    return r;
}

}  // namespace zhc
