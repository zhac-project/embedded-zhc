// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes ZSS-QT-LTH-C 3-in-1 brightness / temperature / humidity sensor
// (z2m v26.92.0 parity, added v26.84.0).
// z2m-source: moes.ts #ZSS-QT-LTH-C  (fingerprint TS0222 / _TZ3000_ubuikmgo)
//
// Despite the TS0222 Tuya modelID this is NOT a datapoint device: upstream is
// `[m.battery(), m.temperature(), m.humidity(), m.illuminance()]`, i.e. plain
// ZCL measurement clusters. It reached the worklist only after the coverage
// check learned that sharing a family modelID is not evidence of coverage.
//
// Upstream also sets `configure: tuya.configureMagicPacket`. That is the
// interview-time Tuya wake-up read; it is a configure step, not decode, and
// the shared Tuya magic-packet configure lives on the tuya defs. Left off
// here — if this sensor turns out to need the poke to start reporting, wire
// ::zhc::tuya::extend::tuya_base_configure().
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::moes {
namespace {
const FzConverter* const kFz_ZSS_QT_LTH_C[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_ZSS_QT_LTH_C[] = { "TS0222" };
constexpr const char* kManus_ZSS_QT_LTH_C[]  = { "_TZ3000_ubuikmgo" };

constexpr Expose kExposes_ZSS_QT_LTH_C[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", "Measured temperature",       nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  "Measured relative humidity", nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", "Measured illuminance",       nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  "Battery percentage",         nullptr, 0},
};

constexpr BindingSpec kBindings_ZSS_QT_LTH_C[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0400},   // msIlluminanceMeasurement
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};
}  // namespace

extern const PreparedDefinition kDef_ZSS_QT_LTH_C{
    .zigbee_models=kModels_ZSS_QT_LTH_C, .zigbee_models_count=sizeof(kModels_ZSS_QT_LTH_C)/sizeof(kModels_ZSS_QT_LTH_C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZSS_QT_LTH_C, .manufacturer_names_count=sizeof(kManus_ZSS_QT_LTH_C)/sizeof(kManus_ZSS_QT_LTH_C[0]),
    .model="ZSS-QT-LTH-C", .vendor="Moes",
    .meta=nullptr, .exposes=kExposes_ZSS_QT_LTH_C, .exposes_count=sizeof(kExposes_ZSS_QT_LTH_C)/sizeof(kExposes_ZSS_QT_LTH_C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZSS_QT_LTH_C, .from_zigbee_count=sizeof(kFz_ZSS_QT_LTH_C)/sizeof(kFz_ZSS_QT_LTH_C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZSS_QT_LTH_C, .bindings_count=sizeof(kBindings_ZSS_QT_LTH_C)/sizeof(kBindings_ZSS_QT_LTH_C[0]),
};

}  // namespace zhc::devices::moes
