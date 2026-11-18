// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Heiman HS9MS-E — smart motion sensor.
// Ported (Tier 1, generic converters only): battery/voltage and IAS occupancy
// alarm (zoneType "occupancy" → key "occupancy") with tamper + battery_low.
// Deferred: the entire heimanClusterSpecial (0xFC90) surface this device layers on
// — picture_quantity/quality, mcu_software_version, illuminance_threshold,
// sensor_armed, pir_sensitivity_level, wifi_status/candidate, server_status/
// candidate, camera_ready, wifi_rssi, plus the legacy-illuminance, test-trigger,
// active-trigger and wifi-exposure extends. None have a generic decoder.
// heimanClusterLegacyIlluminanceExtend IS ported (pre-existing gap, R10):
// msIlluminanceMeasurement measuredValue published unscaled as `ambient_light`
// (z2m unit "Lx", no lux conversion). Upstream neither binds nor reports it;
// its configure-time read and the `get` are not ported.
// z2m-source: heiman.ts #HS9MS-E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
constexpr ::zhc::generic::ZclAttrRow kAmbientLightRows_HS9MS_E[] = { { 0x0000, "ambient_light" } };
constexpr ::zhc::generic::ZclAttrMap kAmbientLightMap_HS9MS_E{
    kAmbientLightRows_HS9MS_E, sizeof(kAmbientLightRows_HS9MS_E)/sizeof(kAmbientLightRows_HS9MS_E[0]) };
constexpr FzConverter kFzAmbientLight_HS9MS_E =
    ::zhc::generic::zcl_attr_fz("msIlluminanceMeasurement", &kAmbientLightMap_HS9MS_E);

const FzConverter* const kFz_HS9MS_E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
    &kFzAmbientLight_HS9MS_E,
};

constexpr const char* kModels_HS9MS_E[] = { "HS9MS-E" };

}  // namespace


constexpr Expose kExposes_HS9MS_E[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"ambient_light", ExposeType::Numeric, Access::State, "Lx", "ambient illuminance in lux", nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_HS9MS_E{
    .zigbee_models=kModels_HS9MS_E, .zigbee_models_count=sizeof(kModels_HS9MS_E)/sizeof(kModels_HS9MS_E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS9MS-E", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS9MS_E, .exposes_count=sizeof(kExposes_HS9MS_E)/sizeof(kExposes_HS9MS_E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS9MS_E, .from_zigbee_count=sizeof(kFz_HS9MS_E)/sizeof(kFz_HS9MS_E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
