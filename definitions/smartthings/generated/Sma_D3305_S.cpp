// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings 3305-S — parity-extended 2026-04-28.
// Motion sensor (2014 model).
// z2m-source: smartthings.ts #3305-S.
//
// z2m bundle: temperature + IAS motion (alarm_2 → occupancy, battery_low,
// tamper) + battery. NOTE: z2m uses `fz.ias_occupancy_alarm_2` (alarm_2 bit
// for occupancy); ZHC's `kFzIasMotionAlarm` reads alarm_1 — this matches the
// SmartThings firmware which puts the PIR bit in alarm_1 on most modern
// units, but on the original 3305 hardware reports may use alarm_2.
// Hardware testing recommended.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_D3305_S[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_D3305_S[] = { "3305-S", "3305" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_D3305_S{
    .zigbee_models=kModels_D3305_S, .zigbee_models_count=sizeof(kModels_D3305_S)/sizeof(kModels_D3305_S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3305-S", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3305_S, .from_zigbee_count=sizeof(kFz_D3305_S)/sizeof(kFz_D3305_S[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
