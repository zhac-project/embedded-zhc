// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya _TZE204_w2vunxzm — auto-generated.
// Air pressure sensor
// z2m-source: tuya.ts #_TZE204_w2vunxzm.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels__TZE204_w2vunxzm[] = { "TS0601" };
constexpr const char* kManus__TZE204_w2vunxzm[] = { "_TZE204_w2vunxzm" };
}  // namespace

namespace {
const FzConverter* const kFz_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef__TZE204_w2vunxzm{
    .zigbee_models=kModels__TZE204_w2vunxzm, .zigbee_models_count=sizeof(kModels__TZE204_w2vunxzm)/sizeof(kModels__TZE204_w2vunxzm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus__TZE204_w2vunxzm, .manufacturer_names_count=sizeof(kManus__TZE204_w2vunxzm)/sizeof(kManus__TZE204_w2vunxzm[0]),
    .model="_TZE204_w2vunxzm", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm, .exposes_count=sizeof(kExp_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm)/sizeof(kExp_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm, .from_zigbee_count=sizeof(kFz_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm)/sizeof(kFz_min__TZE204_w2vunxzm_Tuy__TZE204_w2vunxzm[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
