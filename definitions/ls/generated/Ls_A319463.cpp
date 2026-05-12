// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ls A319463 — hand-ported (LS sweep, 2026-04-29c).
// Home base
// z2m-source: ls.ts #A319463.
//
// z2m: m.light({colorTemp: {range: [153, 454]}, color: true})
//      with a runtime exposes() that filters out endpoint 242
//      (green-power) and emits one
//      light_brightness_colortemp_colorxy() per remaining EP.
//
// ZHC has no per-frame endpoint introspection so we publish the
// flat single-EP shape (state / brightness / color_temp / color).
// The dispatcher will route inbound `state`, `brightness`, etc. to
// EP 1 (the canonical Zigbee root EP for an Emotion home base).
// True multi-EP coverage is the documented `PARTIAL` gap — see
// LS_PARITY.md. Bindings cover the three light-cluster ids on EP 1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ls {
namespace {
const FzConverter* const kFz_A319463[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
const TzConverter* const kTz_A319463[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_A319463[] = { "Emotion" };

constexpr Expose kExposes_A319463[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", "153..454", nullptr, 0},
    {"color",      ExposeType::String,  Access::StateSet, nullptr, "xy",      nullptr, 0},
};

constexpr BindingSpec kBind_A319463[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_A319463{
    .zigbee_models=kModels_A319463, .zigbee_models_count=sizeof(kModels_A319463)/sizeof(kModels_A319463[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="A319463", .vendor="Ls",
    .meta=nullptr, .exposes=kExposes_A319463, .exposes_count=sizeof(kExposes_A319463)/sizeof(kExposes_A319463[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_A319463, .from_zigbee_count=sizeof(kFz_A319463)/sizeof(kFz_A319463[0]),
    .to_zigbee=kTz_A319463, .to_zigbee_count=sizeof(kTz_A319463)/sizeof(kTz_A319463[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_A319463, .bindings_count=sizeof(kBind_A319463)/sizeof(kBind_A319463[0]),
};

}  // namespace zhc::devices::ls
