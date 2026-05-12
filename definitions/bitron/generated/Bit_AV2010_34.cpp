// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bitron AV2010/34 — hand-rewritten 2026-04-28.
// Wall switch with 4 buttons.
// z2m-source: bitron.ts #AV2010/34.
//
// z2m bundle: fz.command_recall
//   exposes [action(recall_*)].
//
// Mapped:
//   kFzCommandRecall — genScenes cmd 0x05 → action="recall_<n>".
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`. AV2010/34
//   is a sender (button switch), not a sink — removed. z2m configure() binds
//   genOnOff and genScenes; we keep both bindings so the device is paired
//   correctly with the coordinator endpoint.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bitron {
namespace {
constexpr const char* kModels_AV2010_34[] = { "AV2010/34", "902010/34" };

constexpr const char* kActions_AV2010_34[] = {
    "recall_0",  "recall_1",  "recall_2",  "recall_3",
    "recall_4",  "recall_5",  "recall_6",  "recall_7",
    "recall_8",  "recall_9",  "recall_10", "recall_11",
    "recall_12", "recall_13", "recall_14", "recall_15",
};

const FzConverter* const kFz_AV2010_34[] = {
    &::zhc::generic::kFzCommandRecall,
};

}  // namespace


constexpr Expose kAutoExposes[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_AV2010_34,
      sizeof(kActions_AV2010_34)/sizeof(kActions_AV2010_34[0]) },
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff (per z2m configure)
    {1, 0x0005},  // genScenes (per z2m configure)
};

extern const PreparedDefinition kDef_AV2010_34{
    .zigbee_models=kModels_AV2010_34, .zigbee_models_count=sizeof(kModels_AV2010_34)/sizeof(kModels_AV2010_34[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AV2010/34", .vendor="Bitron",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AV2010_34, .from_zigbee_count=sizeof(kFz_AV2010_34)/sizeof(kFz_AV2010_34[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bitron
