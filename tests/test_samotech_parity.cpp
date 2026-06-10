// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Samotech relay / dimmer + metering family. Three
// metering variants carried one real gap pinned here after the fix:
//
//   electricityMeter cluster:"both" — 0x0B04 electrical half dropped —
//     z2m wires `m.electricityMeter()` with NO args, which defaults to
//     cluster:"both" + electricalMeasurementType:"ac", decoding seMetering
//     (0x0702: energy) AND haElectricalMeasurement (0x0B04: power/voltage/
//     current) via fz=[fz.electrical_measurement, fz.metering]. The
//     auto-ports wired only kFzMetering (0x0702), exposing just
//     state/energy/power and dropping voltage + current. Added
//     kFzElectricalMeasurement + the voltage/current exposes + the 0x0B04
//     binding to:
//       * SM309-S    (dimmer 400W with metering, single endpoint)
//       * SM323_v2   (retrofit dimmer 250W, SM323 / HK_DIM_A)
//       * SM308-2CH  (2-channel relay, per-channel state_l1/state_l2,
//                     whole-device metering reporting on EP1)
//
// The SM308-2CH endpoint_map {l1:1, l2:2} is asserted PRESENT (the suspected
// missing-endpoint_map is a false flag — relay state is genuinely per-channel,
// z2m m.onOff({endpointNames:["l1","l2"]})). Because the metering frame is
// dispatched against EP1 (which is in the endpoint_map), the runtime suffixes
// the non-global metering keys -> energy_l1 / power_l1 / current_l1, while
// "voltage" stays bare (it lives in dispatch's kAlwaysGlobalKeys). Exposes
// stay bare per project convention.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::samotech {
extern const PreparedDefinition kDef_SM309_S;     // dimmer + metering (1 EP)
extern const PreparedDefinition kDef_SM323_v2;    // retrofit dimmer + metering
extern const PreparedDefinition kDef_SM308_2CH;   // 2-channel relay + metering
}  // namespace zhc::devices::samotech

using namespace zhc;

namespace {

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

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// Keep `ctx` alive while the result is read: multi-endpoint suffixing allocates
// in RuntimeContext scratch, so result pointers alias `ctx`.
DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report (fc=0x18 S->C, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// Shared electrical-half assertions for a single-endpoint metering def:
// exposes both halves; binds 0x0702 + 0x0B04; decodes voltage/current from
// 0x0B04 and energy from 0x0702.
void assert_metering_both_single_ep(const PreparedDefinition& def) {
    // Both halves exposed (bare keys per project convention).
    assert(def_exposes(def, "energy"));    // metering half (kept)
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));   // electrical half (added)
    assert(def_exposes(def, "current"));   // electrical half (added)
    assert(def_exposes(def, "state"));     // controllable dimmer kept
    assert(def_exposes(def, "brightness"));

    // Bindings: both clusters present.
    assert(def_binds(def, 1, 0x0702));     // seMetering
    assert(def_binds(def, 1, 0x0B04));     // haElectricalMeasurement (added)

    // Single endpoint -> no endpoint_map, keys stay bare.
    assert(def.endpoint_map_count == 0);

    // rmsVoltage (0x0505, u16) on haElectricalMeasurement -> "voltage".
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> v = {0xE6, 0x00};   // 230 LE
    auto vr = dispatch_zcl_ep(c1, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0505, 0x21, std::span<const std::uint8_t>(v.data(), v.size())));
    assert(vr.any_matched);
    assert(vr.merged.find("voltage") != nullptr);

    // rmsCurrent (0x0508, u16) -> "current".
    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> i = {0xF4, 0x01};   // 500 LE
    auto ir = dispatch_zcl_ep(c2, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0508, 0x21, std::span<const std::uint8_t>(i.data(), i.size())));
    assert(ir.any_matched);
    assert(ir.merged.find("current") != nullptr);

    // activePower (0x050B, s16) -> "power" (from the electrical half).
    RuntimeContext c3{};
    const std::array<std::uint8_t, 2> p = {0x64, 0x00};   // 100 LE
    auto pr = dispatch_zcl_ep(c3, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x050B, 0x29, std::span<const std::uint8_t>(p.data(), p.size())));
    assert(pr.any_matched);
    assert(pr.merged.find("power") != nullptr);

    // seMetering currentSummDelivered (0x0000, u48) -> "energy" still works.
    RuntimeContext c4{};
    const std::array<std::uint8_t, 6> e = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00};   // 16 LE
    auto er = dispatch_zcl_ep(c4, def, 0x0702, 1, "seMetering",
                              attr_report(0x0000, 0x25, std::span<const std::uint8_t>(e.data(), e.size())));
    assert(er.any_matched);
    assert(er.merged.find("energy") != nullptr);
}

