// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// PushOk Hardware shared converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/pushok.ts
//             (POK003 / POK010 inline `m.binary({name:"contact", ...})`).
#pragma once

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::pushok {

// `kFzPushokContact` — genBinaryInput (0x000F) presentValue → "contact".
//
// POK003 (water level) and POK010 (water level + RH) expose `contact`
// via z2m's `m.binary({name:"contact", cluster:"genBinaryInput",
// attribute:"presentValue", valueOn:["ON",0x01], valueOff:["OFF",0x00]})`.
// Neither generated def wired a decoder for it, so the headline contact
// channel silently dropped. There is no generic genBinaryInput converter
// (mirrors the vendor-local `kFzNodonDryContact`), so it lives here.
extern const FzConverter kFzPushokContact;

}  // namespace zhc::devices::pushok
