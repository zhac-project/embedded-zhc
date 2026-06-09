// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared CTM Lyng (Datek Wireless AS) writeable manuSpec attribute
// specs. Mirrors the lumi-shared pattern: each attribute gets a
// `ZclWriteSpec` and a `TzConverter` keyed by the same name z2m uses
// in `tzLocal.ctm_*.convertSet`.
//
// z2m-source: zigbee-herdsman-converters/src/devices/ctm.ts.
#include "definitions/ctm/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

namespace zhc::ctm {

namespace {

// ── ZCL data-type bytes (per ZCL spec / zigbee-herdsman DataType) ───
constexpr std::uint8_t kZclBool   = 0x10;
constexpr std::uint8_t kZclUint8  = 0x20;
constexpr std::uint8_t kZclUint16 = 0x21;
constexpr std::uint8_t kZclEnum8  = 0x30;
constexpr std::uint8_t kZclCharStr= 0x42;

// ── genOnOff manuSpec writeable attrs (cluster 0x0006, mfg 0x1337) ──
constexpr ::zhc::generic::ZclWriteSpec kSpecDeviceEnabled{
    "device_enabled", 0x4002, kZclBool, kCtmMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRelayState{
    "state",          0x5001, kZclBool, kCtmMfgCode, nullptr, 0,
};

// ── hvacThermostat manuSpec writeable attrs (cluster 0x0201, mfg 0x1337) ─
constexpr ::zhc::generic::ZclWriteSpec kSpecCtmLoad{
    "load",           0x0401, kZclUint16, kCtmMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecCtmDisplayText{
    "display_text",   0x0402, kZclCharStr, kCtmMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecCtmFrostGuard{
    "frost_guard",    0x0412, kZclBool, kCtmMfgCode, nullptr, 0,
};
// child_lock: ts uses {1043: BOOLEAN}. 1043 == 0x0413. NOTE: z2m's
// ctm_thermostat_child_lock writes WITHOUT a manufacturer code — the
// attribute spec doesn't carry one in `addCtmToHvacThermostatCluster`
// either. Pass mfg=0 so we send fc=0x10.
constexpr ::zhc::generic::ZclWriteSpec kSpecCtmChildLockThermo{
    "child_lock",     0x0413, kZclBool, 0, nullptr, 0,
};
// preset → ctmOperationMode. ts writes attr 1058 == 0x0422 as UINT8
// without manufacturer code.
constexpr ::zhc::generic::ZclWriteLookup kPresetLookup[] = {
    {"off", 0}, {"away", 1}, {"sleep", 2}, {"home", 3},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecCtmPreset{
    "preset",         0x0422, kZclUint8, 0,
    kPresetLookup, sizeof(kPresetLookup)/sizeof(kPresetLookup[0]),
};

}  // namespace

#define ZHC_CTM_TZ(var, spec_ref, key_str, cluster_str, cluster_id_v) \
    extern const TzConverter var{                                     \
        .key         = key_str,                                       \
        .cluster     = cluster_str,                                   \
        .cluster_id  = cluster_id_v,                                  \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &spec_ref,                                     \
    };

// genOnOff (0x0006) manuSpec writes
ZHC_CTM_TZ(kTzCtmDeviceEnabled, kSpecDeviceEnabled, "device_enabled",
           "genOnOff", 0x0006)
ZHC_CTM_TZ(kTzCtmRelayState,    kSpecRelayState,    "state",
           "genOnOff", 0x0006)

// hvacThermostat (0x0201) manuSpec / std writes
ZHC_CTM_TZ(kTzCtmLoad,                kSpecCtmLoad,
           "load",         "hvacThermostat", 0x0201)
ZHC_CTM_TZ(kTzCtmDisplayText,         kSpecCtmDisplayText,
           "display_text", "hvacThermostat", 0x0201)
ZHC_CTM_TZ(kTzCtmFrostGuard,          kSpecCtmFrostGuard,
           "frost_guard",  "hvacThermostat", 0x0201)
ZHC_CTM_TZ(kTzCtmChildLockThermostat, kSpecCtmChildLockThermo,
           "child_lock",   "hvacThermostat", 0x0201)
ZHC_CTM_TZ(kTzCtmPreset,              kSpecCtmPreset,
           "preset",       "hvacThermostat", 0x0201)

#undef ZHC_CTM_TZ

// ── ssIasZone (0x0500) read-only zone-status decoders ───────────────
//
// CTM re-labels the standard IAS zoneStatus bits per device. The
// generic `kFzIasZone` only knows the bare `alarm` (bit 0) name, so
// these vendor decoders own the bit→key mapping. zoneStatus arrives
// two ways and z2m's `m.iasZoneAlarm` / `ctm_water_leak_alarm` handle
// both, so we do too:
//   * commandStatusChangeNotification (cmd 0x00): zoneStatus:u16 LE is
//     the first body field after the ZCL header.
//   * attributeReport / readResponse: zoneStatus is attr 0x0002, which
//     the parser surfaces under payload key "2".

namespace {

// Pull the u16 zoneStatus out of whichever wire shape arrived. Returns
// false when neither carries it.
bool ctm_read_zone_status(const DecodedMessage& msg, std::uint16_t& status) {
    if (msg.type == MessageType::Command) {
        // Body: zoneStatus:u16 LE, extendedStatus:u8, zoneId:u8, delay:u16.
        const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
        if (msg.raw_data.size() < hdr + 2) return false;
        status = static_cast<std::uint16_t>(msg.raw_data[hdr]) |
                 (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);
        return true;
    }
    // AttributeReport / ReadResponse — attr 0x0002 (ZoneStatus, u16).
    const Value* v = msg.payload.find("2");
    if (!v || v->type != ValueType::Uint) return false;
    status = static_cast<std::uint16_t>(v->u);
    return true;
}

bool fz_ctm_stove_guard_zone(const DecodedMessage& msg, const FzConverter&,
                              const PreparedDefinition&, RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t s = 0;
    if (!ctm_read_zone_status(msg, s)) return false;
    Value v{}; v.type = ValueType::Bool;
    v.b = (s & (1u << 0)) != 0; out.put("high_temperature", v);
    v.b = (s & (1u << 1)) != 0; out.put("power_cut_off",    v);
    v.b = (s & (1u << 2)) != 0; out.put("tamper",           v);
    v.b = (s & (1u << 3)) != 0; out.put("battery_low",      v);
    return true;
}

bool fz_ctm_water_leak(const DecodedMessage& msg, const FzConverter&,
                       const PreparedDefinition&, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint16_t s = 0;
    if (!ctm_read_zone_status(msg, s)) return false;
    Value v{}; v.type = ValueType::Bool;
    v.b = (s & (1u << 0)) != 0; out.put("active_water_leak", v);
    v.b = (s & (1u << 1)) != 0; out.put("water_leak",        v);
    v.b = (s & (1u << 3)) != 0; out.put("battery_low",       v);
    return true;
}

}  // namespace

#define ZHC_CTM_IAS_FZ(var, fn_ref)                                   \
    extern const FzConverter var{                                     \
        .family            = FrameFamily::Zcl,                        \
        .cluster           = "ssIasZone",                             \
        .type_mask         = type_bit(MessageType::Command) |         \
                             type_bit(MessageType::AttributeReport) | \
                             type_bit(MessageType::ReadResponse),     \
        .command_id        = WILDCARD_CMD_ID,                         \
        .attr_id           = WILDCARD_ATTR_ID,                        \
        .endpoint          = WILDCARD_ENDPOINT,                       \
        .frame_flags_mask  = 0,                                       \
        .frame_flags_value = 0,                                       \
        .direction         = Direction::ServerToClient,              \
        .fn                = { .zcl_fn = fn_ref },                    \
        .user_config       = nullptr,                                 \
    }

ZHC_CTM_IAS_FZ(kFzCtmStoveGuardZone, fz_ctm_stove_guard_zone);
ZHC_CTM_IAS_FZ(kFzCtmWaterLeak,      fz_ctm_water_leak);

#undef ZHC_CTM_IAS_FZ

}  // namespace zhc::ctm
