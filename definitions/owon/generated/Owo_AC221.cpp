// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon AC221 — auto-generated.
// AC controller / IR blaster
// z2m-source: owon.ts #AC221.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz_AC221[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_AC221[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_AC221[] = { "AC221" };

}  // namespace


// z2m: same as AC201 plus manuSpecificOwonAc cluster (0xffac) for one-key pairing,
// writePairingCode, readPairingCode commands. Manufacturer-specific cluster handlers
// (mfgcode OWON_TECHNOLOGY_INC) are runtime-blocked: no ZHC infra to register them.
// TODO(zhc): wire manuSpec OWON cluster 0xffac when vendor command framework lands.
constexpr Expose kAutoExposes[] = {
    {"local_temperature",        ExposeType::Numeric, Access::State,    "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"current_cooling_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"fan_mode",                 ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ac_louver_position",       ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0201},  // hvacThermostat
    {1, 0x0202},  // hvacFanCtrl
};

extern const PreparedDefinition kDef_AC221{
    .zigbee_models=kModels_AC221, .zigbee_models_count=sizeof(kModels_AC221)/sizeof(kModels_AC221[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AC221", .vendor="Owon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AC221, .from_zigbee_count=sizeof(kFz_AC221)/sizeof(kFz_AC221[0]),
    .to_zigbee=kTz_AC221, .to_zigbee_count=sizeof(kTz_AC221)/sizeof(kTz_AC221[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::owon
