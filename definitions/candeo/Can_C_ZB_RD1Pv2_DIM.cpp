// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Candeo C-ZB-RD1Pv2-DIM rotary dimmer pro, v2 hardware
// (dimmer mode) — new upstream def in z2m v26.97.0.
//
// Upstream added the v2 revisions of this family three different ways:
// the DPM and REM variants gained `C-ZB-RD1Pv2-*` as an extra
// fingerprint on their existing entry, while the DIM variant got a
// whole new entry — because the v2 dimmer fingerprints on
// manufacturerName "Candeo" as well as modelID. This def mirrors that
// split rather than folding the v2 model into kDef_C_ZB_RD1P_DIM.
//
// Same functional surface as the v1 dimmer: on/off + brightness plus a
// four-quantity electricity meter (metering 0x0702 for energy, and
// electrical measurement 0x0B04 for power / voltage / current).
//
// DEFERRED: the knob's extra button surface — z2m `fzLocal.rd1p_knob_press`
// and `fzLocal.rd1p_rem_lite_ep2` decode `double_pressed` / `held` /
// `released` on endpoint 2 and gate them behind a `rem_lite_ep2` config
// binary written to a manufacturer attribute on genOnOff. Both need a
// custom-cluster codec with no ::zhc::generic counterpart; the dimming
// function is fully covered without them.
//
// z2m-source: candeo.ts #C-ZB-RD1Pv2-DIM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::candeo {
namespace {

const FzConverter* const kFz_C_ZB_RD1Pv2_DIM[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_C_ZB_RD1Pv2_DIM[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzPowerOnBehavior1,
};

constexpr const char* kModels_C_ZB_RD1Pv2_DIM[] = { "C-ZB-RD1Pv2-DIM" };
constexpr const char* kManu_C_ZB_RD1Pv2_DIM[]   = { "Candeo" };

constexpr Expose kExposes_C_ZB_RD1Pv2_DIM[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",            ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",             ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"voltage",           ExposeType::Numeric, Access::State,    "V",     nullptr, nullptr, 0},
    {"current",           ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config},
};

constexpr BindingSpec kBindings_C_ZB_RD1Pv2_DIM[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0702},   // seMetering
    {1, 0x0B04},   // haElectricalMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_C_ZB_RD1Pv2_DIM{
    .zigbee_models=kModels_C_ZB_RD1Pv2_DIM,
    .zigbee_models_count=sizeof(kModels_C_ZB_RD1Pv2_DIM)/sizeof(kModels_C_ZB_RD1Pv2_DIM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManu_C_ZB_RD1Pv2_DIM,
    .manufacturer_names_count=sizeof(kManu_C_ZB_RD1Pv2_DIM)/sizeof(kManu_C_ZB_RD1Pv2_DIM[0]),
    .model="C-ZB-RD1Pv2-DIM", .vendor="Candeo",
    .meta=nullptr,
    .exposes=kExposes_C_ZB_RD1Pv2_DIM,
    .exposes_count=sizeof(kExposes_C_ZB_RD1Pv2_DIM)/sizeof(kExposes_C_ZB_RD1Pv2_DIM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_C_ZB_RD1Pv2_DIM,
    .from_zigbee_count=sizeof(kFz_C_ZB_RD1Pv2_DIM)/sizeof(kFz_C_ZB_RD1Pv2_DIM[0]),
    .to_zigbee=kTz_C_ZB_RD1Pv2_DIM,
    .to_zigbee_count=sizeof(kTz_C_ZB_RD1Pv2_DIM)/sizeof(kTz_C_ZB_RD1Pv2_DIM[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_C_ZB_RD1Pv2_DIM,
    .bindings_count=sizeof(kBindings_C_ZB_RD1Pv2_DIM)/sizeof(kBindings_C_ZB_RD1Pv2_DIM[0]),
};

}  // namespace zhc::devices::candeo
