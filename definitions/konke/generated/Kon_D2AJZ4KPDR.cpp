// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Konke 2AJZ4KPDR — IAS contact sensor (typed).
// z2m-source: konke.ts #2AJZ4KPDR (fz.ias_contact_alarm_1).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::konke {
namespace {
const FzConverter* const kFz_D2AJZ4KPDR[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_D2AJZ4KPDR[] = { "3AFE130104020015", "3AFE270104020015", "3AFE280104020015" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact",     ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_D2AJZ4KPDR{
    .zigbee_models=kModels_D2AJZ4KPDR, .zigbee_models_count=sizeof(kModels_D2AJZ4KPDR)/sizeof(kModels_D2AJZ4KPDR[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="2AJZ4KPDR", .vendor="Konke",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D2AJZ4KPDR, .from_zigbee_count=sizeof(kFz_D2AJZ4KPDR)/sizeof(kFz_D2AJZ4KPDR[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::konke
