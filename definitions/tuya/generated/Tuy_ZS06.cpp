// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZS06 — auto-generated.
// Universal smart IR remote control
// z2m-source: tuya.ts #ZS06.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZS06[] = { "TS1201" };
constexpr const char* kManus_ZS06[] = { "_TZ3290_7v1k4vufotpowp9z", "_TZ3290_rlkmy85q4pzoxobl", "_TZ3290_jxvzqatwgsaqzx1u", "_TZ3290_lypnqvlem5eq1ree", "_TZ3290_uc8lwbi2", "_TZ3290_nba3knpsarkawgnt", "_TZ3290_8xzb2ghn", "_TZ3290_s6ezpa3j" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZS06_Tuy_ZS06[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZS06_Tuy_ZS06[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZS06_Tuy_ZS06[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZS06{
    .zigbee_models=kModels_ZS06, .zigbee_models_count=sizeof(kModels_ZS06)/sizeof(kModels_ZS06[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZS06, .manufacturer_names_count=sizeof(kManus_ZS06)/sizeof(kManus_ZS06[0]),
    .model="ZS06", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZS06_Tuy_ZS06, .exposes_count=sizeof(kExp_min_ZS06_Tuy_ZS06)/sizeof(kExp_min_ZS06_Tuy_ZS06[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZS06_Tuy_ZS06, .from_zigbee_count=sizeof(kFz_min_ZS06_Tuy_ZS06)/sizeof(kFz_min_ZS06_Tuy_ZS06[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
