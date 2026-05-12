// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared Schneider Electric converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/schneider_electric.ts
// (helpers in `schneiderElectricExtend.*`)

#include "definitions/schneider/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/foundation.hpp"

namespace zhc::schneider {

// ── Generic manuSpecific decoder ──────────────────────────────────
//
// Same structural pattern as `fz_lumi_manu_specific`: walk the
// `SchneiderAttrMap` from `user_config`, look up the numeric attr id
// in `msg.payload` (keyed by the decimal attr id as a string), coerce
// per `SchneiderAttrType`, emit into `out`.
namespace {

bool decode_one(const SchneiderAttrEntry& e,
                 const Value& v,
                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value o{};
    switch (e.type) {
        case SchneiderAttrType::Raw:
            o = v;
            break;
        case SchneiderAttrType::Bool: {
            bool b = false;
            if      (v.type == ValueType::Bool) b = v.b;
            else if (v.type == ValueType::Uint) b = v.u != 0;
            else if (v.type == ValueType::Int)  b = v.i != 0;
            else return false;
            o.type = ValueType::Bool; o.b = b;
            break;
        }
        case SchneiderAttrType::UintU: {
            std::uint64_t u = 0;
            if      (v.type == ValueType::Uint) u = v.u;
            else if (v.type == ValueType::Int)  u = static_cast<std::uint64_t>(v.i);
            else return false;
            o.type = ValueType::Uint; o.u = u;
            break;
        }
        case SchneiderAttrType::Enum: {
            if (!e.enum_table) return false;
            std::uint64_t raw = 0;
            if      (v.type == ValueType::Uint) raw = v.u;
            else if (v.type == ValueType::Int)  raw = static_cast<std::uint64_t>(v.i);
            else return false;
            const char* label = nullptr;
            for (std::uint8_t j = 0; j < e.enum_count; ++j) {
                if (e.enum_table[j].value == raw) {
                    label = e.enum_table[j].label; break;
                }
            }
            if (!label) return false;
            o.type = ValueType::StringRef; o.str = label;
            break;
        }
    }
    out.put(e.out_key, o);
    return true;
}

bool decode_with_map(const DecodedMessage& msg,
                      const SchneiderAttrMap* map,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (!map || !map->entries || map->count == 0) return false;
    bool emitted = false;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        const auto& e = map->entries[i];
        char keybuf[8];
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;
        if (decode_one(e, *v, out)) emitted = true;
    }
    return emitted;
}

}  // namespace

bool fz_schneider_visa_attrs(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return decode_with_map(msg,
        static_cast<const SchneiderAttrMap*>(self.user_config), out);
}

bool fz_schneider_thermostat_attrs(const DecodedMessage& msg,
                                    const FzConverter& self,
                                    const PreparedDefinition&,
                                    RuntimeContext&,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return decode_with_map(msg,
        static_cast<const SchneiderAttrMap*>(self.user_config), out);
}

bool fz_schneider_ballast_attrs(const DecodedMessage& msg,
                                 const FzConverter& self,
                                 const PreparedDefinition&,
                                 RuntimeContext&,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return decode_with_map(msg,
        static_cast<const SchneiderAttrMap*>(self.user_config), out);
}

// ── Shared enum tables ────────────────────────────────────────────
//
// Z2m source: `schneiderElectricExtend.{visaConfigIndicatorLuminanceLevel,
//   visaConfigIndicatorColor, visaIndicatorMode, visaConfigCurtainStatus}`.

