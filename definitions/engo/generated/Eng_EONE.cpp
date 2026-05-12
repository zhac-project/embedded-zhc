// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Engo EONE — auto-generated.
// Smart thermostat
// z2m-source: engo.ts #EONE.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::engo {
namespace {


constexpr const char* kModels_EONE[] = { "TS0601" };
constexpr const char* kManus_EONE[] = { "_TZE204_djurk6p5" };
}  // namespace


namespace {
const FzConverter* const kFz_min_EONE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_EONE[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_EONE[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_EONE{
    .zigbee_models=kModels_EONE, .zigbee_models_count=sizeof(kModels_EONE)/sizeof(kModels_EONE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_EONE, .manufacturer_names_count=sizeof(kManus_EONE)/sizeof(kManus_EONE[0]),
    .model="EONE", .vendor="Engo",
    .meta=nullptr, .exposes=kExp_min_EONE, .exposes_count=sizeof(kExp_min_EONE)/sizeof(kExp_min_EONE[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EONE, .from_zigbee_count=sizeof(kFz_min_EONE)/sizeof(kFz_min_EONE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EONE,
    .bindings_count           = sizeof(kBind_min_EONE)/sizeof(kBind_min_EONE[0]),
};

}  // namespace zhc::devices::engo
