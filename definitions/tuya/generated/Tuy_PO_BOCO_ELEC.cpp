// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya PO-BOCO-ELEC — auto-generated.
// Pilot wire heating module
// z2m-source: tuya.ts #PO-BOCO-ELEC.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_PO_BOCO_ELEC[] = { "TS0601" };
constexpr const char* kManus_PO_BOCO_ELEC[] = { "_TZE204_d6i25bwg", "_TZE204_3q3maeoo" };
}  // namespace

namespace {
const FzConverter* const kFz_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_PO_BOCO_ELEC{
    .zigbee_models=kModels_PO_BOCO_ELEC, .zigbee_models_count=sizeof(kModels_PO_BOCO_ELEC)/sizeof(kModels_PO_BOCO_ELEC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PO_BOCO_ELEC, .manufacturer_names_count=sizeof(kManus_PO_BOCO_ELEC)/sizeof(kManus_PO_BOCO_ELEC[0]),
    .model="PO-BOCO-ELEC", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC, .exposes_count=sizeof(kExp_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC)/sizeof(kExp_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC, .from_zigbee_count=sizeof(kFz_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC)/sizeof(kFz_min_PO_BOCO_ELEC_Tuy_PO_BOCO_ELEC[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
