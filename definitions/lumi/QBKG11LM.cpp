// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara QBKG11LM smart wall switch (neutral, single rocker).
//
// Single relay: genOnOff on endpoint 1 → `state` (no endpoint label map;
// the 2-gang `state_left`/`state_right` map was copied here by mistake).
// Power = genAnalogInput presentValue, energy = MI-struct tag 0x95,
// action = genMultistateInput, operation_mode = genBasic 0xFF22
// (0x12 control_relay / 0xFE decoupled, Lumi manufacturer code).
//
// Not ported: z2m `lumi_action` (a genOnOff report without attr 0xF000 →
// action "single"; the multistate path already carries single/double/
// release/hold), and the `lumi_power` get.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lumi.ts
//             #QBKG11LM (lumi.ctrl_ln1.aq1 / lumi.ctrl_ln1).

#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {

namespace {

// z2m lumi_operation_mode_basic / lumi_switch_operation_mode_basic
// (single-endpoint lookup).
constexpr ::zhc::lumi::LumiActionEntry kOpModeEntries[] = {
    {0x12, "control_relay"},
    {0xFE, "decoupled"},
};
constexpr ::zhc::lumi::LumiActionMap kOpModeMap{ kOpModeEntries, 2 };

constexpr FzConverter kFzOpMode{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_operation_mode_basic },
    .user_config       = &kOpModeMap,
};

constexpr ::zhc::generic::ZclWriteLookup kOpModeLut[] = {
    {"control_relay", 0x12},
    {"decoupled",     0xFE},
};
constexpr ::zhc::generic::ZclWriteSpec kOpModeWrite{
    "operation_mode", 0xFF22, 0x20, 0x115F, kOpModeLut, 2,
};
constexpr TzConverter kTzOpMode{
    .key         = "operation_mode",
    .cluster     = "genBasic",
    .cluster_id  = 0x0000,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kOpModeWrite,
};

const FzConverter* const kFz[] = {
    &::zhc::lumi::kFzLumiBasicEnergy,
    &::zhc::lumi::kFzLumiOnOff,
    &::zhc::lumi::kFzLumiPowerAnalog,
    &::zhc::lumi::kFzLumiActionMultistate,
    &kFzOpMode,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &kTzOpMode,
};

constexpr const char* kZigbeeModels[] = { "lumi.ctrl_ln1", "lumi.ctrl_ln1.aq1" };

constexpr const char* kActionValues[] = { "single", "double", "release", "hold" };
constexpr const char* kOpModeValues[] = { "control_relay", "decoupled" };

}  // namespace

constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kActionValues, 4},
    {"operation_mode", ExposeType::Enum, Access::StateSet, nullptr, "Decoupled mode",
     kOpModeValues, 2, ExposeCategory::Config},
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
