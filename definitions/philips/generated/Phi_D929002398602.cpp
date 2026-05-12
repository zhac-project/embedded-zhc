// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Philips Hue dimmer gen 2 (929002398602 / RWL022).
//
// z2m-source: philips.ts #929002398602 — 4-button dimmer sending
// `hueNotification` on manuSpecificPhilips plus genScenes `recall`
// (cmd 0x05) for `recall_0` / `recall_1` scene-select. Default
// genOnOff / genLevelCtrl commands are ignored (`fz.ignore_command_*`).

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929002398602[] = { "RWL022" };

constexpr const char* kActions_D929002398602[] = {
    "on_press",   "on_hold",   "on_press_release",   "on_hold_release",
    "up_press",   "up_hold",   "up_press_release",   "up_hold_release",
    "down_press", "down_hold", "down_press_release", "down_hold_release",
    "off_press",  "off_hold",  "off_press_release",  "off_hold_release",
    "recall_0",   "recall_1",
};

constexpr Expose kExposes_D929002398602[] = {
    { "action",  ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_D929002398602, sizeof(kActions_D929002398602)/sizeof(kActions_D929002398602[0]) },
    { "battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_D929002398602[] = {
    &::zhc::generic::kFzHueDimmerNotification,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_D929002398602[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff (ignored)
    { 1, 0x0008 },   // genLevelCtrl (ignored)
    { 1, 0xFC00 },   // manuSpecificPhilips (button events)
};

}  // namespace

extern const PreparedDefinition kDef_D929002398602{
    .zigbee_models            = kModels_D929002398602,
    .zigbee_models_count      = sizeof(kModels_D929002398602)/sizeof(kModels_D929002398602[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "929002398602",
    .vendor                   = "Philips",
    .meta                     = nullptr,
    .exposes                  = kExposes_D929002398602,
    .exposes_count            = sizeof(kExposes_D929002398602)/sizeof(kExposes_D929002398602[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_D929002398602,
    .from_zigbee_count        = sizeof(kFz_D929002398602)/sizeof(kFz_D929002398602[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_D929002398602,
    .bindings_count           = sizeof(kBindings_D929002398602)/sizeof(kBindings_D929002398602[0]),
};

}  // namespace zhc::devices::philips
