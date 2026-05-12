// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya BAC-002-ALZB — auto-generated.
// FCU thermostat temperature controller
// z2m-source: tuya.ts #BAC-002-ALZB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_BAC_002_ALZB[] = { "TS0601" };
constexpr const char* kManus_BAC_002_ALZB[] = { "_TZE200_dzuqwsyg", "_TZE204_dzuqwsyg" };
}  // namespace

namespace {
const FzConverter* const kFz_min_BAC_002_ALZB_Tuy_BAC_002_ALZB[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_BAC_002_ALZB_Tuy_BAC_002_ALZB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_BAC_002_ALZB_Tuy_BAC_002_ALZB[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_BAC_002_ALZB{
    .zigbee_models=kModels_BAC_002_ALZB, .zigbee_models_count=sizeof(kModels_BAC_002_ALZB)/sizeof(kModels_BAC_002_ALZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BAC_002_ALZB, .manufacturer_names_count=sizeof(kManus_BAC_002_ALZB)/sizeof(kManus_BAC_002_ALZB[0]),
    .model="BAC-002-ALZB", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_BAC_002_ALZB_Tuy_BAC_002_ALZB, .exposes_count=sizeof(kExp_min_BAC_002_ALZB_Tuy_BAC_002_ALZB)/sizeof(kExp_min_BAC_002_ALZB_Tuy_BAC_002_ALZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_BAC_002_ALZB_Tuy_BAC_002_ALZB, .from_zigbee_count=sizeof(kFz_min_BAC_002_ALZB_Tuy_BAC_002_ALZB)/sizeof(kFz_min_BAC_002_ALZB_Tuy_BAC_002_ALZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
