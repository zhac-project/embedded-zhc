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
#include <cstring>
#include <span>
#include <string_view>
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

// ── Typed IAS-zone sensor bundles ───────────────────────────────────
// The generic `kFzIasZone` above emits a bare `alarm` (zoneStatus bit 0).
// z2m's Bosch security sensors expose the *semantic* key for their zone
// type instead — `contact` (door/window), `occupancy` (motion) and
// `water_leak` (water alarm). Wire the zone-type-specific converters so
// the runtime emits the same key z2m does (bit 0 → semantic key, bit 2 →
// tamper, bit 3 → battery_low). genPowerCfg battery rides alongside.
// z2m-source: bosch.ts reportContactState / tamperAndOccupancyAlarm /
// waterAndTamperAlarm.

// Door/window contact (BSEN-C2 / BSEN-CV / BSEN-C2D) — zoneStatus bit 0
// → `contact`.
const ::zhc::FzConverter* const kFzBoschContact[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};
const std::uint8_t kFzBoschContactCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschContact)/sizeof(kFzBoschContact[0]));
const ::zhc::Expose kExposesBoschContact[] = {
    { "battery",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "battery_low", ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "contact",     ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesBoschContactCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschContact)/sizeof(kExposesBoschContact[0]));

// Motion detector (BSEN-M / ISW-ZPR1-WP13 / RADION TriTech ZB) —
// zoneStatus bit 0 → `occupancy`.
const ::zhc::FzConverter* const kFzBoschMotion[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasMotionAlarm,
};
const std::uint8_t kFzBoschMotionCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschMotion)/sizeof(kFzBoschMotion[0]));
const ::zhc::Expose kExposesBoschMotion[] = {
    { "battery",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "battery_low", ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "occupancy",   ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesBoschMotionCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschMotion)/sizeof(kExposesBoschMotion[0]));

// Water alarm (BSEN-W / BSD-2, both `RBSH-WS`/`RBSH-SD` "Water alarm"
// in z2m) — zoneStatus bit 0 → `water_leak`.
const ::zhc::FzConverter* const kFzBoschWaterLeak[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};
const std::uint8_t kFzBoschWaterLeakCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschWaterLeak)/sizeof(kFzBoschWaterLeak[0]));
const ::zhc::Expose kExposesBoschWaterLeak[] = {
    { "battery",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "battery_low", ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "water_leak",  ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesBoschWaterLeakCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschWaterLeak)/sizeof(kExposesBoschWaterLeak[0]));

// Smoke alarm (BSD-2, RBSH-SD-ZB-EU "Smoke alarm II") — zoneStatus bit 0
// → `smoke`. z2m's `smokeAlarmAndButtonPushes` rides the IAS zone state;
// the button-pushes + broadcast/test commands are manuSpec extras.
const ::zhc::FzConverter* const kFzBoschSmoke[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
};
const std::uint8_t kFzBoschSmokeCount =
    static_cast<std::uint8_t>(sizeof(kFzBoschSmoke)/sizeof(kFzBoschSmoke[0]));
