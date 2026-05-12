// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Danfoss manuSpec hvacThermostat / hvacUserInterfaceCfg runtime.
// Mirrors `danfossExtend.*` from z2m `src/devices/danfoss.ts` (the
// extend definition is inlined in that file — there is no
// `lib/danfoss.ts`). Manufacturer code: `0x1246`
// (`Zcl.ManufacturerCode.DANFOSS_A_S`).
//
// Pattern is cloned from `definitions/lumi/_shared.cpp`:
//   * `tz_zcl_write_attr` (in `_generic/_shared.cpp`) already encodes
//     fc=0x14 + mfg-code header — we just hand it a `ZclWriteSpec`.
//   * `fz_danfoss_thermostat` walks the decoded payload and emits
//     human-readable keys for the Danfoss attribute IDs that the
//     generic `kFzThermostat` ignores (it only handles 0x0000 /
//     0x0008 / 0x0012 / 0x001C).
#include "definitions/danfoss/_shared.hpp"

#include <cstdio>
#include <cstring>

namespace zhc::devices::danfoss {

using ::zhc::FzConverter;
using ::zhc::TzConverter;
using ::zhc::FrameFamily;
using ::zhc::MessageType;
using ::zhc::Direction;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::FixedPayload;
using ::zhc::DecodedMessage;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::type_bit;

namespace {

constexpr std::uint16_t kDanfossMfg = 0x1246;

// ── manuSpec hvacThermostat attribute table ──────────────────────
// Maps attr_id (decimal-string lookup, matches `payload.find()`) to
// expose key + value coercion type.
enum class DType : std::uint8_t { Bool, Uint, Int };

struct DanfossAttrEntry {
    std::uint16_t attr_id;
    const char*   key;
    DType         type;
};

// hvacThermostat (cluster 0x0201) Danfoss-specific attrs.
// Listed in matching order for `fz_danfoss_thermostat`.
constexpr DanfossAttrEntry kHvacThermostatAttrs[] = {
    { 0x4000, "window_open_internal",         DType::Uint },  // ENUM8
    { 0x4003, "window_open_external",         DType::Bool },  // BOOLEAN
    { 0x4012, "mounted_mode_active",          DType::Bool },  // BOOLEAN ro
    { 0x4013, "mounted_mode_control",         DType::Bool },  // BOOLEAN
    { 0x4014, "thermostat_orientation",       DType::Bool },  // BOOLEAN
    { 0x4015, "external_measured_room_sensor",DType::Int  },  // INT16
    { 0x4016, "radiator_covered",             DType::Bool },  // BOOLEAN
    { 0x4020, "algorithm_scale_factor",       DType::Uint },  // UINT8
    { 0x4030, "heat_available",               DType::Bool },  // BOOLEAN
    { 0x4031, "heat_required",                DType::Bool },  // BOOLEAN ro
    { 0x4032, "load_balancing_enable",        DType::Bool },  // BOOLEAN
    { 0x4040, "load_room_mean",               DType::Int  },  // INT16
    { 0x404c, "load_estimate",                DType::Int  },  // INT16 ro
    { 0x404d, "adaption_run_status",          DType::Uint },  // BITMAP8 ro
    { 0x404e, "adaption_run_settings",        DType::Uint },  // BITMAP8
    { 0x404f, "preheat_status",               DType::Bool },  // BOOLEAN ro
    { 0x4051, "window_open_feature",          DType::Bool },  // BOOLEAN
    // Icon-side helper attrs (also under hvacThermostat):
    { 0x4100, "room_status_code",             DType::Uint },  // BITMAP16 ro
    { 0x4110, "output_status",                DType::Uint },  // ENUM8 ro
    { 0x4120, "room_floor_sensor_mode",       DType::Uint },  // ENUM8 ro
};

bool fz_danfoss_thermostat(const DecodedMessage& msg,
                           const FzConverter&,
                           const PreparedDefinition&,
                           RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    char keybuf[8];
    for (const auto& e : kHvacThermostatAttrs) {
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;

        Value o{};
        switch (e.type) {
            case DType::Bool: {
                bool b = false;
                if      (v->type == ValueType::Bool) b = v->b;
                else if (v->type == ValueType::Uint) b = v->u != 0;
                else if (v->type == ValueType::Int)  b = v->i != 0;
                else continue;
                o.type = ValueType::Bool; o.b = b;
                break;
            }
            case DType::Uint: {
                if (v->type == ValueType::Uint) {
                    o.type = ValueType::Uint; o.u = v->u;
                } else if (v->type == ValueType::Int && v->i >= 0) {
                    o.type = ValueType::Uint;
                    o.u   = static_cast<std::uint64_t>(v->i);
                } else { continue; }
                break;
            }
            case DType::Int: {
                if (v->type == ValueType::Int) {
                    o.type = ValueType::Int; o.i = v->i;
                } else if (v->type == ValueType::Uint) {
                    o.type = ValueType::Int;
                    o.i   = static_cast<std::int64_t>(v->u);
                } else { continue; }
                break;
            }
        }
        if (out.put(e.key, o)) emitted = true;
    }
    return emitted;
}

// ── manuSpec hvacUserInterfaceCfg attribute table ────────────────
constexpr DanfossAttrEntry kHvacUserInterfaceCfgAttrs[] = {
    { 0x4000, "viewing_direction", DType::Uint },  // ENUM8
};

bool fz_danfoss_user_interface_cfg(const DecodedMessage& msg,
                                   const FzConverter&,
                                   const PreparedDefinition&,
                                   RuntimeContext&,
                                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    char keybuf[8];
    for (const auto& e : kHvacUserInterfaceCfgAttrs) {
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;
        Value o{};
        if (e.type == DType::Bool) {
            bool b = (v->type == ValueType::Uint) ? (v->u != 0)
                   : (v->type == ValueType::Int)  ? (v->i != 0)
                   : (v->type == ValueType::Bool) ? v->b : false;
            o.type = ValueType::Bool; o.b = b;
        } else {
            // Uint / Int — pass through.
            o = *v;
        }
        if (out.put(e.key, o)) emitted = true;
    }
    return emitted;
}

// ── ZclWriteSpec table (one per writable manu-spec attr) ─────────
// ZCL data-type bytes:
//   0x10 BOOLEAN, 0x20 UINT8, 0x21 UINT16, 0x28 INT8, 0x29 INT16,
//   0x30 ENUM8.
constexpr ::zhc::generic::ZclWriteSpec kSpecWindowOpenInternal{
    "window_open_internal", 0x4000, 0x30, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecWindowOpenExternal{
    "window_open_external", 0x4003, 0x10, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMountedModeControl{
    "mounted_mode_control", 0x4013, 0x10, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecThermostatOrientation{
    "thermostat_orientation", 0x4014, 0x10, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecExternalRoomSensor{
    "external_measured_room_sensor", 0x4015, 0x29, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRadiatorCovered{
    "radiator_covered", 0x4016, 0x10, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecAlgorithmScaleFactor{
    "algorithm_scale_factor", 0x4020, 0x20, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLoadBalancingEnable{
    "load_balancing_enable", 0x4032, 0x10, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLoadRoomMean{
    "load_room_mean", 0x4040, 0x29, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecRegulationSetpointOffset{
    "regulation_setpoint_offset", 0x404c, 0x28, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecAdaptionRunControl{
    "adaption_run_control", 0x404e, 0x18, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecWindowOpenFeatureEnable{
    "window_open_feature", 0x4051, 0x10, kDanfossMfg, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecViewingDirection{
    "viewing_direction", 0x4000, 0x30, kDanfossMfg, nullptr, 0,
};

}  // namespace

// ── FzConverter — manuSpec hvacThermostat ─────────────────────────
extern const FzConverter kFzDanfossThermostat{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_danfoss_thermostat },
    .user_config       = nullptr,
};

extern const FzConverter kFzDanfossUserInterfaceCfg{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacUserInterfaceCfg",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_danfoss_user_interface_cfg },
    .user_config       = nullptr,
};

// ── TzConverters — manuSpec writes ────────────────────────────────
// Cluster string + cluster_id pair the writeAttributes frame to the
// right cluster on the wire; `tz_zcl_write_attr` reads
// manufacturer_code from `user_config` and prepends the mfg header.
#define ZHC_DANFOSS_TZ(var, spec_ref, cluster_str, cluster_id_v, key_str) \
    extern const TzConverter var{                                         \
        .key         = key_str,                                           \
        .cluster     = cluster_str,                                       \
        .cluster_id  = cluster_id_v,                                      \
        .command_id  = 0x02,    /* writeAttributes */                     \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                \
        .user_config = &spec_ref,                                         \
    };

ZHC_DANFOSS_TZ(kTzDanfossWindowOpenInternal,
               kSpecWindowOpenInternal,
               "hvacThermostat", 0x0201, "window_open_internal")
ZHC_DANFOSS_TZ(kTzDanfossWindowOpenExternal,
               kSpecWindowOpenExternal,
               "hvacThermostat", 0x0201, "window_open_external")
ZHC_DANFOSS_TZ(kTzDanfossMountedModeControl,
               kSpecMountedModeControl,
               "hvacThermostat", 0x0201, "mounted_mode_control")
ZHC_DANFOSS_TZ(kTzDanfossThermostatOrientation,
               kSpecThermostatOrientation,
               "hvacThermostat", 0x0201, "thermostat_orientation")
ZHC_DANFOSS_TZ(kTzDanfossExternalRoomSensor,
               kSpecExternalRoomSensor,
               "hvacThermostat", 0x0201, "external_measured_room_sensor")
ZHC_DANFOSS_TZ(kTzDanfossRadiatorCovered,
               kSpecRadiatorCovered,
               "hvacThermostat", 0x0201, "radiator_covered")
ZHC_DANFOSS_TZ(kTzDanfossAlgorithmScaleFactor,
               kSpecAlgorithmScaleFactor,
               "hvacThermostat", 0x0201, "algorithm_scale_factor")
ZHC_DANFOSS_TZ(kTzDanfossLoadBalancingEnable,
               kSpecLoadBalancingEnable,
               "hvacThermostat", 0x0201, "load_balancing_enable")
ZHC_DANFOSS_TZ(kTzDanfossLoadRoomMean,
               kSpecLoadRoomMean,
               "hvacThermostat", 0x0201, "load_room_mean")
ZHC_DANFOSS_TZ(kTzDanfossRegulationSetpointOffset,
               kSpecRegulationSetpointOffset,
               "hvacThermostat", 0x0201, "regulation_setpoint_offset")
ZHC_DANFOSS_TZ(kTzDanfossAdaptionRunControl,
               kSpecAdaptionRunControl,
               "hvacThermostat", 0x0201, "adaption_run_control")
ZHC_DANFOSS_TZ(kTzDanfossWindowOpenFeatureEnable,
               kSpecWindowOpenFeatureEnable,
               "hvacThermostat", 0x0201, "window_open_feature")
ZHC_DANFOSS_TZ(kTzDanfossViewingDirection,
               kSpecViewingDirection,
               "hvacUserInterfaceCfg", 0x0204, "viewing_direction")

#undef ZHC_DANFOSS_TZ

}  // namespace zhc::devices::danfoss
