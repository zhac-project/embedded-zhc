// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi WS-K01D — auto-generated.
// Smart wall switch H1 20A (with neutral, single rocker)
// z2m-source: lumi.ts #WS-K01D.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_WS_K01D[] = { "lumi.switch.acn061" };

}  // namespace

namespace {
const FzConverter* const kFz_min_WS_K01D[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_WS_K01D[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_WS_K01D[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_WS_K01D{
    .zigbee_models=kModels_WS_K01D, .zigbee_models_count=sizeof(kModels_WS_K01D)/sizeof(kModels_WS_K01D[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WS-K01D", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_WS_K01D, .exposes_count=sizeof(kExp_min_WS_K01D)/sizeof(kExp_min_WS_K01D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_WS_K01D, .from_zigbee_count=sizeof(kFz_min_WS_K01D)/sizeof(kFz_min_WS_K01D[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
