// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Three shared action-remote bundles for RGB Genie ports — modelled on
// iluminize/_shared. See _shared.hpp header for matrix.

#include "definitions/rgb_genie/_shared.hpp"

namespace zhc::devices::rgb_genie {

// ── ActionBattery ────────────────────────────────────────────────────
const ::zhc::FzConverter* const kFzActionBattery[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
static_assert(kFzActionBatteryCount == std::size(kFzActionBattery));

const ::zhc::BindingSpec kBindingsActionBattery[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0001 },  // genPowerCfg
};
static_assert(kBindingsActionBatteryCount == std::size(kBindingsActionBattery));

// ── ActionBatteryDim ─────────────────────────────────────────────────
const ::zhc::FzConverter* const kFzActionBatteryDim[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzBattery,
};
static_assert(kFzActionBatteryDimCount == std::size(kFzActionBatteryDim));

const ::zhc::BindingSpec kBindingsActionBatteryDim[] = {
    { 1, 0x0006 },
    { 1, 0x0008 },
    { 1, 0x0001 },
};
static_assert(kBindingsActionBatteryDimCount == std::size(kBindingsActionBatteryDim));

// ── ActionBatteryRGB (battery + color/CT actions) ───────────────────
const ::zhc::FzConverter* const kFzActionBatteryRGB[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandMoveHue,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzCommandMoveColorTemperature,
    &::zhc::generic::kFzBattery,
};
static_assert(kFzActionBatteryRGBCount == std::size(kFzActionBatteryRGB));

// ── ActionRGB (mains-powered, no battery cluster) ───────────────────
const ::zhc::FzConverter* const kFzActionRGBNoBattery[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandMoveHue,
    &::zhc::generic::kFzCommandMoveToColor,
    &::zhc::generic::kFzCommandMoveColorTemperature,
};
static_assert(kFzActionRGBNoBatteryCount == std::size(kFzActionRGBNoBattery));

const ::zhc::BindingSpec kBindingsActionBatteryRGB[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0300 },  // lightingColorCtrl
    { 1, 0x0001 },  // genPowerCfg
};
static_assert(kBindingsActionBatteryRGBCount == std::size(kBindingsActionBatteryRGB));

const ::zhc::BindingSpec kBindingsActionRGBNoBattery[] = {
    { 1, 0x0006 },
    { 1, 0x0008 },
    { 1, 0x0300 },
};
static_assert(kBindingsActionRGBNoBatteryCount == std::size(kBindingsActionRGBNoBattery));

}  // namespace zhc::devices::rgb_genie
