// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ME201WZ — auto-generated.
// Water level sensor
// z2m-source: tuya.ts #ME201WZ.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ME201WZ[] = { "TS0601" };
constexpr const char* kManus_ME201WZ[] = { "_TZE284_kyyu8rbj", "_TZE204_7yyuo8sr" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ME201WZ_Tuy_ME201WZ[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ME201WZ_Tuy_ME201WZ[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ME201WZ_Tuy_ME201WZ[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ME201WZ{
    .zigbee_models=kModels_ME201WZ, .zigbee_models_count=sizeof(kModels_ME201WZ)/sizeof(kModels_ME201WZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ME201WZ, .manufacturer_names_count=sizeof(kManus_ME201WZ)/sizeof(kManus_ME201WZ[0]),
    .model="ME201WZ", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ME201WZ_Tuy_ME201WZ, .exposes_count=sizeof(kExp_min_ME201WZ_Tuy_ME201WZ)/sizeof(kExp_min_ME201WZ_Tuy_ME201WZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ME201WZ_Tuy_ME201WZ, .from_zigbee_count=sizeof(kFz_min_ME201WZ_Tuy_ME201WZ)/sizeof(kFz_min_ME201WZ_Tuy_ME201WZ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
