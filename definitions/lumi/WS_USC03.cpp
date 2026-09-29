// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara WS-USC03 smart wall switch (with neutral, single rocker), US.
// z2m-source: devices/lumi.ts #WS-USC03 (lumi.switch.b1naus01), v26.105.0.
// It sat inside QBKG21LM, which gave it that switch's genBasic operation_mode.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {

// lumi_action_multistate: no button lookup for this model, so the action alone.
constexpr FzConverter kFzAction = ::zhc::lumi::lumi_switch_action_converter(nullptr);

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,             // fz.on_off
    &::zhc::lumi::kFzLumiPowerAnalog,      // lumi_power
    &kFzAction,                            // lumi_action_multistate
    &::zhc::lumi::kFzLumiHeartbeat,        // lumi_specific: 0x00F7
    &::zhc::lumi::kFzLumiSettings,         // lumi_specific: its settings
    &::zhc::lumi::kFzLumiPreventReset,     // lumiPreventReset
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::lumi::kTzLumiOperationMode,    // lumi_switch_operation_mode_opple
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiFlipIndicatorLight,
};

constexpr const char* kModels[] = {"lumi.switch.b1naus01"};
constexpr const char* kActions[] = {"single", "double"};

constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActions, std::size(kActions)},
    {"flip_indicator_light", ExposeType::Binary, Access::StateSet, nullptr,
     "After turn on, the indicator light turns on while switch is off, and vice versa", nullptr, 0,
     ExposeCategory::Config},
    {"power_outage_count", ExposeType::Numeric, Access::State, nullptr,
     "Number of power outages (since last pairing)", nullptr, 0, ExposeCategory::Diagnostic},
    {"device_temperature", ExposeType::Numeric, Access::State, "C", "Temperature of the device",
     nullptr, 0, ExposeCategory::Diagnostic},
    {"power", ExposeType::Numeric, Access::State, "W", "Instantaneous measured power", nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", "Sum of consumed energy", nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", "Measured electrical potential value", nullptr, 0},
    {"power_outage_memory", ExposeType::Binary, Access::StateSet, nullptr,
     "Enable/disable the power outage memory, this recovers the on/off mode after power failure",
     nullptr, 0, ExposeCategory::Config},
    {"operation_mode", ExposeType::Enum, Access::StateSet, nullptr, "Decoupled mode",
     ::zhc::lumi::kLumiOperationModeValues, std::size(::zhc::lumi::kLumiOperationModeValues),
     ExposeCategory::Config},
};

}  // namespace

extern const PreparedDefinition kDefWS_USC03{
    .zigbee_models = kModels, .zigbee_models_count = std::size(kModels),
    .manufacturer_name_prefix = nullptr, .manufacturer_names = nullptr, .manufacturer_names_count = 0,
    .model = "WS-USC03", .vendor = "Aqara",
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
