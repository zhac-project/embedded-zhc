// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya BOT-R15W — auto-generated.
// Beok wall thermostat (battery powered)
// z2m-source: tuya.ts #BOT-R15W.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_BOT_R15W[] = { "TS0601" };
constexpr const char* kManus_BOT_R15W[] = { "_TZE284_agcxaw3f" };
}  // namespace

namespace {
const FzConverter* const kFz_min_BOT_R15W_Tuy_BOT_R15W[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_BOT_R15W_Tuy_BOT_R15W[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_BOT_R15W_Tuy_BOT_R15W[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_BOT_R15W{
    .zigbee_models=kModels_BOT_R15W, .zigbee_models_count=sizeof(kModels_BOT_R15W)/sizeof(kModels_BOT_R15W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_BOT_R15W, .manufacturer_names_count=sizeof(kManus_BOT_R15W)/sizeof(kManus_BOT_R15W[0]),
    .model="BOT-R15W", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_BOT_R15W_Tuy_BOT_R15W, .exposes_count=sizeof(kExp_min_BOT_R15W_Tuy_BOT_R15W)/sizeof(kExp_min_BOT_R15W_Tuy_BOT_R15W[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_BOT_R15W_Tuy_BOT_R15W, .from_zigbee_count=sizeof(kFz_min_BOT_R15W_Tuy_BOT_R15W)/sizeof(kFz_min_BOT_R15W_Tuy_BOT_R15W[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
