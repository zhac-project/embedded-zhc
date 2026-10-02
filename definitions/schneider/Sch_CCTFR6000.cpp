// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Schneider Electric CCTFR6000 Wiser underfloor heating controller,
// graduated from the generated on/off copy. z2m v26.109.0 (#13162) adds, per
// channel (endpoints 1-8), demand_percentage (0-100 %) and cycle_time
// (300-3200 s) on the Schneider cycle-time cluster 0xFF16 (manufacturer code
// 0x105E), and the white labels CCTFR6600 / CCTFR6610. The endpoint map
// suffixes the keys per channel and routes the writes.
// z2m-source: schneider_electric.ts #CCTFR6000.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::schneider {
namespace {
constexpr std::uint16_t kSchneiderManu = 0x105E;
constexpr ::zhc::generic::ZclAttrRow kCycleRows[] = { { 0x0000, "demand_percentage" }, { 0x0010, "cycle_time" } };
constexpr ::zhc::generic::ZclAttrMap kCycleMap{ kCycleRows, 2, kSchneiderManu };
constexpr FzConverter kFzCycle = ::zhc::generic::zcl_attr_fz("schneiderCycleTime", &kCycleMap);
constexpr ::zhc::generic::ZclWriteSpec kDemandSpec{ "demand_percentage", 0x0000, 0x20, kSchneiderManu, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kCycleSpec{ "cycle_time", 0x0010, 0x21, kSchneiderManu, nullptr, 0 };
constexpr TzConverter kTzDemand = ::zhc::generic::zcl_write_tz("schneiderCycleTime", 0xFF16, &kDemandSpec);
constexpr TzConverter kTzCycle = ::zhc::generic::zcl_write_tz("schneiderCycleTime", 0xFF16, &kCycleSpec);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &kFzCycle,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &kTzDemand,
    &kTzCycle,
};
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"demand_percentage", ExposeType::Numeric, Access::StateSet, "%", "Heating demand as share of the cycle time; the relay only closes while the channel is ON and the demand is above 0", nullptr, 0, ExposeCategory::State, 0, 100, 1},
    {"cycle_time", ExposeType::Numeric, Access::StateSet, "s", "Length of the time-proportional switching cycle", nullptr, 0, ExposeCategory::Config, 300, 3200, 1},
};
constexpr const char* kM[] = { "UFH" };
constexpr WhiteLabel kWL[] = { {"Schneider Electric", "CCTFR6600"}, {"Schneider Electric", "CCTFR6610"} };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
};
constexpr EndpointLabel kEp[] = { {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8} };
}  // namespace

extern const PreparedDefinition kDefSchneider_CCTFR6000{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CCTFR6000", .vendor="Schneider",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL, .white_labels_count=sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .endpoint_map=kEp, .endpoint_map_count=sizeof(kEp)/sizeof(kEp[0]),
};

}  // namespace zhc::devices::schneider
