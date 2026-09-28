// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Five shared bundles for Iluminize ports — one per m.light()/m.onOff()
// feature combination seen in z2m's iluminize.ts. Modelled after
// osram/_shared.

#include "definitions/iluminize/_shared.hpp"

#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::iluminize {

// ── On/off only (m.onOff()) ─────────────────────────────────────────
const ::zhc::FzConverter* const kFzIluOnOff[] = {
    &::zhc::generic::kFzOnOff,
};
static_assert(kFzIluOnOffCount == std::size(kFzIluOnOff));

const ::zhc::TzConverter* const kTzIluOnOff[] = {
    &::zhc::generic::kTzOnOff,
};
static_assert(kTzIluOnOffCount == std::size(kTzIluOnOff));

const ::zhc::Expose kExposesIluOnOff[] = {
    { "state", ::zhc::ExposeType::Binary, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesIluOnOffCount == std::size(kExposesIluOnOff));

const ::zhc::BindingSpec kBindingsIluOnOff[] = {
    { 1, 0x0006 },
};
static_assert(kBindingsIluOnOffCount == std::size(kBindingsIluOnOff));

// ── Plain dimmable (m.light({})) ────────────────────────────────────
const ::zhc::FzConverter* const kFzIluLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
static_assert(kFzIluLightCount == std::size(kFzIluLight));

const ::zhc::TzConverter* const kTzIluLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
static_assert(kTzIluLightCount == std::size(kTzIluLight));

const ::zhc::Expose kExposesIluLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesIluLightCount == std::size(kExposesIluLight));

const ::zhc::BindingSpec kBindingsIluLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
static_assert(kBindingsIluLightCount == std::size(kBindingsIluLight));

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
const ::zhc::FzConverter* const kFzIluCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
static_assert(kFzIluCTLightCount == std::size(kFzIluCTLight));

const ::zhc::TzConverter* const kTzIluCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
static_assert(kTzIluCTLightCount == std::size(kTzIluCTLight));

const ::zhc::Expose kExposesIluCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
};
static_assert(kExposesIluCTLightCount == std::size(kExposesIluCTLight));

const ::zhc::BindingSpec kBindingsIluCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsIluCTLightCount == std::size(kBindingsIluCTLight));

// ── Colour-only (m.light({color: true})) ────────────────────────────
const ::zhc::FzConverter* const kFzIluColorLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
};
static_assert(kFzIluColorLightCount == std::size(kFzIluColorLight));

const ::zhc::TzConverter* const kTzIluColorLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
static_assert(kTzIluColorLightCount == std::size(kTzIluColorLight));

const ::zhc::Expose kExposesIluColorLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesIluColorLightCount == std::size(kExposesIluColorLight));

const ::zhc::BindingSpec kBindingsIluColorLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsIluColorLightCount == std::size(kBindingsIluColorLight));

// ── Full RGBCCT (m.light({colorTemp, color: true})) ─────────────────
const ::zhc::FzConverter* const kFzIluColorCTLight[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};
static_assert(kFzIluColorCTLightCount == std::size(kFzIluColorCTLight));

