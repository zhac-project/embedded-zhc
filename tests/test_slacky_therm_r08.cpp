// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the SlackyDiy THERM_SLACKY_DIY_R08 floor-heating
// thermostat (custom firmware), zigbeeModel "Tuya_Thermostat_r08".
//
// z2m-source: slacky_diy.ts #THERM_SLACKY_DIY_R08 —
//   fromZigbee: localFromZigbeeThermostat (incl. fz.thermostat),
//   toZigbee:   localToZigbeeThermostat,
//   configure:  configureCommon,
//   exposes:    e.climate().withLocalTemperature()
//                 .withSetpoint("occupied_heating_setpoint",5,45,0.5)
//                 .withSystemMode(["off","heat"])  (+ child_lock, schedules…)
//
// The auto-generated def mis-lowered R08 as a battery + on/off switch
// (kFzBattery + kFzOnOff, exposes state/battery/voltage, binds 0x0001 +
// 0x0006) — the wrong device class entirely. It was graduated to a Tier-2
// parent override (definitions/slacky_diy/THERM_SLACKY_DIY_R08.cpp) that
// matches the 13 sibling THERM defs: kFzThermostat / kTzThermostat on the
// custom hvacThermostat cluster (0x0201), surfacing the three standard ZCL
// thermostat attributes z2m's fz.thermostat covers. These tests pin that
// the override now (a) exposes local_temperature / current_heating_setpoint
// / system_mode and NOT the phantom switch/battery, (b) binds 0x0201 and
// NOT genOnOff/genPowerCfg, and (c) decodes those attributes from real
// hvacThermostat report wire shapes.
//
// kFzThermostat copies the raw s16/enum values straight through (no
// scaling) — see definitions/_generic/_shared.cpp fz_thermostat:
//   attr 0x0000 LocalTemperature        (s16) -> local_temperature  (Int)
//   attr 0x0012 OccupiedHeatingSetpoint (s16) -> current_heating_setpoint (Int)
//   attr 0x001C SystemMode              (enum8) -> system_mode (Uint)

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::slacky_diy {
extern const PreparedDefinition kDef_THERM_SLACKY_DIY_R08;
}

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

// hvacThermostat (0x0201) attr 0x0000 LocalTemperature, int16.
//   0x092A = 2346 raw -> z2m scales /100 to 23.46 °C; ZHC keeps raw 2346.
constexpr std::uint8_t kLocalTempReport_2346[] = {
    0x08,              // FC: SRV->CLI, disable default rsp
    0x10,              // tsn
    0x0A,              // Report Attributes
    0x00, 0x00,        // attr id 0x0000
    0x29,              // type int16
    0x2A, 0x09,        // LE value 0x092A = 2346
};

// hvacThermostat attr 0x0012 OccupiedHeatingSetpoint, int16.
//   0x0BB8 = 3000 raw -> 30.00 °C.
constexpr std::uint8_t kSetpointReport_3000[] = {
    0x08,
    0x11,
    0x0A,
    0x12, 0x00,        // attr id 0x0012
    0x29,              // type int16
    0xB8, 0x0B,        // LE value 0x0BB8 = 3000
};

// hvacThermostat attr 0x001C SystemMode, enum8.
//   0x04 = "heat".
constexpr std::uint8_t kSystemModeReport_heat[] = {
    0x08,
    0x12,
    0x0A,
    0x1C, 0x00,        // attr id 0x001C
    0x30,              // type enum8
    0x04,              // value 4 = heat
};

}  // namespace

// ─────────────────────────────────────────────────────────────────────
// 1. Definition shape — thermostat exposes, NOT a phantom switch/battery.
// ─────────────────────────────────────────────────────────────────────

