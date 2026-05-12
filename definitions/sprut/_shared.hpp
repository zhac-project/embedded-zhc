// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Sprut.device converters — Bed.box ergomotion controller +
// Drivent window drive. Both devices use bespoke ZCL with manufacturer
// code 26214 (0x6666 / "SPRT" reverse). Bed.box layers presentValue
// reads on `genMultistateInput` / `genMultistateOutput`; Drivent
// extends `closuresWindowCovering` with manuSpec attributes plus a set
// of cluster-specific commands (resetLimit / openLimit / closeLimit /
// resetBlock).
//
// z2m source: zigbee-herdsman-converters/src/devices/sprut.ts
// z2m helper: `sendCommand({name, cluster, command, description})` at
// the top of sprut.ts emits manuCode-tagged cluster commands. We
// implement that as `tz_sprut_cluster_cmd` below — fc=0x05
// (cluster-specific + manu) followed by `<mfg-le> <tsn> <cmd_id>`
// with no payload bytes (every Drivent command takes zero parameters).

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::sprut {

// Manufacturer code on every Sprut.device manu-specific frame.
constexpr std::uint16_t kSprutMfgCode = 26214;  // 0x6666

// ── Fz: presentValue (attr 0x0055, single-precision float or int) ─
//
// genMultistateInput  (0x0012) — Bed.box position / intensity readouts
// genMultistateOutput (0x0013) — Bed.box current head/foot feedback
//
// z2m wires `m.numeric/enumLookup` against attribute "presentValue"
// for these clusters. The decoder emits "presentValue" so dispatch's
// endpoint-suffix rewrite stamps the per-EP label (head_position,
// foot_massage_intensity, …). The device cpp lists each Tz binding
// with the right suffix.
extern const FzConverter kFzSprutMultistateInput;
extern const FzConverter kFzSprutMultistateOutput;

// ── Fz: Drivent manuSpec coverings + WiFi cluster ──────────────────
//
// closuresWindowCovering (0x0102) reports the standard CurrentPosition-
// LiftPercentage (attr 0x0008) and the Sprut extras 0x6605..0x660B.
// We mirror kFzCoverPosition for "position" then layer the manuSpec
// readouts.
extern const FzConverter kFzSprutDriventCovering;

// 0x6600 cluster — single attr 0x6600 boolean for the WiFi AP toggle.
extern const FzConverter kFzSprutWifi;

// ── Tz: cluster-command encoder for `sendCommand({...})` ───────────
//
// `user_config` points to a `SprutCommandSpec` with the input key the
// converter claims. Emits a manu-specific cluster-command frame:
//   fc=0x05 | mfg(le) | tsn | cmd_id
// with no payload (all four Drivent commands are zero-parameter).
struct SprutCommandSpec {
    const char* key;        // input payload key, e.g. "open_limit"
};
bool tz_sprut_cluster_cmd(std::string_view key,
                           const Value& input,
                           const TzConverter& self,
                           const PreparedDefinition& def,
                           RuntimeContext& ctx,
                           std::span<std::uint8_t> out_frame,
                           std::size_t& out_size);

// Concrete TzConverters for each Drivent named command. cluster_id is
// `closuresWindowCovering` (0x0102), command_id matches sprut.ts.
extern const TzConverter kTzSprutResetLimit;   // cmd 0x00
extern const TzConverter kTzSprutOpenLimit;    // cmd 0x01
extern const TzConverter kTzSprutCloseLimit;   // cmd 0x02
extern const TzConverter kTzSprutResetBlock;   // cmd 0x03

// ── Tz: Drivent manuSpec writeAttributes ───────────────────────────
//
// Routes through `::zhc::generic::tz_zcl_write_attr` with mfg=0x6666.
// Speed is read/write u8 0..100 step 25; calibrate is a bool toggle;
// wifi (cluster 0x6600 attr 0x6600) is a bool toggle.
extern const TzConverter kTzSprutSpeed;        // cluster 0x0102 attr 0x6606 u8
extern const TzConverter kTzSprutCalibrate;    // cluster 0x0102 attr 0x6605 bool
extern const TzConverter kTzSprutWifi;         // cluster 0x6600 attr 0x6600 bool

}  // namespace zhc::sprut
