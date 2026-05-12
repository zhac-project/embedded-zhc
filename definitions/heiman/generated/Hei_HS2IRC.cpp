// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Heiman HS2IRC — auto-generated.
// Smart IR Control
// z2m-source: heiman.ts #HS2IRC.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {


constexpr const char* kModels_HS2IRC[] = { "IRControl2-EF-3.0" };

}  // namespace


namespace {
const FzConverter* const kFz_min_HS2IRC[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_HS2IRC[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_HS2IRC[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_HS2IRC{
    .zigbee_models=kModels_HS2IRC, .zigbee_models_count=sizeof(kModels_HS2IRC)/sizeof(kModels_HS2IRC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS2IRC", .vendor="Heiman",
    .meta=nullptr, .exposes=kExp_min_HS2IRC, .exposes_count=sizeof(kExp_min_HS2IRC)/sizeof(kExp_min_HS2IRC[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_HS2IRC, .from_zigbee_count=sizeof(kFz_min_HS2IRC)/sizeof(kFz_min_HS2IRC[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_HS2IRC,
    .bindings_count           = sizeof(kBind_min_HS2IRC)/sizeof(kBind_min_HS2IRC[0]),
};

}  // namespace zhc::devices::heiman
