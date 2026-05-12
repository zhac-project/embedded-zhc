// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared heiman converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/heiman.ts
//             zigbee-herdsman-converters/src/lib/heiman.ts

#include "definitions/heiman/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace zhc::heiman {

// ── fz_heiman_manu_specific ────────────────────────────────────────
//
// Shared decoder for both heimanClusterSpecial (0xFC90) and the
// air-quality cluster (0xFC81). Walks the per-device map and emits
// matching keys into `out`.
//
// Bitmap16 attrs (sensorFaultState, deviceMuteState) are translated
// into a semicolon-joined list of active-bit labels — matching the
// joinedFaults / joinedMutes string z2m publishes.

namespace {

bool emit_bitmap_text(const HeimanSpecificEntry& e,
                      std::uint64_t bits,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!e.bitmap_table || e.bitmap_count == 0) return false;
    if (bits == 0) {
        Value o{}; o.type = ValueType::StringRef; o.str = "normal";
        out.put(e.out_key, o);
        return true;
    }
    // Static scratch is OK here — the ZHC dispatch loop is
    // single-threaded and consumes the StringRef before the next
    // bitmap report arrives. Lumi's enum_table uses string literals
    // for the same reason; we synthesise the joined label so we own
    // the storage instead.
    static char scratch[96];
    std::size_t off = 0;
    for (std::uint8_t i = 0; i < e.bitmap_count; ++i) {
        const auto& b = e.bitmap_table[i];
        if (b.bit > 15) continue;
        if ((bits & (1ull << b.bit)) == 0) continue;
        const std::size_t need = std::strlen(b.label) + (off ? 1 : 0);
        if (off + need + 1 >= sizeof(scratch)) break;
        if (off) scratch[off++] = ';';
        std::memcpy(scratch + off, b.label, std::strlen(b.label));
        off += std::strlen(b.label);
    }
    scratch[off] = '\0';
    Value o{}; o.type = ValueType::StringRef; o.str = scratch;
    out.put(e.out_key, o);
    return true;
}

}  // namespace

bool fz_heiman_manu_specific(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = static_cast<const HeimanSpecificMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) return false;

    bool emitted = false;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        const auto& e = map->entries[i];
        char keybuf[8];
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;

        switch (e.type) {
            case HeimanSpecificType::Raw: {
                Value o = *v;
                out.put(e.out_key, o);
                emitted = true;
                break;
            }
            case HeimanSpecificType::Bool: {
                bool b = false;
                if      (v->type == ValueType::Bool) b = v->b;
                else if (v->type == ValueType::Uint) b = v->u != 0;
                else if (v->type == ValueType::Int)  b = v->i != 0;
                else continue;
                Value o{}; o.type = ValueType::Bool; o.b = b;
                out.put(e.out_key, o);
                emitted = true;
                break;
            }
            case HeimanSpecificType::BitmapText: {
                std::uint64_t bits = 0;
                if      (v->type == ValueType::Uint) bits = v->u;
                else if (v->type == ValueType::Int)  bits = static_cast<std::uint64_t>(v->i);
                else continue;
                if (emit_bitmap_text(e, bits, out)) emitted = true;
                break;
            }
        }
    }
    return emitted;
}

// ── Smoke-detector heimanClusterSpecial map ────────────────────────
//
// Covers the attribute set the z2m extends `heimanClusterSensor*` /
// `heimanClusterDeviceMuteState` / `heimanClusterIndicatorLight`
// publish. Smoke-level / chamber-contamination raw values plus
// fault/mute bitmap text plus interconnectable bool plus heartbeat
// indicator bool.

namespace {

constexpr HeimanBitmapEntry kFaultStateBits[] = {
    {0, "fault"},
    {1, "open_circuit_fault"},
    {2, "short_circuit_fault"},
    {3, "pollution_fault"},
};

constexpr HeimanBitmapEntry kMuteStateBits[] = {
    {0, "muted"},
    {1, "alarm_muted"},
};

constexpr HeimanSpecificEntry kSmokeEntries[] = {
    { 0x0002, "fault_state",         HeimanSpecificType::BitmapText,
      kFaultStateBits, sizeof(kFaultStateBits)/sizeof(kFaultStateBits[0]) },
    { 0x0008, "temporary_mute",      HeimanSpecificType::Bool,       nullptr, 0 },
    { 0x0009, "muted",               HeimanSpecificType::BitmapText,
      kMuteStateBits, sizeof(kMuteStateBits)/sizeof(kMuteStateBits[0]) },
    { 0x0016, "smoke_concentration", HeimanSpecificType::Raw,        nullptr, 0 },
    { 0x0017, "chamber_pollution",   HeimanSpecificType::Raw,        nullptr, 0 },
    { 0x1004, "heartbeat_indicator", HeimanSpecificType::Bool,       nullptr, 0 },
    { 0x1007, "interconnectable",    HeimanSpecificType::Bool,       nullptr, 0 },
};

constexpr HeimanSpecificEntry kAirQualityEntries[] = {
    { 0xF002, "battery_state",  HeimanSpecificType::Raw, nullptr, 0 },
    { 0xF003, "pm10",           HeimanSpecificType::Raw, nullptr, 0 },
    { 0xF004, "voc",            HeimanSpecificType::Raw, nullptr, 0 },
    { 0xF005, "aqi",            HeimanSpecificType::Raw, nullptr, 0 },
};

}  // namespace

