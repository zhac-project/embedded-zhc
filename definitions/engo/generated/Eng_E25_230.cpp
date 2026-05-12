// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Engo E25-230 — auto-generated.
// Smart thermostat
// z2m-source: engo.ts #E25-230.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::engo {
namespace {


constexpr const char* kModels_E25_230[] = { "TS0601" };
constexpr const char* kManus_E25_230[] = { "_TZE204_cmyc8g5i" };
}  // namespace


namespace {
const FzConverter* const kFz_min_E25_230[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_E25_230[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_E25_230[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_E25_230{
    .zigbee_models=kModels_E25_230, .zigbee_models_count=sizeof(kModels_E25_230)/sizeof(kModels_E25_230[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_E25_230, .manufacturer_names_count=sizeof(kManus_E25_230)/sizeof(kManus_E25_230[0]),
    .model="E25-230", .vendor="Engo",
    .meta=nullptr, .exposes=kExp_min_E25_230, .exposes_count=sizeof(kExp_min_E25_230)/sizeof(kExp_min_E25_230[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_E25_230, .from_zigbee_count=sizeof(kFz_min_E25_230)/sizeof(kFz_min_E25_230[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_E25_230,
    .bindings_count           = sizeof(kBind_min_E25_230)/sizeof(kBind_min_E25_230[0]),
};

}  // namespace zhc::devices::engo
