// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Iluminize ports — one per m.light()/m.onOff()
// feature combination seen in z2m's iluminize.ts. Modelled after
// osram/_shared.

#include "definitions/iluminize/_shared.hpp"

namespace zhc::devices::iluminize {

// ── On/off only (m.onOff()) ─────────────────────────────────────────
const ::zhc::FzConverter* const kFzIluOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
const std::uint8_t kFzIluOnOffCount =
    static_cast<std::uint8_t>(sizeof(kFzIluOnOff) / sizeof(kFzIluOnOff[0]));

const ::zhc::TzConverter* const kTzIluOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
const std::uint8_t kTzIluOnOffCount =
    static_cast<std::uint8_t>(sizeof(kTzIluOnOff) / sizeof(kTzIluOnOff[0]));

const ::zhc::Expose kExposesIluOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesIluOnOffCount =
    static_cast<std::uint8_t>(sizeof(kExposesIluOnOff) / sizeof(kExposesIluOnOff[0]));

const ::zhc::BindingSpec kBindingsIluOnOff[] = {
    { 1, 0x0006 },
};
const std::uint8_t kBindingsIluOnOffCount =
    static_cast<std::uint8_t>(sizeof(kBindingsIluOnOff) / sizeof(kBindingsIluOnOff[0]));

// ── Plain dimmable (m.light({})) ────────────────────────────────────
const ::zhc::FzConverter* const kFzIluLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const std::uint8_t kFzIluLightCount =
    static_cast<std::uint8_t>(sizeof(kFzIluLight) / sizeof(kFzIluLight[0]));

const ::zhc::TzConverter* const kTzIluLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
const std::uint8_t kTzIluLightCount =
    static_cast<std::uint8_t>(sizeof(kTzIluLight) / sizeof(kTzIluLight[0]));

const ::zhc::Expose kExposesIluLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesIluLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesIluLight) / sizeof(kExposesIluLight[0]));

const ::zhc::BindingSpec kBindingsIluLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
const std::uint8_t kBindingsIluLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsIluLight) / sizeof(kBindingsIluLight[0]));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzIluCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzIluCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzIluCTLight) / sizeof(kFzIluCTLight[0]));

const ::zhc::TzConverter* const kTzIluCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzIluCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzIluCTLight) / sizeof(kTzIluCTLight[0]));

const ::zhc::Expose kExposesIluCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesIluCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesIluCTLight) / sizeof(kExposesIluCTLight[0]));

const ::zhc::BindingSpec kBindingsIluCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsIluCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsIluCTLight) / sizeof(kBindingsIluCTLight[0]));

// ── Colour-only (m.light({color: true})) ────────────────────────────
const ::zhc::FzConverter* const kFzIluColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzIluColorLightCount =
    static_cast<std::uint8_t>(sizeof(kFzIluColorLight) / sizeof(kFzIluColorLight[0]));

const ::zhc::TzConverter* const kTzIluColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzIluColorLightCount =
    static_cast<std::uint8_t>(sizeof(kTzIluColorLight) / sizeof(kTzIluColorLight[0]));

const ::zhc::Expose kExposesIluColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
const std::uint8_t kExposesIluColorLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesIluColorLight) / sizeof(kExposesIluColorLight[0]));

const ::zhc::BindingSpec kBindingsIluColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsIluColorLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsIluColorLight) / sizeof(kBindingsIluColorLight[0]));

// ── Full RGBCCT (m.light({colorTemp, color: true})) ─────────────────
const ::zhc::FzConverter* const kFzIluColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const std::uint8_t kFzIluColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kFzIluColorCTLight) / sizeof(kFzIluColorCTLight[0]));

const ::zhc::TzConverter* const kTzIluColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
const std::uint8_t kTzIluColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kTzIluColorCTLight) / sizeof(kTzIluColorCTLight[0]));

const ::zhc::Expose kExposesIluColorCTLight[] = {
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
const std::uint8_t kExposesIluColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesIluColorCTLight) / sizeof(kExposesIluColorCTLight[0]));

const ::zhc::BindingSpec kBindingsIluColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsIluColorCTLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsIluColorCTLight) / sizeof(kBindingsIluColorCTLight[0]));

}  // namespace zhc::devices::iluminize
