// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yandex YNDX_00537 — auto-generated.
// Single relay
// z2m-source: yandex.ts #YNDX_00537.
#include "definitions/_generic/_shared.hpp"
#include "definitions/yandex/_shared.hpp"

namespace zhc::devices::yandex {
namespace {
// Endpoints (z2m): {"1": 1, "": 2}. Single relay on EP1 + a default-name
// EP2 carrying genOnOff cluster commands surfaced as `action`.
const FzConverter* const kFz_YNDX_00537[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
};
const TzConverter* const kTz_YNDX_00537[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::yandex::kTzYxPowerType,
    &::zhc::yandex::kTzYxSwitchType,
    &::zhc::yandex::kTzYxLedIndicator,
};
constexpr const char* kModels_YNDX_00537[] = { "YNDX-00537" };

constexpr ::zhc::EndpointLabel kEndpoints_YNDX_00537[] = { {"1", 1} };

}  // namespace


// --- hand-authored exposes / bindings (parity sweep 2026-04-28) ---
constexpr Expose kAutoExposes[] = {
    {"state",         ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",        ExposeType::Enum,   Access::State,    nullptr, nullptr, nullptr, 0},
    {"power_type",    ExposeType::Enum,   Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_type",   ExposeType::Enum,   Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_indicator", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {1, 0xFC03},  // manuSpecificYandex
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00537{
    .zigbee_models=kModels_YNDX_00537, .zigbee_models_count=sizeof(kModels_YNDX_00537)/sizeof(kModels_YNDX_00537[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00537", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00537, .from_zigbee_count=sizeof(kFz_YNDX_00537)/sizeof(kFz_YNDX_00537[0]),
    .to_zigbee=kTz_YNDX_00537, .to_zigbee_count=sizeof(kTz_YNDX_00537)/sizeof(kTz_YNDX_00537[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_YNDX_00537,
    .endpoint_map_count = sizeof(kEndpoints_YNDX_00537)/sizeof(kEndpoints_YNDX_00537[0]),
};

}  // namespace zhc::devices::yandex
