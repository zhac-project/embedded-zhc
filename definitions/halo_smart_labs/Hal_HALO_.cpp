// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HaloSmartLabs HALO+ — graduated from generated/ for a parity fix.
// Halo+ smart smoke & CO detector with weather radio
// z2m-source: halo_smart_labs.ts #HALO+ (haloCommonExtend + haloWeather).
//
// Parity fix — identical to HALO (was a battery+on/off stub). z2m's
// `haloCommonExtend` surfaces, via STANDARD clusters / the existing Halo
// IAS demux converter:
//   * ssIasZone (0x0500) — smoke / tamper / battery_low / test /
//     mains_power_connected on EP1, carbon_monoxide on EP3
//     (decoded by `kFzHaloIasZone`, already implemented in _shared.cpp).
//   * msTemperatureMeasurement (0x0402) → temperature  (kFzTemperature)
//   * msRelativeHumidity       (0x0405) → humidity     (kFzHumidity)
//   * msPressureMeasurement    (0x0403) → pressure     (kFzPressure)
//   * the EP2 colour light (m.light HS, endpointNames:["light"]) →
//     state/brightness/hue/saturation, suffixed `_light` via endpoint_map.
//
// HALO+ additionally has the `haloWeather` (0xFD03) manuSpecific cluster
// (weatherAlertStatus / weatherMute / weather-station RSSI, severe-storm
// radio) on top of the manuSpecific clusters shared with HALO
// (haloDeviceStatus 0xFD00, haloControl 0xFD01, haloSensors 0xFD02
// [co_ppm]). None are wired — cluster_id_to_name() does not know these
// names, so no FzConverter can select them. Tracked as INFRA in the
// parity doc.
#include "definitions/_generic/_shared.hpp"
#include "definitions/halo_smart_labs/_shared.hpp"

namespace zhc::devices::halo_smart_labs {
namespace {


constexpr const char* kModels_HALO_[] = { "halo+", "haloWX", "SABDA1" };

const FzConverter* const kFz_HALO_[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
    &kFzHaloIasZone,
    // EP2 colour light.
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_HALO_[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
constexpr Expose kExp_HALO_[] = {
    // Safety sensors (ssIasZone demux — EP1/EP3, bare keys).
    { "smoke",                 ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "carbon_monoxide",       ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",                ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery_low",           ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "test",                  ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "mains_power_connected", ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    // Environmental sensors (standard clusters on EP1, bare keys).
    { "temperature",           ExposeType::Numeric, ::zhc::Access::State, "C",  nullptr, nullptr, 0 },
    { "humidity",              ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "pressure",              ExposeType::Numeric, ::zhc::Access::State, "hPa",nullptr, nullptr, 0 },
    { "battery",               ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",               ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
    // EP2 colour light — runtime-suffixed `_light` via endpoint_map.
    { "state",                 ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness",            ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "hue",                   ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "saturation",            ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_HALO_[] = {
    { 1, 0x0001 },  // genPowerCfg (battery)
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
    { 1, 0x0403 },  // msPressureMeasurement
    { 1, 0x0500 },  // ssIasZone (smoke + diagnostics)
    { 3, 0x0500 },  // ssIasZone (carbon_monoxide)
    { 2, 0x0006 },  // genOnOff   (light)
    { 2, 0x0008 },  // genLevelCtrl (light)
    { 2, 0x0300 },  // lightingColorCtrl (light)
};
constexpr ::zhc::EndpointLabel kEndpoints_HALO_[] = { {"light", 2} };

}  // namespace

extern const PreparedDefinition kDef_HALO_{
    .zigbee_models=kModels_HALO_, .zigbee_models_count=sizeof(kModels_HALO_)/sizeof(kModels_HALO_[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HALO+", .vendor="HaloSmartLabs",
    .meta=nullptr, .exposes=kExp_HALO_, .exposes_count=sizeof(kExp_HALO_)/sizeof(kExp_HALO_[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HALO_, .from_zigbee_count=sizeof(kFz_HALO_)/sizeof(kFz_HALO_[0]),
    .to_zigbee=kTz_HALO_, .to_zigbee_count=sizeof(kTz_HALO_)/sizeof(kTz_HALO_[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_HALO_,
    .bindings_count           = sizeof(kBind_HALO_)/sizeof(kBind_HALO_[0]),
    .endpoint_map             = kEndpoints_HALO_,
    .endpoint_map_count       = sizeof(kEndpoints_HALO_)/sizeof(kEndpoints_HALO_[0]),
    .default_endpoint         = 2,
};

}  // namespace zhc::devices::halo_smart_labs
