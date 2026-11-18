// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SONOFF MINI-ZB2GS dual-channel relay (l1 = EP1, l2 = EP2), hand
// edit of the generated on/off copy. z2m v26.133.0
// sonoffExtend.externalSwitchActions({endpointNames: [l1, l2], gate fw >= 1.1.2}):
// the external switch's genOnOff `toggle` command plus eWeLink 0xFC11
// attribute 0x0028 (detachRelayActionEvent) 2 double_click / 3 long_press /
// 4 switch_on / 5 switch_off. z2m puts the endpoint in the value
// (`toggle_l1`); the house puts it in the key (`action_l1` = toggle) via
// endpoint_action_suffix. The firmware gate needs no port: older firmware
// never sends the attribute. Configure binds genOnOff + 0xFC11 and reports
// onOff 1 s .. 30 min on BOTH endpoints (the old copy bound EP1 only).
// Not ported: external trigger mode, detach relay, inching, network_indicator,
// turbo_mode, delayed power-on state/time, stepper sequence (0xFC11 config),
// power_on_behavior, the configure-time attribute reads.
// z2m-source: sonoff.ts #MINI-ZB2GS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr ::zhc::generic::ZclWriteLookup kRelayActions_MINI_ZB2GS[] = {
    {"double_click", 2}, {"long_press", 3}, {"switch_on", 4}, {"switch_off", 5},
};
constexpr ::zhc::generic::ZclAttrRow kActionRows_MINI_ZB2GS[] = {
    { 0x0028, "action", 1, kRelayActions_MINI_ZB2GS,
      sizeof(kRelayActions_MINI_ZB2GS)/sizeof(kRelayActions_MINI_ZB2GS[0]) },
};
constexpr ::zhc::generic::ZclAttrMap kActionMap_MINI_ZB2GS{
    kActionRows_MINI_ZB2GS, sizeof(kActionRows_MINI_ZB2GS)/sizeof(kActionRows_MINI_ZB2GS[0]) };
constexpr FzConverter kFzRelayAction_MINI_ZB2GS =
    ::zhc::generic::zcl_attr_fz("manuSpecificWoolley", &kActionMap_MINI_ZB2GS);   // 0xFC11

const FzConverter* const kFz_MINI_ZB2GS[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandToggle,
    &kFzRelayAction_MINI_ZB2GS,
};
const TzConverter* const kTz_MINI_ZB2GS[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_MINI_ZB2GS[] = { "MINI-ZB2GS" };

constexpr ::zhc::EndpointLabel kEndpoints_MINI_ZB2GS[] = { {"l1", 1}, {"l2", 2} };

constexpr const char* kActionOpts_MINI_ZB2GS[] = {
    "toggle", "double_click", "long_press", "switch_on", "switch_off",
};
constexpr Expose kExposes_MINI_ZB2GS[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, "Triggered action (e.g. a button click)",
     kActionOpts_MINI_ZB2GS, sizeof(kActionOpts_MINI_ZB2GS)/sizeof(kActionOpts_MINI_ZB2GS[0])},
};

constexpr BindingSpec kBindings_MINI_ZB2GS[] = {
    {1, 0x0006},
    {1, 0xFC11},
    {2, 0x0006},
    {2, 0xFC11},
};
constexpr ReportingSpec kReports_MINI_ZB2GS[] = {
    {1, 0x0006, 0x0000, 0x10, 1, 1800, 0, 0},
    {2, 0x0006, 0x0000, 0x10, 1, 1800, 0, 0},
};

}  // namespace

extern const PreparedDefinition kDef_MINI_ZB2GS{
    .zigbee_models=kModels_MINI_ZB2GS, .zigbee_models_count=sizeof(kModels_MINI_ZB2GS)/sizeof(kModels_MINI_ZB2GS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MINI-ZB2GS", .vendor="Sonoff",
    .meta=nullptr, .exposes=kExposes_MINI_ZB2GS, .exposes_count=sizeof(kExposes_MINI_ZB2GS)/sizeof(kExposes_MINI_ZB2GS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MINI_ZB2GS, .from_zigbee_count=sizeof(kFz_MINI_ZB2GS)/sizeof(kFz_MINI_ZB2GS[0]),
    .to_zigbee=kTz_MINI_ZB2GS, .to_zigbee_count=sizeof(kTz_MINI_ZB2GS)/sizeof(kTz_MINI_ZB2GS[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_MINI_ZB2GS, .bindings_count=sizeof(kBindings_MINI_ZB2GS)/sizeof(kBindings_MINI_ZB2GS[0]),
    .reports=kReports_MINI_ZB2GS, .reports_count=sizeof(kReports_MINI_ZB2GS)/sizeof(kReports_MINI_ZB2GS[0]),
    .endpoint_map       = kEndpoints_MINI_ZB2GS,
    .endpoint_map_count = sizeof(kEndpoints_MINI_ZB2GS)/sizeof(kEndpoints_MINI_ZB2GS[0]),
    .endpoint_action_suffix = true,  // action -> action_l1 / action_l2 (z2m: toggle_l1 / toggle_l2)
};

}  // namespace zhc::devices::sonoff
