// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Owon WSP406 smart plug with energy metering (z2m v26.115.1 window).
// z2m m.onOff({powerOnBehavior: false}) + m.electricityMeter({cluster: metering})
// + m.forcePowerSource(Mains). publishDuplicateTransaction is runtime-level.
// z2m-source: owon.ts #WSP406.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "WSP406", "WSP406-UK", "WSP406-E" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
    {1, 0x0702},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
};
}  // namespace

extern const PreparedDefinition kDef_WSP406{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WSP406", .vendor="Owon",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .power_source_override=0x01,
};

}  // namespace zhc::devices::owon
