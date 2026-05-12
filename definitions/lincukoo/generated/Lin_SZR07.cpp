// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo SZR07 — auto-generated.
// 24GHz millimeter wave radar
// z2m-source: lincukoo.ts #SZR07.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_SZR07[] = { "TS0601" };
constexpr const char* kManus_SZR07[] = { "_TZE204_khoqss0a" };
}  // namespace

namespace {
const FzConverter* const kFz_min_SZR07[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SZR07[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SZR07[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SZR07{
    .zigbee_models=kModels_SZR07, .zigbee_models_count=sizeof(kModels_SZR07)/sizeof(kModels_SZR07[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SZR07, .manufacturer_names_count=sizeof(kManus_SZR07)/sizeof(kManus_SZR07[0]),
    .model="SZR07", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_SZR07, .exposes_count=sizeof(kExp_min_SZR07)/sizeof(kExp_min_SZR07[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SZR07, .from_zigbee_count=sizeof(kFz_min_SZR07)/sizeof(kFz_min_SZR07[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
