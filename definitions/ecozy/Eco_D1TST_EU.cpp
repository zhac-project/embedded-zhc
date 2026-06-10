// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecozy 1TST-EU — graduated from generated/ 2026-06-10.
// Smart heating thermostat.
// z2m-source: ecozy.ts #1TST-EU.
//
// z2m wires `fz.battery + fz.thermostat` and exposes a climate block with
// withRunningState(["idle","heat"]) + withLocalTemperatureCalibration() +
// withPiHeatingDemand(STATE_GET). fz.thermostat decodes the full
// hvacThermostat attribute set, but the generic kFzThermostat only handles
// 0x0000 (local_temperature) / 0x0012 (occupied_heating_setpoint ->
// current_heating_setpoint) / 0x001C (system_mode). The three extra keys
// the eCozy declares were therefore exposed-but-DEAD:
//
//   * pi_heating_demand             (0x0008, key "8")  — z2m maps raw 0-255
//                                     -> 0-100 % (no dontMapPIHeatingDemand).
//   * local_temperature_calibration (0x0010, key "16") — z2m raw / 10
//                                     (signed int8, 0.1 °C units -> °C float).
//   * running_state                 (0x0029, key "41") — z2m
//                                     thermostatRunningStates lookup; this
//                                     device's exposed enum is {idle, heat}
//                                     (0 -> idle, else heat).
//
// Added via kFzEcozyThermostatExtras, which delegates to the generic
// thermostat decoder first, then layers the three extras. Exposes upgraded
// from the impoverished auto block (Binary system_mode, no running_state /
// lt_cal / pi_heating_demand) to the real climate-flat surface z2m declares.
//
// NOTE: z2m also binds hvacUserInterfaceCfg (0x0204) in configure(), but it
// wires NO fromZigbee for keypadLockout and exposes none — so keypad_lockout
// is NOT a decode gap here (z2m does not decode it either). Not added.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecozy {
namespace {

constexpr const char* kModels_D1TST_EU[] = { "Thermostat" };

// ── Vendor FZ: thermostat extras ─────────────────────────────────────
// Delegates to the generic thermostat decoder (local_temperature,
// current_heating_setpoint, system_mode), then adds the attrs the generic
// decoder skips but z2m's fz.thermostat decodes and this device exposes.
bool fz_ecozy_thermostat_extras(const ::zhc::DecodedMessage& msg,
                                const ::zhc::FzConverter& self,
                                const ::zhc::PreparedDefinition& def,
                                ::zhc::RuntimeContext& ctx,
                                ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = ::zhc::generic::kFzThermostat.fn.zcl_fn(msg, self, def, ctx, out);

    // 0x0008 PIHeatingDemand (u8). z2m mapNumberRange(raw, 0, 255, 0, 100);
    // half-up integer rounding to mirror the integer `%` expose.
    if (const ::zhc::Value* v = msg.payload.find("8")) {
        std::uint64_t raw = 0;
        bool ok = false;
        if (v->type == ::zhc::ValueType::Uint) { raw = v->u; ok = true; }
        else if (v->type == ::zhc::ValueType::Int && v->i >= 0) {
            raw = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok) {
            if (raw > 255) raw = 255;
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Uint;
            o.u    = (raw * 100u + 127u) / 255u;
            if (out.put("pi_heating_demand", o)) emitted = true;
        }
    }

    // 0x0010 LocalTemperatureCalibration (signed int8, 0.1 °C units).
    // z2m: precisionRound(raw, 2) / 10. Emit as float °C.
    if (const ::zhc::Value* v = msg.payload.find("16")) {
        bool ok = false;
        std::int64_t raw = 0;
        if (v->type == ::zhc::ValueType::Int)  { raw = v->i; ok = true; }
        else if (v->type == ::zhc::ValueType::Uint) {
            raw = static_cast<std::int64_t>(v->u); ok = true;
        }
        if (ok) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Float;
            o.f    = static_cast<float>(raw) / 10.0f;
            if (out.put("local_temperature_calibration", o)) emitted = true;
        }
    }

    // 0x0029 RunningState. z2m thermostatRunningStates lookup; eCozy exposes
    // only {idle, heat} (withRunningState(["idle","heat"])). 0 -> idle,
    // anything else -> heat.
    if (const ::zhc::Value* v = msg.payload.find("41")) {
        bool ok = false;
        std::uint64_t st = 0;
        if (v->type == ::zhc::ValueType::Uint) { st = v->u; ok = true; }
        else if (v->type == ::zhc::ValueType::Int && v->i >= 0) {
            st = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = (st == 0) ? "idle" : "heat";
            if (out.put("running_state", o)) emitted = true;
        }
    }

    return emitted;
}

constexpr ::zhc::FzConverter kFzEcozyThermostatExtras{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ecozy_thermostat_extras },
    .user_config       = nullptr,
};

const FzConverter* const kFz_D1TST_EU[] = {
    &::zhc::generic::kFzBattery,
    &kFzEcozyThermostatExtras,   // thermostat + pi_heating_demand + lt_cal + running_state
};
const TzConverter* const kTz_D1TST_EU[] = {
    &::zhc::generic::kTzThermostat,
};

// z2m climate block (flat exposes — no ExposeType::Climate per project rule):
//   battery, occupied_heating_setpoint (key current_heating_setpoint),
//   local_temperature, system_mode {off,auto,heat}, running_state {idle,heat},
//   local_temperature_calibration, pi_heating_demand (STATE_GET).
constexpr Expose kExp_D1TST_EU[] = {
    { "battery",                       ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "voltage",                       ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
    { "local_temperature",             ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "current_heating_setpoint",      ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "system_mode",                   ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "running_state",                 ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "pi_heating_demand",             ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_D1TST_EU[] = {
    { 1, 0x0001 },   // genPowerCfg (battery)
    { 1, 0x0201 },   // hvacThermostat
};

}  // namespace

extern const PreparedDefinition kDef_D1TST_EU{
    .zigbee_models=kModels_D1TST_EU, .zigbee_models_count=sizeof(kModels_D1TST_EU)/sizeof(kModels_D1TST_EU[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1TST-EU", .vendor="Ecozy",
    .meta=nullptr, .exposes=kExp_D1TST_EU, .exposes_count=sizeof(kExp_D1TST_EU)/sizeof(kExp_D1TST_EU[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1TST_EU, .from_zigbee_count=sizeof(kFz_D1TST_EU)/sizeof(kFz_D1TST_EU[0]),
    .to_zigbee=kTz_D1TST_EU, .to_zigbee_count=sizeof(kTz_D1TST_EU)/sizeof(kTz_D1TST_EU[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_D1TST_EU, .bindings_count=sizeof(kBind_D1TST_EU)/sizeof(kBind_D1TST_EU[0]),
};

}  // namespace zhc::devices::ecozy
