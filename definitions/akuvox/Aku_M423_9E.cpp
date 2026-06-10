// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Akuvox M423-9E — smart temperature & humidity sensor (TS0201).
//
// The auto-generated port (Tier 1) reduced this device to battery-only: it
// wired only the generic kFzBattery and exposed battery/voltage. But z2m's
// M423-9E declares `fromZigbee: [fz.temperature, fz.humidity, fz.battery]` and
// `exposes: [e.battery(), e.temperature(), e.humidity()]` — both the
// temperature and humidity channels (the entire reason this sensor exists)
// were dropped on decode.
//
// Fix: add the generic kFzTemperature (msTemperatureMeasurement 0x0402, /100)
// and kFzHumidity (msRelativeHumidity 0x0405, /100) decoders + the
// temperature & humidity exposes. z2m binds msTemperatureMeasurement +
// genPowerCfg on endpoint 1 and msRelativeHumidity on endpoint 2, so the
// bindings mirror that split. The generic converters use WILDCARD_ENDPOINT,
// so they decode regardless of source endpoint. temperature and humidity are
// distinct keys (not multi-gang), so no endpoint_map is needed.
//
// z2m-source: akuvox.ts #M423-9E.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::akuvox {
namespace {
const FzConverter* const kFz_M423_9E[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_M423_9E[] = { "TS0201" };
constexpr const char* kManus_M423_9E[] = { "_TYZB01_ujfk3xd9" };
}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},  // msTemperatureMeasurement (ep1)
    {1, 0x0001},  // genPowerCfg (ep1)
    {2, 0x0405},  // msRelativeHumidity (ep2)
};

extern const PreparedDefinition kDef_M423_9E{
    .zigbee_models=kModels_M423_9E, .zigbee_models_count=sizeof(kModels_M423_9E)/sizeof(kModels_M423_9E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_M423_9E, .manufacturer_names_count=sizeof(kManus_M423_9E)/sizeof(kManus_M423_9E[0]),
    .model="M423-9E", .vendor="Akuvox",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_M423_9E, .from_zigbee_count=sizeof(kFz_M423_9E)/sizeof(kFz_M423_9E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::akuvox
