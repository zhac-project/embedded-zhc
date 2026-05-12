// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Trust ZSDR-850 — hand-rewritten for parity (smoke key).
// Smoke detector. z2m fz.ias_smoke_alarm_1 → emits `smoke` (bit 0).
// z2m-source: trust.ts #ZSDR-850.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::trust {
namespace {
const FzConverter* const kFz_ZSDR_850[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
};
constexpr const char* kModels_ZSDR_850[] = { "ZSDR-850", "SmokeSensor-EM" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"smoke",       ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_ZSDR_850{
    .zigbee_models=kModels_ZSDR_850, .zigbee_models_count=sizeof(kModels_ZSDR_850)/sizeof(kModels_ZSDR_850[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZSDR-850", .vendor="Trust",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZSDR_850, .from_zigbee_count=sizeof(kFz_ZSDR_850)/sizeof(kFz_ZSDR_850[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::trust
