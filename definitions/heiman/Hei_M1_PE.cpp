// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Heiman M1-PE — smart occupancy sensor (PIR + radar).
// Ported (Tier 1, generic converters only): battery/voltage, occupancy (standard
// msOccupancySensing 0x0406), illuminance, temperature and humidity.
// Deferred: the radar/PIR tuning + status surface on heimanClusterSpecial (0xFC90)
// — target_distance, radar detection range, radar learning control/state,
// illuminance_threshold, pir_sensitivity_level, work_mode, work_indicator,
// device fault state, temperature/humidity offsets, reported/rejoined/rebooted
// counters — and the writable msOccupancySensing pirOToUDelay ("unoccupied_delay",
// no generic tz). None have a generic decoder/encoder in _shared.hpp.
// z2m-source: heiman.ts #M1-PE.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_M1_PE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_M1_PE[] = { "M1-PE" };

}  // namespace


constexpr Expose kExposes_M1_PE[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0400},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0406},
};

extern const PreparedDefinition kDef_M1_PE{
    .zigbee_models=kModels_M1_PE, .zigbee_models_count=sizeof(kModels_M1_PE)/sizeof(kModels_M1_PE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="M1-PE", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_M1_PE, .exposes_count=sizeof(kExposes_M1_PE)/sizeof(kExposes_M1_PE[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_M1_PE, .from_zigbee_count=sizeof(kFz_M1_PE)/sizeof(kFz_M1_PE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
