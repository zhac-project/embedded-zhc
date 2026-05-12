// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings IM6001-BTP01 — hand-rewritten 2026-04-28.
// Button.
// z2m-source: smartthings.ts #IM6001-BTP01.
//
// z2m bundle:
//   fz: command_status_change_notification_action, battery, temperature,
//       ignore_iaszone_attreport
//   exposes: battery, temperature, action: ["off","single","double","hold"]
//
// Previous port had on/off + on/off TZ — that was wrong; the device is a
// battery button, no genOnOff cluster.
//
// Runtime gap: action enum decoding from ssIasZone
// commandStatusChangeNotification (cmd 0x00, alarm bit pattern -> label
// map). ZHC's `kFzIasZoneStatusChange` exists but emits the raw zone
// status as `alarm`/`tamper`/`battery_low` rather than mapping to the
// SmartThings button-state-machine values. Hardware testing required to
// confirm the exact zone-status <-> action mapping.
// Until the mapping is wired, only `temperature`, `battery`, and the raw
// IAS zone status will be emitted. The expose array still advertises the
// action enum for UI completeness.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_IM6001_BTP01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasZoneStatusChange,
};

constexpr const char* kModels_IM6001_BTP01[] = { "button" };

constexpr const char* const kActionValues[] = { "off", "single", "double", "hold" };

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"action",      ExposeType::Enum,    Access::State, nullptr, "Triggered button action",
        kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDef_IM6001_BTP01{
    .zigbee_models=kModels_IM6001_BTP01, .zigbee_models_count=sizeof(kModels_IM6001_BTP01)/sizeof(kModels_IM6001_BTP01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="IM6001-BTP01", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_IM6001_BTP01, .from_zigbee_count=sizeof(kFz_IM6001_BTP01)/sizeof(kFz_IM6001_BTP01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
