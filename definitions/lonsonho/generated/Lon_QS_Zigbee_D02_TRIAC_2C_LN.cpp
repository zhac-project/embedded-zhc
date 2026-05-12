// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lonsonho QS-Zigbee-D02-TRIAC-2C-LN — auto-generated.
// 2 gang smart dimmer switch module with neutral
// z2m-source: lonsonho.ts #QS-Zigbee-D02-TRIAC-2C-LN.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lonsonho {
namespace {
const FzConverter* const kFz_QS_Zigbee_D02_TRIAC_2C_LN[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_QS_Zigbee_D02_TRIAC_2C_LN[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_QS_Zigbee_D02_TRIAC_2C_LN[] = { "TS110F" };
constexpr const char* kManus_QS_Zigbee_D02_TRIAC_2C_LN[] = { "_TYZB01_v8gtiaed" };
constexpr ::zhc::EndpointLabel kEndpoints_QS_Zigbee_D02_TRIAC_2C_LN[] = { {"l1", 1}, {"l2", 2} };

// z2m: m.deviceEndpoints({l1:1,l2:2}) + tuya.modernExtend.tuyaLight({minBrightness:"attribute", endpointNames:["l1","l2"]})
constexpr Expose kExposes_QS_Zigbee_D02_TRIAC_2C_LN[] = {
    { "state_l1",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness_l1",     ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "min_brightness_l1", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l2",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness_l2",     ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "min_brightness_l2", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBindings_QS_Zigbee_D02_TRIAC_2C_LN[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 2, 0x0006 }, { 2, 0x0008 },
};
}  // namespace

extern const PreparedDefinition kDef_QS_Zigbee_D02_TRIAC_2C_LN{
    .zigbee_models=kModels_QS_Zigbee_D02_TRIAC_2C_LN, .zigbee_models_count=sizeof(kModels_QS_Zigbee_D02_TRIAC_2C_LN)/sizeof(kModels_QS_Zigbee_D02_TRIAC_2C_LN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QS_Zigbee_D02_TRIAC_2C_LN, .manufacturer_names_count=sizeof(kManus_QS_Zigbee_D02_TRIAC_2C_LN)/sizeof(kManus_QS_Zigbee_D02_TRIAC_2C_LN[0]),
    .model="QS-Zigbee-D02-TRIAC-2C-LN", .vendor="Lonsonho",
    .meta=nullptr, .exposes=kExposes_QS_Zigbee_D02_TRIAC_2C_LN, .exposes_count=sizeof(kExposes_QS_Zigbee_D02_TRIAC_2C_LN)/sizeof(kExposes_QS_Zigbee_D02_TRIAC_2C_LN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_QS_Zigbee_D02_TRIAC_2C_LN, .from_zigbee_count=sizeof(kFz_QS_Zigbee_D02_TRIAC_2C_LN)/sizeof(kFz_QS_Zigbee_D02_TRIAC_2C_LN[0]),
    .to_zigbee=kTz_QS_Zigbee_D02_TRIAC_2C_LN, .to_zigbee_count=sizeof(kTz_QS_Zigbee_D02_TRIAC_2C_LN)/sizeof(kTz_QS_Zigbee_D02_TRIAC_2C_LN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBindings_QS_Zigbee_D02_TRIAC_2C_LN,
    .bindings_count           = sizeof(kBindings_QS_Zigbee_D02_TRIAC_2C_LN)/sizeof(kBindings_QS_Zigbee_D02_TRIAC_2C_LN[0]),
    .endpoint_map       = kEndpoints_QS_Zigbee_D02_TRIAC_2C_LN,
    .endpoint_map_count = sizeof(kEndpoints_QS_Zigbee_D02_TRIAC_2C_LN)/sizeof(kEndpoints_QS_Zigbee_D02_TRIAC_2C_LN[0]),
};

}  // namespace zhc::devices::lonsonho
