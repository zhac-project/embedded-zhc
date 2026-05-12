// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi QBKG34LM — auto-generated.
// Smart wall switch T1 (with neutral, triple rocker)
// z2m-source: lumi.ts #QBKG34LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_QBKG34LM[] = { "lumi.switch.b3n01" };

constexpr ::zhc::EndpointLabel kEndpoints_QBKG34LM[] = { {"left", 1}, {"center", 2}, {"right", 3} };

}  // namespace

namespace {
const FzConverter* const kFz_min_QBKG34LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_QBKG34LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_QBKG34LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_QBKG34LM{
    .zigbee_models=kModels_QBKG34LM, .zigbee_models_count=sizeof(kModels_QBKG34LM)/sizeof(kModels_QBKG34LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="QBKG34LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_QBKG34LM, .exposes_count=sizeof(kExp_min_QBKG34LM)/sizeof(kExp_min_QBKG34LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_QBKG34LM, .from_zigbee_count=sizeof(kFz_min_QBKG34LM)/sizeof(kFz_min_QBKG34LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_QBKG34LM,
    .endpoint_map_count = sizeof(kEndpoints_QBKG34LM)/sizeof(kEndpoints_QBKG34LM[0]),
};

}  // namespace zhc::devices::lumi
