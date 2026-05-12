// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Plaid PS-SPRZMS-SLP3 — auto-generated.
// Spruce temperature and moisture sensor
// z2m-source: plaid.ts #PS-SPRZMS-SLP3.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::plaid {
namespace {


constexpr const char* kModels_PS_SPRZMS_SLP3[] = { "PS-SPRZMS-SLP3" };

}  // namespace


namespace {
const FzConverter* const kFz_min_PS_SPRZMS_SLP3[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PS_SPRZMS_SLP3[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PS_SPRZMS_SLP3[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_PS_SPRZMS_SLP3{
    .zigbee_models=kModels_PS_SPRZMS_SLP3, .zigbee_models_count=sizeof(kModels_PS_SPRZMS_SLP3)/sizeof(kModels_PS_SPRZMS_SLP3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PS-SPRZMS-SLP3", .vendor="Plaid",
    .meta=nullptr, .exposes=kExp_min_PS_SPRZMS_SLP3, .exposes_count=sizeof(kExp_min_PS_SPRZMS_SLP3)/sizeof(kExp_min_PS_SPRZMS_SLP3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PS_SPRZMS_SLP3, .from_zigbee_count=sizeof(kFz_min_PS_SPRZMS_SLP3)/sizeof(kFz_min_PS_SPRZMS_SLP3[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_PS_SPRZMS_SLP3,
    .bindings_count           = sizeof(kBind_min_PS_SPRZMS_SLP3)/sizeof(kBind_min_PS_SPRZMS_SLP3[0]),
};

}  // namespace zhc::devices::plaid
