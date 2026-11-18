// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: CTM Lyng mTouch_OnOff (zigbeeModel "mTouch OnOff"), mTouch OnOff 2P
// OP touch switch with timer, new in z2m v26.126.0..v26.133.0. Endpoints 1/2.
// z2m: m.deviceEndpoints (state / power_on_behavior skip the endpoint suffix,
// and so do the endpoint-less timer keys, so this def carries no endpoint map),
// m.deviceTemperature, m.onOff (+ power_on_behavior, onOff reporting),
// m.binary timer_active = msOccupancySensing occupancy (1 -> true), reported
// 0 s / 1 h / 1, m.numeric timer_remaining = genOnOff onTime (0x4001) / 10
// rounded to whole seconds, m.commandsOnOff({endpointNames: ["2"]}): genOnOff
// on / off / offWithEffect / toggle commands -> action "on_2" / "off_2" /
// "toggle_2" (postfixed with the sending endpoint, as z2m does), endpoint 2's
// genOnOff output bound.
// Not ported: identify.
// z2m-source: ctm.ts #mTouch_OnOff.
#include "definitions/_generic/_shared.hpp"

#include <cstdint>

namespace zhc::devices::ctm {
namespace {

// genOnOff onTime (0x4001, u16 1/10 s) -> timer_remaining in whole seconds
// (z2m scale 10, precision 0: Math.round).
bool fz_timer_remaining(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("16385");   // 0x4001 onTime
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = (v->u + 5) / 10;
    out.put("timer_remaining", o);
    return true;
}

const FzConverter kFzTimerRemaining{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_timer_remaining },
    .user_config       = nullptr,
};

// [endpoint: other, 1, 2][off, on, toggle]; z2m postfixes only an endpoint
// the definition names.
constexpr const char* kActions[3][3] = {
    { "off",   "on",   "toggle" },
    { "off_1", "on_1", "toggle_1" },
    { "off_2", "on_2", "toggle_2" },
};

bool fz_command_on_off(const DecodedMessage& msg,
                       const FzConverter&,
                       const PreparedDefinition&,
                       RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::size_t cmd;
    switch (msg.command_id) {
        case 0x00: case 0x40: cmd = 0; break;   // off, offWithEffect
        case 0x01:            cmd = 1; break;   // on
        case 0x02:            cmd = 2; break;   // toggle
        default:              return false;
    }
    const std::size_t ep = (msg.src_endpoint == 1 || msg.src_endpoint == 2) ? msg.src_endpoint : 0;
    Value v{}; v.type = ValueType::StringRef; v.str = kActions[ep][cmd];
    out.put("action", v);
    return true;
}

const FzConverter kFzCommandOnOff{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_command_on_off },
    .user_config       = nullptr,
};

constexpr ::zhc::generic::ZclAttrRow kTimerActiveRows[] = {
    { 0x0000, "timer_active", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
};
constexpr ::zhc::generic::ZclAttrMap kTimerActiveMap{
    kTimerActiveRows, sizeof(kTimerActiveRows)/sizeof(kTimerActiveRows[0]) };
constexpr FzConverter kFzTimerActive = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kTimerActiveMap);

const FzConverter* const kFz_mTouch_OnOff[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzDeviceTemperature,
    &kFzTimerActive,
    &kFzTimerRemaining,
    &kFzCommandOnOff,
};
const TzConverter* const kTz_mTouch_OnOff[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};

constexpr const char* kModels_mTouch_OnOff[] = { "mTouch OnOff" };

constexpr const char* kPowerOn[] = { "off", "on", "toggle", "previous" };
constexpr const char* kActionValues[] = { "on_2", "off_2", "toggle_2" };

constexpr Expose kExposes_mTouch_OnOff[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kPowerOn, sizeof(kPowerOn)/sizeof(kPowerOn[0])},
    {"device_temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C",
     "Temperature of the device", nullptr, 0, ExposeCategory::Diagnostic},
    {"timer_active", ExposeType::Binary, Access::State, nullptr,
     "Indicates whether the timer is running (HEX switch in position 1-E)", nullptr, 0},
    {"timer_remaining", ExposeType::Numeric, Access::State, "s",
     "Time left before the timer switches the load off", nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, "Triggered action (e.g. a button click)",
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
};

constexpr BindingSpec kBindings_mTouch_OnOff[] = {
    {1, 0x0006},
    {1, 0x0406},
    {2, 0x0006},   // m.commandsOnOff: endpoint 2's genOnOff output
};
constexpr ReportingSpec kReports_mTouch_OnOff[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
    {1, 0x0006, 0x4003, 0x30, 0, 65000, 1, 0, kReportFlagOptional},   // startUpOnOff, may be unsupported
    {1, 0x0406, 0x0000, 0x18, 0, 3600, 1, 0},
};

}  // namespace

extern const PreparedDefinition kDef_mTouch_OnOff{
    .zigbee_models=kModels_mTouch_OnOff, .zigbee_models_count=sizeof(kModels_mTouch_OnOff)/sizeof(kModels_mTouch_OnOff[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mTouch_OnOff", .vendor="Ctm",
    .meta=nullptr, .exposes=kExposes_mTouch_OnOff, .exposes_count=sizeof(kExposes_mTouch_OnOff)/sizeof(kExposes_mTouch_OnOff[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mTouch_OnOff, .from_zigbee_count=sizeof(kFz_mTouch_OnOff)/sizeof(kFz_mTouch_OnOff[0]),
    .to_zigbee=kTz_mTouch_OnOff, .to_zigbee_count=sizeof(kTz_mTouch_OnOff)/sizeof(kTz_mTouch_OnOff[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_mTouch_OnOff, .bindings_count=sizeof(kBindings_mTouch_OnOff)/sizeof(kBindings_mTouch_OnOff[0]),
    .reports=kReports_mTouch_OnOff, .reports_count=sizeof(kReports_mTouch_OnOff)/sizeof(kReports_mTouch_OnOff[0]),
};

}  // namespace zhc::devices::ctm
