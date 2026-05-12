// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Pushok POK016 — battery powered window opener.
// z2m-source: pushok.ts #POK016. Uses m.onOff (NOT a cover); previous
// auto-emit incorrectly treated this as a coverPosition device.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK016[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_POK016[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_POK016[] = { "POK016" };

}  // namespace


// --- hand-authored exposes block (z2m: onOff, battery, force_level enum,
//     status enum). force_level/status are vendor enums on
//     genMultistateOutput / genMultistateInput, exposed for UI. ---
constexpr Expose kAutoExposes[] = {
    {"state",   ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"force_level", ExposeType::Enum, Access::StateSet, nullptr,
        "Force level at which the window stops", nullptr, 0,
        ExposeCategory::Config},
    {"status",  ExposeType::Enum, Access::State, nullptr,
        "Actual window status", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0001},  // genPowerCfg
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_POK016{
    .zigbee_models=kModels_POK016, .zigbee_models_count=sizeof(kModels_POK016)/sizeof(kModels_POK016[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK016", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK016, .from_zigbee_count=sizeof(kFz_POK016)/sizeof(kFz_POK016[0]),
    .to_zigbee=kTz_POK016, .to_zigbee_count=sizeof(kTz_POK016)/sizeof(kTz_POK016[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
