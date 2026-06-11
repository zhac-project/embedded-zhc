// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Cwd HLL6948V1 — hand-restored CCT (color-temp) light axis.
// Collingwood H2 pro.
// z2m extend: m.light({colorTemp: {range: [153, 370]}})
//   → on/off + level + colorTemp (lightingColorCtrl 0x0300, NO xy/hs color).
// The auto-generator collapses m.light() to on/off + brightness only,
// dropping the color_temp axis (modernExtend.ts light() lines 1217-1233
// add tz.light_colortemp + e.withColorTemp(range) + the 0x0300 binding
// only when called with a colorTemp arg). This file restores it via the
// generic kFzColorTemperature (0x0300 attr 0x0007 → color_temp) /
// kTzColorTemp (moveToColorTemp 0x0A). No `color` arg is present in z2m,
// so we add NO color_x/color_y/hue/saturation.
// z2m-source: cwd.ts #HLL6948V1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::cwd {
namespace {
const FzConverter* const kFz_HLL6948V1[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_HLL6948V1[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_HLL6948V1[] = { "ZBT-CCTLight-D0001" };

}  // namespace


// --- hand-restored CCT light surface ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (color_temp)
};
// --- end hand-restored block ---

extern const PreparedDefinition kDef_HLL6948V1{
    .zigbee_models=kModels_HLL6948V1, .zigbee_models_count=sizeof(kModels_HLL6948V1)/sizeof(kModels_HLL6948V1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HLL6948V1", .vendor="Cwd",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HLL6948V1, .from_zigbee_count=sizeof(kFz_HLL6948V1)/sizeof(kFz_HLL6948V1[0]),
    .to_zigbee=kTz_HLL6948V1, .to_zigbee_count=sizeof(kTz_HLL6948V1)/sizeof(kTz_HLL6948V1[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::cwd
