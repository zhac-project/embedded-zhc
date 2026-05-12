// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0224 — auto-generated.
// Smart light & sound siren
// z2m-source: tuya.ts #TS0224.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

const TzConverter* const kTz_TS0224[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_TS0224[] = { "TS0224" };

}  // namespace

namespace {
const FzConverter* const kFz_min_TS0224_Tuy_TS0224[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0224_Tuy_TS0224[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0224_Tuy_TS0224[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0224{
    .zigbee_models=kModels_TS0224, .zigbee_models_count=sizeof(kModels_TS0224)/sizeof(kModels_TS0224[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TS0224", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0224_Tuy_TS0224, .exposes_count=sizeof(kExp_min_TS0224_Tuy_TS0224)/sizeof(kExp_min_TS0224_Tuy_TS0224[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0224_Tuy_TS0224, .from_zigbee_count=sizeof(kFz_min_TS0224_Tuy_TS0224)/sizeof(kFz_min_TS0224_Tuy_TS0224[0]),
    .to_zigbee=kTz_TS0224, .to_zigbee_count=sizeof(kTz_TS0224)/sizeof(kTz_TS0224[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
