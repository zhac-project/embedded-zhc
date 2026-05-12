// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lidl HG08633 — auto-generated.
// Livarno gardenspot RGB
// z2m-source: lidl.ts #HG08633.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lidl {
namespace {


constexpr const char* kModels_HG08633[] = { "TS0505B" };
constexpr const char* kManus_HG08633[] = { "_TZ3000_lxw3zcdk" };
}  // namespace


namespace {
const FzConverter* const kFz_min_HG08633[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_HG08633[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_HG08633[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_HG08633{
    .zigbee_models=kModels_HG08633, .zigbee_models_count=sizeof(kModels_HG08633)/sizeof(kModels_HG08633[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_HG08633, .manufacturer_names_count=sizeof(kManus_HG08633)/sizeof(kManus_HG08633[0]),
    .model="HG08633", .vendor="Lidl",
    .meta=nullptr, .exposes=kExp_min_HG08633, .exposes_count=sizeof(kExp_min_HG08633)/sizeof(kExp_min_HG08633[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_HG08633, .from_zigbee_count=sizeof(kFz_min_HG08633)/sizeof(kFz_min_HG08633[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_HG08633,
    .bindings_count           = sizeof(kBind_min_HG08633)/sizeof(kBind_min_HG08633[0]),
};

}  // namespace zhc::devices::lidl
