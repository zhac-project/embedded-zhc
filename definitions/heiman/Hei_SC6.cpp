// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman SC6 smart smoke & CO alarm (zigbeeModel SC6), new in z2m
// v26.126.0..v26.133.0. Partial port, uses shared heiman converters:
//   * m.battery, m.temperature, m.humidity, m.illuminance (10 s .. 30 min, 16990),
//     m.iasZoneAlarm smoke (smoke, battery_low), tz.warning;
//   * co: msCarbonMonoxide measuredValue with z2m scale 0.000001 (value / 1e-6,
//     i.e. the float mole fraction * 1e6 = ppm) -> kFzCO;
//   * heimanClusterSpecial (0xFC90, mfg 0x120B): fault_state / muted bitmap text,
//     temporary_mute, heartbeat_indicator, interconnectable + link_available (same
//     attribute 0x1007), smoke_level (/100), smoke_unit, chamber_contamination,
//     siren_for_automation_only, temperature_offset (/100), illuminance_threshold,
//     sensitivity_level, reported/rejoin/reboot counters, trigger_selftest;
//   * unoccupied_delay: msOccupancySensing pirOToUDelay 0x0010 (u16, s).
// z2m runs fzLocal.heimanClusterSpecialfz FIRST and the extends after, so the
// extends win where both decode an attribute (smoke_level is the /100 number, not
// the toFixed string; link_available uses the extend lookup with gas_active = 3).
// Not ported: identify, the IAS `test` flag (zoneStatus bit 8, no generic decoder),
// the radar / learning / sensor-life rows (target_distance, radar_detection_max_range,
// work_mode, radar_learning, radar_learning_reset, radar_learning_state, preheating,
// endoflife, alarm_state), battery-percentage reporting, the warning composite.
// z2m-source: heiman.ts #SC6
#include "definitions/_generic/_shared.hpp"
#include "definitions/heiman/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
using ::zhc::generic::ZclAttrRow;
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;
using ::zhc::generic::kZclAttrFlagBool;
using ::zhc::heiman::HeimanBitmapEntry;
using ::zhc::heiman::HeimanSpecificEntry;
using ::zhc::heiman::HeimanSpecificType;
using ::zhc::heiman::kHeimanMfgCode;

// heimanClusterSensorFaultState / heimanClusterDeviceMuteState bit tables.
constexpr HeimanBitmapEntry kFaultBits[] = {
    {0, "fault"}, {1, "open_circuit_fault"}, {2, "short_circuit_fault"}, {3, "pollution_fault"},
};
constexpr HeimanBitmapEntry kMuteBits[] = {
    {0, "muted"}, {1, "alarm_muted"}, {2, "fault_muted"}, {3, "low_battery_muted"},
    {4, "endoflife_muted"}, {5, "warning_muted"},
};
constexpr HeimanSpecificEntry kStateEntries[] = {
    {0x0002, "fault_state", HeimanSpecificType::BitmapText, kFaultBits, sizeof(kFaultBits)/sizeof(kFaultBits[0])},
    {0x0009, "muted",       HeimanSpecificType::BitmapText, kMuteBits,  sizeof(kMuteBits)/sizeof(kMuteBits[0])},
};
constexpr ::zhc::heiman::HeimanSpecificMap kStateMap{ kStateEntries, sizeof(kStateEntries)/sizeof(kStateEntries[0]) };

// The shared bitmap decoder has no manufacturer filter; 0xFC90 is unnamed
// without the Heiman code, so gate it here.
bool fz_sc6_states(const DecodedMessage& msg, const FzConverter& self, const PreparedDefinition& def,
                   RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!msg.manufacturer_specific || msg.manufacturer_code != kHeimanMfgCode) return false;
    return ::zhc::heiman::fz_heiman_manu_specific(msg, self, def, ctx, out);
}
constexpr FzConverter kFzStates{
    .family            = FrameFamily::Zcl,
    .cluster           = "heimanClusterSpecial",
    .type_mask         = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_sc6_states },
    .user_config       = &kStateMap,
};

constexpr ZclWriteLookup kLinkLut[] = {
    {"inactive", 0}, {"smoke_active", 1}, {"co_active", 2}, {"gas_active", 3}, {"heat_active", 4},
};
constexpr ZclWriteLookup kSmokeUnitLut[] = { {"dB/m", 0}, {"%ft OBS", 1} };
constexpr ZclWriteLookup kContaminationLut[] = {
    {"normal", 0}, {"light_contamination", 1}, {"medium_contamination", 2}, {"critical_contamination", 3},
};
constexpr ZclWriteLookup kSirenLut[] = { {"stop", 0}, {"smoke_siren", 1}, {"co_siren", 2} };
constexpr ZclWriteLookup kSensitivityLut[] = { {"low", 0}, {"medium", 1}, {"high", 2} };

