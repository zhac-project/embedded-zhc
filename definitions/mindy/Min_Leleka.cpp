// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: MindY Leleka — graduated from generated/ 2026-06-10 to wire the
// CO2 decoder (parity fix; the kFzCO2 generic now exists). Hand-rewritten
// 2026-04-28 (was wrong-bundle).
// Advanced Environmental Monitoring Device (T/H/P/CO2/lux + dimmable LED).
// z2m-source: mindy.ts #Leleka.
//
// Coverage shape:
//   - kFzTemperature  → msTemperatureMeasurement (0x0402)
//   - kFzHumidity     → msRelativeHumidity       (0x0405)
//   - kFzPressure     → msPressureMeasurement    (0x0403)
//   - kFzCO2          → msCO2                     (0x040D)  (CO2 ppm)
//   - kFzIlluminance  → msIlluminanceMeasurement (0x0400)
//   - kFzOnOff        → genOnOff                 (0x0006)  (LED indicator)
//   - kFzBrightness   → genLevelCtrl             (0x0008)  (LED dim)
//   - kTzOnOff / kTzBrightness                              (LED control)
//
// Gaps vs z2m (kept as exposes for shadow surfacing):
//   - `temperature_sensor` enumLookup (CPU/SCD4X/BMP280) + all
//     `customAttrIds` config attributes (read_interval, night_mode,
//     night_on/off_time, co2_*, lux_*, offset_*) ride manuf-specific
//     genBasic / msCO2 / msIlluminance / ms* attribute reads/writes
//     that no Tier-1 generic decoder covers today. Listed in exposes
//     so the shadow can surface them; round-trip writes need an
//     attribute-write Tz path (out of scope for the parity sweep).
//   - `fzLocal.last_boot` (genTime.lastSetTime epoch-2000 → ISO string)
//     and `fzLocal.wifi` (genBasic.locationDesc → ip_address + wifi
//     ON/OFF). Per-device fz; not Tier-1 reusable.
//   - `tzLocal.last_boot` (writes genTime.time as epoch-2000 seconds)
//     and `tzLocal.wifi` (writes genBasic.locationDesc with WiFi creds);
//     per-device Tz. Out of scope for Tier-1.
//   - `m.poll(last_boot_update)` — reads `genTime.lastSetTime` +
//     `genBasic.locationDesc` on a configurable interval; ZHC has no
//     polling primitive yet.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mindy {
namespace {

const FzConverter* const kFz_Leleka[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
    &::zhc::generic::kFzCO2,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};

const TzConverter* const kTz_Leleka[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};

constexpr const char* kModels_Leleka[] = { "Leleka" };

constexpr Expose kExp_Leleka[] = {
    // Environmental sensors.
    { "temperature", ExposeType::Numeric, ::zhc::Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "pressure",    ExposeType::Numeric, ::zhc::Access::State,    "hPa", nullptr, nullptr, 0 },
    { "co2",         ExposeType::Numeric, ::zhc::Access::State,    "ppm", nullptr, nullptr, 0 },
    { "illuminance", ExposeType::Numeric, ::zhc::Access::State,    "lx",  nullptr, nullptr, 0 },
    // Indicator LED (m.light, configureReporting:true, no PoB / effect).
    { "state",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness",  ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    // Sensor-config knobs (genBasic manuf-specific).
    { "read_interval",  ExposeType::Numeric, ::zhc::Access::StateSet, "seconds", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "night_mode",     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "night_on_time",  ExposeType::Numeric, ::zhc::Access::StateSet, "Hr",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "night_off_time", ExposeType::Numeric, ::zhc::Access::StateSet, "Hr",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    // CO2 calibration / threshold knobs (msCO2 manuf-specific).
    { "co2_control",              ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "co2_invert",               ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "co2_level_high",           ExposeType::Numeric, ::zhc::Access::StateSet, "ppm", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "co2_level_low",            ExposeType::Numeric, ::zhc::Access::StateSet, "ppm", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "co2_auto_calibration",     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "co2_forced_recalibration", ExposeType::Numeric, ::zhc::Access::StateSet, "ppm", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "co2_factory_reset",        ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    // Illuminance threshold knobs (msIlluminanceMeasurement manuf-specific).
    { "lux_control",    ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "lux_invert",     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "lux_level_high", ExposeType::Numeric, ::zhc::Access::StateSet, "lx",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "lux_level_low",  ExposeType::Numeric, ::zhc::Access::StateSet, "lx",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    // Offset trims (signed s16).
    { "offset_illuminance", ExposeType::Numeric, ::zhc::Access::StateSet, "lx",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "offset_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, "\xC2\xB0""C", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "offset_humidity",    ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "offset_pressure",    ExposeType::Numeric, ::zhc::Access::StateSet, "hPa", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    // Per-device fz/tz exposes (last_boot, wifi, ip_address).
    { "last_boot",  ExposeType::String,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "wifi",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "ip_address", ExposeType::String,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
};

constexpr BindingSpec kBind_Leleka[] = {
    { 1, 0x0006 }, // genOnOff
    { 1, 0x0008 }, // genLevelCtrl
    { 1, 0x0400 }, // msIlluminanceMeasurement
    { 1, 0x0402 }, // msTemperatureMeasurement
    { 1, 0x0403 }, // msPressureMeasurement
    { 1, 0x0405 }, // msRelativeHumidity
    { 1, 0x040D }, // msCO2 (kFzCO2)
};

}  // namespace

extern const PreparedDefinition kDef_Leleka{
    .zigbee_models=kModels_Leleka, .zigbee_models_count=sizeof(kModels_Leleka)/sizeof(kModels_Leleka[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Leleka", .vendor="MindY",
    .meta=nullptr, .exposes=kExp_Leleka, .exposes_count=sizeof(kExp_Leleka)/sizeof(kExp_Leleka[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Leleka, .from_zigbee_count=sizeof(kFz_Leleka)/sizeof(kFz_Leleka[0]),
    .to_zigbee=kTz_Leleka, .to_zigbee_count=sizeof(kTz_Leleka)/sizeof(kTz_Leleka[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Leleka, .bindings_count=sizeof(kBind_Leleka)/sizeof(kBind_Leleka[0]),
};

}  // namespace zhc::devices::mindy
