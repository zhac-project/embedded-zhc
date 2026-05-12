// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Legrand vendor converters. See `_shared.hpp` for cluster /
// attribute reference. All TZ writes use the generic
// `tz_zcl_write_attr` encoder with mfgcode=0x1021 to stamp out the
// fc=0x14 + manu-code + writeAttributes(0x02) wire frame.

#include "_shared.hpp"

#include <cstring>
#include <string_view>

namespace zhc::legrand {

using ::zhc::FixedPayload;
using ::zhc::FzConverter;
using ::zhc::TzConverter;
using ::zhc::Direction;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::DecodedMessage;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::type_bit;

// ── fz_cluster_fc01 ─────────────────────────────────────────────────
//
// Payload shape matches the generic ZCL fz path: attribute IDs are
// the decimal-string keys of `msg.payload`. We decode 0x0001 and
// 0x0002 — both BOOLEAN — and emit them as "ON"/"OFF" strings.

bool fz_cluster_fc01(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    auto emit_bool_as_onoff = [&](const char* dec_attr,
                                   const char* out_key) {
        const Value* v = msg.payload.find(dec_attr);
        if (!v) return;
        bool on = false;
        if (v->type == ValueType::Bool)      on = v->b;
        else if (v->type == ValueType::Uint) on = v->u != 0;
        else if (v->type == ValueType::Int)  on = v->i != 0;
        else return;
        Value s{};
        s.type = ValueType::StringRef;
        s.str  = on ? "ON" : "OFF";
        out.put(out_key, s);
        emitted = true;
    };

    emit_bool_as_onoff("1", "led_in_dark");  // attr 0x0001
    emit_bool_as_onoff("2", "led_if_on");    // attr 0x0002
    return emitted;
}

extern const FzConverter kFzClusterFc01{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificLegrandDevices",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                          type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_cluster_fc01 },
    .user_config       = nullptr,
};

// ── fz_pilot_wire_mode ──────────────────────────────────────────────

namespace {

constexpr const char* kPilotWireLabels[] = {
    "comfort",          // 0
    "comfort_-1",       // 1
    "comfort_-2",       // 2
    "eco",              // 3
    "frost_protection", // 4
    "off",              // 5
};

}  // namespace

bool fz_pilot_wire_mode(const DecodedMessage& msg,
                         const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");  // attr 0x0000 pilotWireMode
    if (!v) return false;
    std::uint64_t raw = 0;
    if (v->type == ValueType::Uint)      raw = v->u;
    else if (v->type == ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
    else return false;
    if (raw >= sizeof(kPilotWireLabels) / sizeof(kPilotWireLabels[0])) {
        return false;
    }
    Value s{};
    s.type = ValueType::StringRef;
    s.str  = kPilotWireLabels[raw];
    out.put("pilot_wire_mode", s);
    return true;
}

extern const FzConverter kFzPilotWireMode{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificLegrandDevices2",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                          type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_pilot_wire_mode },
    .user_config       = nullptr,
};

// ── tz writes on 0xFC01 ─────────────────────────────────────────────
//
// Both led_in_dark and led_if_on accept the "ON"/"OFF" strings z2m
// emits, plus raw bool. The lookup gets us string-coercion through
// the generic encoder; bool inputs short-circuit it.

namespace {

constexpr ::zhc::generic::ZclWriteLookup kOnOffLookup[] = {
    { "OFF", 0 },
    { "ON",  1 },
};

constexpr ::zhc::generic::ZclWriteSpec kSpecLedInDark{
    "led_in_dark",
    /*attr_id=*/0x0001,
    /*attr_type=*/0x10,                 // BOOLEAN
    /*manufacturer_code=*/kMfgLegrand,  // 0x1021
    kOnOffLookup,
    static_cast<std::uint8_t>(sizeof(kOnOffLookup) / sizeof(kOnOffLookup[0])),
};

constexpr ::zhc::generic::ZclWriteSpec kSpecLedIfOn{
    "led_if_on",
    /*attr_id=*/0x0002,
    /*attr_type=*/0x10,
    kMfgLegrand,
    kOnOffLookup,
    static_cast<std::uint8_t>(sizeof(kOnOffLookup) / sizeof(kOnOffLookup[0])),
};

// device_mode → DATA16 on attr 0x0000. z2m treats DATA16 like a u16
// LUT; ZclWriteSpec uses 0x21 (UINT16) which encodes identically on
// the wire (LE byte pair, no semantic difference for these fixed
// values).
constexpr ::zhc::generic::ZclWriteLookup kDeviceModeLookup[] = {
    { "dimmer_off", 0x0100 },
    { "dimmer_on",  0x0101 },
    { "pilot_off",  0x0001 },
    { "pilot_on",   0x0002 },
    { "switch",     0x0003 },
    { "auto",       0x0004 },
};

constexpr ::zhc::generic::ZclWriteSpec kSpecDeviceMode{
    "device_mode",
    /*attr_id=*/0x0000,
    /*attr_type=*/0x21,  // UINT16 (wire-compatible with DATA16)
    kMfgLegrand,
    kDeviceModeLookup,
    static_cast<std::uint8_t>(sizeof(kDeviceModeLookup) / sizeof(kDeviceModeLookup[0])),
};

}  // namespace

#define ZHC_LEGRAND_TZ(var, spec_ref, key_str, cluster_str, cid)      \
    extern const TzConverter var{                                     \
        .key         = key_str,                                       \
        .cluster     = cluster_str,                                   \
        .cluster_id  = (cid),                                         \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &spec_ref,                                     \
    };

ZHC_LEGRAND_TZ(kTzLedInDark,  kSpecLedInDark,  "led_in_dark",
                "manuSpecificLegrandDevices", 0xFC01)
ZHC_LEGRAND_TZ(kTzLedIfOn,    kSpecLedIfOn,    "led_if_on",
                "manuSpecificLegrandDevices", 0xFC01)
ZHC_LEGRAND_TZ(kTzDeviceMode, kSpecDeviceMode, "device_mode",
                "manuSpecificLegrandDevices", 0xFC01)

#undef ZHC_LEGRAND_TZ

}  // namespace zhc::legrand
