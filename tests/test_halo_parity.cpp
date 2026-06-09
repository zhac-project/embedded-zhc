// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Halo Smart Labs HALO / HALO+ smoke + CO detectors.
// z2m-source: halo_smart_labs.ts — `haloCommonExtend` / `haloZoneStatus`.
//
// Bug fixed: both generated defs (kDef_HALO, kDef_HALO_) were
// battery+on/off stubs. They dropped the entire safety-sensor channel
// set that z2m surfaces. The Halo IAS demux converter (kFzHaloIasZone)
// already existed in definitions/halo_smart_labs/_shared.cpp but was
// never referenced by either def, and the standard temperature /
// humidity / pressure clusters were not decoded at all.
//
// These tests pin, on real ssIasZone attribute-report wire shapes, the
// endpoint-aware demux:
//   * EP1 zoneStatus -> smoke (bit0) / tamper (bit2) / battery_low
//     (bit3) / test (bit8) / mains_power_connected (!bit7)
//   * EP3 zoneStatus -> carbon_monoxide (bit0)
// and that temperature / humidity / pressure decode from their standard
// clusters. The light channel (EP2) is verified structurally (exposes +
// endpoint_map + bindings) so its keys suffix `_light` at runtime.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::halo_smart_labs {
extern const PreparedDefinition kDef_HALO;
extern const PreparedDefinition kDef_HALO_;
}  // namespace zhc::devices::halo_smart_labs

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

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

// ssIasZone (0x0500) Report Attributes for ZoneStatus (attr 0x0002, u16):
//   fc(1)=0x18  tsn(1)  cmd(1)=0x0A  attr_le(2)=0x0002  type(1)=0x21  value_le(2)
struct ZsFrame { std::array<std::uint8_t, 8> bytes; };

ZsFrame zs_report(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    ZsFrame f{};
    f.bytes = { 0x18, tsn, 0x0A,
                0x02, 0x00,              // attr 0x0002 (ZoneStatus)
                0x21,                    // type uint16
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8) };
    return f;
}

