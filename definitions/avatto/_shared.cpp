// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#include "definitions/avatto/_shared.hpp"
namespace zhc::devices::avatto {
const ::zhc::FzConverter* const kFzAvattoLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const std::uint8_t kFzAvattoLightCount =
    static_cast<std::uint8_t>(sizeof(kFzAvattoLight)/sizeof(kFzAvattoLight[0]));
const ::zhc::TzConverter* const kTzAvattoLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
const std::uint8_t kTzAvattoLightCount =
    static_cast<std::uint8_t>(sizeof(kTzAvattoLight)/sizeof(kTzAvattoLight[0]));
const ::zhc::Expose kExposesAvattoLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
const std::uint8_t kExposesAvattoLightCount =
    static_cast<std::uint8_t>(sizeof(kExposesAvattoLight)/sizeof(kExposesAvattoLight[0]));
const ::zhc::BindingSpec kBindingsAvattoLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
const std::uint8_t kBindingsAvattoLightCount =
    static_cast<std::uint8_t>(sizeof(kBindingsAvattoLight)/sizeof(kBindingsAvattoLight[0]));
}
