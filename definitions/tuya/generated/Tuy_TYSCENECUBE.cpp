// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TYSCENECUBE — auto-generated.
// Scene cube
// z2m-source: tuya.ts #TYSCENECUBE.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TYSCENECUBE[] = { "TS0601" };
constexpr const char* kManus_TYSCENECUBE[] = { "_TZE284_5ys44kzo" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TYSCENECUBE_Tuy_TYSCENECUBE[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TYSCENECUBE_Tuy_TYSCENECUBE[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TYSCENECUBE_Tuy_TYSCENECUBE[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TYSCENECUBE{
    .zigbee_models=kModels_TYSCENECUBE, .zigbee_models_count=sizeof(kModels_TYSCENECUBE)/sizeof(kModels_TYSCENECUBE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TYSCENECUBE, .manufacturer_names_count=sizeof(kManus_TYSCENECUBE)/sizeof(kManus_TYSCENECUBE[0]),
    .model="TYSCENECUBE", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TYSCENECUBE_Tuy_TYSCENECUBE, .exposes_count=sizeof(kExp_min_TYSCENECUBE_Tuy_TYSCENECUBE)/sizeof(kExp_min_TYSCENECUBE_Tuy_TYSCENECUBE[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TYSCENECUBE_Tuy_TYSCENECUBE, .from_zigbee_count=sizeof(kFz_min_TYSCENECUBE_Tuy_TYSCENECUBE)/sizeof(kFz_min_TYSCENECUBE_Tuy_TYSCENECUBE[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
