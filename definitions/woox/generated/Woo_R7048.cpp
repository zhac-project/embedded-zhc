// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Woox R7048 — humidity & temperature sensor (TS0201).
// z2m-source: woox.ts #R7048 (fz.battery, fz.temperature, fz.humidity).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::woox {
namespace {
const FzConverter* const kFz_R7048[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};

constexpr const char* kModels_R7048[] = { "TS0201" };
constexpr const char* kManus_R7048[] = { "_TZ3000_rusu2vzb", "_TZ3000_amqudjr0" };

constexpr Expose kExp_R7048[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C",  nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_R7048[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
};
}  // namespace

extern const PreparedDefinition kDef_R7048{
    .zigbee_models=kModels_R7048, .zigbee_models_count=sizeof(kModels_R7048)/sizeof(kModels_R7048[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_R7048, .manufacturer_names_count=sizeof(kManus_R7048)/sizeof(kManus_R7048[0]),
    .model="R7048", .vendor="Woox",
    .meta=nullptr, .exposes=kExp_R7048, .exposes_count=sizeof(kExp_R7048)/sizeof(kExp_R7048[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_R7048, .from_zigbee_count=sizeof(kFz_R7048)/sizeof(kFz_R7048[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_R7048,
    .bindings_count=sizeof(kBind_R7048)/sizeof(kBind_R7048[0]),
};

}  // namespace zhc::devices::woox
