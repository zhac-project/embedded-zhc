// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yandex YNDX_00531 — auto-generated.
// Single gang switch
// z2m-source: yandex.ts #YNDX_00531.
#include "definitions/_generic/_shared.hpp"
#include "definitions/yandex/_shared.hpp"  // mfg config writes

namespace zhc::devices::yandex {
namespace {
// Endpoints (z2m): {"1": 1, down: 2, up: 3} — relay on 1, decoupled buttons
// surface as `action` from genOnOff cmds bound on EP2/EP3.
const FzConverter* const kFz_YNDX_00531[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
};
const TzConverter* const kTz_YNDX_00531[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::yandex::kTzYxPowerType,
    &::zhc::yandex::kTzYxSwitchMode,
    &::zhc::yandex::kTzYxLedIndicator,
};
constexpr const char* kModels_YNDX_00531[] = { "YNDX-00531" };

constexpr ::zhc::EndpointLabel kEndpoints_YNDX_00531[] = { {"down", 2}, {"up", 3} };

}  // namespace


// --- hand-authored exposes / bindings (parity sweep 2026-04-28) ---
constexpr Expose kAutoExposes[] = {
    {"state",          ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",         ExposeType::Enum,   Access::State,    nullptr, nullptr, nullptr, 0},
    {"power_type",     ExposeType::Enum,   Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"operation_mode", ExposeType::Enum,   Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_indicator",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff (relay)
    {2, 0x0006},  // genOnOff (down — action)
    {3, 0x0006},  // genOnOff (up   — action)
    {1, 0xFC03},  // manuSpecificYandex
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00531{
    .zigbee_models=kModels_YNDX_00531, .zigbee_models_count=sizeof(kModels_YNDX_00531)/sizeof(kModels_YNDX_00531[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00531", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00531, .from_zigbee_count=sizeof(kFz_YNDX_00531)/sizeof(kFz_YNDX_00531[0]),
    .to_zigbee=kTz_YNDX_00531, .to_zigbee_count=sizeof(kTz_YNDX_00531)/sizeof(kTz_YNDX_00531[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_YNDX_00531,
    .endpoint_map_count = sizeof(kEndpoints_YNDX_00531)/sizeof(kEndpoints_YNDX_00531[0]),
};

}  // namespace zhc::devices::yandex
