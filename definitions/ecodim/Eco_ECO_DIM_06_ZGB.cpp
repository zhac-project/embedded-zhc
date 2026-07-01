// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Ecodim ECO-DIM.06-ZGB plug-in dimmer (on/off + brightness) (z2m v26.76.0 parity).
// z2m-source: ecodim.ts #ECO-DIM.06-ZGB
//
// Plug-in dimmer, phase cut-off (trailing edge R,C). Single-endpoint
// dimmable light via m.light({configureReporting:true}): genOnOff (0x0006)
// + genLevelCtrl (0x0008), covered by the generic on/off + brightness
// converter bundle. No color/colorTemp.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ecodim {
namespace {
constexpr const char* kModels_ECO_DIM_06_ZGB[] = { "eco-dim06-zigbee" };
constexpr Expose kExposes_ECO_DIM_06_ZGB[] = {
    { "state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const FzConverter* const kFz_ECO_DIM_06_ZGB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_ECO_DIM_06_ZGB[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr BindingSpec kBindings_ECO_DIM_06_ZGB[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};
}  // namespace
extern const PreparedDefinition kDef_ECO_DIM_06_ZGB{
    .zigbee_models=kModels_ECO_DIM_06_ZGB, .zigbee_models_count=sizeof(kModels_ECO_DIM_06_ZGB)/sizeof(kModels_ECO_DIM_06_ZGB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ECO-DIM.06-ZGB", .vendor="Ecodim",
    .meta=nullptr, .exposes=kExposes_ECO_DIM_06_ZGB, .exposes_count=sizeof(kExposes_ECO_DIM_06_ZGB)/sizeof(kExposes_ECO_DIM_06_ZGB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ECO_DIM_06_ZGB, .from_zigbee_count=sizeof(kFz_ECO_DIM_06_ZGB)/sizeof(kFz_ECO_DIM_06_ZGB[0]),
    .to_zigbee=kTz_ECO_DIM_06_ZGB, .to_zigbee_count=sizeof(kTz_ECO_DIM_06_ZGB)/sizeof(kTz_ECO_DIM_06_ZGB[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ECO_DIM_06_ZGB, .bindings_count=sizeof(kBindings_ECO_DIM_06_ZGB)/sizeof(kBindings_ECO_DIM_06_ZGB[0]),
};
}  // namespace zhc::devices::ecodim
