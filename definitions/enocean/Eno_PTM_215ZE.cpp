// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: EnOcean PTM 215ZE — self-powered Green Power pushbutton transmitter.
// Pushbutton transmitter module (PTM 215ZE — Easyfit / Trio2sys).
// z2m-source: enocean.ts #PTM 215ZE (fz.enocean_ptm215ze).
//
// PARITY FIX (z2m<->embedded-zhc): the generated port mis-classified this
// self-powered Green Power transmitter as a *controllable* on/off relay —
// it wired generic kFzOnOff + kTzOnOff, declared a dead `state` Binary
// expose, and bound genOnOff (0x0006) to the coordinator. z2m gives this
// device `toZigbee: []` (it is NOT controllable) and a single
// `e.action([...])` expose. The phantom relay role is removed here and the
// schema now matches z2m: an `action` enum carrying the rocker events.
//
// RUNTIME BLOCKER (deferred — Green Power infra): z2m decodes the action
// via `cluster: "greenPower"` + `type: commandNotification`, mapping the
// GPDF `commandID` to the action string. embedded-zhc has no Green Power
// frame family yet: `FrameFamily` is only {Zcl, TuyaDp}, `cluster_id_to_name`
// has no 0x0021, and the parser emits no commandNotification frame. Wiring
// the decoder would require a GPDF parse layer (frame-counter dedup +
// commandID extraction) in the core parser — INFRA, out of scope for this
// vendor pass. Schema-only here (from_zigbee/to_zigbee null), exactly as the
// adeo/easyiot precedents express a deferred decoder.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::enocean {
namespace {
constexpr const char* kModels_PTM_215ZE[] = { "GreenPower_2" };

// z2m e.action([...]) for fz.enocean_ptm215ze.
constexpr const char* kActions_PTM_215ZE[] = {
    "press_1", "release_1", "press_2", "release_2",
    "press_3", "release_3", "press_4", "release_4",
    "press_1_and_2", "release_1_and_2",
    "press_1_and_3", "release_1_and_3",
    "press_1_and_4", "release_1_and_4",
    "press_2_and_3", "release_2_and_3",
    "press_2_and_4", "release_2_and_4",
    "press_3_and_4", "release_3_and_4",
    "press_energy_bar", "release_energy_bar",
    "press_or_release_all",
    "lock", "unlock", "half_open", "tilt",
};

constexpr Expose kExposes_PTM_215ZE[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActions_PTM_215ZE, sizeof(kActions_PTM_215ZE)/sizeof(kActions_PTM_215ZE[0])},
};

}  // namespace

extern const PreparedDefinition kDef_PTM_215ZE{
    .zigbee_models=kModels_PTM_215ZE, .zigbee_models_count=sizeof(kModels_PTM_215ZE)/sizeof(kModels_PTM_215ZE[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PTM 215ZE", .vendor="Enocean",
    .meta=nullptr, .exposes=kExposes_PTM_215ZE, .exposes_count=sizeof(kExposes_PTM_215ZE)/sizeof(kExposes_PTM_215ZE[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::enocean
