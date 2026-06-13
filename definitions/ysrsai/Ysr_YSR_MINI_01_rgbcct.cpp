// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ysrsai YSR-MINI-01_rgbcct — graduated from generated/ to carry
//         the tuyaLight write-only control parity (effect / do_not_disturb
//         / color_power_on_behavior).
// Zigbee LED controller (RGB+CCT)
// z2m-source: ysrsai.ts #YSR-MINI-01_rgbcct.
//
// z2m: tuya.modernExtend.tuyaLight({colorTemp:{range:[160,370]}, color:true}).
// Beyond the standard light (state/brightness/color_temp/color x-y), the
// `tuyaLight` modern-extend layers on three WRITE-ONLY controls that the
// Tier-1 auto-port dropped, and — importantly — sets `powerOnBehavior:false`,
// i.e. it does NOT use the generic genOnOff `startUpOnOff` (0x4003)
// power-on. It uses Tuya's own `color_power_on_behavior` command instead.
// So this file:
//   - removes the (wrong-for-this-device) generic `power_on_behavior`
//     expose + `kFzPowerOnBehavior` + the 0x4003 initial read, and
//   - adds three vendor-local write-only command converters:
//       * color_power_on_behavior — lightingColorCtrl(0x0300) cmd 0xf9
//                                   (tuyaOnStartUp): mode u16 LE (= v*256)
//                                   + 10 zero bytes.
//       * do_not_disturb          — lightingColorCtrl(0x0300) cmd 0xfa
//                                   (tuyaDoNotDisturb): 1-byte enable.
//       * effect                  — genIdentify(0x0003) cmd 0x40
//                                   (triggerEffect): [effect_id, variant=0]
//                                   for the 6 standard effects, PLUS
//                                   lightingColorCtrl(0x0300) cmd 0x44
//                                   (colorLoopSet) for colorloop /
//                                   stop_colorloop. Two converters share the
//                                   "effect" key; the dispatcher tries the
//                                   genIdentify one first and falls through
//                                   to the colorLoopSet one (it returns false
//                                   for the two colour-loop labels).
// All three carry no fz (write-only): the device echoes no readable state
// for them, so the SPA renders optimistic state only. (This file is
// WRITE-only control parity; it does not fix the device's missing state
// values, which is a separate rejoin-path issue.)
#include <cstdint>
#include <cstring>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::ysrsai {

using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::TzConverter;
using ::zhc::Value;
using ::zhc::ValueType;

namespace {


// Listed with a manufacturer filter so only YSRSAI-branded devices
// match. Kurvia-branded ZB-CL01 falls through to the Kurvia
// definition in Pass 2 (non-Tuya, no manu filter).
constexpr const char* kModels_YSR_MINI_01_rgbcct[] = { "ZB-CL03", "FB56-ZCW20FB1.2", "ZB-CL01", "TS0503B" };
constexpr const char* kManus_YSR_MINI_01_rgbcct[]  = {
    "YSRSAI", "_TZ3210_f0byevky",
};

}  // namespace


