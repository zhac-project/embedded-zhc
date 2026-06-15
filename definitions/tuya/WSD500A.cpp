// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya WSD500A — TS0201 temperature & humidity sensor.
//
// Graduated from generated/Tuy_WSD500A.cpp: the auto-port wired battery +
// genOnOff and DROPPED temperature + humidity (the device's whole purpose), so
// msTemperatureMeasurement (0x0402) and msRelativeHumidity (0x0405) reports came
// in as "(no match)" and never decoded. (The stale shadow cache masked this
// until the VAL_FLOAT NVS-v9 wipe.) z2m drives it with
// `fromZigbee: [TS0201_battery, fz.temperature, fz.humidity]` — no onOff. Wire
// the generic ZCL temp/humidity converters (both /100 → VAL_FLOAT) + battery,
// drop the spurious onOff. Mirrors the base Tuy_TS0201 def.
//
// z2m-source: tuya.ts #WSD500A.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr const char* kModels_WSD500A[] = { "TS0201" };
constexpr const char* kManus_WSD500A[] = { "_TZ3000_bguser20", "_TZ3000_yd2e749y", "_TZ3000_6uzkisv2", "_TZ3000_xr3htd96", "_TZ3000_fllyghyj", "_TZ3000_saiqcn0y", "_TZ3000_bjawzodf" };

const FzConverter* const kFz_WSD500A[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery,        // genPowerCfg 0x0020/0x0021
    &::zhc::generic::kFzTemperature,    // msTemperatureMeasurement 0x0402 (/100)
    &::zhc::generic::kFzHumidity,       // msRelativeHumidity 0x0405 (/100)
};
constexpr Expose kExp_WSD500A[] = {
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_WSD500A[] = {
    { 1, 0x0001 },   // genPowerCfg — battery reports
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
};
constexpr WhiteLabel kWhiteLabels_WSD500A[] = {
    {"Tuya","TH02Z"},
};
}  // namespace

extern const PreparedDefinition kDef_WSD500A{
    .zigbee_models=kModels_WSD500A, .zigbee_models_count=sizeof(kModels_WSD500A)/sizeof(kModels_WSD500A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WSD500A, .manufacturer_names_count=sizeof(kManus_WSD500A)/sizeof(kManus_WSD500A[0]),
    .model="WSD500A", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_WSD500A, .exposes_count=sizeof(kExp_WSD500A)/sizeof(kExp_WSD500A[0]),
    .white_labels=kWhiteLabels_WSD500A, .white_labels_count=sizeof(kWhiteLabels_WSD500A)/sizeof(kWhiteLabels_WSD500A[0]),
    .from_zigbee=kFz_WSD500A, .from_zigbee_count=sizeof(kFz_WSD500A)/sizeof(kFz_WSD500A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_WSD500A, .bindings_count=sizeof(kBind_WSD500A)/sizeof(kBind_WSD500A[0]),
};

}  // namespace zhc::devices::tuya
