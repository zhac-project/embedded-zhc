// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ME202WZ — auto-generated.
// Water level sensor
// z2m-source: tuya.ts #ME202WZ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ME202WZ[] = { "TS0601" };
constexpr const char* kManus_ME202WZ[] = { "_TZE284_mxujdmxo" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ME202WZ_Tuy_ME202WZ[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ME202WZ_Tuy_ME202WZ[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ME202WZ_Tuy_ME202WZ[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ME202WZ{
    .zigbee_models=kModels_ME202WZ, .zigbee_models_count=sizeof(kModels_ME202WZ)/sizeof(kModels_ME202WZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ME202WZ, .manufacturer_names_count=sizeof(kManus_ME202WZ)/sizeof(kManus_ME202WZ[0]),
    .model="ME202WZ", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ME202WZ_Tuy_ME202WZ, .exposes_count=sizeof(kExp_min_ME202WZ_Tuy_ME202WZ)/sizeof(kExp_min_ME202WZ_Tuy_ME202WZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ME202WZ_Tuy_ME202WZ, .from_zigbee_count=sizeof(kFz_min_ME202WZ_Tuy_ME202WZ)/sizeof(kFz_min_ME202WZ_Tuy_ME202WZ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
