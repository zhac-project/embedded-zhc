// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Silicon Labs. The real z2m<->ez gap caught here:
//
//   * RB-ElectricityDsp-061-3 "Electricity meter" is wired in z2m as
//     m.onOff() + m.electricityMeter(). electricityMeter() defaults to
//     cluster:"both", so z2m decodes energy from seMetering (0x0702,
//     fz.metering) AND power/voltage/current from haElectricalMeasurement
//     (0x0B04, fz.electrical_measurement). The auto-generated port wired
//     only kFzOnOff + kFzMetering and exposed only state/energy/power — it
//     dropped the 0x0B04 half entirely: no kFzElectricalMeasurement, no
//     voltage/current exposes, no 0x0B04 binding. RMSVoltage/RMSCurrent
//     reports were never decoded. Fixed by adding kFzElectricalMeasurement,
//     the voltage+current exposes, and the haElectricalMeasurement binding.
//
// z2m-source: zigbee-herdsman-converters/src/devices/silicon_labs.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::silicon_labs {
extern const PreparedDefinition kDef_RB_ElectricityDsp_061_3;
}  // namespace zhc::devices::silicon_labs

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

constexpr std::uint16_t GEN_ON_OFF  = 0x0006;
constexpr std::uint16_t SE_METERING = 0x0702;
constexpr std::uint16_t HA_ELEC     = 0x0B04;

// genOnOff AttributeReport: attr 0x0000 onOff type 0x10 bool = 1.
constexpr std::uint8_t kOnOffFrame[] = {
    0x18, 0x10, 0x0A,
    0x00, 0x00, 0x10, 0x01,
};

// seMetering AttributeReport: attr 0x0000 currentSummDelivered u48 = 12345.
constexpr std::uint8_t kEnergyFrame[] = {
    0x18, 0x11, 0x0A,
    0x00, 0x00, 0x25, 0x39, 0x30, 0x00, 0x00, 0x00, 0x00,  // u48 = 0x3039 = 12345
};

// haElectricalMeasurement AttributeReport: RMSVoltage (0x0505 u16 = 230).
constexpr std::uint8_t kVoltageFrame[] = {
    0x18, 0x12, 0x0A,
    0x05, 0x05, 0x21, 0xE6, 0x00,  // 0x0505 u16 = 0x00E6 = 230
};

// haElectricalMeasurement AttributeReport: RMSCurrent (0x0508 u16 = 1500).
constexpr std::uint8_t kCurrentFrame[] = {
    0x18, 0x13, 0x0A,
    0x08, 0x05, 0x21, 0xDC, 0x05,  // 0x0508 u16 = 0x05DC = 1500
};

// haElectricalMeasurement AttributeReport: ActivePower (0x050B s16 = 60).
constexpr std::uint8_t kPowerFrame[] = {
    0x18, 0x14, 0x0A,
    0x0B, 0x05, 0x29, 0x3C, 0x00,  // 0x050B s16 = 0x003C = 60
};

void test_exposes_and_bindings() {
    const auto& def = devices::silicon_labs::kDef_RB_ElectricityDsp_061_3;

    // "both"-mode metering plug: on/off + energy + the restored 0x0B04 axis.
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));   // restored
    assert(def_exposes(def, "current"));   // restored

    // Both metering clusters must be bound for the full picture.
    assert(def_binds(def, GEN_ON_OFF));
    assert(def_binds(def, SE_METERING));
    assert(def_binds(def, HA_ELEC));       // restored
}

void test_decodes_electrical_measurement() {
    const auto& def = devices::silicon_labs::kDef_RB_ElectricityDsp_061_3;

    // The dropped 0x0B04 half must now decode voltage/current/power.
    auto rv = dispatch_zcl(def, HA_ELEC, 1, kVoltageFrame);
    assert(rv.any_matched);
    const Value* volt = rv.merged.find("voltage");
    assert(volt && volt->type == ValueType::Uint && volt->u == 230);

    auto rc = dispatch_zcl(def, HA_ELEC, 1, kCurrentFrame);
    assert(rc.any_matched);
    const Value* cur = rc.merged.find("current");
    assert(cur && cur->type == ValueType::Uint && cur->u == 1500);

    auto rp = dispatch_zcl(def, HA_ELEC, 1, kPowerFrame);
    assert(rp.any_matched);
    const Value* pw = rp.merged.find("power");
    assert(pw && pw->type == ValueType::Int && pw->i == 60);
}

void test_metering_and_onoff_still_decode() {
    const auto& def = devices::silicon_labs::kDef_RB_ElectricityDsp_061_3;

    auto ron = dispatch_zcl(def, GEN_ON_OFF, 1, kOnOffFrame);
    assert(ron.any_matched);
    const Value* st = ron.merged.find("state");
    assert(st != nullptr);

    auto ren = dispatch_zcl(def, SE_METERING, 1, kEnergyFrame);
    assert(ren.any_matched);
    const Value* en = ren.merged.find("energy");
    assert(en && en->type == ValueType::Uint && en->u == 12345);
}

}  // namespace

int main() {
    test_exposes_and_bindings();
    test_decodes_electrical_measurement();
    test_metering_and_onoff_still_decode();
    return 0;
}
