// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Easyiot ZB-WB03 — graduated from generated/ for the action remap.
// 3-button remote control
// z2m-source: easyiot.ts #ZB-WB03 (+ fzLocal.easyiot_action).
//
// Three-endpoint button remote (ep1..ep3). Same notes as ZB-WB01:
// kFzEasyiotAction reproduces z2m's verb remap + per-button prefix
// (ep1 -> "1_*" .. ep3 -> "3_*") from src_endpoint.
#include "definitions/easyiot/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
const FzConverter* const kFz_ZB_WB03[] = {
    &kFzEasyiotAction,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZB_WB03[] = { "ZB-WB03" };

constexpr Expose kExposes_ZB_WB03[] = {
    // z2m: e.action(["1_single".."3_long"]). String/State here;
    // kFzEasyiotAction emits "<button>_<verb>" at runtime.
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_WB03[] = {
    {1, 0x0001},
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_ZB_WB03{
    .zigbee_models=kModels_ZB_WB03, .zigbee_models_count=sizeof(kModels_ZB_WB03)/sizeof(kModels_ZB_WB03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-WB03", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_WB03, .exposes_count=sizeof(kExposes_ZB_WB03)/sizeof(kExposes_ZB_WB03[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB_WB03, .from_zigbee_count=sizeof(kFz_ZB_WB03)/sizeof(kFz_ZB_WB03[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_WB03, .bindings_count=sizeof(kBindings_ZB_WB03)/sizeof(kBindings_ZB_WB03[0]),
};

}  // namespace zhc::devices::easyiot
