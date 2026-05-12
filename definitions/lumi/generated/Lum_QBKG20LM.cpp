// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi QBKG20LM — auto-generated.
// Smart wall switch T1 (with neutral, double rocker)
// z2m-source: lumi.ts #QBKG20LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_QBKG20LM[] = { "lumi.switch.b2nacn01" };

constexpr ::zhc::EndpointLabel kEndpoints_QBKG20LM[] = { {"left", 1}, {"right", 2} };

}  // namespace

namespace {
const FzConverter* const kFz_min_QBKG20LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_QBKG20LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_QBKG20LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_QBKG20LM{
    .zigbee_models=kModels_QBKG20LM, .zigbee_models_count=sizeof(kModels_QBKG20LM)/sizeof(kModels_QBKG20LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="QBKG20LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_QBKG20LM, .exposes_count=sizeof(kExp_min_QBKG20LM)/sizeof(kExp_min_QBKG20LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_QBKG20LM, .from_zigbee_count=sizeof(kFz_min_QBKG20LM)/sizeof(kFz_min_QBKG20LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_QBKG20LM,
    .endpoint_map_count = sizeof(kEndpoints_QBKG20LM)/sizeof(kEndpoints_QBKG20LM[0]),
};

}  // namespace zhc::devices::lumi
