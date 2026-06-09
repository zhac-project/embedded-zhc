// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Osram 4058075816459 — hand-rewritten remote.
// SMART+ Switch (4-button wall remote, zigbeeModel "Switch 4x EU-LIGHTIFY" /
// "Switch 4x-LIGHTIFY" / "Switch-LIGHTIFY").
//
// Parity fix: battery-powered REMOTE. z2m wires fz.command_on/off,
// fz.command_step_color_temperature, fz.command_move/stop, fz.command_move_hue,
// fz.command_move_to_saturation and exposes a single `action` enum. The
// generated port instead lowered kFzOnOff (-> phantom `state`) and declared
// only a `state` expose, so all four buttons were dead. Re-pointed at the
// generic cluster-command action converters + an `action` enum (same pattern
// as the Switch Mini and iluminize/robb/legrand remote ports).
//
// Covers the z2m action set that maps to a wire command with a generic
// decoder: on, off, color_temperature_step_up/down, brightness_move_up/down,
// brightness_stop, hue_move/hue_stop. (z2m's move_to_saturation uses
// lightingColorCtrl cmd 0x03, which has no generic converter yet — deferred.)
//
// z2m-source: osram.ts #4058075816459.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::osram {
namespace {
const FzConverter* const kFz_D4058075816459[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzCommandMoveHue,
};

constexpr const char* kModels_D4058075816459[] = { "Switch 4x EU-LIGHTIFY", "Switch 4x-LIGHTIFY", "Switch-LIGHTIFY" };

constexpr Expose kExposes_D4058075816459[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D4058075816459[] = {
    {1, 0x0001}, {1, 0x0006}, {1, 0x0008}, {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D4058075816459{
    .zigbee_models=kModels_D4058075816459, .zigbee_models_count=sizeof(kModels_D4058075816459)/sizeof(kModels_D4058075816459[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4058075816459", .vendor="Osram",
    .meta=nullptr,
    .exposes=kExposes_D4058075816459,
    .exposes_count=sizeof(kExposes_D4058075816459)/sizeof(kExposes_D4058075816459[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D4058075816459, .from_zigbee_count=sizeof(kFz_D4058075816459)/sizeof(kFz_D4058075816459[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D4058075816459,
    .bindings_count=sizeof(kBindings_D4058075816459)/sizeof(kBindings_D4058075816459[0]),
};

}  // namespace zhc::devices::osram