// ── Write-only command encoders (tuyaLight control parity) ───────────
//
// All three are CLUSTER-SPECIFIC commands (not attribute writes, not
// Tuya 0xEF00 DPs) sent profile-wide (no manufacturer code) — exactly
// as z2m's `tuya.modernExtend.tuyaOnStartUp` / `tuyaDoNotDisturb` and
// the standard `triggerEffect` / `colorLoopSet` do. The shared
// `_generic` `write_header()` (FC=0x11 cluster-specific, default-response
// suppressed) is file-private, so a tiny local header writer mirrors it.
namespace {

// ZCL frame-control byte for a cluster-specific c→s command with the
// default-response bit suppressed. Matches z2m + the _generic encoders.
constexpr std::uint8_t kCmdFc_Ysr = 0x11;

// Write [FC, TSN=0, cmd_id] into `out` and report total size. TSN is
// patched by the platform adapter. Returns false if `out` is too small.
bool ysr_write_header(std::span<std::uint8_t> out, std::uint8_t cmd_id,
                      std::size_t payload_len, std::size_t& out_size) {
    const std::size_t total = 3 + payload_len;
    if (out.size() < total) return false;
    out[0] = kCmdFc_Ysr;
    out[1] = 0x00;        // TSN — platform adapter patches.
    out[2] = cmd_id;
    out_size = total;
    return true;
}

// Resolve an enum-style Value to its ordinal. Accepts Uint/Int (already
// an index) or a StringRef label matched against `labels`. Returns false
// on type mismatch / unknown label / out-of-range index.
bool ysr_enum_index(const Value& v, const char* const* labels,
                    std::size_t label_count, std::uint8_t& out) {
    if (v.type == ValueType::Uint) {
        if (v.u >= label_count) return false;
        out = static_cast<std::uint8_t>(v.u);
        return true;
    }
    if (v.type == ValueType::Int) {
        if (v.i < 0 || static_cast<std::size_t>(v.i) >= label_count) return false;
        out = static_cast<std::uint8_t>(v.i);
        return true;
    }
    if (v.type == ValueType::StringRef && v.str) {
        for (std::size_t i = 0; i < label_count; ++i) {
            if (std::strcmp(v.str, labels[i]) == 0) {
                out = static_cast<std::uint8_t>(i);
                return true;
            }
        }
    }
    return false;
}

// ── color_power_on_behavior → lightingColorCtrl 0x0300 cmd 0xf9 ──────
// z2m `tuya.modernExtend.tuyaOnStartUp`: enum {initial,previous,customized}
// → 0/1/2. Payload = mode (u16 LE) = value*256, then 10 bytes of 0x00.
constexpr const char* kColorPowerOnLabels[] = { "initial", "previous", "customized" };

bool tz_color_power_on(std::string_view key, const Value& input,
                       const TzConverter&,
                       const PreparedDefinition&, RuntimeContext&,
                       std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "color_power_on_behavior") return false;
    std::uint8_t idx = 0;
    if (!ysr_enum_index(input, kColorPowerOnLabels,
                        sizeof(kColorPowerOnLabels) / sizeof(kColorPowerOnLabels[0]),
                        idx)) {
        return false;
    }
    // mode (u16 LE) = value*256, then `data` = 10 zero bytes → 12-byte body.
    if (!ysr_write_header(out_frame, 0xf9, /*payload_len=*/12, out_size)) return false;
    const std::uint16_t mode = static_cast<std::uint16_t>(idx) * 256u;
    out_frame[3] = static_cast<std::uint8_t>(mode & 0xFF);
    out_frame[4] = static_cast<std::uint8_t>((mode >> 8) & 0xFF);
    for (std::size_t i = 5; i < 15; ++i) out_frame[i] = 0x00;
    return true;
}

extern const TzConverter kTzColorPowerOn{
    .key         = "color_power_on_behavior",
    .cluster     = "lightingColorCtrl",
    .cluster_id  = 0x0300,
    .command_id  = 0xf9,        // tuyaOnStartUp
    .fn          = tz_color_power_on,
    .user_config = nullptr,
};

// ── do_not_disturb → lightingColorCtrl 0x0300 cmd 0xfa ───────────────
// z2m `tuya.modernExtend.tuyaDoNotDisturb`: binary → 1-byte enable.
bool tz_do_not_disturb(std::string_view key, const Value& input,
                       const TzConverter&,
                       const PreparedDefinition&, RuntimeContext&,
                       std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "do_not_disturb") return false;
    std::uint8_t enable;
    if (input.type == ValueType::Bool) {
        enable = input.b ? 0x01 : 0x00;
    } else if (input.type == ValueType::Uint) {
        enable = input.u ? 0x01 : 0x00;
    } else if (input.type == ValueType::Int) {
        enable = (input.i != 0) ? 0x01 : 0x00;
    } else if (input.type == ValueType::StringRef && input.str) {
        if      (std::strcmp(input.str, "ON")  == 0 || std::strcmp(input.str, "true")  == 0) enable = 0x01;
        else if (std::strcmp(input.str, "OFF") == 0 || std::strcmp(input.str, "false") == 0) enable = 0x00;
        else return false;
    } else {
        return false;
    }
    if (!ysr_write_header(out_frame, 0xfa, /*payload_len=*/1, out_size)) return false;
    out_frame[3] = enable;
    return true;
}

