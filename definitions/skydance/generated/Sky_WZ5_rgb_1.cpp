// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Skydance WZ5_rgb_1 — hand-rewritten for parity (wrong-bundle fix).
// Zigbee & RF 5 in 1 LED controller (RGB mode), TS0503B / _TZB210_zdvrsts8.
// z2m: extend:[tuya.modernExtend.tuyaLight({color:{modes:["hs","xy"]}})] —
// real ZCL color light on genOnOff/genLevelCtrl/lightingColorCtrl, NOT
// a battery+onOff sensor (the auto-generated minimal bundle was wrong).
// z2m-source: skydance.ts #WZ5_rgb_1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::skydance {
namespace {

const FzConverter* const kFz_WZ5_rgb_1[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_WZ5_rgb_1[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_WZ5_rgb_1[] = { "TS0503B" };
constexpr const char* kManus_WZ5_rgb_1[]  = { "_TZB210_zdvrsts8" };

constexpr Expose kExp_WZ5_rgb_1[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_xy",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_hs",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_WZ5_rgb_1[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_WZ5_rgb_1{
    .zigbee_models=kModels_WZ5_rgb_1, .zigbee_models_count=sizeof(kModels_WZ5_rgb_1)/sizeof(kModels_WZ5_rgb_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WZ5_rgb_1, .manufacturer_names_count=sizeof(kManus_WZ5_rgb_1)/sizeof(kManus_WZ5_rgb_1[0]),
    .model="WZ5_rgb_1", .vendor="Skydance",
    .meta=nullptr, .exposes=kExp_WZ5_rgb_1, .exposes_count=sizeof(kExp_WZ5_rgb_1)/sizeof(kExp_WZ5_rgb_1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WZ5_rgb_1, .from_zigbee_count=sizeof(kFz_WZ5_rgb_1)/sizeof(kFz_WZ5_rgb_1[0]),
    .to_zigbee=kTz_WZ5_rgb_1, .to_zigbee_count=sizeof(kTz_WZ5_rgb_1)/sizeof(kTz_WZ5_rgb_1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_WZ5_rgb_1, .bindings_count=sizeof(kBind_WZ5_rgb_1)/sizeof(kBind_WZ5_rgb_1[0]),
};

}  // namespace zhc::devices::skydance
