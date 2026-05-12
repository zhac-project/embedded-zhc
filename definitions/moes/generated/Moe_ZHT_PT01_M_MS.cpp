// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Moes ZHT-PT01-M-MS — auto-generated.
// Smart thermostat for pilot wire heating radiator
// z2m-source: moes.ts #ZHT-PT01-M-MS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::moes {
namespace {


constexpr const char* kModels_ZHT_PT01_M_MS[] = { "TS0601" };
constexpr const char* kManus_ZHT_PT01_M_MS[] = { "_TZE204_x9usygq1" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZHT_PT01_M_MS[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZHT_PT01_M_MS[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZHT_PT01_M_MS[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZHT_PT01_M_MS{
    .zigbee_models=kModels_ZHT_PT01_M_MS, .zigbee_models_count=sizeof(kModels_ZHT_PT01_M_MS)/sizeof(kModels_ZHT_PT01_M_MS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZHT_PT01_M_MS, .manufacturer_names_count=sizeof(kManus_ZHT_PT01_M_MS)/sizeof(kManus_ZHT_PT01_M_MS[0]),
    .model="ZHT-PT01-M-MS", .vendor="Moes",
    .meta=nullptr, .exposes=kExp_min_ZHT_PT01_M_MS, .exposes_count=sizeof(kExp_min_ZHT_PT01_M_MS)/sizeof(kExp_min_ZHT_PT01_M_MS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZHT_PT01_M_MS, .from_zigbee_count=sizeof(kFz_min_ZHT_PT01_M_MS)/sizeof(kFz_min_ZHT_PT01_M_MS[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::moes
