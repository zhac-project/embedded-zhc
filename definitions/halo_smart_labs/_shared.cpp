// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Halo Smart Labs shared converters — see _shared.hpp.
//
// z2m-source: zigbee-herdsman-converters/src/devices/halo_smart_labs.ts
//             (`haloZoneStatus` ModernExtend).

#include "definitions/halo_smart_labs/_shared.hpp"

#include "zhc/types.hpp"

#include <cstdint>

namespace zhc::devices::halo_smart_labs {

namespace {

bool fz_halo_ias_zone(const ::zhc::DecodedMessage& msg,
                      const ::zhc::FzConverter&,
                      const ::zhc::PreparedDefinition&,
                      ::zhc::RuntimeContext&,
                      ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0002 — ZoneStatus (u16). We accept only attribute reports
    // / read responses on this path; the generic IAS zone-status-change
    // command is intentionally not handled here because Halo also sends
    // it but with the same demuxing (z2m treats both alike via
    // commandStatusChangeNotification + attributeReport).
    const ::zhc::Value* v = msg.payload.find("2");
    if (!v || v->type != ::zhc::ValueType::Uint) return false;
    const std::uint32_t s = static_cast<std::uint32_t>(v->u);

    if (msg.src_endpoint == 1) {
        ::zhc::Value smoke{};   smoke.type   = ::zhc::ValueType::Bool;
        smoke.b   = (s & (1u << 0)) != 0;
        ::zhc::Value tamper{};  tamper.type  = ::zhc::ValueType::Bool;
        tamper.b  = (s & (1u << 2)) != 0;
        ::zhc::Value batlow{};  batlow.type  = ::zhc::ValueType::Bool;
        batlow.b  = (s & (1u << 3)) != 0;
        ::zhc::Value test{};    test.type    = ::zhc::ValueType::Bool;
        test.b    = (s & (1u << 8)) != 0;
        ::zhc::Value mains{};   mains.type   = ::zhc::ValueType::Bool;
        // mains_power_connected = !ac_fault, ac_fault is bit 7.
        mains.b   = (s & (1u << 7)) == 0;
        out.put("smoke",                  smoke);
        out.put("tamper",                 tamper);
        out.put("battery_low",            batlow);
        out.put("test",                   test);
        out.put("mains_power_connected",  mains);
        return true;
    }
    if (msg.src_endpoint == 3) {
        ::zhc::Value co{}; co.type = ::zhc::ValueType::Bool;
        co.b = (s & (1u << 0)) != 0;
        out.put("carbon_monoxide", co);
        return true;
    }
    return false;
}

}  // namespace

extern const ::zhc::FzConverter kFzHaloIasZone{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse) |
                         ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_halo_ias_zone },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::halo_smart_labs
