// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Third Reality 3RDP01072Z dual smart plug (+ 3RWP01073Z wall plug),
// graduated from the generated copy. z2m v26.115.1 adds metering_only_mode
// (relay forced ON); ported with the rest of the 3rDualPlugSpecialcluster
// (0xFF03, manufacturer code 0x1407) surface the generated copy lacked:
// reset_total_energy, countdown_time_on_to_off / off_to_on, per endpoint, and
// red_led_brightness (genBasic 0xFF01).
// z2m-source: third_reality.ts #3RDP01072Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::third_reality {
namespace {
constexpr std::uint16_t kThirdManu = 0x1407;
constexpr ::zhc::generic::ZclWriteLookup kReset[] = { {"Reset", 1} };
constexpr ::zhc::generic::ZclWriteLookup kOnOffWords[] = { {"ON", 1}, {"OFF", 0} };
constexpr ::zhc::generic::ZclAttrRow kPlugRows[] = {
    { 0x0000, "reset_total_energy", 1, kReset, 1 },
    { 0x0050, "metering_only_mode", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
    { 0x0001, "countdown_time_on_to_off" },
    { 0x0002, "countdown_time_off_to_on" },
};
constexpr ::zhc::generic::ZclAttrMap kPlugMap{ kPlugRows, 4, kThirdManu };
constexpr FzConverter kFzPlug = ::zhc::generic::zcl_attr_fz("3rDualPlugSpecialcluster", &kPlugMap);
constexpr ::zhc::generic::ZclAttrRow kLedRows[] = { { 0xFF01, "red_led_brightness" } };
constexpr ::zhc::generic::ZclAttrMap kLedMap{ kLedRows, 1 };
constexpr FzConverter kFzLed = ::zhc::generic::zcl_attr_fz("genBasic", &kLedMap);
constexpr ::zhc::generic::ZclWriteSpec kResetSpec{ "reset_total_energy", 0x0000, 0x20, kThirdManu, kReset, 1 };
constexpr ::zhc::generic::ZclWriteSpec kMeteringOnlySpec{ "metering_only_mode", 0x0050, 0x20, kThirdManu, kOnOffWords, 2 };
constexpr ::zhc::generic::ZclWriteSpec kOffSpec{ "countdown_time_on_to_off", 0x0001, 0x21, kThirdManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kOnSpec{ "countdown_time_off_to_on", 0x0002, 0x21, kThirdManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kLedSpec{ "red_led_brightness", 0xFF01, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzReset = ::zhc::generic::zcl_write_tz("3rDualPlugSpecialcluster", 0xFF03, &kResetSpec);
constexpr TzConverter kTzMeteringOnly = ::zhc::generic::zcl_write_tz("3rDualPlugSpecialcluster", 0xFF03, &kMeteringOnlySpec);
constexpr TzConverter kTzCountdownOff = ::zhc::generic::zcl_write_tz("3rDualPlugSpecialcluster", 0xFF03, &kOffSpec);
constexpr TzConverter kTzCountdownOn = ::zhc::generic::zcl_write_tz("3rDualPlugSpecialcluster", 0xFF03, &kOnSpec);
constexpr TzConverter kTzLed = ::zhc::generic::zcl_write_tz("genBasic", 0x0000, &kLedSpec);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &kFzPlug,
    &kFzLed,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &kTzReset,
    &kTzMeteringOnly,
    &kTzCountdownOff,
    &kTzCountdownOn,
    &kTzLed,
};
constexpr const char* kOpts7[] = { "Reset" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"power_factor", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"ac_frequency", ExposeType::Numeric, Access::State, "Hz", nullptr, nullptr, 0},
    {"reset_total_energy", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts7, 1},
    {"metering_only_mode", ExposeType::Binary, Access::StateSet, nullptr, "When enabled, the device enters metering-only mode and the relay is forced to stay ON", nullptr, 0},
    {"countdown_time_on_to_off", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 60000, 1},
    {"countdown_time_off_to_on", ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0, ExposeCategory::State, 0, 60000, 1},
    {"red_led_brightness", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 1},
};
constexpr const char* kM[] = { "3RDP01072Z", "3RWP01073Z" };
constexpr WhiteLabel kWL[] = { {"Third Reality", "3RWP01073Z"} };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
    {2, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
constexpr EndpointLabel kEp[] = { {"1", 1}, {"2", 2} };
}  // namespace

extern const PreparedDefinition kDefThirdReality_3RDP01072Z{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3RDP01072Z", .vendor="Third Reality",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL, .white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .endpoint_map=kEp, .endpoint_map_count=sizeof(kEp)/sizeof(kEp[0]),
};

}  // namespace zhc::devices::third_reality
