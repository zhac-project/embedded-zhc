// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somfy 1800194 — Situo 1 channel blinds remote.
// zigbeeModel "Situo 1 Zigbee". z2m wires this as
// `m.battery() + m.commandsOnOff() + m.commandsWindowCovering()` — a
// battery-powered command-emitting remote (no local relay/motor), so it
// is modelled identically to the already-ported single-channel Ysia 1
// remote (1871157): wire the command-receiver Fz pipeline (on/off/toggle
// + cover open/close/stop -> `action`) plus battery, with NO to_zigbee.
// z2m-source: somfy.ts #1800194.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::somfy {
namespace {
const FzConverter* const kFz_D1800194[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D1800194[] = { "Situo 1 Zigbee" };

constexpr Expose kExposes_D1800194[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D1800194[] = {
    {1, 0x0001},  // genPowerCfg (battery)
    {1, 0x0006},  // genOnOff (command source)
    {1, 0x0102},  // closuresWindowCovering (command source)
};
}  // namespace

extern const PreparedDefinition kDef_D1800194{
    .zigbee_models=kModels_D1800194, .zigbee_models_count=sizeof(kModels_D1800194)/sizeof(kModels_D1800194[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1800194", .vendor="Somfy",
    .meta=nullptr, .exposes=kExposes_D1800194, .exposes_count=sizeof(kExposes_D1800194)/sizeof(kExposes_D1800194[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1800194, .from_zigbee_count=sizeof(kFz_D1800194)/sizeof(kFz_D1800194[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D1800194, .bindings_count=sizeof(kBindings_D1800194)/sizeof(kBindings_D1800194[0]),
};

}  // namespace zhc::devices::somfy
