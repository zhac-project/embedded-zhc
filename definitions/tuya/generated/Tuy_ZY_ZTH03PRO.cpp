// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZY-ZTH03PRO — auto-generated.
// Temperature & humidity sensor with external probe
// z2m-source: tuya.ts #ZY-ZTH03PRO.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZY_ZTH03PRO[] = { "TS0601" };
constexpr const char* kManus_ZY_ZTH03PRO[] = { "_TZE284_hodyryli" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZY_ZTH03PRO{
    .zigbee_models=kModels_ZY_ZTH03PRO, .zigbee_models_count=sizeof(kModels_ZY_ZTH03PRO)/sizeof(kModels_ZY_ZTH03PRO[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZY_ZTH03PRO, .manufacturer_names_count=sizeof(kManus_ZY_ZTH03PRO)/sizeof(kManus_ZY_ZTH03PRO[0]),
    .model="ZY-ZTH03PRO", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO, .exposes_count=sizeof(kExp_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO)/sizeof(kExp_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO, .from_zigbee_count=sizeof(kFz_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO)/sizeof(kFz_min_ZY_ZTH03PRO_Tuy_ZY_ZTH03PRO[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
