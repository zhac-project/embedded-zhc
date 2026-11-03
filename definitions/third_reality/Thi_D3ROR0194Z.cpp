// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Third Reality 3ROR0194Z smart outdoor repeater (z2m v26.125.0 window).
// m.temperature() + m.battery() (percentage only, no voltage).
// z2m-source: third_reality.ts #3ROR0194Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::third_reality {
namespace {
const FzConverter* const kFz_D3ROR0194Z[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D3ROR0194Z[] = { "3ROR0194Z" };

constexpr Expose kExposes_D3ROR0194Z[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D3ROR0194Z[] = {
    {1, 0x0402},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D3ROR0194Z{
    .zigbee_models=kModels_D3ROR0194Z, .zigbee_models_count=sizeof(kModels_D3ROR0194Z)/sizeof(kModels_D3ROR0194Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3ROR0194Z", .vendor="Third Reality",
    .meta=nullptr, .exposes=kExposes_D3ROR0194Z, .exposes_count=sizeof(kExposes_D3ROR0194Z)/sizeof(kExposes_D3ROR0194Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3ROR0194Z, .from_zigbee_count=sizeof(kFz_D3ROR0194Z)/sizeof(kFz_D3ROR0194Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D3ROR0194Z, .bindings_count=sizeof(kBindings_D3ROR0194Z)/sizeof(kBindings_D3ROR0194Z[0]),
};

}  // namespace zhc::devices::third_reality