extern const TzConverter kTzDoNotDisturb{
    .key         = "do_not_disturb",
    .cluster     = "lightingColorCtrl",
    .cluster_id  = 0x0300,
    .command_id  = 0xfa,        // tuyaDoNotDisturb
    .fn          = tz_do_not_disturb,
    .user_config = nullptr,
};

// ── effect → genIdentify 0x0003 cmd 0x40 (triggerEffect) ─────────────
// 6 standard effects. Payload = [effect_id u8, variant u8 = 0x00].
// colorloop / stop_colorloop are handled by the second converter below
// (different cluster — a TzConverter is locked to one cluster_id).
struct EffectId { const char* label; std::uint8_t id; };
constexpr EffectId kIdentifyEffects[] = {
    { "blink",          0x00 },
    { "breathe",        0x01 },
    { "okay",           0x02 },
    { "channel_change", 0x0b },
    { "finish_effect",  0xfe },
    { "stop_effect",    0xff },
};

bool tz_effect_identify(std::string_view key, const Value& input,
                        const TzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "effect") return false;
    if (input.type != ValueType::StringRef || !input.str) return false;
    for (const auto& e : kIdentifyEffects) {
        if (std::strcmp(input.str, e.label) == 0) {
            // triggerEffect (0x40): effect_id u8, variant u8 = 0x00.
            if (!ysr_write_header(out_frame, 0x40, /*payload_len=*/2, out_size)) return false;
            out_frame[3] = e.id;
            out_frame[4] = 0x00;     // variant
            return true;
        }
    }
    return false;   // not an identify effect — let colorLoopSet try.
}

extern const TzConverter kTzEffectIdentify{
    .key         = "effect",
    .cluster     = "genIdentify",
    .cluster_id  = 0x0003,
    .command_id  = 0x40,        // triggerEffect
    .fn          = tz_effect_identify,
    .user_config = nullptr,
};

// ── effect (colorloop) → lightingColorCtrl 0x0300 cmd 0x44 ───────────
// z2m colorloop via `colorLoopSet`:
//   colorloop:      [updateFlags=0x06, action=0x01, direction=0x01,
//                    time(u16 LE)=0x0001, startHue(u16 LE)=0x0000]
//   stop_colorloop: [updateFlags=0x01, action=0x00, direction=0x00,
//                    time=0x0000, startHue=0x0000]
bool tz_effect_colorloop(std::string_view key, const Value& input,
                         const TzConverter&,
                         const PreparedDefinition&, RuntimeContext&,
                         std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "effect") return false;
    if (input.type != ValueType::StringRef || !input.str) return false;

    std::uint8_t flags, action, dir;
    std::uint16_t time;
    if (std::strcmp(input.str, "colorloop") == 0) {
        flags = 0x06; action = 0x01; dir = 0x01; time = 0x0001;
    } else if (std::strcmp(input.str, "stop_colorloop") == 0) {
        flags = 0x01; action = 0x00; dir = 0x00; time = 0x0000;
    } else {
        return false;
    }
    // colorLoopSet (0x44): updateFlags u8, action u8, direction u8,
    // time u16 LE, startHue u16 LE → 7-byte body.
    if (!ysr_write_header(out_frame, 0x44, /*payload_len=*/7, out_size)) return false;
    out_frame[3] = flags;
    out_frame[4] = action;
    out_frame[5] = dir;
    out_frame[6] = static_cast<std::uint8_t>(time & 0xFF);
    out_frame[7] = static_cast<std::uint8_t>((time >> 8) & 0xFF);
    out_frame[8] = 0x00;     // startHue LSB
    out_frame[9] = 0x00;     // startHue MSB
    return true;
}