const ::zhc::TzConverter* const kTzIluColorCTLight[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
static_assert(kTzIluColorCTLightCount == std::size(kTzIluColorCTLight));

const ::zhc::Expose kExposesIluColorCTLight[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      "mired", nullptr, nullptr, 0 },
    { "color_x",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};
static_assert(kExposesIluColorCTLightCount == std::size(kExposesIluColorCTLight));

const ::zhc::BindingSpec kBindingsIluColorCTLight[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};
static_assert(kBindingsIluColorCTLightCount == std::size(kBindingsIluColorCTLight));

// ── Cover-via-brightness pair (5128.10 roller shutter) ──────────────
//
// z2m: fromZigbee=[cover_position_via_brightness, cover_state_via_onoff,
// cover_position_tilt], toZigbee=[cover_state, cover_via_brightness],
// exposes=[cover_position()]  ⇒  position over genLevelCtrl + state over
// genOnOff. The generic kFzCoverPosition (closuresWindowCovering only)
// can't see either, so these add the brightness/onoff legs.
namespace {

// ZCL frame-control for a cluster-specific c→s command, default-response
// suppressed (same convention as the keen_home / generic encoders).
constexpr std::uint8_t kCmdFc = 0x11;

// 0..255 → 0..100 (round-half-up). Matches z2m mapNumberRange semantics.
constexpr std::uint32_t level_to_pct(std::uint32_t lvl) {
    if (lvl > 255) lvl = 255;
    return (lvl * 100u + 127u) / 255u;
}

// 0..100 → 0..255.
constexpr std::uint32_t pct_to_level(std::uint32_t pct) {
    if (pct > 100) pct = 100;
    return (pct * 255u + 50u) / 100u;
}

bool fz_ilu_cover_via_brightness(const DecodedMessage& msg,
                                  const ::zhc::FzConverter&,
                                  const PreparedDefinition&,
                                  RuntimeContext&,
                                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genLevelCtrl currentLevel — attr 0x0000 → key "0".
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint32_t pct = level_to_pct(static_cast<std::uint32_t>(v->u));

    Value pos{}; pos.type = ValueType::Uint; pos.u = pct;
    out.put("position", pos);

    Value st{}; st.type = ValueType::StringRef;
    st.str = (pct > 0) ? "OPEN" : "CLOSE";
    out.put("state", st);
    return true;
}

bool fz_ilu_cover_state_via_onoff(const DecodedMessage& msg,
                                   const ::zhc::FzConverter&,
                                   const PreparedDefinition&,
                                   RuntimeContext&,
                                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genOnOff onOff — attr 0x0000 → key "0".
    const Value* v = msg.payload.find("0");
    if (!v) return false;
    bool on;
    if (v->type == ValueType::Bool)      on = v->b;
    else if (v->type == ValueType::Uint) on = v->u != 0;
    else return false;

    Value st{}; st.type = ValueType::StringRef;
    st.str = on ? "OPEN" : "CLOSE";
    out.put("state", st);
    return true;
}

bool tz_ilu_cover_via_brightness(std::string_view key, const Value& input,
                                  const ::zhc::TzConverter&,
                                  const PreparedDefinition&, RuntimeContext&,
                                  std::span<std::uint8_t> out_frame,
                                  std::size_t& out_size) {
    out_size = 0;
    if (key != "position") return false;
    if (input.type != ValueType::Uint || input.u > 100) return false;

    // genLevelCtrl moveToLevelWithOnOff (cmd 0x04): level u8, transtime u16 LE.
    if (out_frame.size() < 6) return false;
    out_frame[0] = kCmdFc;
    out_frame[1] = 0x00;   // TSN — adapter patches.
    out_frame[2] = 0x04;
    out_frame[3] = static_cast<std::uint8_t>(pct_to_level(static_cast<std::uint32_t>(input.u)));
    out_frame[4] = 0x00;   // transtime LSB
    out_frame[5] = 0x00;   // transtime MSB
    out_size = 6;
    return true;
}

}  // namespace

const ::zhc::FzConverter kFzIluCoverViaBrightness{
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
    .fn                = { .zcl_fn = fz_ilu_cover_via_brightness },
    .user_config       = nullptr,
};

const ::zhc::FzConverter kFzIluCoverStateViaOnOff{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ilu_cover_state_via_onoff },
    .user_config       = nullptr,
};

const ::zhc::TzConverter kTzIluCoverViaBrightness{
    .key          = "position",
    .cluster      = "genLevelCtrl",
    .cluster_id   = 0x0008,
    .command_id   = 0x04,   // moveToLevelWithOnOff
    .fn           = tz_ilu_cover_via_brightness,
    .user_config  = nullptr,
};

}  // namespace zhc::devices::iluminize
