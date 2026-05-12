// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo R12LM-Z11T — auto-generated.
// Human motion & presence sensor
// z2m-source: lincukoo.ts #R12LM-Z11T.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_R12LM_Z11T[] = { "TS0601" };
constexpr const char* kManus_R12LM_Z11T[] = { "_TZE284_zzm83zpz" };
}  // namespace

namespace {
const FzConverter* const kFz_min_R12LM_Z11T[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_R12LM_Z11T[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_R12LM_Z11T[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_R12LM_Z11T{
    .zigbee_models=kModels_R12LM_Z11T, .zigbee_models_count=sizeof(kModels_R12LM_Z11T)/sizeof(kModels_R12LM_Z11T[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_R12LM_Z11T, .manufacturer_names_count=sizeof(kManus_R12LM_Z11T)/sizeof(kManus_R12LM_Z11T[0]),
    .model="R12LM-Z11T", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_R12LM_Z11T, .exposes_count=sizeof(kExp_min_R12LM_Z11T)/sizeof(kExp_min_R12LM_Z11T[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_R12LM_Z11T, .from_zigbee_count=sizeof(kFz_min_R12LM_Z11T)/sizeof(kFz_min_R12LM_Z11T[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
