// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi QBKG18LM — auto-generated.
// Smart wall switch T1 (no neutral, double rocker)
// z2m-source: lumi.ts #QBKG18LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_QBKG18LM[] = { "lumi.switch.b2lacn01" };

constexpr ::zhc::EndpointLabel kEndpoints_QBKG18LM[] = { {"left", 1}, {"right", 2} };

}  // namespace

namespace {
const FzConverter* const kFz_min_QBKG18LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_QBKG18LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_QBKG18LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_QBKG18LM{
    .zigbee_models=kModels_QBKG18LM, .zigbee_models_count=sizeof(kModels_QBKG18LM)/sizeof(kModels_QBKG18LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="QBKG18LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_QBKG18LM, .exposes_count=sizeof(kExp_min_QBKG18LM)/sizeof(kExp_min_QBKG18LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_QBKG18LM, .from_zigbee_count=sizeof(kFz_min_QBKG18LM)/sizeof(kFz_min_QBKG18LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_QBKG18LM,
    .endpoint_map_count = sizeof(kEndpoints_QBKG18LM)/sizeof(kEndpoints_QBKG18LM[0]),
};

}  // namespace zhc::devices::lumi