extern const HeimanSpecificMap kHeimanMapSmokeDetector{
    kSmokeEntries, sizeof(kSmokeEntries)/sizeof(kSmokeEntries[0]),
};

extern const HeimanSpecificMap kHeimanMapAirQuality{
    kAirQualityEntries, sizeof(kAirQualityEntries)/sizeof(kAirQualityEntries[0]),
};

extern const FzConverter kFzHeimanSmokeSpecial{
    .family            = FrameFamily::Zcl,
    .cluster           = "heimanClusterSpecial",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_heiman_manu_specific },
    .user_config       = &kHeimanMapSmokeDetector,
};

extern const FzConverter kFzHeimanAirQuality{
    .family            = FrameFamily::Zcl,
    .cluster           = "heimanSpecificAirQuality",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_heiman_manu_specific },
    .user_config       = &kHeimanMapAirQuality,
};

// ── Tz writeAttributes specs ──────────────────────────────────────

namespace {

// heimanClusterSpecial 0xFC90, mfg 0x120B, fc=0x14 path. Read-only
// converters still need a spec so the device can issue a get; the
// generic encoder handles the wire format.
constexpr ZclWriteSpec kSpecHeimanFaultState{
    "fault_state", 0x0002, 0x19 /* BITMAP16 */, kHeimanMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecHeimanMuted{
    "muted", 0x0009, 0x19 /* BITMAP16 */, kHeimanMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecHeimanIndicatorLight{
    "heartbeat_indicator", 0x1004, 0x20 /* UINT8 */, kHeimanMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecHeimanInterconnectable{
    "interconnectable", 0x1007, 0x20 /* UINT8 */, kHeimanMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecHeimanTemporaryMute{
    "temporary_mute", 0x0008, 0x20 /* UINT8 */, kHeimanMfgCode, nullptr, 0,
};
// heimanClusterRadar (0xFC8B) enableIndicator — same mfg code.
constexpr ZclWriteSpec kSpecHeimanRadarEnableIndicator{
    "enable_indicator", 0xF001, 0x20 /* UINT8 */, kHeimanMfgCode, nullptr, 0,
};

}  // namespace

#define ZHC_HEIMAN_TZ(var, spec_ref, cluster_str, cluster_id_v, key_str)  \
    extern const TzConverter var{                                         \
        .key         = key_str,                                           \
        .cluster     = cluster_str,                                       \
        .cluster_id  = cluster_id_v,                                      \
        .command_id  = 0x02,                                              \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                \
        .user_config = &spec_ref,                                         \
    };

ZHC_HEIMAN_TZ(kTzHeimanFaultStateRead,
              kSpecHeimanFaultState,
              "heimanClusterSpecial", 0xFC90, "fault_state")
ZHC_HEIMAN_TZ(kTzHeimanMutedRead,
              kSpecHeimanMuted,
              "heimanClusterSpecial", 0xFC90, "muted")
ZHC_HEIMAN_TZ(kTzHeimanIndicatorLight,
              kSpecHeimanIndicatorLight,
              "heimanClusterSpecial", 0xFC90, "heartbeat_indicator")
ZHC_HEIMAN_TZ(kTzHeimanInterconnectableRead,
              kSpecHeimanInterconnectable,
              "heimanClusterSpecial", 0xFC90, "interconnectable")
ZHC_HEIMAN_TZ(kTzHeimanTemporaryMute,
              kSpecHeimanTemporaryMute,
              "heimanClusterSpecial", 0xFC90, "temporary_mute")
ZHC_HEIMAN_TZ(kTzHeimanRadarEnableIndicator,
              kSpecHeimanRadarEnableIndicator,
              "heimanClusterRadar", 0xFC8B, "enable_indicator")

#undef ZHC_HEIMAN_TZ

// ── tz_heiman_initiate_test_mode ─────────────────────────────────
//
// ssIasZone (0x0500) cluster-specific command 0x02 with 2-byte
// payload: testModeDuration u8, sensitivityLevel u8. Frame layout
// matches `tz_warning` / standard cluster-specific writes:
//   fc=0x11 (cluster-specific + DDR), tsn=0x00, cmd=0x02, payload.
// No mfg-code path — z2m calls `entity.command("ssIasZone", ...)`
// without a `manufacturerCode` override.

bool tz_heiman_initiate_test_mode(std::string_view key,
                                   const Value& /*input*/,
                                   const TzConverter& /*self*/,
                                   const PreparedDefinition&,
                                   RuntimeContext&,
                                   std::span<std::uint8_t> out_frame,
                                   std::size_t& out_size) {
    out_size = 0;
    if (key != "trigger_selftest") return false;
    constexpr std::size_t total = 1 + 1 + 1 + 1 + 1;  // fc tsn cmd dur sens
    if (out_frame.size() < total) return false;
    std::size_t p = 0;
    out_frame[p++] = 0x11;   // fc: cluster-specific + DDR + c→s
    out_frame[p++] = 0x00;   // tsn placeholder
    out_frame[p++] = 0x02;   // cmd: initiateTestMode
    out_frame[p++] = 0x01;   // testModeDuration (z2m default)
    out_frame[p++] = 0x01;   // sensitivityLevel (z2m default)
    out_size = total;
    return true;
}

extern const TzConverter kTzHeimanInitiateTestMode{
    .key         = "trigger_selftest",
    .cluster     = "ssIasZone",
    .cluster_id  = 0x0500,
    .command_id  = 0x02,
    .fn          = &tz_heiman_initiate_test_mode,
    .user_config = nullptr,
};

}  // namespace zhc::heiman
