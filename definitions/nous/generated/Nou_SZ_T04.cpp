// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nous SZ-T04 — hand-rewritten from z2m (PARTIAL).
// Temperature & humidity sensor with clock (TS0601, multiple TZE200 manufacturers).
// z2m uses legacy.fz.nous_lcd_temperature_humidity_sensor; full DP map not yet
// ported. Exposes are listed; runtime decode awaits legacy port.
// White-label: Tuya TH01Z (_TZE200_locansqn).
// z2m-source: nous.ts #SZ-T04.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nous {
namespace {
const FzConverter* const kFz_SZ_T04[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_SZ_T04[] = { "TS0601" };
constexpr const char* kManus_SZ_T04[] = {
    "_TZE200_lve3dvpy", "_TZE200_c7emyjom", "_TZE200_locansqn", "_TZE200_qrztc3ev",
    "_TZE200_snloy4rw", "_TZE200_eanjj2pa", "_TZE200_ydrdfkim", "_TZE284_locansqn",
};

constexpr Expose kExp_SZ_T04[] = {
    { "temperature",                ExposeType::Numeric, ::zhc::Access::State,    "C",   nullptr, nullptr, 0 },
    { "humidity",                   ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "battery",                    ExposeType::Numeric, ::zhc::Access::State,    "%",   nullptr, nullptr, 0 },
    { "temperature_report_interval",ExposeType::Numeric, ::zhc::Access::StateSet, "min", nullptr, nullptr, 0 },
    { "humidity_report_interval",   ExposeType::Numeric, ::zhc::Access::StateSet, "min", nullptr, nullptr, 0 },
    { "max_temperature",            ExposeType::Numeric, ::zhc::Access::StateSet, "C",   nullptr, nullptr, 0 },
    { "min_temperature",            ExposeType::Numeric, ::zhc::Access::StateSet, "C",   nullptr, nullptr, 0 },
    { "max_humidity",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0 },
    { "min_humidity",               ExposeType::Numeric, ::zhc::Access::StateSet, "%",   nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_SZ_T04[] = {
    { 1, 0xEF00 },  // manuSpecificTuya
    { 1, 0x0000 },  // genBasic (z2m configure binds it)
};
}  // namespace

extern const PreparedDefinition kDef_SZ_T04{
    .zigbee_models=kModels_SZ_T04, .zigbee_models_count=sizeof(kModels_SZ_T04)/sizeof(kModels_SZ_T04[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SZ_T04, .manufacturer_names_count=sizeof(kManus_SZ_T04)/sizeof(kManus_SZ_T04[0]),
    .model="SZ-T04", .vendor="Nous",
    .meta=nullptr, .exposes=kExp_SZ_T04, .exposes_count=sizeof(kExp_SZ_T04)/sizeof(kExp_SZ_T04[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SZ_T04, .from_zigbee_count=sizeof(kFz_SZ_T04)/sizeof(kFz_SZ_T04[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SZ_T04, .bindings_count=sizeof(kBind_SZ_T04)/sizeof(kBind_SZ_T04[0]),
};

}  // namespace zhc::devices::nous
