// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Dawon DNS (Korean smart-plug / in-wall-switch)
// family.
//
// The vendor is overwhelmingly clean against z2m: the PM-S*/KB-* switches
// are m.onOff() (single + multi-gang with deviceEndpoints), the PM-B/PM-C
// metering plugs are fz.on_off + fz.metering (seMetering 0x0702 only — z2m
// does NOT layer haElectricalMeasurement here, so there is no current/voltage
// channel to drop), TH-110 is fz.temperature + fz.humidity + fz.battery, and
// KB-HD100 is an ssIasZone card-holder. All of those map onto generic
// converters and were verified CLEAN.
//
// The one real z2m<->ZHC gap was PM-B540-ZB. Alone among the metering plugs
// it also reports its internal die temperature on genDeviceTempCfg (0x0002):
// z2m wires fz.device_temperature + e.device_temperature() and binds the
// cluster, but the auto-generated port carried only kFzOnOff + kFzMetering
// and dropped device_temperature entirely. There was no generic decoder for
// cluster 0x0002 at all (an explicit TODO(parity) sat in
// custom_devices_diy/Cus_CC2538_ROUTER_V2). Fixed by:
//   * adding 0x0002 -> "genDeviceTempCfg" to cluster_names (so the decoder
//     labels the frame),
//   * adding the generic kFzDeviceTemperature converter (0x0002 attr 0x0000
//     currentTemperature, s16 whole °C, no scaling — mirrors
//     fromZigbee.ts fz.device_temperature which returns the raw value),
//   * graduating Daw_PM_B540_ZB.cpp to a Tier-2 parent override that wires
//     the converter + the device_temperature expose + the 0x0002 bind.
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * a genDeviceTempCfg (0x0002) report yields device_temperature (Int °C),
//   * a seMetering (0x0702) report still yields energy + power (regression),
//   * PM-B540 declares state/power/energy/device_temperature exposes and
//     binds genOnOff/seMetering/genDeviceTempCfg,
//   * the rest of the family keeps its expected shape (TH-110 temp/humidity,
//     multi-gang endpoint_maps).
//
// z2m-source: zigbee-herdsman-converters/src/devices/dawon_dns.ts
//             #PM-B540-ZB + converters/fromZigbee.ts device_temperature.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::dawon_dns {
extern const PreparedDefinition kDef_PM_B540_ZB;
extern const PreparedDefinition kDef_PM_B530_ZB;
extern const PreparedDefinition kDef_TH_110_ZB;
extern const PreparedDefinition kDef_PM_S240_ZB;
extern const PreparedDefinition kDef_PM_S340_ZB;
}  // namespace zhc::devices::dawon_dns

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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

bool def_has_endpoint(const PreparedDefinition& def, const char* label, std::uint8_t ep) {
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i)
        if (def.endpoint_map[i].label && std::strcmp(def.endpoint_map[i].label, label) == 0 &&
            def.endpoint_map[i].endpoint == ep)
            return true;
    return false;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A=ReportAttributes,
