// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HOBEIAN ZG-204Z* mmWave motion sensors running the pvvx/ZigbeeTLc
// open firmware (z2m v26.125.0 window). Five definitions:
//   ZG-204ZV-z / ZG-204ZV2-TH-z  zg204zvMotion + temperature + humidity (WHT20)
//   ZG-204ZV1-z / ZG-204ZV2-z    zg204zvMotion
//   ZG-204ZS-z (+ ZG-204ZM-z)    zg204zsMotion: dual PIR + 24 GHz radar
// Before these exact defs the model IDs reached the stock HOBEIAN ZG-204Z def
// (tuya/ZG_204Z.cpp, IAS) through the substring pass and decoded nothing; the
// stock "ZG-204Z" still matches that def exactly.
//
// Shared surface (as TS202PIR1-z): battery + voltage, illuminance, the genOnOff
// remote-binding enable with its startUpOnOff power_on_behavior, on/off
// commands sent to bound devices, and msOccupancySensing pirOToUDelay (0x0010)
// as fading_time. The V models add pirUToOThreshold (0x0011) as
// motion_detection_sensitivity; the S model decodes the occupancy bitmap as
// bit0 occupancy / bit1 PIR motion / bit2 radar presence + presence_state, and
// has zone thresholds 0x5101-0x5106 and radar_trigger_scans 0x5107 (u8, plain
// attributes on msOccupancySensing).
// z2m-source: zigbeetlc.ts #ZG-204ZV-z #ZG-204ZV1-z #ZG-204ZV2-TH-z #ZG-204ZV2-z #ZG-204ZS-z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::zigbeetlc {
namespace {

constexpr std::uint16_t kOccupancySensing = 0x0406;

// extend.ternaryOccupancy (zigbeetlc.ts).
bool fz_ternary_occupancy(const DecodedMessage& msg, const FzConverter&,
                          const PreparedDefinition&, RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 occupancy bitmap
    if (!v || v->type != ValueType::Uint) return false;
    const bool occupied = (v->u & 0x01u) != 0;
    const bool pir      = (v->u & 0x02u) != 0;
    const bool radar    = (v->u & 0x04u) != 0;
    const char* state = pir && radar ? "motion_and_presence"
                      : pir          ? "motion"
                      : radar        ? "presence"
                                     : "clear";
    Value b{}; b.type = ValueType::Bool;
    b.b = occupied; out.put("occupancy", b);
    b.b = pir;      out.put("motion", b);
    b.b = radar;    out.put("presence", b);
    Value s{}; s.type = ValueType::StringRef; s.str = state;
    out.put("presence_state", s);
    return true;
}

constexpr FzConverter kFzTernaryOccupancy{
    .family            = FrameFamily::Zcl,
    .cluster           = "msOccupancySensing",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ternary_occupancy },
    .user_config       = nullptr,
};

// msOccupancySensing numerics.
constexpr ::zhc::generic::ZclAttrRow kRowsV[] = {
    {0x0010, "fading_time"},
    {0x0011, "motion_detection_sensitivity"},
};
constexpr ::zhc::generic::ZclAttrRow kRowsS[] = {
    {0x0010, "fading_time"},
    {0x5101, "zone1_motion_threshold"},
    {0x5102, "zone1_still_threshold"},
    {0x5103, "zone2_motion_threshold"},
    {0x5104, "zone2_still_threshold"},
    {0x5105, "zone3_motion_threshold"},
    {0x5106, "zone3_still_threshold"},
    {0x5107, "radar_trigger_scans"},
};
constexpr ::zhc::generic::ZclAttrMap kMapV{ kRowsV, sizeof(kRowsV)/sizeof(kRowsV[0]) };
constexpr ::zhc::generic::ZclAttrMap kMapS{ kRowsS, sizeof(kRowsS)/sizeof(kRowsS[0]) };
constexpr FzConverter kFzAttrsV = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kMapV);
constexpr FzConverter kFzAttrsS = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kMapS);

