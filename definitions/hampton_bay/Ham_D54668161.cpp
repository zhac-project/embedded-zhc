// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HamptonBay 54668161 — hand-rewritten for parity (CT axis).
// 12 in. LED smart puff.
//
// z2m: extend:[m.light({colorTemp: {range: undefined}})] — a tunable-white
// light (on/off + brightness + color_temp). The auto-port modelled only the
// on/off + brightness half and DROPPED the color_temp axis entirely, so the
// lightingColorCtrl (0x0300) colorTemperature attr was never decoded and the
// CT setter was unreachable. Added kFzColorTemperature (attr 0x0007 ->
// `color_temp`, mireds) + kTzColorTemp + the color_temp expose + the 0x0300
// binding, mirroring the Trust ZLED-TUNE9 tunable-white precedent.
//
// z2m-source: hampton_bay.ts #54668161.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hampton_bay {
namespace {
const FzConverter* const kFz_D54668161[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_D54668161[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D54668161[] = { "ETI 12-in Puff light" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_D54668161{
    .zigbee_models=kModels_D54668161, .zigbee_models_count=sizeof(kModels_D54668161)/sizeof(kModels_D54668161[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="54668161", .vendor="HamptonBay",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D54668161, .from_zigbee_count=sizeof(kFz_D54668161)/sizeof(kFz_D54668161[0]),
    .to_zigbee=kTz_D54668161, .to_zigbee_count=sizeof(kTz_D54668161)/sizeof(kTz_D54668161[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hampton_bay
