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
// z2m-source: heiman.ts #HS9MS-E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS9MS_E[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_HS9MS_E[] = { "HS9MS-E" };

}  // namespace


constexpr Expose kExposes_HS9MS_E[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
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
