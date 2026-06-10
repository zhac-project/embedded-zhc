// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Casaia CTHS-317-ET remote temperature probe,
// zigbeeModel "CTHS317ET".
//
// The auto-generated port reduced this device to battery-only: it wired only
// the generic kFzBattery and exposed battery/voltage. But z2m declares
// `fromZigbee: [fz.temperature, fz.battery]` and
// `exposes: [e.temperature(), e.battery_low(), e.battery()]` — the temperature
// probe (the whole point of the device) was dropped on decode and the
// battery_low alarm expose was missing. The graduated Tier-2 override adds
// kFzTemperature + the temperature & battery_low exposes (kFzBattery already
// emits battery_low from batteryAlarmState attr 0x0035).
//
// These tests pin (a) the expose list surfaces temperature + battery +
// battery_low, (b) a real msTemperatureMeasurement report decodes to
// `temperature` through the def at the dispatch_from_zigbee boundary, and
// (c) the genPowerCfg battery path still decodes battery + battery_low.
// The probe reports on endpoint 3 (z2m binds ep3); the generic converters use
// WILDCARD_ENDPOINT so they match regardless.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::casaia {
extern const PreparedDefinition kDef_CTHS_317_ET;
}  // namespace zhc::devices::casaia

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

// msTemperatureMeasurement (0x0402) MeasuredValue attr 0x0000, int16.
//   0x07D0 = 2000 raw -> z2m 20.00 °C (ZHC emits /100 as float).
constexpr std::uint8_t kTemp_2000[] = {
    0x08,              // FC: SRV->CLI, disable default rsp
    0x10,              // tsn
    0x0A,              // Report Attributes
    0x00, 0x00,        // attr id 0x0000 (MeasuredValue)
    0x29,              // type int16
    0xD0, 0x07,        // LE value 0x07D0 = 2000
};

// genPowerCfg (0x0001) BatteryPercentageRemaining attr 0x0021, uint8.
//   0xC8 = 200 raw -> 100 % (half-percent units).
constexpr std::uint8_t kBatteryPct_100[] = {
    0x08, 0x11, 0x0A,
    0x21, 0x00,        // attr id 0x0021
    0x20,              // type uint8
    0xC8,              // raw 200 -> 100 %
};

// genPowerCfg (0x0001) BatteryAlarmState attr 0x0035, bitmap32.
//   bit 0 set -> battery_low true.
constexpr std::uint8_t kBatteryAlarm_low[] = {
    0x08, 0x12, 0x0A,
    0x35, 0x00,        // attr id 0x0035
    0x1B,              // type bitmap32
    0x01, 0x00, 0x00, 0x00,  // LE 0x00000001 -> bit0 set
};

}  // namespace

static void test_exposes_present() {
    const auto& def = devices::casaia::kDef_CTHS_317_ET;
    assert(has_expose(def, "temperature"));   // gap fixed (whole device purpose)
    assert(has_expose(def, "battery"));
    assert(has_expose(def, "battery_low"));   // gap fixed
}

static void test_temperature_decodes() {
    auto raw = build_frame(/*cluster=*/0x0402, /*src_ep=*/3, kTemp_2000);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::casaia::kDef_CTHS_317_ET, raw, ctx);

    assert(result.any_matched);
    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(t->f > 19.99f && t->f < 20.01f);  // 2000 / 100 = 20.00 °C
}

static void test_battery_percent_decodes() {
    auto raw = build_frame(/*cluster=*/0x0001, /*src_ep=*/3, kBatteryPct_100);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genPowerCfg";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::casaia::kDef_CTHS_317_ET, raw, ctx);

    assert(result.any_matched);
    const Value* b = result.merged.find("battery");
    assert(b && b->type == ValueType::Uint);
    assert(b->u == 100);  // 200 half-percent -> 100 %
}

static void test_battery_low_decodes() {
    auto raw = build_frame(/*cluster=*/0x0001, /*src_ep=*/3, kBatteryAlarm_low);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genPowerCfg";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::casaia::kDef_CTHS_317_ET, raw, ctx);

    assert(result.any_matched);
    const Value* bl = result.merged.find("battery_low");
    assert(bl && bl->type == ValueType::Bool);
    assert(bl->b == true);
}

int main() {
    test_exposes_present();
    test_temperature_decodes();
    test_battery_percent_decodes();
    test_battery_low_decodes();
    return 0;
}
