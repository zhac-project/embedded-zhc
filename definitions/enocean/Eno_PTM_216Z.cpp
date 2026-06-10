// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: EnOcean PTM 216Z — self-powered Green Power pushbutton transmitter.
// Pushbutton transmitter module (PTM 216Z).
// z2m-source: enocean.ts #PTM 216Z (fz.enocean_ptm216z).
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
// via `cluster: "greenPower"` + `type: commandNotification`. PTM 216Z is
// extra-hostile to the existing infra: its lookup key is the *compound*
// `commandID_<rawFrameByte0>` (e.g. "105_1" -> press_1, "106_0" -> release,
// "104_" -> short_press_2_of_2), so even a generic commandID->action table
// would not suffice — the decoder must reach into `msg.data.commandFrame.raw`.
// embedded-zhc has no Green Power frame family yet (`FrameFamily` is only
// {Zcl, TuyaDp}, `cluster_id_to_name` has no 0x0021, the parser emits no
// commandNotification frame, and no commandFrame.raw is surfaced). INFRA,
// out of scope for this vendor pass. Schema-only here (from_zigbee/to_zigbee
// null), exactly as the adeo/easyiot precedents express a deferred decoder.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::enocean {
namespace {
constexpr const char* kModels_PTM_216Z[] = { "GreenPower_7" };

// z2m e.action([...]) for fz.enocean_ptm216z.
constexpr const char* kActions_PTM_216Z[] = {
    "press_1", "press_2", "press_1_and_2", "press_3",
    "press_1_and_3", "press_2_and_3", "press_1_and_2_and_3",
    "press_4", "press_1_and_4", "press_2_and_4", "press_1_and_2_and_4",
    "press_3_and_4", "press_1_and_3_and_4", "press_2_and_3_and_4",
    "press_all", "press_energy_bar", "release", "short_press_2_of_2",
};

constexpr Expose kExposes_PTM_216Z[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActions_PTM_216Z, sizeof(kActions_PTM_216Z)/sizeof(kActions_PTM_216Z[0])},
};

}  // namespace

extern const PreparedDefinition kDef_PTM_216Z{
    .zigbee_models=kModels_PTM_216Z, .zigbee_models_count=sizeof(kModels_PTM_216Z)/sizeof(kModels_PTM_216Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PTM 216Z", .vendor="Enocean",
    .meta=nullptr, .exposes=kExposes_PTM_216Z, .exposes_count=sizeof(kExposes_PTM_216Z)/sizeof(kExposes_PTM_216Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::enocean
