// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Datek vendor converters. See `_shared.hpp` for cluster /
// attribute reference. All TZ writes use the generic
// `tz_zcl_write_attr` encoder with mfgcode=0x1337 (DATEK_WIRELESS_AS).
//
// z2m-source:
//   zigbee-herdsman-converters/src/converters/fromZigbee.ts
//     idlock, idlock_fw, led_on_motion
//   zigbee-herdsman-converters/src/converters/toZigbee.ts
//     idlock_master_pin_mode, idlock_rfid_enable, idlock_service_mode,
//     idlock_lock_mode, idlock_relock_enabled, led_on_motion

#include "_shared.hpp"

#include <cstdint>

namespace zhc::datek {

using ::zhc::FixedPayload;
using ::zhc::FzConverter;
using ::zhc::TzConverter;
using ::zhc::Direction;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::DecodedMessage;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::type_bit;

namespace {

// Decimal-string keys produced by the foundation parser for the
// Datek manu-specific closuresDoorLock attributes.
//   0x4000 = 16384  master_pin_mode
//   0x4001 = 16385  rfid_enable
//   0x4003 = 16387  service_mode
//   0x4004 = 16388  lock_mode
//   0x4005 = 16389  relock_enabled
constexpr const char* kKey_MasterPinMode = "16384";
constexpr const char* kKey_RfidEnable    = "16385";
constexpr const char* kKey_ServiceMode   = "16387";
constexpr const char* kKey_LockMode      = "16388";
constexpr const char* kKey_RelockEnabled = "16389";

// ssIasZone manuSpec: led_on_motion at 0x4000.
constexpr const char* kKey_LedOnMotion   = "16384";

std::uint64_t u_of(const Value& v) {
    if (v.type == ValueType::Uint) return v.u;
    if (v.type == ValueType::Int)  return static_cast<std::uint64_t>(v.i);
    if (v.type == ValueType::Bool) return v.b ? 1u : 0u;
    return 0;
}

void emit_bool(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
               const char* key, bool value) {
    Value v{};
    v.type = ValueType::Bool;
    v.b    = value;
    out.put(key, v);
}

void emit_str(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
              const char* key, const char* str) {
    Value v{};
    v.type = ValueType::StringRef;
    v.str  = str;
    out.put(key, v);
}

}  // namespace

// ── fz_idlock ──────────────────────────────────────────────────────
//
// Decode closuresDoorLock manuSpec attribute reports.
bool fz_idlock(const DecodedMessage& msg,
               const FzConverter&,
               const PreparedDefinition&,
               RuntimeContext&,
               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool any = false;

    if (const Value* v = msg.payload.find(kKey_MasterPinMode)) {
        emit_bool(out, "master_pin_mode", u_of(*v) == 1u);
        any = true;
    }
    if (const Value* v = msg.payload.find(kKey_RfidEnable)) {
        emit_bool(out, "rfid_enable", u_of(*v) == 1u);
        any = true;
    }
    if (const Value* v = msg.payload.find(kKey_ServiceMode)) {
        // 0=deactivated, 1/5=random_pin_1x_use, 6/9=random_pin_24_hours
        const std::uint64_t raw = u_of(*v);
        const char* label = nullptr;
        switch (raw) {
            case 0:                label = "deactivated";          break;
            case 1: case 5:        label = "random_pin_1x_use";    break;
            case 6: case 9:        label = "random_pin_24_hours";  break;
            default:               break;
        }
        if (label) {
            emit_str(out, "service_mode", label);
            any = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_LockMode)) {
        const std::uint64_t raw = u_of(*v);
        const char* label = nullptr;
        switch (raw) {
            case 0: label = "auto_off_away_off"; break;
            case 1: label = "auto_on_away_off";  break;
            case 2: label = "auto_off_away_on";  break;
            case 3: label = "auto_on_away_on";   break;
            default: break;
        }
        if (label) {
            emit_str(out, "lock_mode", label);
            any = true;
        }
    }
    if (const Value* v = msg.payload.find(kKey_RelockEnabled)) {
        emit_bool(out, "relock_enabled", u_of(*v) == 1u);
        any = true;
    }
    return any;
}

extern const FzConverter kFzIdlock{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresDoorLock",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_idlock },
    .user_config       = nullptr,
};

// ── fz_led_on_motion ───────────────────────────────────────────────
bool fz_led_on_motion(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (const Value* v = msg.payload.find(kKey_LedOnMotion)) {
        emit_bool(out, "led_on_motion", u_of(*v) == 1u);
        return true;
    }
    return false;
}

extern const FzConverter kFzLedOnMotion{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_led_on_motion },
    .user_config       = nullptr,
};

