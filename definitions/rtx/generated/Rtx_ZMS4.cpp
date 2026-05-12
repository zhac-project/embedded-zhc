// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: RTX ZMS4 zigbee PIR sensor.
// z2m-source: rtx.ts #ZMS4 (fz.ias_occupancy_alarm_1).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::rtx {
namespace {
const FzConverter* const kFz_ZMS4[] = {
    // z2m fz.ias_occupancy_alarm_1: ssIasZone status-change notification
    // → emits occupancy (bit 0), tamper (bit 2), battery_low (bit 3).
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_ZMS4[] = { "TS0202" };
constexpr const char* kManus_ZMS4[] = { "_TZ3000_mwd3c2at" };
}  // namespace


// --- hand-authored exposes (z2m presets list) ---
constexpr Expose kAutoExposes[] = {
    {"occupancy",   ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_ZMS4{
    .zigbee_models=kModels_ZMS4, .zigbee_models_count=sizeof(kModels_ZMS4)/sizeof(kModels_ZMS4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZMS4, .manufacturer_names_count=sizeof(kManus_ZMS4)/sizeof(kManus_ZMS4[0]),
    .model="ZMS4", .vendor="RTX",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZMS4, .from_zigbee_count=sizeof(kFz_ZMS4)/sizeof(kFz_ZMS4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::rtx
