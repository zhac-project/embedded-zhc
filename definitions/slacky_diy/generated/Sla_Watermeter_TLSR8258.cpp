// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy Watermeter_TLSR8258 — auto-generated.
// Water Meter
// z2m-source: slacky_diy.ts #Watermeter_TLSR8258.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {
const FzConverter* const kFz_Watermeter_TLSR8258[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzMetering,
};

constexpr const char* kModels_Watermeter_TLSR8258[] = { "Watermeter_TLSR8258" };

constexpr ::zhc::EndpointLabel kEndpoints_Watermeter_TLSR8258[] = { {"1", 1}, {"2", 2} };

}  // namespace


// z2m wires deviceEndpoints (5 EPs: hot/cold/leak/etc.), water_leak iasZoneAlarm, battery,
// switch_actions enumLookup on customSwitchActions, and m.numeric volume reads against
// seMetering currentSummDelivered on EP1 (hot) and EP2 (cold). Runtime kFzMetering decodes
// the seMetering 0x0000 attribute as `energy` for whichever endpoint emits it; per-endpoint
// hot/cold split is not modelled here.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"alarm", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "L", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
    {1, 0x0702},
    {2, 0x0702},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_Watermeter_TLSR8258{
    .zigbee_models=kModels_Watermeter_TLSR8258, .zigbee_models_count=sizeof(kModels_Watermeter_TLSR8258)/sizeof(kModels_Watermeter_TLSR8258[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Watermeter_TLSR8258", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Watermeter_TLSR8258, .from_zigbee_count=sizeof(kFz_Watermeter_TLSR8258)/sizeof(kFz_Watermeter_TLSR8258[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_Watermeter_TLSR8258,
    .endpoint_map_count = sizeof(kEndpoints_Watermeter_TLSR8258)/sizeof(kEndpoints_Watermeter_TLSR8258[0]),
};

}  // namespace zhc::devices::slacky_diy
