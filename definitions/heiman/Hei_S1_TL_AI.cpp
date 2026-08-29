// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HEIMAN S1-TL-AI smart smoke alarm with temperature + humidity.
// New upstream in z2m v26.100.0.
//
// The alarm itself is a stock IAS Zone smoke sensor (alarm_1 / battery_low /
// test) and the climate sensors report on the standard measurement clusters —
// note humidity lives on ENDPOINT 2, not 1, which is why the def carries an
// endpoint map even though everything else is single-endpoint. Battery and
// the siren command are stock too.
//
// DEFERRED — the heimanClusterSpecial surface. Upstream layers six vendor
// extends on top (`heimanClusterSpecial`, `heimanClusterDeviceMuteState`,
// `iasZoneInitiateTestMode`, `heimanClusterSensorMutable`,
// `sirenForAutomationOnly`, `heimanRawData`) covering mute control, mute
// state, interconnect and a raw diagnostic feed. embedded-zhc has tz
// converters for several of those (see definitions/heiman/_shared.hpp) but no
// decode-side map for this model's variant of the cluster, and guessing the
// attribute set on a life-safety device is not the place to be speculative.
// Alarm, test, battery, temperature and humidity — the parts that matter for
// detection — are fully wired.
//
// z2m-source: heiman.ts #S1-TL-AI.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {

const FzConverter* const kFz_S1_TL_AI[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
const TzConverter* const kTz_S1_TL_AI[] = {
    &::zhc::generic::kTzWarning,
};

constexpr const char* kModels_S1_TL_AI[] = { "S1-TL-AI", "SC6-EF2-AI" };

// Deliberately NO endpoint_map: humidity reports from endpoint 2 and
// everything else from endpoint 1, but the device has one logical sensor of
// each kind, so the keys should stay bare. Declaring a map would make
// dispatch.cpp suffix every non-global key by its source endpoint and publish
// `humidity_...` instead of `humidity`. The endpoint-2 bind is listed directly
// in kBindings instead, which is all the device needs to start reporting.
constexpr Expose kExposes_S1_TL_AI[] = {
    {"smoke",       ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"test",        ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_S1_TL_AI[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0500},   // ssIasZone
    {1, 0x0502},   // ssIasWd — siren
    {2, 0x0405},   // msRelativeHumidity — endpoint 2 on this model
};

}  // namespace

extern const PreparedDefinition kDef_S1_TL_AI{
    .zigbee_models=kModels_S1_TL_AI, .zigbee_models_count=sizeof(kModels_S1_TL_AI)/sizeof(kModels_S1_TL_AI[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S1-TL-AI", .vendor="Heiman",
    .meta=nullptr,
    .exposes=kExposes_S1_TL_AI, .exposes_count=sizeof(kExposes_S1_TL_AI)/sizeof(kExposes_S1_TL_AI[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_S1_TL_AI, .from_zigbee_count=sizeof(kFz_S1_TL_AI)/sizeof(kFz_S1_TL_AI[0]),
    .to_zigbee=kTz_S1_TL_AI, .to_zigbee_count=sizeof(kTz_S1_TL_AI)/sizeof(kTz_S1_TL_AI[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_S1_TL_AI, .bindings_count=sizeof(kBindings_S1_TL_AI)/sizeof(kBindings_S1_TL_AI[0]),
};

}  // namespace zhc::devices::heiman
