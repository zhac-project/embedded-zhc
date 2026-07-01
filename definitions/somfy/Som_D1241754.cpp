// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Somfy 1241754 — Sonesse 28 WF roller shades (external battery).
// zigbeeModel "Sonesse 28 WF Zebra". z2m wires this as
// `m.windowCovering({controls: ["lift"]}) + m.battery()` — the external-
// battery variant of 1241755. Modelled identically to its WF-roller
// siblings (1003296 / 1241755 / 1245943): decode lift position via
// kFzCoverPosition (closuresWindowCovering attr 0x0008 -> `position`) and
// battery via kFzBattery (genPowerCfg 0x0001); encode position via
// kTzCoverPosition. The cover open/close/stop state folds into the
// position channel per the established Somfy roller family precedent
// (no separate `state` expose in the sibling defs).
// z2m-source: somfy.ts #1241754.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::somfy {
namespace {
const FzConverter* const kFz_D1241754[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCoverPosition,
};
const TzConverter* const kTz_D1241754[] = {
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_D1241754[] = { "Sonesse 28 WF Zebra" };

constexpr Expose kExposes_D1241754[] = {
    {"battery",  ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",  ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%",  nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D1241754[] = {
    {1, 0x0001},  // genPowerCfg (battery)
    {1, 0x0102},  // closuresWindowCovering (lift position)
};
}  // namespace

extern const PreparedDefinition kDef_D1241754{
    .zigbee_models=kModels_D1241754, .zigbee_models_count=sizeof(kModels_D1241754)/sizeof(kModels_D1241754[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="1241754", .vendor="Somfy",
    .meta=nullptr, .exposes=kExposes_D1241754, .exposes_count=sizeof(kExposes_D1241754)/sizeof(kExposes_D1241754[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D1241754, .from_zigbee_count=sizeof(kFz_D1241754)/sizeof(kFz_D1241754[0]),
    .to_zigbee=kTz_D1241754, .to_zigbee_count=sizeof(kTz_D1241754)/sizeof(kTz_D1241754[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D1241754, .bindings_count=sizeof(kBindings_D1241754)/sizeof(kBindings_D1241754[0]),
};

}  // namespace zhc::devices::somfy
