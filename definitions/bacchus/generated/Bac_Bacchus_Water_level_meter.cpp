// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bacchus Bacchus Water level meter — generic converters only.
// Tank water level sensor with built-in pump relay.
// z2m-source: bacchus.ts #Bacchus Water level meter.
//
// Wire shape from z2m source:
//   - genAnalogInput (0x000C):
//       presentValue        → water_level (cm, scale 10)
//       outOfService        → out_of_service (bool)
//       manu 0xf006 single  → filling (%)
//       manu 0xf005 single  → tank_height (cm, scale 10) [write]
//       manu 0xf007 u16     → measurment_period (min) [write]
//       minPresentValue/maxPresentValue → low/high alarm thresholds [write]
//   - msTemperatureMeasurement (0x0402) → temperature.
//   - genOnOff (0x0006) → pump_relay state + manu 0xf008 invert_threshold.
//
// Coverage in this Tier-1 port:
//   - kFzTemperature → temperature (only attr Tier-1 generic decodes here).
//   - kFzOnOff       → pump_relay state.
//
// BLOCKED: every analog-input attribute (water_level, filling, tank_height,
// out_of_service, alarm thresholds, period) is on cluster 0x000C — there's
// no `kFzAnalogInput` / `kFzPresentValue` in Tier-1 generic today. The
// exposes are listed for parity so the SPA can render the placeholders
// once a generic analog-input decoder lands.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {
const FzConverter* const kFz_Bacchus_Water_level_meter[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzOnOff,
};

constexpr const char* kModels_Bacchus_Water_level_meter[] = { "Bacchus Water level meter" };

constexpr Expose kExp_Bacchus_Water_level_meter[] = {
    { "water_level",      ExposeType::Numeric, ::zhc::Access::State,    "cm",  "Current water level in cm",  nullptr, 0 },
    { "out_of_service",   ExposeType::Binary,  ::zhc::Access::State,    nullptr, "Level is out of service",  nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "filling",          ExposeType::Numeric, ::zhc::Access::State,    "%",   "Tank filling",               nullptr, 0 },
    { "temperature",      ExposeType::Numeric, ::zhc::Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "tank_height",      ExposeType::Numeric, ::zhc::Access::StateSet, "cm",  "Water tank height in cm",    nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "min_threshold",    ExposeType::Numeric, ::zhc::Access::StateSet, "cm",  "Min threshold for alarm binding", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "max_threshold",    ExposeType::Numeric, ::zhc::Access::StateSet, "cm",  "Max threshold for alarm binding", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "invert_threshold", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Invert thresholds for on/off commands", nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "measurment_period",ExposeType::Numeric, ::zhc::Access::StateSet, "min", "Measurement period",         nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "pump_relay",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Internal pump relay",      nullptr, 0 },
};

constexpr BindingSpec kBind_Bacchus_Water_level_meter[] = {
    { 1, 0x000C }, // genAnalogInput (BLOCKED — no Tier-1 decoder)
    { 1, 0x0402 }, // msTemperatureMeasurement
    { 1, 0x0006 }, // genOnOff
};

}  // namespace

extern const PreparedDefinition kDef_Bacchus_Water_level_meter{
    .zigbee_models=kModels_Bacchus_Water_level_meter, .zigbee_models_count=sizeof(kModels_Bacchus_Water_level_meter)/sizeof(kModels_Bacchus_Water_level_meter[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Bacchus Water level meter", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Bacchus_Water_level_meter, .exposes_count=sizeof(kExp_Bacchus_Water_level_meter)/sizeof(kExp_Bacchus_Water_level_meter[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Bacchus_Water_level_meter, .from_zigbee_count=sizeof(kFz_Bacchus_Water_level_meter)/sizeof(kFz_Bacchus_Water_level_meter[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Bacchus_Water_level_meter, .bindings_count=sizeof(kBind_Bacchus_Water_level_meter)/sizeof(kBind_Bacchus_Water_level_meter[0]),
};

}  // namespace zhc::devices::bacchus
