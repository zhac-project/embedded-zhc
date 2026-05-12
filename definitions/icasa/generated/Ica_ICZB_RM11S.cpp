// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Icasa ICZB-RM11S — hand-rewritten command-style remote control.
// z2m: fromZigbee=[command_recall, command_on, command_off,
//                  command_move, command_stop, battery],
// toZigbee=[]. Auto-generator emitted kFzOnOff + kTzOnOff (state path) which
// is wrong — this is a remote: it emits cluster-specific commands.
// action_group is z2m metadata only and not surfaced as a state attribute.
// z2m-source: icasa.ts #ICZB-RM11S.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::icasa {
namespace {
const FzConverter* const kFz_ICZB_RM11S[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};

constexpr Expose kExposes_ICZB_RM11S[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ICZB_RM11S[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0005},  // genScenes (recall)
    {1, 0x0001},  // genPowerCfg
};

constexpr const char* kModels_ICZB_RM11S[] = { "ICZB-RM11S" };

}  // namespace

extern const PreparedDefinition kDef_ICZB_RM11S{
    .zigbee_models=kModels_ICZB_RM11S,
    .zigbee_models_count=sizeof(kModels_ICZB_RM11S)/sizeof(kModels_ICZB_RM11S[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ICZB-RM11S", .vendor="Icasa",
    .meta=nullptr,
    .exposes=kExposes_ICZB_RM11S,
    .exposes_count=sizeof(kExposes_ICZB_RM11S)/sizeof(kExposes_ICZB_RM11S[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ICZB_RM11S,
    .from_zigbee_count=sizeof(kFz_ICZB_RM11S)/sizeof(kFz_ICZB_RM11S[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ICZB_RM11S,
    .bindings_count=sizeof(kBindings_ICZB_RM11S)/sizeof(kBindings_ICZB_RM11S[0]),
};

}  // namespace zhc::devices::icasa
