// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Yandex YNDX_00534 — hand-rewritten (vendor-parity sweep 2026-04-28).
// Single gang wireless switch (battery-powered button, NO on-board relay).
// z2m emits `commandsOnOff` over two virtual endpoints {down: 1, up: 2} —
// the device sends genOnOff cluster commands which we surface as `action`
// (e.g. on/off/toggle). Pairs with battery.
// z2m-source: yandex.ts #YNDX-00534.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yandex {
namespace {
const FzConverter* const kFz_YNDX_00534[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_YNDX_00534[] = { "YNDX-00534" };

constexpr ::zhc::EndpointLabel kEndpoints_YNDX_00534[] = { {"down", 1}, {"up", 2} };

}  // namespace


// --- hand-authored exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff (down endpoint, action source)
    {2, 0x0006},  // genOnOff (up   endpoint, action source)
    {1, 0x0001},  // genPowerCfg
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00534{
    .zigbee_models=kModels_YNDX_00534, .zigbee_models_count=sizeof(kModels_YNDX_00534)/sizeof(kModels_YNDX_00534[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00534", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00534, .from_zigbee_count=sizeof(kFz_YNDX_00534)/sizeof(kFz_YNDX_00534[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_YNDX_00534,
    .endpoint_map_count = sizeof(kEndpoints_YNDX_00534)/sizeof(kEndpoints_YNDX_00534[0]),
};

}  // namespace zhc::devices::yandex
