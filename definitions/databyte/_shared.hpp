// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Databyte (databyte.ch) vendor converters.
//
// Databyte hangs custom manufacturer-specific attributes off the
// standard genOnOff cluster (0x0006) on endpoint 1 — there is no
// proprietary cluster, so the standard ZCL attribute-report parser
// keys each value by its decimal attribute id and these converters
// read them with `payload.find("<decimal id>")` (same mechanism the
// generic genOnOff power-on-behavior converter uses for 0x4003).
//
//   DTB190502A1 (CC2530 IO board), z2m fz.DTB190502A1:
//     attr 41361 (0xA191) → cpu_temperature  (numeric, °C)
//     attr 41362 (0xA192) → key_state         (enum KEY_SYS/UP/DOWN/NONE)
//     attr 41363 (0xA193) → led_state         (binary ON/OFF)
//
//   Touch4 / DTB-ED2011-014, z2m fzLocal.DTB2011014:
//     attr 41361..41364 (0xA191..0xA194) → key_1..key_4 (binary ON/OFF)
//
// z2m-source: zigbee-herdsman-converters/src/devices/databyte.ts
//             (fz.DTB190502A1 lives in converters/fromZigbee.ts).

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::databyte {

using ::zhc::DecodedMessage;
using ::zhc::FzConverter;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::FixedPayload;

// DTB190502A1 IO board — cpu_temperature + key_state + led_state.
extern const FzConverter kFzDtb190502a1;

// Touch4 4-key wall touchsensor — key_1..key_4.
extern const FzConverter kFzTouch4Keys;

}  // namespace zhc::databyte
