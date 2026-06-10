// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Konke KK-ES-J01W — temperature, humidity and illuminance sensor
//         (hand-rewritten).
// z2m-source: konke.ts #KK-ES-J01W
//   (fromZigbee: [fz.battery, fz.humidity, fz.temperature];
//    extend: [m.illuminance()];
//    exposes: [e.battery(), e.battery_voltage(), e.humidity(), e.temperature()]).
// The Tier-1 auto-port dropped every sensor channel — it wired only
// kFzBattery and exposed only battery/voltage, so temperature, humidity and
// illuminance reports were silently discarded. Restored
// msTemperatureMeasurement (0x0402), msRelativeHumidity (0x0405) and
// msIlluminanceMeasurement (0x0400) decoders + the matching exposes.
//
// NOTE on fingerprint: z2m gates modelID "TS0222" to manufacturerName
// "_TYZB01_fi5yftwv" while leaving "3AFE090103021000" ungated. The matcher's
// manufacturer gating is whole-def (applies to all zigbee_models), so the
// "_TYZB01_fi5yftwv" restriction cannot be expressed per-model here without
// also gating the 3AFE model. Left model-only to match the pre-existing
// behaviour; see gaps[] for the per-model fingerprint INFRA note.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::konke {
namespace {
const FzConverter* const kFz_KK_ES_J01W[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_KK_ES_J01W[] = { "TS0222", "3AFE090103021000" };

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0400},  // msIlluminanceMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_KK_ES_J01W{
    .zigbee_models=kModels_KK_ES_J01W, .zigbee_models_count=sizeof(kModels_KK_ES_J01W)/sizeof(kModels_KK_ES_J01W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="KK-ES-J01W", .vendor="Konke",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_KK_ES_J01W, .from_zigbee_count=sizeof(kFz_KK_ES_J01W)/sizeof(kFz_KK_ES_J01W[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::konke
