// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared sber converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/sber.ts

#include "definitions/sber/_shared.hpp"

#include <cstdint>
#include <cstdio>

namespace zhc::devices::sber {

namespace {

constexpr std::uint16_t kSberMfgCode = 0x121B;   // SBERDEVICES_LTD
constexpr std::uint16_t kSberClusterId = 0xFCCF; // manuSpecificSDevices

// ZCL data-type bytes (foundation 2.6.2.2).
constexpr std::uint8_t kZclBool   = 0x10;
constexpr std::uint8_t kZclUint8  = 0x20;
constexpr std::uint8_t kZclUint16 = 0x21;
constexpr std::uint8_t kZclUint32 = 0x23;
constexpr std::uint8_t kZclInt16  = 0x29;
constexpr std::uint8_t kZclInt32  = 0x2B;
constexpr std::uint8_t kZclEnum8  = 0x30;

struct SberAttrEntry {
    std::uint16_t attr_id;
    const char*   out_key;
    ValueType     out_type;   // Bool or Uint (signed values reinterpreted)
};

// Single static map driving `kFzSberManuSpecific`. Keep sorted by
// attr-id for readability — lookup is O(N), N small.
constexpr SberAttrEntry kSberAttrMap[] = {
    {0x1002, "multi_click",             ValueType::Bool},
    {0x1003, "child_lock",              ValueType::Bool},
    {0x2001, "led_on_enable",           ValueType::Bool},
    {0x2005, "led_off_enable",          ValueType::Bool},
    {0x2009, "led_indication_type",     ValueType::Uint},
    {0x3011, "upper_voltage_threshold", ValueType::Uint},
    {0x3012, "lower_voltage_threshold", ValueType::Uint},
    {0x3013, "upper_current_threshold", ValueType::Uint},
    {0x3014, "upper_temp_threshold",    ValueType::Uint},
    {0x4001, "voltage",                 ValueType::Uint},
    {0x4002, "current",                 ValueType::Uint},
    {0x4003, "power",                   ValueType::Uint},
};

}  // namespace

bool fz_sber_manu_specific(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    for (const auto& e : kSberAttrMap) {
        char keybuf[8];
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;

        Value o{};
        if (e.out_type == ValueType::Bool) {
            o.type = ValueType::Bool;
            if      (v->type == ValueType::Bool) o.b = v->b;
            else if (v->type == ValueType::Uint) o.b = (v->u != 0);
            else if (v->type == ValueType::Int)  o.b = (v->i != 0);
            else continue;
        } else {
            o.type = ValueType::Uint;
            if      (v->type == ValueType::Uint) o.u = v->u;
            else if (v->type == ValueType::Int)  o.u = static_cast<std::uint64_t>(v->i);
            else if (v->type == ValueType::Bool) o.u = v->b ? 1u : 0u;
            else continue;
        }
        out.put(e.out_key, o);
        emitted = true;
    }
    return emitted;
}

extern const FzConverter kFzSberManuSpecific{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificSDevices",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_sber_manu_specific },
    .user_config       = nullptr,
};

// ── TZ specs / converters ─────────────────────────────────────────

namespace {
constexpr ::zhc::generic::ZclWriteSpec kSpecChildLock{
    "child_lock",              0x1003, kZclBool,   kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMultiClick{
    "multi_click",             0x1002, kZclBool,   kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLedOnEnable{
    "led_on_enable",           0x2001, kZclBool,   kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLedOffEnable{
    "led_off_enable",          0x2005, kZclBool,   kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLedIndicationType{
    "led_indication_type",     0x2009, kZclEnum8,  kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecUpperVoltageThreshold{
    "upper_voltage_threshold", 0x3011, kZclUint32, kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLowerVoltageThreshold{
    "lower_voltage_threshold", 0x3012, kZclUint32, kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecUpperCurrentThreshold{
    "upper_current_threshold", 0x3013, kZclUint32, kSberMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecUpperTempThreshold{
    "upper_temp_threshold",    0x3014, kZclInt16,  kSberMfgCode, nullptr, 0,
};
}  // namespace

#define ZHC_SBER_TZ(var, spec_ref, key_str)                           \
    extern const TzConverter var{                                     \
        .key         = key_str,                                       \
        .cluster     = "manuSpecificSDevices",                        \
        .cluster_id  = kSberClusterId,                                \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &spec_ref,                                     \
    };
ZHC_SBER_TZ(kTzSberChildLock,              kSpecChildLock,              "child_lock")
ZHC_SBER_TZ(kTzSberMultiClick,             kSpecMultiClick,             "multi_click")
ZHC_SBER_TZ(kTzSberLedOnEnable,            kSpecLedOnEnable,            "led_on_enable")
ZHC_SBER_TZ(kTzSberLedOffEnable,           kSpecLedOffEnable,           "led_off_enable")
ZHC_SBER_TZ(kTzSberLedIndicationType,      kSpecLedIndicationType,      "led_indication_type")
ZHC_SBER_TZ(kTzSberUpperVoltageThreshold,  kSpecUpperVoltageThreshold,  "upper_voltage_threshold")
ZHC_SBER_TZ(kTzSberLowerVoltageThreshold,  kSpecLowerVoltageThreshold,  "lower_voltage_threshold")
ZHC_SBER_TZ(kTzSberUpperCurrentThreshold,  kSpecUpperCurrentThreshold,  "upper_current_threshold")
ZHC_SBER_TZ(kTzSberUpperTempThreshold,     kSpecUpperTempThreshold,     "upper_temp_threshold")
#undef ZHC_SBER_TZ

}  // namespace zhc::devices::sber
