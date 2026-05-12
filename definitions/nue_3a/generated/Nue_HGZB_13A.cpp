// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nue3a HGZB-13A — hand-rewritten 2026-04-28.
// Door / window sensor.
// z2m-source: nue_3a.ts #HGZB-13A.
//
// z2m bundle: fromZigbee=[fz.ias_contact_alarm_1];
//   exposes=[contact, battery_low, tamper]. No fz.battery — battery_low
//   is surfaced through the IAS zone status word (BATTERY bit), not via
//   the genPowerCfg attribute report. Mapped:
//   kFzIasContactAlarm — emits contact / tamper / battery_low.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nue_3a {
namespace {
const FzConverter* const kFz_HGZB_13A[] = {
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_HGZB_13A[] = { "FNB56-DOS07FB3.1" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"contact",     ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_HGZB_13A{
    .zigbee_models=kModels_HGZB_13A, .zigbee_models_count=sizeof(kModels_HGZB_13A)/sizeof(kModels_HGZB_13A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HGZB-13A", .vendor="Nue3a",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HGZB_13A, .from_zigbee_count=sizeof(kFz_HGZB_13A)/sizeof(kFz_HGZB_13A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nue_3a
