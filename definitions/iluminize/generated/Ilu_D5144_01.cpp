// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 5144.01 — hand-rewritten battery-powered remote.
// RGB CCT 3 in 1 Remote Controller (zigbeeModel "ZGRC-KEY-005").
// z2m extend: m.battery({percentageReporting:false}), m.identify(),
//             m.commandsOnOff(), m.commandsLevelCtrl(), m.commandsColorCtrl().
// Pure remote: no controllable state, only action + battery.
// z2m-source: iluminize.ts #5144.01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

const FzConverter* const kFz_D5144_01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
};

constexpr Expose kExposes_D5144_01[] = {
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D5144_01[] = {
    { 1, 0x0001 }, { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};

constexpr const char* kModels_D5144_01[] = { "ZGRC-KEY-005" };

}  // namespace

extern const PreparedDefinition kDef_D5144_01{
    .zigbee_models=kModels_D5144_01,
    .zigbee_models_count=sizeof(kModels_D5144_01)/sizeof(kModels_D5144_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="5144.01", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposes_D5144_01,
    .exposes_count=sizeof(kExposes_D5144_01)/sizeof(kExposes_D5144_01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D5144_01,
    .from_zigbee_count=sizeof(kFz_D5144_01)/sizeof(kFz_D5144_01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D5144_01,
    .bindings_count=sizeof(kBindings_D5144_01)/sizeof(kBindings_D5144_01[0]),
};

}  // namespace zhc::devices::iluminize
