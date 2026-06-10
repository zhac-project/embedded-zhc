// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Atlantic Group GW003-AS-IN-TE-FC (Naviclim interface
// for Takao air conditioners), zigbeeModel "Adapter Zigbee FUJITSU".
//
// The auto-generated port wired only the generic kFzThermostat + kFzFanMode.
// kFzThermostat decodes hvacThermostat attrs 0x0000 / 0x0012 / 0x001C, but
// z2m's fz.thermostat also decodes occupiedCoolingSetpoint (attr 0x0011) and
// programingOperMode (attr 0x0025) — both of which this def EXPOSES, so they
// were dead on read. The graduated Tier-2 override adds
// kFzAtlanticThermostatExtras to close that gap.
//
// These tests pin (a) the expose list still surfaces the climate keys,
// (b) the new extras converter is wired against hvacThermostat, and (c) real
// ZCL reports for attr 0x0011 + 0x0025 decode through the def at the
// dispatch_from_zigbee boundary. local_temperature (still via kFzThermostat)
// and fan_mode (kFzFanMode) are spot-checked to confirm no regression.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::atlantic {
extern const PreparedDefinition kDef_GW003_AS_IN_TE_FC;
}  // namespace zhc::devices::atlantic

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

// hvacThermostat (0x0201) OccupiedCoolingSetpoint attr 0x0011, int16.
//   0x0898 = 2200 raw -> z2m 22.00 °C (ZHC emits the raw int).
constexpr std::uint8_t kCoolingSetpoint_2200[] = {
    0x08,              // FC: SRV->CLI, disable default rsp
    0x10,              // tsn
    0x0A,              // Report Attributes
    0x11, 0x00,        // attr id 0x0011 (OccupiedCoolingSetpoint)
    0x29,              // type int16
    0x98, 0x08,        // LE value 0x0898 = 2200
};

// hvacThermostat (0x0201) ProgrammingOperationMode attr 0x0025, enum8.
//   raw 4 -> z2m "eco" (ZHC emits the raw u8).
constexpr std::uint8_t kProgOperMode_eco[] = {
    0x08, 0x11, 0x0A,
    0x25, 0x00,        // attr id 0x0025 (ProgrammingOperationMode)
    0x30,              // type enum8
    0x04,              // raw 4 -> eco
};

// hvacThermostat (0x0201) LocalTemperature attr 0x0000, int16 (regression).
//   0x093A = 2362 raw -> 23.62 °C.
constexpr std::uint8_t kLocalTemp_2362[] = {
    0x08, 0x12, 0x0A,
    0x00, 0x00,
    0x29,              // int16
    0x3A, 0x09,        // 0x093A = 2362
};

}  // namespace

static void test_exposes_climate_keys_present() {
    const auto& def = devices::atlantic::kDef_GW003_AS_IN_TE_FC;
    assert(has_expose(def, "local_temperature"));
    assert(has_expose(def, "current_heating_setpoint"));
    assert(has_expose(def, "occupied_cooling_setpoint"));    // gap fixed (read)
    assert(has_expose(def, "system_mode"));
    assert(has_expose(def, "fan_mode"));
    assert(has_expose(def, "programming_operation_mode"));   // gap fixed (read)
}

static void test_decodes_cooling_setpoint() {
    auto raw = build_frame(/*cluster=*/0x0201, /*src_ep=*/1, kCoolingSetpoint_2200);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::atlantic::kDef_GW003_AS_IN_TE_FC, raw, ctx);

    assert(result.any_matched);
    const Value* c = result.merged.find("occupied_cooling_setpoint");
    assert(c && c->type == ValueType::Int);
    assert(c->i == 2200);  // raw s16; z2m scales /100 downstream
}

static void test_decodes_programming_operation_mode() {
    auto raw = build_frame(0x0201, 1, kProgOperMode_eco);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::atlantic::kDef_GW003_AS_IN_TE_FC, raw, ctx);

    assert(result.any_matched);
    const Value* p = result.merged.find("programming_operation_mode");
    assert(p && p->type == ValueType::Uint);
    assert(p->u == 4);  // raw enum; SPA maps 4 -> "eco"
}

static void test_local_temperature_still_decodes() {
    auto raw = build_frame(0x0201, 1, kLocalTemp_2362);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::atlantic::kDef_GW003_AS_IN_TE_FC, raw, ctx);

    assert(result.any_matched);
    const Value* t = result.merged.find("local_temperature");
    assert(t && t->type == ValueType::Int);
    assert(t->i == 2362);
}

int main() {
    test_exposes_climate_keys_present();
    test_decodes_cooling_setpoint();
    test_decodes_programming_operation_mode();
    test_local_temperature_still_decodes();
    return 0;
}
