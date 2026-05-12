// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Orvibo DD10Z.
// Smart spotlight
// z2m-source: orvibo.ts #DD10Z.
// DD10Z uses moveToLevel (no OnOff variant) — ack via
// tzLocal.DD10Z_brightness in z2m. ZHC's kTzBrightness uses
// moveToLevelWithOnOff. Brightness will work but the ramp may drive a
// transient ON command — disableDefaultResponse on z2m's side avoids the
// ACK stall. Documented; full parity needs a per-device tz override.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_DD10Z[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_DD10Z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_DD10Z[] = { "ccb9f56837ab41dcad366fb1452096b6", "250bccf66c41421b91b5e3242942c164", "af22cef59b2543d1be1dfab4f1c9c920" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mireds", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_DD10Z{
    .zigbee_models=kModels_DD10Z, .zigbee_models_count=sizeof(kModels_DD10Z)/sizeof(kModels_DD10Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DD10Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DD10Z, .from_zigbee_count=sizeof(kFz_DD10Z)/sizeof(kFz_DD10Z[0]),
    .to_zigbee=kTz_DD10Z, .to_zigbee_count=sizeof(kTz_DD10Z)/sizeof(kTz_DD10Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
