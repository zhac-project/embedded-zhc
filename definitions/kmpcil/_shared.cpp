// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// KMPCIL shared converters. See `_shared.hpp` for the contract +
// rationale.
//
// z2m-source: zigbee-herdsman-converters/src/devices/kmpcil.ts and
//             src/converters/{from,to}Zigbee.ts
//             (kmpcil_res005_occupancy / kmpcil_res005_on_off).
#include "definitions/kmpcil/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::kmpcil {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::FzConverter;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::TzConverter;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::generic::ZclWriteSpec;
using ::zhc::type_bit;
using ::zhc::WILDCARD_ATTR_ID;
using ::zhc::WILDCARD_CMD_ID;
using ::zhc::WILDCARD_ENDPOINT;

namespace {

// Attribute 0x0055 (presentValue) is keyed as decimal "85" by the
// runtime's payload map.
constexpr const char* kAttrPresentValue = "85";

bool fz_kmpcil_res005_on_off(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find(kAttrPresentValue);
    if (!v) return false;
    bool state;
    if (v->type == ValueType::Bool)      state = v->b;
    else if (v->type == ValueType::Uint) state = v->u != 0;
    else return false;
    Value o{}; o.type = ValueType::Bool; o.b = state;
    out.put("state", o);
    return true;
}

bool fz_kmpcil_res005_occupancy(const DecodedMessage& msg,
                                 const FzConverter&,
                                 const PreparedDefinition&,
                                 RuntimeContext&,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // RES005: presentValue == 1 means occupied.
    const Value* v = msg.payload.find(kAttrPresentValue);
    if (!v) return false;
    bool occ;
    if (v->type == ValueType::Bool)      occ = v->b;
    else if (v->type == ValueType::Uint) occ = v->u == 1;
    else return false;
    Value o{}; o.type = ValueType::Bool; o.b = occ;
    out.put("occupancy", o);
    return true;
}

bool fz_kmpcil_presence_binary_input(const DecodedMessage& msg,
                                      const FzConverter&,
                                      const PreparedDefinition&,
                                      RuntimeContext&,
                                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Tag-001 packs three bits into presentValue:
    //   bit 0 → power_state   (1 = on DC power)
    //   bit 1 → vibration
    //   bit 2 → occupancy
    // Frames arrive on every state change; we publish presence=true on
    // every frame so downstream rules can stale-time it as they like.
    const Value* v = msg.payload.find(kAttrPresentValue);
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint64_t pv = v->u;
    Value pres{}; pres.type = ValueType::Bool; pres.b = true;
    out.put("presence", pres);
    Value ps{}; ps.type = ValueType::Bool; ps.b = (pv & 0x01u) != 0;
    out.put("power_state", ps);
    Value vb{}; vb.type = ValueType::Bool; vb.b = (pv & 0x02u) != 0;
    out.put("vibration", vb);
    Value oc{}; oc.type = ValueType::Bool; oc.b = (pv & 0x04u) != 0;
    out.put("occupancy", oc);
    return true;
}

bool fz_kmpcil_presence_power(const DecodedMessage& msg,
                               const FzConverter&,
                               const PreparedDefinition&,
                               RuntimeContext&,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genPowerCfg attr 0x20 batteryVoltage (u8, units of 100 mV) →
    // emit voltage in mV. KMPCIL also publishes presence=true here in
    // z2m; we mirror so battery-only frames keep the device "live".
    const Value* v = msg.payload.find("32");
    if (!v || v->type != ValueType::Uint) return false;
    Value volt{}; volt.type = ValueType::Uint; volt.u = v->u * 100;
    out.put("voltage", volt);
    Value pres{}; pres.type = ValueType::Bool; pres.b = true;
    out.put("presence", pres);
    return true;
}

constexpr ZclWriteSpec kSpecRES005OnOff{
    .key               = "state",
    .attr_id           = 0x0055,         // genBinaryOutput presentValue
    .attr_type         = 0x10,           // BOOLEAN
    .manufacturer_code = 0,
    .lookup            = nullptr,
    .lookup_count      = 0,
    .flags             = 0,
};

}  // namespace

extern const FzConverter kFzKmpcilRES005OnOff{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBinaryOutput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_kmpcil_res005_on_off },
    .user_config       = nullptr,
};

extern const FzConverter kFzKmpcilRES005Occupancy{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBinaryInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_kmpcil_res005_occupancy },
    .user_config       = nullptr,
};

extern const FzConverter kFzKmpcilPresenceBinaryInput{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBinaryInput",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_kmpcil_presence_binary_input },
    .user_config       = nullptr,
};

extern const FzConverter kFzKmpcilPresencePower{
    .family            = FrameFamily::Zcl,
    .cluster           = "genPowerCfg",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_kmpcil_presence_power },
    .user_config       = nullptr,
};

extern const TzConverter kTzKmpcilRES005OnOff{
    .key         = "state",
    .cluster     = "genBinaryOutput",
    .cluster_id  = 0x0010,
    .command_id  = 0x02,                 // writeAttributes
    .fn          = ::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecRES005OnOff,
};

}  // namespace zhc::devices::kmpcil
