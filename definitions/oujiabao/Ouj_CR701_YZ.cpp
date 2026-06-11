// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Oujiabao CR701-YZ — IAS dead-key fix (graduated from generated/).
// Gas and carbon monoxide alarm.
//
// z2m wires `fz.ias_carbon_monoxide_alarm_1_gas_alarm_2`, which decodes a
// single ssIasZone commandStatusChangeNotification into FOUR keys:
//   carbon_monoxide = zoneStatus bit 0   (alarm_1)
//   gas             = zoneStatus bit 1   (alarm_2)
//   tamper          = zoneStatus bit 2
//   battery_low     = zoneStatus bit 3
// The auto-port lowered the generic kFzIasZone (bare "alarm" key) plus a
// phantom "alarm" expose, so neither carbon_monoxide nor gas ever reached
// the shadow. Wire BOTH typed converters: kFzIasCoAlarm (bit 0 →
// carbon_monoxide) + kFzIasGasAlarm2 (bit 1 → gas). Both match the same
// command frame; the dispatch loop merges their payloads (each also writes
// tamper/battery_low idempotently from the shared status word).
// z2m-source: oujiabao.ts #CR701-YZ;
//   converters/fromZigbee.ts fz.ias_carbon_monoxide_alarm_1_gas_alarm_2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::oujiabao {
namespace {
const FzConverter* const kFz_CR701_YZ[] = {
    &::zhc::generic::kFzIasCoAlarm,    // bit 0 → carbon_monoxide (+tamper,battery_low)
    &::zhc::generic::kFzIasGasAlarm2,  // bit 1 → gas
};

constexpr const char* kModels_CR701_YZ[] = { "OJB-CR701-YZ" };

}  // namespace


// Exposes aligned to z2m: e.gas(), e.carbon_monoxide(), e.tamper(),
// e.battery_low(). The phantom bare "alarm" is replaced by the two
// semantic binaries the typed converters actually emit.
constexpr Expose kAutoExposes[] = {
    {"gas", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"carbon_monoxide", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_CR701_YZ{
    .zigbee_models=kModels_CR701_YZ, .zigbee_models_count=sizeof(kModels_CR701_YZ)/sizeof(kModels_CR701_YZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CR701-YZ", .vendor="Oujiabao",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CR701_YZ, .from_zigbee_count=sizeof(kFz_CR701_YZ)/sizeof(kFz_CR701_YZ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::oujiabao
