// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bacchus Presence_Sensor_v2 — uses lumi shared kFzOccupancy.
// 3-endpoint presence + illuminance sensor.
// z2m-source: bacchus.ts #Presence_Sensor_v2.
//
// Coverage shape:
//   - kFzOnOff      → genOnOff (0x0006) — main presence state on EP1.
//   - kFzOccupancy  → msOccupancySensing (0x0406, attr 0x0000 bitmap8) — z2m
//                     publishes `occupancy=true` when bit0 set. Lives in the
//                     `zhc::lumi` namespace because lumi was the first vendor
//                     to need it; it's vendor-neutral wire decoding.
//   - kFzIlluminance→ msIlluminanceMeasurement (0x0400) — raw lx.
//   - kFzBattery    → genPowerCfg (0x0001) — battery + voltage.
//
// Gaps (not in Tier-1/2 generic):
//   - `local_time`           — genTime localTime read-only state-get.
//   - `min_time` / `max_time`/`day_output`/`night_output` — genTime manu
//     attrs surfaced via `timeHHMM` helper. Need a `bacchus_shared` helper
//     to encode HH:MM → seconds.
//   - `led_mode`             — genOnOff manu attr 0xf004 enum8 (Always/
//     Never/Night) on EP3. ManuSpec write — needs vendor TZ helper.
//   - `illuminance_threshold`— msIlluminanceLevelSensing manu attr 0x0010.
//     Read-only published; write needs vendor TZ helper.
//   These are listed in exposes for discoverability but aren't decoded by
//   the Tier-1 generic decoders today.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {

const FzConverter* const kFz_Presence_Sensor_v2[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_Presence_Sensor_v2[] = { "Presence_Sensor_v2" };

constexpr Expose kExp_Presence_Sensor_v2[] = {
    { "occupancy",              ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "illuminance",            ExposeType::Numeric, ::zhc::Access::State,    "lx",    nullptr, nullptr, 0 },
    { "state",                  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery",                ExposeType::Numeric, ::zhc::Access::State,    "%",     nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",                ExposeType::Numeric, ::zhc::Access::State,    "mV",    nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "local_time",             ExposeType::String,  ::zhc::Access::State,    nullptr, "Local time (HH:MM)", nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "led_mode",               ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Led working mode",   nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "illuminance_threshold",  ExposeType::Numeric, ::zhc::Access::StateSet, "raw",   "Illuminance threshold", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "min_time",               ExposeType::String,  ::zhc::Access::StateSet, nullptr, "Day start (HH:MM)",  nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "max_time",               ExposeType::String,  ::zhc::Access::StateSet, nullptr, "Day end (HH:MM)",    nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "day_output",             ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Output during the day",   nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "night_output",           ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Output during the night", nullptr, 0,
      ::zhc::ExposeCategory::Config },
};

constexpr BindingSpec kBind_Presence_Sensor_v2[] = {
    { 1, 0x0001 }, // genPowerCfg
    { 1, 0x0006 }, // genOnOff
    { 1, 0x0406 }, // msOccupancySensing
    { 1, 0x0400 }, // msIlluminanceMeasurement
    { 3, 0x0006 }, // led_mode endpoint
};

constexpr ::zhc::EndpointLabel kEndpoints_Presence_Sensor_v2[] = { {"1", 1} };

}  // namespace

extern const PreparedDefinition kDef_Presence_Sensor_v2{
    .zigbee_models=kModels_Presence_Sensor_v2, .zigbee_models_count=sizeof(kModels_Presence_Sensor_v2)/sizeof(kModels_Presence_Sensor_v2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Presence_Sensor_v2", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Presence_Sensor_v2, .exposes_count=sizeof(kExp_Presence_Sensor_v2)/sizeof(kExp_Presence_Sensor_v2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Presence_Sensor_v2, .from_zigbee_count=sizeof(kFz_Presence_Sensor_v2)/sizeof(kFz_Presence_Sensor_v2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Presence_Sensor_v2, .bindings_count=sizeof(kBind_Presence_Sensor_v2)/sizeof(kBind_Presence_Sensor_v2[0]),
    .endpoint_map       = kEndpoints_Presence_Sensor_v2,
    .endpoint_map_count = sizeof(kEndpoints_Presence_Sensor_v2)/sizeof(kEndpoints_Presence_Sensor_v2[0]),
};

}  // namespace zhc::devices::bacchus
