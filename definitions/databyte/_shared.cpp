// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Databyte (databyte.ch) vendor converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/databyte.ts
//             + converters/fromZigbee.ts (fz.DTB190502A1).

#include "definitions/databyte/_shared.hpp"

namespace zhc::databyte {

using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::Direction;
using ::zhc::type_bit;
using ::zhc::WILDCARD_CMD_ID;
using ::zhc::WILDCARD_ATTR_ID;
using ::zhc::WILDCARD_ENDPOINT;

namespace {

// Read a genOnOff manufacturer-specific attribute by its decimal-string
// key and return its unsigned value, or false when absent / wrong type.
bool read_u(const FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& payload,
            const char* key, std::uint64_t& out_val) {
    const Value* v = payload.find(key);
    if (!v) return false;
    if (v->type == ValueType::Uint)      out_val = v->u;
    else if (v->type == ValueType::Int)  out_val = static_cast<std::uint64_t>(v->i);
    else if (v->type == ValueType::Bool) out_val = v->b ? 1u : 0u;
    else return false;
    return true;
}

void put_on_off(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
                const char* key, std::uint64_t raw) {
    Value o{}; o.type = ValueType::StringRef;
    o.str = (raw == 1) ? "ON" : "OFF";
    out.put(key, o);
}

// ── DTB190502A1 — CC2530 IO board ───────────────────────────────────
//
// z2m fz.DTB190502A1 (genOnOff attributeReport / readResponse):
//   41361 (0xA191) → cpu_temperature  precisionRound(v, 2)
//   41362 (0xA192) → key_state         {0:SYS,1:UP,2:DOWN,3:NONE}
//   41363 (0xA193) → led_state         {0:OFF,1:ON}
bool fz_dtb190502a1(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    std::uint64_t raw = 0;

    if (read_u(msg.payload, "41361", raw)) {
        Value t{}; t.type = ValueType::Float; t.f = static_cast<float>(raw);
        out.put("cpu_temperature", t);
        emitted = true;
    }

    if (read_u(msg.payload, "41362", raw)) {
        const char* label = nullptr;
        switch (raw) {
            case 0: label = "KEY_SYS";  break;
            case 1: label = "KEY_UP";   break;
            case 2: label = "KEY_DOWN"; break;
            case 3: label = "KEY_NONE"; break;
        }
        if (label) {
            Value k{}; k.type = ValueType::StringRef; k.str = label;
            out.put("key_state", k);
            emitted = true;
        }
    }

    if (read_u(msg.payload, "41363", raw)) {
        put_on_off(out, "led_state", raw);
        emitted = true;
    }

    return emitted;
}

// ── Touch4 / DTB-ED2011-014 — 4-key wall touchsensor ────────────────
//
// z2m fzLocal.DTB2011014 (genOnOff attributeReport / readResponse):
//   41361..41364 (0xA191..0xA194) → key_1..key_4  {1:ON, else OFF}
bool fz_touch4_keys(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    static constexpr struct { const char* attr; const char* key; } kMap[] = {
        {"41361", "key_1"}, {"41362", "key_2"},
        {"41363", "key_3"}, {"41364", "key_4"},
    };
    bool emitted = false;
    std::uint64_t raw = 0;
    for (const auto& m : kMap) {
        if (read_u(msg.payload, m.attr, raw)) {
            put_on_off(out, m.key, raw);
            emitted = true;
        }
    }
    return emitted;
}

}  // namespace

extern const FzConverter kFzDtb190502a1{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_dtb190502a1 },
    .user_config       = nullptr,
};

extern const FzConverter kFzTouch4Keys{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_touch4_keys },
    .user_config       = nullptr,
};

}  // namespace zhc::databyte
