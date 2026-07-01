// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Klikaanklikuit ZCC-250 socket dimmer (z2m v26.76.0 parity).
// z2m-source: klikaanklikuit.ts #ZCC-250. extend=[m.light()] → single-endpoint
// dimmable light: on/off (genOnOff 0x0006) via fz/tz on_off + brightness
// (genLevelCtrl 0x0008) via fz/tz brightness. No colorTemp/color axis.
// Mirrors the plain-dimmer surface of icasa/ICZB-IW21D (minus metering).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::klikaanklikuit {
namespace {
constexpr const char* kModels_ZCC_250[] = { "Socket Dimmer" };
constexpr Expose kExposes_ZCC_250[] = {
    { "state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const FzConverter* const kFz_ZCC_250[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_ZCC_250[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr BindingSpec kBindings_ZCC_250[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};
}  // namespace
extern const PreparedDefinition kDef_ZCC_250{
    .zigbee_models=kModels_ZCC_250, .zigbee_models_count=sizeof(kModels_ZCC_250)/sizeof(kModels_ZCC_250[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZCC-250", .vendor="Klikaanklikuit",
    .meta=nullptr, .exposes=kExposes_ZCC_250, .exposes_count=sizeof(kExposes_ZCC_250)/sizeof(kExposes_ZCC_250[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZCC_250, .from_zigbee_count=sizeof(kFz_ZCC_250)/sizeof(kFz_ZCC_250[0]),
    .to_zigbee=kTz_ZCC_250, .to_zigbee_count=sizeof(kTz_ZCC_250)/sizeof(kTz_ZCC_250[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZCC_250, .bindings_count=sizeof(kBindings_ZCC_250)/sizeof(kBindings_ZCC_250[0]),
};
}  // namespace zhc::devices::klikaanklikuit
