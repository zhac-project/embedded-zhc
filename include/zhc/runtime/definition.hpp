// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstdint>
#include <string_view>

#include "zhc/config.hpp"
#include "zhc/platform.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/types.hpp"

namespace zhc {

struct RuntimeContext;
struct PreparedDefinition;
struct FzConverter;

// ── Event ids (v1 minimum) ───────────────────────────────────────────
// Typed to keep `OnEventFn` call-sites unambiguous.
enum class EventId : std::uint16_t {
    FirstSeen = 0,
    Checkin   = 1,
};

// ── Converter function-pointer signatures ────────────────────────────
//
// `FromZigbeeFn`   — called for ZCL-family frames.
// `FromTuyaDpFn`   — called for Tuya-family frames; receives the parsed
//                    DP record list so each Tuya converter avoids having
//                    to walk the byte stream itself.
// `ToZigbeeFn`     — outbound encoder; API landed in v1 even though most
//                    Task-5 work is decode-first.
// `ConfigureFn`    — per-device configure callback (bind + reporting).
// `OnEventFn`      — FirstSeen / Checkin hooks.

using FromZigbeeFn = bool (*)(const DecodedMessage& msg,
                               const FzConverter& self,
                               const PreparedDefinition& def,
                               RuntimeContext& ctx,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

using FromTuyaDpFn = bool (*)(std::span<const TuyaDpRecord> dps,
                               const DecodedMessage& msg,
                               const FzConverter& self,
                               const PreparedDefinition& def,
                               RuntimeContext& ctx,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

struct TzConverter;

using ToZigbeeFn = bool (*)(std::string_view key,
                             const Value& input,
                             const TzConverter& self,
                             const PreparedDefinition& def,
                             RuntimeContext& ctx,
                             std::span<std::uint8_t> out_frame,
                             std::size_t& out_size);

using ConfigureFn = void (*)(std::uint16_t device_index, RuntimeContext& ctx);

// One ZDO_BIND target that fires at device join. `endpoint` is the
// source endpoint on the device; coordinator is always the
// destination. `cluster_id` is the cluster to bind.
struct BindingSpec {
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
};

// Multi-endpoint label → endpoint mapping. Used by `PreparedDefinition::
// endpoint_map` to drive Fz key suffixing and Tz outbound routing.
// `label` is the user-facing tail string after the underscore in
// suffixed keys (e.g. `"l2"` produces keys like `state_l2`); `endpoint`
// is the actual ZCL endpoint id the device exposes.
struct EndpointLabel {
    const char*   label;     // "l1" / "l2" / "left" / "ch1"
    std::uint8_t  endpoint;  // 1 / 2 / 3
};

// One configureReporting entry — per z2m's
// `endpoint.configureReporting(cluster, [{attribute, minReportInterval,
// maxReportInterval, reportableChange}])`. `manufacturer_code` is
// non-zero only for manu-specific reports (Philips, Lumi 0xFCC0).
// `flags` bits:
//   kReportFlagOptional — failure of this specific report does NOT
//     abort `run_configure`. Default 0 = mandatory (matches z2m).
//     Use for redundant / quirky-firmware reports where the device
//     may legitimately reject the spec (e.g. cheap Tuya clones
//     refusing Configure Reporting on certain manuf-spec attrs).
inline constexpr std::uint8_t kReportFlagOptional = 0x01;

struct ReportingSpec {
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
    std::uint16_t attr_id;
    std::uint8_t  attr_type;    // ZCL data-type byte (u8=0x20, u16=0x21, …)
    std::uint16_t min_interval;
    std::uint16_t max_interval;
    std::uint32_t reportable_change;   // encoded per data type
    std::uint16_t manufacturer_code;   // 0 = not manu-specific
    std::uint8_t  flags = 0;           // see kReportFlag* above
};
using OnEventFn   = void (*)(std::uint16_t device_index, EventId event_id,
                              RuntimeContext& ctx);

// ── Declarative configure-step pipeline ─────────────────────────────
//
// Shape intentionally minimal: each step produces one over-the-air
// ZCL transaction (or a C++ escape hatch / wait). The adapter walks
// `def.config_steps` in order after `bindings` and `reports`. First
// failing step aborts the configure cycle; the outer retry loop in
// `zigbee_configure_queue` handles re-attempts.
//
// z2m-source: each `configure:` step in z2m converter files maps to
// one of `Read` / `Cmd` / `Callback`. `Wait` is a ZHC addition for
// Tuya firmwares that need settle time between probes.
enum class ConfigStepOp : std::uint8_t {
    Read     = 0,   // ZCL Read Attributes — payload = attr-id list (2 B each, LE)
    Cmd      = 1,   // Cluster-specific command — payload = body after CmdID
    Callback = 2,   // C++ hook in def.config_callbacks[cmd_id]
    Wait     = 3,   // Sleep `wait_ms` milliseconds (no radio traffic)
};

// Flag bits for `ConfigStep::flags`.
inline constexpr std::uint8_t kStepFlagDisableDefaultResponse = 0x01;
inline constexpr std::uint8_t kStepFlagManuSpecific           = 0x02;

struct ConfigStep {
    ConfigStepOp   op;
    std::uint8_t   endpoint;       // 0 → coerced to 1 by the walker
    std::uint16_t  cluster_id;     // unused for Callback/Wait
    std::uint8_t   cmd_id;         // Cmd: ZCL command id; Callback: index
    std::uint8_t   flags;          // see kStepFlag* above
    const std::uint8_t* payload;   // Read: attr-id list; Cmd: command body
    std::uint8_t   payload_len;
    // Sleep duration in ms for Wait ops only. Not consumed by the
    // dispatcher for Read/Cmd/Callback — leave zero on those steps; the
    // walker force-zeros it before any platform hook fires.
    std::uint16_t  wait_ms;
    // ZCL manufacturer code for Read/Cmd. 0 = profile-wide frame (default).
    // When non-zero the platform sets FC bit 2 (manu-specific) and inserts
    // the LE manu code between FC and TSN per ZCL spec.
    std::uint16_t  manu_code;
};

// Per-device callback signature invoked for `ConfigStepOp::Callback`.
// Returning `false` aborts the configure cycle.
using ConfigCallbackFn =
    bool (*)(std::uint16_t device_nwk, std::uint8_t endpoint,
             RuntimeContext& ctx);

// ── Selector sentinels ───────────────────────────────────────────────
constexpr std::uint16_t WILDCARD_CMD_ID  = 0xFFFF;
constexpr std::uint16_t WILDCARD_ATTR_ID = 0xFFFF;
constexpr std::uint8_t  WILDCARD_ENDPOINT = 0xFF;

// Bit positions for `FzConverter::type_mask` (bit N = MessageType N).
constexpr std::uint32_t type_bit(MessageType t) {
    return 1u << static_cast<std::uint32_t>(t);
}

// ── FzConverter ──────────────────────────────────────────────────────
//
// `family` selects which member of the function-pointer union is active:
//   FrameFamily::Zcl     → fn.zcl_fn
//   FrameFamily::TuyaDp  → fn.tuya_fn
// Mismatching the two is a porter bug and caught by CI via the tier
// comment check (Task 6).
//
// Selector enforcement in v1 dispatch (Task 4):
//   - family, cluster (null = any, else strcmp), type_mask, command_id
//     (0xFFFF = any), endpoint (0xFF = any), direction — all enforced.
//   - attr_id and frame_flags_{mask,value} are reserved fields; v1
//     dispatch does NOT enforce them. Per-converter logic may read
//     them via `user_config` and filter internally. v2 will promote
//     them to first-class selectors.
struct FzConverter {
    FrameFamily   family;
    const char*   cluster;              // nullptr = any cluster
    std::uint32_t type_mask;            // type_bit(MessageType::…) | …
    std::uint16_t command_id;           // WILDCARD_CMD_ID for any
    std::uint16_t attr_id;              // WILDCARD_ATTR_ID — reserved in v1
    std::uint8_t  endpoint;             // WILDCARD_ENDPOINT for any
    std::uint8_t  frame_flags_mask;     // reserved in v1
    std::uint8_t  frame_flags_value;    // reserved in v1
    Direction     direction;
    union {
        FromZigbeeFn zcl_fn;
        FromTuyaDpFn tuya_fn;
    } fn;
    const void* user_config;            // per-converter opaque config
};

struct TzConverter {
    const char*   key;          // input key the converter claims (e.g. "state")
    const char*   cluster;      // ZCL cluster name — debug / parity with Fz
    std::uint16_t cluster_id;   // ZCL cluster id — used by the adapter to route
    std::uint16_t command_id;   // ZCL command id encoded into the frame
    ToZigbeeFn    fn;
    const void*   user_config;
};

struct PreparedDefinition {
    // zigbeeModels as they appear in z2m (a device can carry multiple —
    // e.g. a model and its `.aq2` revision — and each must match). Use
    // the ZHC_ZIGBEE_MODELS(...) helper in the device cpp to avoid
    // boilerplate for the common single-model case.
    const char* const* zigbee_models;
    std::uint8_t       zigbee_models_count;
    // Tuya-style discriminator: ~300 Tuya devices share `modelId =
    // "TS0601"` and the only differentiator is `manufacturerName`
    // (e.g. `_TZE200_abcd1234`). When non-null, a candidate frame's
    // manufacturer name must either start with `manufacturer_name_prefix`
    // OR appear in the `manufacturer_names` set. Both null → match on
    // zigbee_models alone (lumi-style).
    const char*        manufacturer_name_prefix;
    const char* const* manufacturer_names;
    std::uint8_t       manufacturer_names_count;
    const char*  model;
    const char*  vendor;
    const void*  meta;                   // typed per-device, left opaque here
    const Expose* exposes;
    std::uint8_t  exposes_count;
    const WhiteLabel* white_labels;
    std::uint8_t      white_labels_count;
    const FzConverter* const* from_zigbee;
    std::uint8_t              from_zigbee_count;
    const TzConverter* const* to_zigbee;
    std::uint8_t              to_zigbee_count;
    ConfigureFn configure;
    OnEventFn   on_event;
    // Declarative bind + report setup fired at join. Platforms call
    // `ctx.configure_bind_fn` / `ctx.configure_report_fn` per entry.
    // Kept first-class (not in `meta`) so the adapter can walk them
    // without knowing the device's meta type. Defaults make this
    // field optional for every existing device cpp.
    const BindingSpec*   bindings       = nullptr;
    std::uint8_t         bindings_count = 0;
    const ReportingSpec* reports        = nullptr;
    std::uint8_t         reports_count  = 0;
    // Declarative configure pipeline (v2). Fires after bindings +
    // reports. Cmd/Read ops route through ctx.configure_cmd_fn /
    // configure_read_fn; Callback dispatches to
    // config_callbacks[step.cmd_id]. All default to null so device
    // cpps without steps keep compiling.
    const ConfigStep*       config_steps            = nullptr;
    std::uint8_t            config_steps_count      = 0;
    const ConfigCallbackFn* config_callbacks        = nullptr;
    std::uint8_t            config_callbacks_count  = 0;
    // Multi-endpoint demux. When non-null, the dispatcher suffixes
    // every emitted Fz key with `_<label>` (e.g. `state` → `state_l2`)
    // for the source endpoint, and the adapter strips matching
    // suffixes from outbound Tz keys to route the frame to the right
    // endpoint. Keys in `_generic/_shared.cpp::kAlwaysGlobalKeys[]`
    // (battery / linkquality / etc.) are never suffixed. Default null
    // = single-EP behaviour preserved (the vast majority of ports).
    const EndpointLabel* endpoint_map         = nullptr;
    std::uint8_t         endpoint_map_count   = 0;

    // When set, `action` is suffixed per-endpoint just like `state`.
    // Default (false) keeps `action` in the always-global blocklist
    // because most devices report a single composite action regardless
    // of which endpoint it originated from. Multi-EP key fobs / scene
    // remotes (e.g. iris 3450-L, rgb_genie ZB-3008/3009/5004) opt in
    // here so each button surfaces as `action_<label>`.
    bool                 endpoint_action_suffix = false;

    // Outbound Tz default endpoint. When non-zero AND the outbound key
    // doesn't match any `endpoint_map` suffix, the adapter routes the
    // frame to this endpoint instead of the caller-supplied `dst_ep`.
    // Mirrors z2m's `endpoint: () => ({default: 2})` for devices whose
    // root cluster (genOnOff / genLevelCtrl / lightingColorCtrl) lives
    // on a non-1 endpoint. Default 0 = no override (caller's dst_ep
    // wins, preserving single-EP behaviour for most ports).
    std::uint8_t         default_endpoint = 0;

    // ZCL Basic 0x0007 powerSource override (z2m `m.forcePowerSource`).
    // Some `_TZ3000_*` and similar devices misreport their power source
    // on Basic. When non-zero, the interview path skips the cluster
    // read and uses this value directly. Spec values:
    //   0x01 Mains (single phase), 0x02 Mains (3 phase), 0x03 Battery,
    //   0x04 DC, 0x05 Emergency mains constantly powered,
    //   0x06 Emergency mains and transfer switch.
    // Default 0 = no override → trust the cluster read.
    std::uint8_t         power_source_override = 0;
};

}  // namespace zhc
