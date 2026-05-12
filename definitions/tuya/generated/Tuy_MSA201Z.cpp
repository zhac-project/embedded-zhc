// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya MSA201Z — auto-generated.
// 24 GHz human presence sensor
// z2m-source: tuya.ts #MSA201Z.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_MSA201Z[] = { "TS0601" };
constexpr const char* kManus_MSA201Z[] = { "_TZE284_ajuasrmx", "_TZE200_hyhl5y36", "_TZE284_ozf4e02o" };
}  // namespace

namespace {
const FzConverter* const kFz_min_MSA201Z_Tuy_MSA201Z[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_MSA201Z_Tuy_MSA201Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_MSA201Z_Tuy_MSA201Z[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_MSA201Z{
    .zigbee_models=kModels_MSA201Z, .zigbee_models_count=sizeof(kModels_MSA201Z)/sizeof(kModels_MSA201Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_MSA201Z, .manufacturer_names_count=sizeof(kManus_MSA201Z)/sizeof(kManus_MSA201Z[0]),
    .model="MSA201Z", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_MSA201Z_Tuy_MSA201Z, .exposes_count=sizeof(kExp_min_MSA201Z_Tuy_MSA201Z)/sizeof(kExp_min_MSA201Z_Tuy_MSA201Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_MSA201Z_Tuy_MSA201Z, .from_zigbee_count=sizeof(kFz_min_MSA201Z_Tuy_MSA201Z)/sizeof(kFz_min_MSA201Z_Tuy_MSA201Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
