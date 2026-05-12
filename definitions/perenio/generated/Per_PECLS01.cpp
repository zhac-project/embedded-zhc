// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Perenio PECLS01 — hand-rewritten (vendor parity sweep).
// Flood alarm device.
// z2m-source: perenio.ts #PECLS01.
//
// Wire-level: ssIasZone status-change → "water_leak" + "tamper"
// + "battery_low" via the typed IAS converter; battery via genPowerCfg.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::perenio {
namespace {
const FzConverter* const kFz_PECLS01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_PECLS01[] = { "PECLS01" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"water_leak",  ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper",      ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0500},  // ssIasZone
};

extern const PreparedDefinition kDef_PECLS01{
    .zigbee_models=kModels_PECLS01, .zigbee_models_count=sizeof(kModels_PECLS01)/sizeof(kModels_PECLS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PECLS01", .vendor="Perenio",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PECLS01, .from_zigbee_count=sizeof(kFz_PECLS01)/sizeof(kFz_PECLS01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::perenio
