// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Control4 Vibrant A19 lamps (new vendor in z2m v26.126.0..v26.133.0):
// VB-FTW-A19 (zigbeeModel C4-VB-FTW-A19, tunable white) and VB-RGBTW-A19
// (C4-VB-RGBTW-A19, tunable white + xy colour).
// z2m `vibrantLight` = m.light({colorTemp: {range: [153, 555]}} [+ color xy])
// with fz.brightness / tz.light_onoff_brightness swapped for local ones: the
// lamp's genLevelCtrl runs 0-100 instead of 0-254 (moveToLevel above 100 is
// ignored). Read: currentLevel * 254 / 100, rounded. Write: brightness * 100 /
// 254, rounded, sent as moveToLevelWithOnOff. Plus the m.light defaults:
// power_on_behavior, effect (+ colorloop on the colour lamp).
// Not ported: identify; color_temp_startup; the tz trick that turns a plain
// "ON" into ON + the last brightness (needs the published state in the tz);
// brightness_percent.
// z2m-source: control4.ts #VB-FTW-A19
// z2m-source: control4.ts #VB-RGBTW-A19
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::control4 {
namespace {

// currentLevel 0-100 -> brightness 0-254 (Math.round(level * 254 / 100)).
bool fz_vibrant_brightness(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 currentLevel
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = (v->u * 254 + 50) / 100;
    out.put("brightness", o);
    return true;
}

// brightness 0-254 (255 accepted, as the generic writer) -> level 0-100
// (Math.round(brightness * 100 / 254); no ties exist), then the generic
// moveToLevelWithOnOff frame.
bool tz_vibrant_brightness(std::string_view key, const Value& input,
                           const TzConverter& self, const PreparedDefinition& def,
                           RuntimeContext& ctx, std::span<std::uint8_t> out_frame,
                           std::size_t& out_size) {
    out_size = 0;
    std::uint64_t level;
    if (input.type == ValueType::Uint) {
        level = input.u;
    } else if (input.type == ValueType::Float && input.f >= 0.0f && input.f < 255.5f) {
        level = static_cast<std::uint64_t>(input.f + 0.5f);
    } else {
        return false;
    }
    if (level > 255) return false;
    Value scaled{}; scaled.type = ValueType::Uint; scaled.u = (level * 100 + 127) / 254;
    return ::zhc::generic::tz_brightness(key, scaled, self, def, ctx, out_frame, out_size);
}

const FzConverter kFzVibrantBrightness{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_vibrant_brightness },
    .user_config       = nullptr,
};

const TzConverter kTzVibrantBrightness{
    .key         = "brightness",
    .cluster     = "genLevelCtrl",
    .cluster_id  = 0x0008,
    .command_id  = 0x04,   // moveToLevelWithOnOff
    .fn          = tz_vibrant_brightness,
    .user_config = nullptr,
};

const FzConverter* const kFz_VB_FTW_A19[] = {
    &::zhc::generic::kFzOnOff,
    &kFzVibrantBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_VB_FTW_A19[] = {
    &::zhc::generic::kTzOnOff,
    &kTzVibrantBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzPowerOnBehavior1,
    &::zhc::generic::kTzEffect,
};

const FzConverter* const kFz_VB_RGBTW_A19[] = {
    &::zhc::generic::kFzOnOff,
    &kFzVibrantBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzPowerOnBehavior1,
};
const TzConverter* const kTz_VB_RGBTW_A19[] = {
    &::zhc::generic::kTzOnOff,
    &kTzVibrantBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzPowerOnBehavior1,
    &::zhc::generic::kTzEffect,
    &::zhc::generic::kTzEffectColorLoop,
};

constexpr const char* kModels_VB_FTW_A19[] = { "C4-VB-FTW-A19" };
constexpr const char* kModels_VB_RGBTW_A19[] = { "C4-VB-RGBTW-A19" };

constexpr const char* kEffectsWhite[] = {
    "blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect" };
constexpr const char* kEffectsColor[] = {
    "blink", "breathe", "okay", "channel_change", "finish_effect", "stop_effect",
    "colorloop", "stop_colorloop" };
constexpr const char* kPowerOn[] = { "off", "on", "toggle", "previous" };

constexpr Expose kExposes_VB_FTW_A19[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 153, 555, 1},
    {"effect", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffectsWhite, sizeof(kEffectsWhite)/sizeof(kEffectsWhite[0])},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kPowerOn, sizeof(kPowerOn)/sizeof(kPowerOn[0])},
};
constexpr Expose kExposes_VB_RGBTW_A19[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 153, 555, 1},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_mode", ExposeType::Enum, Access::State, nullptr, nullptr, ::zhc::generic::kColorModes,
     sizeof(::zhc::generic::kColorModes)/sizeof(::zhc::generic::kColorModes[0])},
    {"effect", ExposeType::Enum, Access::Set, nullptr, "Triggers an effect on the light",
     kEffectsColor, sizeof(kEffectsColor)/sizeof(kEffectsColor[0])},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr,
     kPowerOn, sizeof(kPowerOn)/sizeof(kPowerOn[0])},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_VB_FTW_A19{
    .zigbee_models=kModels_VB_FTW_A19, .zigbee_models_count=sizeof(kModels_VB_FTW_A19)/sizeof(kModels_VB_FTW_A19[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VB-FTW-A19", .vendor="Control4",
    .meta=nullptr, .exposes=kExposes_VB_FTW_A19, .exposes_count=sizeof(kExposes_VB_FTW_A19)/sizeof(kExposes_VB_FTW_A19[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VB_FTW_A19, .from_zigbee_count=sizeof(kFz_VB_FTW_A19)/sizeof(kFz_VB_FTW_A19[0]),
    .to_zigbee=kTz_VB_FTW_A19, .to_zigbee_count=sizeof(kTz_VB_FTW_A19)/sizeof(kTz_VB_FTW_A19[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};

extern const PreparedDefinition kDef_VB_RGBTW_A19{
    .zigbee_models=kModels_VB_RGBTW_A19, .zigbee_models_count=sizeof(kModels_VB_RGBTW_A19)/sizeof(kModels_VB_RGBTW_A19[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VB-RGBTW-A19", .vendor="Control4",
    .meta=nullptr, .exposes=kExposes_VB_RGBTW_A19, .exposes_count=sizeof(kExposes_VB_RGBTW_A19)/sizeof(kExposes_VB_RGBTW_A19[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VB_RGBTW_A19, .from_zigbee_count=sizeof(kFz_VB_RGBTW_A19)/sizeof(kFz_VB_RGBTW_A19[0]),
    .to_zigbee=kTz_VB_RGBTW_A19, .to_zigbee_count=sizeof(kTz_VB_RGBTW_A19)/sizeof(kTz_VB_RGBTW_A19[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};

}  // namespace zhc::devices::control4
