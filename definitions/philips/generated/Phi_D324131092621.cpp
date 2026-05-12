// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Philips Hue dimmer gen 1 (324131092621 / RWL020 / RWL021).
//
// z2m-source: philips.ts #324131092621 — 4-button dimmer sending
// `hueNotification` on manuSpecificPhilips (0xFC00). genOnOff /
// genLevelCtrl commands from this device are ignored by z2m
// (fz.ignore_command_*); only the manu-specific frames carry the
// button/type payload.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D324131092621[] = { "RWL020", "RWL021" };

constexpr const char* kActions_D324131092621[] = {
    "on_press",   "on_hold",   "on_press_release",   "on_hold_release",
    "up_press",   "up_hold",   "up_press_release",   "up_hold_release",
    "down_press", "down_hold", "down_press_release", "down_hold_release",
    "off_press",  "off_hold",  "off_press_release",  "off_hold_release",
};

constexpr Expose kExposes_D324131092621[] = {
    { "action",          ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_D324131092621, sizeof(kActions_D324131092621)/sizeof(kActions_D324131092621[0]) },
    { "battery",         ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",         ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_D324131092621[] = {
    &::zhc::generic::kFzHueDimmerNotification,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_D324131092621[] = {
    { 1, 0x0006 },   // genOnOff (suppressed — z2m binds but ignores)
    { 1, 0x0008 },   // genLevelCtrl (suppressed likewise)
    { 2, 0x0001 },   // genPowerCfg (battery)
    { 2, 0xFC00 },   // manuSpecificPhilips (button events)
};

}  // namespace

extern const PreparedDefinition kDef_D324131092621{
    .zigbee_models            = kModels_D324131092621,
    .zigbee_models_count      = sizeof(kModels_D324131092621)/sizeof(kModels_D324131092621[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "324131092621",
    .vendor                   = "Philips",
    .meta                     = nullptr,
    .exposes                  = kExposes_D324131092621,
    .exposes_count            = sizeof(kExposes_D324131092621)/sizeof(kExposes_D324131092621[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_D324131092621,
    .from_zigbee_count        = sizeof(kFz_D324131092621)/sizeof(kFz_D324131092621[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_D324131092621,
    .bindings_count           = sizeof(kBindings_D324131092621)/sizeof(kBindings_D324131092621[0]),
};

}  // namespace zhc::devices::philips
