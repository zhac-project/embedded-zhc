// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 15090054 — hand-rewritten 2026-04-28s.
// ADUROLIGHT_CSC remote scene controller (battery-powered toggle + scene recall).
// z2m-source: adurosmart.ts #15090054.
//
// z2m bundle: fz.battery + fz.command_toggle + fz.command_recall
//   exposes [battery, action(toggle, recall_253, recall_254, recall_255)].
//
// Mapped:
//   kFzBattery       — genPowerCfg battery report.
//   kFzCommandToggle — genOnOff cmd 0x02 → action="toggle".
//   kFzCommandRecall — genScenes cmd 0x05 → action="recall_<sceneId>".
//
// NOTE: previous port wired kFzOnOff/kTzOnOff and exposed `state`.
// 15090054 is a battery-powered scene remote, not a sink. Removed.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D15090054[] = { "ADUROLIGHT_CSC" };

constexpr const char* kActions_D15090054[] = {
    "toggle",
    "recall_253", "recall_254", "recall_255",
};

const FzConverter* const kFz_D15090054[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandRecall,
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr,
      kActions_D15090054,
      sizeof(kActions_D15090054)/sizeof(kActions_D15090054[0]) },
    { "battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

constexpr BindingSpec kAutoBindings[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0006 },  // genOnOff (toggle)
    { 1, 0x0005 },  // genScenes (recall)
};

extern const PreparedDefinition kDef_D15090054{
    .zigbee_models=kModels_D15090054,
    .zigbee_models_count=sizeof(kModels_D15090054)/sizeof(kModels_D15090054[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="15090054", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D15090054, .from_zigbee_count=sizeof(kFz_D15090054)/sizeof(kFz_D15090054[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adurosmart
