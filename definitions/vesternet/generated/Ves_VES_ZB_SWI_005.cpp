// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-SWI-005 — hand-curated.
// Zigbee switch — adds power_on_behavior over the auto-stub.
// z2m-source: vesternet.ts #VES-ZB-SWI-005.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_SWI_005[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_VES_ZB_SWI_005[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_VES_ZB_SWI_005[] = { "HK-SL-RELAY-A" };

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};
// --- end ---


constexpr WhiteLabel kWhiteLabels_VES_ZB_SWI_005[] = {
    {"Sunricher","SR-ZG9100A-S"},
};
extern const PreparedDefinition kDef_VES_ZB_SWI_005{
    .zigbee_models=kModels_VES_ZB_SWI_005, .zigbee_models_count=sizeof(kModels_VES_ZB_SWI_005)/sizeof(kModels_VES_ZB_SWI_005[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-SWI-005", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_VES_ZB_SWI_005, .white_labels_count=sizeof(kWhiteLabels_VES_ZB_SWI_005)/sizeof(kWhiteLabels_VES_ZB_SWI_005[0]),
    .from_zigbee=kFz_VES_ZB_SWI_005, .from_zigbee_count=sizeof(kFz_VES_ZB_SWI_005)/sizeof(kFz_VES_ZB_SWI_005[0]),
    .to_zigbee=kTz_VES_ZB_SWI_005, .to_zigbee_count=sizeof(kTz_VES_ZB_SWI_005)/sizeof(kTz_VES_ZB_SWI_005[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vesternet
