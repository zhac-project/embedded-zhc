// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zigbeetlc MHO-C122 — uses shared zigbeetlc converters.
// Temperature and Humidity Monitor (pvxx/ZigbeeTLc)
// z2m-source: zigbeetlc.ts #MHO-C122.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zigbeetlc/_shared.hpp"

namespace zhc::devices::zigbeetlc {
namespace {
constexpr const char* kModels_MHO_C122[] = { "MHO-C122-z", "MHO-C122-bz" };
}  // namespace

extern const PreparedDefinition kDef_MHO_C122{
    .zigbee_models=kModels_MHO_C122, .zigbee_models_count=sizeof(kModels_MHO_C122)/sizeof(kModels_MHO_C122[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MHO-C122", .vendor="MiaoMiaoCe",
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
