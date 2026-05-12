// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nous E5 — hand-rewritten from z2m.
// Temperature & humidity (TS0201, _TZ3000_lbtpiody)
// z2m-source: nous.ts #E5.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nous {
namespace {
const FzConverter* const kFz_E5[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_E5[] = { "TS0201" };
constexpr const char* kManus_E5[] = { "_TZ3000_lbtpiody" };

constexpr Expose kExp_E5[] = {
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_E5[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0402 },  // msTemperatureMeasurement
    { 1, 0x0405 },  // msRelativeHumidity
};
}  // namespace

extern const PreparedDefinition kDef_E5{
    .zigbee_models=kModels_E5, .zigbee_models_count=sizeof(kModels_E5)/sizeof(kModels_E5[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_E5, .manufacturer_names_count=sizeof(kManus_E5)/sizeof(kManus_E5[0]),
    .model="E5", .vendor="Nous",
    .meta=nullptr, .exposes=kExp_E5, .exposes_count=sizeof(kExp_E5)/sizeof(kExp_E5[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E5, .from_zigbee_count=sizeof(kFz_E5)/sizeof(kFz_E5[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_E5, .bindings_count=sizeof(kBind_E5)/sizeof(kBind_E5[0]),
};

}  // namespace zhc::devices::nous
