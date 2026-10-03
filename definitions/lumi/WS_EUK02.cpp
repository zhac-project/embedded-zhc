// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara WS-EUK02 smart wall switch H1 EU (no neutral, double rocker).
// z2m-source: devices/lumi.ts #WS-EUK02 (lumi.switch.l2aeu1), v26.105.0.
// Endpoints as z2m's map: left 1, right 2. It sat inside WS-EUK03, which gave
// it one operation_mode, for the left rocker only.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {

// fz.on_off, postfixed with the endpoint's name.
constexpr ::zhc::lumi::DeviceEndpointLabel kStates[] = {{1, "state_left"}, {2, "state_right"}};
constexpr ::zhc::lumi::DeviceEndpointLabels kStateMap{kStates, std::size(kStates)};
constexpr FzConverter kFzOnOff{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = {.zcl_fn = &::zhc::lumi::fz_lumi_on_off},
    .user_config       = &kStateMap,
};

// lumi_action_multistate: buttonLookup {41 left, 42 right, 51 both}.
constexpr ::zhc::lumi::LumiButton kButtons[] = {{41, "left"}, {42, "right"}, {51, "both"}};
constexpr ::zhc::lumi::LumiButtons kButtonMap{kButtons, std::size(kButtons)};
constexpr FzConverter kFzAction = ::zhc::lumi::lumi_switch_action_converter(&kButtonMap);

const FzConverter* const kFz[] = {
    &kFzOnOff,
    &kFzAction,                            // lumi_action_multistate
    &::zhc::lumi::kFzLumiHeartbeat,        // lumi_specific: 0x00F7
    &::zhc::lumi::kFzLumiSettings,         // lumi_specific: its settings
    &::zhc::lumi::kFzLumiPreventReset,     // lumiPreventReset
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::lumi::kTzLumiStateLeft,
    &::zhc::lumi::kTzLumiStateRight,
    &::zhc::lumi::kTzLumiOperationModeLeft,     // lumi_switch_operation_mode_opple
    &::zhc::lumi::kTzLumiOperationModeRight2,
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiFlipIndicatorLight,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiModeSwitch,
};

constexpr const char* kModels[] = {"lumi.switch.l2aeu1"};
constexpr const char* kActions[] = {"single_left", "double_left", "single_right",
                                    "double_right", "single_both", "double_both"};

constexpr Expose kExposes[] = {
    {"state_left", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_outage_memory", ExposeType::Binary, Access::StateSet, nullptr,
     "Enable/disable the power outage memory, this recovers the on/off mode after power failure",
     nullptr, 0, ExposeCategory::Config},
    {"flip_indicator_light", ExposeType::Binary, Access::StateSet, nullptr,
     "After turn on, the indicator light turns on while switch is off, and vice versa", nullptr, 0,
     ExposeCategory::Config},
    {"led_disabled_night", ExposeType::Binary, Access::StateSet, nullptr,
     "Enable/disable the LED at night", nullptr, 0, ExposeCategory::Config},
    {"power_outage_count", ExposeType::Numeric, Access::State, nullptr,
     "Number of power outages (since last pairing)", nullptr, 0, ExposeCategory::Diagnostic},
    {"device_temperature", ExposeType::Numeric, Access::State, "°C", "Temperature of the device",
     nullptr, 0, ExposeCategory::Diagnostic},
    {"operation_mode_left", ExposeType::Enum, Access::StateSet, nullptr, "Decoupled mode for left button",
     ::zhc::lumi::kLumiOperationModeValues, std::size(::zhc::lumi::kLumiOperationModeValues),
     ExposeCategory::Config},
    {"operation_mode_right", ExposeType::Enum, Access::StateSet, nullptr,
     "Decoupled mode for right button",
     ::zhc::lumi::kLumiOperationModeValues, std::size(::zhc::lumi::kLumiOperationModeValues),
     ExposeCategory::Config},
    {"mode_switch", ExposeType::Enum, Access::StateSet, nullptr,
     "Anti flicker mode can be used to solve blinking issues of some lights. Quick mode makes the "
     "device respond faster.",
     ::zhc::lumi::kLumiModeSwitchValues, std::size(::zhc::lumi::kLumiModeSwitchValues),
     ExposeCategory::Config},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions, std::size(kActions)},
};

}  // namespace

extern const PreparedDefinition kDefWS_EUK02{
    .zigbee_models = kModels, .zigbee_models_count = std::size(kModels),
    .manufacturer_name_prefix = nullptr, .manufacturer_names = nullptr, .manufacturer_names_count = 0,
    .model = "WS-EUK02", .vendor = "Aqara",
    .meta = nullptr, .exposes = kExposes, .exposes_count = std::size(kExposes),
    .white_labels = nullptr, .white_labels_count = 0,
    .from_zigbee = kFz, .from_zigbee_count = std::size(kFz),
    .to_zigbee = kTz, .to_zigbee_count = std::size(kTz),
    .configure = nullptr, .on_event = nullptr,
    // z2m binds nothing; its configure writes manuSpecificLumi mode (0x0009) = 1, "event".
    .config_steps = ::zhc::lumi::kConfigStepsLumiEventMode,
    .config_steps_count = std::size(::zhc::lumi::kConfigStepsLumiEventMode),
};

}  // namespace zhc::devices::lumi
