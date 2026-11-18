// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Third Reality 3RRE0194Z smart outdoor repeater (z2m v26.125.0 window).
// z2m v26.126.0..v26.133.0 renamed it from 3ROR0194Z and replaced the
// temperature extend with on/off: m.onOff() (power_on_behavior on) + m.battery()
// (percentage only). The old modelID stays listed for units already paired.
// z2m-source: third_reality.ts #3RRE0194Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::third_reality {
namespace {
const FzConverter* const kFz_D3ROR0194Z[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_D3ROR0194Z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_D3ROR0194Z[] = { "3RRE0194Z", "3ROR0194Z" };

constexpr Expose kExposes_D3ROR0194Z[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D3ROR0194Z[] = {
    {1, 0x0006},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D3ROR0194Z{
    .zigbee_models=kModels_D3ROR0194Z, .zigbee_models_count=sizeof(kModels_D3ROR0194Z)/sizeof(kModels_D3ROR0194Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3RRE0194Z", .vendor="Third Reality",
    .meta=nullptr, .exposes=kExposes_D3ROR0194Z, .exposes_count=sizeof(kExposes_D3ROR0194Z)/sizeof(kExposes_D3ROR0194Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3ROR0194Z, .from_zigbee_count=sizeof(kFz_D3ROR0194Z)/sizeof(kFz_D3ROR0194Z[0]),
    .to_zigbee=kTz_D3ROR0194Z, .to_zigbee_count=sizeof(kTz_D3ROR0194Z)/sizeof(kTz_D3ROR0194Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D3ROR0194Z, .bindings_count=sizeof(kBindings_D3ROR0194Z)/sizeof(kBindings_D3ROR0194Z[0]),
};

}  // namespace zhc::devices::third_reality
