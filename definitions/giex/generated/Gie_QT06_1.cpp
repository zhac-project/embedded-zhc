// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Giex QT06_1 — auto-generated.
// 
// z2m-source: giex.ts #QT06_1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::giex {
namespace {


constexpr const char* kModels_QT06_1[] = { "TS0601" };
constexpr const char* kManus_QT06_1[] = { "_TZE200_sh1btabb" };
}  // namespace


namespace {
const FzConverter* const kFz_min_QT06_1[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_QT06_1[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_QT06_1[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_QT06_1{
    .zigbee_models=kModels_QT06_1, .zigbee_models_count=sizeof(kModels_QT06_1)/sizeof(kModels_QT06_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QT06_1, .manufacturer_names_count=sizeof(kManus_QT06_1)/sizeof(kManus_QT06_1[0]),
    .model="QT06_1", .vendor="Giex",
    .meta=nullptr, .exposes=kExp_min_QT06_1, .exposes_count=sizeof(kExp_min_QT06_1)/sizeof(kExp_min_QT06_1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_QT06_1, .from_zigbee_count=sizeof(kFz_min_QT06_1)/sizeof(kFz_min_QT06_1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_QT06_1,
    .bindings_count           = sizeof(kBind_min_QT06_1)/sizeof(kBind_min_QT06_1[0]),
};

}  // namespace zhc::devices::giex
