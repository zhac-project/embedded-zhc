// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes ZHT-S03 — auto-generated.
// Zigbee wall thermostat
// z2m-source: moes.ts #ZHT-S03.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::moes {
namespace {


constexpr const char* kModels_ZHT_S03[] = { "TS0601" };
constexpr const char* kManus_ZHT_S03[] = { "_TZE204_zxkwaztm" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZHT_S03[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZHT_S03[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZHT_S03[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZHT_S03{
    .zigbee_models=kModels_ZHT_S03, .zigbee_models_count=sizeof(kModels_ZHT_S03)/sizeof(kModels_ZHT_S03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZHT_S03, .manufacturer_names_count=sizeof(kManus_ZHT_S03)/sizeof(kManus_ZHT_S03[0]),
    .model="ZHT-S03", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_min_ZHT_S03, .exposes_count=sizeof(kExp_min_ZHT_S03)/sizeof(kExp_min_ZHT_S03[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZHT_S03, .from_zigbee_count=sizeof(kFz_min_ZHT_S03)/sizeof(kFz_min_ZHT_S03[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::moes
