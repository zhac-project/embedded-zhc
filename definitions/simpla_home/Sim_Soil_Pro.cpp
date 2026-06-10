// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SimplaHome Soil Pro — graduated from generated/ to wire the
// dropped sensor channels.
//
// z2m-source: simpla_home.ts #Soil Pro.
//
// Bug fixed (MISSING decoder/channel + wrong endpoint_map):
//   The auto-generated def was a battery-only stub. z2m's Soil Pro is a
//   multi-sensor: m.temperature() + m.illuminance() on the default
//   endpoint, two m.soilMoisture() instances on the z1_top (ep 2) and
//   z2_bottom (ep 3) endpoints, plus m.battery(). The generated def
//   dropped temperature, illuminance and BOTH soil_moisture channels,
//   and its endpoint_map placed z1_top/z2_bottom at endpoint 1 (so the
//   two soil zones would have collided on a bare "soil_moisture" key
//   even if the decoder had been wired).
//
//   Now wired: kFzTemperature (msTemperatureMeasurement 0x0402),
//   kFzIlluminance (msIlluminanceMeasurement 0x0400), kFzSoilMoisture
//   (msSoilMoisture 0x0408) and kFzBattery (genPowerCfg 0x0001). The
//   endpoint_map is corrected to {z1_top: 2, z2_bottom: 3} so the two
//   zones decode to suffixed runtime keys soil_moisture_z1_top /
//   soil_moisture_z2_bottom. Endpoint 1 is deliberately NOT in the map
//   so temperature/illuminance keep their bare keys (z2m has no
//   endpointNames on those — they publish unsuffixed).
//
// Deferred (INFRA — no existing converter):
//   * measurement_interval (genAnalogOutput presentValue, ACCESS ALL
//     config write) and linear_mode (genBinaryOutput presentValue config
//     write) are read/write config attributes with no generic converter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::simpla_home {
namespace {
const FzConverter* const kFz_Soil_Pro[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzSoilMoisture,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_Soil_Pro[] = { "Soil Pro" };

// z2m deviceEndpoints {"1": 1, z1_top: 2, z2_bottom: 3}. Only the two
// soil-zone endpoints carry a label: the default endpoint 1 stays out of
// the map so its temperature/illuminance keys are NOT suffixed (mirrors
// z2m, which suffixes only the endpointNames-tagged soilMoisture).
constexpr ::zhc::EndpointLabel kEndpoints_Soil_Pro[] = {
    {"z1_top", 2}, {"z2_bottom", 3}
};

}  // namespace


// Exposes stay bare; dispatch suffixes the per-endpoint soil_moisture
// runtime keys to soil_moisture_z1_top / soil_moisture_z2_bottom.
constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},   // temperature  (endpoint 1)
    {1, 0x0400},   // illuminance  (endpoint 1)
    {2, 0x0408},   // soil_moisture z1_top
    {3, 0x0408},   // soil_moisture z2_bottom
    {1, 0x0001},   // battery / power config
};

extern const PreparedDefinition kDef_Soil_Pro{
    .zigbee_models=kModels_Soil_Pro, .zigbee_models_count=sizeof(kModels_Soil_Pro)/sizeof(kModels_Soil_Pro[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Soil Pro", .vendor="SimplaHome",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Soil_Pro, .from_zigbee_count=sizeof(kFz_Soil_Pro)/sizeof(kFz_Soil_Pro[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_Soil_Pro,
    .endpoint_map_count = sizeof(kEndpoints_Soil_Pro)/sizeof(kEndpoints_Soil_Pro[0]),
};

}  // namespace zhc::devices::simpla_home