namespace {

constexpr SchneiderEnumEntry kEnumIndicatorLuminance[] = {
    {0, "100"}, {1, "80"}, {2, "60"}, {3, "40"}, {4, "20"}, {5, "0"},
};
constexpr SchneiderEnumEntry kEnumIndicatorColor[] = {
    {0, "white"}, {1, "blue"},
};
// visaIndicatorMode is per-device (the ts helper takes an int[4] of
// raw values for [reverseWithLoad, consistentWithLoad, alwaysOff,
// alwaysOn]) — but every device in z2m so far uses the same
// canonical mapping 1/2/4/8. We expose only the canonical labels.
constexpr SchneiderEnumEntry kEnumIndicatorMode[] = {
    {1, "reverse_with_load"},
    {2, "consistent_with_load"},
    {4, "always_off"},
    {8, "always_on"},
};
constexpr SchneiderEnumEntry kEnumCurtainStatus[] = {
    {0, "stop"}, {1, "opening"}, {2, "closing"},
};

// Wiser fipLookup (used with wiserSmartZoneMode tx; we only need the
// inverse map for fz_xxx attrs that emit a label).
constexpr SchneiderEnumEntry kEnumWiserZoneMode[] = {
    {1, "manual"}, {2, "schedule"}, {3, "energy_saver"}, {6, "holiday"},
};

constexpr SchneiderEnumEntry kEnumDimmerMode[] = {
    {0, "RC"}, {1, "RL"}, {2, "RL_LED"},
};

// ── Per-cluster attribute maps ────────────────────────────────────

constexpr SchneiderAttrEntry kVisaSwitchEntries[] = {
    {0x0000, "indicator_luminance_level", SchneiderAttrType::Enum,
        kEnumIndicatorLuminance,
        sizeof(kEnumIndicatorLuminance) / sizeof(kEnumIndicatorLuminance[0])},
    {0x0001, "indicator_color", SchneiderAttrType::Enum,
        kEnumIndicatorColor,
        sizeof(kEnumIndicatorColor) / sizeof(kEnumIndicatorColor[0])},
    {0x0002, "indicator_mode", SchneiderAttrType::Enum,
        kEnumIndicatorMode,
        sizeof(kEnumIndicatorMode) / sizeof(kEnumIndicatorMode[0])},
};

// Curtain (1ch) extends switch with motorTypeChannel1 + curtainStatusChannel1.
constexpr SchneiderEnumEntry kEnumMotorType[] = {
    {0, "ac_motor"}, {1, "pulse_motor"},
};
constexpr SchneiderAttrEntry kVisaCurtain1Entries[] = {
    {0x0000, "indicator_luminance_level", SchneiderAttrType::Enum,
        kEnumIndicatorLuminance,
        sizeof(kEnumIndicatorLuminance) / sizeof(kEnumIndicatorLuminance[0])},
    {0x0001, "indicator_color", SchneiderAttrType::Enum,
        kEnumIndicatorColor,
        sizeof(kEnumIndicatorColor) / sizeof(kEnumIndicatorColor[0])},
    {0x0002, "indicator_mode", SchneiderAttrType::Enum,
        kEnumIndicatorMode,
        sizeof(kEnumIndicatorMode) / sizeof(kEnumIndicatorMode[0])},
    {0x0003, "motor_type",     SchneiderAttrType::Enum,
        kEnumMotorType,
        sizeof(kEnumMotorType) / sizeof(kEnumMotorType[0])},
    {0x0005, "curtain_status", SchneiderAttrType::Enum,
        kEnumCurtainStatus,
        sizeof(kEnumCurtainStatus) / sizeof(kEnumCurtainStatus[0])},
};

constexpr SchneiderAttrEntry kVisaCurtain2Entries[] = {
    {0x0000, "indicator_luminance_level", SchneiderAttrType::Enum,
        kEnumIndicatorLuminance,
        sizeof(kEnumIndicatorLuminance) / sizeof(kEnumIndicatorLuminance[0])},
    {0x0001, "indicator_color", SchneiderAttrType::Enum,
        kEnumIndicatorColor,
        sizeof(kEnumIndicatorColor) / sizeof(kEnumIndicatorColor[0])},
    {0x0002, "indicator_mode", SchneiderAttrType::Enum,
        kEnumIndicatorMode,
        sizeof(kEnumIndicatorMode) / sizeof(kEnumIndicatorMode[0])},
    {0x0003, "motor_type_1",     SchneiderAttrType::Enum,
        kEnumMotorType,
        sizeof(kEnumMotorType) / sizeof(kEnumMotorType[0])},
    {0x0004, "motor_type_2",     SchneiderAttrType::Enum,
        kEnumMotorType,
        sizeof(kEnumMotorType) / sizeof(kEnumMotorType[0])},
    {0x0005, "curtain_status_1", SchneiderAttrType::Enum,
        kEnumCurtainStatus,
        sizeof(kEnumCurtainStatus) / sizeof(kEnumCurtainStatus[0])},
    {0x0006, "curtain_status_2", SchneiderAttrType::Enum,
        kEnumCurtainStatus,
        sizeof(kEnumCurtainStatus) / sizeof(kEnumCurtainStatus[0])},
};

// Wiser thermostat custom attrs (read-side). We pass through the most
// common ones as raw uints since z2m's per-device per-attr enum maps
// vary a lot; downstream consumers can interpret as needed.
constexpr SchneiderAttrEntry kThermostatEntries[] = {
    {0xE010, "wiser_zone_mode", SchneiderAttrType::Enum,
        kEnumWiserZoneMode,
        sizeof(kEnumWiserZoneMode) / sizeof(kEnumWiserZoneMode[0])},
    {0xE011, "wiser_hact_config",          SchneiderAttrType::UintU, nullptr, 0},
    {0xE110, "wiser_specific",             SchneiderAttrType::UintU, nullptr, 0},
    {0xE211, "control_status",             SchneiderAttrType::UintU, nullptr, 0},
    {0xE212, "local_temperature_source",   SchneiderAttrType::UintU, nullptr, 0},
    {0xE213, "control_type",               SchneiderAttrType::UintU, nullptr, 0},
    {0xE216, "thermostat_application",     SchneiderAttrType::UintU, nullptr, 0},
    {0xE217, "heating_fuel",               SchneiderAttrType::UintU, nullptr, 0},
    {0xE218, "heat_transfer_medium",       SchneiderAttrType::UintU, nullptr, 0},
    {0xE21A, "heating_emitter",            SchneiderAttrType::UintU, nullptr, 0},
};

constexpr SchneiderAttrEntry kBallastEntries[] = {
    {0xE000, "dimmer_mode", SchneiderAttrType::Enum,
        kEnumDimmerMode,
        sizeof(kEnumDimmerMode) / sizeof(kEnumDimmerMode[0])},
};

}  // namespace

