// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Easyiot ZB-WB01 — graduated from generated/ for the action remap.
// 1-button remote control
// z2m-source: easyiot.ts #ZB-WB01 (+ fzLocal.easyiot_action).
//
// Battery-powered remote: it never receives genOnOff writes (read-only
// from the gateway perspective). It *emits* genOnOff On/Off/Toggle
// commands which z2m's `easyiot_action` fz remaps to
// `1_single` / `1_double` / `1_long`. The generic kFzCommandOn/Off/Toggle
// converters emit the *standard* literals ("on"/"off"/"toggle") with no
// button prefix — wrong verb AND no button identity — so this def wires
// the vendor kFzEasyiotAction converter (in easyiot/_shared.cpp) which
// reproduces the z2m verb map + per-button prefix from src_endpoint.
#include "definitions/easyiot/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_WB01[] = {
    &kFzEasyiotAction,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZB_WB01[] = { "ZB-WB01" };

constexpr Expose kExposes_ZB_WB01[] = {
    // z2m exposes e.action(["1_single","1_double","1_long"]); ZHC has no
    // enum expose type, so this stays String/State. kFzEasyiotAction
    // emits the matching "1_<single|double|long>" at runtime.
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
