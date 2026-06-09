// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ikea E1746 — TRADFRI signal repeater.
//
// z2m-source: ikea.ts #E1746 — m.identify() + m.linkQuality() only;
// exposes:[] (no controllable/sensor endpoints). It is a mains-powered
// Zigbee router with nothing to read or set.
//
// The auto-generated def wired the full kFzIkeaLight bundle
// (state/brightness/color_temp + OnOff/Level/Color bindings), which is
// a phantom light on a device that has no genOnOff/genLevelCtrl/
// genColorCtrl. Stripped to an empty definition to match z2m.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E1746[] = { "TRADFRI signal repeater" };

}  // namespace

extern const PreparedDefinition kDef_E1746{
    .zigbee_models=kModels_E1746, .zigbee_models_count=sizeof(kModels_E1746)/sizeof(kModels_E1746[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E1746", .vendor="Ikea",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::ikea
