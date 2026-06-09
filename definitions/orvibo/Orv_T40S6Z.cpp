// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Orvibo T40S6Z — graduated from generated/ to decode button actions.
// MixSwitch 6 gangs
// z2m-source: orvibo.ts #T40S6Z (fz.orvibo_raw_2).
//
// The generated stub wired the generic kFzOnOff behind a `state` expose,
// but z2m treats this as a stateless scene remote: `fz.orvibo_raw_2`
// decodes a vendor-private raw frame on cluster 0x0017 into `action` =
// "button_<n>_<click|hold|release>" (z2m exposes button_1..6_click).
// Wired the shared kFzOrviboRaw2Action converter + `action` expose; the
// dead `state` is dropped. See orvibo/_shared.{hpp,cpp}.
#include "definitions/_generic/_shared.hpp"
#include "definitions/orvibo/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T40S6Z[] = {
    &::zhc::orvibo::kFzOrviboRaw2Action,
};
constexpr const char* kModels_T40S6Z[] = { "bcb949e87e8c4ea6bc2803052dd8fbf5" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    // z2m exposes e.action(["button_1_click".."button_6_click"]); ZHC has
    // no enum/list action type, so this stays String/State. The action
    // strings are produced by kFzOrviboRaw2Action.
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
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
