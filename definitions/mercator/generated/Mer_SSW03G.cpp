// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mercator SSW03G — hand-rewritten (multi-EP triple switch).
// Triple switch.
//
// z2m extend: tuya.modernExtend.tuyaOnOff({backlightModeLowMediumHigh:true,
//   endpoints:["left","center","right"]})
//   + endpoint:(d)=>({left:1,center:2,right:3})
//
// 2026-04-28p MERCATOR sweep — wired endpoint_map. The
// `backlightModeLowMediumHigh` config is a writeable enum on cluster
// 0x0008/genLevelCtrl that the generic shared runtime does not yet
// model — see runtime gap in MERCATOR_PARITY.md.
//
// z2m-source: mercator.ts #SSW03G.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mercator {
namespace {

const FzConverter* const kFz_SSW03G[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_SSW03G[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_SSW03G[] = { "TS0013" };
constexpr const char* kManus_SSW03G[] = { "_TZ3000_khtlvdfc" };

constexpr Expose kExposes_SSW03G[] = {
    { "state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_SSW03G[] = {
    { 1, 0x0006 }, { 2, 0x0006 }, { 3, 0x0006 },
};

constexpr ::zhc::EndpointLabel kEndpoints_SSW03G[] = {
    { "left",   1 },
    { "center", 2 },
    { "right",  3 },
};

}  // namespace

extern const PreparedDefinition kDef_SSW03G{
    .zigbee_models=kModels_SSW03G, .zigbee_models_count=sizeof(kModels_SSW03G)/sizeof(kModels_SSW03G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SSW03G, .manufacturer_names_count=sizeof(kManus_SSW03G)/sizeof(kManus_SSW03G[0]),
    .model="SSW03G", .vendor="Mercator",
    .meta=nullptr,
    .exposes=kExposes_SSW03G, .exposes_count=sizeof(kExposes_SSW03G)/sizeof(kExposes_SSW03G[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SSW03G, .from_zigbee_count=sizeof(kFz_SSW03G)/sizeof(kFz_SSW03G[0]),
    .to_zigbee=kTz_SSW03G, .to_zigbee_count=sizeof(kTz_SSW03G)/sizeof(kTz_SSW03G[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SSW03G, .bindings_count=sizeof(kBindings_SSW03G)/sizeof(kBindings_SSW03G[0]),
    .endpoint_map       = kEndpoints_SSW03G,
    .endpoint_map_count = sizeof(kEndpoints_SSW03G)/sizeof(kEndpoints_SSW03G[0]),
};

}  // namespace zhc::devices::mercator
