// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared bosch converters.
//
// z2m-source: zigbee-herdsman-converters/src/lib/bosch.ts
//
// Bosch uses ZCL manufacturer-specific extensions on standard clusters
// (mostly hvacThermostat 0x0201 + hvacUserInterfaceCfg 0x0204) plus a
// few wholly custom clusters (boschEnergyDevice 0xFCA0,
// boschDoorWindowContactCluster 0xFCAD, boschWaterAlarm 0xFCAC,
// twinguardSmokeDetector 0xE000, …). Mfgcode = 0x1209
// (ROBERT_BOSCH_GMBH).
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {

// ── Light bundle (kept for ABI compatibility — see header) ──────────
const ::zhc::FzConverter* const kFzBoschLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzBoschLightCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschLight)/sizeof(kFzBoschLight[0]));
const ::zhc::TzConverter* const kTzBoschLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzBoschLightCount =
    static_cast<std::uint8_t>(sizeof(kTzBoschLight)/sizeof(kTzBoschLight[0]));
const ::zhc::Expose kExposesBoschLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired",  nullptr, nullptr, 0 },
};
const std::uint8_t kExposesBoschLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschLight)/sizeof(kExposesBoschLight[0]));
const ::zhc::BindingSpec kBindingsBoschLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsBoschLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsBoschLight)/sizeof(kBindingsBoschLight[0]));

// ── IAS battery sensor bundle ───────────────────────────────────────
const ::zhc::FzConverter* const kFzBoschIasBattery[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
};
const std::uint8_t kFzBoschIasBatteryCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschIasBattery)/sizeof(kFzBoschIasBattery[0]));
const ::zhc::Expose kExposesBoschIasBattery[] = {
    { "battery",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "battery_low", ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "alarm",       ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesBoschIasBatteryCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschIasBattery)/sizeof(kExposesBoschIasBattery[0]));
const ::zhc::BindingSpec kBindingsBoschIasBattery[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0500 },  // ssIasZone
};
const std::uint8_t kBindingsBoschIasBatteryCount =
    static_cast<std::uint8_t>(sizeof(kBindingsBoschIasBattery)/sizeof(kBindingsBoschIasBattery[0]));

// ── Smart-plug bundle ───────────────────────────────────────────────
const ::zhc::FzConverter* const kFzBoschPlug[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
};
const std::uint8_t kFzBoschPlugCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschPlug)/sizeof(kFzBoschPlug[0]));
const ::zhc::TzConverter* const kTzBoschPlug[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzBoschPlugCount =
    static_cast<std::uint8_t>(sizeof(kTzBoschPlug)/sizeof(kTzBoschPlug[0]));
const ::zhc::Expose kExposesBoschPlug[] = {
    { "state",   ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,  nullptr, nullptr, 0 },
    { "energy",  ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "kWh",    nullptr, nullptr, 0 },
    { "power",   ::zhc::ExposeType::Numeric, ::zhc::Access::State,    "W",      nullptr, nullptr, 0 },
};
const std::uint8_t kExposesBoschPlugCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschPlug)/sizeof(kExposesBoschPlug[0]));
const ::zhc::BindingSpec kBindingsBoschPlug[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0702 },  // seMetering
    { 1, 0x0B04 },  // haElectricalMeasurement
};
const std::uint8_t kBindingsBoschPlugCount =
    static_cast<std::uint8_t>(sizeof(kBindingsBoschPlug)/sizeof(kBindingsBoschPlug[0]));

// ── BTH-RA TRV bundle ───────────────────────────────────────────────
const ::zhc::FzConverter* const kFzBoschTrv[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzBattery,
};
const std::uint8_t kFzBoschTrvCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschTrv)/sizeof(kFzBoschTrv[0]));

// `kTzBoschTrv` populated below the manu-spec block so the converters
// it references already exist at the point of array initialisation.

