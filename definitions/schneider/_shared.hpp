// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Schneider Electric (Wiser / Clipsal / Merten / LK / Elko)
// vendor converters. Top 5+2 helpers from
// `zigbee-herdsman-converters/src/devices/schneider_electric.ts`
// `schneiderElectricExtend.*` namespace, the rest of the long tail
// (addWiserDeviceInfoCluster, customFanSwitchConfigurationCluster,
// dimmingMode, addHeatingCoolingOutputClusterServer,
// visaKeyEventNotification, etc.) is left for a follow-up.
//
// Schneider manufacturer code is `0x105E`
// (Zcl.ManufacturerCode.SCHNEIDER_ELECTRIC).
//
// Custom clusters introduced here:
//
//   * `visaConfiguration`            ID 0xFC04 — Visa light/curtain config
//   * `manuSpecificSchneiderLightSwitchConfiguration`
//                                    ID 0xFF17 — light-switch LED indication
//   * `lightingBallastCfg` (0x0301) — extra Wiser-specific attribute 0xE000
//   * `hvacThermostat`    (0x0201) — extra Wiser thermostat attrs (0xE110,
//                                    0xE211..0xE21A, 0xE010, 0xE011, ...)
//
// All writes to these attributes go through `tz_zcl_write_attr` (the
// generic helper in `_generic/_shared.cpp`) with a `ZclWriteSpec` that
// carries the Schneider mfgcode so the wire frame is stamped with
// fc=0x14 + mfg_code bytes.
//
// All reads/decodes go through `fz_schneider_visa_attrs` /
// `fz_schneider_thermostat_attrs` — light-weight dispatch tables that
// publish well-known attribute IDs as named state keys.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::schneider {

constexpr std::uint16_t kSchneiderMfgCode = 0x105E;

// ── manuSpecific decoder dispatch tables ──────────────────────────
//
// Each entry maps an attribute ID seen on the manuSpecific cluster to
// a state-key + value-coercion mode. Used by the canonical
// `fz_schneider_visa_attrs` / `fz_schneider_thermostat_attrs` decoders;
// per-device cpps just point an FzConverter's `user_config` at one of
// the shared maps below.

enum class SchneiderAttrType : std::uint8_t {
    Raw,    // pass through the Value as-is (Uint / Int / Bool)
    Bool,   // coerce numeric → bool
    Enum,   // numeric → label via enum_table
    UintU,  // numeric → unsigned integer (named state)
};

struct SchneiderEnumEntry {
    std::uint64_t value;
    const char*   label;
};

struct SchneiderAttrEntry {
    std::uint16_t       attr_id;
    const char*         out_key;
    SchneiderAttrType   type;
    const SchneiderEnumEntry* enum_table;
    std::uint8_t              enum_count;
};

struct SchneiderAttrMap {
    const SchneiderAttrEntry* entries;
    std::uint8_t              count;
};

