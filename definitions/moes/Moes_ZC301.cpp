// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes ZC301 Zigbee curtain motor, TS030F _TZ3210_hs01bacp (z2m
// v26.125.0 window). Reports arrive as Tuya datapoints; commands go out as
// closuresWindowCovering:
//   DP3   position = 100 - raw (ignored outside 0..100)
//   DP5   calibration_completed = (raw == 3)
//   DP7   work_state standby/opening/closing + state STOP/OPEN/CLOSE
//   DP10  time_total, ms -> s
//   DP13  battery, DP101 charge_state, DP107 curtain_close_direction
//         (false UP / true DOWN), DP155 charge_led_switch
// position is written as goToLiftPercentage(100 - position), the z2m
// cover_position_tilt default, so it reads back in the same sense as DP3.
// state OPEN/CLOSE/STOP -> upOpen/downClose/stop. calibration writes the
// Tuya attribute tuyaMotorReversal (0xF002, enum8): enter_mode 0, up_start 1,
// down_start 2; save_limit sends stop. A 0xF002 report of 3 also means
// calibration completed. Not ported: invert_cover option, the optimistic
// calibration_completed=false after a calibration write.
// z2m-source: moes.ts #ZC301.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

#include <cmath>
#include <cstring>

namespace zhc::devices::moes {
namespace {

constexpr std::uint16_t kClusterWindowCovering = 0x0102;

constexpr ::zhc::tuya::TuyaEnumEntry kCalibrated[] = { {3, "true"} };
constexpr ::zhc::tuya::TuyaEnumEntry kWorkState[] = { {0, "standby"}, {1, "opening"}, {2, "closing"} };
constexpr ::zhc::tuya::TuyaEnumEntry kState[] = { {0, "STOP"}, {1, "OPEN"}, {2, "CLOSE"} };
constexpr ::zhc::tuya::TuyaEnumEntry kChargeState[] = { {0, "uncharged"}, {1, "charging"}, {2, "charged"} };
constexpr ::zhc::tuya::TuyaEnumEntry kCloseDirection[] = { {1, "DOWN"}, {0, "UP"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::position_inv(3, "position"),
        { 5, "calibration_completed", ::zhc::TuyaDpType::Enum, 1, kCalibrated, 1, ::zhc::tuya::kTuyaDpFlagEnumBool },
        ::zhc::tuya::dp::enum_lookup(7, "work_state", kWorkState, 3),
        ::zhc::tuya::dp::enum_lookup(7, "state", kState, 3),
        ::zhc::tuya::dp::numeric(10, "time_total", 1000),
        ::zhc::tuya::dp::numeric(13, "battery", 1),
        ::zhc::tuya::dp::enum_lookup(101, "charge_state", kChargeState, 3),
        { 107, "curtain_close_direction", ::zhc::TuyaDpType::Bool, 1, kCloseDirection, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::binary(155, "charge_led_switch"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};

// tuyaMotorReversal (0xF002) report: 3 = calibration completed, 0..2 = not.
bool fz_zc301_calibration(const DecodedMessage& msg, const FzConverter&,
                          const PreparedDefinition&, RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("61442");
    if (!v || v->type != ValueType::Uint || v->u > 3) return false;
    Value b{}; b.type = ValueType::Bool; b.b = v->u == 3;
    out.put("calibration_completed", b);
    return true;
}

constexpr FzConverter kFzCalibration{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zc301_calibration },
    .user_config       = nullptr,
};

// goToLiftPercentage (0x05) with the z2m default inversion.
bool tz_zc301_position(std::string_view key, const Value& input,
                       const TzConverter&, const PreparedDefinition&,
                       RuntimeContext&, std::span<std::uint8_t> out_frame,
                       std::size_t& out_size) {
    if (key != "position") return false;
    std::int64_t pct = -1;
    if (input.type == ValueType::Uint) pct = static_cast<std::int64_t>(input.u);
    else if (input.type == ValueType::Int) pct = input.i;
    else if (input.type == ValueType::Float) pct = std::lround(input.f);
    if (pct < 0 || pct > 100 || out_frame.size() < 4) return false;
    out_frame[0] = 0x11; out_frame[1] = 0; out_frame[2] = 0x05;
    out_frame[3] = static_cast<std::uint8_t>(100 - pct);
    out_size = 4;
    return true;
}

// calibration "save_limit" = stop (0x02); other actions fall through to the
// 0xF002 attribute write below.
bool tz_zc301_save_limit(std::string_view key, const Value& input,
                         const TzConverter&, const PreparedDefinition&,
                         RuntimeContext&, std::span<std::uint8_t> out_frame,
                         std::size_t& out_size) {
    if (key != "calibration" || input.type != ValueType::StringRef || !input.str ||
        std::strcmp(input.str, "save_limit") != 0 || out_frame.size() < 3) return false;
    out_frame[0] = 0x11; out_frame[1] = 0; out_frame[2] = 0x02;
    out_size = 3;
    return true;
}

constexpr TzConverter kTzPosition{
    .key = "position", .cluster = "closuresWindowCovering", .cluster_id = kClusterWindowCovering,
    .command_id = 0x05, .fn = tz_zc301_position, .user_config = nullptr,
};
constexpr TzConverter kTzSaveLimit{
    .key = "calibration", .cluster = "closuresWindowCovering", .cluster_id = kClusterWindowCovering,
    .command_id = 0x02, .fn = tz_zc301_save_limit, .user_config = nullptr,
};
constexpr ::zhc::generic::ZclWriteLookup kCalibrationActions[] = {
    {"enter_mode", 0}, {"up_start", 1}, {"down_start", 2} };
constexpr ::zhc::generic::ZclWriteSpec kSpecCalibration{
    "calibration", 0xF002, 0x30, 0, kCalibrationActions, 3 };
constexpr TzConverter kTzCalibration = ::zhc::generic::zcl_write_tz(
    "closuresWindowCovering", kClusterWindowCovering, &kSpecCalibration);

const FzConverter* const kFz[] = {
    &kFzCalibration,
    &::zhc::tuya::factory::detail::BoundFz<cfg>::converter,
};
// Order matters: the first converter that accepts a key wins, so position /
// state / calibration go out as ZCL before the DP writer could claim them.
const TzConverter* const kTz[] = {
    &kTzPosition,
    &::zhc::generic::kTzCoverState,
    &kTzSaveLimit,
    &kTzCalibration,
    &::zhc::tuya::factory::detail::BoundTz<cfg>::converter,
};

constexpr const char* kCoverStates[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kCalibrationOpts[] = { "enter_mode", "up_start", "save_limit", "down_start" };
constexpr const char* kWorkStateOpts[] = { "standby", "opening", "closing" };
constexpr const char* kChargeStateOpts[] = { "uncharged", "charging", "charged" };
constexpr const char* kCloseDirectionOpts[] = { "DOWN", "UP" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kCoverStates, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0,
     ExposeCategory::State, 0, 100, 1},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"calibration", ExposeType::Enum, Access::Set, nullptr,
     "Enter mode and wait for the motor to stop; up_start, STOP to save, down_start, STOP to save. "
     "The motor reports completion.", kCalibrationOpts, 4, ExposeCategory::Config},
    {"calibration_completed", ExposeType::Binary, Access::State, nullptr,
     "Whether the device has reported calibration completion", nullptr, 0},
    {"work_state", ExposeType::Enum, Access::State, nullptr, nullptr, kWorkStateOpts, 3,
     ExposeCategory::Diagnostic},
    {"time_total", ExposeType::Numeric, Access::State, "s",
     "Total travel time reported by the device after calibration", nullptr, 0,
     ExposeCategory::State, 0, 120},
    {"charge_state", ExposeType::Enum, Access::State, nullptr, nullptr, kChargeStateOpts, 3,
     ExposeCategory::Diagnostic},
    {"curtain_close_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kCloseDirectionOpts, 2, ExposeCategory::Config},
    {"charge_led_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config},
};

constexpr const char* kModels[] = { "TS030F" };
constexpr const char* kManus[] = { "_TZ3210_hs01bacp" };

}  // namespace

extern const PreparedDefinition kDef_ZC301{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="ZC301", .vendor="Moes",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=::zhc::tuya::extend::magic_packet(), .on_event=nullptr,
};

}  // namespace zhc::devices::moes