constexpr ZclAttrRow kSpecialRows[] = {
    { 0x0008, "temporary_mute", 1, nullptr, 0, kZclAttrFlagBool },
    { 0x1004, "heartbeat_indicator", 1, nullptr, 0, kZclAttrFlagBool },
    { 0x1007, "interconnectable", 1, nullptr, 0, kZclAttrFlagBool },
    { 0x1007, "link_available", 1, kLinkLut, sizeof(kLinkLut)/sizeof(kLinkLut[0]) },
    { 0x0016, "smoke_level", 100 },
    { 0x0018, "smoke_unit", 1, kSmokeUnitLut, sizeof(kSmokeUnitLut)/sizeof(kSmokeUnitLut[0]) },
    { 0x0017, "chamber_contamination", 1, kContaminationLut, sizeof(kContaminationLut)/sizeof(kContaminationLut[0]) },
    { 0x0012, "siren_for_automation_only", 1, kSirenLut, sizeof(kSirenLut)/sizeof(kSirenLut[0]) },
    { 0x100D, "temperature_offset", 100 },
    { 0x002A, "illuminance_threshold" },
    { 0x0004, "sensitivity_level", 1, kSensitivityLut, sizeof(kSensitivityLut)/sizeof(kSensitivityLut[0]) },
    { 0x001B, "reported_packages" },
    { 0x001A, "rejoin_count" },
    { 0x0019, "reboot_count" },
};
constexpr ::zhc::generic::ZclAttrMap kSpecialMap{ kSpecialRows, sizeof(kSpecialRows)/sizeof(kSpecialRows[0]), kHeimanMfgCode };
constexpr FzConverter kFzSpecial = ::zhc::generic::zcl_attr_fz("heimanClusterSpecial", &kSpecialMap);

constexpr ZclAttrRow kOccRows[] = { { 0x0010, "unoccupied_delay" } };
constexpr ::zhc::generic::ZclAttrMap kOccMap{ kOccRows, sizeof(kOccRows)/sizeof(kOccRows[0]) };
constexpr FzConverter kFzOcc = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kOccMap);

constexpr ZclWriteSpec kSirenSpec{ "siren_for_automation_only", 0x0012, 0x30, kHeimanMfgCode, kSirenLut,
                                   sizeof(kSirenLut)/sizeof(kSirenLut[0]) };
constexpr ZclWriteSpec kTempOffsetSpec{ "temperature_offset", 0x100D, 0x29, kHeimanMfgCode, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kIllumThresholdSpec{ "illuminance_threshold", 0x002A, 0x21, kHeimanMfgCode, nullptr, 0 };
constexpr ZclWriteSpec kSensitivitySpec{ "sensitivity_level", 0x0004, 0x20, kHeimanMfgCode, kSensitivityLut,
                                         sizeof(kSensitivityLut)/sizeof(kSensitivityLut[0]) };
constexpr ZclWriteSpec kUnoccupiedDelaySpec{ "unoccupied_delay", 0x0010, 0x21, 0, nullptr, 0 };
constexpr TzConverter kTzSiren = ::zhc::generic::zcl_write_tz("heimanClusterSpecial", 0xFC90, &kSirenSpec);
constexpr TzConverter kTzTempOffset = ::zhc::generic::zcl_write_tz("heimanClusterSpecial", 0xFC90, &kTempOffsetSpec);
constexpr TzConverter kTzIllumThreshold = ::zhc::generic::zcl_write_tz("heimanClusterSpecial", 0xFC90, &kIllumThresholdSpec);
constexpr TzConverter kTzSensitivity = ::zhc::generic::zcl_write_tz("heimanClusterSpecial", 0xFC90, &kSensitivitySpec);
constexpr TzConverter kTzUnoccupiedDelay = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kUnoccupiedDelaySpec);

