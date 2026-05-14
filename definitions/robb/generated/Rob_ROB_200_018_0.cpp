// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Robb ROB_200-018-0 — hand-rewritten from a wrong on/off bundle.
// Zigbee knob smart dimmer (Sunricher SR-ZG2835 white-label).
// z2m-source: robb.ts #ROB_200-018-0.
//
// z2m emits actions on_1, off_1, brightness_move_to_level_1,
// color_temperature_move_1, color_move_1 (the `_1` suffix is the legacy
// multiEndpoint:true tail; z2m's own source comments mark it as a deprecated
// breaking change). We pick up on/off/move_to_level/move_to_color_temp via
// the standard command decoders. PARTIAL — color_move arrives via
// command_move_to_color which has no shared decoder yet (see PARITY).
// MoveToLevel/MoveToColorTemp emit attribute-shaped events, not raw action
// strings — consumers see `brightness=<n>` / `color_temperature=<n>`, which
// is the same wire shape z2m forwards under the `action_*` umbrella.
// No to_zigbee path — this is a battery remote, the device is the genOnOff
// / genLevelCtrl client and we never write back.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_018_0[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_ROB_200_018_0[] = { "ROB_200-018-0" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0008},
};


constexpr WhiteLabel kWhiteLabels_ROB_200_018_0[] = {
    {"Sunricher","SR-ZG2835"},
};
extern const PreparedDefinition kDef_ROB_200_018_0{
    .zigbee_models=kModels_ROB_200_018_0, .zigbee_models_count=sizeof(kModels_ROB_200_018_0)/sizeof(kModels_ROB_200_018_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-018-0", .vendor="Robb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_ROB_200_018_0, .white_labels_count=sizeof(kWhiteLabels_ROB_200_018_0)/sizeof(kWhiteLabels_ROB_200_018_0[0]),
    .from_zigbee=kFz_ROB_200_018_0, .from_zigbee_count=sizeof(kFz_ROB_200_018_0)/sizeof(kFz_ROB_200_018_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::robb
