// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Pulsar LLC LPC-V1 leak protection controller
// (z2m v26.92.0 parity, added v26.82.0).
// z2m-source: pulsar.ts #LPC-V1
//
// Two halves: an IAS Zone water-leak sensor, and four relay outputs on
// endpoints 2..5 that drive the shut-off valves. kFzIasWaterLeakAlarm emits
// water_leak plus the tamper / battery_low zone bits.
//
// Upstream also reads zone alarm_2; ZHC's water-leak labeller reports the
// primary alarm bit only, which is the leak itself.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pulsar {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasWaterLeakAlarm,
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz[] = { &::zhc::generic::kTzOnOff };
constexpr const char* kModels[] = { "LPC-V1" };

constexpr Expose kExposes[] = {
    {"water_leak",  ExposeType::Binary, Access::State,    nullptr, "Water leak detected", nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State,    nullptr, "Tamper detected",     nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State,    nullptr, "Battery low",         nullptr, 0},
    {"state",       ExposeType::Binary, Access::StateSet, nullptr, "Valve output state",  nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0500},
    {2, 0x0006},
    {3, 0x0006},
    {4, 0x0006},
    {5, 0x0006},
};
}  // namespace

extern const PreparedDefinition kDef_LPC_V1{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LPC-V1", .vendor="Pulsar LLC",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};

}  // namespace zhc::devices::pulsar
