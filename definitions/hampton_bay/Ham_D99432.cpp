// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: HamptonBay 99432 — hand-rewritten for parity (fan_state derivation).
// Universal wink enabled white ceiling fan premier remote control.
//
// z2m: fromZigbee:[fz.fan], toZigbee:[tz.fan_mode],
//   exposes: e.fan().withState("fan_state").withModes(
//              ["low","medium","high","on","smart"]),
//   extend:  [m.light({configureReporting: true}), m.forcePowerSource(...)]
//
// z2m fz.fan (converters/fromZigbee.ts) decodes hvacFanCtrl attr 0x0000
// (fanMode, enum8) and publishes BOTH:
//   fan_mode  : the enum key  (constants.fanMode: off/low/medium/high/on/auto/smart)
//   fan_state : "OFF" when fanMode == off (0), else "ON"
//
// The generated port wired the generic ::zhc::generic::kFzFanMode, which emits
// ONLY `fan_mode` (raw u8) and DROPS the derived `fan_state` — and it declared
// `fan_mode` as a Binary with no `fan_state` expose at all. Fixed by an inlined
// vendor converter that, from the same 0x0000 report, emits `fan_mode` (raw u8,
// matching the project's established enum-as-uint convention — cf. owon
// thermostats / mercator SSWF01G) AND `fan_state` (Bool: off/0 -> false, else
// true) to mirror z2m. fan_mode is now an Enum expose with the z2m mode labels;
// fan_state is a Binary. The light half (m.light, no colorTemp) stays on/off +
// brightness. Mirrors the Mercator SSWF01G fan-controller precedent.
//
// z2m-source: hampton_bay.ts #99432.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hampton_bay {
namespace {

// fz_hampton_fan — hvacFanCtrl (0x0202) attr 0x0000 (FanMode, enum8).
// Emits fan_mode (u8) + fan_state (bool: off==0 -> false). Mirrors z2m
// fz.fan which returns {fan_mode, fan_state}.
bool fz_hampton_fan(const DecodedMessage& msg,
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

const FzConverter kFzHamptonFan{
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
    .fn                = { .zcl_fn = fz_hampton_fan },
    .user_config       = nullptr,
};

const FzConverter* const kFz_D99432[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &kFzHamptonFan,
};
const TzConverter* const kTz_D99432[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr const char* kModels_D99432[] = { "HDC52EastwindFan", "HBUniversalCFRemote" };

// z2m e.fan().withModes(["low","medium","high","on","smart"]); the picker
// labels for the fan_mode enum. The wire value is the raw constants.fanMode
// code (off=0/low=1/medium=2/high=3/on=4/smart=6), emitted as-is by the
// converter — the labels are UI metadata, not a contiguous re-index.
constexpr const char* kFanModes_D99432[] = { "low", "medium", "high", "on", "smart" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fan_mode", ExposeType::Enum, Access::StateSet, nullptr, "Mode of this fan",
     kFanModes_D99432, 5},
    {"fan_state", ExposeType::Binary, Access::StateSet, nullptr, "On/off state of this fan",
     nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0202},
};

extern const PreparedDefinition kDef_D99432{
    .zigbee_models=kModels_D99432, .zigbee_models_count=sizeof(kModels_D99432)/sizeof(kModels_D99432[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="99432", .vendor="HamptonBay",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D99432, .from_zigbee_count=sizeof(kFz_D99432)/sizeof(kFz_D99432[0]),
    .to_zigbee=kTz_D99432, .to_zigbee_count=sizeof(kTz_D99432)/sizeof(kTz_D99432[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hampton_bay
