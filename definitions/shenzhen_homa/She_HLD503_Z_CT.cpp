// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: ShenzhenHoma HLD503-Z-CT — hand-rewritten CTLight bundle.
// Smart LED driver (tunable white).
// z2m extend: m.light({colorTemp: {range: [150, 500]}})
//   → on/off + brightness + color_temp (mireds, range 150-500).
// The auto-generator collapsed m.light() to on/off + brightness only, dropping
// the colorTemp axis (z2m adds fz.color_colortemp + the color_temp expose when
// colorTemp is set) — this file restores it via kFzColorTemperature (0x0007).
// z2m-source: shenzhen_homa.ts #HLD503-Z-CT.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::shenzhen_homa {
namespace {
const FzConverter* const kFz_HLD503_Z_CT[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_HLD503_Z_CT[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_HLD503_Z_CT[] = { "HOMA1009", "050", "HOMA1022" };

}  // namespace


// Hand-curated CTLight surface (color_temp in mireds, range 150-500).
constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (color_temp)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_HLD503_Z_CT{
    .zigbee_models=kModels_HLD503_Z_CT, .zigbee_models_count=sizeof(kModels_HLD503_Z_CT)/sizeof(kModels_HLD503_Z_CT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HLD503-Z-CT", .vendor="ShenzhenHoma",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HLD503_Z_CT, .from_zigbee_count=sizeof(kFz_HLD503_Z_CT)/sizeof(kFz_HLD503_Z_CT[0]),
    .to_zigbee=kTz_HLD503_Z_CT, .to_zigbee_count=sizeof(kTz_HLD503_Z_CT)/sizeof(kTz_HLD503_Z_CT[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::shenzhen_homa