extern const SchneiderAttrMap kVisaMapSwitch{
    kVisaSwitchEntries,
    sizeof(kVisaSwitchEntries) / sizeof(kVisaSwitchEntries[0]),
};
extern const SchneiderAttrMap kVisaMapCurtain1{
    kVisaCurtain1Entries,
    sizeof(kVisaCurtain1Entries) / sizeof(kVisaCurtain1Entries[0]),
};
extern const SchneiderAttrMap kVisaMapCurtain2{
    kVisaCurtain2Entries,
    sizeof(kVisaCurtain2Entries) / sizeof(kVisaCurtain2Entries[0]),
};
extern const SchneiderAttrMap kThermostatMap{
    kThermostatEntries,
    sizeof(kThermostatEntries) / sizeof(kThermostatEntries[0]),
};
extern const SchneiderAttrMap kBallastMap{
    kBallastEntries,
    sizeof(kBallastEntries) / sizeof(kBallastEntries[0]),
};

// ── Canonical FzConverters ────────────────────────────────────────

#define ZHC_SCHNEIDER_FZ(var, cluster_str, fn_ptr, map_ref)            \
    extern const FzConverter var{                                       \
        .family            = FrameFamily::Zcl,                          \
        .cluster           = cluster_str,                               \
        .type_mask         = type_bit(MessageType::AttributeReport) |   \
                             type_bit(MessageType::ReadResponse),       \
        .command_id        = WILDCARD_CMD_ID,                           \
        .attr_id           = WILDCARD_ATTR_ID,                          \
        .endpoint          = WILDCARD_ENDPOINT,                         \
        .frame_flags_mask  = 0, .frame_flags_value = 0,                 \
        .direction         = Direction::ServerToClient,                 \
        .fn                = { .zcl_fn = &fn_ptr },                     \
        .user_config       = &map_ref,                                  \
    }

ZHC_SCHNEIDER_FZ(kFzSchneiderVisaSwitch,   "visaConfiguration",
                 fz_schneider_visa_attrs,  kVisaMapSwitch);
ZHC_SCHNEIDER_FZ(kFzSchneiderVisaCurtain1, "visaConfiguration",
                 fz_schneider_visa_attrs,  kVisaMapCurtain1);
