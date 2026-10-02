// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara ZNMHLDJ01LM smart vertical blinds motor H1
// (zigbeeModel "lumi.curtain.acn011"; z2m v26.105.0, #13093).
//
// Plain closuresWindowCovering with lift + tilt. Upstream marks the device
// `coverInverted` (it reports 100 = open natively); the generic lift/tilt
// decoders pass the reported percentage straight through, which is exactly
// the value z2m publishes for this device. identify() is upstream surface
// with no ZHC counterpart.
//
// z2m v26.107.0 (#13116) adds the curtain attributes on manuSpecificLumi (0xFCC0,
// manufacturer code 0x115F): manual_open_close (0x0401, inverted: 0 = ON),
// status (0x0421), last_manual_operation (0x0425), traverse_time (0x0403),
// calibration_status (0x0426), calibrated (0x0402) and identify_beep (0x0404,
// off / short / long).
// z2m-source: lumi.ts #ZNMHLDJ01LM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {
constexpr std::uint16_t kLumiManu = 0x115F;
constexpr ::zhc::generic::ZclWriteLookup kStatus[] = { {"closing", 0}, {"opening", 1}, {"stopped", 2}, {"blocked", 3} };
constexpr ::zhc::generic::ZclWriteLookup kLastManual[] = { {"open", 1}, {"close", 2}, {"stop", 3} };
constexpr ::zhc::generic::ZclWriteLookup kCalibration[] = { {"not_calibrated", 0}, {"half_calibrated", 1}, {"fully_calibrated", 2} };
constexpr ::zhc::generic::ZclWriteLookup kBeep[] = { {"off", 0}, {"short", 1}, {"long", 2} };
constexpr ::zhc::generic::ZclWriteLookup kManualWords[] = { {"ON", 0}, {"OFF", 1} };
constexpr ::zhc::generic::ZclAttrRow kCurtainRows[] = {
    { 0x0401, "manual_open_close", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool | ::zhc::generic::kZclAttrFlagInvert },
    { 0x0421, "status", 1, kStatus, 4 },
    { 0x0425, "last_manual_operation", 1, kLastManual, 3 },
    { 0x0403, "traverse_time" },
    { 0x0426, "calibration_status", 1, kCalibration, 3 },
    { 0x0402, "calibrated", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0404, "identify_beep", 1, kBeep, 3 },
};
constexpr ::zhc::generic::ZclAttrMap kCurtainMap{ kCurtainRows, sizeof(kCurtainRows)/sizeof(kCurtainRows[0]), kLumiManu };
constexpr FzConverter kFzCurtain = ::zhc::generic::zcl_attr_fz("manuSpecificLumi", &kCurtainMap);
constexpr ::zhc::generic::ZclWriteSpec kManualSpec{ "manual_open_close", 0x0401, 0x10, kLumiManu, kManualWords, 2,
                                                    ::zhc::generic::kZclWriteFlagInvertBool };
constexpr ::zhc::generic::ZclWriteSpec kBeepSpec{ "identify_beep", 0x0404, 0x20, kLumiManu, kBeep, 3 };
constexpr TzConverter kTzManual = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kManualSpec);
constexpr TzConverter kTzBeep = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kBeepSpec);

const FzConverter* const kFz[] = {
    &::zhc::lumi::kFzLumiBasic,
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzCoverTilt,
    &kFzCurtain,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPositionLift,
    &::zhc::generic::kTzCoverPositionTilt,
    &kTzManual,
    &kTzBeep,
};
constexpr const char* kStatusOpts[] = { "closing", "opening", "stopped", "blocked" };
constexpr const char* kLastManualOpts[] = { "open", "close", "stop" };
constexpr const char* kCalibrationOpts[] = { "not_calibrated", "half_calibrated", "fully_calibrated" };
constexpr const char* kBeepOpts[] = { "off", "short", "long" };
constexpr const char* kModels[] = { "lumi.curtain.acn011" };
constexpr const char* kStateOpts[] = { "OPEN", "CLOSE", "STOP" };
constexpr Expose kExposes[] = {
    {"state",    ExposeType::Enum,    Access::Set,      nullptr, "Open, close or stop the blinds", kStateOpts, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", "Lift position", nullptr, 0},
    {"tilt",     ExposeType::Numeric, Access::StateSet, "%", "Tilt position", nullptr, 0},
    {"manual_open_close", ExposeType::Binary, Access::StateSet, nullptr, "Open or close by hand", nullptr, 0, ExposeCategory::Config},
    {"status", ExposeType::Enum, Access::State, nullptr, nullptr, kStatusOpts, 4},
    {"last_manual_operation", ExposeType::Enum, Access::State, nullptr, nullptr, kLastManualOpts, 3},
    {"traverse_time", ExposeType::Numeric, Access::State, "s", nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"calibration_status", ExposeType::Enum, Access::State, nullptr, nullptr, kCalibrationOpts, 3, ExposeCategory::Diagnostic},
    {"calibrated", ExposeType::Binary, Access::State, nullptr, "Indicates if this device is calibrated", nullptr, 0, ExposeCategory::Diagnostic},
    {"identify_beep", ExposeType::Enum, Access::StateSet, nullptr, "Device will beep for chosen time duration", kBeepOpts, 3, ExposeCategory::Config},
};
constexpr BindingSpec kBindings[] = {
    {1, 0x0102},
};
}  // namespace

extern const PreparedDefinition kDefZNMHLDJ01LM{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="ZNMHLDJ01LM",.vendor="Aqara",
    .meta=nullptr,.exposes=kExposes,.exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};
}  // namespace zhc::devices::lumi
