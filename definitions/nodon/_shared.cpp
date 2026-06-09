// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// NodOn shared converters. See `_shared.hpp` for the contract.
//
// z2m-source: zigbee-herdsman-converters/src/devices/nodon.ts
//             `nodonModernExtend`.
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {

using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::WILDCARD_ATTR_ID;
using ::zhc::WILDCARD_CMD_ID;
using ::zhc::WILDCARD_ENDPOINT;
using ::zhc::type_bit;
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

// NodOn manufacturer code (Zcl.ManufacturerCode.NODON = 0x128B).
inline constexpr std::uint16_t kNodonManu = 0x128B;

// ── Lookup tables ───────────────────────────────────────────────────
constexpr ZclWriteLookup kLkSwitchTypeOnOff[] = {
    {"bistable",    0x00},
    {"monostable",  0x01},
    {"auto_detect", 0x02},
};

constexpr ZclWriteLookup kLkSwitchTypeWindowCovering[] = {
    {"bistable",    0x00},
    {"monostable",  0x01},
};

constexpr ZclWriteLookup kLkTrvMode[] = {
    {"auto",                0x00},
    {"valve_position_mode", 0x01},
    {"manual",              0x02},
};

// ── ZclWriteSpec instances (manu = 0x128B) ──────────────────────────
//
// Attribute IDs come straight from `nodonModernExtend` in z2m's
// `nodon.ts`. ZCL data-type codes:
//   0x20 u8, 0x21 u16, 0x28 i8, 0x30 enum8.

extern const ZclWriteSpec kSpecImpulseMode{
    "impulse_mode_configuration",
    /*attr_id*/ 0x0001,
    /*attr_type*/ 0x21,            // u16 (ms)
    /*manu*/ kNodonManu,
    /*lookup*/ nullptr, 0,
};

extern const ZclWriteSpec kSpecSwitchTypeOnOff{
    "switch_type",
    /*attr_id*/ 0x1001,
    /*attr_type*/ 0x30,            // enum8
    /*manu*/ kNodonManu,
    kLkSwitchTypeOnOff, sizeof(kLkSwitchTypeOnOff)/sizeof(kLkSwitchTypeOnOff[0]),
};

extern const ZclWriteSpec kSpecCalibVertUp{
    "calibration_vertical_run_time_up",
    /*attr_id*/ 0x0001, 0x21, kNodonManu, nullptr, 0,
};
extern const ZclWriteSpec kSpecCalibVertDown{
    "calibration_vertical_run_time_down",
    /*attr_id*/ 0x0002, 0x21, kNodonManu, nullptr, 0,
};
extern const ZclWriteSpec kSpecCalibRotUp{
    "calibration_rotation_run_time_up",
    /*attr_id*/ 0x0003, 0x21, kNodonManu, nullptr, 0,
};
extern const ZclWriteSpec kSpecCalibRotDown{
    "calibration_rotation_run_time_down",
    /*attr_id*/ 0x0004, 0x21, kNodonManu, nullptr, 0,
};
extern const ZclWriteSpec kSpecSwitchTypeWindowCovering{
    "switch_type",
    /*attr_id*/ 0x1001, 0x30, kNodonManu,
    kLkSwitchTypeWindowCovering,
    sizeof(kLkSwitchTypeWindowCovering)/sizeof(kLkSwitchTypeWindowCovering[0]),
};

extern const ZclWriteSpec kSpecTrvMode{
    "trv_mode",
    /*attr_id*/ 0x4000, 0x30, kNodonManu,
    kLkTrvMode, sizeof(kLkTrvMode)/sizeof(kLkTrvMode[0]),
};
extern const ZclWriteSpec kSpecValvePosition{
    "valve_position",
    /*attr_id*/ 0x4001, 0x20, kNodonManu, nullptr, 0,
};

// ── Tz macros ───────────────────────────────────────────────────────
//
// Each Tz is a thin shim around the generic `tz_zcl_write_attr` with
// the appropriate spec wired via `user_config`.

#define ZHC_NODON_TZ(var, spec_ref, key_str, cluster_str, cluster_hex)\
    extern const TzConverter var{                                    \
        .key         = key_str,                                      \
        .cluster     = cluster_str,                                  \
        .cluster_id  = cluster_hex,                                  \
        .command_id  = 0x02,        /* Write Attributes */            \
        .fn          = &::zhc::generic::tz_zcl_write_attr,           \
        .user_config = &spec_ref,                                    \
    };

ZHC_NODON_TZ(kTzNodonImpulseMode,             kSpecImpulseMode,
             "impulse_mode_configuration", "genOnOff", 0x0006)
ZHC_NODON_TZ(kTzNodonSwitchTypeOnOff,         kSpecSwitchTypeOnOff,
             "switch_type",                "genOnOff", 0x0006)
ZHC_NODON_TZ(kTzNodonCalibVertUp,             kSpecCalibVertUp,
             "calibration_vertical_run_time_up",
             "closuresWindowCovering", 0x0102)
ZHC_NODON_TZ(kTzNodonCalibVertDown,           kSpecCalibVertDown,
             "calibration_vertical_run_time_down",
             "closuresWindowCovering", 0x0102)
ZHC_NODON_TZ(kTzNodonCalibRotUp,              kSpecCalibRotUp,
             "calibration_rotation_run_time_up",
             "closuresWindowCovering", 0x0102)
ZHC_NODON_TZ(kTzNodonCalibRotDown,            kSpecCalibRotDown,
             "calibration_rotation_run_time_down",
             "closuresWindowCovering", 0x0102)
ZHC_NODON_TZ(kTzNodonSwitchTypeWindowCovering, kSpecSwitchTypeWindowCovering,
             "switch_type",
             "closuresWindowCovering", 0x0102)
ZHC_NODON_TZ(kTzNodonTrvMode,                  kSpecTrvMode,
             "trv_mode",      "hvacThermostat", 0x0201)
ZHC_NODON_TZ(kTzNodonValvePosition,            kSpecValvePosition,
             "valve_position","hvacThermostat", 0x0201)

#undef ZHC_NODON_TZ

// ── Fz: dry_contact (genBinaryInput attr 0x0055 enum8) ──────────────
//
// SDC-4-1-00. z2m's `nodonModernExtend.dryContact()` is an enumLookup
// over genBinaryInput attr 0x055; the runtime's payload map keys
// attribute IDs as decimal strings, so 0x55 → "85".
namespace {

bool fz_dry_contact(const DecodedMessage& msg,
                     const FzConverter&,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("85");   // 0x0055
    if (!v || v->type != ValueType::Uint) return false;
    // Static-storage labels — Value::str borrows, no copy.
    static constexpr const char* kClosed = "contact_closed";
    static constexpr const char* kOpen   = "contact_open";
    Value o{};
    o.type = ValueType::StringRef;
    o.str  = (v->u == 0) ? kClosed : kOpen;
    out.put("dry_contact", o);
    return true;
}

}  // namespace

extern const FzConverter kFzNodonDryContact{
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
    .fn                = { .zcl_fn = fz_dry_contact },
    .user_config       = nullptr,
};

// ── Fz: trv_mode + valve_position (hvacThermostat manuSpec attrs) ────
//
// TRV-4-1-00. z2m wires `nodonModernExtend.trvMode()` (enumLookup over
// hvacThermostat attr 0x4000) and `valvePosition()` (numeric over attr
// 0x4001); both auto-generate a fromZigbee path. The generic
// `kFzThermostat` only decodes 0x0000 / 0x0012 / 0x001C, so the
// `trv_mode` / `valve_position` exposes had matching Tz writers but no
// decode — STATE_GET / reporting never surfaced. This converter fills
// the read path on the same hvacThermostat cluster (0x0201), keyed by
// the runtime's decimal attribute strings ("16384" = 0x4000, "16385" =
// 0x4001).
//
// z2m-source: nodon.ts `nodonModernExtend.trvMode` / `.valvePosition`.
namespace {

// trv_mode enum (attr 0x4000): mirrors the z2m enumLookup mapping.
constexpr const char* kTrvModeLabels[] = {
    "auto",                 // 0x00
    "valve_position_mode",  // 0x01
    "manual",               // 0x02
};

bool fz_trv_extras(const DecodedMessage& msg,
                   const FzConverter&,
                   const PreparedDefinition&,
                   RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x4000 — trv_mode (enum8). getFromLookup miss is dropped.
    if (const Value* v = msg.payload.find("16384")) {
        std::uint64_t raw = 0;
        bool ok = false;
        if (v->type == ValueType::Uint)      { raw = v->u; ok = true; }
        else if (v->type == ValueType::Int && v->i >= 0) {
            raw = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok && raw < sizeof(kTrvModeLabels) / sizeof(kTrvModeLabels[0])) {
            Value o{};
            o.type = ValueType::StringRef;
            o.str  = kTrvModeLabels[raw];  // static storage — borrowed
            if (out.put("trv_mode", o)) emitted = true;
        }
    }

    // attr 0x4001 — valve_position (u8, 0..100 %).
    if (const Value* v = msg.payload.find("16385")) {
        std::uint64_t pct = 0;
        bool ok = false;
        if (v->type == ValueType::Uint)      { pct = v->u; ok = true; }
        else if (v->type == ValueType::Int && v->i >= 0) {
            pct = static_cast<std::uint64_t>(v->i); ok = true;
        }
        if (ok && pct <= 100) {
            Value o{};
            o.type = ValueType::Uint;
            o.u    = pct;
            if (out.put("valve_position", o)) emitted = true;
        }
    }

    return emitted;
}

}  // namespace

extern const FzConverter kFzNodonTrvExtras{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_trv_extras },
    .user_config       = nullptr,
};

