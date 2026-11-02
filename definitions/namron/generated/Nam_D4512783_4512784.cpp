// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Namron Zigbee Edge thermostat 4512783/4512784/4566702/4566703 —
// hand-ported (z2m v26.125.0 fzEdge / tzEdge rework).
// z2m-source: namron.ts #4566702.
//
// The Edge keeps its settings in custom hvacThermostat attributes (no
// manufacturer code), reported and read back as plain numbers:
//   0x8000 window_open_check, 0x8001 frost, 0x801f vacation_mode,
//   0x8022 auto_time (OFF/ON); 0x8002 window_state; 0x8003 week_program;
//   0x8004 sensor_mode; 0x8005 panel_brightness; 0x8006 fault (bit names);
//   0x8007 regulator_cycle; 0x8013 holiday_temp_set /100; 0x801d
//   regulator_percentage; 0x8023 countdown_set (5-minute steps); 0x8024
//   countdown_left (values above 120 mean none); 0x8025 max_heat_temp /10;
//   0x8029 screen_on_time.
// Standard attributes on top of the house thermostat decode: running_state
// from the 0x0029 bitmap (bit 0 heat, bit 1 cool), programming_operation_mode
// from 0x0025 (0x04 eco, 0x01 schedule, else setpoint), pi_heating_demand,
// calibration /10, cooling setpoint and the absolute setpoint limits.
// Writes ask for a default response: the firmware answers NOT_AUTHORIZED to
// several writes sent with default responses disabled.
// Not ported: upstream's read-before-write on frost / window_open_check /
// vacation_mode / auto_time (only a configure-time read precedes them here),
// programming_operation_mode writes (custom setEco / setProgram commands),
// countdown_set writes (minutes / 5), the °F mirrors 0x8011/0x8012 and their
// write twins 0x801b/0x8026, vacation dates, clock sync (0x800a/0x800b), the
// derived thermostat_mode, the 900 s week_program poll, the week program
// schedule text (cluster 0xE002) and keypad / display-unit settings.
#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"

#include <cstdio>
#include <cstring>

