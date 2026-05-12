// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Profalux NB102 — auto-generated.
// Cover remote
// z2m-source: profalux.ts #NB102.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::profalux {
namespace {


constexpr const char* kModels_NB102[] = { "MAI-ZTS" };

}  // namespace


namespace {
const FzConverter* const kFz_min_NB102[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_NB102[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_NB102[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_NB102{
    .zigbee_models=kModels_NB102, .zigbee_models_count=sizeof(kModels_NB102)/sizeof(kModels_NB102[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NB102", .vendor="Profalux",
    .meta=nullptr, .exposes=kExp_min_NB102, .exposes_count=sizeof(kExp_min_NB102)/sizeof(kExp_min_NB102[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_NB102, .from_zigbee_count=sizeof(kFz_min_NB102)/sizeof(kFz_min_NB102[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_NB102,
    .bindings_count           = sizeof(kBind_min_NB102)/sizeof(kBind_min_NB102[0]),
};

}  // namespace zhc::devices::profalux
