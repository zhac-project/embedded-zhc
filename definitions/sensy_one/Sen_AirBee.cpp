// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sensy-One AirBee air quality sensor (zigbeeModel AirBee), new vendor in
// z2m v26.126.0..v26.133.0. m.temperature + m.humidity + m.pressure + m.co2, with
// their default reporting (10 s .. 1 h on 1 C / 1 % / 5 hPa / 50 ppm; the co2
// change is a single float, 0.00005 mole fraction). Generic converters only.
// Not ported: identify.
// z2m-source: sensy_one.ts #AirBee
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sensy_one {
namespace {
const FzConverter* const kFz_AirBee[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
    &::zhc::generic::kFzCO2,
};
constexpr const char* kModels_AirBee[] = { "AirBee" };
constexpr Expose kExposes_AirBee[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"pressure", ExposeType::Numeric, Access::State, "hPa", nullptr, nullptr, 0},
    {"co2", ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
};
constexpr BindingSpec kBindings_AirBee[] = {
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0403},
    {1, 0x040D},
};
constexpr ReportingSpec kReports_AirBee[] = {
    {1, 0x0402, 0x0000, 0x29, 10, 3600, 100, 0},
    {1, 0x0405, 0x0000, 0x21, 10, 3600, 100, 0},
    {1, 0x0403, 0x0000, 0x29, 10, 3600, 5, 0},
    {1, 0x040D, 0x0000, 0x39, 10, 3600, 0x3851B717u /* 0.00005f */, 0},
};
}  // namespace

extern const PreparedDefinition kDef_AirBee{
    .zigbee_models=kModels_AirBee, .zigbee_models_count=sizeof(kModels_AirBee)/sizeof(kModels_AirBee[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AirBee", .vendor="Sensy-One",
    .meta=nullptr, .exposes=kExposes_AirBee, .exposes_count=sizeof(kExposes_AirBee)/sizeof(kExposes_AirBee[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AirBee, .from_zigbee_count=sizeof(kFz_AirBee)/sizeof(kFz_AirBee[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_AirBee, .bindings_count=sizeof(kBindings_AirBee)/sizeof(kBindings_AirBee[0]),
    .reports=kReports_AirBee, .reports_count=sizeof(kReports_AirBee)/sizeof(kReports_AirBee[0]),
};

}  // namespace zhc::devices::sensy_one
