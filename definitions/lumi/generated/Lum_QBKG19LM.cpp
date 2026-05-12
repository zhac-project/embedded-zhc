// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi QBKG19LM — auto-generated.
// Smart wall switch T1 (with neutral, single rocker)
// z2m-source: lumi.ts #QBKG19LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_QBKG19LM[] = { "lumi.switch.b1nacn01" };

}  // namespace

namespace {
const FzConverter* const kFz_min_QBKG19LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_QBKG19LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_QBKG19LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_QBKG19LM{
    .zigbee_models=kModels_QBKG19LM, .zigbee_models_count=sizeof(kModels_QBKG19LM)/sizeof(kModels_QBKG19LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="QBKG19LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_QBKG19LM, .exposes_count=sizeof(kExp_min_QBKG19LM)/sizeof(kExp_min_QBKG19LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_QBKG19LM, .from_zigbee_count=sizeof(kFz_min_QBKG19LM)/sizeof(kFz_min_QBKG19LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
