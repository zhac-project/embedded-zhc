// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nue3a HGZB-045 — hand-rewritten 2026-04-28.
// Smart 4 key scene wall switch.
// z2m-source: nue_3a.ts #HGZB-045.
//
// Same bundle as HGZB-1S: tz.on_off + fz.command_recall +
// fz.ignore_power_report; exposes [action(["recall_*"]), switch()].
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nue_3a {
namespace {
const FzConverter* const kFz_HGZB_045[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandRecall,
};
const TzConverter* const kTz_HGZB_045[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_HGZB_045[] = { "FB56+ZSN08KJ2.3", "FEB56-ZSN26YS1.3" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum,   Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0005},  // genScenes
    {1, 0x0006},  // genOnOff
};

extern const PreparedDefinition kDef_HGZB_045{
    .zigbee_models=kModels_HGZB_045, .zigbee_models_count=sizeof(kModels_HGZB_045)/sizeof(kModels_HGZB_045[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HGZB-045", .vendor="Nue3a",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HGZB_045, .from_zigbee_count=sizeof(kFz_HGZB_045)/sizeof(kFz_HGZB_045[0]),
    .to_zigbee=kTz_HGZB_045, .to_zigbee_count=sizeof(kTz_HGZB_045)/sizeof(kTz_HGZB_045[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nue_3a
