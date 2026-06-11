// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared VAV vendor converters.
//
// VAV ships NodOn-derived hardware. The VAV-256215-MOD1 "Pilot wire
// heating module" uses the NodOn custom pilot-wire cluster:
//   0xFC00 customClusterNodOnPilotWire (manufacturerCode NODON)
//     attr 0x0000 mode (UINT8) — exposed as `pilot_wire_mode`
//
// NOTE: cluster id 0xFC00 maps to "manuSpecificPhilips" in the global
// cluster_id_to_name() table, so the runtime stamps that name onto the
// decoded message. The converter therefore matches on the
// "manuSpecificPhilips" cluster name; this is safe because the
// converter is only wired into MOD1's from_zigbee list and reads the
// single `mode` attribute (0x0000) that the NodOn cluster carries.
//
// z2m-source: zigbee-herdsman-converters/src/lib/nodon.ts
//             nodonPilotWire / pilotWireConfig.

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::vav {

// ── fz_pilot_wire_mode ──────────────────────────────────────────────
//
// Decode the NodOn pilot-wire cluster (0xFC00) attr 0x0000 `mode`
// (UINT8) into a `pilot_wire_mode` string. Lookup table (NodOn order):
//   0 → off
//   1 → comfort
//   2 → eco
//   3 → frost_protection
//   4 → comfort_-1
//   5 → comfort_-2
//
// z2m-source: nodon.ts pilotWireConfig.fromZigbee.
bool fz_pilot_wire_mode(const DecodedMessage& msg,
                        const FzConverter& self,
                        const PreparedDefinition& def,
                        RuntimeContext& ctx,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzPilotWireMode;

}  // namespace zhc::vav
