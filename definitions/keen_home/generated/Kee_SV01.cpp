// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: KeenHome SV01 — smart vent. Position is reported on
// genLevelCtrl currentLevel (0..255) rather than closuresWindowCovering;
// z2m wires fz.cover_position_via_brightness + tz.cover_via_brightness
// for that, plus fz.battery / fz.temperature / fz.keen_home_smart_vent_pressure
// (the last is plain msPressureMeasurement attr 0x0000).
// `fz.ignore_onoff_report` is a no-op claim in z2m to suppress spurious
// genOnOff reports — no equivalent needed here, the runtime drops
// unmatched frames anyway and we don't bind genOnOff.
// z2m-source: keen_home.ts #SV01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/keen_home/_shared.hpp"

namespace zhc::devices::keen_home {
namespace {
const FzConverter* const kFz_SV01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::devices::keen_home::kFzKeenCoverPosition,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzPressure,
};
const TzConverter* const kTz_SV01[] = {
    &::zhc::devices::keen_home::kTzKeenCoverPosition,
};
constexpr const char* kModels_SV01[] = {
    "SV01-410-MP-1.0", "SV01-410-MP-1.1", "SV01-410-MP-1.4", "SV01-410-MP-1.5",
    "SV01-412-MP-1.0", "SV01-412-MP-1.1", "SV01-412-MP-1.3", "SV01-412-MP-1.4",
    "SV01-610-MP-1.0", "SV01-610-MP-1.1", "SV01-612-MP-1.0", "SV01-612-MP-1.1",
    "SV01-612-MP-1.2", "SV01-610-MP-1.4", "SV01-612-MP-1.4", "SV01-612-EP-1.4",
};

constexpr Expose kExp_SV01[] = {
    {"battery",     ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, ::zhc::Access::State,    "mV",  nullptr, nullptr, 0},
    {"position",    ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0},
    {"state",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, ::zhc::Access::State,    "C",   nullptr, nullptr, 0},
    {"pressure",    ExposeType::Numeric, ::zhc::Access::State,    "hPa", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_SV01[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0008},   // genLevelCtrl  (cover-via-brightness)
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0403},   // msPressureMeasurement
};
}  // namespace

extern const PreparedDefinition kDef_SV01{
    .zigbee_models=kModels_SV01,
    .zigbee_models_count=sizeof(kModels_SV01)/sizeof(kModels_SV01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SV01", .vendor="KeenHome",
    .meta=nullptr,
    .exposes=kExp_SV01, .exposes_count=sizeof(kExp_SV01)/sizeof(kExp_SV01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SV01, .from_zigbee_count=sizeof(kFz_SV01)/sizeof(kFz_SV01[0]),
    .to_zigbee=kTz_SV01,   .to_zigbee_count=sizeof(kTz_SV01)/sizeof(kTz_SV01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SV01,
    .bindings_count=sizeof(kBind_SV01)/sizeof(kBind_SV01[0]),
};

}  // namespace zhc::devices::keen_home
