// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo T40S6Z.
// MixSwitch 6 gangs
// z2m-source: orvibo.ts #T40S6Z.
// BLOCKED — z2m fz.orvibo_raw_2 emits 6 button_N_click actions on a raw
// cluster. Same gap as CR11S8UZ. Stub only.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T40S6Z[] = {
    &::zhc::generic::kFzOnOff,
};
constexpr const char* kModels_T40S6Z[] = { "bcb949e87e8c4ea6bc2803052dd8fbf5" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_T40S6Z{
    .zigbee_models=kModels_T40S6Z, .zigbee_models_count=sizeof(kModels_T40S6Z)/sizeof(kModels_T40S6Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T40S6Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T40S6Z, .from_zigbee_count=sizeof(kFz_T40S6Z)/sizeof(kFz_T40S6Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
