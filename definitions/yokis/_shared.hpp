// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Yokis vendor converters. Every device that pulls from this
// header should carry a `// Tier 1/2: uses shared yokis converters`
// comment and a `// z2m-source:` provenance line.
//
// Yokis manufacturer code (z2m): Zcl.ManufacturerCode.YOKIS = 0x132D.
// Cluster table (from src/devices/yokis.ts YokisClustersDefinition):
//   manuSpecificYokisDevice            0xFC01
//   manuSpecificYokisInput             0xFC02
//   manuSpecificYokisEntryConfigurator 0xFC03
//   manuSpecificYokisSubSystem         0xFC04
//   manuSpecificYokisLoadManager       0xFC05
//   manuSpecificYokisLightControl      0xFC06
//   manuSpecificYokisDimmer            0xFC07
//   manuSpecificYokisWindowCovering    0xFC08
//   manuSpecificYokisChannel           0xFC09
//   manuSpecificYokisPilotWire         0xFC0A
//   manuSpecificYokisTemperatureMeasurement 0xFC0B
//   manuSpecificYokisStats             0xFCF0

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"
#include "definitions/_generic/_shared.hpp"

namespace zhc::yokis {

inline constexpr std::uint16_t kYokisManufacturerCode = 0x132D;

// ── Fz: data-driven manuSpecificYokis* attribute-report decoder ─
//
// Mirrors the lumi pattern: each device wires a `YokisAttrMap*` in
// the FzConverter's `user_config` and the converter walks the
// payload, mapping declared attr_id → output key + value type +
// optional enum lookup. Cluster identity is selected by the per-
// converter `cluster` field (so one decoder serves every YokisXxx
// cluster — distinct cluster names share one switch).
enum class YokisAttrType : std::uint8_t {
    Raw,    // passthrough as Int / Uint
    Bool,   // coerce to bool (non-zero = true)
    Enum,   // use enum_table
};
struct YokisAttrEnumEntry {
    std::uint64_t value;
    const char*   label;
};
struct YokisAttrEntry {
    std::uint16_t attr_id;
    const char*   out_key;
    YokisAttrType type;
    const YokisAttrEnumEntry* enum_table;
    std::uint8_t              enum_count;
};
struct YokisAttrMap {
    const YokisAttrEntry* entries;
    std::uint8_t          count;
};

bool fz_yokis_manu_specific(const DecodedMessage& msg,
                             const FzConverter& self,
                             const PreparedDefinition& def,
                             RuntimeContext& ctx,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Canonical maps (one per cluster the Fz decoder is wired against).
// Per-device extra attrs can declare their own map.
extern const YokisAttrMap kYokisLightControlMap;  // FC06
extern const YokisAttrMap kYokisDimmerMap;        // FC07
extern const YokisAttrMap kYokisPilotWireMap;     // FC0A — incl. actualOrder
extern const YokisAttrMap kYokisInputMap;         // FC02
extern const YokisAttrMap kYokisEntryMap;         // FC03
extern const YokisAttrMap kYokisChannelMap;       // FC09
extern const YokisAttrMap kYokisSubSystemMap;     // FC04 — powerFailureMode

// Canonical FzConverters — one per cluster, each pointing at the
// matching map above. Devices wire whichever clusters apply.
extern const FzConverter kFzYokisLightControl;
extern const FzConverter kFzYokisDimmer;
extern const FzConverter kFzYokisPilotWire;
extern const FzConverter kFzYokisInput;
extern const FzConverter kFzYokisEntry;
extern const FzConverter kFzYokisChannel;
extern const FzConverter kFzYokisSubSystem;

// ── Tz: writeAttributes encoders ────────────────────────────────
//
// Every Tz here just delegates to `::zhc::generic::tz_zcl_write_attr`
// with a per-attribute `ZclWriteSpec` carrying:
//   manufacturer_code = 0x132D (forces fc=0x14 + manu code bytes)
//   attr_id           = z2m attribute id
//   attr_type         = ZCL data-type byte (0x10 bool, 0x20 u8, 0x21
//                       u16, 0x23 u32, 0x30 enum8)
//
// The Tz key is the z2m attribute name (snake_case). Enums use a
// `ZclWriteLookup` table so the SPA may submit either the label
// ("on", "previous", "infinite") or the raw uint.

// LightControl (0xFC06). The blink + NC + power-failure surface.
extern const TzConverter kTzYokisOnTimer;          // 0x0002 u32
extern const TzConverter kTzYokisEOnTimer;         // 0x0003 bool
extern const TzConverter kTzYokisPreOnDelay;       // 0x0004 u32
extern const TzConverter kTzYokisEPreOnDelay;      // 0x0005 bool
extern const TzConverter kTzYokisPreOffDelay;      // 0x0008 u32
extern const TzConverter kTzYokisEPreOffDelay;     // 0x0009 bool
extern const TzConverter kTzYokisPulseDuration;    // 0x000A u16
extern const TzConverter kTzYokisLongOnDuration;   // 0x000C u32
extern const TzConverter kTzYokisOperatingMode;    // 0x000D enum8
extern const TzConverter kTzYokisEStopAnnounce;    // 0x0014 bool
extern const TzConverter kTzYokisEDeaf;            // 0x0015 bool
extern const TzConverter kTzYokisEBlink;           // 0x0016 bool
extern const TzConverter kTzYokisBlinkAmount;      // 0x0017 u8
extern const TzConverter kTzYokisBlinkOnTime;      // 0x0018 u32
extern const TzConverter kTzYokisBlinkOffTime;     // 0x0019 u32
extern const TzConverter kTzYokisStateAfterBlink;  // 0x001C enum8
extern const TzConverter kTzYokisENcCommand;       // 0x001D bool

// Dimmer (0xFC07).
extern const TzConverter kTzYokisMemoryPosition;   // 0x0001 u8
extern const TzConverter kTzYokisERampUp;          // 0x0002 bool
extern const TzConverter kTzYokisRampUp;           // 0x0003 u32
extern const TzConverter kTzYokisERampDown;        // 0x0004 bool
extern const TzConverter kTzYokisRampDown;         // 0x0005 u32
extern const TzConverter kTzYokisStepUp;           // 0x0007 u8
extern const TzConverter kTzYokisStepDown;         // 0x0016 u8
extern const TzConverter kTzYokisLowDimLimit;      // 0x0008 u8
extern const TzConverter kTzYokisHighDimLimit;     // 0x0009 u8

// PilotWire (0xFC0A) — attr writes only. setOrder/toggleOrder are
// cluster-specific commands and currently TODO (see _shared.cpp).
extern const TzConverter kTzYokisOrderTimer;       // 0x0001 u32
extern const TzConverter kTzYokisPreOrderTimer;    // 0x0002 u32
extern const TzConverter kTzYokisLedMode;          // 0x0004 u8
extern const TzConverter kTzYokisFallbackOrder;    // 0x0008 u8

// SubSystem (0xFC04).
extern const TzConverter kTzYokisPowerFailureMode; // 0x0001 enum8

// Input (0xFC02).
extern const TzConverter kTzYokisInputMode;        // 0x0000 enum8
extern const TzConverter kTzYokisContactMode;      // 0x0001 bool

// Entry / Configurator (0xFC03).
extern const TzConverter kTzYokisEShortPress;        // 0x0001 bool
extern const TzConverter kTzYokisELongPress;         // 0x0002 bool
extern const TzConverter kTzYokisLongPressDuration;  // 0x0003 u16
extern const TzConverter kTzYokisTimeBetweenPress;   // 0x0004 u16

// PilotWire setOrder cluster command (cluster 0xFC0A, cmd 0x00).
// Builds a manu-specific cluster-cmd frame:
//   fc=0x15 (cluster-specific + manu-specific + DDR + c→s),
//   manu_code_LE (0x132D), tsn=0x00, cmd=0x00, payload=[order:u8].
// Input: payload key "pilotwire_order"; accepts uint or one of
// {stop, frost_off, eco, confort_2, confort_1, confort}.
extern const TzConverter kTzYokisPilotWireSetOrder;

}  // namespace zhc::yokis
