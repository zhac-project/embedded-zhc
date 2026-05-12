// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Linkind plumbing — modelled on osram/_shared.
//
// Linkind's z2m surface is `m.light({})` and `m.light({colorTemp: ...})`
// over stock ZCL clusters (no Color RGB, no manuSpec) plus a handful
// of IAS sensors and ssIasAce / command_* button remotes (hand-rolled
// per-device).
//
// Two shared bundles cover the m.light() permutations seen in
// linkind.ts. No Color/ColorCT bundles are needed — Linkind ships only
// dim and tunable-white lights.
//
//   Tier         | feature set                          | bindings
//   -------------+--------------------------------------+----------------
//   Light        | m.light({}) — plain dimmable          | 0x0006, 0x0008
//   CTLight      | + colorTemp (mired)                   | + 0x0300

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::linkind {

// ── Plain dimmable (m.light({})) ────────────────────────────────────
extern const ::zhc::FzConverter* const kFzLinkindLight[];
extern const std::uint8_t              kFzLinkindLightCount;

extern const ::zhc::TzConverter* const kTzLinkindLight[];
extern const std::uint8_t              kTzLinkindLightCount;

extern const ::zhc::Expose             kExposesLinkindLight[];
extern const std::uint8_t              kExposesLinkindLightCount;

extern const ::zhc::BindingSpec        kBindingsLinkindLight[];
extern const std::uint8_t              kBindingsLinkindLightCount;

// ── Tunable white (m.light({colorTemp})) ────────────────────────────
extern const ::zhc::FzConverter* const kFzLinkindCTLight[];
extern const std::uint8_t              kFzLinkindCTLightCount;

extern const ::zhc::TzConverter* const kTzLinkindCTLight[];
extern const std::uint8_t              kTzLinkindCTLightCount;

extern const ::zhc::Expose             kExposesLinkindCTLight[];
extern const std::uint8_t              kExposesLinkindCTLightCount;

extern const ::zhc::BindingSpec        kBindingsLinkindCTLight[];
extern const std::uint8_t              kBindingsLinkindCTLightCount;

}  // namespace zhc::devices::linkind
