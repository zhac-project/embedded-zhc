// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo SZLR08T — auto-generated.
// 24GHz millimeter wave radar
// z2m-source: lincukoo.ts #SZLR08T.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_SZLR08T[] = { "TS0601" };
constexpr const char* kManus_SZLR08T[] = { "_TZE204_b8vxct9l" };
}  // namespace

namespace {
const FzConverter* const kFz_min_SZLR08T[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SZLR08T[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SZLR08T[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SZLR08T{
    .zigbee_models=kModels_SZLR08T, .zigbee_models_count=sizeof(kModels_SZLR08T)/sizeof(kModels_SZLR08T[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SZLR08T, .manufacturer_names_count=sizeof(kManus_SZLR08T)/sizeof(kManus_SZLR08T[0]),
    .model="SZLR08T", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_SZLR08T, .exposes_count=sizeof(kExp_min_SZLR08T)/sizeof(kExp_min_SZLR08T[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SZLR08T, .from_zigbee_count=sizeof(kFz_min_SZLR08T)/sizeof(kFz_min_SZLR08T[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
