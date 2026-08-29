// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 929004296901 — Hue wired dimmer switch.
// New upstream in z2m v26.100.0.
// z2m-source: philips.ts #929004296901.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929004296901[] = { "LWM007" };

// z2m: `philips.m.light({configureReporting: true})` with
// `endpoint: () => ({default: 11})` — a plain dimmer (no colour, no colour
// temperature) whose single endpoint is 11, not 1. The endpoint_map carries
// one entry so binds and outbound writes reach the right endpoint; because
// it is the device's only endpoint the runtime suffixes keys with its label,
// so the label is "l1" to keep the key shape conventional.
constexpr ::zhc::EndpointLabel kEndpoints_D929004296901[] = {
    {"l1", 11},
};

constexpr Expose kExposes_D929004296901[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

const FzConverter* const kFz_D929004296901[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_D929004296901[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};

constexpr BindingSpec kBindings_D929004296901[] = {
    {11, 0x0006},   // genOnOff
    {11, 0x0008},   // genLevelCtrl
};

}  // namespace

extern const PreparedDefinition kDef_D929004296901{
    .zigbee_models           = kModels_D929004296901,
    .zigbee_models_count     = sizeof(kModels_D929004296901)/sizeof(kModels_D929004296901[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929004296901",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposes_D929004296901,
    .exposes_count           = sizeof(kExposes_D929004296901)/sizeof(kExposes_D929004296901[0]),
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFz_D929004296901,
    .from_zigbee_count       = sizeof(kFz_D929004296901)/sizeof(kFz_D929004296901[0]),
    .to_zigbee               = kTz_D929004296901,
    .to_zigbee_count         = sizeof(kTz_D929004296901)/sizeof(kTz_D929004296901[0]),
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindings_D929004296901,
    .bindings_count          = sizeof(kBindings_D929004296901)/sizeof(kBindings_D929004296901[0]),
    .endpoint_map            = kEndpoints_D929004296901,
    .endpoint_map_count      = sizeof(kEndpoints_D929004296901)/sizeof(kEndpoints_D929004296901[0]),
};

}  // namespace zhc::devices::philips