namespace zhc::devices::namron {
namespace {

constexpr std::uint16_t kClusterThermostat = 0x0201;

using ::zhc::generic::ZclAttrRow;
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

constexpr ZclWriteLookup kOnOff[] = { {"OFF", 0}, {"ON", 1} };
constexpr ZclWriteLookup kWindowState[] = { {"closed", 0}, {"open", 1} };
constexpr ZclWriteLookup kWeekProgram[] = {
    {"mon_fri_sat_sun", 0}, {"mon_sat_sun", 1}, {"no_time_off", 2}, {"time_off", 3} };
constexpr ZclWriteLookup kSensorMode[] = {
    {"air", 0}, {"floor", 1}, {"air_floor", 2}, {"external", 3},
    {"external_floor", 4}, {"floor_percent", 5}, {"regulator", 6} };
constexpr ZclWriteLookup kScreenOnTime[] = { {"always_on", 0}, {"10s", 1}, {"30s", 2}, {"60s", 3} };

constexpr ZclAttrRow kEdgeRows[] = {
    {0x0003, "abs_min_heat_setpoint_limit", 100},
    {0x0004, "abs_max_heat_setpoint_limit", 100},
    {0x0008, "pi_heating_demand"},
    {0x0010, "local_temperature_calibration", 10},
    {0x0011, "occupied_cooling_setpoint", 100},
    {0x8000, "window_open_check", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool},
    {0x8001, "frost", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool},
    {0x8002, "window_state", 1, kWindowState, 2},
    {0x8003, "week_program", 1, kWeekProgram, 4},
    {0x8004, "sensor_mode", 1, kSensorMode, 7},
    {0x8005, "panel_brightness"},
    {0x8007, "regulator_cycle"},
    {0x8013, "holiday_temp_set", 100},
    {0x801d, "regulator_percentage"},
    {0x801f, "vacation_mode", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool},
    {0x8022, "auto_time", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool},
    {0x8025, "max_heat_temp", 10},
    {0x8029, "screen_on_time", 1, kScreenOnTime, 4},
};
constexpr ::zhc::generic::ZclAttrMap kEdgeMap{ kEdgeRows, sizeof(kEdgeRows)/sizeof(kEdgeRows[0]) };
constexpr FzConverter kFzEdgeAttrs = ::zhc::generic::zcl_attr_fz("hvacThermostat", &kEdgeMap);

bool as_uint(const Value* v, std::uint32_t& out) {
    if (!v) return false;
    if (v->type == ValueType::Uint) { out = static_cast<std::uint32_t>(v->u); return true; }
    if (v->type == ValueType::Int && v->i >= 0) { out = static_cast<std::uint32_t>(v->i); return true; }
    if (v->type == ValueType::Bool) { out = v->b ? 1 : 0; return true; }
    return false;
}

void put_str(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, const char* s) {
    Value o{}; o.type = ValueType::StringRef; o.str = s;
    out.put(key, o);
}
void put_uint(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, std::uint32_t n) {
    Value o{}; o.type = ValueType::Uint; o.u = n;
    out.put(key, o);
}

// fault (0x8006) bit -> ERR code in the manual; bit 0 keeps Namron's er0 name.
constexpr const char* kFaultNames[8] = {
    "er0", "zigbee_error", "bluetooth_error", "internal_sensor_error",
    "floor_sensor_error", "external_sensor_error", "overheat_error", "overload_error" };

// The attributes that need more than a row: bitmaps, scaled steps and the
// fault list.
bool fz_edge_local(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                   RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool any = false;
    std::uint32_t v = 0;
    if (as_uint(msg.payload.find("41"), v)) {            // 0x0029 runningState
        put_str(out, "running_state", (v & 0x01) ? "heat" : (v & 0x02) ? "cool" : "idle");
        any = true;
    }
    if (as_uint(msg.payload.find("37"), v)) {            // 0x0025 programingOperMode
        put_str(out, "programming_operation_mode", (v & 0x04) ? "eco" : (v & 0x01) ? "schedule" : "setpoint");
        any = true;
    }
    if (as_uint(msg.payload.find("32803"), v)) {         // 0x8023 countdown_set, 5-minute steps
        put_uint(out, "countdown_set", v * 5);
        any = true;
    }
    if (as_uint(msg.payload.find("32804"), v)) {         // 0x8024 countdown_left
        put_uint(out, "countdown_left", v > 120 ? 0 : v);
        any = true;
    }
    if (as_uint(msg.payload.find("32774"), v)) {         // 0x8006 fault bitmap
        char buf[160] = "none";
        std::size_t n = 0;
        for (int bit = 0; bit < 8; ++bit) {
            if (!(v & (1u << bit))) continue;
            const int w = std::snprintf(buf + n, sizeof(buf) - n, "%s%s", n ? "," : "", kFaultNames[bit]);
            if (w > 0) n += static_cast<std::size_t>(w);
        }
        const std::size_t len = n ? n : std::strlen(buf);
        if (const char* s = ctx.alloc_str(buf, len)) { put_str(out, "fault", s); any = true; }
    }
    return any;
}

constexpr FzConverter kFzEdgeLocal{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_edge_local },
    .user_config       = nullptr,
};

constexpr std::uint8_t kDR = ::zhc::generic::kZclWriteFlagDefaultResponse;
constexpr ZclWriteSpec kWrites[] = {
    {"occupied_cooling_setpoint",     0x0011, 0x29, 0, nullptr, 0, kDR, 100},
    {"local_temperature_calibration", 0x0010, 0x28, 0, nullptr, 0, kDR, 10},
    {"window_open_check",             0x8000, 0x10, 0, kOnOff, 2, kDR},
    {"frost",                         0x8001, 0x10, 0, kOnOff, 2, kDR},
    {"sensor_mode",                   0x8004, 0x30, 0, kSensorMode, 7, kDR},
    {"panel_brightness",              0x8005, 0x20, 0, nullptr, 0, kDR},
    {"holiday_temp_set",              0x8013, 0x29, 0, nullptr, 0, kDR, 100},
    {"regulator_percentage",          0x801d, 0x29, 0, nullptr, 0, kDR},
    {"vacation_mode",                 0x801f, 0x10, 0, kOnOff, 2, kDR},
    {"auto_time",                     0x8022, 0x10, 0, kOnOff, 2, kDR},
    {"max_heat_temp",                 0x8025, 0x29, 0, nullptr, 0, kDR, 10},
    {"screen_on_time",                0x8029, 0x30, 0, kScreenOnTime, 4, kDR},
};
#define ZHC_EDGE_TZ(i) ::zhc::generic::zcl_write_tz("hvacThermostat", kClusterThermostat, &kWrites[i])
constexpr TzConverter kTzWrites[] = {
    ZHC_EDGE_TZ(0), ZHC_EDGE_TZ(1), ZHC_EDGE_TZ(2), ZHC_EDGE_TZ(3), ZHC_EDGE_TZ(4), ZHC_EDGE_TZ(5),
    ZHC_EDGE_TZ(6), ZHC_EDGE_TZ(7), ZHC_EDGE_TZ(8), ZHC_EDGE_TZ(9), ZHC_EDGE_TZ(10), ZHC_EDGE_TZ(11),
};
#undef ZHC_EDGE_TZ
static_assert(sizeof(kTzWrites)/sizeof(kTzWrites[0]) == sizeof(kWrites)/sizeof(kWrites[0]));

const FzConverter* const kFz_D4512783_4512784[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzHumidity,
    &kFzEdgeAttrs,
    &kFzEdgeLocal,
};
const TzConverter* const kTz_D4512783_4512784[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
    &kTzWrites[0], &kTzWrites[1], &kTzWrites[2], &kTzWrites[3], &kTzWrites[4], &kTzWrites[5],
    &kTzWrites[6], &kTzWrites[7], &kTzWrites[8], &kTzWrites[9], &kTzWrites[10], &kTzWrites[11],
};
constexpr const char* kModels_D4512783_4512784[] = { "4512783", "4512784", "4566702", "4566703" };

constexpr const char* kSystemModeOpts[] = { "off", "heat", "cool" };
constexpr const char* kRunningOpts[] = { "idle", "heat", "cool" };
constexpr const char* kProgOpts[] = { "setpoint", "schedule", "eco" };
constexpr const char* kSensorOpts[] = { "air", "floor", "air_floor", "external", "external_floor", "floor_percent", "regulator" };
constexpr const char* kScreenOpts[] = { "always_on", "10s", "30s", "60s" };
constexpr const char* kWeekOpts[] = { "mon_fri_sat_sun", "mon_sat_sun", "no_time_off", "time_off" };
constexpr const char* kWindowOpts[] = { "closed", "open" };

constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"occupied_cooling_setpoint", ExposeType::Numeric, Access::StateSet, "°C",
     "Cooling setpoint; the device has one setpoint on its display. Cooling needs Equipment = Water on the device.",
     nullptr, 0, ExposeCategory::State, 10, 40, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSystemModeOpts, 3},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kRunningOpts, 3},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, -10, 10, 0},
    {"pi_heating_demand", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"programming_operation_mode", ExposeType::Enum, Access::State, nullptr,
     "Run mode: setpoint = manual, schedule = weekly program, eco = ECO mode", kProgOpts, 3},
    {"sensor_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "Which sensor(s) control heating, or regulator for plain duty-cycle control", kSensorOpts, 7, ExposeCategory::Config},
    {"regulator_percentage", ExposeType::Numeric, Access::StateSet, "%",
     "Output duty cycle when sensor_mode is regulator", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"regulator_cycle", ExposeType::Numeric, Access::State, "min", "Regulator cycle length held by the Zigbee module", nullptr, 0, ExposeCategory::Diagnostic},
    {"frost", ExposeType::Binary, Access::StateSet, nullptr, "Frost protection (only while system_mode is heat)", nullptr, 0},
    {"window_open_check", ExposeType::Binary, Access::StateSet, nullptr, "Open-window detection (auto pause heating)", nullptr, 0, ExposeCategory::Config},
    {"window_state", ExposeType::Enum, Access::State, nullptr, "Open-window detection result", kWindowOpts, 2},
    {"vacation_mode", ExposeType::Binary, Access::StateSet, nullptr, "Holds holiday_temp_set until the vacation end", nullptr, 0},
    {"holiday_temp_set", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, 5, 40, 0},
    {"max_heat_temp", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, 15, 35, 0},
    {"auto_time", ExposeType::Binary, Access::StateSet, nullptr, "Let the device auto-sync its clock", nullptr, 0, ExposeCategory::Config},
    {"panel_brightness", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::Config, 1, 100, 1},
    {"screen_on_time", ExposeType::Enum, Access::StateSet, nullptr, "How long the backlight stays on after a touch", kScreenOpts, 4, ExposeCategory::Config},
    {"week_program", ExposeType::Enum, Access::State, nullptr, "Week program split set on the device", kWeekOpts, 4, ExposeCategory::Diagnostic},
    {"countdown_set", ExposeType::Numeric, Access::State, "min", "Countdown timer; heating stops when it reaches 0", nullptr, 0},
    {"countdown_left", ExposeType::Numeric, Access::State, "min", "Minutes left of a running countdown", nullptr, 0},
    {"fault", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006}, {1, 0x000A}, {1, 0x0201}, {1, 0x0204}, {1, 0x0405}, {1, 0x0702}, {1, 0x0B04},
};

