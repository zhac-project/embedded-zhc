// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings SZ-SRN12N — hand-rewritten 2026-04-28.
// Smart siren.
// z2m-source: smartthings.ts #SZ-SRN12N.
//
// z2m bundle:
//   fz: (none)        // device has no fromZigbee converters
//   tz: tz.warning    // ssIasWd 0x0502 startWarning command
//   exposes: e.warning()
//
// Previous port had `kFzOnOff` and `kFzBattery` — z2m has neither. Removed
// them. Bind genPowerCfg (0x0001) to satisfy the configure step (z2m does
// `reporting.bind(endpoint, coordinatorEndpoint, ["genPowerCfg"])`).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {

const TzConverter* const kTz_SZ_SRN12N[] = {
    &::zhc::generic::kTzWarning,
};

constexpr const char* kModels_SZ_SRN12N[] = { "Z-SRN12N", "SZ-SRN12N" };

constexpr Expose kAutoExposes[] = {
    {"warning", ExposeType::String, Access::Set, nullptr,
        "Trigger an IAS-WD warning (mode/level/duration JSON via tz_warning)",
        nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0502},
};
}  // namespace

extern const PreparedDefinition kDef_SZ_SRN12N{
    .zigbee_models=kModels_SZ_SRN12N, .zigbee_models_count=sizeof(kModels_SZ_SRN12N)/sizeof(kModels_SZ_SRN12N[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SZ-SRN12N", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=kTz_SZ_SRN12N, .to_zigbee_count=sizeof(kTz_SZ_SRN12N)/sizeof(kTz_SZ_SRN12N[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