const ::zhc::Expose kExposesBoschTrv[] = {
    // Standard thermostat (cluster 0x0201) attributes.
    { "local_temperature",          ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "current_heating_setpoint",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "system_mode",                ::zhc::ExposeType::Enum,    ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "running_state",              ::zhc::ExposeType::Enum,    ::zhc::Access::State,
      nullptr, nullptr, nullptr, 0 },
    // Manu-specific extras.
    { "operating_mode",             ::zhc::ExposeType::Enum,    ::zhc::Access::StateSet,
      nullptr, "schedule | manual | pause", nullptr, 0 },
    { "boost_heating",              ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "window_open",                ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "valve_adapt_status",         ::zhc::ExposeType::Enum,    ::zhc::Access::State,
      nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "remote_temperature",         ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "\xC2\xB0""C", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "child_lock",                 ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "display_orientation",        ::zhc::ExposeType::Enum,    ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "displayed_temperature",      ::zhc::ExposeType::Enum,    ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "display_brightness",         ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "display_ontime",             ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "s", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "battery",                    ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "%", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "battery_low",                ::zhc::ExposeType::Binary,  ::zhc::Access::State,
      nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
};
const std::uint8_t kExposesBoschTrvCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschTrv)/sizeof(kExposesBoschTrv[0]));
const ::zhc::BindingSpec kBindingsBoschTrv[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0201 },  // hvacThermostat
    { 1, 0x0204 },  // hvacUserInterfaceCfg
};
const std::uint8_t kBindingsBoschTrvCount =
    static_cast<std::uint8_t>(sizeof(kBindingsBoschTrv)/sizeof(kBindingsBoschTrv[0]));

