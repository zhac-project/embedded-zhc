// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Yandex YNDX_00528 — graduated from generated/ (vendor-parity sweep).
// Motion and illuminance sensor. z2m = m.occupancy() + m.illuminance() +
// m.battery() (+ a sensitivity enum on msOccupancySensing 0xF000, manu 0x132F).
// The generated def exposed `occupancy` but registered NO occupancy decoder
// (the comment claimed no generic helper existed) — so the primary motion
// state was a dead key. The generic kFzOccupancy (msOccupancySensing 0x0000,
// z2m fz.occupancy equivalent) now exists; wire it. The 0x0406 binding is
// already declared below.
// z2m-source: yandex.ts #YNDX_00528 (m.occupancy()).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yandex {
namespace {
const FzConverter* const kFz_YNDX_00528[] = {
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_YNDX_00528[] = { "YNDX-00528" };

}  // namespace


// --- hand-authored exposes / bindings (parity sweep 2026-04-28) ---
// occupancy + illuminance + battery. The mfg `sensitivity` enum
// (msOccupancySensing attr 0xF000, manu 0x132F) is exposed read-only;
// per-cluster manu-specific tz helper not yet available — see
// docs/YANDEX_PARITY.md.
constexpr Expose kAutoExposes[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0406},  // msOccupancySensing
    {1, 0x0400},  // msIlluminanceMeasurement
    {1, 0x0001},  // genPowerCfg
};
// --- end ---

extern const PreparedDefinition kDef_YNDX_00528{
    .zigbee_models=kModels_YNDX_00528, .zigbee_models_count=sizeof(kModels_YNDX_00528)/sizeof(kModels_YNDX_00528[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00528", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00528, .from_zigbee_count=sizeof(kFz_YNDX_00528)/sizeof(kFz_YNDX_00528[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yandex
