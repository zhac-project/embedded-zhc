// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Robb ROB_200-016-0 — hand-rewritten from a wrong on/off bundle.
// RGB CCT DIM 3-in-1 Zigbee remote.
// z2m-source: robb.ts #ROB_200-016-0.
//
// Actions emitted: on, off, toggle, brightness_move_up/down, brightness_stop,
// brightness_step_up/down, recall_<n>. PARTIAL — z2m additionally emits
// color_move, color_temperature_move, hue_move, color_loop_set,
// enhanced_move_to_hue_and_saturation and move_to_hue_and_saturation; the
// matching command-mode lightingColorCtrl decoders (move_to_color, move_hue,
// color_loop_set, …) are not yet present in `_generic/_shared.{hpp,cpp}`.
// MoveToColorTemp arrives via the existing kFzCommandMoveToColorTemp which
// emits the `color_temperature` attribute (legacy "state" path), not the
// command-mode `color_temperature_move` action — see PARITY for the gap.
// No to_zigbee path — this is a battery-powered remote/scene controller,
// the device is the genOnOff/genLevelCtrl client and we never write back.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::robb {
namespace {
const FzConverter* const kFz_ROB_200_016_0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMove,
};

constexpr const char* kModels_ROB_200_016_0[] = { "ROB_200-016-0" };
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

extern const PreparedDefinition kDef_ROB_200_016_0{
    .zigbee_models=kModels_ROB_200_016_0, .zigbee_models_count=sizeof(kModels_ROB_200_016_0)/sizeof(kModels_ROB_200_016_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ROB_200-016-0", .vendor="Robb",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ROB_200_016_0, .from_zigbee_count=sizeof(kFz_ROB_200_016_0)/sizeof(kFz_ROB_200_016_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::robb
