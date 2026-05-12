// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya PO-THCO-EAU — auto-generated.
// Thermostat radiator valve
// z2m-source: tuya.ts #PO-THCO-EAU.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_PO_THCO_EAU[] = { "TS0601" };
constexpr const char* kManus_PO_THCO_EAU[] = { "_TZE204_tbgecldg", "_TZE284_tbgecldg", "_TZE200_tbgecldg" };
}  // namespace

namespace {
const FzConverter* const kFz_min_PO_THCO_EAU_Tuy_PO_THCO_EAU[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PO_THCO_EAU_Tuy_PO_THCO_EAU[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PO_THCO_EAU_Tuy_PO_THCO_EAU[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_PO_THCO_EAU{
    .zigbee_models=kModels_PO_THCO_EAU, .zigbee_models_count=sizeof(kModels_PO_THCO_EAU)/sizeof(kModels_PO_THCO_EAU[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PO_THCO_EAU, .manufacturer_names_count=sizeof(kManus_PO_THCO_EAU)/sizeof(kManus_PO_THCO_EAU[0]),
    .model="PO-THCO-EAU", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_PO_THCO_EAU_Tuy_PO_THCO_EAU, .exposes_count=sizeof(kExp_min_PO_THCO_EAU_Tuy_PO_THCO_EAU)/sizeof(kExp_min_PO_THCO_EAU_Tuy_PO_THCO_EAU[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PO_THCO_EAU_Tuy_PO_THCO_EAU, .from_zigbee_count=sizeof(kFz_min_PO_THCO_EAU_Tuy_PO_THCO_EAU)/sizeof(kFz_min_PO_THCO_EAU_Tuy_PO_THCO_EAU[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
