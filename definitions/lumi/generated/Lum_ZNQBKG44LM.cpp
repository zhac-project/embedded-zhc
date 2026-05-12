// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi ZNQBKG44LM — auto-generated.
// Smart wall switch Z1 Pro (triple rocker)
// z2m-source: lumi.ts #ZNQBKG44LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_ZNQBKG44LM[] = { "lumi.switch.acn058" };

constexpr ::zhc::EndpointLabel kEndpoints_ZNQBKG44LM[] = { {"top", 1}, {"center", 2}, {"bottom", 3} };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZNQBKG44LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZNQBKG44LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZNQBKG44LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZNQBKG44LM{
    .zigbee_models=kModels_ZNQBKG44LM, .zigbee_models_count=sizeof(kModels_ZNQBKG44LM)/sizeof(kModels_ZNQBKG44LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZNQBKG44LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_ZNQBKG44LM, .exposes_count=sizeof(kExp_min_ZNQBKG44LM)/sizeof(kExp_min_ZNQBKG44LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZNQBKG44LM, .from_zigbee_count=sizeof(kFz_min_ZNQBKG44LM)/sizeof(kFz_min_ZNQBKG44LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_ZNQBKG44LM,
    .endpoint_map_count = sizeof(kEndpoints_ZNQBKG44LM)/sizeof(kEndpoints_ZNQBKG44LM[0]),
};

}  // namespace zhc::devices::lumi
