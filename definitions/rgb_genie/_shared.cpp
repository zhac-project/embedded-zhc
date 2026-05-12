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
const std::uint8_t kFzActionBatteryCount =
    static_cast<std::uint8_t>(sizeof(kFzActionBattery) / sizeof(kFzActionBattery[0]));

const ::zhc::BindingSpec kBindingsActionBattery[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0001 },  // genPowerCfg
};
const std::uint8_t kBindingsActionBatteryCount =
    static_cast<std::uint8_t>(sizeof(kBindingsActionBattery) / sizeof(kBindingsActionBattery[0]));

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
const std::uint8_t kFzActionBatteryDimCount =
    static_cast<std::uint8_t>(sizeof(kFzActionBatteryDim) / sizeof(kFzActionBatteryDim[0]));

const ::zhc::BindingSpec kBindingsActionBatteryDim[] = {
    { 1, 0x0006 },
    { 1, 0x0008 },
    { 1, 0x0001 },
};
const std::uint8_t kBindingsActionBatteryDimCount =
    static_cast<std::uint8_t>(sizeof(kBindingsActionBatteryDim) / sizeof(kBindingsActionBatteryDim[0]));

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
const std::uint8_t kFzActionBatteryRGBCount =
    static_cast<std::uint8_t>(sizeof(kFzActionBatteryRGB) / sizeof(kFzActionBatteryRGB[0]));

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
const std::uint8_t kFzActionRGBNoBatteryCount =
    static_cast<std::uint8_t>(sizeof(kFzActionRGBNoBattery) / sizeof(kFzActionRGBNoBattery[0]));

const ::zhc::BindingSpec kBindingsActionBatteryRGB[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0300 },  // lightingColorCtrl
    { 1, 0x0001 },  // genPowerCfg
};
const std::uint8_t kBindingsActionBatteryRGBCount =
    static_cast<std::uint8_t>(sizeof(kBindingsActionBatteryRGB) / sizeof(kBindingsActionBatteryRGB[0]));

const ::zhc::BindingSpec kBindingsActionRGBNoBattery[] = {
    { 1, 0x0006 },
    { 1, 0x0008 },
    { 1, 0x0300 },
};
const std::uint8_t kBindingsActionRGBNoBatteryCount =
    static_cast<std::uint8_t>(sizeof(kBindingsActionRGBNoBattery) / sizeof(kBindingsActionRGBNoBattery[0]));

}  // namespace zhc::devices::rgb_genie
