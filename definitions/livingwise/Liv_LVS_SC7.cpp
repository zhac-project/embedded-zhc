// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: LivingWise LVS-SC7 — graduated from generated/ to decode button actions.
// Scene controller
// z2m-source: livingwise.ts #LVS-SC7 (fz.orvibo_raw_2).
//
// The generated stub wired the generic kFzOnOff/kTzOnOff behind a `state`
// StateSet expose, mis-porting a stateless 7-button scene controller as a
// controllable on/off switch (dead state — the device has no genOnOff
// relay). z2m treats LVS-SC7 as action-only: fz.orvibo_raw_2 decodes a
// vendor-private raw frame on cluster 0x0017 into `action` =
// "button_<n>_<click|hold|release>" for buttons 1..7.
//
// Reuses the already-implemented shared kFzOrviboRaw2Action converter
// (orvibo/_shared.cpp) — identical wire shape, identity button lookup
// {1..7}, action byte {0:click,2:hold,3:release}. The dead `state`
// expose + kTzOnOff are dropped; `action` (String) is added. The 0x0006
// binding is kept (harmless; matches the generated baseline + T40S6Z).
#include "definitions/_generic/_shared.hpp"
#include "definitions/orvibo/_shared.hpp"

namespace zhc::devices::livingwise {
namespace {
const FzConverter* const kFz_LVS_SC7[] = {
    &::zhc::orvibo::kFzOrviboRaw2Action,
};
constexpr const char* kModels_LVS_SC7[] = { "75d430d66c164c26ac8601c05932dc94" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    // z2m exposes e.action(["button_1_click".."button_7_release"]); ZHC has
    // no enum/list action type, so this stays String/State. The action
    // strings are produced by kFzOrviboRaw2Action.
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_LVS_SC7{
    .zigbee_models=kModels_LVS_SC7, .zigbee_models_count=sizeof(kModels_LVS_SC7)/sizeof(kModels_LVS_SC7[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LVS-SC7", .vendor="LivingWise",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LVS_SC7, .from_zigbee_count=sizeof(kFz_LVS_SC7)/sizeof(kFz_LVS_SC7[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livingwise
