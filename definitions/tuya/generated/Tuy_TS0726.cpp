// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0726 multi-channel on/off switch (1-6 gangs).
// Generic ZCL genOnOff fallback; battery not applicable (mains-powered).
// z2m-source: tuya.ts #TS0726.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

const FzConverter* const kFz_TS0726[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaPowerOnBehavior,
};

const TzConverter* const kTz_TS0726[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::tuya::kTzTuyaPowerOnBehavior,
    &::zhc::tuya::kTzTuyaChildLock,
};

constexpr const char* kModels_TS0726[] = { "TS0726" };

constexpr Expose kExposes_TS0726[] = {
    { "state",             ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "power_on_behavior", ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_TS0726[] = {
    { 1, 0x0006 },  // genOnOff
};

}  // namespace

extern const PreparedDefinition kDef_TS0726{
    .zigbee_models            = kModels_TS0726,
    .zigbee_models_count      = 1,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "TS0726",
    .vendor                   = "Tuya",
    .meta                     = nullptr,
    .exposes                  = kExposes_TS0726,
    .exposes_count            = sizeof(kExposes_TS0726)/sizeof(kExposes_TS0726[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_TS0726,
    .from_zigbee_count        = sizeof(kFz_TS0726)/sizeof(kFz_TS0726[0]),
    .to_zigbee                = kTz_TS0726,
    .to_zigbee_count          = sizeof(kTz_TS0726)/sizeof(kTz_TS0726[0]),
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_TS0726,
    .bindings_count           = sizeof(kBindings_TS0726)/sizeof(kBindings_TS0726[0]),
};

}  // namespace zhc::devices::tuya
