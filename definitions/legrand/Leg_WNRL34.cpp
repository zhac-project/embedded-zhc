// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand WNRL34, wired home & away switch, new in z2m
// v26.126.0..v26.133.0.
// legrand.fz.scenes: every press is a genScenes recall (client to server); the
// group id in the command BODY (bytes 0-1, LE; not the APS group) picks the
// action: 65527 enter, 65526 leave, 65524 sleep, 65525 wakeup, 65518-65520
// ambiance_I..III, anything else "default" (all of these are published,
// although upstream exposes only enter / leave / sleep / wakeup). Endpoint 1
// genIdentify bound, as upstream (its only configure step).
// zigbeeModel: the leading-space form and the trimmed form, as upstream.
// Not ported: the manuSpecificLegrandDevices (0xFC01) cluster upstream adds;
// no attribute of it is read or exposed for this model.
// z2m-source: legrand.ts #WNRL34.
#include "definitions/_generic/_shared.hpp"

#include <cstdint>

namespace zhc::devices::legrand {
namespace {

bool fz_wnrl34_scenes(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.size() < 2) return false;   // recall body: groupid u16, sceneid u8
    const std::uint16_t group = static_cast<std::uint16_t>(
        msg.raw_body[0] | (static_cast<std::uint16_t>(msg.raw_body[1]) << 8));
    const char* label;
    switch (group) {
        case 65527: label = "enter";        break;
        case 65526: label = "leave";        break;
        case 65524: label = "sleep";        break;
        case 65525: label = "wakeup";       break;
        case 65518: label = "ambiance_I";   break;
        case 65519: label = "ambiance_II";  break;
        case 65520: label = "ambiance_III"; break;
        default:    label = "default";      break;
    }
    Value v{}; v.type = ValueType::StringRef; v.str = label;
    out.put("action", v);
    return true;
}

const FzConverter kFzWnrl34Scenes{
    .family            = FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x05,   // recall
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_wnrl34_scenes },
    .user_config       = nullptr,
};

const FzConverter* const kFz_WNRL34[] = { &kFzWnrl34Scenes };

constexpr const char* kModels_WNRL34[] = { " Wired master SW Home / Away", "Wired master SW Home / Away" };

constexpr const char* kActionValues[] = { "enter", "leave", "sleep", "wakeup" };

constexpr Expose kExposes_WNRL34[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActionValues, sizeof(kActionValues)/sizeof(kActionValues[0])},
};

constexpr BindingSpec kBindings_WNRL34[] = {
    {1, 0x0003},
};

}  // namespace

extern const PreparedDefinition kDef_WNRL34{
    .zigbee_models=kModels_WNRL34, .zigbee_models_count=sizeof(kModels_WNRL34)/sizeof(kModels_WNRL34[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WNRL34", .vendor="Legrand",
    .meta=nullptr, .exposes=kExposes_WNRL34, .exposes_count=sizeof(kExposes_WNRL34)/sizeof(kExposes_WNRL34[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WNRL34, .from_zigbee_count=sizeof(kFz_WNRL34)/sizeof(kFz_WNRL34[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_WNRL34, .bindings_count=sizeof(kBindings_WNRL34)/sizeof(kBindings_WNRL34[0]),
};

}  // namespace zhc::devices::legrand