// then one record: <attr_id LE><type><value LE>.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// ── PM-B540: the real fix. device_temperature + metering regression. ──
void check_pm_b540() {
    using namespace zhc::devices::dawon_dns;
    const PreparedDefinition& def = kDef_PM_B540_ZB;

    // Static shape: device_temperature added without dropping metering.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "device_temperature"));
    assert(def_binds(def, 0x0006));   // genOnOff
    assert(def_binds(def, 0x0702));   // seMetering
    assert(def_binds(def, 0x0002));   // genDeviceTempCfg

    // genDeviceTempCfg (0x0002) attr 0x0000 currentTemperature, int16s.
    // 0x001A = 26 °C, returned raw (no /100 scaling).
    const std::uint8_t temp_b[] = {0x1A, 0x00};
    auto rt = dispatch_zcl(def, 0x0002, "genDeviceTempCfg", 1,
                           attr_report(0x0000, 0x29, temp_b));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("device_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 26);
    // Must NOT masquerade as the 0.01 °C measurement channel.
    assert(rt.merged.find("temperature") == nullptr);

    // Negative die temperature round-trips as signed (int16s = -5 °C).
    const std::uint8_t neg_b[] = {0xFB, 0xFF};
    auto rn = dispatch_zcl(def, 0x0002, "genDeviceTempCfg", 1,
                           attr_report(0x0000, 0x29, neg_b));
    assert(rn.any_matched);
    const Value* nv = rn.merged.find("device_temperature");
    assert(nv && nv->type == ValueType::Int && nv->i == -5);

    // Regression: seMetering (0x0702) still yields energy + power.
    // attr 0x0000 CurrentSummationDelivered, uint48 = 1234.
    const std::uint8_t energy_b[] = {0xD2, 0x04, 0x00, 0x00, 0x00, 0x00};
    auto re = dispatch_zcl(def, 0x0702, "seMetering", 1,
                           attr_report(0x0000, 0x25, energy_b));
    assert(re.any_matched);
    assert(re.merged.find("energy") != nullptr);

    // attr 0x0400 InstantaneousDemand, int24s = 150 W.
    const std::uint8_t demand_b[] = {0x96, 0x00, 0x00};
    auto rd = dispatch_zcl(def, 0x0702, "seMetering", 1,
                           attr_report(0x0400, 0x2A, demand_b));
    assert(rd.any_matched);
    assert(rd.merged.find("power") != nullptr);

    // onOff (0x0006) attr 0x0000 bool → state still decodes.
    const std::uint8_t on_b[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                           attr_report(0x0000, 0x10, on_b));
    assert(ro.any_matched);
    assert(ro.merged.find("state") != nullptr);
}

// ── Regression: a sibling metering plug WITHOUT device_temperature must
//    not have grown the channel, and must not decode 0x0002. ──
void check_pm_b530_unaffected() {
    using namespace zhc::devices::dawon_dns;
    const PreparedDefinition& def = kDef_PM_B530_ZB;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(!def_exposes(def, "device_temperature"));
    assert(!def_binds(def, 0x0002));

    const std::uint8_t temp_b[] = {0x1A, 0x00};
    auto rt = dispatch_zcl(def, 0x0002, "genDeviceTempCfg", 1,
                           attr_report(0x0000, 0x29, temp_b));
    assert(rt.merged.find("device_temperature") == nullptr);
}

// ── TH-110 temp/humidity sensor stays whole. ──
void check_th110() {
    using namespace zhc::devices::dawon_dns;
    const PreparedDefinition& def = kDef_TH_110_ZB;
    assert(def_exposes(def, "temperature"));
    assert(def_exposes(def, "humidity"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0402));   // msTemperatureMeasurement
    assert(def_binds(def, 0x0405));   // msRelativeHumidity

    // msTemperatureMeasurement 0x0000 measuredValue, int16s = 2350 → 23.5 °C.
    const std::uint8_t t_b[] = {0x2E, 0x09};
    auto rt = dispatch_zcl(def, 0x0402, "msTemperatureMeasurement", 1,
                           attr_report(0x0000, 0x29, t_b));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);

    // msRelativeHumidity 0x0000 measuredValue, uint16 = 5000 → 50 %.
    const std::uint8_t h_b[] = {0x88, 0x13};
    auto rh = dispatch_zcl(def, 0x0405, "msRelativeHumidity", 1,
                           attr_report(0x0000, 0x21, h_b));
    assert(rh.any_matched);
    assert(rh.merged.find("humidity") != nullptr);
}

// ── Multi-gang endpoint_maps unchanged. ──
void check_multigang() {
    using namespace zhc::devices::dawon_dns;
    assert(def_has_endpoint(kDef_PM_S240_ZB, "top", 1));
    assert(def_has_endpoint(kDef_PM_S240_ZB, "bottom", 2));
    assert(def_has_endpoint(kDef_PM_S340_ZB, "top", 1));
    assert(def_has_endpoint(kDef_PM_S340_ZB, "center", 2));
    assert(def_has_endpoint(kDef_PM_S340_ZB, "bottom", 3));
}

}  // namespace

int main() {
    check_pm_b540();
    check_pm_b530_unaffected();
    check_th110();
    check_multigang();
    return 0;
}
