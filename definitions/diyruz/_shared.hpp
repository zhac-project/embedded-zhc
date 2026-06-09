// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// DIYRuZ shared converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/diyruz.ts
#pragma once

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::diyruz {

// `kFzDiyruzContact` — genOnOff (0x0006) onOff → "contact".
//
// DIYRuZ_magnet exposes `contact` via z2m's `fz.diyruz_contact`, which
// reads the standard genOnOff `onOff` attribute (0x0000) and returns
// `{contact: msg.data.onOff !== 0}`. This is NOT an IAS Zone device and
// NOT a manufacturer attribute — it is the plain onOff attr reused as a
// contact signal. The generic `kFzOnOff` decodes the same attr but emits
// `state` (an on/off switch semantic), so it cannot be reused here without
// publishing the wrong key. There is no generic onOff→contact converter
// (the closest analogue is the vendor-local `kFzPushokContact`, which
// reads genBinaryInput), so it lives here.
extern const FzConverter kFzDiyruzContact;

}  // namespace zhc::devices::diyruz
