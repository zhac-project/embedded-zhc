// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for three smarli. devices whose auto-generated ports dropped
// a real channel that z2m decodes. Each was graduated to a Tier-2 parent
// override (definitions/smarli/Sma_*.cpp).
//
//   * S-ZB-PDM1-R251 — phase dimmer + electricity meter.
//       z2m: m.light() + m.electricityMeter(). electricityMeter() defaults to
//       cluster:"both" → seMetering 0x0702 (energy) PLUS haElectricalMeasurement
//       0x0B04 (power/voltage/current). The auto-port wired only kFzMetering
//       and dropped the 0x0B04 half (current/voltage absent, power undecoded).
//       Override adds kFzElectricalMeasurement + current/voltage exposes +
//       0x0B04 binding.
//   * S-ZB-1RE1-R251 — 2ch relay + meter on endpoint 3. Same 0x0B04 gap; the
//       meter lives on ep3, so dispatch suffixes its runtime keys to <key>_3
//       (except `voltage`, which is an always-global key) via the endpoint_map.
//   * S-ZB-COV1-R251 — curtain with BOTH lift and tilt controls + meter.
//       z2m: m.windowCovering({controls:["lift","tilt"]}). The auto-port wired
//       only kFzCoverPosition (attr 0x0008) and dropped tilt (attr 0x0009) plus
//       the 0x0B04 meter half. Override adds kFzCoverTilt + kTzCoverPositionTilt
//       + "tilt" expose and the electrical-measurement channel.
//
// These tests pin the expose lists / bindings and that real ZCL reports decode
// through the def at the dispatch_from_zigbee boundary.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::smarli {
extern const PreparedDefinition kDef_S_ZB_PDM1_R251;
extern const PreparedDefinition kDef_S_ZB_1RE1_R251;
extern const PreparedDefinition kDef_S_ZB_COV1_R251;
}  // namespace zhc::devices::smarli

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                            std::uint8_t  src_endpoint,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_endpoint;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool has_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const char* n = def.exposes[i].name;
        if (n && std::strcmp(n, name) == 0) return true;
    }
    return false;
}

bool has_binding(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

bool has_fz_cluster(const PreparedDefinition& def, const char* cluster) {
    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* fz = def.from_zigbee[i];
        if (fz && fz->cluster && std::strcmp(fz->cluster, cluster) == 0) return true;
    }
    return false;
}

DispatchResult dispatch_report(const PreparedDefinition& def,
                               std::uint16_t cluster_id,
                               const char* cluster_name,
                               std::uint8_t src_ep,
                               std::span<const std::uint8_t> bytes) {
    auto raw = build_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, std::span<const TuyaDpRecord>{}, def, raw, ctx);
}

// haElectricalMeasurement (0x0B04) Report Attributes carrying:
//   attr 0x050B ActivePower    (s16) = 0x0096 = 150  -> "power"   150 W
//   attr 0x0505 RMSVoltage     (u16) = 0x00E6 = 230  -> "voltage" 230 V
//   attr 0x0508 RMSCurrent     (u16) = 0x0271 = 625  -> "current" 625 (mA raw)
constexpr std::uint8_t kElecReport[] = {
    0x08, 0x51, 0x0A,
    0x0B, 0x05, 0x29, 0x96, 0x00,   // 0x050B int16 = 150
    0x05, 0x05, 0x21, 0xE6, 0x00,   // 0x0505 u16  = 230
    0x08, 0x05, 0x21, 0x71, 0x02,   // 0x0508 u16  = 625
};

// closuresWindowCovering (0x0102):
//   attr 0x0008 CurrentPositionLiftPercentage (u8) = 60 -> "position"
constexpr std::uint8_t kLiftReport[] = {
    0x08, 0x53, 0x0A,
    0x08, 0x00, 0x20, 0x3C,   // 0x0008 u8 = 60
};
//   attr 0x0009 CurrentPositionTiltPercentage (u8) = 25 -> "tilt"
constexpr std::uint8_t kTiltReport[] = {
    0x08, 0x54, 0x0A,
    0x09, 0x00, 0x20, 0x19,   // 0x0009 u8 = 25
};

}  // namespace

// ─────────────────────────────────────────────────────────────────────
// S-ZB-PDM1-R251 — phase dimmer that dropped the 0x0B04 meter half.
// ─────────────────────────────────────────────────────────────────────

void test_pdm1_exposes_and_bindings() {
    const auto& def = devices::smarli::kDef_S_ZB_PDM1_R251;
    assert(has_expose(def, "state"));
    assert(has_expose(def, "brightness"));
    assert(has_expose(def, "energy"));
    assert(has_expose(def, "power"));
    assert(has_expose(def, "voltage"));   // ← gap fixed (0x0B04)
    assert(has_expose(def, "current"));   // ← gap fixed (0x0B04)
    assert(has_fz_cluster(def, "haElectricalMeasurement"));  // ← gap fixed
    assert(has_fz_cluster(def, "seMetering"));
    assert(has_binding(def, 0x0B04));     // ← gap fixed
    assert(has_binding(def, 0x0702));
    assert(def.endpoint_map_count == 0);  // single-endpoint: no map, bare keys
}

