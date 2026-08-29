// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 929004296701 (z2m v26.99.0 parity).
// Hue wired on/off switch (1 channel)
// z2m-source: philips.ts #929004296701.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels_D929004296701[] = { "LOM013" };

// z2m: `m.deviceEndpoints({endpoints: {l1: 11}}) +
// philips.m.onOff({endpointNames: ["l1"]})`. One relay, but it lives on
// endpoint 11 rather than 1, so the endpoint_map is required even for a
// single gang — without it the bind and the outbound write both target
// the wrong endpoint.
constexpr ::zhc::EndpointLabel kEndpoints_D929004296701[] = {
    {"l1", 11},
};

constexpr Expose kExposes_D929004296701[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

const FzConverter* const kFz_D929004296701[] = { &::zhc::generic::kFzOnOff };
const TzConverter* const kTz_D929004296701[] = { &::zhc::generic::kTzOnOff };

constexpr BindingSpec kBindings_D929004296701[] = {
    {11, 0x0006},   // genOnOff
};

}  // namespace

extern const PreparedDefinition kDef_D929004296701{
    .zigbee_models           = kModels_D929004296701,
    .zigbee_models_count     = sizeof(kModels_D929004296701)/sizeof(kModels_D929004296701[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929004296701",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposes_D929004296701,
    .exposes_count           = sizeof(kExposes_D929004296701)/sizeof(kExposes_D929004296701[0]),
    .white_labels            = nullptr,
    .white_labels_count      = 0,
    .from_zigbee             = kFz_D929004296701,
    .from_zigbee_count       = sizeof(kFz_D929004296701)/sizeof(kFz_D929004296701[0]),
    .to_zigbee               = kTz_D929004296701,
    .to_zigbee_count         = sizeof(kTz_D929004296701)/sizeof(kTz_D929004296701[0]),
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindings_D929004296701,
    .bindings_count          = sizeof(kBindings_D929004296701)/sizeof(kBindings_D929004296701[0]),
    .endpoint_map            = kEndpoints_D929004296701,
    .endpoint_map_count      = sizeof(kEndpoints_D929004296701)/sizeof(kEndpoints_D929004296701[0]),
};

}  // namespace zhc::devices::philips
