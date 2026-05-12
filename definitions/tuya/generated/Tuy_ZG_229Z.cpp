// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZG-229Z — auto-generated.
// Smart light & sound siren
// z2m-source: tuya.ts #ZG-229Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZG_229Z[] = { "ZG-229Z" };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZG_229Z_Tuy_ZG_229Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZG_229Z_Tuy_ZG_229Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZG_229Z_Tuy_ZG_229Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZG_229Z{
    .zigbee_models=kModels_ZG_229Z, .zigbee_models_count=sizeof(kModels_ZG_229Z)/sizeof(kModels_ZG_229Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-229Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZG_229Z_Tuy_ZG_229Z, .exposes_count=sizeof(kExp_min_ZG_229Z_Tuy_ZG_229Z)/sizeof(kExp_min_ZG_229Z_Tuy_ZG_229Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZG_229Z_Tuy_ZG_229Z, .from_zigbee_count=sizeof(kFz_min_ZG_229Z_Tuy_ZG_229Z)/sizeof(kFz_min_ZG_229Z_Tuy_ZG_229Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
