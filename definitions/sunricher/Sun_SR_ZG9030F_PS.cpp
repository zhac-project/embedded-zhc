// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sunricher SR-ZG9030F-PS — graduated from generated/ to fix a real parity gap.
// Smart human presence sensor.
// z2m-source: sunricher.ts #SR-ZG9030F-PS — m.occupancy(), m.illuminance({scale: identity}),
//   m.commandsOnOff(), + custom sunricherSensor cluster (config attrs).
//
// PARITY FIX (wrong device class): the auto-generated port modelled this presence
// sensor as a controllable on/off RELAY (genOnOff server, kFzOnOff/kTzOnOff, expose
// "state"). The device has no relay — its core outputs are occupancy
// (msOccupancySensing) and illuminance (msIlluminanceMeasurement). Replace the
// relay with kFzOccupancy + kFzIlluminance and expose `occupancy` + `illuminance`
// to match z2m. The generic kFzIlluminance passes the raw measuredValue, identical
// to z2m's `scale: (value) => value`.
//
// DEFERRED (infra, not a device-function gap): m.commandsOnOff() surfaces emitted
// on/off commands as an `action` — there is no generic genOnOff-command->action
// producer. The custom sunricherSensor cluster (indicator light / detection area /
// illuminance threshold) is a manuSpecific config surface with no generic converter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sunricher {
namespace {
const FzConverter* const kFz_SR_ZG9030F_PS[] = {
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
};
constexpr const char* kModels_SR_ZG9030F_PS[] = { "HK-SENSOR-PRE" };

}  // namespace

constexpr Expose kExposes_SR_ZG9030F_PS[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SR_ZG9030F_PS[] = {
    {1, 0x0406},  // msOccupancySensing
    {1, 0x0400},  // msIlluminanceMeasurement
};

extern const PreparedDefinition kDef_SR_ZG9030F_PS{
    .zigbee_models=kModels_SR_ZG9030F_PS, .zigbee_models_count=sizeof(kModels_SR_ZG9030F_PS)/sizeof(kModels_SR_ZG9030F_PS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SR-ZG9030F-PS", .vendor="Sunricher",
    .meta=nullptr, .exposes=kExposes_SR_ZG9030F_PS, .exposes_count=sizeof(kExposes_SR_ZG9030F_PS)/sizeof(kExposes_SR_ZG9030F_PS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SR_ZG9030F_PS, .from_zigbee_count=sizeof(kFz_SR_ZG9030F_PS)/sizeof(kFz_SR_ZG9030F_PS[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_SR_ZG9030F_PS,.bindings_count=sizeof(kBindings_SR_ZG9030F_PS)/sizeof(kBindings_SR_ZG9030F_PS[0]),
};

}  // namespace zhc::devices::sunricher
