// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared lixee converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lixee.ts
//             (lixee_metering / lixee_ha_electrical_measurement /
//              lixee_private_fz)

#include "definitions/lixee/_shared.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "zhc/types.hpp"

namespace zhc::lixee {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::FzConverter;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::type_bit;

bool fz_lixee_attrs(const DecodedMessage& msg,
                    const FzConverter& self,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = static_cast<const LixeeAttrMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) return false;

    bool emitted = false;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        const auto& e = map->entries[i];
        // The parser keys numeric attrs by decimal attr-id (matching
        // lumi/_shared's pattern). Format then look up.
        char keybuf[8];
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;

        Value o{};
        switch (e.type) {
            case LixeeFieldType::String: {
                if (v->type != ValueType::StringRef) continue;
                o.type = ValueType::StringRef;
                o.str  = v->str;
                break;
            }
            case LixeeFieldType::Raw: {
                std::int64_t raw = 0;
                if      (v->type == ValueType::Uint) raw = static_cast<std::int64_t>(v->u);
                else if (v->type == ValueType::Int)  raw = v->i;
                else continue;
                if (e.unit_div > 1) {
                    o.type = ValueType::Int;
                    o.i    = raw / static_cast<std::int64_t>(e.unit_div);
                } else {
                    o.type = ValueType::Int;
                    o.i    = raw;
                }
                break;
            }
            case LixeeFieldType::Bitmap: {
                if (v->type != ValueType::Uint) continue;
                o.type = ValueType::Uint;
                o.u    = v->u;
                break;
            }
        }
        if (!out.put(e.out_key, o)) break;  // payload full
        emitted = true;
    }
    return emitted;
}

namespace {

// ── liXeePrivate (0xFF66) ──────────────────────────────────────────
//
// Most-common 12 attrs. Output keys follow z2m's snake_case names,
// abbreviated to the 20-char shadow cap when needed (see
// docs/LIXEE_PARITY.md for the mapping).
constexpr LixeeAttrEntry kPrivateEntries[] = {
    // attr_id  out_key                  type                       div
    { 0x0000, "current_tarif",          LixeeFieldType::String, 0 },  // CHAR_STR
    { 0x0001, "tomorrow_color",         LixeeFieldType::String, 0 },  // CHAR_STR
    { 0x0002, "schedule_hphc",          LixeeFieldType::Raw,    0 },  // u8
    { 0x0003, "presence_potential",     LixeeFieldType::Raw,    0 },  // u8
    { 0x0004, "start_notice_ejp",       LixeeFieldType::Raw,    0 },  // u8
    { 0x0005, "warn_dps",               LixeeFieldType::Raw,    0 },  // u16
    { 0x0008, "linky_mode",             LixeeFieldType::Bitmap, 0 },  // u8 bits
    { 0x0009, "current_index_tarif",    LixeeFieldType::Raw,    0 },  // u8 (NTARF)
    { 0x0020, "active_register_tier",   LixeeFieldType::Raw,    0 },  // u8
    { 0x0039, "tariff_period",          LixeeFieldType::String, 0 },  // CHAR_STR (modern)
    { 0x0050, "current_date",           LixeeFieldType::String, 0 },  // CHAR_STR (DATE)
    { 0x0051, "relais",                 LixeeFieldType::Bitmap, 0 },  // u16 mask
    { 0x0052, "status_register",        LixeeFieldType::String, 0 },  // hex string
};

// ── haElectricalMeasurement (0x0B04) ───────────────────────────────
//
// Phase-A subset (single-phase Linky). Three-phase B/C variants live
// in the same cluster and follow the same id pattern (0x0905+, 0xA05+)
// — left out of v1 to keep the shared map small.
constexpr LixeeAttrEntry kElecEntries[] = {
    { 0x0305, "total_reactive_power",   LixeeFieldType::Raw, 0 },  // ERQ1
    { 0x0306, "total_apparent_power",   LixeeFieldType::Raw, 0 },
    { 0x0505, "rms_voltage",            LixeeFieldType::Raw, 0 },  // URMS1, V
    { 0x0508, "rms_current",            LixeeFieldType::Raw, 0 },  // IRMS1, A
    { 0x050A, "rms_current_max",        LixeeFieldType::Raw, 0 },
    { 0x050B, "active_power",           LixeeFieldType::Raw, 0 },  // CCASN, W
    { 0x050D, "active_power_max",       LixeeFieldType::Raw, 0 },
    { 0x050E, "reactive_power",         LixeeFieldType::Raw, 0 },
    { 0x050F, "apparent_power",         LixeeFieldType::Raw, 0 },  // SINSTS, VA
    { 0x0511, "avg_rms_voltage_period", LixeeFieldType::Raw, 0 },  // UMOY1
};

// ── seMetering (0x0702) ────────────────────────────────────────────
//
// Wh registers — z2m converts to kWh by dividing 1000. We emit the
// integer kWh value (raw Wh / 1000); the precision option (kWhP) is
// not honoured in v1.
constexpr LixeeAttrEntry kMeterEntries[] = {
    { 0x0000, "current_summ_delivered", LixeeFieldType::Raw, 1000 },  // EAST
    { 0x0001, "current_summ_received",  LixeeFieldType::Raw, 1000 },  // EAIT
    { 0x0100, "tier1_summ_delivered",   LixeeFieldType::Raw, 1000 },  // EASF01
    { 0x0102, "tier2_summ_delivered",   LixeeFieldType::Raw, 1000 },  // EASF02
    { 0x0104, "tier3_summ_delivered",   LixeeFieldType::Raw, 1000 },  // EASF03
    { 0x0106, "tier4_summ_delivered",   LixeeFieldType::Raw, 1000 },  // EASF04
    { 0x0307, "meter_serial_number",    LixeeFieldType::String, 0 },  // ADSC
    { 0x0308, "site_id",                LixeeFieldType::String, 0 },  // PRM
};

}  // namespace

