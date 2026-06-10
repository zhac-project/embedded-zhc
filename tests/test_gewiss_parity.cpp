// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Gewiss (Italian wired switch / shutter / contact)
// family. The auto-generated ports carried three real gaps pinned here
// after the fix:
//
//   electricityMeter cluster:"both" — 0x0B04 electrical half dropped —
//     * GWA1201_TWO_WAY_SWITCH: z2m extend = [m.onOff(), m.electricityMeter(),
//       m.identify()]. m.electricityMeter() (no args) defaults to
//       cluster:"both" + ac, decoding seMetering (0x0702: energy) AND
//       haElectricalMeasurement (0x0B04: power/voltage/current). The port
//       wired only kFzMetering (0x0702), dropping voltage/current. Added
//       kFzElectricalMeasurement + the voltage/current exposes + 0x0B04 bind.
//
//   genBinaryInput contact channels dropped —
//     * GWA1502 (230V, mains): z2m exposes TWO `input` binaries via
//       genBinaryInput.presentValue on ep1/ep2 — NO battery, NO on/off. The
//       port mis-wired a phantom kFzBattery + phantom kFzOnOff (dead
//       state/battery/voltage). Rewired to the new generic kFzBinaryInput on
//       both endpoints (dispatch suffixes input -> input_1 / input_2).
//     * GWA1501: battery was kept (correct) but both `input` channels were
//       dropped. Added kFzBinaryInput on ep1/ep2 alongside the battery.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::gewiss {
extern const PreparedDefinition kDef_GWA1201_TWO_WAY_SWITCH;  // switch + metering + electrical
extern const PreparedDefinition kDef_GWA1501;  // battery contact, 2 inputs
extern const PreparedDefinition kDef_GWA1502;  // mains contact, 2 inputs
}  // namespace zhc::devices::gewiss

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

bool def_uses_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i)
        if (def.from_zigbee[i] == conv) return true;
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

// ── GWA1201: metering + electrical halves both live. ───────────────────────
void test_gwa1201_electrical() {
    using namespace zhc::devices::gewiss;
    const auto& def = kDef_GWA1201_TWO_WAY_SWITCH;

    // Both halves exposed: metering energy (kept) + electrical power/V/I (added).
    assert(def_exposes(def, "state"));     // controllable relay kept
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));   // electrical half added
    assert(def_exposes(def, "current"));   // electrical half added
    assert(def.to_zigbee_count == 1);      // tz.on_off kept
    assert(def_binds(def, 1, 0x0006));
    assert(def_binds(def, 1, 0x0702));
    assert(def_binds(def, 1, 0x0B04));     // electrical-measurement bind added

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

    // seMetering currentSummDelivered (0x0000, u48) -> "energy" still works.
    RuntimeContext c3{};
    const std::array<std::uint8_t, 6> e = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00};   // 16 LE
    auto er = dispatch_zcl_ep(c3, def, 0x0702, 1, "seMetering",
                              attr_report(0x0000, 0x25, std::span<const std::uint8_t>(e.data(), e.size())));
    assert(er.any_matched);
    assert(er.merged.find("energy") != nullptr);
}

// ── GWA1502: mains contact -> two genBinaryInput channels, no battery/onoff. ─
void test_gwa1502_binary_inputs() {
    using namespace zhc::devices::gewiss;
    const auto& def = kDef_GWA1502;

    // Phantom battery + phantom on/off removed; pure 2-channel input device.
    assert(def_exposes(def, "input"));
    assert(!def_exposes(def, "state"));     // no on/off (was phantom)
    assert(!def_exposes(def, "battery"));   // mains 230V, no battery (was phantom)
    assert(def.to_zigbee_count == 0);       // no write path
    assert(!def_binds(def, 1, 0x0006));     // no genOnOff bind
    assert(!def_binds(def, 1, 0x0001));     // no power-config bind
    assert(def_binds(def, 1, 0x000F));      // genBinaryInput ep1
    assert(def_binds(def, 2, 0x000F));      // genBinaryInput ep2
    assert(def.endpoint_map && def.endpoint_map_count == 2);

    // presentValue (0x0055, bool) on ep1 -> input_1 (suffixed via endpoint_map).
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto r1 = dispatch_zcl_ep(c1, def, 0x000F, 1, "genBinaryInput",
                              attr_report(0x0055, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(r1.any_matched);
    assert(r1.merged.find("input_1") != nullptr);
    assert(r1.merged.find("input") == nullptr);   // bare key suffixed away

    // presentValue on ep2 -> input_2.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> off = {0x00};
    auto r2 = dispatch_zcl_ep(c2, def, 0x000F, 2, "genBinaryInput",
                              attr_report(0x0055, 0x10, std::span<const std::uint8_t>(off.data(), off.size())));
    assert(r2.any_matched);
    assert(r2.merged.find("input_2") != nullptr);
}

// ── GWA1501: battery contact -> battery kept + two input channels added. ────
void test_gwa1501_battery_plus_inputs() {
    using namespace zhc::devices::gewiss;
    const auto& def = kDef_GWA1501;

    assert(def_exposes(def, "battery"));    // battery kept
    assert(def_exposes(def, "input"));      // input channels added
    assert(def_binds(def, 1, 0x0001));      // power-config (battery) bind kept
    assert(def_binds(def, 1, 0x000F));      // genBinaryInput ep1
    assert(def_binds(def, 2, 0x000F));      // genBinaryInput ep2
    assert(def.endpoint_map && def.endpoint_map_count == 2);

    // presentValue on ep2 -> input_2.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto r = dispatch_zcl_ep(c1, def, 0x000F, 2, "genBinaryInput",
                             attr_report(0x0055, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(r.any_matched);
    assert(r.merged.find("input_2") != nullptr);

    // Battery voltage (powerConfiguration 0x0001 attr 0x0021 batteryPercentage)
    // still decodes via the kept kFzBattery.
    assert(def_uses_fz(def, &::zhc::generic::kFzBattery));
}

}  // namespace

int main() {
    test_gwa1201_electrical();
    test_gwa1502_binary_inputs();
    test_gwa1501_battery_plus_inputs();
    return 0;
}
