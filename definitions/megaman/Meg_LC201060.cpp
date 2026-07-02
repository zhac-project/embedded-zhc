// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Megaman LC201060 dimmable light (z2m v26.77.0 parity).
// z2m-source: megaman.ts #LC201060 — extend:[m.light()], dimmable only (no color_temp).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::megaman {
namespace {
// modelID "ZLL-DimmableLight" is a generic ZLL id shared across vendors, so
// manufacturerName is the discriminator. z2m fingerprints it null-padded
// (manufacturerName "Megaman" + a trailing NUL); we store the bare "Megaman"
// — the runtime candidate is a NUL-terminated C-string, so the exact-list
// strcmp matches without the padding (mirrors the lonsonho/giex names idiom).
constexpr const char* kModels_LC201060[] = { "ZLL-DimmableLight" };
constexpr const char* kManus_LC201060[]  = { "Megaman" };
constexpr Expose kExposes_LC201060[] = {
    { "state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const FzConverter* const kFz_LC201060[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_LC201060[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr BindingSpec kBindings_LC201060[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};
}  // namespace
extern const PreparedDefinition kDef_LC201060{
    .zigbee_models=kModels_LC201060, .zigbee_models_count=sizeof(kModels_LC201060)/sizeof(kModels_LC201060[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_LC201060, .manufacturer_names_count=sizeof(kManus_LC201060)/sizeof(kManus_LC201060[0]),
    .model="LC201060", .vendor="Megaman",
    .meta=nullptr, .exposes=kExposes_LC201060, .exposes_count=sizeof(kExposes_LC201060)/sizeof(kExposes_LC201060[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LC201060, .from_zigbee_count=sizeof(kFz_LC201060)/sizeof(kFz_LC201060[0]),
    .to_zigbee=kTz_LC201060, .to_zigbee_count=sizeof(kTz_LC201060)/sizeof(kTz_LC201060[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_LC201060, .bindings_count=sizeof(kBindings_LC201060)/sizeof(kBindings_LC201060[0]),
};
}  // namespace zhc::devices::megaman