static void test_r08_exposes_thermostat_not_switch() {
    const auto& def = devices::slacky_diy::kDef_THERM_SLACKY_DIY_R08;

    bool has_local_temp = false, has_setpoint = false, has_mode = false;
    bool has_state = false, has_battery = false, has_voltage = false;
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const char* n = def.exposes[i].name;
        if (!n) continue;
        if (std::strcmp(n, "local_temperature") == 0)        has_local_temp = true;
        if (std::strcmp(n, "current_heating_setpoint") == 0) has_setpoint   = true;
        if (std::strcmp(n, "system_mode") == 0)              has_mode       = true;
        if (std::strcmp(n, "state") == 0)                    has_state      = true;
        if (std::strcmp(n, "battery") == 0)                  has_battery    = true;
        if (std::strcmp(n, "voltage") == 0)                  has_voltage    = true;
    }
    assert(has_local_temp);   // ← the gap this override fixes
    assert(has_setpoint);     // ← the gap this override fixes
    assert(has_mode);         // ← the gap this override fixes
    // The mis-generated phantom switch/battery surface must be gone.
    assert(!has_state);
    assert(!has_battery);
    assert(!has_voltage);
}

static void test_r08_from_zigbee_and_bindings_are_thermostat() {
    const auto& def = devices::slacky_diy::kDef_THERM_SLACKY_DIY_R08;

    bool fz_therm = false, fz_onoff = false, fz_batt = false;
    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* fz = def.from_zigbee[i];
        if (!fz || !fz->cluster) continue;
        if (std::strcmp(fz->cluster, "hvacThermostat") == 0) fz_therm = true;
        if (std::strcmp(fz->cluster, "genOnOff") == 0)       fz_onoff = true;
        if (std::strcmp(fz->cluster, "genPowerCfg") == 0)    fz_batt  = true;
    }
    assert(fz_therm);
    assert(!fz_onoff);
    assert(!fz_batt);

    // tz path must drive the thermostat too.
    bool tz_therm = false;
    for (std::uint8_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* tz = def.to_zigbee[i];
        if (tz && tz->cluster_id == 0x0201) tz_therm = true;
    }
    assert(tz_therm);

    bool bind_therm = false, bind_onoff = false, bind_batt = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        switch (def.bindings[i].cluster_id) {
            case 0x0201: bind_therm = true; break;
            case 0x0006: bind_onoff = true; break;
            case 0x0001: bind_batt  = true; break;
            default: break;
        }
    }
    assert(bind_therm);
    assert(!bind_onoff);
    assert(!bind_batt);
}

// ─────────────────────────────────────────────────────────────────────
// 2. Decode — dispatch real hvacThermostat reports through the def.
// ─────────────────────────────────────────────────────────────────────

static void test_r08_decodes_local_temperature() {
    auto raw = build_frame(/*cluster=*/0x0201, /*src_ep=*/1, kLocalTempReport_2346);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::slacky_diy::kDef_THERM_SLACKY_DIY_R08, raw, ctx);

    assert(result.any_matched);
    const Value* t = result.merged.find("local_temperature");
    assert(t && t->type == ValueType::Int);
    assert(t->i == 2346);
}

static void test_r08_decodes_setpoint() {
    auto raw = build_frame(0x0201, 1, kSetpointReport_3000);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::slacky_diy::kDef_THERM_SLACKY_DIY_R08, raw, ctx);

    assert(result.any_matched);
    const Value* s = result.merged.find("current_heating_setpoint");
    assert(s && s->type == ValueType::Int);
    assert(s->i == 3000);
}

static void test_r08_decodes_system_mode() {
    auto raw = build_frame(0x0201, 1, kSystemModeReport_heat);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::slacky_diy::kDef_THERM_SLACKY_DIY_R08, raw, ctx);

    assert(result.any_matched);
    const Value* m = result.merged.find("system_mode");
    assert(m && m->type == ValueType::Uint);
    assert(m->u == 4);
}

int main() {
    test_r08_exposes_thermostat_not_switch();
    test_r08_from_zigbee_and_bindings_are_thermostat();
    test_r08_decodes_local_temperature();
    test_r08_decodes_setpoint();
    test_r08_decodes_system_mode();
    return 0;
}
