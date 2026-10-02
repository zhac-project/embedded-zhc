// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SONOFF MINI-ZBD (ZBMINIR2) relay, graduated from the generated on/off
// copy. z2m v26.115.1 (zbminiR2ExternalSwitchActions): the external switch's
// `toggle` command plus, from firmware 1.1.0, double_click / long_press
// through eWeLink 0xFC11 attribute 0x0028 (detachRelayActionEvent). The
// firmware gate needs no port: older firmware never sends the attribute.
// Binds genOnOff and 0xFC11, onOff reported 1 s .. 30 min.
// z2m-source: sonoff.ts #MINI-ZBD.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr ::zhc::generic::ZclWriteLookup kRelayActions[] = { {"double_click", 2}, {"long_press", 3} };
constexpr ::zhc::generic::ZclAttrRow kActionRows[] = { { 0x0028, "action", 1, kRelayActions, 2 } };
constexpr ::zhc::generic::ZclAttrMap kActionMap{ kActionRows, 1 };
constexpr FzConverter kFzRelayAction = ::zhc::generic::zcl_attr_fz("manuSpecificWoolley", &kActionMap);   // 0xFC11
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandToggle,
    &kFzRelayAction,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kOpts1[] = { "toggle", "double_click", "long_press" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, "Triggered action (e.g. a button click)", kOpts1, 3},
};
constexpr const char* kM[] = { "ZBMINIR2", "MINI-ZBD" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
    {1, 0xFC11},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0006, 0x0000, 0x10, 1, 1800, 0, 0},
};
}  // namespace

extern const PreparedDefinition kDefSonoff_MINI_ZBD{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MINI-ZBD", .vendor="SONOFF",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::sonoff
