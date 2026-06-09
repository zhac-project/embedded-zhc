// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bosch BSD-2 — Smoke alarm II (RBSH-SD-ZB-EU).
// Wires the bosch/_shared typed-smoke bundle: genPowerCfg battery +
// ssIasZone decoded to the semantic `smoke` key (zoneStatus bit 0),
// matching z2m's `smokeAlarmAndButtonPushes`. The generic IAS bundle it
// used to share emitted a bare `alarm` the SPA can't map; switched to
// kFzBoschSmoke.
//
// Gaps vs z2m: button_pushes (smoke alarm cluster custom attrs 0x8F01/
// 0x8F06 mfgcode 0x1209), alarm_control / broadcast_alarm command
// (cluster ssIasZone cmd 0x80 mfgcode 0x1209), test_mode and the
// enforced default sensitivity level write are not yet wired —
// see docs/BOSCH_PARITY.md.
// z2m-source: bosch.ts #BSD-2.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {
constexpr const char* kModels_BSD_2[] = { "RBSH-SD-ZB-EU" };
}  // namespace

extern const PreparedDefinition kDef_BSD_2{
    .zigbee_models           = kModels_BSD_2,
    .zigbee_models_count     = sizeof(kModels_BSD_2)/sizeof(kModels_BSD_2[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "BSD-2",
    .vendor                  = "Bosch",
    .meta                    = nullptr,
    .exposes                 = kExposesBoschSmoke,
    .exposes_count           = kExposesBoschSmokeCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzBoschSmoke,
    .from_zigbee_count       = kFzBoschSmokeCount,
    .to_zigbee               = nullptr,
    .to_zigbee_count         = 0,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsBoschIasBattery,
    .bindings_count          = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
