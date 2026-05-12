// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: UniversalElectronicsInc XHK1-UE — hand-rewritten 2026-04-29a.
// Xfinity security keypad.
// z2m-source: universal_electronics_inc.ts #XHK1-UE.
//
// z2m bundle:
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
// Mapped:
//   kFzBattery, kFzTemperature  — genPowerCfg + msTemperatureMeasurement.
//   kFzIasMotionAlarm           — alarm_1 → "occupancy".
//   kFzIasContactAlarm          — alarm_1 → "contact" (zoneStatus bits decoded twice for the
//                                 same frame; cheap, two separate keys land in the shadow).
//   kFzIasAceArm                — ssIasAce cmd 0x00 → action / action_code / action_zone.
//
// PARTIAL:
//   - `action_transaction`: z2m's command_arm_with_transaction copies the ZCL TSN
//     into action_transaction; ZHC's kFzIasAceArm doesn't expose the TSN yet.
//     Single-vendor demand at the moment, so deferred.
//   - `fz.ias_ace_occupancy_with_timeout`: ssIasAce cmd 0x02 (getPanelStatus) sets
//     `occupancy=true` for a configurable timeout. No generic decoder; runtime
//     gap. Live keypad still gets occupancy via the IAS-Zone status-change path.
//   - `fz.identify`: not yet wired; ssIasAce/Identify isn't user-actionable here.
//   - `tz.arm_mode`: ssIasAce armRsp + panelStatusChanged outbound encoders are
//     not implemented. Tracked alongside the linkind/centralite keypad gap.
//   - `m.iasGetPanelStatusResponse`: same as tz.arm_mode — needs an armRsp
//     encoder triggered on incoming getPanelStatus (cmd 0x02).
//
// NOTE: previous auto-port wired kFzOnOff/kTzOnOff and bound 0x0006. The keypad
//   has no genOnOff cluster — it's a sender + IAS source. Removed.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::universal_electronics_inc {
namespace {
const FzConverter* const kFz_XHK1_UE[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::generic::kFzIasContactAlarm,
    &::zhc::generic::kFzIasAceArm,
};

constexpr const char* kModels_XHK1_UE[] = { "URC4450BC0-X-R" };

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
    // action_transaction + tz.arm_mode write surface — pending runtime work.
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0003},  // genIdentify
    {1, 0x0500},  // ssIasZone
    {1, 0x0501},  // ssIasAce
};

}  // namespace

extern const PreparedDefinition kDef_XHK1_UE{
    .zigbee_models=kModels_XHK1_UE, .zigbee_models_count=sizeof(kModels_XHK1_UE)/sizeof(kModels_XHK1_UE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="XHK1-UE", .vendor="UniversalElectronicsInc",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_XHK1_UE, .from_zigbee_count=sizeof(kFz_XHK1_UE)/sizeof(kFz_XHK1_UE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::universal_electronics_inc
