// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Six shared bundles for AduroSmart ports — one per m.light()/m.onOff()/
// m.electricityMeter() feature combination seen in z2m's adurosmart.ts.
// Modelled after ledvance/_shared.

#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {

// ── On/off only (m.onOff()) ─────────────────────────────────────────
const ::zhc::FzConverter* const kFzAduOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
const std::uint8_t kFzAduOnOffCount =
    static_cast<std::uint8_t>(sizeof(kFzAduOnOff) / sizeof(kFzAduOnOff[0]));

const ::zhc::TzConverter* const kTzAduOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzAduOnOffCount =
    static_cast<std::uint8_t>(sizeof(kTzAduOnOff) / sizeof(kTzAduOnOff[0]));

const ::zhc::Expose kExposesAduOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAduOnOffCount =
    static_cast<std::uint8_t>(sizeof(kExposesAduOnOff) / sizeof(kExposesAduOnOff[0]));

const ::zhc::BindingSpec kBindingsAduOnOff[] = {
    { 1, 0x0006 },
};
const std::uint8_t kBindingsAduOnOffCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAduOnOff) / sizeof(kBindingsAduOnOff[0]));

// ── On/off + electricity meter (m.onOff() + m.electricityMeter) ─────
const ::zhc::FzConverter* const kFzAduOnOffEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const std::uint8_t kFzAduOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kFzAduOnOffEM) / sizeof(kFzAduOnOffEM[0]));

const ::zhc::TzConverter* const kTzAduOnOffEM[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzAduOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kTzAduOnOffEM) / sizeof(kTzAduOnOffEM[0]));

const ::zhc::Expose kExposesAduOnOffEM[] = {
    { "state",   ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "energy",  ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "kWh", nullptr, nullptr, 0 },
    { "power",   ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "W",   nullptr, nullptr, 0 },
    { "voltage", ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "V",   nullptr, nullptr, 0 },
    { "current", ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "A",   nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAduOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kExposesAduOnOffEM) / sizeof(kExposesAduOnOffEM[0]));

const ::zhc::BindingSpec kBindingsAduOnOffEM[] = {
    { 1, 0x0006 }, { 1, 0x0702 }, { 1, 0x0B04 },
};
const std::uint8_t kBindingsAduOnOffEMCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAduOnOffEM) / sizeof(kBindingsAduOnOffEM[0]));

// ── Plain dimmable (m.light()) ──────────────────────────────────────
const ::zhc::FzConverter* const kFzAduLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzAduLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAduLight) / sizeof(kFzAduLight[0]));

const ::zhc::TzConverter* const kTzAduLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzAduLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAduLight) / sizeof(kTzAduLight[0]));

const ::zhc::Expose kExposesAduLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAduLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAduLight) / sizeof(kExposesAduLight[0]));

const ::zhc::BindingSpec kBindingsAduLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsAduLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAduLight) / sizeof(kBindingsAduLight[0]));

// ── Dimmable + electricity meter (m.light() + m.electricityMeter) ───
const ::zhc::FzConverter* const kFzAduLightEM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const std::uint8_t kFzAduLightEMCount =
    static_cast<std::uint8_t>(sizeof(kFzAduLightEM) / sizeof(kFzAduLightEM[0]));

const ::zhc::TzConverter* const kTzAduLightEM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzAduLightEMCount =
    static_cast<std::uint8_t>(sizeof(kTzAduLightEM) / sizeof(kTzAduLightEM[0]));

const ::zhc::Expose kExposesAduLightEM[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "energy",     ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "kWh", nullptr, nullptr, 0 },
    { "power",      ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "W",   nullptr, nullptr, 0 },
    { "voltage",    ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "V",   nullptr, nullptr, 0 },
    { "current",    ::zhc::ExposeType::Numeric, ::zhc::Access::State,
      "A",   nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAduLightEMCount =
    static_cast<std::uint8_t>(sizeof(kExposesAduLightEM) / sizeof(kExposesAduLightEM[0]));

const ::zhc::BindingSpec kBindingsAduLightEM[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0702 }, { 1, 0x0B04 },
};
const std::uint8_t kBindingsAduLightEMCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAduLightEM) / sizeof(kBindingsAduLightEM[0]));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzAduCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzAduCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAduCTLight) / sizeof(kFzAduCTLight[0]));

const ::zhc::TzConverter* const kTzAduCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzAduCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAduCTLight) / sizeof(kTzAduCTLight[0]));

const ::zhc::Expose kExposesAduCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAduCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAduCTLight) / sizeof(kExposesAduCTLight[0]));

const ::zhc::BindingSpec kBindingsAduCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsAduCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAduCTLight) / sizeof(kBindingsAduCTLight[0]));

// ── Full RGBCCT (m.light({colorTemp, color})) ───────────────────────
const ::zhc::FzConverter* const kFzAduColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzAduColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAduColorCTLight) / sizeof(kFzAduColorCTLight[0]));

const ::zhc::TzConverter* const kTzAduColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzAduColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAduColorCTLight) / sizeof(kTzAduColorCTLight[0]));

const ::zhc::Expose kExposesAduColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAduColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAduColorCTLight) / sizeof(kExposesAduColorCTLight[0]));

const ::zhc::BindingSpec kBindingsAduColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsAduColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAduColorCTLight) / sizeof(kBindingsAduColorCTLight[0]));

}  // namespace zhc::devices::adurosmart
