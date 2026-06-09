// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Awox EPIR_Zm — PIR motion sensor, occupancy decode restored.
//
// Connect-Z motion (PIR) sensor. z2m models it as
//   m.battery() + m.occupancy() + m.commandsOnOff() + m.commandsLevelCtrl()
// so the device's PRIMARY signal is the msOccupancySensing (0x0406)
// attribute report. The generated def wired only kFzOnOff + kFzBattery
// and exposed state/battery/voltage — `occupancy` (the whole reason this
// is a sensor) was dropped, leaving the motion report dead. Re-wired with
// the generic kFzOccupancy decoder + an `occupancy` Binary expose +
// 0x0406 binding.
//
// Despite the "motion (PIR) sensor" label there is NO illuminance channel
// in z2m (no m.illuminance / msIlluminanceMeasurement) — the EGLO 99106
// is motion + battery only — so none is modelled.
//
// z2m's m.commandsOnOff/commandsLevelCtrl add command-action passthroughs
// (the PIR can drive a bound light directly); like the EBF_RGB_Zm light
// those action passthroughs are not surfaced here, but the existing
// kFzOnOff/state plumbing is harmless and retained.
//
// z2m-source: awox.ts #EPIR_Zm.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::awox {
namespace {
const FzConverter* const kFz_EPIR_Zm[] = {
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_EPIR_Zm[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_EPIR_Zm[] = { "EPIR_Zm" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0406},
    {1, 0x0006},
    {1, 0x0001},
};


constexpr WhiteLabel kWhiteLabels_EPIR_Zm[] = {
    {"EGLO","99106"},
};
extern const PreparedDefinition kDef_EPIR_Zm{
    .zigbee_models=kModels_EPIR_Zm, .zigbee_models_count=sizeof(kModels_EPIR_Zm)/sizeof(kModels_EPIR_Zm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EPIR_Zm", .vendor="Awox",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_EPIR_Zm, .white_labels_count=sizeof(kWhiteLabels_EPIR_Zm)/sizeof(kWhiteLabels_EPIR_Zm[0]),
    .from_zigbee=kFz_EPIR_Zm, .from_zigbee_count=sizeof(kFz_EPIR_Zm)/sizeof(kFz_EPIR_Zm[0]),
    .to_zigbee=kTz_EPIR_Zm, .to_zigbee_count=sizeof(kTz_EPIR_Zm)/sizeof(kTz_EPIR_Zm[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::awox
