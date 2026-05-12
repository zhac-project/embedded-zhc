// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2 test: fz_lumi_specific with per-device TagMap.
//
// Three synthetic devices share the same converter but carry different
// `LumiTagMap`s in their `meta`:
//   1. plug metering — tags 0x95 (energy) + 0x98 (power)
//   2. button battery — tags 0x01 / 0x03 / 0x04
//   3. temp/humidity sensor — tags 0x64 / 0x65 / 0x66

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

PreparedDefinition make_def(const lumi::LumiTagMap* map) {
    PreparedDefinition def{};
    static const char* models[] = { "SYNTH" };
    def.zigbee_models       = models;
    def.zigbee_models_count = 1;
    def.model               = "SYNTH";
    def.vendor              = "Test";
    static const FzConverter* const fz_list[] = { &lumi::kFzLumiSpecific };
    def.from_zigbee         = fz_list;
    def.from_zigbee_count   = 1;
    def.meta                = map;
    return def;
}

bool approx(float a, float b, float eps) { return std::fabs(a - b) <= eps; }

// ── Plug metering frame ────────────────────────────────────────────
// MI-struct carries tag 0x95 u32=123456 + tag 0x98 u16=100.
// Wrap in genBasic 0xFF01 char-string record, 10-byte body.
constexpr std::uint8_t kPlugFrame[] = {
    0x18, 0x0A, 0x0A,                     // ZCL header (AttributeReport)
    0x01, 0xFF, 0x42, 0x0A,                // attr 0xFF01, type 0x42 str, len 10
    // MI-struct body (10 bytes):
    0x95, 0x23, 0x40, 0xE2, 0x01, 0x00,     // tag 0x95 u32 123456
    0x98, 0x21, 0x64, 0x00,                 // tag 0x98 u16 100
};

static void test_plug_metering_map() {
    static constexpr lumi::LumiTagEntry entries[] = {
        { lumi::kLumiTagEnergy, "energy", 1000 },   // kWh
        { lumi::kLumiTagPower,  "power",  10   },   // W
    };
    static constexpr lumi::LumiTagMap map{ entries,
                                     sizeof(entries) / sizeof(entries[0]) };

    auto raw = build_frame(0x0000, kPlugFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBasic";

    auto def = make_def(&map);
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);

    const Value* energy = result.merged.find("energy");
    assert(energy && energy->type == ValueType::Float);
    assert(approx(energy->f, 123.456f, 0.001f));

    const Value* power = result.merged.find("power");
    assert(power && power->type == ValueType::Float);
    assert(approx(power->f, 10.0f, 0.001f));
}

// ── Button battery frame (WXKG01LM TLV shape) ───────────────────────
// Same 11-byte MI-struct as the fixture: voltage 2984 / temp 29 / outages 18.
constexpr std::uint8_t kButtonFrame[] = {
    0x18, 0x0B, 0x0A,
    0x01, 0xFF, 0x42, 0x0B,
    0x01, 0x21, 0xA8, 0x0B,     // tag 0x01 u16 = 2984 (voltage mV)
    0x03, 0x28, 0x1D,            // tag 0x03 i8  = 29   (°C)
    0x04, 0x21, 0x12, 0x00,      // tag 0x04 u16 = 18   (outages)
};

static void test_button_battery_map_raw_passthrough() {
    // Divisor 1 everywhere — prove the same converter does raw pass-
    // through when the map doesn't request scaling (fz_lumi_basic owns
    // the voltage-curve derivation; fz_lumi_specific stays mechanical).
    static constexpr lumi::LumiTagEntry entries[] = {
        { lumi::kLumiTagVoltage,     "voltage",            1 },
        { lumi::kLumiTagDeviceTemp,  "device_temperature", 1 },
        { lumi::kLumiTagOutageCount, "power_outage_count", 1 },
    };
    static constexpr lumi::LumiTagMap map{ entries,
                                     sizeof(entries) / sizeof(entries[0]) };

    auto raw = build_frame(0x0000, kButtonFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBasic";

    auto def = make_def(&map);
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);

    const Value* voltage = result.merged.find("voltage");
    assert(voltage && voltage->type == ValueType::Int);
    assert(voltage->i == 2984);

    const Value* dtemp = result.merged.find("device_temperature");
    assert(dtemp && dtemp->type == ValueType::Int);
    assert(dtemp->i == 29);

    const Value* outages = result.merged.find("power_outage_count");
    assert(outages && outages->type == ValueType::Int);
    assert(outages->i == 18);

    // The basic-curve battery% key MUST NOT appear — that derivation
    // belongs to fz_lumi_basic.
    assert(result.merged.find("battery") == nullptr);
}

// ── Temperature/humidity sensor frame ──────────────────────────────
// tags 0x64 temp /100, 0x65 humidity /100, 0x66 pressure /100 (kPa).
constexpr std::uint8_t kTHPFrame[] = {
    0x18, 0x0C, 0x0A,
    0x01, 0xFF, 0x42, 0x0E,                // len = 14
    0x64, 0x21, 0xE6, 0x09,                 // tag 0x64 u16 = 2534 → 25.34
    0x65, 0x21, 0xE0, 0x12,                 // tag 0x65 u16 = 4832 → 48.32
    0x66, 0x23, 0x18, 0x87, 0x01, 0x00,     // tag 0x66 u32 = 100120 → 1001.20
};

static void test_sensor_temp_map() {
    static constexpr lumi::LumiTagEntry entries[] = {
        { lumi::kLumiTagState,    "temperature", 100 },
        { lumi::kLumiTagHumidity, "humidity",    100 },
        { lumi::kLumiTagPressure, "pressure",    100 },
    };
    static constexpr lumi::LumiTagMap map{ entries,
                                     sizeof(entries) / sizeof(entries[0]) };

    auto raw = build_frame(0x0000, kTHPFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBasic";

    auto def = make_def(&map);
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);

    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(approx(t->f, 25.34f, 0.01f));

    const Value* h = result.merged.find("humidity");
    assert(h && h->type == ValueType::Float);
    assert(approx(h->f, 48.32f, 0.01f));

    const Value* p = result.merged.find("pressure");
    assert(p && p->type == ValueType::Float);
    assert(approx(p->f, 1001.20f, 0.01f));
}

// Tag not present in the device's map is silently dropped.
static void test_unknown_tag_ignored() {
    static constexpr lumi::LumiTagEntry entries[] = {
        { lumi::kLumiTagVoltage, "voltage", 1 },   // only tag 0x01 mapped
    };
    static constexpr lumi::LumiTagMap map{ entries, 1 };

    auto raw = build_frame(0x0000, kButtonFrame);   // carries 0x01 / 0x03 / 0x04
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBasic";

    auto def = make_def(&map);
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);
    assert(result.merged.find("voltage")            != nullptr);
    assert(result.merged.find("device_temperature") == nullptr);
    assert(result.merged.find("power_outage_count") == nullptr);
}

// No meta → converter bails.
static void test_missing_meta_returns_false() {
    auto raw = build_frame(0x0000, kButtonFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBasic";

    auto def = make_def(nullptr);
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched == false);
}

}  // namespace

int main() {
    test_plug_metering_map();
    test_button_battery_map_raw_passthrough();
    test_sensor_temp_map();
    test_unknown_tag_ignored();
    test_missing_meta_returns_false();
    return 0;
}
