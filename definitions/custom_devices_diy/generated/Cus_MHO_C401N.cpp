// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy MHO-C401N — auto-generated.
// Xiaomi temperature & humidity sensor with custom firmware
// z2m-source: custom_devices_diy.ts #MHO-C401N.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: extend=[m.battery, m.temperature, m.humidity, m.enumLookup(temperatureDisplayMode)
// + m.enumLookup(showSmile) + comfort_* numerics]. Same family as LYWSD03MMC.
// TODO(parity): hvacUserInterfaceCfg comfort_*/display_mode attrs need tz_zcl_write_attr.
const FzConverter* const kFz_MHO_C401N[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_MHO_C401N[] = { "MHO-C401N" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
};

extern const PreparedDefinition kDef_MHO_C401N{
    .zigbee_models=kModels_MHO_C401N, .zigbee_models_count=sizeof(kModels_MHO_C401N)/sizeof(kModels_MHO_C401N[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MHO-C401N", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MHO_C401N, .from_zigbee_count=sizeof(kFz_MHO_C401N)/sizeof(kFz_MHO_C401N[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