const FzConverter* const kFz_SC6[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzIasSmokeAlarm,
    &::zhc::generic::kFzCO,
    &kFzStates,
    &kFzSpecial,
    &kFzOcc,
};
const TzConverter* const kTz_SC6[] = {
    &::zhc::generic::kTzWarning,
    &::zhc::heiman::kTzHeimanInitiateTestMode,
    &::zhc::heiman::kTzHeimanTemporaryMute,
    &::zhc::heiman::kTzHeimanIndicatorLight,
    &kTzSiren,
    &kTzTempOffset,
    &kTzUnoccupiedDelay,
    &kTzIllumThreshold,
    &kTzSensitivity,
};
constexpr const char* kModels_SC6[] = { "SC6" };
constexpr const char* kSelftestOpts[] = { "test" };
constexpr const char* kSmokeUnitOpts[] = { "dB/m", "%ft OBS" };
constexpr const char* kContaminationOpts[] = { "normal", "light_contamination", "medium_contamination", "critical_contamination" };
constexpr const char* kLinkOpts[] = { "inactive", "smoke_active", "co_active", "gas_active", "heat_active" };
constexpr const char* kSirenOpts[] = { "stop", "smoke_siren", "co_siren" };
constexpr const char* kSensitivityOpts[] = { "low", "medium", "high" };
constexpr Expose kExposes_SC6[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"co", ExposeType::Numeric, Access::State, "ppm", "The measured CO level", nullptr, 0,
     ExposeCategory::State, 0, 900, 0},
    {"fault_state", ExposeType::String, Access::State, nullptr, "Device fault status (normal or fault types).", nullptr, 0},
    {"muted", ExposeType::String, Access::State, nullptr, "Device mute status (normal or mute types).", nullptr, 0},
    {"trigger_selftest", ExposeType::Enum, Access::Set, nullptr, "Trigger alarm self-check.",
     kSelftestOpts, sizeof(kSelftestOpts)/sizeof(kSelftestOpts[0])},
    {"temporary_mute", ExposeType::Binary, Access::StateSet, nullptr, "Silence the alarm temporarily", nullptr, 0},
    {"heartbeat_indicator", ExposeType::Binary, Access::StateSet, nullptr, "Enable/disable the indicator on product", nullptr, 0},
    {"interconnectable", ExposeType::Binary, Access::State, nullptr, "used for interconnection automation.", nullptr, 0},
    {"smoke_level", ExposeType::Numeric, Access::State, nullptr, "smoke level", nullptr, 0,
     ExposeCategory::State, 0, 20, 0},
    {"smoke_unit", ExposeType::Enum, Access::State, nullptr, "smoke level unit",
     kSmokeUnitOpts, sizeof(kSmokeUnitOpts)/sizeof(kSmokeUnitOpts[0])},
    {"chamber_contamination", ExposeType::Enum, Access::State, nullptr,
     "it indicates that how serious the smoke chamber get contaminated.",
     kContaminationOpts, sizeof(kContaminationOpts)/sizeof(kContaminationOpts[0])},
    {"link_available", ExposeType::Enum, Access::State, nullptr, "used for interconnection automation.",
     kLinkOpts, sizeof(kLinkOpts)/sizeof(kLinkOpts[0])},
    {"siren_for_automation_only", ExposeType::Enum, Access::StateSet, nullptr, "siren effect",
     kSirenOpts, sizeof(kSirenOpts)/sizeof(kSirenOpts[0])},
    {"temperature_offset", ExposeType::Numeric, Access::StateSet, nullptr, "used for temperature offset, unit: \xE2\x84\x83",
     nullptr, 0, ExposeCategory::State, -15, 15, 0},
    {"unoccupied_delay", ExposeType::Numeric, Access::StateSet, "s", "occupied to unoccupied delay", nullptr, 0,
     ExposeCategory::State, 5, 3600, 0},
    {"illuminance_threshold", ExposeType::Numeric, Access::StateSet, "lx",
     "when the illuminance exceeds the threshold, it activates local linkages.", nullptr, 0,
     ExposeCategory::State, 0, 1000, 0},
    {"sensitivity_level", ExposeType::Enum, Access::StateSet, nullptr, "The sensitivity of Sensor",
     kSensitivityOpts, sizeof(kSensitivityOpts)/sizeof(kSensitivityOpts[0])},
    {"reported_packages", ExposeType::Numeric, Access::State, nullptr,
     "For diagnostic purpose, how many zigbee packages the product reported in a day", nullptr, 0,
     ExposeCategory::Diagnostic, 0, 60000, 0},
    {"rejoin_count", ExposeType::Numeric, Access::State, nullptr,
     "For diagnostic purpose, how many times the product rejoined the zigbee network", nullptr, 0,
     ExposeCategory::Diagnostic, 0, 60000, 0},
    {"reboot_count", ExposeType::Numeric, Access::State, nullptr,
     "For diagnostic purpose, how many times the product rebooted", nullptr, 0,
     ExposeCategory::Diagnostic, 0, 60000, 0},
};
constexpr BindingSpec kBindings_SC6[] = {
    {1, 0x0001},
    {1, 0xFC90},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0400},
    {1, 0x040C},
};
constexpr ReportingSpec kReports_SC6[] = {
    {1, 0x0402, 0x0000, 0x29, 10, 3600, 100, 0},
    {1, 0x0405, 0x0000, 0x21, 10, 3600, 100, 0},
    {1, 0x0400, 0x0000, 0x21, 10, 1800, 16990, 0},
    {1, 0x040C, 0x0000, 0x39, 1, 14400, 0x3727C5ACu /* 0.00001f */, 0},
};
}  // namespace

extern const PreparedDefinition kDef_SC6{
    .zigbee_models=kModels_SC6, .zigbee_models_count=sizeof(kModels_SC6)/sizeof(kModels_SC6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SC6", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_SC6, .exposes_count=sizeof(kExposes_SC6)/sizeof(kExposes_SC6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SC6, .from_zigbee_count=sizeof(kFz_SC6)/sizeof(kFz_SC6[0]),
    .to_zigbee=kTz_SC6, .to_zigbee_count=sizeof(kTz_SC6)/sizeof(kTz_SC6[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SC6, .bindings_count=sizeof(kBindings_SC6)/sizeof(kBindings_SC6[0]),
    .reports=kReports_SC6, .reports_count=sizeof(kReports_SC6)/sizeof(kReports_SC6[0]),
};

}  // namespace zhc::devices::heiman
