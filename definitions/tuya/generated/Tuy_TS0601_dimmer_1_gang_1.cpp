// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0601_dimmer_1_gang_1 — auto-generated.
// 1 gang smart dimmer
// z2m-source: tuya.ts #TS0601_dimmer_1_gang_1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TS0601_dimmer_1_gang_1[] = { "TS0601" };
constexpr const char* kManus_TS0601_dimmer_1_gang_1[] = { "_TZE200_ip2akl4w", "_TZE200_1agwnems", "_TZE200_la2c2uo9", "_TZE200_579lguh2", "_TZE200_vucankjx", "_TZE200_4mh6tyyo", "_TZE204_hlx9tnzb", "_TZE204_n9ctkb6j", "_TZE204_9qhuzgo0", "_TZE200_9cxuhakf", "_TZE200_a0syesf5", "_TZE200_3p5ydos3", "_TZE200_swaamsoy", "_TZE200_ojzhk75b", "_TZE200_w4cryh2i", "_TZE200_dfxkcots", "_TZE204_68utemio", "_TZE200_9i9dt8is", "_TZE200_ctq0k47x", "_TZE200_ebwgzdqq", "_TZE204_vevc4c6g", "_TZE200_0nauxa0p", "_TZE200_ykgar0ow", "_TZE284_m1cvyneb", "_TZE200_0hb4rdnp", "_TZE200_gne0e6mk", "_TZE284_68utemio" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TS0601_dimmer_1_gang_1{
    .zigbee_models=kModels_TS0601_dimmer_1_gang_1, .zigbee_models_count=sizeof(kModels_TS0601_dimmer_1_gang_1)/sizeof(kModels_TS0601_dimmer_1_gang_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TS0601_dimmer_1_gang_1, .manufacturer_names_count=sizeof(kManus_TS0601_dimmer_1_gang_1)/sizeof(kManus_TS0601_dimmer_1_gang_1[0]),
    .model="TS0601_dimmer_1_gang_1", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1, .exposes_count=sizeof(kExp_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1)/sizeof(kExp_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1, .from_zigbee_count=sizeof(kFz_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1)/sizeof(kFz_min_TS0601_dimmer_1_gang_1_Tuy_TS0601_dimmer_1_gang_1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
