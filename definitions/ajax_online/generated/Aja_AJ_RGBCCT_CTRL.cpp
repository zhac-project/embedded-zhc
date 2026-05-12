// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: AjaxOnline AJ_RGBCCT_CTRL — auto-generated.
// Smart Zigbee LED strip RGB+CCT
// z2m-source: ajax_online.ts #AJ_RGBCCT_CTRL.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ajax_online {
namespace {


constexpr const char* kModels_AJ_RGBCCT_CTRL[] = { "TS0505B" };
constexpr const char* kManus_AJ_RGBCCT_CTRL[] = { "_TZ3210_hzy4rjz3" };
}  // namespace


namespace {
const FzConverter* const kFz_min_AJ_RGBCCT_CTRL[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_AJ_RGBCCT_CTRL[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_AJ_RGBCCT_CTRL[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_AJ_RGBCCT_CTRL{
    .zigbee_models=kModels_AJ_RGBCCT_CTRL, .zigbee_models_count=sizeof(kModels_AJ_RGBCCT_CTRL)/sizeof(kModels_AJ_RGBCCT_CTRL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_AJ_RGBCCT_CTRL, .manufacturer_names_count=sizeof(kManus_AJ_RGBCCT_CTRL)/sizeof(kManus_AJ_RGBCCT_CTRL[0]),
    .model="AJ_RGBCCT_CTRL", .vendor="AjaxOnline",
    .meta=nullptr, .exposes=kExp_min_AJ_RGBCCT_CTRL, .exposes_count=sizeof(kExp_min_AJ_RGBCCT_CTRL)/sizeof(kExp_min_AJ_RGBCCT_CTRL[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_AJ_RGBCCT_CTRL, .from_zigbee_count=sizeof(kFz_min_AJ_RGBCCT_CTRL)/sizeof(kFz_min_AJ_RGBCCT_CTRL[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_AJ_RGBCCT_CTRL,
    .bindings_count           = sizeof(kBind_min_AJ_RGBCCT_CTRL)/sizeof(kBind_min_AJ_RGBCCT_CTRL[0]),
};

}  // namespace zhc::devices::ajax_online
