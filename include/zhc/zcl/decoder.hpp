// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <span>

#include "zhc/types.hpp"
#include "zhc/zcl/foundation.hpp"

namespace zhc {

// Orchestrator: parse an inbound APS frame into a DecodedMessage.
//
//   * Parses the ZCL header from `frame.data`.
//   * Classifies `family` (Zcl vs TuyaDp) from cluster id + FC.
//   * Classifies `type` from foundation command id or cluster-specific bit.
//   * For AttributeReport / ReadResponse, runs the foundation record
//     walker and fills `out.payload`. Unknown attribute ids are formatted
//     into `out.unknown_key_scratch`.
//   * For other types (Read / Write / Command / Raw) the caller / dispatch
//     layer walks `out.raw_data` directly — the core parser stays out of
//     the way until domain-specific converters decide what to do.
//
// `known` is a caller-supplied attr_id → static_string_key table used to
// resolve well-known attributes. Pass `{}` when the caller only wants the
// decimal-string fallback.
//
// Returns false on malformed frames (header truncated, value-length
// mismatch, scratch / payload overflow). On failure `out` is partially
// populated but not safe to consume.
bool decode_frame(const InboundApsFrame& frame,
                  std::span<const AttrKeyEntry> known,
                  DecodedMessage& out);

}  // namespace zhc
