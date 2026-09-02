// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Schneider CCTFR6400 — "Temperature/Humidity measurement with
// thermostat interface" (modelID "Thermostat"). Uses shared schneider converters.
// z2m-source: schneider.ts #CCTFR6400.
//
// GRADUATED from generated/ (hand-edited — do NOT let the generator clobber).
// The thermostat half was wired, but the device's named "Humidity measurement"
// was missing: z2m fromZigbee carries fz.humidity + e.humidity() and binds
// msRelativeHumidity, yet the port had no kFzHumidity. Added the generic
// humidity converter (cluster 0x0405) + expose + binding to restore parity.
// (Temperature surfaces as local_temperature via the hvacThermostat path.)
//
// 2026-09-02 (z2m v26.103.0, #13053 "hub-less operation"): the device keeps no
// boost state of its own — ~8 s after the last centre-button press it sends
// ONE manufacturer-specific hvacThermostat command 0x80
// (schneiderWiserThermostatBoost: [command u8][enable u8][temperature u16]
// [duration u16]) and forgets the boost. That command now surfaces as
// `action` boost_set / boost_cancel with `boost_duration` (min) and
// `boost_temperature` (°C), exactly what upstream publishes. NOT reproduced,
// deliberately: upstream's controller-side boost POLICY (save the pre-boost
// setpoint, apply the boost temperature, restore on expiry — persisted timers)
// and its emulation of the thermostat's attribute READS of the coordinator
// (occupiedHeatingSetpoint / systemMode / ctrlSeqeOfOper / pIHeatingDemand).
// Both belong to the coordinator firmware, not a definition — follow-up ticket.
#include "definitions/_generic/_shared.hpp"
#include "definitions/schneider/_shared.hpp"

namespace zhc::devices::schneider {
namespace {

// hvacThermostat cmd 0x80 schneiderWiserThermostatBoost, device → coordinator.
// Cancel = enable 0, duration 0, temperature 0xFFF.
bool fz_cctfr6400_boost(const ::zhc::DecodedMessage& msg, const FzConverter&,
                        const PreparedDefinition&, ::zhc::RuntimeContext&,
                        ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.command_id != 0x80) return false;
    if (msg.raw_body.size() < 6) return false;
    const bool enable = msg.raw_body[1] == 1;
    const std::uint16_t temperature = static_cast<std::uint16_t>(msg.raw_body[2] | (msg.raw_body[3] << 8));
    const std::uint16_t duration    = static_cast<std::uint16_t>(msg.raw_body[4] | (msg.raw_body[5] << 8));
    ::zhc::Value a{}; a.type = ::zhc::ValueType::StringRef; a.str = enable ? "boost_set" : "boost_cancel";
    out.put("action", a);
    ::zhc::Value d{}; d.type = ::zhc::ValueType::Uint; d.u = enable ? duration : 0;
    out.put("boost_duration", d);
    if (enable) {
        ::zhc::Value t{}; t.type = ::zhc::ValueType::Float; t.f = static_cast<float>(temperature) / 100.0f;
        out.put("boost_temperature", t);
    }
    return true;
}
constexpr FzConverter kFzCctfr6400Boost{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x80,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cctfr6400_boost },
    .user_config       = nullptr,
};

const FzConverter* const kFz_CCTFR6400[] = {
    &kFzCctfr6400Boost,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzHumidity,
    &::zhc::schneider::kFzSchneiderThermostat,
};
const TzConverter* const kTz_CCTFR6400[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzThermostat,
    &::zhc::schneider::kTzSchneiderWiserZoneMode,
    &::zhc::schneider::kTzSchneiderControlType,
    &::zhc::schneider::kTzSchneiderHeatingFuel,
};
constexpr const char* kModels_CCTFR6400[] = { "Thermostat" };
constexpr const char* kActionOpts[] = { "boost_set", "boost_cancel" };

}  // namespace


// exposes + bindings (originally emit_expose_bindings.py; hand-extended with
// the humidity row/binding to match the wired kFzHumidity converter above).
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActionOpts, 2},
    {"boost_duration", ExposeType::Numeric, Access::State, "min",
     "Duration in minutes of the last boost committed on the device (0 when cancelled)", nullptr, 0},
    {"boost_temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C",
     "Target temperature of the last boost committed on the device", nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0001},
    {1, 0x0201},
    {1, 0x0405},
};

extern const PreparedDefinition kDef_CCTFR6400{
    .zigbee_models=kModels_CCTFR6400, .zigbee_models_count=sizeof(kModels_CCTFR6400)/sizeof(kModels_CCTFR6400[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CCTFR6400", .vendor="Schneider",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CCTFR6400, .from_zigbee_count=sizeof(kFz_CCTFR6400)/sizeof(kFz_CCTFR6400[0]),
    .to_zigbee=kTz_CCTFR6400, .to_zigbee_count=sizeof(kTz_CCTFR6400)/sizeof(kTz_CCTFR6400[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::schneider
