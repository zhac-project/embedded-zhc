// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Istar SCCV2403-2 — hand-rewritten CCT light bundle.
// Zigbee 3.0 LED controller, dimmable white spectrum.
// z2m extend: m.light({colorTemp: {range: [153, 370]}, turnsOffAtBrightness1: true})
//   → on/off + level + colorTemp. The colorTemp option adds the
//   lightingColorCtrl (0x0300) colour-temperature channel and a
//   color_temp expose (mireds, 153-370). The auto-port wired only
//   kFzOnOff + kFzBrightness and exposed state + brightness, dropping
//   the entire CCT channel. Restored kFzColorTemperature / kTzColorTemp,
//   the color_temp expose, and the 0x0300 binding.
// z2m-source: istar.ts #SCCV2403-2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::istar {
namespace {
const FzConverter* const kFz_SCCV2403_2[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_SCCV2403_2[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_SCCV2403_2[] = { "iStar CCT Light" };

}  // namespace


// Hand-curated CCT surface (CT range 153–370 mired per z2m).
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 153, 370, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0x0300},  // lightingColorCtrl (CT)
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_SCCV2403_2{
    .zigbee_models=kModels_SCCV2403_2, .zigbee_models_count=sizeof(kModels_SCCV2403_2)/sizeof(kModels_SCCV2403_2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SCCV2403-2", .vendor="Istar",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SCCV2403_2, .from_zigbee_count=sizeof(kFz_SCCV2403_2)/sizeof(kFz_SCCV2403_2[0]),
    .to_zigbee=kTz_SCCV2403_2, .to_zigbee_count=sizeof(kTz_SCCV2403_2)/sizeof(kTz_SCCV2403_2[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::istar
