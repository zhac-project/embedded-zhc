// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-WB01 — auto-generated.
// 1-button remote control
// z2m-source: easyiot.ts #ZB-WB01.
//
// Battery-powered remote: it never receives genOnOff writes (read-only
// from the gateway perspective). It *emits* genOnOff On/Off/Toggle
// commands which z2m's `easyiot_action` fz turns into
// `1_single` / `1_double` / `1_long`. ZHC has no command->action
// label mapper yet, so the raw command FzConverters are wired here
// and the actual `action` string is left for the runtime.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_WB01[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZB_WB01[] = { "ZB-WB01" };

constexpr Expose kExposes_ZB_WB01[] = {
    // TODO(easyiot): runtime needs to map genOnOff
    // commandOn/Off/Toggle into action strings
    // ("1_single"/"1_double"/"1_long"). Exposed as String/State here
    // for forward-compat — currently no fz emits it.
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_WB01[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff (button command source)
};

}  // namespace

extern const PreparedDefinition kDef_ZB_WB01{
    .zigbee_models=kModels_ZB_WB01, .zigbee_models_count=sizeof(kModels_ZB_WB01)/sizeof(kModels_ZB_WB01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-WB01", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_WB01, .exposes_count=sizeof(kExposes_ZB_WB01)/sizeof(kExposes_ZB_WB01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_WB01, .from_zigbee_count=sizeof(kFz_ZB_WB01)/sizeof(kFz_ZB_WB01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_WB01, .bindings_count=sizeof(kBindings_ZB_WB01)/sizeof(kBindings_ZB_WB01[0]),
};

}  // namespace zhc::devices::easyiot
