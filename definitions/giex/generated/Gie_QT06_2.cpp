// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Giex QT06_2 — auto-generated.
// 
// z2m-source: giex.ts #QT06_2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::giex {
namespace {


constexpr const char* kModels_QT06_2[] = { "TS0601" };
constexpr const char* kManus_QT06_2[] = { "_TZE200_a7sghmms", "_TZE204_a7sghmms", "_TZE204_7ytb3h8u", "_TZE200_7ytb3h8u", "_TZE284_7ytb3h8u" };
}  // namespace


namespace {
const FzConverter* const kFz_min_QT06_2[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_QT06_2[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_QT06_2[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}

constexpr WhiteLabel kWhiteLabels_QT06_2[] = {
    {"GIEX","GX02"},
};
extern const PreparedDefinition kDef_QT06_2{
    .zigbee_models=kModels_QT06_2, .zigbee_models_count=sizeof(kModels_QT06_2)/sizeof(kModels_QT06_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QT06_2, .manufacturer_names_count=sizeof(kManus_QT06_2)/sizeof(kManus_QT06_2[0]),
    .model="QT06_2", .vendor="Giex",
    .meta=nullptr, .exposes=kExp_min_QT06_2, .exposes_count=sizeof(kExp_min_QT06_2)/sizeof(kExp_min_QT06_2[0]),
    .white_labels=kWhiteLabels_QT06_2, .white_labels_count=sizeof(kWhiteLabels_QT06_2)/sizeof(kWhiteLabels_QT06_2[0]),
    .from_zigbee=kFz_min_QT06_2, .from_zigbee_count=sizeof(kFz_min_QT06_2)/sizeof(kFz_min_QT06_2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_QT06_2,
    .bindings_count           = sizeof(kBind_min_QT06_2)/sizeof(kBind_min_QT06_2[0]),
};

}  // namespace zhc::devices::giex
