// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Coselig H2-ZB-SW-V1 four-channel switch (new vendor in z2m
// v26.126.0..v26.133.0). A pure input device: no relay, no genOnOff state.
// Endpoints ch1-ch4 (1-4).
// * Custom cluster h2ActionEvent 0xFC10, command actionEvent 0x00, payload
//   {action u8, sequence u16} -> `action` via the upstream lookup (toggle 1 ...
//   scene 12), per channel. 0xFC10 has no name in cluster_names.hpp, so its
//   frames decode with a null cluster name and reach every converter
//   ("fails open"); the converter's own label "h2ActionEvent" never equals a
//   real cluster name, so only unnamed-cluster frames reach it, and it also
//   pins command 0x00 + client-to-server + a 3-byte body + a known action.
// * genMultistateValue presentValue (u16) per channel -> `mode` {button_multi
//   0, button_toggle 1, button_scene 2, rocker_toggle 3, rocker_follow 4,
//   rocker_reverse 5}, writable, reported 0 s / 1 h / 0 per channel.
// The channel rides on the key (action_ch1, mode_ch1), as on the other
// multi-endpoint remotes here; z2m puts it in the value (toggle_ch1).
// Not ported: nothing else upstream (no identify, no onOff on this device).
// z2m-source: coselig.ts #H2-ZB-SW-V1.
#include "definitions/_generic/_shared.hpp"

#include <cstdint>

namespace zhc::devices::coselig {
namespace {

// Index = wire action value (coselig.ts actionLookup).
constexpr const char* kActionByValue[] = {
    nullptr,
    "toggle",
    "brightness_move_up",
    "brightness_move_down",
    "brightness_stop",
    "color_temperature_cool",
    "color_temperature_warm",
    "color_temperature_move_cooler",
    "color_temperature_move_warmer",
    "color_temperature_stop",
    "on",
    "off",
    "scene",
};
constexpr std::size_t kActionCount = sizeof(kActionByValue)/sizeof(kActionByValue[0]);

bool fz_h2_action_event(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.size() < 3) return false;   // action u8 + sequence u16
    const std::uint8_t a = msg.raw_body[0];
    if (a == 0 || a >= kActionCount) return false;   // z2m: no lookup hit, nothing published
    Value v{}; v.type = ValueType::StringRef; v.str = kActionByValue[a];
    out.put("action", v);
    return true;
}

const FzConverter kFzH2ActionEvent{
    .family            = FrameFamily::Zcl,
    .cluster           = "h2ActionEvent",   // 0xFC10: unnamed, see header
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,              // actionEvent
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_h2_action_event },
    .user_config       = nullptr,
};

constexpr ::zhc::generic::ZclWriteLookup kModes[] = {
    {"button_multi", 0}, {"button_toggle", 1}, {"button_scene", 2},
    {"rocker_toggle", 3}, {"rocker_follow", 4}, {"rocker_reverse", 5},
};
constexpr ::zhc::generic::ZclAttrRow kModeRows[] = {
    { 0x0055, "mode", 1, kModes, sizeof(kModes)/sizeof(kModes[0]) },
};
constexpr ::zhc::generic::ZclAttrMap kModeMap{ kModeRows, sizeof(kModeRows)/sizeof(kModeRows[0]) };
constexpr FzConverter kFzMode = ::zhc::generic::zcl_attr_fz("genMultistateValue", &kModeMap);
constexpr ::zhc::generic::ZclWriteSpec kModeSpec{ "mode", 0x0055, 0x21, 0, kModes, sizeof(kModes)/sizeof(kModes[0]) };
constexpr TzConverter kTzMode = ::zhc::generic::zcl_write_tz("genMultistateValue", 0x0014, &kModeSpec);

const FzConverter* const kFz_H2_ZB_SW_V1[] = {
    &kFzH2ActionEvent,
    &kFzMode,
};
const TzConverter* const kTz_H2_ZB_SW_V1[] = {
    &kTzMode,
};

constexpr const char* kModels_H2_ZB_SW_V1[] = { "H2-ZB-SW-V1" };

