// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "definitions/_generic/_shared.hpp"
namespace zhc::devices::avatto {
extern const ::zhc::FzConverter* const kFzAvattoLight[];
extern const std::uint8_t              kFzAvattoLightCount;
extern const ::zhc::TzConverter* const kTzAvattoLight[];
extern const std::uint8_t              kTzAvattoLightCount;
extern const ::zhc::Expose             kExposesAvattoLight[];
extern const std::uint8_t              kExposesAvattoLightCount;
extern const ::zhc::BindingSpec        kBindingsAvattoLight[];
extern const std::uint8_t              kBindingsAvattoLightCount;
}
