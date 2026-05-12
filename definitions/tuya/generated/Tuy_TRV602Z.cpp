// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TRV602Z — auto-generated.
// Thermostatic radiator valve.
// z2m-source: tuya.ts #TRV602Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TRV602Z[] = { "TS0601" };
constexpr const char* kManus_TRV602Z[] = { "_TZE204_qyr2m29i", "_TZE204_ltwbm23f", "_TZE284_ltwbm23f" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TRV602Z_Tuy_TRV602Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TRV602Z_Tuy_TRV602Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TRV602Z_Tuy_TRV602Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TRV602Z{
    .zigbee_models=kModels_TRV602Z, .zigbee_models_count=sizeof(kModels_TRV602Z)/sizeof(kModels_TRV602Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TRV602Z, .manufacturer_names_count=sizeof(kManus_TRV602Z)/sizeof(kManus_TRV602Z[0]),
    .model="TRV602Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TRV602Z_Tuy_TRV602Z, .exposes_count=sizeof(kExp_min_TRV602Z_Tuy_TRV602Z)/sizeof(kExp_min_TRV602Z_Tuy_TRV602Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TRV602Z_Tuy_TRV602Z, .from_zigbee_count=sizeof(kFz_min_TRV602Z_Tuy_TRV602Z)/sizeof(kFz_min_TRV602Z_Tuy_TRV602Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
