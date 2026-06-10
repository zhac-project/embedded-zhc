// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Wally U02I007C.01 — hand-maintained parity override.
// WallyHome multi-sensor (zigbeeModel "MultiSensor").
//
// Graduated from generated/Wal_U02I007C_01.cpp, which degraded the device
// to an on/off relay + battery + dead bare-alarm IAS:
//   * kFzOnOff + kTzOnOff + `state` — z2m has NO toZigbee and never decodes
//     genOnOff *attributes*; the device is a SENDER of genOnOff On/Off
//     *commands* (fz.command_on / fz.command_off → action "on"/"off"). The
//     phantom controllable `state` is removed; wired kFzCommandOn/Off + an
//     `action` enum expose instead.
//   * temperature + humidity channels were DROPPED entirely — z2m decodes
//     fz.temperature (msTemperatureMeasurement) + fz.humidity
//     (msRelativeHumidity). Wired the generic kFzTemperature / kFzHumidity.
//   * generic kFzIasZone emitted a bare `alarm` behind a dead `alarm` expose.
//     z2m splits the IAS into TWO per-endpoint custom converters over
//     ssIasZone status_change (zoneStatus bit 0):
//       endpoint 1 → contact    = !(bit0)   (fz.U02I007C01_contact)
//       endpoint 2 → water_leak =  (bit0)   (fz.U02I007C01_water_leak)
//     Wired the endpoint-pinned kFzContactEp1 / kFzWaterLeakEp2 (see
//     _shared.cpp) so each zone routes to exactly one semantic key — the
//     generic wildcard-endpoint typed converters would cross-fire both keys
//     on every report.
//
// z2m-source: wally.ts #U02I007C.01.
#include "definitions/_generic/_shared.hpp"
#include "_shared.hpp"

namespace zhc::devices::wally {
namespace {
const FzConverter* const kFz_U02I007C_01[] = {
    &::zhc::generic::kFzCommandOn,    // genOnOff cmd On  → action "on"
    &::zhc::generic::kFzCommandOff,   // genOnOff cmd Off → action "off"
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,  // msTemperatureMeasurement
    &::zhc::generic::kFzHumidity,     // msRelativeHumidity
    &kFzContactEp1,                   // ssIasZone ep1 → contact
    &kFzWaterLeakEp2,                 // ssIasZone ep2 → water_leak
};
constexpr const char* kModels_U02I007C_01[] = { "MultiSensor" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff (command source)
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0500},  // ssIasZone — contact zone (endpoint 1)
    {2, 0x0500},  // ssIasZone — water-leak zone (endpoint 2)
};

extern const PreparedDefinition kDef_U02I007C_01{
    .zigbee_models=kModels_U02I007C_01, .zigbee_models_count=sizeof(kModels_U02I007C_01)/sizeof(kModels_U02I007C_01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="U02I007C.01", .vendor="Wally",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_U02I007C_01, .from_zigbee_count=sizeof(kFz_U02I007C_01)/sizeof(kFz_U02I007C_01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::wally
