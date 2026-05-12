// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zigbeetlc ZG-227Z-z — uses shared zigbeetlc converters.
// Temperature & humidity sensor (pvxx/ZigbeeTLc)
// z2m-source: zigbeetlc.ts #ZG-227Z-z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zigbeetlc/_shared.hpp"

namespace zhc::devices::zigbeetlc {
namespace {
constexpr const char* kModels_ZG_227Z_z[] = { "ZG-227Z-z" };
}  // namespace

extern const PreparedDefinition kDef_ZG_227Z_z{
    .zigbee_models=kModels_ZG_227Z_z, .zigbee_models_count=sizeof(kModels_ZG_227Z_z)/sizeof(kModels_ZG_227Z_z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-227Z-z", .vendor="Tuya",
    .meta=nullptr,
    .exposes=::zhc::zigbeetlc::kSharedExposes,
    .exposes_count=::zhc::zigbeetlc::kSharedExposesCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=::zhc::zigbeetlc::kSharedFz,
    .from_zigbee_count=::zhc::zigbeetlc::kSharedFzCount,
    .to_zigbee=::zhc::zigbeetlc::kSharedTz,
    .to_zigbee_count=::zhc::zigbeetlc::kSharedTzCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=::zhc::zigbeetlc::kSharedBindings,
    .bindings_count=::zhc::zigbeetlc::kSharedBindingsCount,
};

}  // namespace zhc::devices::zigbeetlc
