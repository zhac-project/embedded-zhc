// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ysrsai YSR-MINI-01_dimmer — dimmable LED controller.
// Zigbee LED controller (Dimmer)
// z2m-source: ysrsai.ts #YSR-MINI-01_dimmer.
//
// z2m uses tuya.modernExtend.tuyaLight({configureReporting:true}), which
// builds on modernExtend.light() → a plain ZCL dimmable light:
//   genOnOff(0x0006) + genLevelCtrl(0x0008) → state + brightness.
// "tuya" in the name only adds tuyaFz.brightness + a "Mains (single
// phase)" power source; it is NOT an 0xEF00 DP device and has NO battery.
// The auto-port mis-classified it as a battery sensor (kFzBattery + bare
// onOff) and dropped brightness entirely — fixed here to a real light.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ysrsai {
namespace {


constexpr const char* kModels_YSR_MINI_01_dimmer[] = { "ZB-DL01" };

}  // namespace


namespace {
const FzConverter* const kFz_min_YSR_MINI_01_dimmer[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_min_YSR_MINI_01_dimmer[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr Expose kExp_min_YSR_MINI_01_dimmer[] = {
    { "state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_YSR_MINI_01_dimmer[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};

// configureReporting:true in z2m → emit attr reports on change so the
// States tab updates without a manual read after each command.
constexpr ReportingSpec kReports_Ysr_dimmer[] = {
    // cluster 0x0006 genOnOff, attr 0x0000 onOff (bool, 0x10)
    { 1, 0x0006, 0x0000, 0x10, 0, 3600, 1, 0 },
    // cluster 0x0008 genLevelCtrl, attr 0x0000 currentLevel (u8, 0x20)
    { 1, 0x0008, 0x0000, 0x20, 5, 3600, 1, 0 },
};
}
extern const PreparedDefinition kDef_YSR_MINI_01_dimmer{
    .zigbee_models=kModels_YSR_MINI_01_dimmer, .zigbee_models_count=sizeof(kModels_YSR_MINI_01_dimmer)/sizeof(kModels_YSR_MINI_01_dimmer[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YSR-MINI-01_dimmer", .vendor="Ysrsai",
    .meta=nullptr, .exposes=kExp_min_YSR_MINI_01_dimmer, .exposes_count=sizeof(kExp_min_YSR_MINI_01_dimmer)/sizeof(kExp_min_YSR_MINI_01_dimmer[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_YSR_MINI_01_dimmer, .from_zigbee_count=sizeof(kFz_min_YSR_MINI_01_dimmer)/sizeof(kFz_min_YSR_MINI_01_dimmer[0]),
    .to_zigbee=kTz_min_YSR_MINI_01_dimmer, .to_zigbee_count=sizeof(kTz_min_YSR_MINI_01_dimmer)/sizeof(kTz_min_YSR_MINI_01_dimmer[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_YSR_MINI_01_dimmer,
    .bindings_count           = sizeof(kBind_min_YSR_MINI_01_dimmer)/sizeof(kBind_min_YSR_MINI_01_dimmer[0]),
    .reports                  = kReports_Ysr_dimmer,
    .reports_count            = sizeof(kReports_Ysr_dimmer)/sizeof(kReports_Ysr_dimmer[0]),
};

}  // namespace zhc::devices::ysrsai
