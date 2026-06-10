// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// PLAID SYSTEMS shared converters. See `_shared.hpp` for the contract +
// rationale.
//
// z2m-source: zigbee-herdsman-converters/src/devices/plaid.ts and
//             src/converters/fromZigbee.ts (fz.plaid_battery) +
//             src/lib/utils.ts (toPercentage).
#include <algorithm>
#include <cstdint>

#include "definitions/plaid/_shared.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::plaid {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::FzConverter;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::type_bit;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::WILDCARD_ATTR_ID;
using ::zhc::WILDCARD_CMD_ID;
using ::zhc::WILDCARD_ENDPOINT;

namespace {

// z2m toPercentage(value, min, max): clamp to [min,max] then
//   round((value - min) / (max - min) * 100).
std::uint64_t plaid_battery_pct(std::uint32_t voltage_mv) {
    constexpr std::uint32_t kMin = 2500;
    constexpr std::uint32_t kMax = 3000;
    const std::uint32_t v = std::clamp(voltage_mv, kMin, kMax);
    // (v - 2500) / 500 * 100, rounded.
    const std::uint32_t pct = ((v - kMin) * 100u + (kMax - kMin) / 2u) /
                              (kMax - kMin);
    return static_cast<std::uint64_t>(pct);
}

bool fz_plaid_battery(const DecodedMessage& msg,
                       const FzConverter&,
                       const PreparedDefinition&,
                       RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genPowerCfg attr 0x0000 = mainsVoltage (u16, raw mV). The runtime
    // keys attribute 0x0000 as decimal "0". z2m fz.plaid_battery passes
    // this straight to `voltage` (no scaling) and derives `battery`.
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint32_t voltage_mv = static_cast<std::uint32_t>(v->u);
    Value volt{}; volt.type = ValueType::Uint; volt.u = voltage_mv;
    out.put("voltage", volt);
    Value bat{}; bat.type = ValueType::Uint;
    bat.u = plaid_battery_pct(voltage_mv);
    out.put("battery", bat);
    return true;
}

}  // namespace

extern const FzConverter kFzPlaidBattery{
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
    .fn                = { .zcl_fn = fz_plaid_battery },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::plaid
