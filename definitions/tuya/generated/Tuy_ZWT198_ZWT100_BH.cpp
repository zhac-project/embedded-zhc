// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZWT198/ZWT100-BH — auto-generated.
// Wall thermostat
// z2m-source: tuya.ts #ZWT198/ZWT100-BH.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZWT198_ZWT100_BH[] = { "TS0601" };
constexpr const char* kManus_ZWT198_ZWT100_BH[] = { "_TZE200_viy9ihs7", "_TZE204_lzriup1j", "_TZE204_xnbkhhdr", "_TZE284_xnbkhhdr", "_TZE204_oh8y8pv8", "_TZE204_gops3slb", "_TZE284_gops3slb", "_TZE284_zjhoqbrd", "_TZE204_zjhoqbrd", "_TZE284_aaeaifez" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZWT198_ZWT100_BH{
    .zigbee_models=kModels_ZWT198_ZWT100_BH, .zigbee_models_count=sizeof(kModels_ZWT198_ZWT100_BH)/sizeof(kModels_ZWT198_ZWT100_BH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZWT198_ZWT100_BH, .manufacturer_names_count=sizeof(kManus_ZWT198_ZWT100_BH)/sizeof(kManus_ZWT198_ZWT100_BH[0]),
    .model="ZWT198/ZWT100-BH", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH, .exposes_count=sizeof(kExp_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH)/sizeof(kExp_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH, .from_zigbee_count=sizeof(kFz_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH)/sizeof(kFz_min_ZWT198_ZWT100_BH_Tuy_ZWT198_ZWT100_BH[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
