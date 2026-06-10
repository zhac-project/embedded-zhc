// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Akuvox M423-9E smart temperature & humidity sensor,
// zigbeeModel "TS0201" / manufacturer "_TYZB01_ujfk3xd9".
//
// The auto-generated port reduced this device to battery-only: it wired only
// the generic kFzBattery and exposed battery/voltage. But z2m declares
// `fromZigbee: [fz.temperature, fz.humidity, fz.battery]` and
// `exposes: [e.battery(), e.temperature(), e.humidity()]` — both the
// temperature and humidity channels (the whole point of the sensor) were
// dropped on decode. The graduated Tier-2 override adds kFzTemperature +
// kFzHumidity + the temperature & humidity exposes.
//
// These tests pin (a) the expose list surfaces temperature + humidity +
// battery, (b) a real msTemperatureMeasurement report (ep1) decodes to
// `temperature`, (c) a real msRelativeHumidity report (ep2) decodes to
// `humidity`, and (d) the genPowerCfg battery path still decodes battery.
// z2m binds temperature on ep1 and humidity on ep2, but the generic converters
// use WILDCARD_ENDPOINT so they match regardless of source endpoint.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::akuvox {
extern const PreparedDefinition kDef_M423_9E;
}  // namespace zhc::devices::akuvox

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

// msRelativeHumidity (0x0405) MeasuredValue attr 0x0000, uint16.
//   0x1388 = 5000 raw -> z2m 50.00 % (ZHC emits /100 as float).
constexpr std::uint8_t kHumidity_5000[] = {
    0x08,              // FC: SRV->CLI, disable default rsp
    0x11,              // tsn
    0x0A,              // Report Attributes
    0x00, 0x00,        // attr id 0x0000 (MeasuredValue)
    0x21,              // type uint16
    0x88, 0x13,        // LE value 0x1388 = 5000
};

// genPowerCfg (0x0001) BatteryPercentageRemaining attr 0x0021, uint8.
//   0xC8 = 200 raw -> 100 % (half-percent units).
constexpr std::uint8_t kBatteryPct_100[] = {
    0x08, 0x12, 0x0A,
    0x21, 0x00,        // attr id 0x0021
    0x20,              // type uint8
    0xC8,              // raw 200 -> 100 %
};

}  // namespace

static void test_exposes_present() {
    const auto& def = devices::akuvox::kDef_M423_9E;
    assert(has_expose(def, "temperature"));   // gap fixed (whole device purpose)
    assert(has_expose(def, "humidity"));      // gap fixed
    assert(has_expose(def, "battery"));
}

static void test_temperature_decodes() {
    auto raw = build_frame(/*cluster=*/0x0402, /*src_ep=*/1, kTemp_2000);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::akuvox::kDef_M423_9E, raw, ctx);

    assert(result.any_matched);
    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(t->f > 19.99f && t->f < 20.01f);  // 2000 / 100 = 20.00 °C
}

static void test_humidity_decodes() {
    auto raw = build_frame(/*cluster=*/0x0405, /*src_ep=*/2, kHumidity_5000);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msRelativeHumidity";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::akuvox::kDef_M423_9E, raw, ctx);

    assert(result.any_matched);
    const Value* h = result.merged.find("humidity");
    assert(h && h->type == ValueType::Float);
    assert(h->f > 49.99f && h->f < 50.01f);  // 5000 / 100 = 50.00 %
}

static void test_battery_percent_decodes() {
    auto raw = build_frame(/*cluster=*/0x0001, /*src_ep=*/1, kBatteryPct_100);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genPowerCfg";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::akuvox::kDef_M423_9E, raw, ctx);

    assert(result.any_matched);
    const Value* b = result.merged.find("battery");
    assert(b && b->type == ValueType::Uint);
    assert(b->u == 100);  // 200 half-percent -> 100 %
}

int main() {
    test_exposes_present();
    test_temperature_decodes();
    test_humidity_decodes();
    test_battery_percent_decodes();
    return 0;
}
