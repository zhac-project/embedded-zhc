// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Girier (Tuya-OEM) device family.
// z2m-source: girier.ts.
//
// Bug fixed: JR-ZDS01 (1 gang mini switch) is z2m
// tuya.modernExtend.tuyaOnOff({switchType: true}), which surfaces THREE
// states: `state` (genOnOff on/off), `power_on_behavior` (genOnOff attr
// 0x8002 moesStartUpOnOff) and `switch_type` (manuSpecificTuya2/0xE001 attr
// 0xD030 switchType). The generated Tier-1 def carried only `state`; the
// two config-attr decoders + their write paths were dropped. The def was
// graduated to a Tier-2 override wiring the existing tuya/_shared converters
// (kFzTuyaPowerOnBehavior / kFzTuyaSwitchType + kTz twins).
//
// These tests pin both the expose surface and the runtime attribute decode
// (power_on_behavior off/on; switch_type toggle/state/momentary).

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/devices/girier_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::girier {
extern const PreparedDefinition kDef_JR_ZDS01;
}  // namespace zhc::devices::girier

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("  FAIL: %s\n", what);
        ++g_failures;
    }
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

const Expose* def_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool def_has_manu(const PreparedDefinition& def, const char* manu) {
    for (std::size_t i = 0; i < def.manufacturer_names_count; ++i)
        if (def.manufacturer_names[i] && std::strcmp(def.manufacturer_names[i], manu) == 0)
            return true;
    return false;
}

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

const Value* find(const DispatchResult& r, const char* k) { return r.merged.find(k); }

// Report Attributes (server->client) frame for a single ENUM8 attribute:
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attrId_lo  attrId_hi  dataType=0x30  value(1)
std::array<std::uint8_t, 7> enum8_report(std::uint16_t attr_id, std::uint8_t value) {
    return { 0x18, 0x42, 0x0A,
             static_cast<std::uint8_t>(attr_id & 0xFF),
             static_cast<std::uint8_t>(attr_id >> 8),
             0x30, value };
}

// ── expose surface: state + the two config attrs ─────────────────────
void test_expose_surface() {
    const auto& def = devices::girier::kDef_JR_ZDS01;
    check(def_exposes(def, "state"), "JR-ZDS01 exposes state");
    check(def_exposes(def, "power_on_behavior"), "JR-ZDS01 exposes power_on_behavior");
    check(def_exposes(def, "switch_type"), "JR-ZDS01 exposes switch_type");

    const Expose* pob = def_expose(def, "power_on_behavior");
    check(pob && pob->type == ExposeType::Enum, "power_on_behavior is an enum");
    const Expose* st = def_expose(def, "switch_type");
    check(st && st->type == ExposeType::Enum, "switch_type is an enum");

    // Plain genOnOff switch — binds 0x0006, NOT a 0xEF00 Tuya-DP device.
    check(def_binds(def, 0x0006), "binds genOnOff");
    check(!def_binds(def, 0xEF00), "not a Tuya-DP device");
    // Single gang: no endpoint_map.
    check(def.endpoint_map_count == 0, "single gang carries no endpoint_map");
    // Fingerprint manufacturer set intact.
    check(def_has_manu(def, "_TZ3000_majwnphg"), "manu majwnphg");
    check(def_has_manu(def, "_TZ3000_6axxqqi2"), "manu 6axxqqi2");
    check(def_has_manu(def, "_TZ3000_zw7yf6yk"), "manu zw7yf6yk");
}

// ── runtime: power_on_behavior decodes genOnOff attr 0x8002 ──────────
void test_power_on_behavior_decode() {
    const auto& def = devices::girier::kDef_JR_ZDS01;
    {
        auto f = enum8_report(0x8002, 0);  // off
        auto r = dispatch(def, 0x0006 /* genOnOff */,
                          std::span<const std::uint8_t>(f.data(), f.size()));
        const Value* v = find(r, "power_on_behavior");
        check(v && v->type == ValueType::StringRef && std::strcmp(v->str, "off") == 0,
              "power_on_behavior 0 -> off");
    }
    {
        auto f = enum8_report(0x8002, 1);  // on
        auto r = dispatch(def, 0x0006,
                          std::span<const std::uint8_t>(f.data(), f.size()));
        const Value* v = find(r, "power_on_behavior");
        check(v && v->type == ValueType::StringRef && std::strcmp(v->str, "on") == 0,
              "power_on_behavior 1 -> on");
    }
}

// ── runtime: switch_type decodes manuSpecificTuya2 (0xE001) attr 0xD030 ──
void test_switch_type_decode() {
    const auto& def = devices::girier::kDef_JR_ZDS01;
    const struct { std::uint8_t raw; const char* label; } cases[] = {
        {0, "toggle"}, {1, "state"}, {2, "momentary"},
    };
    for (const auto& c : cases) {
        auto f = enum8_report(0xD030, c.raw);
        auto r = dispatch(def, 0xE001 /* manuSpecificTuya2 */,
                          std::span<const std::uint8_t>(f.data(), f.size()));
        const Value* v = find(r, "switch_type");
        check(v && v->type == ValueType::StringRef && std::strcmp(v->str, c.label) == 0,
              "switch_type decode");
    }
}

}  // namespace

int main() {
    test_expose_surface();
    test_power_on_behavior_decode();
    test_switch_type_decode();
    if (g_failures != 0) {
        std::printf("girier parity: %d failure(s)\n", g_failures);
        return 1;
    }
    std::printf("girier parity: all checks passed\n");
    return 0;
}
