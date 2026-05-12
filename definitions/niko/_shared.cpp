// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Niko shared converters — cluster 0xFC00 (NikoConfig) + 0xFC01
// (NikoState). Cloned from the lumi `_shared.cpp` macro pattern;
// `tz_zcl_write_attr` is the generic encoder declared in
// `_generic/_shared.hpp`. Manufacturer code = NIKO_NV (0x125F).
//
// z2m-source: niko.ts `local.tz.*` / `local.fz.*`.
#include "definitions/niko/_shared.hpp"

#include <cstring>
#include <string_view>

namespace zhc::devices::niko {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::WILDCARD_ATTR_ID;
using ::zhc::WILDCARD_CMD_ID;
using ::zhc::WILDCARD_ENDPOINT;
using ::zhc::type_bit;

// ── Tz lookup tables (string→uint) ─────────────────────────────────
namespace {

constexpr ::zhc::generic::ZclWriteLookup kLkOperationMode[] = {
    {"control_relay", 0x02},
    {"decoupled",     0x01},
};

constexpr ::zhc::generic::ZclWriteLookup kLkChildLock[] = {
    {"LOCK",   0x00},
    {"UNLOCK", 0x01},
    {"lock",   0x00},
    {"unlock", 0x01},
};

constexpr ::zhc::generic::ZclWriteLookup kLkLedColor[] = {
    {"OFF",    0x00000000},
    {"ON",     0x000000FF},
    {"Blue",   0x0000FF00},
    {"Red",    0x00FF0000},
    {"Purple", 0x00FFFFFF},
};

constexpr ::zhc::generic::ZclWriteLookup kLkLedState[] = {
    {"OFF", 0},
    {"ON",  1},
};

constexpr ::zhc::generic::ZclWriteLookup kLkActionReporting[] = {
    {"false", 0x01},
    {"true",  0x1F},
};

// ── ZclWriteSpec instances ─────────────────────────────────────────
// Manu code = 0x125F (NIKO_NV).

constexpr ::zhc::generic::ZclWriteSpec kSpecOperationMode{
    "operation_mode", 0x0000, 0x30 /*enum8*/, 0x125F,
    kLkOperationMode, sizeof(kLkOperationMode)/sizeof(kLkOperationMode[0]),
};

constexpr ::zhc::generic::ZclWriteSpec kSpecChildLock{
    "child_lock", 0x0101, 0x20 /*u8*/, 0x125F,
    kLkChildLock, sizeof(kLkChildLock)/sizeof(kLkChildLock[0]),
};

constexpr ::zhc::generic::ZclWriteSpec kSpecLedEnable{
    "led_enable", 0x0104, 0x20 /*u8*/, 0x125F,
    nullptr, 0,
};

constexpr ::zhc::generic::ZclWriteSpec kSpecLedColor{
    "led_color", 0x0100, 0x22 /*u24*/, 0x125F,
    kLkLedColor, sizeof(kLkLedColor)/sizeof(kLkLedColor[0]),
};

constexpr ::zhc::generic::ZclWriteSpec kSpecLedState{
    "led_state", 0x0105, 0x18 /*bitmap8*/, 0x125F,
    kLkLedState, sizeof(kLkLedState)/sizeof(kLkLedState[0]),
};

// led_sync_mode: 0x0107 bitmap32. Generic `tz_zcl_write_attr` only
// supports types up to 4 bytes via 0x23 (u32) and bitmap8/16 via
// 0x18/0x19 — bitmap32 (0x1B) is not in `zcl_type_len`. We therefore
// expose this attribute as `BLOCKED` in the parity doc and do not
// register a tz converter for it. Adding bitmap32 to the generic
// encoder is the correct upstream fix.

constexpr ::zhc::generic::ZclWriteSpec kSpecActionReporting{
    "action_reporting", 0x0001, 0x18 /*bitmap8*/, 0x125F,
    kLkActionReporting, sizeof(kLkActionReporting)/sizeof(kLkActionReporting[0]),
};

}  // namespace

// ── Tz macros — cluster 0xFC00 (NikoConfig) ────────────────────────
#define ZHC_NIKO_TZ_FC00(var, spec_ref, key_str)                      \
    extern const TzConverter var{                                     \
        .key         = key_str,                                       \
        .cluster     = "manuSpecificNikoConfig",                      \
        .cluster_id  = 0xFC00,                                        \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &spec_ref,                                     \
    };

ZHC_NIKO_TZ_FC00(kTzNikoOperationMode, kSpecOperationMode, "operation_mode")
ZHC_NIKO_TZ_FC00(kTzNikoChildLock,     kSpecChildLock,     "child_lock")
ZHC_NIKO_TZ_FC00(kTzNikoLedEnable,     kSpecLedEnable,     "led_enable")
ZHC_NIKO_TZ_FC00(kTzNikoLedColor,      kSpecLedColor,      "led_color")
ZHC_NIKO_TZ_FC00(kTzNikoLedState,      kSpecLedState,      "led_state")
#undef ZHC_NIKO_TZ_FC00

// ── Tz macros — cluster 0xFC01 (NikoState) ─────────────────────────
extern const TzConverter kTzNikoActionReporting{
    .key         = "action_reporting",
    .cluster     = "manuSpecificNikoState",
    .cluster_id  = 0xFC01,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecActionReporting,
};

// ── Fz: NikoConfig (cluster 0xFC00) ────────────────────────────────
//
// Decodes whatever attributes are present in `msg.payload` (keyed by
// decimal attr id string per the existing convention). For multi-
// endpoint LED state/sync we only emit the raw uint — per-endpoint
// fan-out happens in the consumer. PARTIAL parity for those attrs.

namespace {

bool fz_niko_config(const DecodedMessage& msg,
                     const FzConverter&,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // 0x0000 switchOperationMode — enum8.
    if (const Value* v = msg.payload.find("0"); v && v->type == ValueType::Uint) {
        Value o{}; o.type = ValueType::StringRef;
        switch (v->u) {
            case 2: o.str = "control_relay"; break;
            case 1: o.str = "decoupled";     break;
            default: o.str = "unknown";      break;
        }
        out.put("operation_mode", o);
        emitted = true;
    }

    // 0x0101 outletChildLock — u8 (0=lock, 1=unlock).
    if (const Value* v = msg.payload.find("257"); v && v->type == ValueType::Uint) {
        Value o{}; o.type = ValueType::StringRef;
        o.str = (v->u == 0) ? "LOCK" : "UNLOCK";
        out.put("child_lock", o);
        emitted = true;
    }

    // 0x0104 ledEnable — u8 (0/1).
    if (const Value* v = msg.payload.find("260"); v && v->type == ValueType::Uint) {
        Value o{}; o.type = ValueType::Bool; o.b = (v->u == 1);
        out.put("led_enable", o);
        emitted = true;
    }

    // 0x0100 outletLedColor — u24.
    if (const Value* v = msg.payload.find("256"); v && v->type == ValueType::Uint) {
        Value o{}; o.type = ValueType::StringRef;
        switch (v->u) {
            case 0x00000000: o.str = "OFF";    break;
            case 0x000000FF: o.str = "ON";     break;
            case 0x0000FF00: o.str = "Blue";   break;
            case 0x00FF0000: o.str = "Red";    break;
            case 0x00FFFFFF: o.str = "Purple"; break;
            default:         o.str = "unknown"; break;
        }
        out.put("led_color", o);
        emitted = true;
    }

    // 0x0105 ledState — bitmap8. Single-endpoint mapping only.
    if (const Value* v = msg.payload.find("261"); v && v->type == ValueType::Uint) {
        Value o{}; o.type = ValueType::StringRef;
        o.str = (v->u & 0x01) ? "ON" : "OFF";
        out.put("led_state", o);
        emitted = true;
    }

    // 0x0107 ledSyncMode — bitmap32. Emit the raw uint; the consumer
    // does the per-endpoint shift if they need it. PARTIAL parity.
    if (const Value* v = msg.payload.find("263"); v && v->type == ValueType::Uint) {
        Value o{}; o.type = ValueType::Uint; o.u = v->u;
        out.put("led_sync_mode_raw", o);
        emitted = true;
    }

    return emitted;
}

// ── Fz: NikoState switchAction (cluster 0xFC01) ────────────────────
//
// We expose `action_reporting` (bool) and `action_raw` (uint32) only.
// The TS reference does considerable bit-fiddling on `switchAction` to
// emit named events like `single_left` / `hold_right_ext`. Ports that
// surface "action" expose entries should compose the string in the
// runtime layer — emitting the raw 32-bit field here keeps `_shared`
// stateless (no static buffers, no model branching) and matches the
// rest of the ZHC convention for opaque button events. PARTIAL parity
// for the named-event mapping (BLOCKED until the runtime gets a
// per-device shifted-button decoder).

bool fz_niko_state(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // 0x0001 switchActionReporting — bitmap8 (any of bits 1..4 set).
    if (const Value* v = msg.payload.find("1"); v && v->type == ValueType::Uint) {
        Value o{}; o.type = ValueType::Bool; o.b = (v->u & 0x1E) != 0;
        out.put("action_reporting", o);
        emitted = true;
    }

    // 0x0002 switchAction — bitmap32 → forwarded as uint.
    if (const Value* v = msg.payload.find("2");
            v && (v->type == ValueType::Uint || v->type == ValueType::Int)) {
        Value o{}; o.type = ValueType::Uint;
        o.u = (v->type == ValueType::Uint) ? v->u
                                            : static_cast<std::uint64_t>(v->i);
        out.put("action_raw", o);
        emitted = true;
    }

    return emitted;
}

}  // namespace

extern const FzConverter kFzNikoConfig{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificNikoConfig",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_niko_config },
    .user_config       = nullptr,
};

extern const FzConverter kFzNikoSwitchAction1Btn{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificNikoState",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_niko_state },
    .user_config       = nullptr,
};

extern const FzConverter kFzNikoSwitchAction2Btn{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificNikoState",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_niko_state },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::niko
