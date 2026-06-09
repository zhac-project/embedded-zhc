// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sengled E13-N11 — hand-maintained parity override.
//
// Flood light with motion sensor light outdoor.
// z2m-source: sengled.ts #E13-N11.
// Mapping: sengledLight() + iasZoneAlarm({zoneType: "occupancy", zoneAttributes: ["alarm_1"]})
//          -> Light bundle + per-port FZ/binding for IAS Zone (occupancy alarm only).
//
// Fix (2026-06): the 2026-04-28 rewrite renamed the expose to the semantic
// "occupancy" key but left the decoder as the generic kFzIasZone — which
// emits the bare key "alarm". So occupancy reports decoded to a key no
// expose surfaced (dead motion state). Swapped to the typed
// kFzIasMotionAlarm converter, which emits "occupancy" directly to match
// z2m's fz.ias_occupancy_alarm_1 (zoneStatus bit 0). Mirrors heiman PIR_TPV12.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sengled/_shared.hpp"

namespace zhc::devices::sengled {
namespace {

const ::zhc::FzConverter* const kFz_E13_N11[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzIasMotionAlarm,
};

constexpr const char* kModels_E13_N11[] = { "E13-N11" };

constexpr Expose kExposes_E13_N11[] = {
    { "state",      ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "occupancy",  ::zhc::ExposeType::Binary,  ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_E13_N11[] = {
    { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0500 },
};

}  // namespace

extern const PreparedDefinition kDef_E13_N11{
    .zigbee_models=kModels_E13_N11, .zigbee_models_count=sizeof(kModels_E13_N11)/sizeof(kModels_E13_N11[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E13-N11", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposes_E13_N11, .exposes_count=sizeof(kExposes_E13_N11)/sizeof(kExposes_E13_N11[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E13_N11, .from_zigbee_count=sizeof(kFz_E13_N11)/sizeof(kFz_E13_N11[0]),
    .to_zigbee=kTzSengledLight, .to_zigbee_count=kTzSengledLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E13_N11, .bindings_count=sizeof(kBindings_E13_N11)/sizeof(kBindings_E13_N11[0]),
};

}  // namespace zhc::devices::sengled
