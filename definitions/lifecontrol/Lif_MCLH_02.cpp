// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lifecontrol MCLH-02 — hand-rewritten (vendor parity sweep).
// Smart light bulb (full-colour CT bulb).
// z2m-source: lifecontrol.ts #MCLH-02.
//
// z2m: m.light({colorTemp: {range: [167, 333]}, color: true}) — i.e. a
// dimmable bulb with BOTH colour-temperature AND xy/hue-sat colour
// control. The auto-generated port wired only genOnOff + genLevelCtrl
// (state + brightness), dropping the entire lightingColorCtrl (0x0300)
// half — color_temp / color_x / color_y / hue / saturation were dead.
// Wire the generic colour-temperature + colour converters (fz + tz) and
// expose the full channel set, plus the 0x0300 bind.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lifecontrol {
namespace {
const FzConverter* const kFz_MCLH_02[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_MCLH_02[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_MCLH_02[] = { "vivi ZLight" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired",  nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr,  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl
};

extern const PreparedDefinition kDef_MCLH_02{
    .zigbee_models=kModels_MCLH_02, .zigbee_models_count=sizeof(kModels_MCLH_02)/sizeof(kModels_MCLH_02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MCLH-02", .vendor="Lifecontrol",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MCLH_02, .from_zigbee_count=sizeof(kFz_MCLH_02)/sizeof(kFz_MCLH_02[0]),
    .to_zigbee=kTz_MCLH_02, .to_zigbee_count=sizeof(kTz_MCLH_02)/sizeof(kTz_MCLH_02[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lifecontrol
