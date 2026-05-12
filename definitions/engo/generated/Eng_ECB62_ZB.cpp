// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Engo ECB62-ZB — auto-generated.
// Control box for underfloor heating system
// z2m-source: engo.ts #ECB62-ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::engo {
namespace {


constexpr const char* kModels_ECB62_ZB[] = { "TS0601" };
constexpr const char* kManus_ECB62_ZB[] = { "_TZE200_oahqgdig", "_TZE200_zaabefnt" };
}  // namespace


namespace {
const FzConverter* const kFz_min_ECB62_ZB[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ECB62_ZB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ECB62_ZB[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_ECB62_ZB{
    .zigbee_models=kModels_ECB62_ZB, .zigbee_models_count=sizeof(kModels_ECB62_ZB)/sizeof(kModels_ECB62_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ECB62_ZB, .manufacturer_names_count=sizeof(kManus_ECB62_ZB)/sizeof(kManus_ECB62_ZB[0]),
    .model="ECB62-ZB", .vendor="Engo",
    .meta=nullptr, .exposes=kExp_min_ECB62_ZB, .exposes_count=sizeof(kExp_min_ECB62_ZB)/sizeof(kExp_min_ECB62_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ECB62_ZB, .from_zigbee_count=sizeof(kFz_min_ECB62_ZB)/sizeof(kFz_min_ECB62_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_ECB62_ZB,
    .bindings_count           = sizeof(kBind_min_ECB62_ZB)/sizeof(kBind_min_ECB62_ZB[0]),
};

}  // namespace zhc::devices::engo
