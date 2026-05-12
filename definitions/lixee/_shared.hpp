// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared lixee converters (French Linky / Téléinfo).
//
// z2m-source: zigbee-herdsman-converters/src/devices/lixee.ts
//
// Lixee ZLinky_TIC bridges the French ENEDIS Linky meter's
// "téléinformation client" (TIC) protocol to Zigbee. Three clusters
// carry data:
//   - haElectricalMeasurement (0x0B04)  → live electrical (P/Q/I/U)
//   - seMetering (0x0702)               → cumulative energy (kWh)
//   - liXeePrivate (0xFF66, manuSpec)   → most TIC fields (≈100 attrs)
//
// liXeePrivate is declared with `manufacturerCode:
// Zcl.ManufacturerCode.NXP_SEMICONDUCTORS` (= 0x1037) but z2m reads
// with `manufacturerCode: null`, so the dispatcher matches on cluster
// id alone. Writes for `linkyMode`/`currentTarif` are not implemented
// in v1 (z2m's `toZigbee:[]` for ZLinky).
#pragma once

#include <cstdint>

#include "zhc/runtime/definition.hpp"

namespace zhc::lixee {

using ::zhc::FzConverter;

// One liXeePrivate / haElectricalMeasurement / seMetering attr → key.
// Unlike Lumi's manu-specific cluster (which packs everything into
// 0xFF01 as a TLV blob), Lixee reports each TIC field as a discrete
// attribute. The shared decoder iterates the active map, looks each
// attr up in `msg.payload` (decoded by the parser), and forwards the
// `Value` to the configured output key.
//
// `unit_div` (when non-zero) divides numeric values for Wh→kWh. The
// canonical TIC kWh registers (EAST/EAIT/EASF*/EASD*) come in Wh,
// z2m reduces by 1000. For attrs that need raw passthrough leave
// `unit_div` = 0.
enum class LixeeFieldType : std::uint8_t {
    Raw,        // Uint or Int passthrough (after optional /unit_div)
    String,     // CHAR_STR / OCTET_STR — keep as StringRef
    Bitmap,     // Uint kept verbatim (e.g. relais bits, statusRegister)
};
struct LixeeAttrEntry {
    std::uint16_t   attr_id;
    const char*     out_key;
    LixeeFieldType  type;
    std::uint16_t   unit_div;   // 0 = no scaling
};
struct LixeeAttrMap {
    const LixeeAttrEntry* entries;
    std::uint8_t          count;
};

// One generic decoder, parameterised on the per-cluster map via
// `FzConverter.user_config`. Same shape on every cluster — the
// parser already routes by cluster name, so the decoder is map-agnostic.
bool fz_lixee_attrs(const ::zhc::DecodedMessage& msg,
                    const FzConverter& self,
                    const ::zhc::PreparedDefinition& def,
                    ::zhc::RuntimeContext& ctx,
                    ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// ── Shared maps ─────────────────────────────────────────────────────
//
// We cover the most-common 25 TIC attrs (the "auto" tariff set z2m
// exposes by default plus the global meter-id fields). Tariff-mode
// specific subsets (BBR / EJP / TEMPO daily counters) are deferred
// to v2; the unmatched-attr path simply ignores them so frames
// containing them won't crash the decoder.

// liXeePrivate (0xFF66) — TIC text + tariff/contract metadata.
extern const LixeeAttrMap kLixeePrivateMap;
// haElectricalMeasurement (0x0B04) — instantaneous power/voltage.
extern const LixeeAttrMap kLixeeElecMap;
// seMetering (0x0702) — cumulative energy registers (Wh→kWh).
extern const LixeeAttrMap kLixeeMeterMap;

// Canonical FzConverters — one per cluster, each pointing at the
// matching map above via `user_config`.
extern const FzConverter kFzLixeePrivate;
extern const FzConverter kFzLixeeElec;
extern const FzConverter kFzLixeeMetering;

}  // namespace zhc::lixee
