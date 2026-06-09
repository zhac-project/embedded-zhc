// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Adeo LDSENK10 — hand-maintained parity override.
// ENKI LEXMAN motion sensor
//
// Graduated from generated/Ade_LDSENK10.cpp: the generated def lowered the
// generic kFzIasZone (which emits the bare key `alarm`) behind an `alarm`
// expose, while z2m decodes this PIR via fz.ias_occupancy_alarm_1 (zoneStatus
// bit 0 -> `occupancy`) and exposes occupancy/battery_low/tamper. With no
// rename layer the motion flag never reached the shadow. Wired the typed
// kFzIasMotionAlarm (emits `occupancy` + tamper + battery_low) and renamed the
// expose to `occupancy`. Mirrors the heiman / sunricher PIR ports.
// z2m-source: adeo.ts #LDSENK10.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_LDSENK10[] = {
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_LDSENK10[] = { "LDSENK10" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_LDSENK10{
    .zigbee_models=kModels_LDSENK10, .zigbee_models_count=sizeof(kModels_LDSENK10)/sizeof(kModels_LDSENK10[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LDSENK10", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LDSENK10, .from_zigbee_count=sizeof(kFz_LDSENK10)/sizeof(kFz_LDSENK10[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
