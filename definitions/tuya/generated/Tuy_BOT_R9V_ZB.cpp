// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya BOT-R9V-ZB — auto-generated.
// Wall-mount thermostat
// z2m-source: tuya.ts #BOT-R9V-ZB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_BOT_R9V_ZB[] = { "TS0601" };
constexpr const char* kManus_BOT_R9V_ZB[] = { "_TZE204_wc2w9t1s" };
}  // namespace

namespace {
const FzConverter* const kFz_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_BOT_R9V_ZB{
    .zigbee_models=kModels_BOT_R9V_ZB, .zigbee_models_count=sizeof(kModels_BOT_R9V_ZB)/sizeof(kModels_BOT_R9V_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BOT_R9V_ZB, .manufacturer_names_count=sizeof(kManus_BOT_R9V_ZB)/sizeof(kManus_BOT_R9V_ZB[0]),
    .model="BOT-R9V-ZB", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB, .exposes_count=sizeof(kExp_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB)/sizeof(kExp_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB, .from_zigbee_count=sizeof(kFz_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB)/sizeof(kFz_min_BOT_R9V_ZB_Tuy_BOT_R9V_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
