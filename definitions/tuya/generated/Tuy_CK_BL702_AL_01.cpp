// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya CK-BL702-AL-01 — auto-generated.
// Zigbee LED bulb
// z2m-source: tuya.ts #CK-BL702-AL-01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_CK_BL702_AL_01[] = { "CK-BL702-AL-01(7009_Z102LG03-1)", "CK-BL702-AL-01(7009_Z102LG04-1)", "CK-BL702-AL-01(7009_Z102LG04-2)" };

}  // namespace

namespace {
const FzConverter* const kFz_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_CK_BL702_AL_01{
    .zigbee_models=kModels_CK_BL702_AL_01, .zigbee_models_count=sizeof(kModels_CK_BL702_AL_01)/sizeof(kModels_CK_BL702_AL_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CK-BL702-AL-01", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01, .exposes_count=sizeof(kExp_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01)/sizeof(kExp_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01, .from_zigbee_count=sizeof(kFz_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01)/sizeof(kFz_min_CK_BL702_AL_01_Tuy_CK_BL702_AL_01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
