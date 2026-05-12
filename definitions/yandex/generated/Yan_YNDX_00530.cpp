// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yandex YNDX_00530 — auto-generated.
// Dimmer
// z2m-source: yandex.ts #YNDX_00530.
#include "definitions/_generic/_shared.hpp"
#include "definitions/yandex/_shared.hpp"  // kTzYxLedIndicator, kTzYxButtonMode

namespace zhc::devices::yandex {
namespace {
const FzConverter* const kFz_YNDX_00530[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_YNDX_00530[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::yandex::kTzYxLedIndicator,
    &::zhc::yandex::kTzYxButtonMode,
};
constexpr const char* kModels_YNDX_00530[] = { "YNDX-00530" };

}  // namespace


// --- hand-authored exposes / bindings (parity sweep 2026-04-28) ---
// Light + dimmer plus two manuSpecificYandex config writes. lightingBallast
// (min/max level) is NOT yet wired — needs cluster 0x0301 helpers.
constexpr Expose kAutoExposes[] = {
    {"state",         ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_indicator", ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"button_mode",   ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0xFC03},  // manuSpecificYandex
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00530{
    .zigbee_models=kModels_YNDX_00530, .zigbee_models_count=sizeof(kModels_YNDX_00530)/sizeof(kModels_YNDX_00530[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00530", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00530, .from_zigbee_count=sizeof(kFz_YNDX_00530)/sizeof(kFz_YNDX_00530[0]),
    .to_zigbee=kTz_YNDX_00530, .to_zigbee_count=sizeof(kTz_YNDX_00530)/sizeof(kTz_YNDX_00530[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yandex
