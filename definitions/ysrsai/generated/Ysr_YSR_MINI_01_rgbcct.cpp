// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ysrsai YSR-MINI-01_rgbcct — auto-generated.
// Zigbee LED controller (RGB+CCT)
// z2m-source: ysrsai.ts #YSR-MINI-01_rgbcct.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ysrsai {
namespace {


// Listed with a manufacturer filter so only YSRSAI-branded devices
// match. Kurvia-branded ZB-CL01 falls through to the Kurvia
// definition in Pass 2 (non-Tuya, no manu filter).
constexpr const char* kModels_YSR_MINI_01_rgbcct[] = { "ZB-CL03", "FB56-ZCW20FB1.2", "ZB-CL01", "TS0503B" };
constexpr const char* kManus_YSR_MINI_01_rgbcct[]  = {
    "YSRSAI", "_TZ3210_f0byevky",
};

}  // namespace


namespace {
// z2m: tuya.modernExtend.tuyaLight({colorTemp:[160,370], color:true})
// → state, brightness, color_temp (mired 160..370), color (x/y).
// Mains-powered LED controller: no genPowerCfg, no battery/voltage.
constexpr const char* kColorMode_Light[] = { "hs", "xy", "color_temp" };

const FzConverter* const kFz_min_YSR_MINI_01_rgbcct[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,             // currentX / currentY / colorMode
    &::zhc::generic::kFzPowerOnBehavior,   // startUpOnOff on genOnOff
};
const TzConverter* const kTz_min_YSR_MINI_01_rgbcct[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    // tz for XY color / power_on_behavior not yet in the library —
    // fz lands the reading, write-back is follow-up.
};
constexpr Expose kExp_min_YSR_MINI_01_rgbcct[] = {
    { "state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ExposeType::Numeric, ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "color_y",    ExposeType::Numeric, ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "color_mode", ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr,
      kColorMode_Light, sizeof(kColorMode_Light)/sizeof(kColorMode_Light[0]) },
    { "power_on_behavior", ExposeType::Enum, ::zhc::Access::State, nullptr, nullptr,
      nullptr, 0, ::zhc::ExposeCategory::Config },
};
constexpr BindingSpec kBind_min_YSR_MINI_01_rgbcct[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
    { 1, 0x0300 },   // lightingColorCtrl
};

// Configure-reporting — tell the device to emit attr reports on
// change. Tuya LED drivers often stay silent on command-driven
// state changes without this, which means the UI StatesTab never
// updates until the next full read.
constexpr ReportingSpec kReports_Ysr[] = {
    // cluster 0x0006 genOnOff, attr 0x0000 onOff (bool, 0x10)
    { 1, 0x0006, 0x0000, 0x10, 0,    3600, 1, 0 },
    // cluster 0x0008 genLevelCtrl, attr 0x0000 currentLevel (u8, 0x20)
    { 1, 0x0008, 0x0000, 0x20, 5,    3600, 1, 0 },
    // cluster 0x0300 lightingColorCtrl, attr 0x0007 colorTemperature (u16, 0x21)
    { 1, 0x0300, 0x0007, 0x21, 5,    3600, 1, 0 },
};

// Initial-state read after bindings. Without this the device sends no
// reports until the user physically toggles the bulb — States tab sits
// empty. Mirrors z2m's m.light() "read initial values" step.
//   genOnOff(0x0006) attr 0x0000 onOff
//                    attr 0x4003 startUpOnOff (→ power_on_behavior)
//   genLevelCtrl(0x0008) attr 0x0000 currentLevel
//   lightingColorCtrl(0x0300) attrs 0x0003/0x0004/0x0007/0x0008
//     (currentX / currentY / colorTemperature / colorMode)
constexpr std::uint8_t kReadOnOff_Ysr[]  = {
    0x00, 0x00,        // onOff
    0x03, 0x40,        // startUpOnOff (0x4003)
};
constexpr std::uint8_t kReadLevel_Ysr[]  = { 0x00, 0x00 };   // currentLevel
constexpr std::uint8_t kReadColor_Ysr[]  = {
    0x03, 0x00,        // currentX
    0x04, 0x00,        // currentY
    0x07, 0x00,        // colorTemperature
    0x08, 0x00,        // colorMode
};
constexpr ConfigStep kConfig_Ysr[] = {
    { ConfigStepOp::Wait, 0, 0,      0x00, 0, nullptr, 0, 300  },
    { ConfigStepOp::Read, 1, 0x0006, 0x00, 0, kReadOnOff_Ysr,
      sizeof(kReadOnOff_Ysr), 1500 },
    { ConfigStepOp::Read, 1, 0x0008, 0x00, 0, kReadLevel_Ysr,
      sizeof(kReadLevel_Ysr), 1500 },
    { ConfigStepOp::Read, 1, 0x0300, 0x00, 0, kReadColor_Ysr,
      sizeof(kReadColor_Ysr), 1500 },
};
}
extern const PreparedDefinition kDef_YSR_MINI_01_rgbcct{
    .zigbee_models=kModels_YSR_MINI_01_rgbcct, .zigbee_models_count=sizeof(kModels_YSR_MINI_01_rgbcct)/sizeof(kModels_YSR_MINI_01_rgbcct[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_YSR_MINI_01_rgbcct,
    .manufacturer_names_count=sizeof(kManus_YSR_MINI_01_rgbcct)/sizeof(kManus_YSR_MINI_01_rgbcct[0]),
    .model="YSR-MINI-01_rgbcct", .vendor="Ysrsai",
    .meta=nullptr, .exposes=kExp_min_YSR_MINI_01_rgbcct, .exposes_count=sizeof(kExp_min_YSR_MINI_01_rgbcct)/sizeof(kExp_min_YSR_MINI_01_rgbcct[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_YSR_MINI_01_rgbcct, .from_zigbee_count=sizeof(kFz_min_YSR_MINI_01_rgbcct)/sizeof(kFz_min_YSR_MINI_01_rgbcct[0]),
    .to_zigbee=kTz_min_YSR_MINI_01_rgbcct, .to_zigbee_count=sizeof(kTz_min_YSR_MINI_01_rgbcct)/sizeof(kTz_min_YSR_MINI_01_rgbcct[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_YSR_MINI_01_rgbcct,
    .bindings_count           = sizeof(kBind_min_YSR_MINI_01_rgbcct)/sizeof(kBind_min_YSR_MINI_01_rgbcct[0]),
    .reports                  = kReports_Ysr,
    .reports_count            = sizeof(kReports_Ysr)/sizeof(kReports_Ysr[0]),
    .config_steps             = kConfig_Ysr,
    .config_steps_count       = sizeof(kConfig_Ysr)/sizeof(kConfig_Ysr[0]),
};

}  // namespace zhc::devices::ysrsai
