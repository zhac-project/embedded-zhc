// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Aqara C200 curtain motor (z2m v26.76.0 parity).
// z2m-source: lumi.ts #C200
//
// z2m wires m.windowCovering({controls:["lift"], coverInverted:true}) for cover
// state/position on closuresWindowCovering (0x0102) plus lumiCurtainPosition(),
// which reads the live position from genAnalogOutput attr 0x0055. Ported here as:
// position readout via kFzLumiCurtainPosition (0x0055) + cover open/close/stop and
// go-to-lift-% control via the generic cover tz on 0x0102.
//
// DEFERRED: the ~12 Aqara 0xFCC0 curtain-config extends (speed, manual
// open/close, reverse direction, traverse-time, calibration-status, calibrated,
// identify-beep, limits/automatic calibration). coverInverted is not modelled.
#include "definitions/lumi/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::lumi::kFzLumiCurtainPosition,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzCoverState,
    &::zhc::generic::kTzCoverPositionLift,
};
constexpr const char* kModels[] = { "lumi.curtain.acn018" };
}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},  // closuresWindowCovering (cover state + go-to-position)
    {1, 0x000C},  // genAnalogOutput (live position, attr 0x0055)
};

extern const PreparedDefinition kDefC200{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .model="C200",.vendor="Xiaomi",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::lumi
