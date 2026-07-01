// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Pushok POK019 battery powered window handle (z2m v26.76.0 parity).
// z2m-source: pushok.ts #POK019.
//
// New device (z2m v26.41.0). Extends the POK016 window-opener shape:
//   m.windowCovering({controls:["lift"]}) -> cover state+position on
//     closuresWindowCovering (0x0102): kFzCoverPosition +
//     kTzCoverState/kTzCoverPositionLift (mirrors POK016).
//   m.battery({percentage,voltage})       -> kFzBattery (genPowerCfg 0x0001).
//   m.iasZoneAlarm({zoneType:"contact", zoneAttributes:["alarm_1"]}) ->
//     ssIasZone (0x0500) alarm_1 -> `contact` via generic kFzIasContactAlarm1
//     ("closed = true, opened = false" per upstream).
// Two vendor enums ride genMultistateOutput/genMultistateInput presentValue
// with no generic decoder (same stance as POK016 force_level/status):
//   window_opening_mode  (access ALL   -> StateSet, Config) exposed only.
//   status               (access STATE -> State)           exposed only.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK019[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm1,
};
const TzConverter* const kTz_POK019[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPositionLift,
};
constexpr const char* kModels_POK019[] = { "POK019" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"contact",  ExposeType::Binary,  Access::State, nullptr,
        "Indicates whether the window is closed (true) or opened (false)",
        nullptr, 0},
    {"window_opening_mode", ExposeType::Enum, Access::StateSet, nullptr,
        "Window opening mode depending on supported handle positions and installation orientation",
        nullptr, 0, ExposeCategory::Config},
    {"status",  ExposeType::Enum, Access::State, nullptr,
        "Actual window status", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},  // closuresWindowCovering
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone (contact)
};

extern const PreparedDefinition kDef_POK019{
    .zigbee_models=kModels_POK019, .zigbee_models_count=sizeof(kModels_POK019)/sizeof(kModels_POK019[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK019", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK019, .from_zigbee_count=sizeof(kFz_POK019)/sizeof(kFz_POK019[0]),
    .to_zigbee=kTz_POK019, .to_zigbee_count=sizeof(kTz_POK019)/sizeof(kTz_POK019[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
