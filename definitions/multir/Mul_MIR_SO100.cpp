// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Multir MIR-SO100 — SOS button (action remote).
// z2m role: fromZigbee:[fzLocal.MIRSO100], exposes:[e.action(["single",
// "double","hold"])] + m.battery(). The button is NOT controllable — it
// reports presses via a raw ssIasZone command (first payload byte 0/1/128
// → single/double/hold). The Tier-1 auto-port mis-modelled it as a settable
// genOnOff switch: it wired kFzOnOff + kTzOnOff, exposed a settable `state`,
// and bound 0x0006 (a cluster the device never speaks) — a dead, phantom
// write path. Replaced with the vendor kFzMultirSosAction (→ `action`),
// dropped the on/off state + write path + 0x0006 binding.
// z2m-source: multir.ts #MIR-SO100 (fzLocal.MIRSO100).
#include "definitions/multir/_shared.hpp"

namespace zhc::devices::multir {
namespace {
const FzConverter* const kFz_MIR_SO100[] = {
    &kFzMultirSosAction,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_MIR_SO100[] = { "MIR-SO100" };

}  // namespace


// --- hand-rewritten (was emit_expose_bindings.py) ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end block ---

extern const PreparedDefinition kDef_MIR_SO100{
    .zigbee_models=kModels_MIR_SO100, .zigbee_models_count=sizeof(kModels_MIR_SO100)/sizeof(kModels_MIR_SO100[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MIR-SO100", .vendor="Multir",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MIR_SO100, .from_zigbee_count=sizeof(kFz_MIR_SO100)/sizeof(kFz_MIR_SO100[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::multir