constexpr ::zhc::EndpointLabel kEndpoints_H2_ZB_SW_V1[] = {
    {"ch1", 1}, {"ch2", 2}, {"ch3", 3}, {"ch4", 4} };

constexpr const char* kActionValues[] = {
    "toggle", "brightness_move_up", "brightness_move_down", "brightness_stop",
    "color_temperature_cool", "color_temperature_warm", "color_temperature_move_cooler",
    "color_temperature_move_warmer", "color_temperature_stop", "on", "off", "scene",
};
constexpr const char* kModeValues[] = {
    "button_multi", "button_toggle", "button_scene", "rocker_toggle", "rocker_follow", "rocker_reverse" };
constexpr const char* kModeDesc = "Input mode of the channel";

constexpr Expose kExposes_H2_ZB_SW_V1[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, "Triggered action (e.g. a button click)",
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0]), ExposeCategory::Diagnostic},
    {"mode_ch1", ExposeType::Enum, Access::StateSet, nullptr, kModeDesc,
     kModeValues, sizeof(kModeValues)/sizeof(kModeValues[0]), ExposeCategory::Config},
    {"mode_ch2", ExposeType::Enum, Access::StateSet, nullptr, kModeDesc,
     kModeValues, sizeof(kModeValues)/sizeof(kModeValues[0]), ExposeCategory::Config},
    {"mode_ch3", ExposeType::Enum, Access::StateSet, nullptr, kModeDesc,
     kModeValues, sizeof(kModeValues)/sizeof(kModeValues[0]), ExposeCategory::Config},
    {"mode_ch4", ExposeType::Enum, Access::StateSet, nullptr, kModeDesc,
     kModeValues, sizeof(kModeValues)/sizeof(kModeValues[0]), ExposeCategory::Config},
};

constexpr BindingSpec kBindings_H2_ZB_SW_V1[] = {
    {1, 0x0014}, {2, 0x0014}, {3, 0x0014}, {4, 0x0014},
};
constexpr ReportingSpec kReports_H2_ZB_SW_V1[] = {
    {1, 0x0014, 0x0055, 0x21, 0, 3600, 0, 0},
    {2, 0x0014, 0x0055, 0x21, 0, 3600, 0, 0},
    {3, 0x0014, 0x0055, 0x21, 0, 3600, 0, 0},
    {4, 0x0014, 0x0055, 0x21, 0, 3600, 0, 0},
};

}  // namespace

extern const PreparedDefinition kDef_H2_ZB_SW_V1{
    .zigbee_models=kModels_H2_ZB_SW_V1, .zigbee_models_count=sizeof(kModels_H2_ZB_SW_V1)/sizeof(kModels_H2_ZB_SW_V1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="H2-ZB-SW-V1", .vendor="Coselig",
    .meta=nullptr, .exposes=kExposes_H2_ZB_SW_V1, .exposes_count=sizeof(kExposes_H2_ZB_SW_V1)/sizeof(kExposes_H2_ZB_SW_V1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_H2_ZB_SW_V1, .from_zigbee_count=sizeof(kFz_H2_ZB_SW_V1)/sizeof(kFz_H2_ZB_SW_V1[0]),
    .to_zigbee=kTz_H2_ZB_SW_V1, .to_zigbee_count=sizeof(kTz_H2_ZB_SW_V1)/sizeof(kTz_H2_ZB_SW_V1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_H2_ZB_SW_V1, .bindings_count=sizeof(kBindings_H2_ZB_SW_V1)/sizeof(kBindings_H2_ZB_SW_V1[0]),
    .reports=kReports_H2_ZB_SW_V1, .reports_count=sizeof(kReports_H2_ZB_SW_V1)/sizeof(kReports_H2_ZB_SW_V1[0]),
    .endpoint_map=kEndpoints_H2_ZB_SW_V1,
    .endpoint_map_count=sizeof(kEndpoints_H2_ZB_SW_V1)/sizeof(kEndpoints_H2_ZB_SW_V1[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::coselig