extern const LixeeAttrMap kLixeePrivateMap{
    kPrivateEntries, sizeof(kPrivateEntries) / sizeof(kPrivateEntries[0]),
};
extern const LixeeAttrMap kLixeeElecMap{
    kElecEntries,    sizeof(kElecEntries)    / sizeof(kElecEntries[0]),
};
extern const LixeeAttrMap kLixeeMeterMap{
    kMeterEntries,   sizeof(kMeterEntries)   / sizeof(kMeterEntries[0]),
};

#define ZHC_LIXEE_FZ(var, cluster_str, map_ref)                           \
    extern const FzConverter var{                                         \
        .family            = FrameFamily::Zcl,                            \
        .cluster           = cluster_str,                                 \
        .type_mask         = type_bit(MessageType::AttributeReport) |     \
                              type_bit(MessageType::ReadResponse),        \
        .command_id        = ::zhc::WILDCARD_CMD_ID,                      \
        .attr_id           = ::zhc::WILDCARD_ATTR_ID,                     \
        .endpoint          = ::zhc::WILDCARD_ENDPOINT,                    \
        .frame_flags_mask  = 0,                                           \
        .frame_flags_value = 0,                                           \
        .direction         = Direction::ServerToClient,                   \
        .fn                = { .zcl_fn = &fz_lixee_attrs },                \
        .user_config       = &map_ref,                                    \
    };
ZHC_LIXEE_FZ(kFzLixeePrivate,  "liXeePrivate",            kLixeePrivateMap)
ZHC_LIXEE_FZ(kFzLixeeElec,     "haElectricalMeasurement", kLixeeElecMap)
ZHC_LIXEE_FZ(kFzLixeeMetering, "seMetering",              kLixeeMeterMap)
#undef ZHC_LIXEE_FZ

}  // namespace zhc::lixee
