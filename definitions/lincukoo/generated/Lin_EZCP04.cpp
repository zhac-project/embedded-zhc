// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo EZCP04 — auto-generated.
// Smart air quality monitor (CO2+PM2.5)
// z2m-source: lincukoo.ts #EZCP04.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_EZCP04[] = { "TS0601" };
constexpr const char* kManus_EZCP04[] = { "_TZE204_fpwtjlfh" };
}  // namespace

namespace {
const FzConverter* const kFz_min_EZCP04[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_EZCP04[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_EZCP04[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_EZCP04{
    .zigbee_models=kModels_EZCP04, .zigbee_models_count=sizeof(kModels_EZCP04)/sizeof(kModels_EZCP04[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_EZCP04, .manufacturer_names_count=sizeof(kManus_EZCP04)/sizeof(kManus_EZCP04[0]),
    .model="EZCP04", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_EZCP04, .exposes_count=sizeof(kExp_min_EZCP04)/sizeof(kExp_min_EZCP04[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EZCP04, .from_zigbee_count=sizeof(kFz_min_EZCP04)/sizeof(kFz_min_EZCP04[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