// ── Fz: cover position + tilt (closuresWindowCovering) ──────────────
//
// SIN-4-RS-20 / SIN-4-RS-20_PRO. z2m's
// `m.windowCovering({controls:['tilt','lift']})` exposes both
// `position` and `tilt` and decodes them via `fz.cover_position_tilt`
// from attrs 0x0008 (CurrentPositionLiftPercentage → position) and
// 0x0009 (CurrentPositionTiltPercentage → tilt). The generic
// `kFzCoverPosition` only handles the lift half, leaving `tilt` as a
// dead expose. This converter decodes both halves; mirrors
// `kFzUbisysCoverPositionTilt`. Values > 100 (e.g. 0xFF "unknown") are
// skipped, matching z2m.
//
// z2m-source: fromZigbee.ts `cover_position_tilt`.
namespace {

bool fz_cover_position_tilt(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    auto emit_pct = [&](const char* attr_key, const char* out_key) {
        if (const Value* v = msg.payload.find(attr_key)) {
            std::uint64_t pct = 0;
            bool ok = false;
            if (v->type == ValueType::Uint)      { pct = v->u; ok = true; }
            else if (v->type == ValueType::Int && v->i >= 0) {
                pct = static_cast<std::uint64_t>(v->i); ok = true;
            }
            if (ok && pct <= 100) {
                Value o{};
                o.type = ValueType::Uint;
                o.u    = pct;
                if (out.put(out_key, o)) emitted = true;
            }
        }
    };

    emit_pct("8", "position");  // 0x0008 CurrentPositionLiftPercentage
    emit_pct("9", "tilt");      // 0x0009 CurrentPositionTiltPercentage
    return emitted;
}

}  // namespace

extern const FzConverter kFzNodonCoverPositionTilt{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_cover_position_tilt },
    .user_config       = nullptr,
};

}  // namespace zhc::devices::nodon