// ── Manu-specific TZ specs (mfgcode 0x1209 ROBERT_BOSCH_GMBH) ───────
// Encoded by `tz_zcl_write_attr` which prepends fc=0x14 + mfgcode when
// `manufacturer_code` is non-zero. ZCL data-type byte values follow the
// foundation spec: 0x10 BOOL, 0x18 BITMAP8, 0x20 UINT8, 0x21 UINT16,
// 0x22 UINT24, 0x23 UINT32, 0x29 INT16, 0x30 ENUM8.
namespace {
constexpr std::uint16_t kBoschMfg = 0x1209;

// hvacThermostat (0x0201) manu-specific attributes ─────────────────
constexpr ::zhc::generic::ZclWriteSpec kSpecOperatingMode{
    "operating_mode",       0x4007, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecValveAdaptStatus{
    "valve_adapt_status",   0x4022, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRemoteTemperature{
    "remote_temperature",   0x4040, 0x29, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecWindowOpenMode{
    "window_open",          0x4042, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecBoostHeating{
    "boost_heating",        0x4043, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecValveType{
    "valve_type",           0x4060, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecHeaterType{
    "heater_type",          0x4063, 0x30, kBoschMfg, nullptr, 0,
};

// hvacUserInterfaceCfg (0x0204) manu-specific attributes ───────────
constexpr ::zhc::generic::ZclWriteSpec kSpecDisplayOrientation{
    "display_orientation",  0x400B, 0x20, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecActivityLed{
    "activity_led",         0x4033, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDisplayedTemperature{
    "displayed_temperature", 0x4039, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDisplaySwitchDur{
    "display_ontime",       0x403A, 0x30, kBoschMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDisplayBrightness{
    "display_brightness",   0x403B, 0x30, kBoschMfg, nullptr, 0,
};

// Standard hvacUserInterfaceCfg keypadLockout (attr 0x0001) — not
// manu-specific, but we expose it as `child_lock` for parity with
// z2m's BTH-RA / BTH-RM mapping.
constexpr ::zhc::generic::ZclWriteSpec kSpecChildLockUi{
    "child_lock",           0x0001, 0x30, /*mfg=*/0, nullptr, 0,
};

// Twinguard sensitivity — custom cluster 0xE000 attr 0x4003 UINT16.
constexpr ::zhc::generic::ZclWriteSpec kSpecTwinguardSensitivity{
    "sensitivity",          0x4003, 0x21, kBoschMfg, nullptr, 0,
};
}  // namespace

#define ZHC_BOSCH_TZ(var, spec_ref, key_str, cluster_str, cluster_hex) \
    extern const ::zhc::TzConverter var{                               \
        .key         = key_str,                                        \
        .cluster     = cluster_str,                                    \
        .cluster_id  = cluster_hex,                                    \
        .command_id  = 0x02,                                           \
        .fn          = &::zhc::generic::tz_zcl_write_attr,             \
        .user_config = &spec_ref,                                      \
    };

// hvacThermostat 0x0201 ────────────────────────────────────────────
ZHC_BOSCH_TZ(kTzBoschOperatingMode,    kSpecOperatingMode,
             "operating_mode",        "hvacThermostat", 0x0201)
ZHC_BOSCH_TZ(kTzBoschValveAdaptStatus, kSpecValveAdaptStatus,
             "valve_adapt_status",    "hvacThermostat", 0x0201)
ZHC_BOSCH_TZ(kTzBoschRemoteTemperature,kSpecRemoteTemperature,
             "remote_temperature",    "hvacThermostat", 0x0201)
ZHC_BOSCH_TZ(kTzBoschWindowOpenMode,   kSpecWindowOpenMode,
             "window_open",           "hvacThermostat", 0x0201)
ZHC_BOSCH_TZ(kTzBoschBoostHeating,     kSpecBoostHeating,
             "boost_heating",         "hvacThermostat", 0x0201)
ZHC_BOSCH_TZ(kTzBoschValveType,        kSpecValveType,
             "valve_type",            "hvacThermostat", 0x0201)
ZHC_BOSCH_TZ(kTzBoschHeaterType,       kSpecHeaterType,
             "heater_type",           "hvacThermostat", 0x0201)

// hvacUserInterfaceCfg 0x0204 ──────────────────────────────────────
ZHC_BOSCH_TZ(kTzBoschChildLockUi,      kSpecChildLockUi,
             "child_lock",            "hvacUserInterfaceCfg", 0x0204)
ZHC_BOSCH_TZ(kTzBoschDisplayOrientation, kSpecDisplayOrientation,
             "display_orientation",   "hvacUserInterfaceCfg", 0x0204)
ZHC_BOSCH_TZ(kTzBoschActivityLed,      kSpecActivityLed,
             "activity_led",          "hvacUserInterfaceCfg", 0x0204)
ZHC_BOSCH_TZ(kTzBoschDisplayedTemperature, kSpecDisplayedTemperature,
             "displayed_temperature", "hvacUserInterfaceCfg", 0x0204)
ZHC_BOSCH_TZ(kTzBoschDisplaySwitchDur, kSpecDisplaySwitchDur,
             "display_ontime",        "hvacUserInterfaceCfg", 0x0204)
ZHC_BOSCH_TZ(kTzBoschDisplayBrightness, kSpecDisplayBrightness,
             "display_brightness",    "hvacUserInterfaceCfg", 0x0204)

// twinguardSmokeDetector 0xE000 ────────────────────────────────────
ZHC_BOSCH_TZ(kTzBoschTwinguardSensitivity, kSpecTwinguardSensitivity,
             "sensitivity",           "twinguardSmokeDetector", 0xE000)

#undef ZHC_BOSCH_TZ

// kTzBoschTrv populated after all kTzBosch* converters exist.
const ::zhc::TzConverter* const kTzBoschTrv[] = {
    &::zhc::generic::kTzThermostat,
    &kTzBoschOperatingMode,
    &kTzBoschBoostHeating,
    &kTzBoschWindowOpenMode,
    &kTzBoschRemoteTemperature,
    &kTzBoschChildLockUi,
    &kTzBoschDisplayOrientation,
    &kTzBoschDisplayedTemperature,
    &kTzBoschDisplaySwitchDur,
    &kTzBoschDisplayBrightness,
};
const std::uint8_t kTzBoschTrvCount =
    static_cast<std::uint8_t>(sizeof(kTzBoschTrv)/sizeof(kTzBoschTrv[0]));

}  // namespace zhc::devices::bosch