ZHC_SCHNEIDER_FZ(kFzSchneiderVisaCurtain2, "visaConfiguration",
                 fz_schneider_visa_attrs,  kVisaMapCurtain2);
ZHC_SCHNEIDER_FZ(kFzSchneiderThermostat,   "hvacThermostat",
                 fz_schneider_thermostat_attrs, kThermostatMap);
ZHC_SCHNEIDER_FZ(kFzSchneiderBallast,      "lightingBallastCfg",
                 fz_schneider_ballast_attrs, kBallastMap);

// addSchneiderLightSwitchConfigurationCluster — same custom cluster
// but it only has one read-relevant attr (ledIndication 0x0000) plus
// a handful of write-only group/scene IDs. We reuse the visa decoder
// shape against a tiny one-entry map.
namespace {
constexpr SchneiderEnumEntry kEnumLedIndication[] = {
    {0, "always_off"}, {1, "on_when_off"},
    {2, "on_when_on"}, {3, "always_on"},
};
constexpr SchneiderAttrEntry kLightSwitchCfgEntries[] = {
    {0x0000, "led_indication", SchneiderAttrType::Enum,
        kEnumLedIndication,
        sizeof(kEnumLedIndication) / sizeof(kEnumLedIndication[0])},
};
}  // namespace
extern const SchneiderAttrMap kLightSwitchCfgMap{
    kLightSwitchCfgEntries,
    sizeof(kLightSwitchCfgEntries) / sizeof(kLightSwitchCfgEntries[0]),
};
ZHC_SCHNEIDER_FZ(kFzSchneiderLightSwitchConfig,
                 "manuSpecificSchneiderLightSwitchConfiguration",
                 fz_schneider_visa_attrs, kLightSwitchCfgMap);

#undef ZHC_SCHNEIDER_FZ

// ── ZclWriteSpec table + TzConverters ─────────────────────────────
//
// Wire format is identical to Lumi's manu-specific writeAttributes —
// `tz_zcl_write_attr` already encodes fc=0x14 (manu + DDR) when
// `manufacturer_code != 0` and emits `mfg_code_LE` after fc.
//
// Per-device cpps just reference these and let the dispatcher pick
// based on the payload key.