void test_pdm1_decodes_electrical() {
    const auto& def = devices::smarli::kDef_S_ZB_PDM1_R251;
    auto r = dispatch_report(def, 0x0B04, "haElectricalMeasurement", 1,
                             {kElecReport, sizeof(kElecReport)});
    assert(r.any_matched);
    const Value* p = r.merged.find("power");
    assert(p && p->type == ValueType::Int && p->i == 150);
    const Value* v = r.merged.find("voltage");
    assert(v && v->type == ValueType::Uint && v->u == 230);
    const Value* c = r.merged.find("current");
    assert(c && c->type == ValueType::Uint && c->u == 625);
}

// ─────────────────────────────────────────────────────────────────────
// S-ZB-1RE1-R251 — 2ch relay, meter on endpoint 3 (suffixed keys).
// ─────────────────────────────────────────────────────────────────────

void test_1re1_exposes_and_bindings() {
    const auto& def = devices::smarli::kDef_S_ZB_1RE1_R251;
    assert(has_expose(def, "state"));
    assert(has_expose(def, "power"));
    assert(has_expose(def, "voltage"));   // ← gap fixed
    assert(has_expose(def, "current"));   // ← gap fixed
    assert(has_expose(def, "energy"));
    assert(has_fz_cluster(def, "haElectricalMeasurement"));  // ← gap fixed
    assert(has_binding(def, 0x0B04));     // ← gap fixed
    assert(def.endpoint_map_count == 3);
}

void test_1re1_decodes_electrical_suffixed_to_ep3() {
    const auto& def = devices::smarli::kDef_S_ZB_1RE1_R251;
    // Meter cluster lives on endpoint 3 → keys suffixed _3, except `voltage`
    // which is an always-global key (never suffixed).
    auto r = dispatch_report(def, 0x0B04, "haElectricalMeasurement", 3,
                             {kElecReport, sizeof(kElecReport)});
    assert(r.any_matched);
    const Value* p = r.merged.find("power_3");
    assert(p && p->type == ValueType::Int && p->i == 150);
    const Value* c = r.merged.find("current_3");
    assert(c && c->type == ValueType::Uint && c->u == 625);
    const Value* v = r.merged.find("voltage");   // global key, not suffixed
    assert(v && v->type == ValueType::Uint && v->u == 230);
    assert(r.merged.find("power") == nullptr);   // bare key was suffixed away
}

// ─────────────────────────────────────────────────────────────────────
// S-ZB-COV1-R251 — curtain that dropped tilt + the 0x0B04 meter half.
// ─────────────────────────────────────────────────────────────────────

void test_cov1_exposes_and_bindings() {
    const auto& def = devices::smarli::kDef_S_ZB_COV1_R251;
    assert(has_expose(def, "position"));
    assert(has_expose(def, "tilt"));      // ← gap fixed
    assert(has_expose(def, "power"));
    assert(has_expose(def, "voltage"));   // ← gap fixed
    assert(has_expose(def, "current"));   // ← gap fixed
    assert(has_expose(def, "energy"));
    assert(has_fz_cluster(def, "haElectricalMeasurement"));  // ← gap fixed
    assert(has_binding(def, 0x0B04));     // ← gap fixed
    // tilt TZ present (kTzCoverPositionTilt) — accepts "tilt".
    bool tz_tilt = false;
    for (std::uint8_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* tz = def.to_zigbee[i];
        if (tz && tz->key && std::strcmp(tz->key, "tilt") == 0) tz_tilt = true;
    }
    assert(tz_tilt);                      // ← gap fixed (tilt set path)
    assert(def.endpoint_map_count == 1);  // only ep3 mapped (meter); cover bare
}

void test_cov1_decodes_lift_and_tilt() {
    const auto& def = devices::smarli::kDef_S_ZB_COV1_R251;
    // Cover lives on endpoint 1 (not in the ep-map) → bare keys.
    {
        auto r = dispatch_report(def, 0x0102, "closuresWindowCovering", 1,
                                 {kLiftReport, sizeof(kLiftReport)});
        assert(r.any_matched);
        const Value* pos = r.merged.find("position");
        assert(pos && pos->type == ValueType::Uint && pos->u == 60);
    }
    {
        auto r = dispatch_report(def, 0x0102, "closuresWindowCovering", 1,
                                 {kTiltReport, sizeof(kTiltReport)});
        assert(r.any_matched);
        const Value* tilt = r.merged.find("tilt");   // ← gap fixed
        assert(tilt && tilt->type == ValueType::Uint && tilt->u == 25);
    }
}

void test_cov1_decodes_electrical_suffixed_to_ep3() {
    const auto& def = devices::smarli::kDef_S_ZB_COV1_R251;
    auto r = dispatch_report(def, 0x0B04, "haElectricalMeasurement", 3,
                             {kElecReport, sizeof(kElecReport)});
    assert(r.any_matched);
    const Value* p = r.merged.find("power_3");
    assert(p && p->type == ValueType::Int && p->i == 150);
    const Value* c = r.merged.find("current_3");
    assert(c && c->type == ValueType::Uint && c->u == 625);
}

int main() {
    test_pdm1_exposes_and_bindings();
    test_pdm1_decodes_electrical();
    test_1re1_exposes_and_bindings();
    test_1re1_decodes_electrical_suffixed_to_ep3();
    test_cov1_exposes_and_bindings();
    test_cov1_decodes_lift_and_tilt();
    test_cov1_decodes_electrical_suffixed_to_ep3();
    return 0;
}
