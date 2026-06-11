// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Soanalarm SNT858Z — a TS0601 Tuya-MCU soil moisture
// sensor reporting on cluster 0xEF00 (manuSpecificTuya) via the datapoint
// stream.
//
// Bug fixed: the auto-port mis-classified this Tuya-DP sensor as a phantom
// on/off + battery device. It wired kFzOnOff + kFzBattery, exposed
// state/battery/voltage, and bound genOnOff(0x0006) + genPowerCfg(0x0001). A
// Tuya-MCU device never speaks those clusters, so SNT858Z decoded NOTHING real
// (no soil_moisture, temperature, temperature_unit, or battery) and carried a
// dead controllable on/off state. The def now wires the z2m DP map
// (soanalarm.ts #SNT858Z) via factory::TuyaRw:
//   dp3  soil_moisture     Numeric raw (/1)  → soil_moisture
//   dp5  temperature       Numeric raw (/1)  → temperature  (z2m valueConverter.raw,
//                                              NOT the /10 dpTemperature default)
//   dp9  temperature_unit  Enum {0:celsius,1:fahrenheit} → temperature_unit
//   dp15 battery           Numeric raw (/1)  → battery
//
// These tests pin (a) the def resolves to a Tuya-DP def with a real DP fz/tz
// converter and NO phantom genOnOff/genPowerCfg bindings or `state` expose,
// (b) it carries the four sensor exposes incl. the temperature_unit enum, and
// (c) the core datapoints decode on the real 0xEF00 wire shape with raw (/1)
// scaling and the correct unit-enum mapping.
//
// z2m-source: soanalarm.ts #SNT858Z (tuya.modernExtend.tuyaBase + tuyaDatapoints).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/soanalarm_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> soanalarm_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::soanalarm::kSoanalarmRegistry,
        devices::soanalarm::kSoanalarmRegistryCount);
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return &def.exposes[i];
        }
    }
    return nullptr;
}

DecodedMessage make_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

}  // namespace

// The zigbeeModel "SNT858Z" resolves to a real Tuya-DP def: a DP fz converter
// AND a DP tz converter (TuyaRw, temperature_unit is STATE_SET). The phantom
// on/off + battery bundle must be gone — no genOnOff/genPowerCfg bindings and
// no controllable `state` expose.
static void test_resolves_to_tuya_dp_def() {
    const auto* d = find_definition("SNT858Z", nullptr, soanalarm_registry());
    assert(d && "SNT858Z model must resolve to a def");
    assert(d->model && std::strcmp(d->model, "SNT858Z") == 0);
    assert(d->from_zigbee_count >= 1 && "must wire a Tuya DP fz converter");
    assert(d->to_zigbee && d->to_zigbee_count >= 1 &&
           "TuyaRw wires a DP write converter for temperature_unit");

    // The Tuya DP fz converter must be wired on cluster manuSpecificTuya.
    bool tuya_fz = false;
    for (std::uint8_t i = 0; i < d->from_zigbee_count; ++i) {
        const FzConverter* fz = d->from_zigbee[i];
        if (fz && fz->cluster && std::strcmp(fz->cluster, "manuSpecificTuya") == 0) {
            tuya_fz = true;
        }
    }
    assert(tuya_fz && "Tuya DP fz must be on manuSpecificTuya");

    // No phantom genOnOff(0x0006) / genPowerCfg(0x0001) bindings.
    for (std::size_t i = 0; i < d->bindings_count; ++i) {
        assert(d->bindings[i].cluster_id != 0x0006 &&
               "no phantom genOnOff binding on a Tuya-MCU sensor");
        assert(d->bindings[i].cluster_id != 0x0001 &&
               "no phantom genPowerCfg binding on a Tuya-MCU sensor");
    }

    // The dead controllable on/off state expose must be gone.
    assert(find_expose(*d, "state") == nullptr &&
           "phantom on/off state expose removed");
    assert(find_expose(*d, "voltage") == nullptr &&
           "phantom voltage expose removed");
}

// Sensor exposes: temperature + soil_moisture + battery (Numeric, State) and a
// temperature_unit enum (StateSet, {celsius,fahrenheit}).
static void test_sensor_exposes() {
    const auto* d = find_definition("SNT858Z", nullptr, soanalarm_registry());
    assert(d);

    const Expose* t = find_expose(*d, "temperature");
    assert(t && t->type == ExposeType::Numeric);

    const Expose* sm = find_expose(*d, "soil_moisture");
    assert(sm && sm->type == ExposeType::Numeric);

    const Expose* b = find_expose(*d, "battery");
    assert(b && b->type == ExposeType::Numeric);

    const Expose* tu = find_expose(*d, "temperature_unit");
    assert(tu && tu->type == ExposeType::Enum &&
           tu->access == Access::StateSet && tu->enum_count == 2);
}

// dp3 soil_moisture and dp15 battery decode raw (/1) as integers; dp5
// temperature decodes raw (/1) — NOT the /10 default. All on the real 0xEF00
// wire shape (big-endian 4-byte numeric).
static void test_numeric_dps_decode_raw() {
    const auto* d = find_definition("SNT858Z", nullptr, soanalarm_registry());
    assert(d);

    const std::uint8_t kSoil55[] = { 0x00, 0x00, 0x00, 0x37 };  // 55 %
    const std::uint8_t kTemp23[] = { 0x00, 0x00, 0x00, 0x17 };  // 23 (raw /1)
    const std::uint8_t kBatt90[] = { 0x00, 0x00, 0x00, 0x5A };  // 90 %
    const TuyaDpRecord recs[] = {
        {  3, 0x02, std::span<const std::uint8_t>(kSoil55, 4) },
        {  5, 0x02, std::span<const std::uint8_t>(kTemp23, 4) },
        { 15, 0x02, std::span<const std::uint8_t>(kBatt90, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 3));
    assert(result.any_matched);

    const Value* sm = result.merged.find("soil_moisture");
    assert(sm && sm->type == ValueType::Int && sm->i == 55);

    // raw /1: 23 stays 23, NOT 2.3 (would be the /10 dpTemperature default).
    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Int && t->i == 23);

    const Value* b = result.merged.find("battery");
    assert(b && b->type == ValueType::Int && b->i == 90);
}

// dp9 temperature_unit enum → string label with z2m mapping (1 → "fahrenheit").
static void test_temperature_unit_enum_decode() {
    const auto* d = find_definition("SNT858Z", nullptr, soanalarm_registry());
    assert(d);

    const std::uint8_t kFahrenheit[] = { 0x01 };  // → "fahrenheit"
    const TuyaDpRecord recs[] = {
        { 9, 0x04, std::span<const std::uint8_t>(kFahrenheit, 1) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);

    const Value* tu = result.merged.find("temperature_unit");
    assert(tu && tu->type == ValueType::StringRef);
    assert(tu->str && std::strcmp(tu->str, "fahrenheit") == 0);
}

int main() {
    test_resolves_to_tuya_dp_def();
    test_sensor_exposes();
    test_numeric_dps_decode_raw();
    test_temperature_unit_enum_decode();
    return 0;
}
