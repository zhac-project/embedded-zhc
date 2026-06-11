// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared BYUN gas/smoke-sensor vendor converters — see _shared.hpp for
// the full z2m mapping rationale.
//
// z2m-source: byun.ts fz.byun_{smoke,gas}_{true,false}.

#include "definitions/byun/_shared.hpp"

namespace zhc::devices::byun {

constexpr ByunAlarmConfig kCfgSmoke{ "smoke" };
constexpr ByunAlarmConfig kCfgGas{ "gas" };

namespace {

// ── ALARM (true): ssIasZone commandStatusChangeNotification ──────────
//
// z2m fires {smoke|gas}: true ONLY when the raw zoneStatus word == 33
// (0x21). This is a magic equality, not a bit test — so unlike the
// generic IAS decoder we do NOT emit tamper / battery_low and do NOT
// trigger on any other status value. zoneStatus is the first two body
// bytes after the ZCL header (fc/tsn/cmd = 3 bytes; +2 for a
// manufacturer-specific header, which these frames are not).
bool fz_byun_alarm_true(const DecodedMessage& msg, const FzConverter& self,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* cfg = reinterpret_cast<const ByunAlarmConfig*>(self.user_config);
    if (!cfg || !cfg->key) return false;

    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    const std::uint16_t status =
        static_cast<std::uint16_t>(msg.raw_data[hdr]) |
        (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);
    if (status != 33) return false;

    Value v{}; v.type = ValueType::Bool; v.b = true;
    out.put(cfg->key, v);
    return true;
}

// ── CLEAR (false), smoke: pHMeasurement measuredValue == 0 ───────────
//
// z2m byun_smoke_false: pHMeasurement (0x0409) attributeReport,
// measuredValue (attr 0x0000) === 0 → {smoke: false}.
bool fz_byun_smoke_false(const DecodedMessage& msg, const FzConverter&,
                         const PreparedDefinition&, RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 measuredValue
    if (!v) return false;
    std::int64_t raw;
    if (v->type == ValueType::Int)       raw = v->i;
    else if (v->type == ValueType::Uint) raw = static_cast<std::int64_t>(v->u);
    else return false;
    if (raw != 0) return false;

    Value o{}; o.type = ValueType::Bool; o.b = false;
    out.put("smoke", o);
    return true;
}

// ── CLEAR (false), gas: raw 0x040A frame, frame-control byte == 0x18 ──
//
// z2m byun_gas_false: cluster 1034 (0x040A), type:["raw"], fires when
// `msg.data[0] === 24`. herdsman's raw converter receives the whole-frame
// buffer, so data[0] is the ZCL frame-control octet (0x18). Mirror that
// by inspecting raw_data[0].
bool fz_byun_gas_false(const DecodedMessage& msg, const FzConverter&,
                       const PreparedDefinition&, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_data.empty() || msg.raw_data[0] != 0x18) return false;

    Value o{}; o.type = ValueType::Bool; o.b = false;
    out.put("gas", o);
    return true;
}

}  // namespace

#define ZHC_BYUN_ALARM_TRUE_CVT(name, cfg_ptr)                             \
    extern const FzConverter name{                                         \
        .family            = FrameFamily::Zcl,                            \
        .cluster           = "ssIasZone",                                 \
        .type_mask         = type_bit(MessageType::Command),              \
        .command_id        = 0x00,                                        \
        .attr_id           = WILDCARD_ATTR_ID,                            \
        .endpoint          = WILDCARD_ENDPOINT,                           \
        .frame_flags_mask  = 0,                                           \
        .frame_flags_value = 0,                                           \
        .direction         = Direction::ServerToClient,                  \
        .fn                = { .zcl_fn = fz_byun_alarm_true },            \
        .user_config       = cfg_ptr,                                     \
    }

ZHC_BYUN_ALARM_TRUE_CVT(kFzByunSmokeTrue, &kCfgSmoke);
ZHC_BYUN_ALARM_TRUE_CVT(kFzByunGasTrue,   &kCfgGas);

extern const FzConverter kFzByunSmokeFalse{
    .family            = FrameFamily::Zcl,
    .cluster           = "pHMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_byun_smoke_false },
    .user_config       = nullptr,
};

extern const FzConverter kFzByunGasFalse{
    .family            = FrameFamily::Zcl,
    .cluster           = "byunGasRaw",
    // The 0x040A frame carries fc=0x18 (global, no cluster-specific bit),
    // so the decoder classifies it by its foundation command id. Accept
    // the realistic classifications; the function guards on raw_data[0].
    .type_mask         = type_bit(MessageType::Raw) |
                         type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse) |
                         type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_byun_gas_false },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::byun
