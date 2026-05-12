// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zigbeetlc LYWSD03MMC-z — uses shared zigbeetlc converters.
// Temperature and Humidity Monitor (pvxx/ZigbeeTLc)
// z2m-source: zigbeetlc.ts #LYWSD03MMC-z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zigbeetlc/_shared.hpp"

namespace zhc::devices::zigbeetlc {
namespace {
constexpr const char* kModels_LYWSD03MMC_z[] = { "LYWSD03MMC-z", "LYWSD03MMC-bz" };
}  // namespace

extern const PreparedDefinition kDef_LYWSD03MMC_z{
    .zigbee_models=kModels_LYWSD03MMC_z, .zigbee_models_count=sizeof(kModels_LYWSD03MMC_z)/sizeof(kModels_LYWSD03MMC_z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LYWSD03MMC-z", .vendor="Xiaomi",
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
