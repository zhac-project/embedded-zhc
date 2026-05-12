// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi ZNQBKG41LM — auto-generated.
// Smart wall switch Z1 (quadruple rocker)
// z2m-source: lumi.ts #ZNQBKG41LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_ZNQBKG41LM[] = { "lumi.switch.acn055" };

constexpr ::zhc::EndpointLabel kEndpoints_ZNQBKG41LM[] = { {"top", 1}, {"center", 2}, {"bottom", 3}, {"wireless", 4} };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZNQBKG41LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZNQBKG41LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZNQBKG41LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZNQBKG41LM{
    .zigbee_models=kModels_ZNQBKG41LM, .zigbee_models_count=sizeof(kModels_ZNQBKG41LM)/sizeof(kModels_ZNQBKG41LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZNQBKG41LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_ZNQBKG41LM, .exposes_count=sizeof(kExp_min_ZNQBKG41LM)/sizeof(kExp_min_ZNQBKG41LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZNQBKG41LM, .from_zigbee_count=sizeof(kFz_min_ZNQBKG41LM)/sizeof(kFz_min_ZNQBKG41LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_ZNQBKG41LM,
    .endpoint_map_count = sizeof(kEndpoints_ZNQBKG41LM)/sizeof(kEndpoints_ZNQBKG41LM[0]),
};

}  // namespace zhc::devices::lumi
