// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi ZNQBKG43LM — auto-generated.
// Smart wall switch Z1 Pro (double rocker)
// z2m-source: lumi.ts #ZNQBKG43LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_ZNQBKG43LM[] = { "lumi.switch.acn057" };

constexpr ::zhc::EndpointLabel kEndpoints_ZNQBKG43LM[] = { {"top", 1}, {"bottom", 2} };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZNQBKG43LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZNQBKG43LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZNQBKG43LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZNQBKG43LM{
    .zigbee_models=kModels_ZNQBKG43LM, .zigbee_models_count=sizeof(kModels_ZNQBKG43LM)/sizeof(kModels_ZNQBKG43LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZNQBKG43LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_ZNQBKG43LM, .exposes_count=sizeof(kExp_min_ZNQBKG43LM)/sizeof(kExp_min_ZNQBKG43LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZNQBKG43LM, .from_zigbee_count=sizeof(kFz_min_ZNQBKG43LM)/sizeof(kFz_min_ZNQBKG43LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_ZNQBKG43LM,
    .endpoint_map_count = sizeof(kEndpoints_ZNQBKG43LM)/sizeof(kEndpoints_ZNQBKG43LM[0]),
};

}  // namespace zhc::devices::lumi
