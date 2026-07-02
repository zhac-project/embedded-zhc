// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: NoDieby ND-01 — infrasonic intrusion detector (z2m v26.77.0 parity).
// z2m-source: nodieby.ts #ND-01
//   Occupancy via IAS Zone alarm_1 (kFzIasOccupancyAlarm1) + on/off for the
//   armed state (EP1 "state") and siren (EP2 "state_siren" via endpoint_map),
//   wired to kFzOnOff / kTzOnOff.
//   The five settings live on the vendor custom cluster 0xFC00 (z2m
//   "nodiebyConfig", attrs 0x0001-0x0005, no manufacturer code, plain
//   writeAttributes). z2m marks them access ALL; ZHAC wires the WRITE path via
//   the generic tz_zcl_write_attr encoder (SET-only config — read-back deferred
//   per convention, so no fz). Defaults to EP1 (alarm) where the attrs live.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodieby {
namespace {

const FzConverter* const kFz_ND_01[] = {
    &::zhc::generic::kFzIasOccupancyAlarm1,  // ssIasZone alarm_1 -> "occupancy"
    &::zhc::generic::kFzOnOff,               // genOnOff -> "state" (armed EP1 / siren EP2)
};

// ── 0xFC00 "nodiebyConfig" write specs ──────────────────────────────
// All attributes are plain UINT8 on the wire except alarm_duration (UINT16).
// sensitivity is a UINT8 with a {low:0, medium:1, high:2} label lookup.
constexpr ::zhc::generic::ZclWriteLookup kSensitivityLookup_ND_01[] = {
    {"low", 0}, {"medium", 1}, {"high", 2},
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLedBrightness_ND_01{
    "led_brightness", 0x0001, 0x20, 0x0000, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecVolume_ND_01{
    "volume", 0x0002, 0x20, 0x0000, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecSensitivity_ND_01{
    "sensitivity", 0x0003, 0x20, 0x0000, kSensitivityLookup_ND_01,
    sizeof(kSensitivityLookup_ND_01)/sizeof(kSensitivityLookup_ND_01[0]) };
constexpr ::zhc::generic::ZclWriteSpec kSpecAlarmDuration_ND_01{
    "alarm_duration", 0x0004, 0x21, 0x0000, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecAlarmDelay_ND_01{
    "alarm_delay", 0x0005, 0x20, 0x0000, nullptr, 0 };

constexpr TzConverter kTzLedBrightness_ND_01{
    "led_brightness", "nodiebyConfig", 0xFC00, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecLedBrightness_ND_01 };
constexpr TzConverter kTzVolume_ND_01{
    "volume", "nodiebyConfig", 0xFC00, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecVolume_ND_01 };
constexpr TzConverter kTzSensitivity_ND_01{
    "sensitivity", "nodiebyConfig", 0xFC00, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecSensitivity_ND_01 };
constexpr TzConverter kTzAlarmDuration_ND_01{
    "alarm_duration", "nodiebyConfig", 0xFC00, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecAlarmDuration_ND_01 };
constexpr TzConverter kTzAlarmDelay_ND_01{
    "alarm_delay", "nodiebyConfig", 0xFC00, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecAlarmDelay_ND_01 };

const TzConverter* const kTz_ND_01[] = {
    &::zhc::generic::kTzOnOff,   // arm/disarm (EP1) + siren (EP2 via endpoint_map)
    &kTzLedBrightness_ND_01,
    &kTzVolume_ND_01,
    &kTzSensitivity_ND_01,
    &kTzAlarmDuration_ND_01,
    &kTzAlarmDelay_ND_01,
};

constexpr const char* kModels_ND_01[] = { "ND-01" };

// z2m enumLookup {low:0, medium:1, high:2}.
constexpr const char* kSensitivityValues_ND_01[] = { "low", "medium", "high" };

constexpr Expose kExposes_ND_01[] = {
    {"occupancy",      ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr,                  0},
    {"state",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr,                  0},
    // Config settings on vendor cluster 0xFC00 — write-only (SET), see header.
    {"led_brightness", ExposeType::Numeric, Access::Set, "%",     "Brightness of the status LED",                    nullptr,                  0, ExposeCategory::Config},
    {"volume",         ExposeType::Numeric, Access::Set, "%",     "Siren volume",                                    nullptr,                  0, ExposeCategory::Config},
    {"sensitivity",    ExposeType::Enum,    Access::Set, nullptr, "Intrusion detection sensitivity",                 kSensitivityValues_ND_01, 3, ExposeCategory::Config},
    {"alarm_duration", ExposeType::Numeric, Access::Set, "s",     "Siren duration once triggered",                   nullptr,                  0, ExposeCategory::Config},
    {"alarm_delay",    ExposeType::Numeric, Access::Set, "s",     "Delay before the alarm triggers after detection", nullptr,                  0, ExposeCategory::Config},
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
