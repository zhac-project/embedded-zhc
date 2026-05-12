// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo SZLR08 — auto-generated.
// 24GHz millimeter wave radar
// z2m-source: lincukoo.ts #SZLR08.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_SZLR08[] = { "TS0601" };

}  // namespace

namespace {
const FzConverter* const kFz_min_SZLR08[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SZLR08[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SZLR08[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SZLR08{
    .zigbee_models=kModels_SZLR08, .zigbee_models_count=sizeof(kModels_SZLR08)/sizeof(kModels_SZLR08[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SZLR08", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_SZLR08, .exposes_count=sizeof(kExp_min_SZLR08)/sizeof(kExp_min_SZLR08[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SZLR08, .from_zigbee_count=sizeof(kFz_min_SZLR08)/sizeof(kFz_min_SZLR08[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
