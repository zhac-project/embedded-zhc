// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sonoff SNZB-06P — auto-generated.
// Zigbee occupancy sensor
// z2m-source: sonoff.ts #SNZB-06P.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {


constexpr const char* kModels_SNZB_06P[] = { "SNZB-06P" };

}  // namespace


namespace {
const FzConverter* const kFz_min_SNZB_06P[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SNZB_06P[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SNZB_06P[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_SNZB_06P{
    .zigbee_models=kModels_SNZB_06P, .zigbee_models_count=sizeof(kModels_SNZB_06P)/sizeof(kModels_SNZB_06P[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNZB-06P", .vendor="Sonoff",
    .meta=nullptr, .exposes=kExp_min_SNZB_06P, .exposes_count=sizeof(kExp_min_SNZB_06P)/sizeof(kExp_min_SNZB_06P[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SNZB_06P, .from_zigbee_count=sizeof(kFz_min_SNZB_06P)/sizeof(kFz_min_SNZB_06P[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_SNZB_06P,
    .bindings_count           = sizeof(kBind_min_SNZB_06P)/sizeof(kBind_min_SNZB_06P[0]),
};

}  // namespace zhc::devices::sonoff
