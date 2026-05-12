// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm mTouch_Bryter — auto-generated.
// mTouch Bryter OP, 3 channel switch
// z2m-source: ctm.ts #mTouch_Bryter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
// mTouch Bryter is a battery-powered 3-channel remote — no relay, so
// no `fz.on_off` / `tz.on_off`. z2m wires command_{recall,on,off,
// toggle,move,stop} on genOnOff/genLevelCtrl + ctm_group_config (BLOCKED:
// ctmGroupConfig 0xFEA7 manuSpec). We surface action via the generic
// command-* converters; group_id needs the manuSpec cluster decoder.
const FzConverter* const kFz_mTouch_Bryter[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
};
constexpr const char* kModels_mTouch_Bryter[] = { "mTouch Bryter" };

}  // namespace


// Manual: drop spurious `state` (no relay) + add temperature + action.
// `group_id` BLOCKED on ctmGroupConfig 0xFEA7 manuSpec read decoder.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0402},
};

extern const PreparedDefinition kDef_mTouch_Bryter{
    .zigbee_models=kModels_mTouch_Bryter, .zigbee_models_count=sizeof(kModels_mTouch_Bryter)/sizeof(kModels_mTouch_Bryter[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mTouch_Bryter", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mTouch_Bryter, .from_zigbee_count=sizeof(kFz_mTouch_Bryter)/sizeof(kFz_mTouch_Bryter[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
