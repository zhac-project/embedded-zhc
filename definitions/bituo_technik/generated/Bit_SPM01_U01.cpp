// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: BituoTechnik SPM01-U01 — auto-generated.
// Smart energy monitor for 1P+N system
// z2m-source: bituo_technik.ts #SPM01-U01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bituo_technik {
namespace {
const FzConverter* const kFz_SPM01_U01[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_SPM01_U01[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SPM01_U01[] = { "SPM01X001", "SPM01X", "SPM01-1Z2" };

}  // namespace


// Hand-extended 2026-04-28: 1P+N exposes per z2m TS extend
// (m.electricityMeter producedEnergy + acFrequency + powerFactor + power_apparent).
// White-label Zemismart SPM01-1Z2 not yet wired (TS has whiteLabel array).
// See BITUO_TECHNIK_PARITY.md for decode gaps.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"power_apparent", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"ac_frequency", ExposeType::Numeric, Access::State, "Hz", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end auto-generated block ---


constexpr WhiteLabel kWhiteLabels_SPM01_U01[] = {
    {"Zemismart","SPM01-1Z2"},
};
extern const PreparedDefinition kDef_SPM01_U01{
    .zigbee_models=kModels_SPM01_U01, .zigbee_models_count=sizeof(kModels_SPM01_U01)/sizeof(kModels_SPM01_U01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SPM01-U01", .vendor="BituoTechnik",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_SPM01_U01, .white_labels_count=sizeof(kWhiteLabels_SPM01_U01)/sizeof(kWhiteLabels_SPM01_U01[0]),
    .from_zigbee=kFz_SPM01_U01, .from_zigbee_count=sizeof(kFz_SPM01_U01)/sizeof(kFz_SPM01_U01[0]),
    .to_zigbee=kTz_SPM01_U01, .to_zigbee_count=sizeof(kTz_SPM01_U01)/sizeof(kTz_SPM01_U01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bituo_technik
