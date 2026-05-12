// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Engo EONE-BATB — auto-generated.
// Zigbee smart thermostat
// z2m-source: engo.ts #EONE-BATB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::engo {
namespace {


constexpr const char* kModels_EONE_BATB[] = { "TS0601" };
constexpr const char* kManus_EONE_BATB[] = { "_TZE200_gtouvmvl" };
}  // namespace


namespace {
const FzConverter* const kFz_min_EONE_BATB[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_EONE_BATB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_EONE_BATB[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_EONE_BATB{
    .zigbee_models=kModels_EONE_BATB, .zigbee_models_count=sizeof(kModels_EONE_BATB)/sizeof(kModels_EONE_BATB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_EONE_BATB, .manufacturer_names_count=sizeof(kManus_EONE_BATB)/sizeof(kManus_EONE_BATB[0]),
    .model="EONE-BATB", .vendor="Engo",
    .meta=nullptr, .exposes=kExp_min_EONE_BATB, .exposes_count=sizeof(kExp_min_EONE_BATB)/sizeof(kExp_min_EONE_BATB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EONE_BATB, .from_zigbee_count=sizeof(kFz_min_EONE_BATB)/sizeof(kFz_min_EONE_BATB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EONE_BATB,
    .bindings_count           = sizeof(kBind_min_EONE_BATB)/sizeof(kBind_min_EONE_BATB[0]),
};

}  // namespace zhc::devices::engo
