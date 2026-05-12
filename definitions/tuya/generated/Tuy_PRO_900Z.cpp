// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya PRO-900Z — auto-generated.
// Thermostat for electric floor
// z2m-source: tuya.ts #PRO-900Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_PRO_900Z[] = { "TS0601" };
constexpr const char* kManus_PRO_900Z[] = { "_TZE204_tagezcph" };
}  // namespace

namespace {
const FzConverter* const kFz_min_PRO_900Z_Tuy_PRO_900Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_PRO_900Z_Tuy_PRO_900Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PRO_900Z_Tuy_PRO_900Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_PRO_900Z{
    .zigbee_models=kModels_PRO_900Z, .zigbee_models_count=sizeof(kModels_PRO_900Z)/sizeof(kModels_PRO_900Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PRO_900Z, .manufacturer_names_count=sizeof(kManus_PRO_900Z)/sizeof(kManus_PRO_900Z[0]),
    .model="PRO-900Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_PRO_900Z_Tuy_PRO_900Z, .exposes_count=sizeof(kExp_min_PRO_900Z_Tuy_PRO_900Z)/sizeof(kExp_min_PRO_900Z_Tuy_PRO_900Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PRO_900Z_Tuy_PRO_900Z, .from_zigbee_count=sizeof(kFz_min_PRO_900Z_Tuy_PRO_900Z)/sizeof(kFz_min_PRO_900Z_Tuy_PRO_900Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