constexpr ReportingSpec kReports[] = {
    {1, 0x0201, 0x0000, 0x29, 10, 300, 10,  0},   // localTemp
    {1, 0x0201, 0x0012, 0x29, 10, 300, 50,  0},   // occupiedHeatingSetpoint
    {1, 0x0201, 0x0011, 0x29, 10, 300, 50,  0},   // occupiedCoolingSetpoint
    {1, 0x0405, 0x0000, 0x21, 10, 300, 100, 0},   // humidity
};

// Initial read of everything that is not reported, eight attributes a frame.
constexpr std::uint8_t kReadStd[] = { 0x29,0x00, 0x10,0x00, 0x08,0x00, 0x25,0x00, 0x03,0x00, 0x04,0x00, 0x11,0x00 };
constexpr std::uint8_t kReadCustom1[] = { 0x00,0x80, 0x01,0x80, 0x02,0x80, 0x03,0x80, 0x04,0x80, 0x05,0x80, 0x06,0x80, 0x07,0x80 };
constexpr std::uint8_t kReadCustom2[] = { 0x13,0x80, 0x1d,0x80, 0x1f,0x80, 0x22,0x80, 0x23,0x80, 0x24,0x80, 0x25,0x80, 0x29,0x80 };
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, kClusterThermostat, 0x00, 0, kReadStd, sizeof(kReadStd), 0 },
    { ConfigStepOp::Read, 1, kClusterThermostat, 0x00, 0, kReadCustom1, sizeof(kReadCustom1), 0 },
    { ConfigStepOp::Read, 1, kClusterThermostat, 0x00, 0, kReadCustom2, sizeof(kReadCustom2), 0 },
};

