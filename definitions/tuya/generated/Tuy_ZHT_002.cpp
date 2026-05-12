// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZHT-002 — auto-generated.
// ZHT series thermostat
// z2m-source: tuya.ts #ZHT-002.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZHT_002[] = { "TS0601" };
constexpr const char* kManus_ZHT_002[] = { "_TZE204_xalsoe3m" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZHT_002_Tuy_ZHT_002[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZHT_002_Tuy_ZHT_002[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZHT_002_Tuy_ZHT_002[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZHT_002{
    .zigbee_models=kModels_ZHT_002, .zigbee_models_count=sizeof(kModels_ZHT_002)/sizeof(kModels_ZHT_002[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZHT_002, .manufacturer_names_count=sizeof(kManus_ZHT_002)/sizeof(kManus_ZHT_002[0]),
    .model="ZHT-002", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZHT_002_Tuy_ZHT_002, .exposes_count=sizeof(kExp_min_ZHT_002_Tuy_ZHT_002)/sizeof(kExp_min_ZHT_002_Tuy_ZHT_002[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZHT_002_Tuy_ZHT_002, .from_zigbee_count=sizeof(kFz_min_ZHT_002_Tuy_ZHT_002)/sizeof(kFz_min_ZHT_002_Tuy_ZHT_002[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