DispatchResult dispatch_zs(const PreparedDefinition& def, std::uint8_t src_ep,
                           const ZsFrame& f) {
    auto raw = make_frame(0x0500, src_ep,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// EP1 demux: smoke (bit0) + the diagnostic bits.
void check_ep1(const PreparedDefinition& def) {
    assert(def_exposes(def, "smoke"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "test"));
    assert(def_exposes(def, "mains_power_connected"));

    // smoke only (bit0). ac_fault(bit7) clear -> mains_power_connected true.
    auto smoke = dispatch_zs(def, 1, zs_report(0x0001));
    assert(smoke.any_matched);
    assert(b_true(smoke.merged.find("smoke")));
    assert(b_false(smoke.merged.find("tamper")));
    assert(b_false(smoke.merged.find("battery_low")));
    assert(b_false(smoke.merged.find("test")));
    assert(b_true(smoke.merged.find("mains_power_connected")));
    // An EP1 frame must NOT emit carbon_monoxide (that is EP3 only).
    assert(smoke.merged.find("carbon_monoxide") == nullptr);
    // EP1 keys are bare (the light is the only suffixed endpoint).
    assert(smoke.merged.find("smoke_light") == nullptr);

    // clear everything: smoke false, but ac_fault(bit7) now also clear ->
    // mains true; flip bit7 to assert AC fault -> mains false.
    auto clear = dispatch_zs(def, 1, zs_report(0x0000));
    assert(b_false(clear.merged.find("smoke")));
    assert(b_true(clear.merged.find("mains_power_connected")));

    auto acfault = dispatch_zs(def, 1, zs_report(0x0080));  // bit7
    assert(b_false(acfault.merged.find("mains_power_connected")));

    // tamper(bit2) + battery_low(bit3) + test(bit8).
    auto diag = dispatch_zs(def, 1, zs_report(0x010C));
    assert(b_true(diag.merged.find("tamper")));
    assert(b_true(diag.merged.find("battery_low")));
    assert(b_true(diag.merged.find("test")));
    assert(b_false(diag.merged.find("smoke")));
}

// EP3 demux: carbon_monoxide (bit0) only.
void check_ep3(const PreparedDefinition& def) {
    assert(def_exposes(def, "carbon_monoxide"));

    auto co_on = dispatch_zs(def, 3, zs_report(0x0001));
    assert(co_on.any_matched);
    assert(b_true(co_on.merged.find("carbon_monoxide")));
    // EP3 must not emit the EP1 channels.
    assert(co_on.merged.find("smoke") == nullptr);
    assert(co_on.merged.find("tamper") == nullptr);

    auto co_off = dispatch_zs(def, 3, zs_report(0x0000));
    assert(b_false(co_off.merged.find("carbon_monoxide")));
}

// temperature (0x0402) + humidity (0x0405) decode from standard clusters.
void check_env(const PreparedDefinition& def) {
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "pressure"));

    // msTemperatureMeasurement Report Attributes: attr 0x0000 (int16) = 2150 (21.50 C).
    const std::uint8_t temp_report[] = {
        0x18, 0x42, 0x0A, 0x00, 0x00, 0x29, 0x66, 0x08,
    };
    auto traw = make_frame(0x0402, 1, temp_report);
    DecodedMessage tmsg{};
    assert(decode_frame(traw, {}, tmsg));
    tmsg.cluster = "msTemperatureMeasurement";
    RuntimeContext tctx{};
    auto tr = dispatch_from_zigbee(tmsg, {}, def, traw, tctx);
    assert(tr.any_matched);
    assert(tr.merged.find("temperature") != nullptr);

    // msRelativeHumidity Report Attributes: attr 0x0000 (uint16) = 4500 (45.00 %).
    const std::uint8_t hum_report[] = {
        0x18, 0x42, 0x0A, 0x00, 0x00, 0x21, 0x94, 0x11,
    };
    auto hraw = make_frame(0x0405, 1, hum_report);
    DecodedMessage hmsg{};
    assert(decode_frame(hraw, {}, hmsg));
    hmsg.cluster = "msRelativeHumidity";
    RuntimeContext hctx{};
    auto hr = dispatch_from_zigbee(hmsg, {}, def, hraw, hctx);
    assert(hr.any_matched);
    assert(hr.merged.find("humidity") != nullptr);
}

// EP2 colour light is structurally present: state/brightness/hue/saturation
// exposed, endpoint_map -> {"light",2}, and the light clusters bound on EP2.
void check_light(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(def_exposes(def, "hue"));
    assert(def_exposes(def, "saturation"));

    assert(def.endpoint_map != nullptr && def.endpoint_map_count == 1);
    assert(def.endpoint_map[0].endpoint == 2);
    assert(std::strcmp(def.endpoint_map[0].label, "light") == 0);
    assert(def.default_endpoint == 2);

    assert(def_binds(def, 2, 0x0006));  // genOnOff
    assert(def_binds(def, 2, 0x0008));  // genLevelCtrl
    assert(def_binds(def, 2, 0x0300));  // lightingColorCtrl
    assert(def_binds(def, 1, 0x0500));  // IAS smoke
    assert(def_binds(def, 3, 0x0500));  // IAS carbon_monoxide
    assert(def_binds(def, 1, 0x0402));  // temperature
    assert(def_binds(def, 1, 0x0405));  // humidity
    assert(def_binds(def, 1, 0x0403));  // pressure
}

void check_def(const PreparedDefinition& def) {
    check_ep1(def);
    check_ep3(def);
    check_env(def);
    check_light(def);
}

}  // namespace

int main() {
    check_def(devices::halo_smart_labs::kDef_HALO);
    check_def(devices::halo_smart_labs::kDef_HALO_);
    return 0;
}
