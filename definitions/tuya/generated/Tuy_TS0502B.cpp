// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0502B — auto-generated.
// Light controller
// z2m-source: tuya.ts #TS0502B.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0502B[] = { "TS0502B", "TS0503B" };
constexpr const char* kManus_TS0502B[] = { "_TZB210_lmqquxus" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0502B_Tuy_TS0502B[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0502B_Tuy_TS0502B[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0502B_Tuy_TS0502B[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0502B{
    .zigbee_models=kModels_TS0502B, .zigbee_models_count=sizeof(kModels_TS0502B)/sizeof(kModels_TS0502B[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0502B, .manufacturer_names_count=sizeof(kManus_TS0502B)/sizeof(kManus_TS0502B[0]),
    .model="TS0502B", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0502B_Tuy_TS0502B, .exposes_count=sizeof(kExp_min_TS0502B_Tuy_TS0502B)/sizeof(kExp_min_TS0502B_Tuy_TS0502B[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0502B_Tuy_TS0502B, .from_zigbee_count=sizeof(kFz_min_TS0502B_Tuy_TS0502B)/sizeof(kFz_min_TS0502B_Tuy_TS0502B[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
