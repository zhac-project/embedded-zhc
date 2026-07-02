// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Namron Simplify 6-button remote 4512793 / 4512794 (z2m v26.77.0
// parity). z2m-source: namron.ts #4512793 + fzLocal.namronSimplifyRemote.
// Graduated from a broken auto-extract stub (kFzOnOff + phantom state/voltage
// + genOnOff/genPowerCfg binds) — this is a scene remote, not a switch. Button
// presses arrive as a raw frame on the private cluster namronPrivateE004
// (0xE004). The last two payload bytes carry (button, action): button 1..6 maps
// to a 3-column up/down layout (col = (btn-1)/2, sub = odd->up / even->down);
// action 0=press, 1=release, 2=hold. Emits `action` = "button_<1-3>_<up|down>_
// <press|release|hold>". NOTE: 0xE004 also resolves to "zosungIRControl" in the
// shared cluster-name table, so the converter matches any cluster (.cluster=
// nullptr) and is scoped by being listed only in this def's from_zigbee.
// The z2m hold->release synthesis + press-then-release double publish are
// stateful UI conveniences and are not reproduced (single action per frame).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::namron {
namespace {

// [column 0..2][sub 0=up/1=down][verb 0=press/1=release/2=hold].
constexpr const char* kActionTable[3][2][3] = {
    { {"button_1_up_press",   "button_1_up_release",   "button_1_up_hold"},
      {"button_1_down_press", "button_1_down_release", "button_1_down_hold"} },
    { {"button_2_up_press",   "button_2_up_release",   "button_2_up_hold"},
      {"button_2_down_press", "button_2_down_release", "button_2_down_hold"} },
    { {"button_3_up_press",   "button_3_up_release",   "button_3_up_hold"},
      {"button_3_down_press", "button_3_down_release", "button_3_down_hold"} },
};

bool fz_namron_simplify(const ::zhc::DecodedMessage& msg, const ::zhc::FzConverter&,
                        const ::zhc::PreparedDefinition&, ::zhc::RuntimeContext&,
                        ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::size_t n = msg.raw_body.size();
    if (n < 2) return false;
    const std::uint8_t btn = msg.raw_body[n - 2];
    const std::uint8_t raw = msg.raw_body[n - 1];
    if (btn < 1 || btn > 6) return false;
    int verb;
    switch (raw) {
        case 0: verb = 0; break;   // press
        case 1: verb = 1; break;   // release
        case 2: verb = 2; break;   // hold
        default: return false;
    }
    const int col = (btn - 1) / 2;            // 0..2
    const int sub = (btn % 2 == 1) ? 0 : 1;   // odd -> up, even -> down
    ::zhc::Value a{};
    a.type = ::zhc::ValueType::StringRef;
    a.str  = kActionTable[col][sub][verb];
    out.put("action", a);
    return true;
}

constexpr ::zhc::FzConverter kFzNamronSimplifyRemote{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = nullptr,   // 0xE004 shares the "zosungIRControl" name; match any (per-def scoped)
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command) |
                         ::zhc::type_bit(::zhc::MessageType::Raw),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_namron_simplify },
    .user_config       = nullptr,
};

const FzConverter* const kFz_D4512793[] = {
    &kFzNamronSimplifyRemote,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D4512793[] = { "4512793", "4512794" };

// z2m exposes e.action([...18 strings...]); ZHC has no enum/list action type,
// so `action` stays String/State (strings produced by kFzNamronSimplifyRemote).
constexpr Expose kExp_D4512793[] = {
    {"action",  ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
};
}  // namespace

extern const PreparedDefinition kDef_D4512793{
    .zigbee_models=kModels_D4512793, .zigbee_models_count=sizeof(kModels_D4512793)/sizeof(kModels_D4512793[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="4512793", .vendor="Namron",
    .meta=nullptr, .exposes=kExp_D4512793, .exposes_count=sizeof(kExp_D4512793)/sizeof(kExp_D4512793[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D4512793, .from_zigbee_count=sizeof(kFz_D4512793)/sizeof(kFz_D4512793[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::namron
