// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo SZT04 — auto-generated.
// Temperature and humidity sensor with clock
// z2m-source: lincukoo.ts #SZT04.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_SZT04[] = { "TS0601" };
constexpr const char* kManus_SZT04[] = { "_TZE284_rs62zxk8", "_TZE284_4dosadbh", "_TZE284_mpzuabwk" };
}  // namespace

namespace {
const FzConverter* const kFz_min_SZT04[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SZT04[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SZT04[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_SZT04{
    .zigbee_models=kModels_SZT04, .zigbee_models_count=sizeof(kModels_SZT04)/sizeof(kModels_SZT04[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SZT04, .manufacturer_names_count=sizeof(kManus_SZT04)/sizeof(kManus_SZT04[0]),
    .model="SZT04", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_SZT04, .exposes_count=sizeof(kExp_min_SZT04)/sizeof(kExp_min_SZT04[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SZT04, .from_zigbee_count=sizeof(kFz_min_SZT04)/sizeof(kFz_min_SZT04[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
