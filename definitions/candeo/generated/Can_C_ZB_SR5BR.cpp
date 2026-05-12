// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Candeo C-ZB-SR5BR — BLOCKED on candeoRotaryRemoteControl (0xFF03), see CANDEO_PARITY.md.
// Zigbee scene switch remote - 5 button rotary
// z2m-source: candeo.ts #C-ZB-SR5BR.
#include "definitions/_generic/_shared.hpp"


namespace zhc::devices::candeo {
namespace {
const FzConverter* const kFz_C_ZB_SR5BR[] = {
    &::zhc::generic::kFzBattery
};
constexpr const char* kModels_C_ZB_SR5BR[] = { "C-ZB-SR5BR" };

constexpr Expose kAutoExposes[] = {
    {"battery",           ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "mV",    nullptr, nullptr, 0}
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001}
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_SR5BR{
    .zigbee_models=kModels_C_ZB_SR5BR, .zigbee_models_count=sizeof(kModels_C_ZB_SR5BR)/sizeof(kModels_C_ZB_SR5BR[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="C-ZB-SR5BR", .vendor="Candeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_SR5BR, .from_zigbee_count=sizeof(kFz_C_ZB_SR5BR)/sizeof(kFz_C_ZB_SR5BR[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::candeo
