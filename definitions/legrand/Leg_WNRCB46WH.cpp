// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Legrand WNRCB46WH wired 4-scene control.
// New upstream in z2m v26.100.0.
//
// The four buttons do not send distinct scene ids — every press is a genScenes
// commandRecall, and the BUTTON is identified by the frame's GROUP id:
//
//   65517 (0xFFED) -> button_1     65516 (0xFFEC) -> button_2
//   65515 (0xFFEB) -> button_3     65514 (0xFFEA) -> button_4
//
// The raw group id always goes out as `action_group`, matching z2m. z2m also
// synthesises `recall_group_<id>` as the action for an unrecognised group;
// that needs a formatted string, and a converter only gets a const view of the
// message scratch buffer, so an unknown group here publishes `action_group`
// alone and no `action`. A unit reporting other group ids is still fully
// diagnosable from that, and a made-up action string is worse than none.
//
// The generic kFzCommandRecall cannot be reused: it keys the action off the
// scene id in the command body, which is constant here.
//
// The zigbeeModel is the device's Basic modelId verbatim, INCLUDING the
// leading space and the ten NUL bytes the firmware pads it with — matching is
// byte-exact, so the padding has to be reproduced.
//
// z2m-source: legrand.ts #WNRCB46WH (fzLocal.command_recall_by_groupid).
#include "definitions/_generic/_shared.hpp"

#include <cstdint>

namespace zhc::devices::legrand {
namespace {

bool fz_recall_by_groupid(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genScenes Recall Scene body is groupId(2, little-endian) + sceneId(1).
    // z2m reads `msg.data.groupid`, which is this BODY field — not the APS
    // group the frame was addressed to, so `msg.group_id` is the wrong source.
    if (msg.raw_body.size() < 2) return false;
    const std::uint16_t group =
        static_cast<std::uint16_t>(msg.raw_body[0]) |
        static_cast<std::uint16_t>(static_cast<std::uint16_t>(msg.raw_body[1]) << 8);

    const char* label = nullptr;
    switch (group) {
        case 65517: label = "button_1"; break;
        case 65516: label = "button_2"; break;
        case 65515: label = "button_3"; break;
        case 65514: label = "button_4"; break;
        default:    label = nullptr;    break;
    }

    if (label) {
        Value action{}; action.type = ValueType::StringRef;
        action.str = label;
        out.put("action", action);
    }

    Value gid{}; gid.type = ValueType::Uint; gid.u = group;
    out.put("action_group", gid);
    return true;
}

const FzConverter kFzRecallByGroupId{
    .family            = FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x05,          // commandRecall
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_recall_by_groupid },
    .user_config       = nullptr,
};

const FzConverter* const kFz_WNRCB46WH[] = { &kFzRecallByGroupId };

// " Wired Scenes Command" + ten NUL pad bytes, exactly as the firmware
// reports it.
constexpr const char kModelId_WNRCB46WH[] =
    " Wired Scenes Command\0\0\0\0\0\0\0\0\0\0";
constexpr const char* kModels_WNRCB46WH[] = { kModelId_WNRCB46WH };

constexpr const char* kActionValues[] = {
    "button_1", "button_2", "button_3", "button_4",
};

constexpr Expose kExposes_WNRCB46WH[] = {
    {"action",       ExposeType::Enum,    Access::State, nullptr, nullptr,
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
    {"action_group", ExposeType::Numeric, Access::State, nullptr,
     "Group id the recall arrived on", nullptr, 0, ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_WNRCB46WH[] = {
    {1, 0x0003},   // genIdentify
    {1, 0x0005},   // genScenes
};

}  // namespace

extern const PreparedDefinition kDef_WNRCB46WH{
    .zigbee_models=kModels_WNRCB46WH,
    .zigbee_models_count=sizeof(kModels_WNRCB46WH)/sizeof(kModels_WNRCB46WH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WNRCB46WH", .vendor="Legrand",
    .meta=nullptr,
    .exposes=kExposes_WNRCB46WH,
    .exposes_count=sizeof(kExposes_WNRCB46WH)/sizeof(kExposes_WNRCB46WH[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WNRCB46WH, .from_zigbee_count=sizeof(kFz_WNRCB46WH)/sizeof(kFz_WNRCB46WH[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_WNRCB46WH,
    .bindings_count=sizeof(kBindings_WNRCB46WH)/sizeof(kBindings_WNRCB46WH[0]),
};

}  // namespace zhc::devices::legrand
