// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bosch BSEN-M — Motion detector (RFPR-ZB-SH-EU).
// Wires the bosch/_shared IAS-zone battery sensor bundle
// (kFzBattery + kFzIasZone + battery/voltage/battery_low/alarm/tamper
// exposes).
//
// Gaps vs z2m: sensitivity_level / test_mode / illuminance / temperature
// (boschBsenExtend.* with custom 0xFCAB / 0xFCAD style cluster, mfgcode
// 0x1209) are not yet wired — see docs/BOSCH_PARITY.md.
// z2m-source: bosch.ts #BSEN-M.
#include "definitions/_generic/_shared.hpp"
#include "definitions/bosch/_shared.hpp"

namespace zhc::devices::bosch {
namespace {
constexpr const char* kModels_BSEN_M[] = { "RFPR-ZB-SH-EU" };
}  // namespace

extern const PreparedDefinition kDef_BSEN_M{
    .zigbee_models           = kModels_BSEN_M,
    .zigbee_models_count     = sizeof(kModels_BSEN_M)/sizeof(kModels_BSEN_M[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "BSEN-M",
    .vendor                  = "Bosch",
    .meta                    = nullptr,
    .exposes                 = kExposesBoschIasBattery,
    .exposes_count           = kExposesBoschIasBatteryCount,
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFzBoschIasBattery,
    .from_zigbee_count       = kFzBoschIasBatteryCount,
    .to_zigbee               = nullptr,
    .to_zigbee_count         = 0,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsBoschIasBattery,
    .bindings_count          = kBindingsBoschIasBatteryCount,
};

}  // namespace zhc::devices::bosch
