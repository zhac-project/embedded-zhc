// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yandex YNDX_00538 — auto-generated.
// Double relay
// z2m-source: yandex.ts #YNDX_00538.
#include "definitions/_generic/_shared.hpp"
#include "definitions/yandex/_shared.hpp"

namespace zhc::devices::yandex {
namespace {
// Endpoints (z2m): {"1": 1, "2": 2, b1: 3, b2: 4}. Two relays + two decoupled
// button endpoints. switch_type is per-relay in z2m (EP1/EP2); we register
// one converter (per-EP split is a follow-up — see docs/YANDEX_PARITY.md).
const FzConverter* const kFz_YNDX_00538[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
};
const TzConverter* const kTz_YNDX_00538[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::yandex::kTzYxPowerType,
    &::zhc::yandex::kTzYxSwitchType,
    &::zhc::yandex::kTzYxInterlock,
    &::zhc::yandex::kTzYxLedIndicator,
};
constexpr const char* kModels_YNDX_00538[] = { "YNDX-00538" };

constexpr ::zhc::EndpointLabel kEndpoints_YNDX_00538[] = { {"b1", 3}, {"b2", 4} };

}  // namespace


// --- hand-authored exposes / bindings (parity sweep 2026-04-28) ---
constexpr Expose kAutoExposes[] = {
    {"state",         ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",        ExposeType::Enum,   Access::State,    nullptr, nullptr, nullptr, 0},
    {"power_type",    ExposeType::Enum,   Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_type",   ExposeType::Enum,   Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"interlock",     ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_indicator", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
    {4, 0x0006},
    {1, 0xFC03},  // manuSpecificYandex
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00538{
    .zigbee_models=kModels_YNDX_00538, .zigbee_models_count=sizeof(kModels_YNDX_00538)/sizeof(kModels_YNDX_00538[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00538", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00538, .from_zigbee_count=sizeof(kFz_YNDX_00538)/sizeof(kFz_YNDX_00538[0]),
    .to_zigbee=kTz_YNDX_00538, .to_zigbee_count=sizeof(kTz_YNDX_00538)/sizeof(kTz_YNDX_00538[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_YNDX_00538,
    .endpoint_map_count = sizeof(kEndpoints_YNDX_00538)/sizeof(kEndpoints_YNDX_00538[0]),
};

}  // namespace zhc::devices::yandex
