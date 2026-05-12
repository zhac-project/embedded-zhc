// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lonsonho QS-Zigbee-D02-TRIAC-LN — auto-generated.
// 1 gang smart dimmer switch module with neutral
// z2m-source: lonsonho.ts #QS-Zigbee-D02-TRIAC-LN.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lonsonho {
namespace {
const FzConverter* const kFz_QS_Zigbee_D02_TRIAC_LN[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_QS_Zigbee_D02_TRIAC_LN[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_QS_Zigbee_D02_TRIAC_LN[] = { "TS110F" };
constexpr const char* kManus_QS_Zigbee_D02_TRIAC_LN[] = { "_TYZB01_qezuin6k" };

// z2m: tuya.modernExtend.tuyaLight({minBrightness: "attribute"})
constexpr Expose kExposes_QS_Zigbee_D02_TRIAC_LN[] = {
    { "state",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness",     ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "min_brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBindings_QS_Zigbee_D02_TRIAC_LN[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
}  // namespace

extern const PreparedDefinition kDef_QS_Zigbee_D02_TRIAC_LN{
    .zigbee_models=kModels_QS_Zigbee_D02_TRIAC_LN, .zigbee_models_count=sizeof(kModels_QS_Zigbee_D02_TRIAC_LN)/sizeof(kModels_QS_Zigbee_D02_TRIAC_LN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QS_Zigbee_D02_TRIAC_LN, .manufacturer_names_count=sizeof(kManus_QS_Zigbee_D02_TRIAC_LN)/sizeof(kManus_QS_Zigbee_D02_TRIAC_LN[0]),
    .model="QS-Zigbee-D02-TRIAC-LN", .vendor="Lonsonho",
    .meta=nullptr, .exposes=kExposes_QS_Zigbee_D02_TRIAC_LN, .exposes_count=sizeof(kExposes_QS_Zigbee_D02_TRIAC_LN)/sizeof(kExposes_QS_Zigbee_D02_TRIAC_LN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_QS_Zigbee_D02_TRIAC_LN, .from_zigbee_count=sizeof(kFz_QS_Zigbee_D02_TRIAC_LN)/sizeof(kFz_QS_Zigbee_D02_TRIAC_LN[0]),
    .to_zigbee=kTz_QS_Zigbee_D02_TRIAC_LN, .to_zigbee_count=sizeof(kTz_QS_Zigbee_D02_TRIAC_LN)/sizeof(kTz_QS_Zigbee_D02_TRIAC_LN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindings_QS_Zigbee_D02_TRIAC_LN,
    .bindings_count           = sizeof(kBindings_QS_Zigbee_D02_TRIAC_LN)/sizeof(kBindings_QS_Zigbee_D02_TRIAC_LN[0]),
};

}  // namespace zhc::devices::lonsonho