constexpr ::zhc::generic::ZclWriteSpec kSpecFadingTime{ "fading_time", 0x0010, 0x21, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecSensitivity{ "motion_detection_sensitivity", 0x0011, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecZone1Motion{ "zone1_motion_threshold", 0x5101, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecZone1Still{ "zone1_still_threshold", 0x5102, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecZone2Motion{ "zone2_motion_threshold", 0x5103, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecZone2Still{ "zone2_still_threshold", 0x5104, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecZone3Motion{ "zone3_motion_threshold", 0x5105, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecZone3Still{ "zone3_still_threshold", 0x5106, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecTriggerScans{ "radar_trigger_scans", 0x5107, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzFadingTime = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecFadingTime);
constexpr TzConverter kTzSensitivity = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecSensitivity);
constexpr TzConverter kTzZone1Motion = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecZone1Motion);
constexpr TzConverter kTzZone1Still = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecZone1Still);
constexpr TzConverter kTzZone2Motion = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecZone2Motion);
constexpr TzConverter kTzZone2Still = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecZone2Still);
constexpr TzConverter kTzZone3Motion = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecZone3Motion);
constexpr TzConverter kTzZone3Still = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecZone3Still);
constexpr TzConverter kTzTriggerScans = ::zhc::generic::zcl_write_tz("msOccupancySensing", kOccupancySensing, &kSpecTriggerScans);

const FzConverter* const kFzV[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &kFzAttrsV,
};
const FzConverter* const kFzVTH[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &kFzAttrsV,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
const FzConverter* const kFzS[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIlluminance,
    &kFzTernaryOccupancy,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &kFzAttrsS,
};
const TzConverter* const kTzV[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
    &kTzFadingTime,
    &kTzSensitivity,
};
const TzConverter* const kTzS[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
    &kTzFadingTime,
    &kTzZone1Motion,
    &kTzZone1Still,
    &kTzZone2Motion,
    &kTzZone2Still,
    &kTzZone3Motion,
    &kTzZone3Still,
    &kTzTriggerScans,
};

constexpr const char* kPobOpts[] = { "off", "on", "toggle", "previous" };
constexpr const char* kPresenceStates[] = { "clear", "motion", "presence", "motion_and_presence" };

#define ZG204_COMMON_EXPOSES \
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic}, \
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0, ExposeCategory::Diagnostic}, \
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0}, \
    {"state", ExposeType::Binary, Access::StateSet, nullptr, \
     "Enable remote on/off binding: occupied sends on, clear sends off", nullptr, 0, ExposeCategory::Config}, \
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, \
     "Power-on behavior for state (remote on/off enable)", kPobOpts, 4, ExposeCategory::Config}, \
    {"action", ExposeType::String, Access::State, nullptr, \
     "on / off issued to bound devices on occupancy change", nullptr, 0}, \
    {"fading_time", ExposeType::Numeric, Access::StateSet, "s", \
     "Occupancy hold time (firmware default 35 s)", nullptr, 0, ExposeCategory::Config, 0, 524, 1}

#define ZG204_THRESHOLD(key, desc) \
    {key, ExposeType::Numeric, Access::StateSet, nullptr, desc, nullptr, 0, ExposeCategory::Config, 0, 99, 1}

constexpr Expose kExpV[] = {
    ZG204_COMMON_EXPOSES,
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"motion_detection_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr,
     "Lower is more sensitive (firmware default 125)", nullptr, 0, ExposeCategory::Config, 1, 254, 1},
};
constexpr Expose kExpVTH[] = {
    ZG204_COMMON_EXPOSES,
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"motion_detection_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr,
     "Lower is more sensitive (firmware default 125)", nullptr, 0, ExposeCategory::Config, 1, 254, 1},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};
