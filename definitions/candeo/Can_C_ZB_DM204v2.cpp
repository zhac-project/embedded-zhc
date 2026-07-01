// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: hand-authored z2m v26.76.0 parity port (Phase A).
// Candeo C-ZB-DM204v2 — Zigbee micro smart dimmer (z2m v26.76.0 parity, new device).
//
// z2m-source: candeo.ts #C-ZB-DM204v2. Hardware v2 of C-ZB-DM204 with an
// identical converter surface. Primary function = dimmable light (genOnOff
// 0x0006 + genLevelCtrl 0x0008) plus electricity metering (seMetering 0x0702 +
// haElectricalMeasurement 0x0B04) and the Candeo manu-specific external switch
// type attribute. The manu-specific minimum_brightness_level attribute (custom
// genBasic attr, no generic converter) is DEFERRED — mirrors the in-tree
// C-ZB-DM204 def exactly. Fingerprint carries both the upper- and lower-case
// modelID spellings emitted by the hardware.
#include "definitions/_generic/_shared.hpp"
#include "definitions/candeo/_shared.hpp"

namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_DM204v2[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::candeo::kFzExternalSwitchType
};
const TzConverter* const kTz_C_ZB_DM204v2[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::candeo::kTzExternalSwitchType
};
constexpr const char* kModels_C_ZB_DM204v2[] = { "C-ZB-DM204V2", "C-ZB-DM204v2" };

constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",            ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",             ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0},
    {"current",           ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
    {"external_switch_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0702},
    {1, 0x0B04}
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_DM204v2{
    .zigbee_models=kModels_C_ZB_DM204v2, .zigbee_models_count=sizeof(kModels_C_ZB_DM204v2)/sizeof(kModels_C_ZB_DM204v2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-DM204v2", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_DM204v2, .from_zigbee_count=sizeof(kFz_C_ZB_DM204v2)/sizeof(kFz_C_ZB_DM204v2[0]),
    .to_zigbee=kTz_C_ZB_DM204v2, .to_zigbee_count=sizeof(kTz_C_ZB_DM204v2)/sizeof(kTz_C_ZB_DM204v2[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
