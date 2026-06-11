// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Cel CGW-Z-0010 — Cortet range extender (router, no exposes).
//
// CEL (California Eastern Labs / Cortet) MeshConnect "Z10" range extender.
// z2m wires it as a pure router: fromZigbee:[fz.linkquality_from_basic]
// (linkquality is stack-level metadata, NOT a device expose/state),
// toZigbee:[], exposes:[]. The auto-port mis-ported it as a controllable
// on/off switch (kFzOnOff + kTzOnOff + phantom `state` expose + a 0x0006
// OnOff bind) — a dead control: the extender never speaks genOnOff. Fixed
// to the canonical router shape: no converters, no exposes, no bindings.
//
// z2m-source: cel.ts #CGW-Z-0010.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::cel {
namespace {
constexpr const char* kModels_CGW_Z_0010[] = { "Z10" };
}  // namespace

extern const PreparedDefinition kDef_CGW_Z_0010{
    .zigbee_models=kModels_CGW_Z_0010, .zigbee_models_count=sizeof(kModels_CGW_Z_0010)/sizeof(kModels_CGW_Z_0010[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CGW-Z-0010", .vendor="Cel",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::cel
