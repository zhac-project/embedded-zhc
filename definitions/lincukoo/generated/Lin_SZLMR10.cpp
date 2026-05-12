// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo SZLMR10 — auto-generated.
// Human Motion & Presence Sensor
// z2m-source: lincukoo.ts #SZLMR10.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_SZLMR10[] = { "TS0601" };
constexpr const char* kManus_SZLMR10[] = { "_TZE204_sndkanfr", "_TZE204_bjf8qum1" };
}  // namespace

namespace {
const FzConverter* const kFz_min_SZLMR10[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SZLMR10[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SZLMR10[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SZLMR10{
    .zigbee_models=kModels_SZLMR10, .zigbee_models_count=sizeof(kModels_SZLMR10)/sizeof(kModels_SZLMR10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SZLMR10, .manufacturer_names_count=sizeof(kManus_SZLMR10)/sizeof(kManus_SZLMR10[0]),
    .model="SZLMR10", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_SZLMR10, .exposes_count=sizeof(kExp_min_SZLMR10)/sizeof(kExp_min_SZLMR10[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SZLMR10, .from_zigbee_count=sizeof(kFz_min_SZLMR10)/sizeof(kFz_min_SZLMR10[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
