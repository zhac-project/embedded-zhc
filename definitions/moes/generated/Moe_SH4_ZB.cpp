// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes SH4-ZB — auto-generated.
// Thermostatic radiator valve
// z2m-source: moes.ts #SH4-ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::moes {
namespace {


constexpr const char* kModels_SH4_ZB[] = { "TS0601" };
constexpr const char* kManus_SH4_ZB[] = { "_TZE200_fhn3negr" };
}  // namespace

namespace {
const FzConverter* const kFz_min_SH4_ZB[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SH4_ZB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SH4_ZB[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SH4_ZB{
    .zigbee_models=kModels_SH4_ZB, .zigbee_models_count=sizeof(kModels_SH4_ZB)/sizeof(kModels_SH4_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SH4_ZB, .manufacturer_names_count=sizeof(kManus_SH4_ZB)/sizeof(kManus_SH4_ZB[0]),
    .model="SH4-ZB", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_min_SH4_ZB, .exposes_count=sizeof(kExp_min_SH4_ZB)/sizeof(kExp_min_SH4_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SH4_ZB, .from_zigbee_count=sizeof(kFz_min_SH4_ZB)/sizeof(kFz_min_SH4_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::moes
