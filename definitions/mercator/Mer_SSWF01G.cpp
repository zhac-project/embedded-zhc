// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Mercator SSWF01G — hand-rewritten (fan controller).
// AC fan controller.
//
// z2m: fromZigbee:[fz.on_off, fz.fan]
//   toZigbee:  [tz.fan_mode, tz.on_off]
//   exposes:   e.switch()  -> "state"
//              e.fan().withState("fan_state").withModes(
//                ["off","low","medium","high","on"])
//
// z2m fz.fan (converters/fromZigbee.ts) decodes hvacFanCtrl attr 0x0000
// (fanMode, enum8) and publishes BOTH:
//   fan_mode  : the enum key  (constants.fanMode: off/low/medium/high/on/...)
//   fan_state : "OFF" when fanMode == off (0), else "ON"
//
// The generic ::zhc::generic::kFzFanMode emits ONLY `fan_mode` (raw u8),
// so the `fan_state` expose this device declares was DEAD — nothing ever
// populated it. Fixed by an inlined vendor converter that, from the same
// 0x0000 report, emits `fan_mode` (raw u8, matching the project's
// established enum-as-uint convention — cf. owon thermostats) AND
// `fan_state` (Bool: false when off/0, true otherwise) to mirror z2m.
// Only one Mercator fan SKU exists, so the converter is inlined here
// rather than promoted to a shared file.
//
// z2m-source: mercator.ts #SSWF01G.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

// fz_mercator_fan — hvacFanCtrl (0x0202) attr 0x0000 (FanMode, enum8).
// Emits fan_mode (u8) + fan_state (bool: off==0 -> false). Mirrors z2m
// fz.fan which returns {fan_mode, fan_state}.
bool fz_mercator_fan(const DecodedMessage& msg,
                     const FzConverter&,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    Value mode{}; mode.type = ValueType::Uint; mode.u = v->u;
    out.put("fan_mode", mode);
    Value state{}; state.type = ValueType::Bool; state.b = (v->u != 0);
    out.put("fan_state", state);
    return true;
}

const FzConverter kFzMercatorFan{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacFanCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_mercator_fan },
    .user_config       = nullptr,
};

const FzConverter* const kFz_SSWF01G[] = {
    &::zhc::generic::kFzOnOff,
    &kFzMercatorFan,
};
const TzConverter* const kTz_SSWF01G[] = {
    &::zhc::generic::kTzOnOff,
};

constexpr const char* kModels_SSWF01G[] = { "TS0501" };
constexpr const char* kManus_SSWF01G[] = {
    "_TZ3210_lzqq3u4r",
    "_TZ3210_4whigl8i",
};

constexpr Expose kExposes_SSWF01G[] = {
    { "state",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "fan_mode",  ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0 },
    { "fan_state", ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SSWF01G[] = {
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0202 },  // hvacFanCtrl
};

}  // namespace

extern const PreparedDefinition kDef_SSWF01G{
    .zigbee_models=kModels_SSWF01G, .zigbee_models_count=sizeof(kModels_SSWF01G)/sizeof(kModels_SSWF01G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SSWF01G, .manufacturer_names_count=sizeof(kManus_SSWF01G)/sizeof(kManus_SSWF01G[0]),
    .model="SSWF01G", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SSWF01G, .exposes_count=sizeof(kExposes_SSWF01G)/sizeof(kExposes_SSWF01G[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SSWF01G, .from_zigbee_count=sizeof(kFz_SSWF01G)/sizeof(kFz_SSWF01G[0]),
    .to_zigbee=kTz_SSWF01G, .to_zigbee_count=sizeof(kTz_SSWF01G)/sizeof(kTz_SSWF01G[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SSWF01G, .bindings_count=sizeof(kBindings_SSWF01G)/sizeof(kBindings_SSWF01G[0]),
};

}  // namespace zhc::devices::mercator
