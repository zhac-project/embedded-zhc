// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AcuityBrandsLighting WF4C_WF6C — graduated for a parity fix.
// Juno 4" and 6" LED smart wafer downlight (tunable white).
// z2m-source: acuity_brands_lighting.ts #WF4C_WF6C.
//
// z2m bundle: m.light({colorTemp: {range: [200, 370], startup: false}}).
//   The colorTemp option adds the lightingColorCtrl (0x0300)
//   color-temperature channel + a color_temp expose (mireds, 200-370).
//   The auto-port wired only kFzOnOff + kFzBrightness and exposed
//   state + brightness, dropping the entire CCT channel. Restored
//   kFzColorTemperature / kTzColorTemp, the color_temp expose, and the
//   0x0300 binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::acuity_brands_lighting {
namespace {
const FzConverter* const kFz_WF4C_WF6C[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_WF4C_WF6C[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_WF4C_WF6C[] = { "ABL-LIGHT-Z-001" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 200, 370, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_WF4C_WF6C{
    .zigbee_models=kModels_WF4C_WF6C, .zigbee_models_count=sizeof(kModels_WF4C_WF6C)/sizeof(kModels_WF4C_WF6C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WF4C_WF6C", .vendor="AcuityBrandsLighting",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WF4C_WF6C, .from_zigbee_count=sizeof(kFz_WF4C_WF6C)/sizeof(kFz_WF4C_WF6C[0]),
    .to_zigbee=kTz_WF4C_WF6C, .to_zigbee_count=sizeof(kTz_WF4C_WF6C)/sizeof(kTz_WF4C_WF6C[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::acuity_brands_lighting
