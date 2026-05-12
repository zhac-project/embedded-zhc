// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Icasa ICZB-KPD12 — hand-rewritten command-style 2-button keypad.
// z2m: fromZigbee=[command_on, command_off, command_move, command_stop, battery],
// toZigbee=[]. Auto-generator emitted kFzOnOff + kTzOnOff (state path) which
// is wrong — this is a remote: it emits cluster-specific commands, not
// attribute reports.
// z2m-source: icasa.ts #ICZB-KPD12.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::icasa {
namespace {
const FzConverter* const kFz_ICZB_KPD12[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

constexpr Expose kExposes_ICZB_KPD12[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ICZB_KPD12[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0001},  // genPowerCfg
};

constexpr const char* kModels_ICZB_KPD12[] = { "ICZB-KPD12" };

}  // namespace

extern const PreparedDefinition kDef_ICZB_KPD12{
    .zigbee_models=kModels_ICZB_KPD12,
    .zigbee_models_count=sizeof(kModels_ICZB_KPD12)/sizeof(kModels_ICZB_KPD12[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ICZB-KPD12", .vendor="Icasa",
    .meta=nullptr,
    .exposes=kExposes_ICZB_KPD12,
    .exposes_count=sizeof(kExposes_ICZB_KPD12)/sizeof(kExposes_ICZB_KPD12[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ICZB_KPD12,
    .from_zigbee_count=sizeof(kFz_ICZB_KPD12)/sizeof(kFz_ICZB_KPD12[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ICZB_KPD12,
    .bindings_count=sizeof(kBindings_ICZB_KPD12)/sizeof(kBindings_ICZB_KPD12[0]),
};

}  // namespace zhc::devices::icasa
