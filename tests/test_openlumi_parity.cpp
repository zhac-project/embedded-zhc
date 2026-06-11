// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for OpenLumi (Xiaomi-clone) — single def GWRJN5169.
//
// z2m defines GWRJN5169 ("Lumi Router (JN5169)") as a mains-powered router
// with a single modernExtend: `m.deviceTemperature()` — i.e. genDeviceTempCfg
// (0x0002) currentTemperature, exposed as device_temperature. It has NO
// on/off and NO battery.
//
// The Tier-1 auto-port mis-modelled it: it invented a phantom on/off
// (kFzOnOff + a `state` Binary expose + a 0x0006 bind) and a phantom battery
// (kFzBattery + battery/voltage exposes + a 0x0001 bind), while dropping the
// ONE real channel — device_temperature — entirely. Same gap class as
// onesti S4RX-110 / dawon_dns PM-B540-ZB (device_temperature dropped) plus a
// phantom default-bundle (gap class e).
//
// The Tier-2 override (definitions/openlumi/Ope_GWRJN5169.cpp) wires the
// generic kFzDeviceTemperature (0x0002 attr 0x0000 currentTemperature, raw
// whole °C, signed — mirrors fromZigbee.ts fz.device_temperature), exposes
// only device_temperature, and binds genDeviceTempCfg (0x0002).
//
// These tests pin, at the dispatch_from_zigbee boundary:
//   * a genDeviceTempCfg (0x0002) report yields device_temperature (Int °C,
//     signed, no scaling),
//   * no phantom state / battery exposes survive,
//   * the only binding is genDeviceTempCfg (0x0002).
//
// z2m-source: zigbee-herdsman-converters/src/devices/openlumi.ts #GWRJN5169
//             + converters/fromZigbee.ts device_temperature.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::openlumi {
extern const PreparedDefinition kDef_GWRJN5169;
}  // namespace zhc::devices::openlumi

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

bool def_binds_ep(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
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

void check_gwrjn5169() {
    using namespace zhc::devices::openlumi;
    const PreparedDefinition& def = kDef_GWRJN5169;

    // Static shape: the SOLE real channel is device_temperature.
    assert(def_exposes(def, "device_temperature"));
    assert(def.exposes_count == 1);

    // No phantom on/off or battery channels survive.
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_exposes(def, "voltage"));

    // The only binding is genDeviceTempCfg (0x0002). No phantom 0x0001/0x0006.
    assert(def.bindings_count == 1);
    assert(def_binds_ep(def, 1, 0x0002));
    assert(!def_binds_ep(def, 1, 0x0001));   // no genPowerCfg (phantom battery)
    assert(!def_binds_ep(def, 1, 0x0006));   // no genOnOff (phantom on/off)

    // genDeviceTempCfg (0x0002) attr 0x0000 currentTemperature, int16s.
    // 0x0021 = 33 °C, returned raw (no scaling).
    const std::uint8_t temp_b[] = {0x21, 0x00};
    auto rt = dispatch_zcl(def, 0x0002, "genDeviceTempCfg", 1,
                           attr_report(0x0000, 0x29, temp_b));
    assert(rt.any_matched);
    const Value* tv = rt.merged.find("device_temperature");
    assert(tv && tv->type == ValueType::Int && tv->i == 33);
    // Must NOT masquerade as the 0.01 °C measurement channel.
    assert(rt.merged.find("temperature") == nullptr);

    // Negative die temperature round-trips as signed (int16s = -4 °C).
    const std::uint8_t neg_b[] = {0xFC, 0xFF};
    auto rn = dispatch_zcl(def, 0x0002, "genDeviceTempCfg", 1,
                           attr_report(0x0000, 0x29, neg_b));
    assert(rn.any_matched);
    const Value* nv = rn.merged.find("device_temperature");
    assert(nv && nv->type == ValueType::Int && nv->i == -4);

    // Phantom regression: a genOnOff report no longer decodes a `state`.
    const std::uint8_t on_b[] = {0x01};
    auto ro = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                           attr_report(0x0000, 0x10, on_b));
    assert(ro.merged.find("state") == nullptr);
}

}  // namespace

int main() {
    check_gwrjn5169();
    return 0;
}
