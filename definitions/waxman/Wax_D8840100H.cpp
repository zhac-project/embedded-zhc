// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Waxman 8840100H leakSMART water sensor v2 (z2m v26.115.1 window).
//
// The leak state lives in Appliance Events and Alerts (0x0B02), which has no
// attributes: the device sends alertsNotification (0x01) on its own and
// answers getAlerts with getAlertsRsp (0x00), both carrying the full list of
// active alerts. Body: alertsCount (bits 0-3 = number of alerts), then one
// 24-bit alert each — bits 0-7 id, 8-11 category, 12-13 presence (0 means
// recovered). As upstream: a present alert with id 0x82 is the low-battery
// warning; any other present alert is reported as water (fail-safe — the wet
// id has never been captured). An empty list clears both.
//
// Configure binds 0x0B02 and asks getAlerts once so the state starts known;
// battery (voltage + %) and temperature as m.battery / m.temperature.
// z2m-source: waxman.ts #8840100H.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::waxman {
namespace {

constexpr std::uint8_t kAlertIdLowBattery = 0x82;

bool fz_leaksmart_alerts(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                         RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto b = msg.raw_body;
    if (b.empty()) return false;
    const std::size_t count = b[0] & 0x0F;
    bool water = false, low_battery = false;
    for (std::size_t i = 0; i < count && 3 + 3 * i < b.size(); ++i) {
        const std::uint32_t alert = static_cast<std::uint32_t>(b[1 + 3 * i]) |
                                    (static_cast<std::uint32_t>(b[2 + 3 * i]) << 8) |
                                    (static_cast<std::uint32_t>(b[3 + 3 * i]) << 16);
        if (((alert >> 12) & 0x3) == 0) continue;           // recovered
        if ((alert & 0xFF) == kAlertIdLowBattery) low_battery = true;
        else water = true;
    }
    Value v{}; v.type = ValueType::Bool;
    v.b = water;       out.put("water_leak", v);
    v.b = low_battery; out.put("battery_low", v);
    return true;
}

constexpr FzConverter alerts_cmd(std::uint8_t cmd) {
    return FzConverter{
        .family            = FrameFamily::Zcl,
        .cluster           = "haApplianceEventsAlerts",
        .type_mask         = type_bit(MessageType::Command),
        .command_id        = cmd,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = fz_leaksmart_alerts },
        .user_config       = nullptr,
    };
}
constexpr FzConverter kFzAlertsRsp = alerts_cmd(0x00);            // getAlertsRsp
constexpr FzConverter kFzAlertsNotification = alerts_cmd(0x01);   // alertsNotification

const FzConverter* const kFz[] = {
    &kFzAlertsNotification,
    &kFzAlertsRsp,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};
constexpr Expose kExp[] = {
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "leakSMART Water Sensor V2" };
constexpr BindingSpec kBind[] = {
    {1, 0x0B02},
    {1, 0x0001},
    {1, 0x0402},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 10, 0},   // batteryPercentageRemaining
    {1, 0x0001, 0x0020, 0x20, 3600, 65000, 10, 0},   // batteryVoltage (voltageReporting: true)
    {1, 0x0402, 0x0000, 0x29, 10, 3600, 100, 0},     // temperature
};
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Cmd, 1, 0x0B02, 0x00, 0, nullptr, 0, 0 },   // getAlerts
};
}  // namespace

extern const PreparedDefinition kDef_D8840100H{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="8840100H", .vendor="Waxman",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::waxman
