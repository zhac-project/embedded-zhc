// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared J.XUAN vendor converters. Devices pulling from this header
// should carry a `// Tier 2: uses shared jxuan converters` comment and a
// `// z2m-source:` provenance line.
//
// z2m-source: zigbee-herdsman-converters/src/devices/jxuan.ts +
//             converters/fromZigbee.ts.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::jxuan {

// ── WSZ01 wireless-switch button action (cluster manuSpecificJxuan) ───
//
// The WSZ01 reports button events on its manuSpecific cluster 0xFE05
// (named "manuSpecificJxuan" in cluster_names.hpp) as an attributeReport
// carrying attribute id 1. z2m's `fz.WSZ01_on_off_action` reads
// `msg.data["1"]` and maps {0:release, 1:single, 2:double, 3:hold} into a
// single `action` string. The attribute-report path keys the unknown
// attr id by its decimal form, so the value lands in `msg.payload` under
// the key "1".
//
// The generic kFzOnOff is wrong here — the device has no controllable
// genOnOff state; wiring it would publish a phantom `state` that never
// changes. This converter emits `action`.
//
// z2m-source: converters/fromZigbee.ts `WSZ01_on_off_action`.
extern const FzConverter kFzWsz01Action;

}  // namespace zhc::devices::jxuan