constexpr Expose kExpS[] = {
    ZG204_COMMON_EXPOSES,
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"motion", ExposeType::Binary, Access::State, nullptr, "PIR motion detected", nullptr, 0},
    {"presence", ExposeType::Binary, Access::State, nullptr, "mmWave radar presence detected", nullptr, 0},
    {"presence_state", ExposeType::Enum, Access::State, nullptr, "Current presence state",
     kPresenceStates, 4},
    ZG204_THRESHOLD("zone1_motion_threshold", "Zone 1 (0-2 m) motion threshold, 99 = disabled"),
    ZG204_THRESHOLD("zone1_still_threshold", "Zone 1 (0-2 m) still threshold, 99 = disabled"),
    ZG204_THRESHOLD("zone2_motion_threshold", "Zone 2 (2-5 m) motion threshold, 99 = disabled"),
    ZG204_THRESHOLD("zone2_still_threshold", "Zone 2 (2-5 m) still threshold, 99 = disabled"),
    ZG204_THRESHOLD("zone3_motion_threshold", "Zone 3 (5-10 m) motion threshold, 99 = disabled"),
    ZG204_THRESHOLD("zone3_still_threshold", "Zone 3 (5-10 m) still threshold, 99 = disabled"),
    {"radar_trigger_scans", ExposeType::Numeric, Access::StateSet, nullptr,
     "Consecutive radar scans to trigger presence (1 = instant)", nullptr, 0, ExposeCategory::Config, 1, 10, 1},
};

#undef ZG204_THRESHOLD
#undef ZG204_COMMON_EXPOSES

constexpr BindingSpec kBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0006},   // genOnOff
    {1, 0x0400},   // msIlluminanceMeasurement
    {1, 0x0406},   // msOccupancySensing
};
constexpr BindingSpec kBindingsTH[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0400},
    {1, 0x0406},
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
};

constexpr const char* kModels_ZV[]    = { "ZG-204ZV-z" };
constexpr const char* kModels_ZV1[]   = { "ZG-204ZV1-z" };
constexpr const char* kModels_ZV2TH[] = { "ZG-204ZV2-TH-z" };
constexpr const char* kModels_ZV2[]   = { "ZG-204ZV2-z" };
constexpr const char* kModels_ZS[]    = { "ZG-204ZS-z", "ZG-204ZM-z" };

}  // namespace

#define ZG204_DEF(sym, models, name, fz, tz, exp, binds) \
extern const PreparedDefinition sym{ \
    .zigbee_models=models, .zigbee_models_count=sizeof(models)/sizeof(models[0]), \
    .manufacturer_name_prefix=nullptr, \
    .manufacturer_names=nullptr, .manufacturer_names_count=0, \
    .model=name, .vendor="HOBEIAN", \
    .meta=nullptr, .exposes=exp, .exposes_count=sizeof(exp)/sizeof(exp[0]), \
    .white_labels=nullptr, .white_labels_count=0, \
    .from_zigbee=fz, .from_zigbee_count=sizeof(fz)/sizeof(fz[0]), \
    .to_zigbee=tz, .to_zigbee_count=sizeof(tz)/sizeof(tz[0]), \
    .configure=nullptr, .on_event=nullptr, \
    .bindings=binds, .bindings_count=sizeof(binds)/sizeof(binds[0]), \
}

ZG204_DEF(kDef_ZG_204ZV_z,    kModels_ZV,    "ZG-204ZV-z",     kFzVTH, kTzV, kExpVTH, kBindingsTH);
ZG204_DEF(kDef_ZG_204ZV1_z,   kModels_ZV1,   "ZG-204ZV1-z",    kFzV,   kTzV, kExpV,   kBindings);
ZG204_DEF(kDef_ZG_204ZV2_TH_z, kModels_ZV2TH, "ZG-204ZV2-TH-z", kFzVTH, kTzV, kExpVTH, kBindingsTH);
ZG204_DEF(kDef_ZG_204ZV2_z,   kModels_ZV2,   "ZG-204ZV2-z",    kFzV,   kTzV, kExpV,   kBindings);
ZG204_DEF(kDef_ZG_204ZS_z,    kModels_ZS,    "ZG-204ZS-z",     kFzS,   kTzS, kExpS,   kBindings);

#undef ZG204_DEF

}  // namespace zhc::devices::zigbeetlc
