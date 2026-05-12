// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nous A11Z — hand-rewritten from z2m.
// 3-channel power strip with total energy monitoring (TS011F, _TZ3210_6cmeijtd).
// Multi-endpoint: l1=1, l2=2, l3=3 (per-EP state + child lock).
// Energy/current/voltage/power are aggregate (multiEndpointSkip on EP1).
// z2m-source: nous.ts #A11Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nous {
namespace {
const FzConverter* const kFz_A11Z[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_A11Z[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_A11Z[] = { "TS011F" };
constexpr const char* kManus_A11Z[] = { "_TZ3210_6cmeijtd" };
constexpr ::zhc::EndpointLabel kEndpoints_A11Z[] = { {"l1", 1}, {"l2", 2}, {"l3", 3} };

constexpr Expose kExp_A11Z[] = {
    { "state_l1", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l2", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "state_l3", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "energy",   ExposeType::Numeric, ::zhc::Access::State,    "kWh", nullptr, nullptr, 0 },
    { "power",    ExposeType::Numeric, ::zhc::Access::State,    "W",   nullptr, nullptr, 0 },
    { "current",  ExposeType::Numeric, ::zhc::Access::State,    "A",   nullptr, nullptr, 0 },
    { "voltage",  ExposeType::Numeric, ::zhc::Access::State,    "V",   nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_A11Z[] = {
    { 1, 0x0006 },  // genOnOff EP1
    { 2, 0x0006 },  // genOnOff EP2
    { 3, 0x0006 },  // genOnOff EP3
    { 1, 0x0702 },  // seMetering (aggregate on EP1)
    { 1, 0x0B04 },  // haElectricalMeasurement (aggregate on EP1)
};
}  // namespace

extern const PreparedDefinition kDef_A11Z{
    .zigbee_models=kModels_A11Z, .zigbee_models_count=sizeof(kModels_A11Z)/sizeof(kModels_A11Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_A11Z, .manufacturer_names_count=sizeof(kManus_A11Z)/sizeof(kManus_A11Z[0]),
    .model="A11Z", .vendor="Nous",
    .meta=nullptr, .exposes=kExp_A11Z, .exposes_count=sizeof(kExp_A11Z)/sizeof(kExp_A11Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_A11Z, .from_zigbee_count=sizeof(kFz_A11Z)/sizeof(kFz_A11Z[0]),
    .to_zigbee=kTz_A11Z, .to_zigbee_count=sizeof(kTz_A11Z)/sizeof(kTz_A11Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_A11Z, .bindings_count=sizeof(kBind_A11Z)/sizeof(kBind_A11Z[0]),
    .endpoint_map=kEndpoints_A11Z, .endpoint_map_count=sizeof(kEndpoints_A11Z)/sizeof(kEndpoints_A11Z[0]),
};

}  // namespace zhc::devices::nous
