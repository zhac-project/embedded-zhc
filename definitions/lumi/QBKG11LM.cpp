// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara QBKG11LM smart wall switch (neutral, single rocker).
//
// Single relay: genOnOff on endpoint 1 → `state` (no endpoint label map;
// the 2-gang `state_left`/`state_right` map was copied here by mistake).
// Power = genAnalogInput presentValue, energy = MI-struct tag 0x95,
// action = genMultistateInput (single/double/release/hold) or, from
// firmware that reports a rocker press as genOnOff, z2m `lumi_action`
// (below), operation_mode = genBasic 0xFF22 (0x12 control_relay /
// 0xFE decoupled, Lumi manufacturer code, endpoint 1). The hub answers the
// switch's "may I reset?" request (z2m lumiPreventReset).
//
// Not ported: the `lumi_power` get.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lumi.ts
//             #QBKG11LM (lumi.ctrl_ln1.aq1 / lumi.ctrl_ln1).

#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {

namespace {

// z2m lumi_action, QBKG11LM branch: a genOnOff attribute report is a rocker
// press ({0: single, 1: single}) unless its 0xF000 (61440) is truthy, as on
// the relay's own state reports. In decoupled mode the press arrives on EP4,
// which kFzLumiOnOff skips as relay state.
bool fz_on_off_action(const DecodedMessage& msg, const FzConverter&,
                      const PreparedDefinition&, RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (const Value* f = msg.payload.find("61440")) {
        const bool zero = (f->type == ValueType::Uint && f->u == 0) ||
                          (f->type == ValueType::Int && f->i == 0);
        if (!zero) return false;
    }
    const Value* v = msg.payload.find("0");   // onOff
    if (!v || !(v->type == ValueType::Bool || (v->type == ValueType::Uint && v->u <= 1)))
        return false;
    Value a{}; a.type = ValueType::StringRef; a.str = "single";
    out.put("action", a);
    return true;
}

constexpr FzConverter kFzOnOffAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_on_off_action },
    .user_config       = nullptr,
};

const FzConverter* const kFz[] = {
    &::zhc::lumi::kFzLumiBasicEnergy,
    &::zhc::lumi::kFzLumiOnOff,
    &kFzOnOffAction,
    &::zhc::lumi::kFzLumiPowerAnalog,
    &::zhc::lumi::kFzLumiActionMultistate,
    &::zhc::lumi::kFzLumiOperationMode,
    &::zhc::lumi::kFzLumiPreventReset,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::lumi::kTzLumiOperationModeBasic,
};

constexpr const char* kZigbeeModels[] = { "lumi.ctrl_ln1", "lumi.ctrl_ln1.aq1" };

constexpr const char* kActionValues[] = { "single", "double", "release", "hold" };

}  // namespace

constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActionValues, 4},
    {"operation_mode", ExposeType::Enum, Access::StateSet, nullptr, "Decoupled mode",
     ::zhc::lumi::kLumiOperationModeValues, std::size(::zhc::lumi::kLumiOperationModeValues),
     ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0000},
    {1, 0x0006},
};

extern const PreparedDefinition kDefQBKG11LM{
    .zigbee_models=kZigbeeModels,.zigbee_models_count=sizeof(kZigbeeModels)/sizeof(kZigbeeModels[0]),
    .model               = "QBKG11LM",
    .vendor              = "Xiaomi",
    .meta                = nullptr,
    .exposes=kExposes,
    .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels        = nullptr,
    .white_labels_count  = 0,
    .from_zigbee         = kFz,
    .from_zigbee_count   = sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee           = kTz,
    .to_zigbee_count     = sizeof(kTz)/sizeof(kTz[0]),
    .configure           = nullptr,
    .on_event            = nullptr,
.bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    // z2m m.forcePowerSource({powerSource: "Mains (single phase)"}).
    .power_source_override=0x01,
};

}  // namespace zhc::devices::lumi
