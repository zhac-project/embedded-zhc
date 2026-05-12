// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Soanalarm SNT858Z — auto-generated.
// Soil moisture sensor
// z2m-source: soanalarm.ts #SNT858Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::soanalarm {
namespace {


constexpr const char* kModels_SNT858Z[] = { "SNT858Z" };

}  // namespace


namespace {
const FzConverter* const kFz_min_SNT858Z[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SNT858Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SNT858Z[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_SNT858Z{
    .zigbee_models=kModels_SNT858Z, .zigbee_models_count=sizeof(kModels_SNT858Z)/sizeof(kModels_SNT858Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNT858Z", .vendor="Soanalarm",
    .meta=nullptr, .exposes=kExp_min_SNT858Z, .exposes_count=sizeof(kExp_min_SNT858Z)/sizeof(kExp_min_SNT858Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SNT858Z, .from_zigbee_count=sizeof(kFz_min_SNT858Z)/sizeof(kFz_min_SNT858Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_SNT858Z,
    .bindings_count           = sizeof(kBind_min_SNT858Z)/sizeof(kBind_min_SNT858Z[0]),
};

}  // namespace zhc::devices::soanalarm
