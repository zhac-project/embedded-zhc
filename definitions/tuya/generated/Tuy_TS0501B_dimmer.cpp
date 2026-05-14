// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0501B_dimmer — auto-generated.
// Zigbee dimmer
// z2m-source: tuya.ts #TS0501B_dimmer.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0501B_dimmer[] = { "TS0501B" };
constexpr const char* kManus_TS0501B_dimmer[] = { "_TZB210_rkgngb5o" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0501B_dimmer_Tuy_TS0501B_dimmer[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0501B_dimmer_Tuy_TS0501B_dimmer[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0501B_dimmer_Tuy_TS0501B_dimmer[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace

constexpr WhiteLabel kWhiteLabels_TS0501B_dimmer[] = {
    {"Tuya","L1(ZW)"},
};
extern const PreparedDefinition kDef_TS0501B_dimmer{
    .zigbee_models=kModels_TS0501B_dimmer, .zigbee_models_count=sizeof(kModels_TS0501B_dimmer)/sizeof(kModels_TS0501B_dimmer[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0501B_dimmer, .manufacturer_names_count=sizeof(kManus_TS0501B_dimmer)/sizeof(kManus_TS0501B_dimmer[0]),
    .model="TS0501B_dimmer", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0501B_dimmer_Tuy_TS0501B_dimmer, .exposes_count=sizeof(kExp_min_TS0501B_dimmer_Tuy_TS0501B_dimmer)/sizeof(kExp_min_TS0501B_dimmer_Tuy_TS0501B_dimmer[0]),
    .white_labels=kWhiteLabels_TS0501B_dimmer, .white_labels_count=sizeof(kWhiteLabels_TS0501B_dimmer)/sizeof(kWhiteLabels_TS0501B_dimmer[0]),
    .from_zigbee=kFz_min_TS0501B_dimmer_Tuy_TS0501B_dimmer, .from_zigbee_count=sizeof(kFz_min_TS0501B_dimmer_Tuy_TS0501B_dimmer)/sizeof(kFz_min_TS0501B_dimmer_Tuy_TS0501B_dimmer[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
