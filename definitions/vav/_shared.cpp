// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared VAV vendor converters. See `_shared.hpp` for cluster /
// attribute reference.

#include "_shared.hpp"

namespace zhc::vav {

using ::zhc::FixedPayload;
using ::zhc::FzConverter;
using ::zhc::Direction;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::DecodedMessage;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::type_bit;

// ── fz_pilot_wire_mode ──────────────────────────────────────────────

namespace {

constexpr const char* kPilotWireLabels[] = {
    "off",              // 0
    "comfort",          // 1
    "eco",              // 2
    "frost_protection", // 3
    "comfort_-1",       // 4
    "comfort_-2",       // 5
};

}  // namespace

bool fz_pilot_wire_mode(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");  // attr 0x0000 mode (UINT8)
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
    // 0xFC00 → "manuSpecificPhilips" in cluster_id_to_name(); gate on it
    // so genOnOff/seMetering reports (which also carry attr "0") don't
    // mis-fire this decoder.
    .cluster           = "manuSpecificPhilips",
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

}  // namespace zhc::vav
