// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Schneider EKO07259 — Wiser smart thermostat, zigbeeModel "EKO07259".
// z2m-source: schneider.ts #EKO07259.
//
// GRADUATED from generated/ (hand-edited — do NOT let the generator clobber).
// z2m v26.35→v26.76 migrated this device to modernExtend `m.thermostat(...)`
// (delta: +m:thermostat). The auto-emit stub had dropped the entire climate,
// leaving a "Smart thermostat" exposing only energy/power (fz.metering). Wired
// the generic thermostat converters so local_temperature / current_heating_setpoint
// / system_mode surface (standard hvacThermostat 0x0201), mirroring sibling
// WDE011680 / CCTFR6700. Climate ships FLAT (project rule — no ExposeType::Climate).
// Deferred: Schneider custom-cluster config attrs (child_lock/keypadLockout,
// temperature_display_mode, display brightness, control type/status, heating
// emitter/output mode, local-temperature source select, floor sensor
// correction/type) + running_state (runningStateFromPower) — all ride Schneider
// manuSpecific / custom clusters with no generic converter in tree.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::schneider {
namespace {
const FzConverter* const kFz_EKO07259[] = {
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_EKO07259[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr const char* kModels_EKO07259[] = { "EKO07259" };

constexpr ::zhc::EndpointLabel kEndpoints_EKO07259[] = { {"floor", 3} };

}  // namespace


// exposes + bindings (originally emit_expose_bindings.py; hand-extended to
// match the wired thermostat converters above — mirrors WDE011680).
constexpr Expose kAutoExposes[] = {
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"local_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "C", nullptr, nullptr, 0},
    {"system_mode", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0702},
    {1, 0x0201},
};

extern const PreparedDefinition kDef_EKO07259{
    .zigbee_models=kModels_EKO07259, .zigbee_models_count=sizeof(kModels_EKO07259)/sizeof(kModels_EKO07259[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EKO07259", .vendor="Schneider",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EKO07259, .from_zigbee_count=sizeof(kFz_EKO07259)/sizeof(kFz_EKO07259[0]),
    .to_zigbee=kTz_EKO07259, .to_zigbee_count=sizeof(kTz_EKO07259)/sizeof(kTz_EKO07259[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_EKO07259,
    .endpoint_map_count = sizeof(kEndpoints_EKO07259)/sizeof(kEndpoints_EKO07259[0]),
};

}  // namespace zhc::devices::schneider
