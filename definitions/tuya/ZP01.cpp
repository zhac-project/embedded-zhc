// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya ZP01 PIR motion sensor (z2m v26.133.0, #13840).
// TS0202 `_TZ3000_lf56vpxj` / `_TZ3040_o4mkahkc`; before this def they fell
// to the model-only tuya/TS0202.cpp, which has no sensitivity / keep_time.
//
// z2m fromZigbee [fz.ias_occupancy_alarm_1, fz.battery,
// fz.ias_occupancy_alarm_1_report, fzLocal.ZM35HQ_attr], toZigbee
// [tzLocal.ZM35HQ_attr] — the same pair as HOBEIAN ZG-204Z's zg204_attr, so
// the writes mirror tuya/ZG_204Z.cpp:
//   ssIasZone cmd 0x00 zoneStatus          → occupancy, tamper, battery_low
//   ssIasZone attr 0x0002 zoneStatus report → the same three (local fz)
//   ssIasZone attr 0x0013 (UINT8)          ↔ sensitivity low / medium / high
//   ssIasZone attr 0xF001 (UINT8)          ↔ keep_time 30 / 60 / 120 s
//   genPowerCfg                            → battery %, voltage
// Configure = bind genPowerCfg + battery % / voltage reporting (z2m wraps both
// in try/catch, hence optional).
//
// z2m-source: tuya.ts #ZP01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

// z2m fz.ias_occupancy_alarm_1_report: zoneStatus attribute (0x0002).
bool fz_zone_status_report(const ::zhc::DecodedMessage& msg, const FzConverter&,
                           const PreparedDefinition&, ::zhc::RuntimeContext&,
                           ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("2");
    if (!v || v->type != ::zhc::ValueType::Uint) return false;
    ::zhc::Value o{}; o.type = ::zhc::ValueType::Bool;
    o.b = (v->u & 0x0001) != 0; out.put("occupancy", o);
    o.b = (v->u & 0x0004) != 0; out.put("tamper", o);
    o.b = (v->u & 0x0008) != 0; out.put("battery_low", o);
    return true;
}
constexpr FzConverter kFzZoneStatusReport{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zone_status_report },
    .user_config       = nullptr,
};

constexpr ZclWriteLookup kSensitivityLut[] = { {"low", 0}, {"medium", 1}, {"high", 2} };
constexpr ZclWriteLookup kKeepTimeLut[]    = { {"30", 0}, {"60", 1}, {"120", 2} };
constexpr ZclWriteSpec kSpecSensitivity{ "sensitivity", 0x0013, 0x20, 0, kSensitivityLut, 3 };
constexpr ZclWriteSpec kSpecKeepTime{ "keep_time", 0xF001, 0x20, 0, kKeepTimeLut, 3 };
constexpr TzConverter kTzSensitivity = ::zhc::generic::zcl_write_tz("ssIasZone", 0x0500, &kSpecSensitivity);
constexpr TzConverter kTzKeepTime    = ::zhc::generic::zcl_write_tz("ssIasZone", 0x0500, &kSpecKeepTime);

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasMotionAlarm,   // cmd 0x00 → occupancy + tamper + battery_low
    &::zhc::generic::kFzBattery,
    &kFzZoneStatusReport,
    &::zhc::generic::kFzIasZoneConfig,    // 0x0013 / 0xF001 → sensitivity / keep_time
};
const TzConverter* const kTz[] = { &kTzSensitivity, &kTzKeepTime };

constexpr const char* kSensitivityOpts[] = { "low", "medium", "high" };
constexpr const char* kKeepTimeOpts[]    = { "30", "60", "120" };
constexpr Expose kExposes[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, "PIR sensor sensitivity", kSensitivityOpts, 3},
    {"keep_time",   ExposeType::Enum, Access::StateSet, nullptr, "PIR keep time in seconds", kKeepTimeOpts, 3},
};

constexpr const char* kModels[] = { "TS0202" };
constexpr const char* kManus[]  = { "_TZ3000_lf56vpxj", "_TZ3040_o4mkahkc" };
constexpr BindingSpec kBindings[] = { {1, 0x0001} };
// z2m reporting.batteryPercentageRemaining / batteryVoltage: 1 h .. MAX, change 0.
constexpr ::zhc::ReportingSpec kReports[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 0, 0, ::zhc::kReportFlagOptional},
    {1, 0x0001, 0x0020, 0x20, 3600, 65000, 0, 0, ::zhc::kReportFlagOptional},
};
}  // namespace

extern const PreparedDefinition kDef_ZP01{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="ZP01", .vendor="Tuya",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
};

}  // namespace zhc::devices::tuya