namespace {

// visaConfiguration (0xFC04) — mfg 0x105E
constexpr ::zhc::generic::ZclWriteLookup kLookupIndicatorLuminance[] = {
    {"100", 0}, {"80", 1}, {"60", 2}, {"40", 3}, {"20", 4}, {"0", 5},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupIndicatorColor[] = {
    {"white", 0}, {"blue", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupIndicatorMode[] = {
    {"reverse_with_load", 1}, {"consistent_with_load", 2},
    {"always_off", 4},        {"always_on", 8},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupCurtainStatus[] = {
    {"stop", 0}, {"opening", 1}, {"closing", 2},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupMotorType[] = {
    {"ac_motor", 0}, {"pulse_motor", 1},
};
constexpr ::zhc::generic::ZclWriteLookup kLookupDimmerMode[] = {
    {"RC", 0}, {"RL", 1}, {"RL_LED", 2},
};

constexpr ::zhc::generic::ZclWriteSpec kSpecIndicatorLuminance{
    "indicator_luminance_level", 0x0000, 0x30, kSchneiderMfgCode,
    kLookupIndicatorLuminance,
    sizeof(kLookupIndicatorLuminance) / sizeof(kLookupIndicatorLuminance[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecIndicatorColor{
    "indicator_color", 0x0001, 0x30, kSchneiderMfgCode,
    kLookupIndicatorColor,
    sizeof(kLookupIndicatorColor) / sizeof(kLookupIndicatorColor[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecIndicatorMode{
    "indicator_mode", 0x0002, 0x30, kSchneiderMfgCode,
    kLookupIndicatorMode,
    sizeof(kLookupIndicatorMode) / sizeof(kLookupIndicatorMode[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMotorTypeCh1{
    "motor_type_1", 0x0003, 0x20, kSchneiderMfgCode,
    kLookupMotorType,
    sizeof(kLookupMotorType) / sizeof(kLookupMotorType[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMotorTypeCh2{
    "motor_type_2", 0x0004, 0x20, kSchneiderMfgCode,
    kLookupMotorType,
    sizeof(kLookupMotorType) / sizeof(kLookupMotorType[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecCurtainStatusCh1{
    "curtain_status_1", 0x0005, 0x20, kSchneiderMfgCode,
    kLookupCurtainStatus,
    sizeof(kLookupCurtainStatus) / sizeof(kLookupCurtainStatus[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecCurtainStatusCh2{
    "curtain_status_2", 0x0006, 0x20, kSchneiderMfgCode,
    kLookupCurtainStatus,
    sizeof(kLookupCurtainStatus) / sizeof(kLookupCurtainStatus[0]),
};

// manuSpecificSchneiderLightSwitchConfiguration (0xFF17) — mfg 0x105E
constexpr ::zhc::generic::ZclWriteLookup kLookupLedIndication[] = {
    {"always_off", 0}, {"on_when_off", 1},
    {"on_when_on", 2}, {"always_on", 3},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLedIndication{
    "led_indication", 0x0000, 0x30, kSchneiderMfgCode,
    kLookupLedIndication,
    sizeof(kLookupLedIndication) / sizeof(kLookupLedIndication[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpecUpSceneId{
    "up_scene_id", 0x0010, 0x20, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecUpGroupId{
    "up_group_id", 0x0011, 0x21, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDownSceneId{
    "down_scene_id", 0x0020, 0x20, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecDownGroupId{
    "down_group_id", 0x0021, 0x21, kSchneiderMfgCode, nullptr, 0,
};

// hvacThermostat (0x0201) — mfg 0x105E (Wiser custom attrs)
constexpr ::zhc::generic::ZclWriteSpec kSpecWiserSpecific{
    "wiser_specific", 0xE110, 0x30, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLocalTempSrcSelect{
    "local_temperature_source", 0xE212, 0x20, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecControlType{
    "control_type", 0xE213, 0x30, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecThermoApplication{
    "thermostat_application", 0xE216, 0x30, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecHeatingFuel{
    "heating_fuel", 0xE217, 0x30, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecHeatTransferMedium{
    "heat_transfer_medium", 0xE218, 0x30, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecHeatingEmitter{
    "heating_emitter", 0xE21A, 0x30, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecWiserZoneMode{
    "wiser_zone_mode", 0xE010, 0x30, kSchneiderMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecWiserHactConfig{
    "wiser_hact_config", 0xE011, 0x30, kSchneiderMfgCode, nullptr, 0,
};

// lightingBallastCfg (0x0301) — mfg 0x105E
constexpr ::zhc::generic::ZclWriteSpec kSpecDimmerMode{
    "dimmer_mode", 0xE000, 0x30, kSchneiderMfgCode,
    kLookupDimmerMode,
    sizeof(kLookupDimmerMode) / sizeof(kLookupDimmerMode[0]),
};

}  // namespace

#define ZHC_SCHNEIDER_TZ(var, spec_ref, key_str, cluster_str, cluster_id_v) \
    extern const TzConverter var{                                            \
        .key         = key_str,                                              \
        .cluster     = cluster_str,                                          \
        .cluster_id  = cluster_id_v,                                         \
        .command_id  = 0x02,                                                 \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                   \
        .user_config = &spec_ref,                                            \
    }

// visaConfiguration (0xFC04)
ZHC_SCHNEIDER_TZ(kTzSchneiderIndicatorLuminance, kSpecIndicatorLuminance,
                 "indicator_luminance_level", "visaConfiguration", 0xFC04);
ZHC_SCHNEIDER_TZ(kTzSchneiderIndicatorColor, kSpecIndicatorColor,
                 "indicator_color", "visaConfiguration", 0xFC04);
ZHC_SCHNEIDER_TZ(kTzSchneiderIndicatorMode, kSpecIndicatorMode,
                 "indicator_mode", "visaConfiguration", 0xFC04);
ZHC_SCHNEIDER_TZ(kTzSchneiderMotorTypeCh1, kSpecMotorTypeCh1,
                 "motor_type_1", "visaConfiguration", 0xFC04);
ZHC_SCHNEIDER_TZ(kTzSchneiderMotorTypeCh2, kSpecMotorTypeCh2,
                 "motor_type_2", "visaConfiguration", 0xFC04);
ZHC_SCHNEIDER_TZ(kTzSchneiderCurtainStatusCh1, kSpecCurtainStatusCh1,
                 "curtain_status_1", "visaConfiguration", 0xFC04);
ZHC_SCHNEIDER_TZ(kTzSchneiderCurtainStatusCh2, kSpecCurtainStatusCh2,
                 "curtain_status_2", "visaConfiguration", 0xFC04);

// manuSpecificSchneiderLightSwitchConfiguration (0xFF17)
ZHC_SCHNEIDER_TZ(kTzSchneiderLedIndication, kSpecLedIndication,
                 "led_indication",
                 "manuSpecificSchneiderLightSwitchConfiguration", 0xFF17);
ZHC_SCHNEIDER_TZ(kTzSchneiderUpSceneId, kSpecUpSceneId,
                 "up_scene_id",
                 "manuSpecificSchneiderLightSwitchConfiguration", 0xFF17);
ZHC_SCHNEIDER_TZ(kTzSchneiderUpGroupId, kSpecUpGroupId,
                 "up_group_id",
                 "manuSpecificSchneiderLightSwitchConfiguration", 0xFF17);
ZHC_SCHNEIDER_TZ(kTzSchneiderDownSceneId, kSpecDownSceneId,
                 "down_scene_id",
                 "manuSpecificSchneiderLightSwitchConfiguration", 0xFF17);
ZHC_SCHNEIDER_TZ(kTzSchneiderDownGroupId, kSpecDownGroupId,
                 "down_group_id",
                 "manuSpecificSchneiderLightSwitchConfiguration", 0xFF17);

// hvacThermostat (0x0201)
ZHC_SCHNEIDER_TZ(kTzSchneiderWiserSpecific, kSpecWiserSpecific,
                 "wiser_specific", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderLocalTempSrcSelect, kSpecLocalTempSrcSelect,
                 "local_temperature_source", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderControlType, kSpecControlType,
                 "control_type", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderThermoApplication, kSpecThermoApplication,
                 "thermostat_application", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderHeatingFuel, kSpecHeatingFuel,
                 "heating_fuel", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderHeatTransferMedium, kSpecHeatTransferMedium,
                 "heat_transfer_medium", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderHeatingEmitter, kSpecHeatingEmitter,
                 "heating_emitter", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderWiserZoneMode, kSpecWiserZoneMode,
                 "wiser_zone_mode", "hvacThermostat", 0x0201);
ZHC_SCHNEIDER_TZ(kTzSchneiderWiserHactConfig, kSpecWiserHactConfig,
                 "wiser_hact_config", "hvacThermostat", 0x0201);

// lightingBallastCfg (0x0301)
ZHC_SCHNEIDER_TZ(kTzSchneiderDimmerMode, kSpecDimmerMode,
                 "dimmer_mode", "lightingBallastCfg", 0x0301);

#undef ZHC_SCHNEIDER_TZ

// ── Long-tail helpers intentionally NOT ported ────────────────────
//
// The following entries from `schneiderElectricExtend` in z2m are
// not represented here. Each costs ~5 ZclWriteSpec + 1 small enum map
// to land — leave for a follow-up PR.
//
//   * addWiserDeviceInfoCluster                  — Wiser zone/floor
//                                                  metadata cluster.
//   * customFanSwitchConfigurationCluster        — fan-speed labels;
//                                                  z2m generates 4
//                                                  attrs on a custom
//                                                  cluster ID.
//   * dimmingMode                                — `dimmer_mode`
//                                                  toggle on a
//                                                  *different* cluster
//                                                  than `wiserControlMode`.
//   * addHeatingCoolingOutputClusterServer       — Wiser thermostat
//                                                  output config.
//   * visaKeyEventNotification                   — Visa key-press
//                                                  notification (used
//                                                  by E8332RWMZB,
//                                                  E8334RWMZB Freelocate
//                                                  remotes).
//   * addOccupancyConfiguration                  — Wiser PIR-config
//                                                  cluster.
//   * Schneider GreenPower / 0x0B05 PowerTag     — out of scope; needs
//                                                  ZHC core support
//                                                  for unidirectional
//                                                  encrypted GP frames.

}  // namespace zhc::schneider