constexpr WhiteLabel kWhiteLabels_D4512783_4512784[] = {
    {"Namron","4566702"},
};

}  // namespace

extern const PreparedDefinition kDef_D4512783_4512784{
    .zigbee_models=kModels_D4512783_4512784, .zigbee_models_count=sizeof(kModels_D4512783_4512784)/sizeof(kModels_D4512783_4512784[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4512783/4512784", .vendor="Namron",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=kWhiteLabels_D4512783_4512784, .white_labels_count=sizeof(kWhiteLabels_D4512783_4512784)/sizeof(kWhiteLabels_D4512783_4512784[0]),
    .from_zigbee=kFz_D4512783_4512784, .from_zigbee_count=sizeof(kFz_D4512783_4512784)/sizeof(kFz_D4512783_4512784[0]),
    .to_zigbee=kTz_D4512783_4512784, .to_zigbee_count=sizeof(kTz_D4512783_4512784)/sizeof(kTz_D4512783_4512784[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
    .config_callbacks=nullptr, .config_callbacks_count=0,
    .endpoint_map=nullptr, .endpoint_map_count=0,
    .endpoint_action_suffix=false,
    .default_endpoint=0,
    .power_source_override=0x01,  // Mains (single phase)
};

}  // namespace zhc::devices::namron
