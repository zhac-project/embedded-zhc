// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo GEZ65 — auto-generated.
// Zigbee Repeater
// z2m-source: lincukoo.ts #GEZ65.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_GEZ65[] = { "Zigbee-Repeater" };

}  // namespace

namespace {
const FzConverter* const kFz_min_GEZ65[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_GEZ65[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_GEZ65[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_GEZ65{
    .zigbee_models=kModels_GEZ65, .zigbee_models_count=sizeof(kModels_GEZ65)/sizeof(kModels_GEZ65[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GEZ65", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_GEZ65, .exposes_count=sizeof(kExp_min_GEZ65)/sizeof(kExp_min_GEZ65[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_GEZ65, .from_zigbee_count=sizeof(kFz_min_GEZ65)/sizeof(kFz_min_GEZ65[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
