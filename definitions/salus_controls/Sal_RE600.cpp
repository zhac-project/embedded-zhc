// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SalusControls RE600 — Zigbee router / range extender.
// z2m-source: salus_controls.ts #RE600 — fromZigbee:[], toZigbee:[],
// exposes:[]. It is a pure mains repeater with NO reportable state.
// The auto-generated port carried a phantom on/off (state) plus a
// battery/voltage bundle and bound genOnOff(0x0006)+genPowerCfg(0x0001),
// none of which the device emits. Stripped to match z2m exactly: no
// exposes, no decoders, no toZigbee, no bindings.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::salus_controls {
namespace {

constexpr const char* kModels_RE600[] = { "RE600" };

}  // namespace

extern const PreparedDefinition kDef_RE600{
    .zigbee_models=kModels_RE600, .zigbee_models_count=sizeof(kModels_RE600)/sizeof(kModels_RE600[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RE600", .vendor="SalusControls",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::salus_controls
