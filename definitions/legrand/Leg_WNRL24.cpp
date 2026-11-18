// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand WNRL24, wired remote smart switch with Netatmo, new in z2m
// v26.126.0..v26.133.0. A mains remote with no relay: it only sends genOnOff /
// genLevelCtrl commands. fz.command_on / command_off / command_toggle /
// command_move / command_stop -> action on / off / toggle /
// brightness_move_up|down / brightness_stop; endpoint 1 genIdentify, genOnOff
// and genLevelCtrl bound, as upstream.
// zigbeeModel: upstream lists the leading-space form the firmware reports and
// the trimmed form; both are carried.
// Not ported: fz.identify (the `identify` action value).
// z2m-source: legrand.ts #WNRL24.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::legrand {
namespace {

const FzConverter* const kFz_WNRL24[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStopWithOnOff,
};

constexpr const char* kModels_WNRL24[] = { " Wired remote on / off switch", "Wired remote on / off switch" };

constexpr const char* kActionValues[] = {
    "on", "off", "toggle", "brightness_move_up", "brightness_move_down", "brightness_stop" };

constexpr Expose kExposes_WNRL24[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
};

constexpr BindingSpec kBindings_WNRL24[] = {
    {1, 0x0003},
    {1, 0x0006},
    {1, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_WNRL24{
    .zigbee_models=kModels_WNRL24, .zigbee_models_count=sizeof(kModels_WNRL24)/sizeof(kModels_WNRL24[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WNRL24", .vendor="Legrand",
    .meta=nullptr, .exposes=kExposes_WNRL24, .exposes_count=sizeof(kExposes_WNRL24)/sizeof(kExposes_WNRL24[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WNRL24, .from_zigbee_count=sizeof(kFz_WNRL24)/sizeof(kFz_WNRL24[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_WNRL24, .bindings_count=sizeof(kBindings_WNRL24)/sizeof(kBindings_WNRL24[0]),
};

}  // namespace zhc::devices::legrand
