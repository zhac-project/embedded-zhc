// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Bticino K4003C/L4003C/N4003C/NT4003C — restore button action via genBinaryInput.
//
// Light switch with neutral. z2m decodes the physical button on
// genBinaryInput (0x000F) presentValue via fz.K4003C_binary_input,
// emitting BOTH action ("on"/"off") and the relay state. The generated
// port wired only kFzOnOff (genOnOff 0x0006 → state), so the
// e.action(["identify","on","off"]) surface was dead and the 0x000F
// channel was unbound/undecoded. Added the shared kFzK4003CBinaryInput
// converter + the `action` expose + the 0x000F binding to match z2m.
//
// (LED-in-dark / led-if-on / identify config = Legrand manuSpecific
// cluster 0xFC01 — INFRA, deferred; not a decode gap.)
//
// z2m-source: bticino.ts #K4003C/L4003C/N4003C/NT4003C —
//   fromZigbee: [fz.identify, fz.K4003C_binary_input, fzLegrand.cluster_fc01]
//   exposes: [e.switch(), e.action(["identify","on","off"]), ...]
#include "definitions/_generic/_shared.hpp"
#include "definitions/bticino/_shared.hpp"

namespace zhc::devices::bticino {
namespace {
const FzConverter* const kFz_K4003C_L4003C_N4003C_NT4003C[] = {
    &::zhc::generic::kFzOnOff,
    &kFzK4003CBinaryInput,
};
const TzConverter* const kTz_K4003C_L4003C_N4003C_NT4003C[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_K4003C_L4003C_N4003C_NT4003C[] = { " Light switch with neutral" };

}  // namespace


// --- exposes/bindings: state + action via genBinaryInput ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x000F},
};
// --- end block ---

extern const PreparedDefinition kDef_K4003C_L4003C_N4003C_NT4003C{
    .zigbee_models=kModels_K4003C_L4003C_N4003C_NT4003C, .zigbee_models_count=sizeof(kModels_K4003C_L4003C_N4003C_NT4003C)/sizeof(kModels_K4003C_L4003C_N4003C_NT4003C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="K4003C/L4003C/N4003C/NT4003C", .vendor="Bticino",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_K4003C_L4003C_N4003C_NT4003C, .from_zigbee_count=sizeof(kFz_K4003C_L4003C_N4003C_NT4003C)/sizeof(kFz_K4003C_L4003C_N4003C_NT4003C[0]),
    .to_zigbee=kTz_K4003C_L4003C_N4003C_NT4003C, .to_zigbee_count=sizeof(kTz_K4003C_L4003C_N4003C_NT4003C)/sizeof(kTz_K4003C_L4003C_N4003C_NT4003C[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::bticino
