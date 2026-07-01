// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Sonoff MINI-ZB1GS 1-gang smart switch (z2m v26.76.0 parity).
//
// z2m-source: sonoff.ts #MINI-ZB1GS. Single-endpoint on/off relay
// (genOnOff 0x0006) via fz.on_off + standard on/off write. The eWeLink
// custom cluster 0xFC11 (faultCode diagnostics) is manu-specific and
// DEFERRED — the primary switch function is fully covered here.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr const char* kModels_MINI_ZB1GS[] = { "MINI-ZB1GS" };
constexpr Expose kExposes_MINI_ZB1GS[] = {
    { "state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
const FzConverter* const kFz_MINI_ZB1GS[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_MINI_ZB1GS[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr BindingSpec kBindings_MINI_ZB1GS[] = {
    { 1, 0x0006 },   // genOnOff
};
}  // namespace
extern const PreparedDefinition kDef_MINI_ZB1GS{
    .zigbee_models=kModels_MINI_ZB1GS, .zigbee_models_count=sizeof(kModels_MINI_ZB1GS)/sizeof(kModels_MINI_ZB1GS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MINI-ZB1GS", .vendor="Sonoff",
    .meta=nullptr, .exposes=kExposes_MINI_ZB1GS, .exposes_count=sizeof(kExposes_MINI_ZB1GS)/sizeof(kExposes_MINI_ZB1GS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MINI_ZB1GS, .from_zigbee_count=sizeof(kFz_MINI_ZB1GS)/sizeof(kFz_MINI_ZB1GS[0]),
    .to_zigbee=kTz_MINI_ZB1GS, .to_zigbee_count=sizeof(kTz_MINI_ZB1GS)/sizeof(kTz_MINI_ZB1GS[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_MINI_ZB1GS, .bindings_count=sizeof(kBindings_MINI_ZB1GS)/sizeof(kBindings_MINI_ZB1GS[0]),
};
}  // namespace zhc::devices::sonoff
