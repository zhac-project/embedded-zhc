// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mill Mill-gen-4 — auto-generated.
// WiFi heating panel gen4
// z2m-source: mill.ts #Mill-gen-4.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mill {
namespace {


constexpr const char* kModels_Mill_gen_4[] = { "Panel gen4", "Mill Wi-Fi Panel Heater Gen4" };

}  // namespace


namespace {
const FzConverter* const kFz_min_Mill_gen_4[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_Mill_gen_4[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_Mill_gen_4[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_Mill_gen_4{
    .zigbee_models=kModels_Mill_gen_4, .zigbee_models_count=sizeof(kModels_Mill_gen_4)/sizeof(kModels_Mill_gen_4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Mill-gen-4", .vendor="Mill",
    .meta=nullptr, .exposes=kExp_min_Mill_gen_4, .exposes_count=sizeof(kExp_min_Mill_gen_4)/sizeof(kExp_min_Mill_gen_4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_Mill_gen_4, .from_zigbee_count=sizeof(kFz_min_Mill_gen_4)/sizeof(kFz_min_Mill_gen_4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_Mill_gen_4,
    .bindings_count           = sizeof(kBind_min_Mill_gen_4)/sizeof(kBind_min_Mill_gen_4[0]),
};

}  // namespace zhc::devices::mill