// ── SM309-S: dimmer 400W with metering, single endpoint. ────────────────────
void test_sm309_s_metering() {
    assert_metering_both_single_ep(zhc::devices::samotech::kDef_SM309_S);
}

// ── SM323_v2: retrofit dimmer 250W (SM323 / HK_DIM_A), single endpoint. ──────
void test_sm323_v2_metering() {
    using namespace zhc::devices::samotech;
    const auto& def = kDef_SM323_v2;
    // Carries both zigbee model + the HK_DIM_A fingerprint variant.
    bool has_sm323 = false, has_hk = false;
    for (std::size_t i = 0; i < def.zigbee_models_count; ++i) {
        if (std::strcmp(def.zigbee_models[i], "SM323") == 0) has_sm323 = true;
        if (std::strcmp(def.zigbee_models[i], "HK_DIM_A") == 0) has_hk = true;
    }
    assert(has_sm323 && has_hk);
    assert_metering_both_single_ep(def);
}

// ── SM308-2CH: per-channel relay + whole-device metering on EP1. ────────────
void test_sm308_2ch() {
    using namespace zhc::devices::samotech;
    const auto& def = kDef_SM308_2CH;

    // Suspected missing endpoint_map is FALSE — it is present (l1:1, l2:2),
    // because relay on/off state is genuinely per-channel.
    assert(def.endpoint_map && def.endpoint_map_count == 2);

    // Electrical half added (exposes stay bare).
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));   // added
    assert(def_exposes(def, "current"));   // added
    assert(def_exposes(def, "state"));

    // genOnOff bound on BOTH relay channels; metering + electrical on EP1.
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 2, 0x0006));     // 2nd channel relay bind (added)
    assert(def_binds(def, 1, 0x0702));
    assert(def_binds(def, 1, 0x0B04));     // electrical-measurement bind (added)

    // Per-channel relay state: genOnOff On (cmd 0x01) on EP2 -> state_l2.
    RuntimeContext c0{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto sr = dispatch_zcl_ep(c0, def, 0x0006, 2, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(sr.any_matched);
    assert(sr.merged.find("state_l2") != nullptr);   // suffixed per endpoint_map
    assert(sr.merged.find("state") == nullptr);

    // Electrical half: rmsCurrent (0x0508) on EP1 -> current_l1 (EP1 is in the
    // map, so a non-global key is suffixed); "voltage" stays bare (global key).
    RuntimeContext c1{};
    const std::array<std::uint8_t, 2> i = {0xF4, 0x01};   // 500 LE
    auto ir = dispatch_zcl_ep(c1, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0508, 0x21, std::span<const std::uint8_t>(i.data(), i.size())));
    assert(ir.any_matched);
    assert(ir.merged.find("current_l1") != nullptr);    // current is suffixed

    RuntimeContext c2{};
    const std::array<std::uint8_t, 2> v = {0xE6, 0x00};   // 230 LE
    auto vr = dispatch_zcl_ep(c2, def, 0x0B04, 1, "haElectricalMeasurement",
                              attr_report(0x0505, 0x21, std::span<const std::uint8_t>(v.data(), v.size())));
    assert(vr.any_matched);
    assert(vr.merged.find("voltage") != nullptr);        // voltage stays bare

    // seMetering energy on EP1 -> energy_l1 (suffixed; reported whole-device).
    RuntimeContext c3{};
    const std::array<std::uint8_t, 6> e = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto er = dispatch_zcl_ep(c3, def, 0x0702, 1, "seMetering",
                              attr_report(0x0000, 0x25, std::span<const std::uint8_t>(e.data(), e.size())));
    assert(er.any_matched);
    assert(er.merged.find("energy_l1") != nullptr);
}

}  // namespace

int main() {
    test_sm309_s_metering();
    test_sm323_v2_metering();
    test_sm308_2ch();
    return 0;
}
