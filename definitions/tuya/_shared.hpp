// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Tuya converters. Every device that pulls from here should
// carry a `// Tier 2: shared tuya converters` comment + a
// `// z2m-source:` provenance line.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/tuya_dp.hpp"

namespace zhc::tuya {

// ── fz_tuya_mcu_sync_time ─────────────────────────────────────────
//
// Matches `manuSpecificTuya` (0xEF00) cluster-specific command 0x24
// (`mcuSyncTime`). On match, fires `RuntimeContext::tuya_sync_time`
// with the device index + the inbound transaction sequence number so
// the adapter can craft the reply using its own radio API.
//
// The converter itself emits nothing and always returns true —
// matching claims the frame so the "unhandled" hook doesn't fire for
// this routine chatter.
//
// z2m-source: tuya.ts `fz.tuya_set_time_request`.
bool fz_tuya_mcu_sync_time(const DecodedMessage& msg,
                             const FzConverter& self,
                             const PreparedDefinition& def,
                             RuntimeContext& ctx,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzTuyaMcuSyncTime;

// ── fz_tuya_datapoints ─────────────────────────────────────────────
//
// Meta-driven generic for the ~287 Tuya devices that use the DP-stream
// pattern. Each device's `FzConverter::user_config` points at a
// `TuyaDatapointMap` that maps dp_ids to output keys, types, scaling,
// and (optional) enum lookup tables.
//
// Decoding path:
//   - Unknown dp_id → silently skipped.
//   - Malformed payload (wrong length for declared type) → skipped;
//     other DPs in the same frame still processed.
//   - `divisor > 1` on Numeric → emit Float = raw / divisor.
//   - `enum_table` non-null on Enum → emit StringRef from lookup;
//     unmapped enum values drop.
//
// z2m-source: tuya.ts `fz.datapoints`.

struct TuyaEnumEntry {
    std::uint64_t value;
    const char*   label;
};

struct TuyaDpMapEntry {
    std::uint8_t         dp_id;
    const char*          out_key;
    TuyaDpType           type;
    std::int32_t         divisor;      // 0 / 1 → integer pass-through;
                                         //  >1 → raw/divisor as Float;
                                         //  <0 → raw * abs(divisor) as Int.
    const TuyaEnumEntry* enum_table;   // optional, used when type == Enum
    std::uint8_t         enum_count;
    std::uint8_t         flags{0};     // bit 0 = invert Bool (trueFalse0)
    // Float divisor for fractional scaling (e.g. fl-oz → L = /33.814).
    // Non-zero takes precedence over the int `divisor`. Decode emits
    // Float = raw / divisor_f. Encode multiplies input by divisor_f
    // and rounds. Default 0.0 preserves int-divisor semantics for
    // every existing port (638+ generated cpps unaffected).
    float                divisor_f{0.0f};
};

inline constexpr std::uint8_t kTuyaDpFlagInvertBool     = 0x01;
// Cover position inverted — emit `100 - raw` so 0=closed, 100=open
// aligns with z2m regardless of which way the Tuya firmware counts.
inline constexpr std::uint8_t kTuyaDpFlagInvertPosition = 0x02;
// Thermostat schedule — the raw DP bytes are a packed 4-period
// day schedule: 4× [hour:1][minute:1][temp*2:1]. Decode emits a
// formatted string `"HH:MM/T.T HH:MM/T.T HH:MM/T.T HH:MM/T.T"`.
// Works only on DPs whose declared type is Raw / String (BytesRef
// on the wire). tz encode is not implemented yet — see §T2 in the
// parity plan.
inline constexpr std::uint8_t kTuyaDpFlagScheduleDay    = 0x04;

struct TuyaDatapointMap {
    const TuyaDpMapEntry* entries;
    std::uint8_t          count;
};

bool fz_tuya_datapoints(std::span<const TuyaDpRecord> dps,
                         const DecodedMessage& msg,
                         const FzConverter& self,
                         const PreparedDefinition& def,
                         RuntimeContext& ctx,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzTuyaDatapoints;

// ── Non-DP Tuya converters ─────────────────────────────────────────

bool fz_tuya_power_on_behavior(const DecodedMessage& msg,
                                 const FzConverter& self,
                                 const PreparedDefinition& def,
                                 RuntimeContext& ctx,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzTuyaPowerOnBehavior;

bool fz_tuya_indicator_mode(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzTuyaIndicatorMode;

bool fz_tuya_operation_mode(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzTuyaOperationMode;

bool fz_tuya_on_off_action(const DecodedMessage& msg,
                             const FzConverter& self,
                             const PreparedDefinition& def,
                             RuntimeContext& ctx,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzTuyaOnOffAction;

// Decode the Tuya-private `tuyaAction` command (genOnOff cmd 0xFD):
// body = { value:u8, data:buffer }. Emits
//   action       = "switch_scene"
//   action_scene = value
// z2m-source: lib/tuya.ts `fz.switch_scene`.
bool fz_tuya_switch_scene(const DecodedMessage& msg,
                           const FzConverter& self,
                           const PreparedDefinition& def,
                           RuntimeContext& ctx,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzTuyaSwitchScene;

// Tuya multi-action decoder — handles BOTH custom genOnOff commands
// used by smart-knob / scene-remote TS004F family:
//   cmd 0xFD `tuyaAction`   body { value:u8 } → "single"/"double"/"hold"
//   cmd 0xFC `tuyaAction2`  body { value:u8 } → "rotate_right"/"rotate_left"
// Emits a single `action` string. Registered with
// `command_id = WILDCARD_CMD_ID`; the function filters by
// `msg.command_id` internally so unrelated commands fall through.
//
// z2m-source: lib/tuya.ts `fz.multi_action`.
bool fz_tuya_multi_action(const DecodedMessage& msg,
                            const FzConverter& self,
                            const PreparedDefinition& def,
                            RuntimeContext& ctx,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzTuyaMultiAction;

extern const FzConverter kFzIgnoreTuyaSetTime;

// ── tz_tuya_datapoints (write path) ────────────────────────────────
//
// Inverse of `fz_tuya_datapoints`. Looks up the DP map entry by input
// key, encodes a manuSpecificTuya (0xEF00) cmd 0x00 (setData) frame:
//   header: [fc=0x01][tsn=0x00 placeholder][cmd=0x00]
//   body:   [seq_be:2=0x00 0x01][dp_id:1][dp_type:1][len_be:2][value:N]
//
// Value encoding:
//   Bool     — 1 byte (0x00 / 0x01, honours kTuyaDpFlagInvertBool)
//   Numeric  — 4 bytes big-endian s32 (multiplied by divisor — or,
//              when divisor is negative, divided by abs(divisor))
//   Enum     — 1 byte (looked up in enum_table by input StringRef;
//              numeric inputs pass through as-is)
//
// The converter's user_config points at the same `TuyaDatapointMap`
// used by the fz side. `TzConverter::key = nullptr` makes it claim any
// key that the map lists.
bool tz_tuya_datapoints(std::string_view key,
                         const Value& input,
                         const TzConverter& self,
                         const PreparedDefinition& def,
                         RuntimeContext& ctx,
                         std::span<std::uint8_t> out,
                         std::size_t& out_size);
extern const TzConverter kTzTuyaDatapointsProto;

// ── Tuya non-DP attribute writes ─────────────────────────────────
//
// Aqara-style attribute writes on `genOnOff` for Tuya devices that
// expose these features via the private 0x80xx attribute range
// (vs. the DP stream). Data-driven via the generic `ZclWriteSpec`.
//
// z2m-source: lib/tuya.ts tz.power_on_behavior / operation_mode /
//             indicator_mode / child_lock.
extern const TzConverter kTzTuyaPowerOnBehavior;   // attr 0x8002 enum
extern const TzConverter kTzTuyaOperationMode;     // attr 0x8004 enum
extern const TzConverter kTzTuyaIndicatorMode;     // attr 0x8001 enum
extern const TzConverter kTzTuyaChildLock;         // attr 0x8000 bool

}  // namespace zhc::tuya
