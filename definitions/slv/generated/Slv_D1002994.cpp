// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Slv 1002994 — auto-generated.
// VALETO remote (binds to device)
// z2m-source: slv.ts #1002994.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slv {
namespace {


constexpr const char* kModels_D1002994[] = { "1002994" };

}  // namespace


namespace {
const FzConverter* const kFz_min_1002994[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_1002994[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_1002994[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_D1002994{
    .zigbee_models=kModels_D1002994, .zigbee_models_count=sizeof(kModels_D1002994)/sizeof(kModels_D1002994[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1002994", .vendor="Slv",
    .meta=nullptr, .exposes=kExp_min_1002994, .exposes_count=sizeof(kExp_min_1002994)/sizeof(kExp_min_1002994[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_1002994, .from_zigbee_count=sizeof(kFz_min_1002994)/sizeof(kFz_min_1002994[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_1002994,
    .bindings_count           = sizeof(kBind_min_1002994)/sizeof(kBind_min_1002994[0]),
};

}  // namespace zhc::devices::slv
