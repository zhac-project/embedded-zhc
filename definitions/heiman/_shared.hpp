// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Heiman vendor converters covering the manuSpecific clusters
// `heimanClusterSpecial` (0xFC90) and `heimanClusterRadar` (0xFC8B),
// plus the `heimanSpecificAirQuality` (0xFC81) attr-report stream.
//
// Every device that pulls from this header should carry a
// `// Tier 2: uses shared heiman converters` comment and a
// `// z2m-source:` provenance line.
//
// z2m-source: zigbee-herdsman-converters/src/devices/heiman.ts
//             (heimanExtend object, heimanClusterSpecialfz local Fz)
//             zigbee-herdsman-converters/src/lib/heiman.ts
//             (addCustomClusterHeimanSpecificAirQuality).

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::heiman {

using ::zhc::generic::ZclWriteSpec;

// HEIMAN_TECHNOLOGY_CO_LTD manufacturer code. Used both for
// `writeAttributes` (fc=0x14) and for cluster-specific commands
// such as `initiateTestMode`.
constexpr std::uint16_t kHeimanMfgCode = 0x120B;

// ── Fz dispatcher: heimanClusterSpecial (0xFC90) ───────────────────
//
// One generic decoder that walks a per-device map of attr-id → output
// key. Devices wire `kFzHeimanSpecial` into `from_zigbee` and supply
// a `HeimanSpecificMap*` via `user_config`. Mirrors the lumi
// `fz_lumi_manu_specific` pattern.

enum class HeimanSpecificType : std::uint8_t {
    Raw,        // pass through as Int / Uint
    Bool,       // coerce to bool (non-zero = true)
    BitmapText, // BITMAP16 → semicolon-joined active-bit labels
};

struct HeimanBitmapEntry {
    std::uint8_t bit;       // 0..15
    const char*  label;
};

struct HeimanSpecificEntry {
    std::uint16_t            attr_id;
    const char*              out_key;
    HeimanSpecificType       type;
    const HeimanBitmapEntry* bitmap_table;   // BitmapText only
    std::uint8_t             bitmap_count;
};

struct HeimanSpecificMap {
    const HeimanSpecificEntry* entries;
    std::uint8_t               count;
};

bool fz_heiman_manu_specific(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Canonical FzConverter — devices choose a map via per-device
// instantiation if their attribute set is unique. A shared "smoke"
// map covers HS1SA-E-PLUS / HS3SA / HS1SA-E and any other smoke
// detector that publishes the same heimanClusterSpecial subset.
extern const HeimanSpecificMap kHeimanMapSmokeDetector;
extern const FzConverter       kFzHeimanSmokeSpecial;

// Air-quality map — heimanSpecificAirQuality (0xFC81) attribute
// reports for HS2AQ-EM / HS2AQ-EF-3.0 / HS3AQ. Most attrs are raw
// uint passthrough; batteryState becomes a labelled enum string
// elsewhere, kept out for simplicity.
extern const HeimanSpecificMap kHeimanMapAirQuality;
extern const FzConverter       kFzHeimanAirQuality;

// ── Tz writers: heimanClusterSpecial manuSpecific writeAttributes ──
//
// Each TzConverter wraps the generic `tz_zcl_write_attr` encoder
// (which already handles fc=0x14 + mfg-code prefix when
// `ZclWriteSpec.manufacturer_code != 0`).

extern const TzConverter kTzHeimanFaultStateRead;        // tagged "fault_state" — read-only via shadow
extern const TzConverter kTzHeimanMutedRead;             // "muted" read-only on deviceMuteState
extern const TzConverter kTzHeimanIndicatorLight;        // "heartbeat_indicator" → indicatorLightLevelControlOf1
extern const TzConverter kTzHeimanInterconnectableRead;  // "interconnectable" read-only
extern const TzConverter kTzHeimanTemporaryMute;         // "temporary_mute" → deviceMuteControl
extern const TzConverter kTzHeimanRadarEnableIndicator;  // "enable_indicator" on heimanClusterRadar

// ── iasZoneInitiateTestMode (cluster ssIasZone, cmd 0x02) ──────────
//
// Per z2m, payload is two u8: testModeDuration, sensitivityLevel.
// Triggered by the "trigger_selftest" key (any value); we pin the
// duration + sensitivity to z2m's defaults of 0x01.
bool tz_heiman_initiate_test_mode(std::string_view key,
                                   const Value& input,
                                   const TzConverter& self,
                                   const PreparedDefinition& def,
                                   RuntimeContext& ctx,
                                   std::span<std::uint8_t> out_frame,
                                   std::size_t& out_size);

extern const TzConverter kTzHeimanInitiateTestMode;

}  // namespace zhc::heiman
