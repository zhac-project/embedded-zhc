// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: NoDieby ND-01 — infrasonic intrusion detector (z2m v26.77.0 parity).
// z2m-source: nodieby.ts #ND-01
//   Occupancy via IAS Zone alarm_1 (kFzIasOccupancyAlarm1) + on/off for the
//   armed state (EP1 "state") and siren (EP2 "state_siren" via endpoint_map),
//   wired to kFzOnOff / kTzOnOff.
// GAP: the five config settings (led_brightness, volume, sensitivity,
//   alarm_duration, alarm_delay) live on the vendor custom cluster 0xFC00
//   (z2m "nodiebyConfig", attrs 0x0001-0x0005). No generic converter exists for
//   that cluster, so they are exposed for schema parity but have no read/write
//   path until a nodieby-specific fz/tz for 0xFC00 is authored.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodieby {
namespace {

const FzConverter* const kFz_ND_01[] = {
    &::zhc::generic::kFzIasOccupancyAlarm1,  // ssIasZone alarm_1 -> "occupancy"
    &::zhc::generic::kFzOnOff,               // genOnOff -> "state" (armed EP1 / siren EP2)
};
const TzConverter* const kTz_ND_01[] = {
    &::zhc::generic::kTzOnOff,                // arm/disarm (EP1) + siren (EP2 via endpoint_map)
};

constexpr const char* kModels_ND_01[] = { "ND-01" };

// z2m enumLookup {low:0, medium:1, high:2}.
constexpr const char* kSensitivityValues_ND_01[] = { "low", "medium", "high" };

constexpr Expose kExposes_ND_01[] = {
    {"occupancy",      ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr,                 0},
    {"state",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,                 0},
    // Config settings on vendor cluster 0xFC00 (no converter yet — see GAP).
    {"led_brightness", ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,                 0},
    {"volume",         ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr,                 0},
    {"sensitivity",    ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kSensitivityValues_ND_01, 3},
    {"alarm_duration", ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr,                 0},
    {"alarm_delay",    ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr,                 0},
};

constexpr BindingSpec kBindings_ND_01[] = {
    {1, 0x0500},  // ssIasZone (occupancy)
    {1, 0x0006},  // genOnOff  (armed, EP1)
    {2, 0x0006},  // genOnOff  (siren, EP2)
};

// EP1 = alarm (default), EP2 = siren; siren onOff surfaces as "state_siren".
constexpr ::zhc::EndpointLabel kEndpoints_ND_01[] = { {"siren", 2} };

}  // namespace

extern const PreparedDefinition kDef_ND_01{
    .zigbee_models=kModels_ND_01, .zigbee_models_count=sizeof(kModels_ND_01)/sizeof(kModels_ND_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ND-01", .vendor="NoDieby",
    .meta=nullptr, .exposes=kExposes_ND_01, .exposes_count=sizeof(kExposes_ND_01)/sizeof(kExposes_ND_01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ND_01, .from_zigbee_count=sizeof(kFz_ND_01)/sizeof(kFz_ND_01[0]),
    .to_zigbee=kTz_ND_01, .to_zigbee_count=sizeof(kTz_ND_01)/sizeof(kTz_ND_01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ND_01, .bindings_count=sizeof(kBindings_ND_01)/sizeof(kBindings_ND_01[0]),
    .endpoint_map=kEndpoints_ND_01, .endpoint_map_count=sizeof(kEndpoints_ND_01)/sizeof(kEndpoints_ND_01[0]),
    .default_endpoint=1,
};

}  // namespace zhc::devices::nodieby
