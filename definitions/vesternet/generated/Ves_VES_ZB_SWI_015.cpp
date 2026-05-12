// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-SWI-015 — hand-curated.
// Zigbee 2-channel switch with multi-EP demux ({l1:1, l2:2}) and
// power_on_behavior. Energy/Power/Voltage/Current report on EP1 only,
// per z2m exposes (no .withEndpoint() on those scalars).
// z2m-source: vesternet.ts #VES-ZB-SWI-015.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_SWI_015[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_VES_ZB_SWI_015[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_VES_ZB_SWI_015[] = { "ON/OFF(2CH)" };

// Multi-endpoint label map. The dispatcher suffixes Fz keys with `_l1`/
// `_l2` for genOnOff reports on the matching endpoint, and the adapter
// strips the suffix from outbound `state_l1` / `state_l2` Tz keys.
constexpr ::zhc::EndpointLabel kEndpoints_VES_ZB_SWI_015[] = {
    {"l1", 1}, {"l2", 2},
};

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end ---

extern const PreparedDefinition kDef_VES_ZB_SWI_015{
    .zigbee_models=kModels_VES_ZB_SWI_015, .zigbee_models_count=sizeof(kModels_VES_ZB_SWI_015)/sizeof(kModels_VES_ZB_SWI_015[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-SWI-015", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VES_ZB_SWI_015, .from_zigbee_count=sizeof(kFz_VES_ZB_SWI_015)/sizeof(kFz_VES_ZB_SWI_015[0]),
    .to_zigbee=kTz_VES_ZB_SWI_015, .to_zigbee_count=sizeof(kTz_VES_ZB_SWI_015)/sizeof(kTz_VES_ZB_SWI_015[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=nullptr, .reports_count=0,
    .config_steps=nullptr, .config_steps_count=0,
    .config_callbacks=nullptr, .config_callbacks_count=0,
    .endpoint_map=kEndpoints_VES_ZB_SWI_015,
    .endpoint_map_count=sizeof(kEndpoints_VES_ZB_SWI_015)/sizeof(kEndpoints_VES_ZB_SWI_015[0]),
};

}  // namespace zhc::devices::vesternet
