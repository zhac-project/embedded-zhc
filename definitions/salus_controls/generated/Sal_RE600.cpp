// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SalusControls RE600 — auto-generated.
// Router Zigbee
// z2m-source: salus_controls.ts #RE600.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::salus_controls {
namespace {


constexpr const char* kModels_RE600[] = { "RE600" };

}  // namespace


namespace {
const FzConverter* const kFz_min_RE600[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_RE600[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_RE600[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_RE600{
    .zigbee_models=kModels_RE600, .zigbee_models_count=sizeof(kModels_RE600)/sizeof(kModels_RE600[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RE600", .vendor="SalusControls",
    .meta=nullptr, .exposes=kExp_min_RE600, .exposes_count=sizeof(kExp_min_RE600)/sizeof(kExp_min_RE600[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_RE600, .from_zigbee_count=sizeof(kFz_min_RE600)/sizeof(kFz_min_RE600[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_RE600,
    .bindings_count           = sizeof(kBind_min_RE600)/sizeof(kBind_min_RE600[0]),
};

}  // namespace zhc::devices::salus_controls