const ::zhc::Expose kExposesBoschSmoke[] = {
    { "battery",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ::zhc::ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "battery_low", ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "smoke",       ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "tamper",      ::zhc::ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesBoschSmokeCount =
    static_cast<std::uint8_t>(sizeof(kExposesBoschSmoke)/sizeof(kExposesBoschSmoke[0]));

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


// ── BTH-RM230Z bundle ────────────────────────────────────────────────
namespace {
// 0x4023 humidityAlarmLed (enum8, mfg-specific) → "16419". 0x07 = on, 0x06 = off.
bool fz_bosch_humidity_alarm_led(const ::zhc::DecodedMessage& msg, const ::zhc::FzConverter&,
                                 const ::zhc::PreparedDefinition&, ::zhc::RuntimeContext&,
                                 ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("16419");
    if (!v) return false;
    std::uint64_t raw;
    if      (v->type == ::zhc::ValueType::Uint) raw = v->u;
    else if (v->type == ::zhc::ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
    else return false;
    ::zhc::Value o{}; o.type = ::zhc::ValueType::Bool; o.b = (raw == 0x07);
    out.put("humidity_alarm_led", o);
    return true;
}
constexpr ::zhc::generic::ZclWriteSpec kSpecHumidityAlarmLed{
    "humidity_alarm_led", 0x4023, 0x30, kBoschMfg, nullptr, 0,
};
// Bool / ON / OFF → the two observed raw values, then the generic encoder.
bool tz_bosch_humidity_alarm_led(std::string_view key, const ::zhc::Value& input,
                                 const ::zhc::TzConverter& self, const ::zhc::PreparedDefinition& def,
                                 ::zhc::RuntimeContext& ctx, std::span<std::uint8_t> out_frame,
                                 std::size_t& out_size) {
    if (key != "humidity_alarm_led") return false;
    bool on;
    switch (input.type) {
        case ::zhc::ValueType::Bool: on = input.b; break;
        case ::zhc::ValueType::Uint: on = input.u != 0; break;
        case ::zhc::ValueType::Int:  on = input.i != 0; break;
        case ::zhc::ValueType::StringRef:
            if (!input.str) return false;
            if      (std::strcmp(input.str, "ON")  == 0) on = true;
            else if (std::strcmp(input.str, "OFF") == 0) on = false;
            else return false;
            break;
        default: return false;
    }
    ::zhc::Value v{}; v.type = ::zhc::ValueType::Uint; v.u = on ? 0x07 : 0x06;
    const ::zhc::TzConverter tmp{ "humidity_alarm_led", self.cluster, self.cluster_id, self.command_id,
                                  &::zhc::generic::tz_zcl_write_attr, &kSpecHumidityAlarmLed };
    return ::zhc::generic::tz_zcl_write_attr(key, v, tmp, def, ctx, out_frame, out_size);
}
}  // namespace

extern const ::zhc::FzConverter kFzBoschHumidityAlarmLed{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_bosch_humidity_alarm_led },
    .user_config       = nullptr,
};
extern const ::zhc::TzConverter kTzBoschHumidityAlarmLed{
    "humidity_alarm_led", "hvacThermostat", 0x0201, 0x02,
    &tz_bosch_humidity_alarm_led, nullptr };

const ::zhc::FzConverter* const kFzBoschRm230z[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzHumidity,
    &kFzBoschHumidityAlarmLed,
};
const std::uint8_t kFzBoschRm230zCount = sizeof(kFzBoschRm230z) / sizeof(kFzBoschRm230z[0]);

const ::zhc::TzConverter* const kTzBoschRm230z[] = {
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
    &kTzBoschHumidityAlarmLed,
};
const std::uint8_t kTzBoschRm230zCount = sizeof(kTzBoschRm230z) / sizeof(kTzBoschRm230z[0]);

const ::zhc::Expose kExposesBoschRm230z[] = {
    { "local_temperature",          ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "current_heating_setpoint",   ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "\xC2\xB0""C", nullptr, nullptr, 0 },
    { "system_mode",                ::zhc::ExposeType::Enum,    ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "running_state",              ::zhc::ExposeType::Enum,    ::zhc::Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "humidity",                   ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "%", nullptr, nullptr, 0 },
    { "operating_mode",             ::zhc::ExposeType::Enum,    ::zhc::Access::StateSet,
      nullptr, "schedule | manual | pause", nullptr, 0 },
    { "boost_heating",              ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "window_open",                ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
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
    { "humidity_alarm_led",         ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, "LED warning when humidity is outside 30-70 % (raw 0x07 on / 0x06 off)", nullptr, 0,
      ::zhc::ExposeCategory::Config },
};
const std::uint8_t kExposesBoschRm230zCount = sizeof(kExposesBoschRm230z) / sizeof(kExposesBoschRm230z[0]);

const ::zhc::BindingSpec kBindingsBoschRm230z[] = {
    { 1, 0x0201 },  // hvacThermostat
    { 1, 0x0204 },  // hvacUserInterfaceCfg
    { 1, 0x0405 },  // msRelativeHumidity
};
const std::uint8_t kBindingsBoschRm230zCount = sizeof(kBindingsBoschRm230z) / sizeof(kBindingsBoschRm230z[0]);

}  // namespace zhc::devices::bosch
