// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ctm mkomfy25 — graduated from generated/ for an IAS-zone fix.
// 2.5 Stove guard.
//
// Parity fix: z2m wires `m.iasZoneAlarm({zoneType:"generic",
// manufacturerZoneAttributes:[{bit:0,name:"high_temperature"},
// {bit:1,name:"power_cut_off"}], zoneAttributes:["tamper",
// "battery_low"]})`. The auto-port used the generic `kFzIasZone`,
// which emits a bare `alarm` (bit 0) and never matched any expose,
// while the two real stove-guard signals (high_temperature / power_
// cut_off) had no decoder at all. Re-wired onto `kFzCtmStoveGuardZone`.
// z2m-source: ctm.ts #mkomfy25.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ctm/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
const FzConverter* const kFz_mkomfy25[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::ctm::kFzCtmStoveGuardZone,
};
const TzConverter* const kTz_mkomfy25[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_mkomfy25[] = { "mKomfy 2.5" };

}  // namespace


// Manual: replace the dead generic `alarm` expose with the two z2m
// stove-guard zone signals (high_temperature / power_cut_off).
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"high_temperature", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"power_cut_off", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_mkomfy25{
    .zigbee_models=kModels_mkomfy25, .zigbee_models_count=sizeof(kModels_mkomfy25)/sizeof(kModels_mkomfy25[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mkomfy25", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mkomfy25, .from_zigbee_count=sizeof(kFz_mkomfy25)/sizeof(kFz_mkomfy25[0]),
    .to_zigbee=kTz_mkomfy25, .to_zigbee_count=sizeof(kTz_mkomfy25)/sizeof(kTz_mkomfy25[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
