// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Linxura smart controllers — SCHA-1-MO (4 buttons) and SHCB-1-MO
// Aura (12 buttons). New vendor upstream in z2m v26.101.0.
//
// Both encode every button press into the IAS Zone `zonestatus` field rather
// than using a scene or on/off cluster, so one decoder covers the family:
//
//   status = (button - 1) * 6 + offset + 1,  offset in {0, 2, 4}
//     offset 0 -> click        offset 2 -> double_click      offset 4 -> hold
//
// i.e. status 1/3/5 are button 1's three gestures, 7/9/11 button 2's, and so
// on. Odd-looking values (offsets 1, 3, 5) and anything past `buttons * 6 - 1`
// are not gestures and are dropped, matching z2m's guard exactly.
//
// The action strings are a static table because ZHC publishes them as
// StringRef and the pointer must outlive the payload; 12 buttons x 3 gestures
// is 36 literals, ~450 bytes of .rodata, which beats formatting on the decode
// path.
//
// z2m-source: linxura.ts #SCHA-1-MO / #SHCB-1-MO (linxuraButton).
#include "definitions/_generic/_shared.hpp"

#include <cstdint>

namespace zhc::devices::linxura {
namespace {

constexpr std::uint8_t kMaxButtons = 12;

// [button-1][gesture] where gesture 0=click, 1=double_click, 2=hold.
constexpr const char* kActions[kMaxButtons][3] = {
    {"button_1_click",  "button_1_double_click",  "button_1_hold"},
    {"button_2_click",  "button_2_double_click",  "button_2_hold"},
    {"button_3_click",  "button_3_double_click",  "button_3_hold"},
    {"button_4_click",  "button_4_double_click",  "button_4_hold"},
    {"button_5_click",  "button_5_double_click",  "button_5_hold"},
    {"button_6_click",  "button_6_double_click",  "button_6_hold"},
    {"button_7_click",  "button_7_double_click",  "button_7_hold"},
    {"button_8_click",  "button_8_double_click",  "button_8_hold"},
    {"button_9_click",  "button_9_double_click",  "button_9_hold"},
    {"button_10_click", "button_10_double_click", "button_10_hold"},
    {"button_11_click", "button_11_double_click", "button_11_hold"},
    {"button_12_click", "button_12_double_click", "button_12_hold"},
};

// How many buttons the matched device has; supplied via user_config.
constexpr std::uint8_t kFourButtons   = 4;
constexpr std::uint8_t kTwelveButtons = 12;

bool fz_linxura_button(const DecodedMessage& msg,
                        const FzConverter& self,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* cfg = static_cast<const std::uint8_t*>(self.user_config);
    const std::uint8_t buttons = cfg ? *cfg : kFourButtons;

    // zoneStatus arrives either as attribute 0x0002 on a report or as the
    // first field of a commandStatusChangeNotification body.
    std::int64_t status = -1;
    if (const Value* v = msg.payload.find("2")) {
        if      (v->type == ValueType::Uint) status = static_cast<std::int64_t>(v->u);
        else if (v->type == ValueType::Int)  status = v->i;
    } else if (msg.type == MessageType::Command && msg.raw_body.size() >= 2) {
        status = static_cast<std::int64_t>(msg.raw_body[0]) |
                 (static_cast<std::int64_t>(msg.raw_body[1]) << 8);
    }
    if (status < 1 || status > static_cast<std::int64_t>(buttons) * 6 - 1) return false;

    const std::int64_t offset = (status - 1) % 6;
    if (offset != 0 && offset != 2 && offset != 4) return false;

    const std::int64_t button = (status - 1) / 6;      // 0-based
    if (button < 0 || button >= kMaxButtons) return false;

    Value action{}; action.type = ValueType::StringRef;
    action.str = kActions[button][offset / 2];
    out.put("action", action);
    return true;
}

const FzConverter kFzLinxura4{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse) |
                         type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_linxura_button },
    .user_config       = &kFourButtons,
};

const FzConverter kFzLinxura12{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse) |
                         type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_linxura_button },
    .user_config       = &kTwelveButtons,
};

const FzConverter* const kFz_SCHA[] = { &kFzLinxura4 };
const FzConverter* const kFz_SHCB[] = { &kFzLinxura12, &::zhc::generic::kFzBattery };

constexpr const char* kModels_SCHA[] = { "Smart Controller" };
constexpr const char* kManus_SCHA[]  = { "Linxura" };
constexpr const char* kModels_SHCB[] = { "Aura Smart Button" };
constexpr const char* kManus_SHCB[]  = { "Linxura" };

constexpr Expose kExposes_SCHA[] = {
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr Expose kExposes_SHCB[] = {
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_SCHA[] = {
    {1, 0x0500},   // ssIasZone — carries the button presses
};
constexpr BindingSpec kBindings_SHCB[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0500},   // ssIasZone
};

}  // namespace

extern const PreparedDefinition kDef_SCHA_1_MO{
    .zigbee_models=kModels_SCHA, .zigbee_models_count=sizeof(kModels_SCHA)/sizeof(kModels_SCHA[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SCHA, .manufacturer_names_count=sizeof(kManus_SCHA)/sizeof(kManus_SCHA[0]),
    .model="SCHA-1-MO", .vendor="Linxura",
    .meta=nullptr,
    .exposes=kExposes_SCHA, .exposes_count=sizeof(kExposes_SCHA)/sizeof(kExposes_SCHA[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SCHA, .from_zigbee_count=sizeof(kFz_SCHA)/sizeof(kFz_SCHA[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SCHA, .bindings_count=sizeof(kBindings_SCHA)/sizeof(kBindings_SCHA[0]),
};

extern const PreparedDefinition kDef_SHCB_1_MO{
    .zigbee_models=kModels_SHCB, .zigbee_models_count=sizeof(kModels_SHCB)/sizeof(kModels_SHCB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SHCB, .manufacturer_names_count=sizeof(kManus_SHCB)/sizeof(kManus_SHCB[0]),
    .model="SHCB-1-MO", .vendor="Linxura",
    .meta=nullptr,
    .exposes=kExposes_SHCB, .exposes_count=sizeof(kExposes_SHCB)/sizeof(kExposes_SHCB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SHCB, .from_zigbee_count=sizeof(kFz_SHCB)/sizeof(kFz_SHCB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SHCB, .bindings_count=sizeof(kBindings_SHCB)/sizeof(kBindings_SHCB[0]),
};

}  // namespace zhc::devices::linxura
