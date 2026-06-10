// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0

#include "definitions/meazon/_shared.hpp"

#include <cstdint>

namespace zhc::meazon {

namespace {

// Raw unsigned pass-through (u16/u24/u32 wire types). Mirrors the
// generic metering/electrical convention; the runtime scales downstream.
bool put_uint(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key,
              const Value* v) {
    if (!v) return false;
    if (v->type != ValueType::Uint && v->type != ValueType::Int) return false;
    Value o{};
    o.type = ValueType::Uint;
    o.u    = v->type == ValueType::Uint ? v->u
                                        : static_cast<std::uint64_t>(v->i);
    return out.put(key, o);
}

// Raw signed pass-through (s16/s24/s32 wire types). Accepts an unsigned
// record too — mirroring the generic active-power branch.
bool put_int(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key,
             const Value* v) {
    if (!v) return false;
    if (v->type != ValueType::Int && v->type != ValueType::Uint) return false;
    Value o{};
    o.type = ValueType::Int;
    o.i    = v->type == ValueType::Int ? v->i
                                       : static_cast<std::int64_t>(v->u);
    return out.put(key, o);
}

// ── fz.meazon_meter (seMetering 0x0702, MEAZON_S_A manufacturer attrs) ─
//
//   0x2001 (8193)  -> power     (signed; consumption/production)
//   0x2004 (8196)  -> voltage   (primary slot)
//   0x2015 (8213)  -> voltage   (alternate slot — last write wins)
//   0x2007 (8199)  -> current   (primary slot)
//   0x2018 (8216)  -> current   (alternate slot)
//   0x3000 (12288) -> energy    (BIZY_PLUG only; DINRAIL exposes none)
//
// z2m emits whichever attrs are present (each `if (msg.data[..])`); a
// report carries one or a few at a time, so the converter decodes every
// present slot and merges. The DINRAIL def has no `energy` expose, so a
// 0x3000 report there is harmless (key merged but unexposed) — kept here
// to keep the converter shared across both defs.
bool fz_meazon_meter(const DecodedMessage& msg,
                     const FzConverter&,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // power — attr 0x2001 (8193), signed.
    if (put_int(out, "power", msg.payload.find("8193"))) emitted = true;

    // voltage — attr 0x2004 (8196) primary, 0x2015 (8213) alternate.
    if (put_uint(out, "voltage", msg.payload.find("8196"))) emitted = true;
    if (put_uint(out, "voltage", msg.payload.find("8213"))) emitted = true;

    // current — attr 0x2007 (8199) primary, 0x2018 (8216) alternate.
    if (put_uint(out, "current", msg.payload.find("8199"))) emitted = true;
    if (put_uint(out, "current", msg.payload.find("8216"))) emitted = true;

    // energy — attr 0x3000 (12288) currentSummDelivered (BIZY_PLUG).
    if (put_uint(out, "energy", msg.payload.find("12288"))) emitted = true;

    return emitted;
}

}  // namespace

extern const FzConverter kFzMeazonMeter{
    .family            = FrameFamily::Zcl,
    .cluster           = "seMetering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_meazon_meter },
    .user_config       = nullptr,
};

}  // namespace zhc::meazon
