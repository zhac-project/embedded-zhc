// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Bacchus Water_Station — generic converters only.
// 6-endpoint irrigation/water-station controller.
// z2m-source: bacchus.ts #Water_Station.
//
// Real device shape (from z2m source):
//   - EP1..EP4 — pump_1 / pump_2 / pump_3 / pump_all on genOnOff (0x0006)
//                with manu attr 0xf003 = duration (sec).
//   - EP5     — beeper on genOnOff manu attr 0xf005 (bool).
//   - EP6     — ssIasZone (0x0500) water-leak alarm with manu bit 1 = `full`
//               (tank-full indicator).
//
// Coverage in this Tier-1 port:
//   - kFzOnOff   — decodes the four pump on/off states (state attribute on
//                  0x0006). z2m's `binaryWithOnOffCommand` writes the same
//                  attribute, but the runtime here is Fz-only (Tz writes
//                  for the manu duration attr would need a vendor `_shared`
//                  helper — left as a gap).
//   - kFzIasZone — decodes the EP6 water-leak alarm (alarm_1 / tamper /
//                  battery_low). The `full` manu bit (bit 1) is also
//                  carried in zoneStatus, but the generic decoder publishes
//                  it as `alarm_2`; we list `full` in exposes for clarity
//                  even though the wire-level emission is `alarm_2`.
// Multi-endpoint dispatch is endpoint-agnostic at the Tier-1 layer — the
// adapter routes by cluster id only — so all four pumps produce a single
// `state` attribute today (a v2 multi-endpoint Fz layer is future work).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::bacchus {
namespace {

const FzConverter* const kFz_Water_Station[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzIasZone,
};

constexpr const char* kModels_Water_Station[] = { "Water_Station", "Water_Station.Modkam" };

constexpr Expose kExp_Water_Station[] = {
    // On/off pumps + beeper. State exposed once at Tier-1 — multi-endpoint
    // disambiguation lives in the v2 Fz layer.
    { "pump_1",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Enable first pump",   nullptr, 0 },
    { "pump_1_duration",   ExposeType::Numeric, ::zhc::Access::StateSet, "sec",   "Duration of pump 1",  nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "pump_2",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Enable second pump",  nullptr, 0 },
    { "pump_2_duration",   ExposeType::Numeric, ::zhc::Access::StateSet, "sec",   "Duration of pump 2",  nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "pump_3",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Enable third pump",   nullptr, 0 },
    { "pump_3_duration",   ExposeType::Numeric, ::zhc::Access::StateSet, "sec",   "Duration of pump 3",  nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "pump_all",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Enable all pumps",    nullptr, 0 },
    { "pump_all_duration", ExposeType::Numeric, ::zhc::Access::StateSet, "sec",   "Duration of pump all",nullptr, 0,
      ::zhc::ExposeCategory::Config },
    { "beeper",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Beeper",              nullptr, 0,
      ::zhc::ExposeCategory::Config },
    // ssIasZone (EP6).
    { "water_leak",        ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "tamper",            ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "battery_low",       ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "full",              ExposeType::Binary,  ::zhc::Access::State,    nullptr,
      "Indicates whether the water tank is full (manu zoneStatus bit 1)", nullptr, 0 },
};

constexpr BindingSpec kBind_Water_Station[] = {
    { 1, 0x0006 },
    { 2, 0x0006 },
    { 3, 0x0006 },
    { 4, 0x0006 },
    { 5, 0x0006 }, // beeper
    { 6, 0x0500 }, // ssIasZone
};

constexpr ::zhc::EndpointLabel kEndpoints_Water_Station[] = { {"1", 1}, {"2", 2}, {"3", 3} };

}  // namespace

extern const PreparedDefinition kDef_Water_Station{
    .zigbee_models=kModels_Water_Station, .zigbee_models_count=sizeof(kModels_Water_Station)/sizeof(kModels_Water_Station[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Water_Station", .vendor="Bacchus",
    .meta=nullptr, .exposes=kExp_Water_Station, .exposes_count=sizeof(kExp_Water_Station)/sizeof(kExp_Water_Station[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Water_Station, .from_zigbee_count=sizeof(kFz_Water_Station)/sizeof(kFz_Water_Station[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Water_Station, .bindings_count=sizeof(kBind_Water_Station)/sizeof(kBind_Water_Station[0]),
    .endpoint_map       = kEndpoints_Water_Station,
    .endpoint_map_count = sizeof(kEndpoints_Water_Station)/sizeof(kEndpoints_Water_Station[0]),
};

}  // namespace zhc::devices::bacchus
