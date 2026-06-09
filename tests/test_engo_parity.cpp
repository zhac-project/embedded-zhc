// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Engo (ENGO Controls) TS0601 thermostat / heating
// family — pure Tuya-datapoint devices reporting on cluster 0xEF00
// (manuSpecificTuya) cmd 0x02.
//
// Bug fixed: the engo registry held SEVEN stale defs left over from an
// earlier generation pass —
//   kDef_E25_230, kDef_E25_BATB, kDef_E40, kDef_ECB62_ZB,
//   kDef_EONE, kDef_EONE_230W, kDef_EONE_BATB
// — each a bare battery + on/off stub (kFzBattery + kFzOnOff, exposes
// state/battery/voltage, bound genOnOff 0x0006 and NO Tuya DP decoder).
// Every stub duplicated the {zigbeeModel "TS0601" + manufacturerName
// "_TZE20x_*"} fingerprint of one of the ten kDefEng__TZE* DP defs, which
// carry the full datapoint map (setpoint / local_temperature / system_mode /
// running_state / child_lock / …). Two defs per fingerprint made the
// resolved device behaviour depend purely on registry order
// (find_definition is first-match within the Tuya-discriminated pass), so a
// thermostat could surface only battery+state instead of its core climate
// surface. The seven stubs were removed; the kDefEng__TZE* DP defs cover all
// ten manufacturer names as the sole match each.
//
// These tests pin (a) that every Engo manufacturer name resolves through
// find_definition to its full DP def (the proper model label + thermostat /
// control-box exposes), never a battery+on/off stub, and (b) that the
// surviving def actually decodes the device's core datapoints on the real
// 0xEF00/cmd-0x02 wire shape — keys the removed stub could never produce.
//
// z2m-source: engo.ts — every entry is
// tuya.modernExtend.tuyaBase({dp:true}) with a per-DP map (e.g. E25-230
// _TZE204_cmyc8g5i: dp1 state, dp2 system_mode enum {heat,cool}, dp3
// running_state, dp16 setpoint /10, dp24 local_temperature /10).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/engo_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> engo_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::engo::kEngoRegistry, devices::engo::kEngoRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

// The signature of the removed stubs: exposes were exactly
// {state, battery, voltage}, with no datapoint-bearing key.
bool is_battery_onoff_stub(const PreparedDefinition& def) {
    if (def.exposes_count != 3) return false;
    return def_exposes(def, "state") && def_exposes(def, "battery") &&
           def_exposes(def, "voltage") &&
           !def_exposes(def, "current_heating_setpoint") &&
           !def_exposes(def, "local_temperature") &&
           !def_exposes(def, "pump_state");
}

// Build the DecodedMessage + InboundApsFrame pair the dispatcher expects for
// an inbound Tuya DP report on cluster 0xEF00 cmd 0x02.
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

// Every Engo manufacturer name, paired with the model label its full DP def
// must carry. If a stub still shadowed the fingerprint, find_definition would
// resolve to the battery+on/off def instead (which the guards below reject).
struct ManuModel {
    const char* manu;
    const char* model;
};
constexpr ManuModel kEngoManus[] = {
    { "_TZE200_awnadkan", "EONE-230W" },
    { "_TZE200_gtouvmvl", "EONE-BATB" },
    { "_TZE200_oahqgdig", "ECB62-ZB" },
    { "_TZE200_zaabefnt", "ECB62-ZB" },
    { "_TZE204_ca3i8m8p", "EONE-230W" },
    { "_TZE204_cg8hdnjv", "E25-BATB" },
    { "_TZE204_cmyc8g5i", "E25-230" },
    { "_TZE204_djurk6p5", "EONE" },
    { "_TZE204_glk6viwg", "E40" },
    { "_TZE204_lnxdk2ch", "E40" },
};

}  // namespace

// Core fix: each Engo manufacturer name resolves through find_definition to
// its full DP def — correct model label, and a real datapoint-bearing surface
// (never the removed battery+on/off stub).
static void test_all_manus_resolve_to_full_dp_def() {
    for (const auto& mm : kEngoManus) {
        const auto* d = find_definition("TS0601", mm.manu, engo_registry());
        assert(d && "Engo manufacturer name must resolve to a def");
        assert(d->model && std::strcmp(d->model, mm.model) == 0);
        assert(!is_battery_onoff_stub(*d) &&
               "resolved def must not be the battery+on/off stub");
        // Full DP defs wire a Tuya DP from_zigbee converter (state was empty
        // on the stub, which carried only kFzBattery + kFzOnOff).
        assert(d->from_zigbee_count >= 1);
        assert(d->to_zigbee && d->to_zigbee_count >= 1 &&
               "DP defs carry a Tuya write converter; stubs had none");
    }
}

