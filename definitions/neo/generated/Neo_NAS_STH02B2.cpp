// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Neo NAS-STH02B2 — auto-generated.
// Soil moisture, temperature, and ec
// z2m-source: neo.ts #NAS-STH02B2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::neo {
namespace {


constexpr const char* kModels_NAS_STH02B2[] = { "TS0601" };
constexpr const char* kManus_NAS_STH02B2[] = { "_TZE284_rqcuwlsa" };
}  // namespace


namespace {
const FzConverter* const kFz_min_NAS_STH02B2[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_NAS_STH02B2[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_NAS_STH02B2[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_NAS_STH02B2{
    .zigbee_models=kModels_NAS_STH02B2, .zigbee_models_count=sizeof(kModels_NAS_STH02B2)/sizeof(kModels_NAS_STH02B2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_NAS_STH02B2, .manufacturer_names_count=sizeof(kManus_NAS_STH02B2)/sizeof(kManus_NAS_STH02B2[0]),
    .model="NAS-STH02B2", .vendor="Neo",
    .meta=nullptr, .exposes=kExp_min_NAS_STH02B2, .exposes_count=sizeof(kExp_min_NAS_STH02B2)/sizeof(kExp_min_NAS_STH02B2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_NAS_STH02B2, .from_zigbee_count=sizeof(kFz_min_NAS_STH02B2)/sizeof(kFz_min_NAS_STH02B2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_NAS_STH02B2,
    .bindings_count           = sizeof(kBind_min_NAS_STH02B2)/sizeof(kBind_min_NAS_STH02B2[0]),
};

}  // namespace zhc::devices::neo
