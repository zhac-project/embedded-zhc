// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: TaskLighting T-TWAV-60W-PSR — hand-rewritten ColorCTLight bundle.
// WAV smart receiver LED lighting controller.
// z2m extend: m.light({colorTemp: {range: [150, 500]}, color: true})
//   → on/off + level + colorTemp (150-500 mired) + color (xy mode only).
// `color: true` defaults to {modes: ["xy"], enhancedHue: false} (modernExtend.ts
// L1194-1201), so the surface is color_x/color_y — NO hue/saturation.
// The auto-generator collapses m.light() to on/off + brightness only, dropping
// the CT/color axes — this file restores them.
// z2m-source: task_lighting.ts #T-TWAV-60W-PSR.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::task_lighting {
namespace {
const FzConverter* const kFz_T_TWAV_60W_PSR[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_T_TWAV_60W_PSR[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_T_TWAV_60W_PSR[] = { "WAV Smart Receiver" };

}  // namespace


// Hand-curated ColorCTLight surface (xy color mode + colorTemp).
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT + color)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_T_TWAV_60W_PSR{
    .zigbee_models=kModels_T_TWAV_60W_PSR, .zigbee_models_count=sizeof(kModels_T_TWAV_60W_PSR)/sizeof(kModels_T_TWAV_60W_PSR[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T-TWAV-60W-PSR", .vendor="TaskLighting",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T_TWAV_60W_PSR, .from_zigbee_count=sizeof(kFz_T_TWAV_60W_PSR)/sizeof(kFz_T_TWAV_60W_PSR[0]),
    .to_zigbee=kTz_T_TWAV_60W_PSR, .to_zigbee_count=sizeof(kTz_T_TWAV_60W_PSR)/sizeof(kTz_T_TWAV_60W_PSR[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::task_lighting
