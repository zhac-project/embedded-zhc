// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nous E6 — hand-rewritten from z2m (PARTIAL).
// Temperature & humidity LCD sensor (TS0601, multiple TZE manufacturers).
// z2m uses legacy.fz.nous_lcd_temperature_humidity_sensor; full DP map not yet
// ported. Exposes the publishable fields; runtime decode awaits legacy port.
// z2m-source: nous.ts #E6.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nous {
namespace {
const FzConverter* const kFz_E6[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_E6[] = { "TS0601" };
constexpr const char* kManus_E6[] = {
    "_TZE284_wtikaxzs", "_TZE200_nnrfa68v", "_TZE200_zppcgbdj", "_TZE200_wtikaxzs",
};

constexpr Expose kExp_E6[] = {
    { "temperature",     ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "humidity",        ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "battery",         ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "battery_low",     ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "max_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "min_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "max_humidity",    ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0 },
    { "min_humidity",    ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_E6[] = {
    { 1, 0xEF00 },  // manuSpecificTuya
};
}  // namespace

extern const PreparedDefinition kDef_E6{
    .zigbee_models=kModels_E6, .zigbee_models_count=sizeof(kModels_E6)/sizeof(kModels_E6[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_E6, .manufacturer_names_count=sizeof(kManus_E6)/sizeof(kManus_E6[0]),
    .model="E6", .vendor="Nous",
    .meta=nullptr, .exposes=kExp_E6, .exposes_count=sizeof(kExp_E6)/sizeof(kExp_E6[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E6, .from_zigbee_count=sizeof(kFz_E6)/sizeof(kFz_E6[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_E6, .bindings_count=sizeof(kBind_E6)/sizeof(kBind_E6[0]),
};

}  // namespace zhc::devices::nous
