// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ecodim ED-10042 — Tuya filament dimmer (hand-rewritten, 2026-04-28q).
// Zigbee LED filament light dimmable E27, globe G125, flame 2200K.
// z2m-source: ecodim.ts #ED-10042.
//
// fingerprint: TS0501B + manufacturerName "_TZ3210_yluvwhjc".
// extend: tuya.modernExtend.tuyaLight() — bare brightness dimmer, no
// CT, no color. Minimal-port previously dropped brightness; restored.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
const FzConverter* const kFz_ED_10042[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_ED_10042[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_ED_10042[] = { "TS0501B" };
constexpr const char* kManus_ED_10042[]  = { "_TZ3210_yluvwhjc" };

constexpr Expose kExposes_ED_10042[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ED_10042[] = {
    {1, 0x0006},
    {1, 0x0008},
};

}  // namespace

extern const PreparedDefinition kDef_ED_10042{
    .zigbee_models=kModels_ED_10042, .zigbee_models_count=sizeof(kModels_ED_10042)/sizeof(kModels_ED_10042[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ED_10042, .manufacturer_names_count=sizeof(kManus_ED_10042)/sizeof(kManus_ED_10042[0]),
    .model="ED-10042", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ED_10042, .exposes_count=sizeof(kExposes_ED_10042)/sizeof(kExposes_ED_10042[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ED_10042, .from_zigbee_count=sizeof(kFz_ED_10042)/sizeof(kFz_ED_10042[0]),
    .to_zigbee=kTz_ED_10042, .to_zigbee_count=sizeof(kTz_ED_10042)/sizeof(kTz_ED_10042[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ED_10042, .bindings_count=sizeof(kBindings_ED_10042)/sizeof(kBindings_ED_10042[0]),
};

}  // namespace zhc::devices::ecodim
