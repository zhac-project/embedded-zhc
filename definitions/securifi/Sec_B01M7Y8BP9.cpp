// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Securifi B01M7Y8BP9 — graduated from generated/ for a
// wrong-class / dead-state fix.
// Almond Click multi-function button.
//
// Parity fix: z2m wires `fromZigbee:[fz.almond_click]`,
// `exposes:[e.action(["single","double","long"])]`, and NO toZigbee.
// The auto-port mis-ported this button as a controllable on/off switch
// (kFzOnOff + kTzOnOff + a bare `state` expose) — a dead control: the
// device never speaks genOnOff, it reports clicks on ssIasAce
// `commandArm`. Re-wired onto the vendor `kFzAlmondClick` decoder
// (armmode 3->single, 0->double, 2->long) and an `action` expose;
// dropped the phantom on/off state + writeable path.
// z2m-source: securifi.ts #B01M7Y8BP9.
#include "definitions/_generic/_shared.hpp"
#include "definitions/securifi/_shared.hpp"

namespace zhc::devices::securifi {
namespace {
const FzConverter* const kFz_B01M7Y8BP9[] = {
    &::zhc::securifi::kFzAlmondClick,
};
constexpr const char* kModels_B01M7Y8BP9[] = { "ZB2-BU01" };

}  // namespace


// Button: surface clicks as `action` (single/double/long). No on/off
// state, no writeable path — matches z2m exposes:[e.action(...)] /
// toZigbee:[].
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

extern const PreparedDefinition kDef_B01M7Y8BP9{
    .zigbee_models=kModels_B01M7Y8BP9, .zigbee_models_count=sizeof(kModels_B01M7Y8BP9)/sizeof(kModels_B01M7Y8BP9[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="B01M7Y8BP9", .vendor="Securifi",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_B01M7Y8BP9, .from_zigbee_count=sizeof(kFz_B01M7Y8BP9)/sizeof(kFz_B01M7Y8BP9[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=nullptr,.bindings_count=0,
};

}  // namespace zhc::devices::securifi
