// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ysrsai YSR-MINI-01_wwcw — tunable-white (WW/CW) LED controller.
// Zigbee LED controller (WW/CW)
// z2m-source: ysrsai.ts #YSR-MINI-01_wwcw.
//
// z2m uses tuya.modernExtend.tuyaLight({colorTemp:{range:[153,500]}}),
// which builds on modernExtend.light() → a ZCL tunable-white light:
//   genOnOff(0x0006) + genLevelCtrl(0x0008) + lightingColorCtrl(0x0300)
//   → state + brightness + color_temp (mired 153..500). No x/y color.
// "tuya" only adds tuyaFz.brightness + a "Mains (single phase)" power
// source; it is NOT an 0xEF00 DP device and has NO battery.
// The auto-port mis-classified it as a battery sensor (kFzBattery + bare
// onOff) and dropped brightness + color_temp — fixed here to a real CCT
// light. z2m configure also seeds colorCapabilities=0x10 (CT only).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ysrsai {
namespace {


constexpr const char* kModels_YSR_MINI_01_wwcw[] = { "ZB-CT01" };

}  // namespace


namespace {
const FzConverter* const kFz_min_YSR_MINI_01_wwcw[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_min_YSR_MINI_01_wwcw[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr Expose kExp_min_YSR_MINI_01_wwcw[] = {
    { "state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr,   nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,   nullptr, nullptr, 0 },
    { "color_temp", ExposeType::Numeric, ::zhc::Access::StateSet, "mired",   nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_YSR_MINI_01_wwcw[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
    { 1, 0x0300 },   // lightingColorCtrl
};

constexpr ReportingSpec kReports_Ysr_wwcw[] = {
    // cluster 0x0006 genOnOff, attr 0x0000 onOff (bool, 0x10)
    { 1, 0x0006, 0x0000, 0x10, 0, 3600, 1, 0 },
    // cluster 0x0008 genLevelCtrl, attr 0x0000 currentLevel (u8, 0x20)
    { 1, 0x0008, 0x0000, 0x20, 5, 3600, 1, 0 },
    // cluster 0x0300 lightingColorCtrl, attr 0x0007 colorTemperature (u16, 0x21)
    { 1, 0x0300, 0x0007, 0x21, 5, 3600, 1, 0 },
};
}
extern const PreparedDefinition kDef_YSR_MINI_01_wwcw{
    .zigbee_models=kModels_YSR_MINI_01_wwcw, .zigbee_models_count=sizeof(kModels_YSR_MINI_01_wwcw)/sizeof(kModels_YSR_MINI_01_wwcw[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YSR-MINI-01_wwcw", .vendor="Ysrsai",
    .meta=nullptr, .exposes=kExp_min_YSR_MINI_01_wwcw, .exposes_count=sizeof(kExp_min_YSR_MINI_01_wwcw)/sizeof(kExp_min_YSR_MINI_01_wwcw[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_YSR_MINI_01_wwcw, .from_zigbee_count=sizeof(kFz_min_YSR_MINI_01_wwcw)/sizeof(kFz_min_YSR_MINI_01_wwcw[0]),
    .to_zigbee=kTz_min_YSR_MINI_01_wwcw, .to_zigbee_count=sizeof(kTz_min_YSR_MINI_01_wwcw)/sizeof(kTz_min_YSR_MINI_01_wwcw[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_YSR_MINI_01_wwcw,
    .bindings_count           = sizeof(kBind_min_YSR_MINI_01_wwcw)/sizeof(kBind_min_YSR_MINI_01_wwcw[0]),
    .reports                  = kReports_Ysr_wwcw,
    .reports_count            = sizeof(kReports_Ysr_wwcw)/sizeof(kReports_Ysr_wwcw[0]),
};

}  // namespace zhc::devices::ysrsai
