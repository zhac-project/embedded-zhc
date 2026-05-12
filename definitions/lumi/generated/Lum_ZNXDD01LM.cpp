// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi ZNXDD01LM — auto-generated.
// Ceiling light L1-350
// z2m-source: lumi.ts #ZNXDD01LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_ZNXDD01LM[] = { "lumi.light.acn003" };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZNXDD01LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZNXDD01LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZNXDD01LM[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZNXDD01LM{
    .zigbee_models=kModels_ZNXDD01LM, .zigbee_models_count=sizeof(kModels_ZNXDD01LM)/sizeof(kModels_ZNXDD01LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZNXDD01LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_ZNXDD01LM, .exposes_count=sizeof(kExp_min_ZNXDD01LM)/sizeof(kExp_min_ZNXDD01LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZNXDD01LM, .from_zigbee_count=sizeof(kFz_min_ZNXDD01LM)/sizeof(kFz_min_ZNXDD01LM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
