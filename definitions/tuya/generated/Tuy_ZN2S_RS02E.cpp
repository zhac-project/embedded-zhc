// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZN2S-RS02E — auto-generated.
// Two gang switch with colored backlight modes
// z2m-source: tuya.ts #ZN2S-RS02E.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZN2S_RS02E[] = { "TS0601" };
constexpr const char* kManus_ZN2S_RS02E[] = { "_TZE284_zpvusbtv" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZN2S_RS02E_Tuy_ZN2S_RS02E[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZN2S_RS02E_Tuy_ZN2S_RS02E[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZN2S_RS02E_Tuy_ZN2S_RS02E[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZN2S_RS02E{
    .zigbee_models=kModels_ZN2S_RS02E, .zigbee_models_count=sizeof(kModels_ZN2S_RS02E)/sizeof(kModels_ZN2S_RS02E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZN2S_RS02E, .manufacturer_names_count=sizeof(kManus_ZN2S_RS02E)/sizeof(kManus_ZN2S_RS02E[0]),
    .model="ZN2S-RS02E", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZN2S_RS02E_Tuy_ZN2S_RS02E, .exposes_count=sizeof(kExp_min_ZN2S_RS02E_Tuy_ZN2S_RS02E)/sizeof(kExp_min_ZN2S_RS02E_Tuy_ZN2S_RS02E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZN2S_RS02E_Tuy_ZN2S_RS02E, .from_zigbee_count=sizeof(kFz_min_ZN2S_RS02E_Tuy_ZN2S_RS02E)/sizeof(kFz_min_ZN2S_RS02E_Tuy_ZN2S_RS02E[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
