// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Namron Simplify wireless rotary dimmer 4512797 / 4512798 (z2m
// v26.125.0 window). zigbeeModels 4512797, 4512797_98, 4512798.
// Turns and presses arrive as ordinary genOnOff / genLevelCtrl /
// lightingColorCtrl commands. The private cluster namronPrivateE004 (0xE004)
// adds a buttonEvent command (0x00: button u8, code u8) of which upstream maps
// code 4 to `hold` and 8 to `double`, plus five config attributes: step sizes
// 0x0002-0x0005 (u8) and level_color_mode 0x0006 (enum8). 0xE004 resolves to
// "zosungIRControl" in the shared cluster-name table, so the converters below
// use that label (same cluster as D4512793, scoped by this def's lists).
// z2m-source: namron.ts #4512797 + fzLocal.namronSimplifyRotary.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::namron {
namespace {

constexpr std::uint16_t kClusterE004 = 0xE004;

bool fz_namron_simplify_rotary(const DecodedMessage& msg, const FzConverter&,
                               const PreparedDefinition&, RuntimeContext&,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.command_id != 0x00 || msg.raw_body.size() < 2) return false;
    const char* action = nullptr;
    switch (msg.raw_body[1]) {   // [button, code]
        case 4: action = "hold";   break;
        case 8: action = "double"; break;
        default: return false;     // press / turn come as genOnOff / genLevelCtrl
    }
    Value a{}; a.type = ValueType::StringRef; a.str = action;
    out.put("action", a);
    return true;
}

constexpr FzConverter kFzNamronSimplifyRotary{
    .family            = FrameFamily::Zcl,
    .cluster           = "zosungIRControl",   // 0xE004 namronPrivateE004
    .type_mask         = type_bit(MessageType::Command) |
                         type_bit(MessageType::Raw),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_namron_simplify_rotary },
    .user_config       = nullptr,
};

constexpr ::zhc::generic::ZclWriteLookup kLevelColorModes[] = {
    {"brightness_only", 0}, {"auto_switch", 1} };
constexpr ::zhc::generic::ZclAttrRow kRowsE004[] = {
    {0x0002, "level_up_step_size"},
    {0x0003, "level_down_step_size"},
    {0x0004, "color_temp_up_step_size"},
    {0x0005, "color_temp_down_step_size"},
    {0x0006, "level_color_mode", 1, kLevelColorModes, 2},
};
constexpr ::zhc::generic::ZclAttrMap kMapE004{ kRowsE004, sizeof(kRowsE004)/sizeof(kRowsE004[0]) };
constexpr FzConverter kFzE004Attrs = ::zhc::generic::zcl_attr_fz("zosungIRControl", &kMapE004);

constexpr ::zhc::generic::ZclWriteSpec kSpecLevelUp{ "level_up_step_size", 0x0002, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecLevelDown{ "level_down_step_size", 0x0003, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecCtUp{ "color_temp_up_step_size", 0x0004, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecCtDown{ "color_temp_down_step_size", 0x0005, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecLevelColorMode{ "level_color_mode", 0x0006, 0x30, 0, kLevelColorModes, 2 };
constexpr TzConverter kTzLevelUp = ::zhc::generic::zcl_write_tz("zosungIRControl", kClusterE004, &kSpecLevelUp);
constexpr TzConverter kTzLevelDown = ::zhc::generic::zcl_write_tz("zosungIRControl", kClusterE004, &kSpecLevelDown);
constexpr TzConverter kTzCtUp = ::zhc::generic::zcl_write_tz("zosungIRControl", kClusterE004, &kSpecCtUp);
constexpr TzConverter kTzCtDown = ::zhc::generic::zcl_write_tz("zosungIRControl", kClusterE004, &kSpecCtDown);
constexpr TzConverter kTzLevelColorMode = ::zhc::generic::zcl_write_tz("zosungIRControl", kClusterE004, &kSpecLevelColorMode);

const FzConverter* const kFz_D4512797[] = {
    &kFzNamronSimplifyRotary,
    &kFzE004Attrs,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMoveToLevel,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandMoveColorTemperature,
};
const TzConverter* const kTz_D4512797[] = {
    &kTzLevelUp,
    &kTzLevelDown,
    &kTzCtUp,
    &kTzCtDown,
    &kTzLevelColorMode,
};
constexpr const char* kModels_D4512797[] = { "4512797", "4512797_98", "4512798" };
constexpr WhiteLabel kWL_D4512797[] = { {"Namron", "4512798"} };

constexpr const char* kLevelColorModeOpts[] = { "brightness_only", "auto_switch" };
constexpr Expose kExp_D4512797[] = {
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"level_up_step_size", ExposeType::Numeric, Access::StateSet, "%",
     "Brightness step when turning up", nullptr, 0, ExposeCategory::Config, 1, 100, 1},
    {"level_down_step_size", ExposeType::Numeric, Access::StateSet, "%",
     "Brightness step when turning down", nullptr, 0, ExposeCategory::Config, 1, 100, 1},
    {"color_temp_up_step_size", ExposeType::Numeric, Access::StateSet, "mired",
     "Color temperature step when turning up", nullptr, 0, ExposeCategory::Config, 1, 254, 1},
    {"color_temp_down_step_size", ExposeType::Numeric, Access::StateSet, "mired",
     "Color temperature step when turning down", nullptr, 0, ExposeCategory::Config, 1, 254, 1},
    {"level_color_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "Automatically return to brightness control after 10 seconds", kLevelColorModeOpts, 2,
     ExposeCategory::Config},
};

constexpr BindingSpec kBindings_D4512797[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_D4512797{
    .zigbee_models=kModels_D4512797, .zigbee_models_count=sizeof(kModels_D4512797)/sizeof(kModels_D4512797[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4512797", .vendor="Namron",
    .meta=nullptr, .exposes=kExp_D4512797, .exposes_count=sizeof(kExp_D4512797)/sizeof(kExp_D4512797[0]),
    .white_labels=kWL_D4512797, .white_labels_count=sizeof(kWL_D4512797)/sizeof(kWL_D4512797[0]),
    .from_zigbee=kFz_D4512797, .from_zigbee_count=sizeof(kFz_D4512797)/sizeof(kFz_D4512797[0]),
    .to_zigbee=kTz_D4512797, .to_zigbee_count=sizeof(kTz_D4512797)/sizeof(kTz_D4512797[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D4512797, .bindings_count=sizeof(kBindings_D4512797)/sizeof(kBindings_D4512797[0]),
};

}  // namespace zhc::devices::namron
