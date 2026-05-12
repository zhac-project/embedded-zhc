// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZBN-JT-63 — auto-generated.
// Din rail switch with power monitoring
// z2m-source: tuya.ts #ZBN-JT-63.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZBN_JT_63[] = { "TS0601" };
constexpr const char* kManus_ZBN_JT_63[] = { "_TZE204_jcwbwckh" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZBN_JT_63_Tuy_ZBN_JT_63[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZBN_JT_63_Tuy_ZBN_JT_63[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZBN_JT_63_Tuy_ZBN_JT_63[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZBN_JT_63{
    .zigbee_models=kModels_ZBN_JT_63, .zigbee_models_count=sizeof(kModels_ZBN_JT_63)/sizeof(kModels_ZBN_JT_63[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZBN_JT_63, .manufacturer_names_count=sizeof(kManus_ZBN_JT_63)/sizeof(kManus_ZBN_JT_63[0]),
    .model="ZBN-JT-63", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZBN_JT_63_Tuy_ZBN_JT_63, .exposes_count=sizeof(kExp_min_ZBN_JT_63_Tuy_ZBN_JT_63)/sizeof(kExp_min_ZBN_JT_63_Tuy_ZBN_JT_63[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZBN_JT_63_Tuy_ZBN_JT_63, .from_zigbee_count=sizeof(kFz_min_ZBN_JT_63_Tuy_ZBN_JT_63)/sizeof(kFz_min_ZBN_JT_63_Tuy_ZBN_JT_63[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
