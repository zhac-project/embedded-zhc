// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara ZNCZ15LM smart plug (EU variant, lumi.plug.mmeu01).
//
// Full decode + write path:
//   fz: lumi_basic + lumi_power + lumi_meter + genOnOff
//   tz: generic on/off + manu-specific writes for:
//        power_outage_memory   (attr 0x0201 bool)
//        led_disabled_night    (attr 0x0203 bool)
//
// z2m-source: lumi.ts #ZNCZ15LM (lumi.plug.mmeu01) +
//             lib/lumi.ts tz.lumi_switch_power_outage_memory /
//             tz.lumi_led_disabled_night.

#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {

namespace {

// ── fz plumbing ───────────────────────────────────────────────────
constexpr ::zhc::lumi::LumiPowerCalibration kPowerCal{
    .voltage_div = 1,
    .current_div = 1000,
    .power_div   = 10,
};
constexpr ::zhc::lumi::LumiMeterCalibration kMeterCal{
    .energy_div = 1000,
};

constexpr FzConverter kFzPowerBound{
    .family            = FrameFamily::Zcl,
    .cluster           = "haElectricalMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_power },
    .user_config       = &kPowerCal,
};
constexpr FzConverter kFzMeterBound{
    .family            = FrameFamily::Zcl,
    .cluster           = "seMetering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_electricity_meter },
    .user_config       = &kMeterCal,
};

// ── arrays ───────────────────────────────────────────────────────
const FzConverter* const kFz[] = {
    &::zhc::lumi::kFzLumiBasic,
    &::zhc::generic::kFzOnOff,
    &kFzPowerBound,
    &kFzMeterBound,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
};

constexpr const char* kZigbeeModels[] = { "lumi.plug.macn01" };

// Bindings + reporting setup run at device join via run_configure.
// Matches z2m's `configure(device, coordinatorEndpoint)` for this
// SKU — bind genOnOff, seMetering, haElectricalMeasurement, and
// manuSpecificLumi (for the power_outage / led_night attrs).
constexpr BindingSpec kBindings[] = {
    {1, 0x0006},    // genOnOff
    {1, 0x0702},    // seMetering
    {1, 0x0B04},    // haElectricalMeasurement
    {1, 0xFCC0},    // manuSpecificLumi
};
constexpr ReportingSpec kReports[] = {
    // genOnOff.onOff         bool   1s..3600s, reportable change = any
    {1, 0x0006, 0x0000, 0x10,    1, 3600, 0, 0},
    // seMetering.curSummDlv  u48    30s..3600s, rc=1 (raw units)
    {1, 0x0702, 0x0000, 0x25,   30, 3600, 1, 0},
    // haElectricalMeasurement.activePower  s16   5s..3600s, rc=1 deci-watt
    {1, 0x0B04, 0x050B, 0x29,    5, 3600, 1, 0},
    // haElectricalMeasurement.rmsVoltage   u16  30s..3600s, rc=5
    {1, 0x0B04, 0x0505, 0x21,   30, 3600, 5, 0},
    // haElectricalMeasurement.rmsCurrent   u16  10s..3600s, rc=5 mA
    {1, 0x0B04, 0x0508, 0x21,   10, 3600, 5, 0},
};

// Exposes per z2m #ZNCZ15LM. Read-only metrics (power/energy/etc.) +
// controllable state + power_outage_memory + led_disabled_night.
constexpr Expose kExposes[] = {
    {"state",               ExposeType::Binary,  Access::StateSet,
     nullptr, "Relay state",                       nullptr, 0},
    {"power",               ExposeType::Numeric, Access::State,
     "W",     "Instantaneous active power",        nullptr, 0},
    {"energy",              ExposeType::Numeric, Access::State,
     "kWh",   "Cumulative energy",                 nullptr, 0},
    {"current",             ExposeType::Numeric, Access::State,
     "A",     "RMS current",                       nullptr, 0},
    {"voltage",             ExposeType::Numeric, Access::State,
     "V",     "RMS voltage",                       nullptr, 0},
    {"power_outage_memory", ExposeType::Binary,  Access::StateSet,
     nullptr, "Retain relay state across mains loss", nullptr, 0},
    {"led_disabled_night",  ExposeType::Binary,  Access::StateSet,
     nullptr, "Disable LED indicator at night",    nullptr, 0},
};

}  // namespace

extern const PreparedDefinition kDefZNCZ15LM{
    .zigbee_models=kZigbeeModels,.zigbee_models_count=sizeof(kZigbeeModels)/sizeof(kZigbeeModels[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model               = "ZNCZ15LM",
    .vendor              = "Xiaomi",
    .meta                = nullptr,
    .exposes             = kExposes,
    .exposes_count       = sizeof(kExposes) / sizeof(kExposes[0]),
    .white_labels        = nullptr,
    .white_labels_count  = 0,
    .from_zigbee         = kFz,
    .from_zigbee_count   = sizeof(kFz) / sizeof(kFz[0]),
    .to_zigbee           = kTz,
    .to_zigbee_count     = sizeof(kTz) / sizeof(kTz[0]),
    .configure           = nullptr,
    .on_event            = nullptr,
    .bindings            = kBindings,
    .bindings_count      = sizeof(kBindings) / sizeof(kBindings[0]),
    .reports             = kReports,
    .reports_count       = sizeof(kReports) / sizeof(kReports[0]),
};

}  // namespace zhc::devices::lumi
