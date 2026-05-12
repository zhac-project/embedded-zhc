// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lidl 14153806L — auto-generated.
// Livarno smart LED ceiling light
// z2m-source: lidl.ts #14153806L.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lidl {
namespace {


constexpr const char* kModels_D14153806L[] = { "TS0504B" };
constexpr const char* kManus_D14153806L[] = { "_TZ3210_sroezl0s" };
}  // namespace


namespace {
const FzConverter* const kFz_min_14153806L[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_14153806L[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_14153806L[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_D14153806L{
    .zigbee_models=kModels_D14153806L, .zigbee_models_count=sizeof(kModels_D14153806L)/sizeof(kModels_D14153806L[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D14153806L, .manufacturer_names_count=sizeof(kManus_D14153806L)/sizeof(kManus_D14153806L[0]),
    .model="14153806L", .vendor="Lidl",
    .meta=nullptr, .exposes=kExp_min_14153806L, .exposes_count=sizeof(kExp_min_14153806L)/sizeof(kExp_min_14153806L[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_14153806L, .from_zigbee_count=sizeof(kFz_min_14153806L)/sizeof(kFz_min_14153806L[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_14153806L,
    .bindings_count           = sizeof(kBind_min_14153806L)/sizeof(kBind_min_14153806L[0]),
};

}  // namespace zhc::devices::lidl
