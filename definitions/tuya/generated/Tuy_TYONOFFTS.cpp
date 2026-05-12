// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TYONOFFTS — auto-generated.
// Smart switch with temperature sensor
// z2m-source: tuya.ts #TYONOFFTS.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TYONOFFTS[] = { "TS0001" };
constexpr const char* kManus_TYONOFFTS[] = { "_TZE21C_dohbhb5k" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TYONOFFTS_Tuy_TYONOFFTS[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TYONOFFTS_Tuy_TYONOFFTS[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TYONOFFTS_Tuy_TYONOFFTS[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TYONOFFTS{
    .zigbee_models=kModels_TYONOFFTS, .zigbee_models_count=sizeof(kModels_TYONOFFTS)/sizeof(kModels_TYONOFFTS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TYONOFFTS, .manufacturer_names_count=sizeof(kManus_TYONOFFTS)/sizeof(kManus_TYONOFFTS[0]),
    .model="TYONOFFTS", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TYONOFFTS_Tuy_TYONOFFTS, .exposes_count=sizeof(kExp_min_TYONOFFTS_Tuy_TYONOFFTS)/sizeof(kExp_min_TYONOFFTS_Tuy_TYONOFFTS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TYONOFFTS_Tuy_TYONOFFTS, .from_zigbee_count=sizeof(kFz_min_TYONOFFTS_Tuy_TYONOFFTS)/sizeof(kFz_min_TYONOFFTS_Tuy_TYONOFFTS[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
