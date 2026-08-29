// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Gewiss GWA1241 ChorusSmart 2-channel push button controller.
// New upstream in z2m v26.98.0.
//
// The two buttons report as genBinaryInput `presentValue` on two
// endpoints — 1 ("top") and 10 ("bottom"). z2m emits a single
// `action` of `press_top` / `press_bottom`; the endpoint rides in the
// action VALUE, not the key, which matches the `kAlwaysGlobalKeys`
// policy in dispatch.cpp (action is never suffixed unless a def opts
// in). So no endpoint_map is declared: the labels here are the action
// strings, not key suffixes.
//
// Only a rising edge counts — z2m ignores `presentValue === 0`, so a
// button release does not emit an action.
//
// z2m-source: gewiss.ts #GWA1241.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gewiss {
namespace {

// genBinaryInput presentValue is attribute 0x0055 == 85 decimal; the
// parser exposes unknown/plain attribute ids in decimal-string form.
constexpr const char* kPresentValueKey = "85";

bool fz_gwa1241_action(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find(kPresentValueKey);
    if (!v) return false;

    // z2m accepts presentValue === 1 or === true and ignores everything
    // else (including the 0 that arrives on release).
    bool pressed = false;
    if      (v->type == ValueType::Bool) pressed = v->b;
    else if (v->type == ValueType::Uint) pressed = (v->u == 1);
    else if (v->type == ValueType::Int)  pressed = (v->i == 1);
    else return false;
    if (!pressed) return false;

    const char* label = nullptr;
    if      (msg.src_endpoint == 1)  label = "press_top";
    else if (msg.src_endpoint == 10) label = "press_bottom";
    else return false;   // z2m's "unknown" branch emits nothing

    Value action{}; action.type = ValueType::StringRef;
    action.str = label;
    out.put("action", action);
    return true;
}

const FzConverter kFzGwa1241Action{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBinaryInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_gwa1241_action },
    .user_config       = nullptr,
};

const FzConverter* const kFz_GWA1241[] = { &kFzGwa1241Action };

constexpr const char* kModels_GWA1241[] = { "GWA1241_PUSH_BUTTON_2_CH" };

constexpr const char* kActionValues_GWA1241[] = {
    "press_top", "press_bottom",
};

constexpr Expose kExposes_GWA1241[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues_GWA1241,
     sizeof(kActionValues_GWA1241)/sizeof(kActionValues_GWA1241[0])},
};

constexpr BindingSpec kBindings_GWA1241[] = {
    {1,  0x000F},   // genBinaryInput — top
    {10, 0x000F},   // genBinaryInput — bottom
};

}  // namespace

extern const PreparedDefinition kDef_GWA1241{
    .zigbee_models=kModels_GWA1241,
    .zigbee_models_count=sizeof(kModels_GWA1241)/sizeof(kModels_GWA1241[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GWA1241", .vendor="Gewiss",
    .meta=nullptr,
    .exposes=kExposes_GWA1241,
    .exposes_count=sizeof(kExposes_GWA1241)/sizeof(kExposes_GWA1241[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_GWA1241, .from_zigbee_count=sizeof(kFz_GWA1241)/sizeof(kFz_GWA1241[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_GWA1241,
    .bindings_count=sizeof(kBindings_GWA1241)/sizeof(kBindings_GWA1241[0]),
};

}  // namespace zhc::devices::gewiss
