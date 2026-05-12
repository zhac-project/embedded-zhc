// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZD24_Presence_Sensor — auto-generated.
// PIR 24GHz human presence sensor
// z2m-source: tuya.ts #ZD24_Presence_Sensor.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZD24_Presence_Sensor[] = { "TS0601" };
constexpr const char* kManus_ZD24_Presence_Sensor[] = { "_TZE284_bw4ayyeh" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZD24_Presence_Sensor{
    .zigbee_models=kModels_ZD24_Presence_Sensor, .zigbee_models_count=sizeof(kModels_ZD24_Presence_Sensor)/sizeof(kModels_ZD24_Presence_Sensor[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZD24_Presence_Sensor, .manufacturer_names_count=sizeof(kManus_ZD24_Presence_Sensor)/sizeof(kManus_ZD24_Presence_Sensor[0]),
    .model="ZD24_Presence_Sensor", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor, .exposes_count=sizeof(kExp_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor)/sizeof(kExp_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor, .from_zigbee_count=sizeof(kFz_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor)/sizeof(kFz_min_ZD24_Presence_Sensor_Tuy_ZD24_Presence_Sensor[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
