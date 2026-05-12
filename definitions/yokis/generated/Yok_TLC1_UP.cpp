// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yokis TLC1-UP — parity-extended 2026-04-28.
// Tabletop Design series 1-button remote control
// z2m-source: yokis.ts #TLC1-UP.
//
// z2m extend: deviceEndpoints({ep1:1}), identify, commandsOnOff/LevelCtrl/WindowCovering,
//             temperature, battery, YokisDeviceExtend, YokisInputExtend, YokisChannelExtend,
//             YokisPilotWireExtend, YokisTemperatureMeasurementExtend.
// Runtime gap: manuSpecificYokis* clusters (Pilot Wire, Temperature, Channel, Input)
//   not yet implemented in ZHC. msTemperatureMeasurement (0x0402) is the standard
//   cluster and is decoded.
// TODO 2026-04-28: wire manuSpecificYokis* converters from
//   definitions/yokis/_shared.hpp (kFzYokis{Input,Entry,Channel,LightControl,
//   Dimmer,SubSystem,PilotWire} + matching kTz*) — see Yok_MTR500E_UP.cpp /
//   Yok_TLC2_UP.cpp / Yok_MFP_UP.cpp for canonical wiring patterns.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yokis {
namespace {
const FzConverter* const kFz_TLC1_UP[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
};
const TzConverter* const kTz_TLC1_UP[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_TLC1_UP[] = { "TLC1-UP" };

constexpr Expose kAutoExposes[] = {
    {"state",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State,    "mV",    nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State,    "°C",    nullptr, nullptr, 0},
    {"action",      ExposeType::Enum,    Access::State,    nullptr, "Triggered button action", nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0102},
    {1, 0x0001}, {1, 0x0402},
};
}  // namespace

extern const PreparedDefinition kDef_TLC1_UP{
    .zigbee_models=kModels_TLC1_UP, .zigbee_models_count=sizeof(kModels_TLC1_UP)/sizeof(kModels_TLC1_UP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TLC1-UP", .vendor="Yokis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TLC1_UP, .from_zigbee_count=sizeof(kFz_TLC1_UP)/sizeof(kFz_TLC1_UP[0]),
    .to_zigbee=kTz_TLC1_UP, .to_zigbee_count=sizeof(kTz_TLC1_UP)/sizeof(kTz_TLC1_UP[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yokis
