// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-HLD-017 — hand-curated.
// Zigbee high load switch — adds power_on_behavior over the auto-stub.
// z2m-source: vesternet.ts #VES-ZB-HLD-017.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_HLD_017[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_VES_ZB_HLD_017[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
// z2m fingerprint matches modelID="ON/OFF -M" with softwareBuildIDs r54/r55/r56.
// We match on modelID alone (softwareBuildID isn't part of v1 selectors).
constexpr const char* kModels_VES_ZB_HLD_017[] = { "ON/OFF -M" };

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end ---


constexpr WhiteLabel kWhiteLabels_VES_ZB_HLD_017[] = {
    {"Sunricher","SR-ZG9101SAC-HP-SWITCH-B"},
};
extern const PreparedDefinition kDef_VES_ZB_HLD_017{
    .zigbee_models=kModels_VES_ZB_HLD_017, .zigbee_models_count=sizeof(kModels_VES_ZB_HLD_017)/sizeof(kModels_VES_ZB_HLD_017[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-HLD-017", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_VES_ZB_HLD_017, .white_labels_count=sizeof(kWhiteLabels_VES_ZB_HLD_017)/sizeof(kWhiteLabels_VES_ZB_HLD_017[0]),
    .from_zigbee=kFz_VES_ZB_HLD_017, .from_zigbee_count=sizeof(kFz_VES_ZB_HLD_017)/sizeof(kFz_VES_ZB_HLD_017[0]),
    .to_zigbee=kTz_VES_ZB_HLD_017, .to_zigbee_count=sizeof(kTz_VES_ZB_HLD_017)/sizeof(kTz_VES_ZB_HLD_017[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vesternet
