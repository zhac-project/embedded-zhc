// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TR-M3Z — auto-generated.
// Thermostatic radiator valve actuator
// z2m-source: tuya.ts #TR-M3Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TR_M3Z[] = { "TS0601" };
constexpr const char* kManus_TR_M3Z[] = { "_TZE204_eekpf0ft", "_TZE284_eekpf0ft" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TR_M3Z_Tuy_TR_M3Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TR_M3Z_Tuy_TR_M3Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TR_M3Z_Tuy_TR_M3Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TR_M3Z{
    .zigbee_models=kModels_TR_M3Z, .zigbee_models_count=sizeof(kModels_TR_M3Z)/sizeof(kModels_TR_M3Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TR_M3Z, .manufacturer_names_count=sizeof(kManus_TR_M3Z)/sizeof(kManus_TR_M3Z[0]),
    .model="TR-M3Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TR_M3Z_Tuy_TR_M3Z, .exposes_count=sizeof(kExp_min_TR_M3Z_Tuy_TR_M3Z)/sizeof(kExp_min_TR_M3Z_Tuy_TR_M3Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TR_M3Z_Tuy_TR_M3Z, .from_zigbee_count=sizeof(kFz_min_TR_M3Z_Tuy_TR_M3Z)/sizeof(kFz_min_TR_M3Z_Tuy_TR_M3Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