extern const TzConverter kTzEffectColorLoop{
    .key         = "effect",
    .cluster     = "lightingColorCtrl",
    .cluster_id  = 0x0300,
    .command_id  = 0x44,        // colorLoopSet
    .fn          = tz_effect_colorloop,
    .user_config = nullptr,
};

}  // namespace


namespace {
// → state, brightness, color_temp (mired 160..370), color (x/y).
// Mains-powered LED controller: no genPowerCfg, no battery/voltage.
constexpr const char* kColorMode_Light[] = { "hs", "xy", "color_temp" };

// Write-only command enums surfaced to the SPA.
constexpr const char* kEffectLabels[] = {
    "blink", "breathe", "okay", "channel_change", "finish_effect",
    "stop_effect", "colorloop", "stop_colorloop",
};

const FzConverter* const kFz_min_YSR_MINI_01_rgbcct[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,             // currentX / currentY / colorMode
    // No kFzPowerOnBehavior: z2m's tuyaLight sets powerOnBehavior:false —
    // this device uses color_power_on_behavior (cmd 0xf9), not the
    // generic genOnOff startUpOnOff (0x4003).
};
const TzConverter* const kTz_min_YSR_MINI_01_rgbcct[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    // Write-only tuyaLight control parity (vendor-local, this file):
    &kTzColorPowerOn,      // color_power_on_behavior — 0x0300 cmd 0xf9
    &kTzDoNotDisturb,      // do_not_disturb          — 0x0300 cmd 0xfa
    &kTzEffectIdentify,    // effect (6 std)          — 0x0003 cmd 0x40
    &kTzEffectColorLoop,   // effect (colour loop)    — 0x0300 cmd 0x44
    // tz for XY color write-back not yet in the library — fz lands the
    // reading, write-back is follow-up.
};
constexpr Expose kExp_min_YSR_MINI_01_rgbcct[] = {
    { "state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp", ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
    { "color_x",    ExposeType::Numeric, ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "color_y",    ExposeType::Numeric, ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "color_mode", ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr,
      kColorMode_Light, sizeof(kColorMode_Light)/sizeof(kColorMode_Light[0]) },
    // ── tuyaLight write-only control parity ──────────────────────────
    // `effect` is a momentary command trigger → write-only (Access::Set).
    { "effect", ExposeType::Enum, ::zhc::Access::Set, nullptr, nullptr,
      kEffectLabels, sizeof(kEffectLabels)/sizeof(kEffectLabels[0]) },
    // `do_not_disturb` + `color_power_on_behavior` are settable config.
    // The device echoes no readable state (write-only on this firmware),
    // so they are Access::Set + Config category (optimistic SPA state).
    { "do_not_disturb", ExposeType::Binary, ::zhc::Access::Set, nullptr, nullptr,
      nullptr, 0, ::zhc::ExposeCategory::Config },
    { "color_power_on_behavior", ExposeType::Enum, ::zhc::Access::Set, nullptr, nullptr,
      kColorPowerOnLabels, sizeof(kColorPowerOnLabels)/sizeof(kColorPowerOnLabels[0]),
      ::zhc::ExposeCategory::Config },
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
// NOTE: startUpOnOff (0x4003) deliberately NOT read — z2m's tuyaLight
// sets powerOnBehavior:false for this device; power-on is driven by the
// Tuya color_power_on_behavior command (0xf9), not genOnOff 0x4003.
//   genOnOff(0x0006) attr 0x0000 onOff
//   genLevelCtrl(0x0008) attr 0x0000 currentLevel
//   lightingColorCtrl(0x0300) attrs 0x0003/0x0004/0x0007/0x0008
//     (currentX / currentY / colorTemperature / colorMode)
constexpr std::uint8_t kReadOnOff_Ysr[]  = {
    0x00, 0x00,        // onOff
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
