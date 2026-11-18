// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Schneider EER53000 — Wiser radiator thermostat (VACT), zigbeeModel
// "EH-ZB-VACT". z2m-source: schneider.ts #EER53000.
//
// GRADUATED from generated/ (hand-edited — do NOT let the generator clobber).
// The auto-emit dropped the thermostat decode/encode entirely, leaving this
// battery TRV ported as a bare on/off switch (state/battery only). z2m
// decodes a full climate (fzLocal.wiser_smart_thermostat +
// tz.wiser_sed_occupied_heating_setpoint). Brought to parity with sibling
// EER51000 by wiring the generic thermostat converters so local_temperature /
// current_heating_setpoint / system_mode surface. Climate ships FLAT (project
// rule — no ExposeType::Climate). zone_mode / keypad_lockout / valve-calibration
// ride Schneider manuSpecific attrs (0x105E) — deferred (needs a Schneider
// manuSpecific converter not yet in tree).
//
// z2m v26.133.0 (R152), read side: window_open (hvacThermostat 0xE012, via the
// shared Schneider thermostat map; the valve reports it without the
// manufacturer code) and pi_heating_demand from the valve position 0xE030, only
// when <= 100 (larger values are raw calibration codes).
// Endpoint: the Wiser devices live on EP 11 (z2m device.getEndpoint(11)); the
// EP 1 binds failed and aborted configure. Binds and the default endpoint now
// use EP 11; the genOnOff bind is dropped (z2m binds genBasic / genPowerCfg /
// hvacThermostat only).
// DEFERRED (needs coordinator EP 11 emulation in firmware + a per-device queue
// flushed on the valve's next report): the bind destination EP 11, the
// setpoint push (cmd 0xE0 wiserSmartSetSetpoint), open_window_detection,
// calibrate_valve and keypad writes, and the custom read responses. The
// leftover kFzOnOff / kTzOnOff / `state` are pre-existing (z2m has no on/off).
#include "definitions/_generic/_shared.hpp"
#include "definitions/schneider/_shared.hpp"

namespace zhc::devices::schneider {
namespace {
// fzLocal.wiser_smart_thermostat: wiserSmartValvePosition (0xE030, u8) ->
// pi_heating_demand, dropped above 100.
bool fz_valve_position(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                       RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("57392");  // 0xE030
    if (!v || v->type != ValueType::Uint || v->u > 100) return false;
    return out.put("pi_heating_demand", *v);
}
constexpr FzConverter kFzValvePosition{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_valve_position },
    .user_config       = nullptr,
};

const FzConverter* const kFz_EER53000[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzThermostat,
    &::zhc::schneider::kFzSchneiderThermostat,
    &kFzValvePosition,
};
const TzConverter* const kTz_EER53000[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_EER53000[] = { "EH-ZB-VACT" };

}  // namespace


// exposes + bindings (originally emit_expose_bindings.py; hand-extended to
// match the wired thermostat converters above — mirrors EER51000).
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_open", ExposeType::Binary, Access::State, nullptr, "Open window detected by the valve", nullptr, 0},
    {"pi_heating_demand", ExposeType::Numeric, Access::State, "%", "Position of the valve (= demanded heat) where 0% is fully closed and 100% is fully open", nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {11, 0x0001},
    {11, 0x0201},
};

extern const PreparedDefinition kDef_EER53000{
    .zigbee_models=kModels_EER53000, .zigbee_models_count=sizeof(kModels_EER53000)/sizeof(kModels_EER53000[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EER53000", .vendor="Schneider",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EER53000, .from_zigbee_count=sizeof(kFz_EER53000)/sizeof(kFz_EER53000[0]),
    .to_zigbee=kTz_EER53000, .to_zigbee_count=sizeof(kTz_EER53000)/sizeof(kTz_EER53000[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .default_endpoint=11,
};

}  // namespace zhc::devices::schneider
