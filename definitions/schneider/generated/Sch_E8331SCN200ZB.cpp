// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Schneider E8331SCN200ZB — uses shared schneider converters.
// Wiser AvatarOn 1G curtain switch
// z2m-source: schneider.ts #E8331SCN200ZB.
// Indicator + curtain status now wired via Visa-config helpers; cover
// open/close still uses generic windowCovering on endpoint l1 (10).
// Long-tail TODO: visaWiserCurtain custom commands (UpOpen/DnClose/Stop
// on the Visa cluster) are not yet routed.
#include "definitions/_generic/_shared.hpp"
#include "definitions/schneider/_shared.hpp"

namespace zhc::devices::schneider {
namespace {
const FzConverter* const kFz_E8331SCN200ZB[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::schneider::kFzSchneiderVisaCurtain1,
};
const TzConverter* const kTz_E8331SCN200ZB[] = {
    &::zhc::generic::kTzCoverPosition,
    &::zhc::schneider::kTzSchneiderIndicatorLuminance,
    &::zhc::schneider::kTzSchneiderIndicatorColor,
    &::zhc::schneider::kTzSchneiderIndicatorMode,
    &::zhc::schneider::kTzSchneiderMotorTypeCh1,
    &::zhc::schneider::kTzSchneiderCurtainStatusCh1,
};

constexpr const char* kModels_E8331SCN200ZB[] = { "E8331SCN200ZB" };

constexpr Expose kExposes_E8331SCN200ZB[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"indicator_luminance_level", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"indicator_color", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"motor_type_1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"curtain_status_1", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E8331SCN200ZB[] = {
    {10, 0x0102},
};

constexpr ::zhc::EndpointLabel kEndpoints_E8331SCN200ZB[] = { {"l1", 10} };

}  // namespace

extern const PreparedDefinition kDef_E8331SCN200ZB{
    .zigbee_models=kModels_E8331SCN200ZB, .zigbee_models_count=sizeof(kModels_E8331SCN200ZB)/sizeof(kModels_E8331SCN200ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E8331SCN200ZB", .vendor="Schneider",
    .meta=nullptr, .exposes=kExposes_E8331SCN200ZB, .exposes_count=sizeof(kExposes_E8331SCN200ZB)/sizeof(kExposes_E8331SCN200ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E8331SCN200ZB, .from_zigbee_count=sizeof(kFz_E8331SCN200ZB)/sizeof(kFz_E8331SCN200ZB[0]),
    .to_zigbee=kTz_E8331SCN200ZB, .to_zigbee_count=sizeof(kTz_E8331SCN200ZB)/sizeof(kTz_E8331SCN200ZB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kBindings_E8331SCN200ZB,.bindings_count=sizeof(kBindings_E8331SCN200ZB)/sizeof(kBindings_E8331SCN200ZB[0]),
    .endpoint_map       = kEndpoints_E8331SCN200ZB,
    .endpoint_map_count = sizeof(kEndpoints_E8331SCN200ZB)/sizeof(kEndpoints_E8331SCN200ZB[0]),
};

}  // namespace zhc::devices::schneider
