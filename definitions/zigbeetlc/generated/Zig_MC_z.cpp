// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zigbeetlc MC-z — uses shared zigbeetlc converters.
// Temperature & Humidity Sensor (pvxx/ZigbeeTLc)
// z2m-source: zigbeetlc.ts #MC-z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zigbeetlc/_shared.hpp"

namespace zhc::devices::zigbeetlc {
namespace {
constexpr const char* kModels_MC_z[] = { "MC-z", "TH01-z", "TH01-2-z" };
}  // namespace

extern const PreparedDefinition kDef_MC_z{
    .zigbee_models=kModels_MC_z, .zigbee_models_count=sizeof(kModels_MC_z)/sizeof(kModels_MC_z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MC-z", .vendor="ZBeacon",
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
