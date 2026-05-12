// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Schneider E8332SCN300ZB — uses shared schneider converters.
// Wiser AvatarOn 2G curtain switch
// z2m-source: schneider.ts #E8332SCN300ZB.
// Indicator + 2-channel motor type/curtain-status wired via Visa-config
// helpers. Cover open/close uses generic windowCovering on l1=10, l2=11.
// TODO(parity): visaWiserCurtain custom commands (UpOpen/DnClose/Stop on
// the Visa cluster) are not yet routed through ZHC.
#include "definitions/_generic/_shared.hpp"
#include "definitions/schneider/_shared.hpp"

namespace zhc::devices::schneider {
namespace {
const FzConverter* const kFz_E8332SCN300ZB[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::schneider::kFzSchneiderVisaCurtain2,
};
const TzConverter* const kTz_E8332SCN300ZB[] = {
    &::zhc::generic::kTzCoverPosition,
    &::zhc::schneider::kTzSchneiderIndicatorLuminance,
    &::zhc::schneider::kTzSchneiderIndicatorColor,
    &::zhc::schneider::kTzSchneiderIndicatorMode,
    &::zhc::schneider::kTzSchneiderMotorTypeCh1,
    &::zhc::schneider::kTzSchneiderMotorTypeCh2,
    &::zhc::schneider::kTzSchneiderCurtainStatusCh1,
    &::zhc::schneider::kTzSchneiderCurtainStatusCh2,
};

constexpr const char* kModels_E8332SCN300ZB[] = { "E8332SCN300ZB" };

constexpr Expose kExposes_E8332SCN300ZB[] = {
    {"position",      ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"indicator_luminance_level", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"indicator_color", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"indicator_mode",  ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"motor_type_1",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"motor_type_2",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"curtain_status_1", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"curtain_status_2", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E8332SCN300ZB[] = {
    {10, 0x0102},
    {11, 0x0102},
};

constexpr ::zhc::EndpointLabel kEndpoints_E8332SCN300ZB[] = { {"l1", 10}, {"l2", 11} };

}  // namespace

extern const PreparedDefinition kDef_E8332SCN300ZB{
    .zigbee_models=kModels_E8332SCN300ZB, .zigbee_models_count=sizeof(kModels_E8332SCN300ZB)/sizeof(kModels_E8332SCN300ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E8332SCN300ZB", .vendor="Schneider",
    .meta=nullptr, .exposes=kExposes_E8332SCN300ZB, .exposes_count=sizeof(kExposes_E8332SCN300ZB)/sizeof(kExposes_E8332SCN300ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E8332SCN300ZB, .from_zigbee_count=sizeof(kFz_E8332SCN300ZB)/sizeof(kFz_E8332SCN300ZB[0]),
    .to_zigbee=kTz_E8332SCN300ZB, .to_zigbee_count=sizeof(kTz_E8332SCN300ZB)/sizeof(kTz_E8332SCN300ZB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_E8332SCN300ZB,.bindings_count=sizeof(kBindings_E8332SCN300ZB)/sizeof(kBindings_E8332SCN300ZB[0]),
    .endpoint_map       = kEndpoints_E8332SCN300ZB,
    .endpoint_map_count = sizeof(kEndpoints_E8332SCN300ZB)/sizeof(kEndpoints_E8332SCN300ZB[0]),
};

}  // namespace zhc::devices::schneider
