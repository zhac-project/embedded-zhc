// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Aqara ZNJLBL01LM — Roller shade driver E1 (lumi.curtain.acn002), which sat
// on the ZNCLDJ14LM stub until z2m v26.125.0 parity (#13479 / #13362):
//   * position: genAnalogOutput presentValue, read and written as float32;
//     OPEN / CLOSE write 100 / 0, STOP writes genMultistateOutput
//     presentValue = 2.
//   * motor_state / running: genMultistateOutput presentValue {0 closing,
//     1 opening, 2 stopped, 3 blocked}; once the motor stops the position is
//     read back (z2m lumiReadPositionOnReport("genMultistateOutput")).
//   * manuSpecificLumi: reverse_direction (0x0400), motor_speed (0x0408),
//     charging_status (0x0409), battery (0x040A, also heartbeat tag 101).
// Not ported: invert_cover, hand_open / limits_calibration (not exposed
// upstream). z2m publishes no optimistic position for this model; ZHAC's
// command path does.
// z2m-source: lumi.ts #ZNJLBL01LM.
#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"

#include <cctype>
#include <cstring>
#include <string_view>

namespace zhc::devices::lumi {
namespace {

constexpr std::uint16_t kLumiManu = 0x115F;

bool ieq(const char* s, const char* target) {
    for (; *s && *target; ++s, ++target)
        if (std::tolower(static_cast<unsigned char>(*s)) != *target) return false;
    return *s == *target;
}

bool fz_curtain_status(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                       RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");   // presentValue
    if (!v || v->type != ValueType::Uint) return false;
    static constexpr const char* kStates[] = {"closing", "opening", "stopped", "blocked"};
    if (v->u < 4) {
        Value s{}; s.type = ValueType::StringRef; s.str = kStates[v->u];
        out.put("motor_state", s);
    }
    Value r{}; r.type = ValueType::Bool; r.b = v->u < 2;
    out.put("running", r);
    if (v->u > 1 && ctx.configure_read) {
        static constexpr std::uint8_t kPresentValue[] = {0x55, 0x00};
        ctx.configure_read(ctx.device_index, 1, 0x000D, kPresentValue, 1, 0);
    }
    return true;
}
constexpr FzConverter kFzCurtainStatus{
    .family            = FrameFamily::Zcl,
    .cluster           = "genMultistateOutput",
    .type_mask         = type_bit(MessageType::AttributeReport),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_curtain_status },
    .user_config       = nullptr,
};

constexpr ::zhc::generic::ZclWriteLookup kSpeeds[] = { {"low", 0}, {"medium", 1}, {"high", 2} };
constexpr ::zhc::generic::ZclAttrRow kOptionRows[] = {
    { 0x0400, "reverse_direction", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0408, "motor_speed", 1, kSpeeds, 3 },
    { 0x0409, "charging_status", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x040A, "battery" },
};
constexpr ::zhc::generic::ZclAttrMap kOptionMap{
    kOptionRows, sizeof(kOptionRows)/sizeof(kOptionRows[0]), kLumiManu };
constexpr FzConverter kFzOptions = ::zhc::generic::zcl_attr_fz("manuSpecificLumi", &kOptionMap);

// STOP on genMultistateOutput (u16 2); OPEN / CLOSE and `position` on
// genAnalogOutput presentValue as float32.
bool tz_curtain(std::string_view key, const Value& in, const TzConverter& self, const PreparedDefinition&,
                RuntimeContext&, std::span<std::uint8_t> out, std::size_t& out_size) {
    out_size = 0;
    const bool is_str = in.type == ValueType::StringRef && in.str;
    if (self.cluster_id == 0x0013) {
        if (!is_str || !ieq(in.str, "stop") || out.size() < 8) return false;
        const std::uint8_t f[] = {0x10, 0x00, 0x02, 0x55, 0x00, 0x21, 0x02, 0x00};
        std::memcpy(out.data(), f, sizeof(f));
        out_size = sizeof(f);
        return true;
    }
    float pos = 0.0f;
    if (key == "state") {
        if (!is_str) return false;
        if (ieq(in.str, "open") || ieq(in.str, "on"))        pos = 100.0f;
        else if (ieq(in.str, "close") || ieq(in.str, "off")) pos = 0.0f;
        else return false;
    } else if (in.type == ValueType::Float) {
        pos = in.f;
    } else if (in.type == ValueType::Uint) {
        pos = static_cast<float>(in.u);
    } else if (in.type == ValueType::Int) {
        pos = static_cast<float>(in.i);
    } else {
        return false;
    }
    if (pos < 0.0f) pos = 0.0f;
    if (pos > 100.0f) pos = 100.0f;
    if (out.size() < 10) return false;
    std::uint32_t bits = 0;
    std::memcpy(&bits, &pos, sizeof(bits));
    const std::uint8_t f[] = {0x10, 0x00, 0x02, 0x55, 0x00, 0x39,
                              std::uint8_t(bits), std::uint8_t(bits >> 8), std::uint8_t(bits >> 16),
                              std::uint8_t(bits >> 24)};
    std::memcpy(out.data(), f, sizeof(f));
    out_size = sizeof(f);
    return true;
}
constexpr TzConverter curtain_tz(const char* key, const char* cluster, std::uint16_t cluster_id) {
    return TzConverter{ .key = key, .cluster = cluster, .cluster_id = cluster_id, .command_id = 0x02,
                        .fn = tz_curtain, .user_config = nullptr };
}
constexpr TzConverter kTzStop     = curtain_tz("state", "genMultistateOutput", 0x0013);
constexpr TzConverter kTzState    = curtain_tz("state", "genAnalogOutput", 0x000D);
constexpr TzConverter kTzPosition = curtain_tz("position", "genAnalogOutput", 0x000D);

constexpr ::zhc::generic::ZclWriteSpec kReverseSpec{ "reverse_direction", 0x0400, 0x10, kLumiManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpeedSpec{ "motor_speed", 0x0408, 0x20, kLumiManu, kSpeeds, 3 };
constexpr TzConverter kTzReverse = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kReverseSpec);
constexpr TzConverter kTzSpeed   = ::zhc::generic::zcl_write_tz("manuSpecificLumi", 0xFCC0, &kSpeedSpec);

const FzConverter* const kFz[] = {
    &::zhc::lumi::kFzLumiCurtainPosition,
    &kFzCurtainStatus,
    &kFzOptions,
    &::zhc::lumi::kFzLumiHeartbeat,
};
const TzConverter* const kTz[] = { &kTzStop, &kTzState, &kTzPosition, &kTzReverse, &kTzSpeed };
constexpr const char* kModels[] = { "lumi.curtain.acn002" };
constexpr WhiteLabel kWhiteLabels[] = { {"Aqara", "RSD-M01"} };
constexpr const char* kCoverStates[] = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kMotorStates[] = { "closing", "opening", "stopped", "blocked" };
constexpr const char* kSpeedOpts[] = { "low", "medium", "high" };

constexpr Expose kExposes[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kCoverStates, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"charging_status", ExposeType::Binary, Access::State, nullptr, "The current charging status.", nullptr, 0},
    {"reverse_direction", ExposeType::Binary, Access::StateSet, nullptr,
     "Whether the curtain direction is inverted", nullptr, 0},
    {"motor_state", ExposeType::Enum, Access::State, nullptr, "The current state of the motor.", kMotorStates, 4},
    {"running", ExposeType::Binary, Access::State, nullptr, "Whether the motor is moving or not", nullptr, 0},
    {"motor_speed", ExposeType::Enum, Access::StateSet, nullptr, "Controls the motor speed", kSpeedOpts, 3,
     ExposeCategory::Config},
};

// z2m configure reads the battery (0x040A).
constexpr std::uint8_t kReadBattery[] = { 0x0A, 0x04 };
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0xFCC0, 0x00, 0, kReadBattery, sizeof(kReadBattery), 0, kLumiManu },
};

}  // namespace

extern const PreparedDefinition kDefZNJLBL01LM{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .model="ZNJLBL01LM", .vendor="Aqara",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=kWhiteLabels, .white_labels_count=sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
    .power_source_override = 0x03,   // ZCL Basic 0x0007 = Battery (z2m device.powerSource)
};

}  // namespace zhc::devices::lumi
