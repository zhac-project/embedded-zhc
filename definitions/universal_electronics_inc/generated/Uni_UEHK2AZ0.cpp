// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: UniversalElectronicsInc UEHK2AZ0 — hand-rewritten 2026-04-29a.
// Xfinity security keypad (re-branded XHK1-UE successor with the same wiring).
// z2m-source: universal_electronics_inc.ts #UEHK2AZ0.
//
// Identical z2m bundle to XHK1-UE:
//   fz.command_arm_with_transaction       — ssIasAce arm command → "action" + action_code + action_zone (+ action_transaction)
//   fz.temperature                         — msTemperatureMeasurement
//   fz.battery                             — genPowerCfg
//   fz.ias_occupancy_alarm_1               — ssIasZone status_change → occupancy + tamper + battery_low
//   fz.identify                            — genIdentify
//   fz.ias_contact_alarm_1                 — ssIasZone status_change → contact + tamper + battery_low (same frame)
//   fz.ias_ace_occupancy_with_timeout      — ssIasAce getPanelStatus → derived "occupancy"
//   tz.arm_mode                            — outbound ssIasAce armRsp / panelStatusChanged
//   m.iasGetPanelStatusResponse            — fixed armRsp on getPanelStatus
//
// Mapped: kFzBattery, kFzTemperature, kFzIasMotionAlarm, kFzIasContactAlarm, kFzIasAceArm.
//
// PARTIAL — same gaps as XHK1-UE:
//   - action_transaction (TSN passthrough)
//   - fz.ias_ace_occupancy_with_timeout (ssIasAce cmd 0x02)
//   - fz.identify
//   - tz.arm_mode + m.iasGetPanelStatusResponse outbound encoders
//
// NOTE: previous auto-port wired kFzOnOff/kTzOnOff and bound 0x0006. Removed
//   — keypad has no genOnOff cluster.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::universal_electronics_inc {
namespace {
const FzConverter* const kFz_UEHK2AZ0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::generic::kFzIasContactAlarm,
    &::zhc::generic::kFzIasAceArm,
};

constexpr const char* kModels_UEHK2AZ0[] = { "H34450BA00-00007" };

constexpr Expose kAutoExposes[] = {
    {"battery",            ExposeType::Numeric, Access::State, "%",          nullptr, nullptr, 0},
    {"voltage",            ExposeType::Numeric, Access::State, "mV",         nullptr, nullptr, 0},
    {"temperature",        ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"occupancy",          ExposeType::Binary,  Access::State, nullptr,      nullptr, nullptr, 0},
    {"contact",            ExposeType::Binary,  Access::State, nullptr,      nullptr, nullptr, 0},
    {"tamper",             ExposeType::Binary,  Access::State, nullptr,      nullptr, nullptr, 0},
    {"battery_low",        ExposeType::Binary,  Access::State, nullptr,      nullptr, nullptr, 0},
    {"action",             ExposeType::Enum,    Access::State, nullptr,      nullptr, nullptr, 0},
    {"action_code",        ExposeType::Numeric, Access::State, nullptr,      nullptr, nullptr, 0},
    {"action_zone",        ExposeType::Numeric, Access::State, nullptr,      nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0003},  // genIdentify
    {1, 0x0500},  // ssIasZone
    {1, 0x0501},  // ssIasAce
};

}  // namespace

extern const PreparedDefinition kDef_UEHK2AZ0{
    .zigbee_models=kModels_UEHK2AZ0, .zigbee_models_count=sizeof(kModels_UEHK2AZ0)/sizeof(kModels_UEHK2AZ0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="UEHK2AZ0", .vendor="UniversalElectronicsInc",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_UEHK2AZ0, .from_zigbee_count=sizeof(kFz_UEHK2AZ0)/sizeof(kFz_UEHK2AZ0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::universal_electronics_inc