// Decode `visaConfiguration` (cluster 0xFC04) attribute reports into
// well-known state keys (`indicator_luminance_level`, `indicator_color`,
// `indicator_mode`, `motor_type`, `curtain_status`, ...). The exact set
// depends on the per-device map handed in via `user_config`.
//
// z2m-source: schneiderElectricExtend.{visaIndicatorMode,
//             visaConfigIndicatorColor, visaConfigIndicatorLuminanceLevel,
//             visaConfigCurtainStatus, ...}
bool fz_schneider_visa_attrs(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Decode `hvacThermostat` (0x0201) Schneider-specific manu attributes.
// Same pattern, used by Wiser thermostats / boiler actuators that ship
// with `customThermostatCluster`.
bool fz_schneider_thermostat_attrs(const DecodedMessage& msg,
                                    const FzConverter& self,
                                    const PreparedDefinition& def,
                                    RuntimeContext& ctx,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Decode `lightingBallastCfg` (0x0301) Schneider-specific
// `wiserControlMode` attribute (0xE000). Single attribute → emits
// `dimmer_mode` enum.
bool fz_schneider_ballast_attrs(const DecodedMessage& msg,
                                 const FzConverter& self,
                                 const PreparedDefinition& def,
                                 RuntimeContext& ctx,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Shared maps + canonical FzConverters wired at namespace scope.
// Per-device cpps just include this header and reference these.
extern const SchneiderAttrMap kVisaMapSwitch;        // indicator_*
extern const SchneiderAttrMap kVisaMapCurtain1;      // curtain_status_1 + indicator_*
extern const SchneiderAttrMap kVisaMapCurtain2;      // curtain_status_1/2 + indicator_*
extern const SchneiderAttrMap kThermostatMap;        // wiser thermostat manu attrs
extern const SchneiderAttrMap kBallastMap;           // wiserControlMode

extern const FzConverter kFzSchneiderVisaSwitch;
extern const FzConverter kFzSchneiderVisaCurtain1;
extern const FzConverter kFzSchneiderVisaCurtain2;
extern const FzConverter kFzSchneiderThermostat;
extern const FzConverter kFzSchneiderBallast;
extern const FzConverter kFzSchneiderLightSwitchConfig;  // 0xFF17 ledIndication

// ── TzConverters — one per writable attribute ─────────────────────
//
// Each converter targets a single payload key (e.g. "indicator_mode")
// and dispatches to `tz_zcl_write_attr` with a manu-specific spec.
// String inputs are accepted; the generic helper does the lookup.

// visaConfiguration (0xFC04, mfg 0x105E):
extern const TzConverter kTzSchneiderIndicatorLuminance;  // 0x0000 enum8
extern const TzConverter kTzSchneiderIndicatorColor;      // 0x0001 enum8
extern const TzConverter kTzSchneiderIndicatorMode;       // 0x0002 enum8
extern const TzConverter kTzSchneiderMotorTypeCh1;        // 0x0003 u8
extern const TzConverter kTzSchneiderMotorTypeCh2;        // 0x0004 u8
extern const TzConverter kTzSchneiderCurtainStatusCh1;    // 0x0005 u8
extern const TzConverter kTzSchneiderCurtainStatusCh2;    // 0x0006 u8

// manuSpecificSchneiderLightSwitchConfiguration (0xFF17, mfg 0x105E):
extern const TzConverter kTzSchneiderLedIndication;       // 0x0000 enum8
extern const TzConverter kTzSchneiderUpSceneId;           // 0x0010 u8
extern const TzConverter kTzSchneiderUpGroupId;           // 0x0011 u16
extern const TzConverter kTzSchneiderDownSceneId;         // 0x0020 u8
extern const TzConverter kTzSchneiderDownGroupId;         // 0x0021 u16

// hvacThermostat (0x0201, mfg 0x105E) — Wiser custom attrs:
extern const TzConverter kTzSchneiderWiserSpecific;       // 0xE110 enum8
extern const TzConverter kTzSchneiderLocalTempSrcSelect;  // 0xE212 u8
extern const TzConverter kTzSchneiderControlType;         // 0xE213 enum8
extern const TzConverter kTzSchneiderThermoApplication;   // 0xE216 enum8
extern const TzConverter kTzSchneiderHeatingFuel;         // 0xE217 enum8
extern const TzConverter kTzSchneiderHeatTransferMedium;  // 0xE218 enum8
extern const TzConverter kTzSchneiderHeatingEmitter;      // 0xE21A enum8
extern const TzConverter kTzSchneiderWiserZoneMode;       // 0xE010 enum8
extern const TzConverter kTzSchneiderWiserHactConfig;     // 0xE011 enum8

// lightingBallastCfg (0x0301, mfg 0x105E) — Wiser custom attr:
extern const TzConverter kTzSchneiderDimmerMode;          // 0xE000 enum8

// (Long tail intentionally deferred — see _shared.cpp comment block at
// the bottom for the helpers we did NOT port and the reason.)

}  // namespace zhc::schneider
