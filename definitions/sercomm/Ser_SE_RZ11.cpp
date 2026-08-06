// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Sercomm SE-RZ11 boiler control relay module, as shipped in the Enel
// Homix system (z2m v26.92.0 parity, added v26.78.0).
// z2m-source: sercomm.ts #SE-RZ11
//
// Plain genOnOff relay: upstream is `m.onOff({powerOnBehavior: false})`, so
// there is deliberately no power_on_behavior expose here either.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sercomm {
namespace {
const FzConverter* const kFz_SE_RZ11[] = { &::zhc::generic::kFzOnOff };
const TzConverter* const kTz_SE_RZ11[] = { &::zhc::generic::kTzOnOff };
constexpr const char* kModels_SE_RZ11[] = { "SE-RZ11" };

constexpr Expose kExposes_SE_RZ11[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, "Relay state", nullptr, 0},
};

constexpr BindingSpec kBindings_SE_RZ11[] = {
    {1, 0x0006},
};
}  // namespace

extern const PreparedDefinition kDef_SE_RZ11{
    .zigbee_models=kModels_SE_RZ11, .zigbee_models_count=sizeof(kModels_SE_RZ11)/sizeof(kModels_SE_RZ11[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SE-RZ11", .vendor="Sercomm",
    .meta=nullptr, .exposes=kExposes_SE_RZ11, .exposes_count=sizeof(kExposes_SE_RZ11)/sizeof(kExposes_SE_RZ11[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SE_RZ11, .from_zigbee_count=sizeof(kFz_SE_RZ11)/sizeof(kFz_SE_RZ11[0]),
    .to_zigbee=kTz_SE_RZ11, .to_zigbee_count=sizeof(kTz_SE_RZ11)/sizeof(kTz_SE_RZ11[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SE_RZ11, .bindings_count=sizeof(kBindings_SE_RZ11)/sizeof(kBindings_SE_RZ11[0]),
};

}  // namespace zhc::devices::sercomm