// Registry hygiene: exactly the ten DP defs remain, and not one of them is a
// battery+on/off stub. Pins that the stale seven are gone and none crept back.
static void test_registry_has_no_stub_defs() {
    assert(devices::engo::kEngoRegistryCount == 10);
    for (const auto* d : engo_registry()) {
        assert(d != nullptr);
        assert(!is_battery_onoff_stub(*d));
    }
}

// The four thermostat models (E25-230/E25-BATB/E40/EONE/EONE-230W/EONE-BATB)
// must declare the core climate surface a stub never exposed.
static void test_thermostat_defs_declare_climate_exposes() {
    const char* therm_manus[] = {
        "_TZE204_cmyc8g5i",  // E25-230
        "_TZE204_cg8hdnjv",  // E25-BATB
        "_TZE204_glk6viwg",  // E40
        "_TZE204_djurk6p5",  // EONE
        "_TZE200_awnadkan",  // EONE-230W
        "_TZE200_gtouvmvl",  // EONE-BATB
    };
    for (const char* manu : therm_manus) {
        const auto* d = find_definition("TS0601", manu, engo_registry());
        assert(d);
        assert(def_exposes(*d, "current_heating_setpoint"));
        assert(def_exposes(*d, "local_temperature"));
    }
    // The ECB62-ZB control box is not a room thermostat: it exposes the
    // underfloor-heating outputs instead (pump/boiler), which the stub also
    // dropped. Pin its real surface too.
    const auto* cb = find_definition("TS0601", "_TZE200_oahqgdig", engo_registry());
    assert(cb && std::strcmp(cb->model, "ECB62-ZB") == 0);
    assert(def_exposes(*cb, "pump_state"));
    assert(def_exposes(*cb, "boiler_state"));
}

// Real-wire decode through the dispatcher against the resolved E25-230 def.
// dp16 currentHeatingSetpoint, Numeric divisor 10 → Float (raw/10).
// A battery+on/off stub would surface none of these keys.
static void test_e25_230_setpoint_and_local_temp_decode() {
    const auto* d = find_definition("TS0601", "_TZE204_cmyc8g5i", engo_registry());
    assert(d);

    // Big-endian 4-byte numeric: 215 = 0x000000D7 → 21.5 °C.
    const std::uint8_t kSp215[] = { 0x00, 0x00, 0x00, 0xD7 };
    const std::uint8_t kLt208[] = { 0x00, 0x00, 0x00, 0xD0 };  // 208 → 20.8
    const TuyaDpRecord recs[] = {
        { 16, 0x02, std::span<const std::uint8_t>(kSp215, 4) },
        { 24, 0x02, std::span<const std::uint8_t>(kLt208, 4) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* sp = result.merged.find("current_heating_setpoint");
    assert(sp && sp->type == ValueType::Float);
    assert(sp->f > 21.49 && sp->f < 21.51);

    const Value* lt = result.merged.find("local_temperature");
    assert(lt && lt->type == ValueType::Float);
    assert(lt->f > 20.79 && lt->f < 20.81);
}

// dp2 system_mode, Enum {0:heat, 1:cool} → StringRef "heat";
// dp1 state, Bool=true → state=true. The stub bound genOnOff but never
// decoded the Tuya DP frame, so neither key appeared.
static void test_e25_230_mode_and_state_decode() {
    const auto* d = find_definition("TS0601", "_TZE204_cmyc8g5i", engo_registry());
    assert(d);

    const std::uint8_t kEnumHeat[] = { 0x00 };
    const std::uint8_t kBoolOn[]   = { 0x01 };
    const TuyaDpRecord recs[] = {
        { 2, 0x04, std::span<const std::uint8_t>(kEnumHeat, 1) },
        { 1, 0x01, std::span<const std::uint8_t>(kBoolOn, 1) },
    };
    const auto result = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(result.any_matched);

    const Value* mode = result.merged.find("system_mode");
    assert(mode && mode->type == ValueType::StringRef);
    assert(mode->str && std::strcmp(mode->str, "heat") == 0);

    const Value* st = result.merged.find("state");
    assert(st && st->type == ValueType::Bool && st->b == true);
}

int main() {
    test_all_manus_resolve_to_full_dp_def();
    test_registry_has_no_stub_defs();
    test_thermostat_defs_declare_climate_exposes();
    test_e25_230_setpoint_and_local_temp_decode();
    test_e25_230_mode_and_state_decode();
    return 0;
}
