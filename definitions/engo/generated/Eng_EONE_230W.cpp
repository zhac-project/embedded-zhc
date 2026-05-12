// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Engo EONE-230W — auto-generated.
// Zigbee smart thermostat
// z2m-source: engo.ts #EONE-230W.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::engo {
namespace {


constexpr const char* kModels_EONE_230W[] = { "TS0601" };
constexpr const char* kManus_EONE_230W[] = { "_TZE204_ca3i8m8p", "_TZE200_awnadkan" };
}  // namespace


namespace {
const FzConverter* const kFz_min_EONE_230W[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_EONE_230W[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_EONE_230W[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_EONE_230W{
    .zigbee_models=kModels_EONE_230W, .zigbee_models_count=sizeof(kModels_EONE_230W)/sizeof(kModels_EONE_230W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_EONE_230W, .manufacturer_names_count=sizeof(kManus_EONE_230W)/sizeof(kManus_EONE_230W[0]),
    .model="EONE-230W", .vendor="Engo",
    .meta=nullptr, .exposes=kExp_min_EONE_230W, .exposes_count=sizeof(kExp_min_EONE_230W)/sizeof(kExp_min_EONE_230W[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EONE_230W, .from_zigbee_count=sizeof(kFz_min_EONE_230W)/sizeof(kFz_min_EONE_230W[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EONE_230W,
    .bindings_count           = sizeof(kBind_min_EONE_230W)/sizeof(kBind_min_EONE_230W[0]),
};

}  // namespace zhc::devices::engo
