// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sonoff MG1_5RZ — auto-generated.
// Zigbee human presence radar (5.8 GHz)
// z2m-source: sonoff.ts #MG1_5RZ.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {


constexpr const char* kModels_MG1_5RZ[] = { "MG1_5RZ" };

}  // namespace


namespace {
const FzConverter* const kFz_min_MG1_5RZ[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_MG1_5RZ[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_MG1_5RZ[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_MG1_5RZ{
    .zigbee_models=kModels_MG1_5RZ, .zigbee_models_count=sizeof(kModels_MG1_5RZ)/sizeof(kModels_MG1_5RZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MG1_5RZ", .vendor="Sonoff",
    .meta=nullptr, .exposes=kExp_min_MG1_5RZ, .exposes_count=sizeof(kExp_min_MG1_5RZ)/sizeof(kExp_min_MG1_5RZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_MG1_5RZ, .from_zigbee_count=sizeof(kFz_min_MG1_5RZ)/sizeof(kFz_min_MG1_5RZ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_MG1_5RZ,
    .bindings_count           = sizeof(kBind_min_MG1_5RZ)/sizeof(kBind_min_MG1_5RZ[0]),
};

}  // namespace zhc::devices::sonoff
