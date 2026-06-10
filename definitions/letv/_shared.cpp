// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: LeTV shared converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/letv.ts
//             + converters/fromZigbee.ts (qlwz_letv8key_switch).
#include "definitions/letv/_shared.hpp"

namespace zhc::devices::letv {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;

namespace {

// ── kFzLetv8KeyAction ───────────────────────────────────────────────
//
// genMultistateInput.presentValue (attr 0x0055) + source endpoint →
// "action" string, mirroring z2m's fz.qlwz_letv8key_switch:
//
//   buttonLookup = {4:up, 2:down, 5:left, 3:right, 8:center, 1:back,
//                   7:play, 6:voice}      (keyed by msg.endpoint.ID)
//   actionLookup = {0:hold, 1:single, 2:double, 3:tripple}  (presentValue)
//   action       = `${gesture}_${button}`
//
// z2m only emits when the endpoint maps to a known button; an unknown
// presentValue in z2m falls through to the raw number, but the static
// `action` enum only declares the four named gestures, so we emit nothing
// for an out-of-range gesture (no phantom numeric action key).
//
// Every emitted string is a compile-time literal from the table below, so
// the StringRef stored in the payload outlives the dispatch call. The
// button is encoded *in* the action string (matching z2m), hence this
// converter resolves src_endpoint itself rather than relying on an
// endpoint_map suffix — the def carries NO endpoint_map.

// [button_index][gesture] → action literal. Column order = gesture
// (0:hold, 1:single, 2:double, 3:tripple). Row order = the button slots,
// indexed via kEndpointToButton below.
constexpr const char* kActions[8][4] = {
    {"hold_up",     "single_up",     "double_up",     "tripple_up"},
    {"hold_down",   "single_down",   "double_down",   "tripple_down"},
    {"hold_left",   "single_left",   "double_left",   "tripple_left"},
    {"hold_right",  "single_right",  "double_right",  "tripple_right"},
    {"hold_center", "single_center", "double_center", "tripple_center"},
    {"hold_back",   "single_back",   "double_back",   "tripple_back"},
    {"hold_play",   "single_play",   "double_play",   "tripple_play"},
    {"hold_voice",  "single_voice",  "double_voice",  "tripple_voice"},
};

// Map a source endpoint ID to a row in kActions, or -1 if unmapped.
// z2m buttonLookup: {4:up, 2:down, 5:left, 3:right, 8:center, 1:back,
//                    7:play, 6:voice}.
int button_row(std::uint8_t ep) {
    switch (ep) {
        case 4: return 0;  // up
        case 2: return 1;  // down
        case 5: return 2;  // left
        case 3: return 3;  // right
        case 8: return 4;  // center
        case 1: return 5;  // back
        case 7: return 6;  // play
        case 6: return 7;  // voice
        default: return -1;
    }
}

bool fz_letv8key_action(const DecodedMessage& msg,
                        const ::zhc::FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const int row = button_row(msg.src_endpoint);
    if (row < 0) return false;  // endpoint maps to no button (z2m drops it)

    // attr 0x0055 — PresentValue (multistate, reported as Uint/Int).
    const Value* v = msg.payload.find("85");
    if (v == nullptr) return false;
    std::uint64_t pv;
    if      (v->type == ValueType::Uint) pv = v->u;
    else if (v->type == ValueType::Int && v->i >= 0)
        pv = static_cast<std::uint64_t>(v->i);
    else return false;
    if (pv > 3) return false;  // gesture out of {0..3}; no numeric fallthrough

    Value o{};
    o.type = ValueType::StringRef;
    o.str  = kActions[row][pv];
    out.put("action", o);
    return true;
}

}  // namespace

const ::zhc::FzConverter kFzLetv8KeyAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genMultistateInput",
    .type_mask         = ::zhc::type_bit(MessageType::AttributeReport) |
                         ::zhc::type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_letv8key_action },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::letv
