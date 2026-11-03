// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Aqara FP310 presence sensor (z2m v26.76.0 parity).
// z2m-source: lumi.ts #FP310
//
// z2m: m.temperature()/m.humidity()/m.illuminance() (standard ZCL clusters) +
// lumiBattery(voltageAttribute 0x0017) + fp1ePresence() (0xFCC0 mmWave) +
// presence_sensitivity / distance config. Ported here as the standard ZCL
// channels: temperature (0x0402 /100), humidity (0x0405 /100), illuminance
// (0x0400) and occupancy (msOccupancySensing 0x0406).
//
// The 0xFCC0 0x00F7 heartbeat (z2m lumi_specific + lumiBattery on tag 0x17 with
// the 2850-3000 mV curve) gives battery, voltage and the outage count, and the
// hub answers the "leave" flag (lumiPreventLeave).
//
// mmWave presence (fp1ePresence, 0xFCC0 0x0142) publishes `presence`; 0x0142 is
// bound, reported and read at configure (z2m v26.125.0).
// DEFERRED (all Aqara 0xFCC0 manuSpecificLumi): presence_sensitivity,
// absence delay, approach/distance settings.
// NOTE: `occupancy` populates only if the device mirrors presence onto
// standard msOccupancySensing.
#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {
// z2m lumi_specific + lumiBattery({voltageToPercentage: {min: 2850, max: 3000},
// voltageAttribute: 0x0017}).
constexpr ::zhc::lumi::LumiHeartbeatOpts kHeartbeatOpts{
    .specific = true, .tag1_battery = false, .lb_volt_tag = 0x17, .lb_pct_tag = 1,
    .lb_curve = true, .min_mv = 2850, .max_mv = 3000,
};
constexpr FzConverter kFzHeartbeat = ::zhc::lumi::lumi_heartbeat_converter(&kHeartbeatOpts);
// z2m fp1ePresence (m.binary on 0xFCC0 0x0142): 1 is presence, anything else none.
bool fz_presence(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                 RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("322");   // 0x0142
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Bool; o.b = v->u == 1;
    return out.put("presence", o);
}
constexpr FzConverter kFzPresence{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificLumi",
    .type_mask         = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_presence },
    .user_config       = nullptr,
};
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzOccupancy,
    &kFzHeartbeat,
    &::zhc::lumi::kFzLumiPreventLeave,
    &kFzPresence,
};
constexpr const char* kModels[] = { "lumi.sensor_occupy.acn1" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"presence",    ExposeType::Binary,  Access::State, nullptr,
     "Indicates whether the device detected presence", nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0400},  // msIlluminanceMeasurement
};
// z2m v26.125.0 (#13384): report 0x0142 so presence keeps updating after idle,
// and read it at configure. z2m also binds manuSpecificLumi; no lumi def here
// does (3b74e93): the device reports 0xFCC0 to the coordinator unbound.
constexpr ReportingSpec kReports[] = {
    {1, 0xFCC0, 0x0142, 0x20, 0, 3600, 1, 0x115F},
};
constexpr std::uint8_t kReadPresence[] = { 0x42, 0x01 };
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0xFCC0, 0x00, 0, kReadPresence, sizeof(kReadPresence), 0, 0x115F },
};

extern const PreparedDefinition kDefFP310{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .model="FP310",.vendor="Xiaomi",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};
}  // namespace zhc::devices::lumi
