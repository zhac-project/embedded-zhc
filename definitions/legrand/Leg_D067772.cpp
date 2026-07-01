// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Legrand 067772 — double wired switch with neutral (graduated).
// z2m v26.35->v26.76 reclassified this from a dimmable light (m.light) to a
// two-endpoint on/off switch (m.onOff, left=2 / right=1). Dropped the stale
// `brightness` expose + brightness converters; surface led_in_dark / led_if_on
// via the shared FC01 converters (parity with sibling 281506).
// z2m-source: legrand.ts #067772 —
//   fromZigbee: [fz.identify, fzLegrand.binary_input_on_off, fzLegrand.cluster_fc01]
//   toZigbee:   [tzLegrand.identify, tzLegrand.led_mode]
//   exposes:    state (left/right), led_in_dark, led_if_on
#include "definitions/_generic/_shared.hpp"
#include "definitions/legrand/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
const FzConverter* const kFz_D067772[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::legrand::kFzClusterFc01,
};
const TzConverter* const kTz_D067772[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::legrand::kTzLedInDark,
    &::zhc::legrand::kTzLedIfOn,
};
constexpr const char* kModels_D067772[] = { " NLIS - Double light switch" };

constexpr ::zhc::EndpointLabel kEndpoints_D067772[] = { {"right", 1}, {"left", 2} };

constexpr Expose kAutoExposes[] = {
    {"state",       ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_in_dark", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_if_on",   ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {1, 0xFC01},
};

}  // namespace

extern const PreparedDefinition kDef_D067772{
    .zigbee_models=kModels_D067772, .zigbee_models_count=sizeof(kModels_D067772)/sizeof(kModels_D067772[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="067772", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D067772, .from_zigbee_count=sizeof(kFz_D067772)/sizeof(kFz_D067772[0]),
    .to_zigbee=kTz_D067772, .to_zigbee_count=sizeof(kTz_D067772)/sizeof(kTz_D067772[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_D067772,
    .endpoint_map_count = sizeof(kEndpoints_D067772)/sizeof(kEndpoints_D067772[0]),
};

}  // namespace zhc::devices::legrand