// ── tz writes (data-driven via kFzWrite/ZclWriteSpec) ──────────────

namespace {

// ID Lock manuSpec write specs.
constexpr ::zhc::generic::ZclWriteSpec kSpecMasterPinMode{
    "master_pin_mode", 0x4000, 0x10 /*BOOLEAN*/, kMfgDatek, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRfidEnable{
    "rfid_enable",     0x4001, 0x10, kMfgDatek, nullptr, 0,
};

// service_mode: lookup → UINT8. z2m only emits the canonical numeric
// values (1/5/6) on writes; readback may also see 9. We mirror z2m's
// convertSet table.
constexpr ::zhc::generic::ZclWriteLookup kServiceModeLookup[] = {
    { "deactivated",         0 },
    { "random_pin_1x_use",   5 },
    { "random_pin_24_hours", 6 },
};
constexpr ::zhc::generic::ZclWriteSpec kSpecServiceMode{
    "service_mode", 0x4003, 0x20 /*UINT8*/, kMfgDatek,
    kServiceModeLookup,
    static_cast<std::uint8_t>(sizeof(kServiceModeLookup) / sizeof(kServiceModeLookup[0])),
};

// lock_mode: lookup → UINT8.
constexpr ::zhc::generic::ZclWriteLookup kLockModeLookup[] = {
    { "auto_off_away_off", 0 },
    { "auto_on_away_off",  1 },
    { "auto_off_away_on",  2 },
    { "auto_on_away_on",   3 },
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLockMode{
    "lock_mode", 0x4004, 0x20, kMfgDatek,
    kLockModeLookup,
    static_cast<std::uint8_t>(sizeof(kLockModeLookup) / sizeof(kLockModeLookup[0])),
};

constexpr ::zhc::generic::ZclWriteSpec kSpecRelockEnabled{
    "relock_enabled", 0x4005, 0x10, kMfgDatek, nullptr, 0,
};

// Standard write: closuresDoorLock soundVolume (UINT8, mfg=0).
// z2m's lockSoundVolume table is index-based (silent_mode=0,
// low_volume=1, high_volume=2) — we expose both string labels and
// raw uint inputs through ZclWriteSpec's lookup.
constexpr ::zhc::generic::ZclWriteLookup kSoundVolumeLookup[] = {
    { "silent_mode", 0 },
    { "low_volume",  1 },
    { "high_volume", 2 },
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLockSoundVolume{
    "sound_volume", 0x0024, 0x20 /*UINT8*/, /*mfg=*/0,
    kSoundVolumeLookup,
    static_cast<std::uint8_t>(sizeof(kSoundVolumeLookup) / sizeof(kSoundVolumeLookup[0])),
};

// Motion sensor manuSpec write spec (ssIasZone 0x4000 BOOLEAN).
constexpr ::zhc::generic::ZclWriteSpec kSpecLedOnMotion{
    "led_on_motion", 0x4000, 0x10, kMfgDatek, nullptr, 0,
};

// Non-mfg-spec write: msOccupancySensing pirOToUDelay (UINT16).
constexpr ::zhc::generic::ZclWriteSpec kSpecOccupancyTimeout{
    "occupancy_timeout", 0x0010, 0x21 /*UINT16*/, /*mfg=*/0, nullptr, 0,
};

}  // namespace

#define ZHC_DATEK_TZ(var, spec_ref, key_str, cluster_str, cluster_hex)  \
    extern const TzConverter var{                                       \
        .key         = key_str,                                         \
        .cluster     = cluster_str,                                     \
        .cluster_id  = (cluster_hex),                                   \
        .command_id  = 0x02, /* writeAttributes */                      \
        .fn          = &::zhc::generic::tz_zcl_write_attr,              \
        .user_config = &spec_ref,                                       \
    };

ZHC_DATEK_TZ(kTzMasterPinMode, kSpecMasterPinMode, "master_pin_mode",
              "closuresDoorLock", 0x0101)
ZHC_DATEK_TZ(kTzRfidEnable,    kSpecRfidEnable,    "rfid_enable",
              "closuresDoorLock", 0x0101)
ZHC_DATEK_TZ(kTzServiceMode,   kSpecServiceMode,   "service_mode",
              "closuresDoorLock", 0x0101)
ZHC_DATEK_TZ(kTzLockMode,      kSpecLockMode,      "lock_mode",
              "closuresDoorLock", 0x0101)
ZHC_DATEK_TZ(kTzRelockEnabled, kSpecRelockEnabled, "relock_enabled",
              "closuresDoorLock", 0x0101)
ZHC_DATEK_TZ(kTzLockSoundVolume, kSpecLockSoundVolume, "sound_volume",
              "closuresDoorLock", 0x0101)
ZHC_DATEK_TZ(kTzLedOnMotion,   kSpecLedOnMotion,   "led_on_motion",
              "ssIasZone", 0x0500)
ZHC_DATEK_TZ(kTzOccupancyTimeout, kSpecOccupancyTimeout, "occupancy_timeout",
              "msOccupancySensing", 0x0406)

#undef ZHC_DATEK_TZ

// ── fz: electricity-meter extras (HSE2905E) ────────────────────────
//
// See _shared.hpp for the z2m bundle breakdown. These add the channels
// the generic metering / electrical-measurement converters drop.

namespace {

// Raw unsigned pass-through (u16/u48 wire types). Matches the generic
// kFzMetering/kFzElectricalMeasurement convention; the runtime applies
// the cluster multiplier/divisor downstream.
bool meter_put_uint(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key,
                    const Value* v) {
    if (!v || v->type != ValueType::Uint) return false;
    Value o{};
    o.type = ValueType::Uint;
    o.u    = v->u;
    return out.put(key, o);
}

// haElectricalMeasurement (0x0B04): phase B/C voltage/current — the
// threePhase channels z2m's `m.electricityMeter({cluster:"electrical",
// threePhase:true, power:false})` decodes but the generic
// kFzElectricalMeasurement (phase-A power/voltage/current only) drops.
// NB: z2m leaves acFrequency/powerFactor defaulted OFF for this bundle
// (electricityMeter defaults acFrequency:false, powerFactor:false and the
// HSE2905E electrical bundle does not override them), so those two are
// intentionally NOT decoded/exposed here.
bool fz_datek_elec_extras(const DecodedMessage& msg,
                          const FzConverter&,
                          const PreparedDefinition&,
                          RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // Per-phase B/C (threePhase electrical bundle, power:false → no
    // active-power phases):
    //   rmsVoltagePhB 0x0905 (2309) / PhC 0x0A05 (2565) -> voltage_phase_b/c
    //   rmsCurrentPhB 0x0908 (2312) / PhC 0x0A08 (2568) -> current_phase_b/c
    if (meter_put_uint(out, "voltage_phase_b", msg.payload.find("2309")))
        emitted = true;
    if (meter_put_uint(out, "voltage_phase_c", msg.payload.find("2565")))
        emitted = true;
    if (meter_put_uint(out, "current_phase_b", msg.payload.find("2312")))
        emitted = true;
    if (meter_put_uint(out, "current_phase_c", msg.payload.find("2568")))
        emitted = true;

    return emitted;
}

// seMetering (0x0702): produced_energy — the `producedEnergy:true`
// channel z2m's metering bundle decodes but generic kFzMetering drops.
bool fz_datek_metering_extras(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0001 (1) — CurrentSummReceived (u48) -> "produced_energy".
    return meter_put_uint(out, "produced_energy", msg.payload.find("1"));
}

}  // namespace

extern const FzConverter kFzElectricalMeasurementExtras{
    .family            = FrameFamily::Zcl,
    .cluster           = "haElectricalMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_datek_elec_extras },
    .user_config       = nullptr,
};

extern const FzConverter kFzMeteringExtras{
    .family            = FrameFamily::Zcl,
    .cluster           = "seMetering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_datek_metering_extras },
    .user_config       = nullptr,
};

}  // namespace zhc::datek
