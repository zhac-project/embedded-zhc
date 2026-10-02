// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: SONOFF SNZT-03P smart motion sensor (z2m v26.115.1 window).
// z2m: occupancy + illuminance (no reporting), battery %, the PIR hold
// time msOccupancySensing 0x0010 (u16 s; this firmware may report it
// under 0x3C00 too, z2m accepts both) and the eWeLink 0xFC11 attribute
// 0x2018 illumination_compensation_offset (int16 lx).
// z2m-source: sonoff.ts #SNZT-03P.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr ::zhc::generic::ZclAttrRow kOccRows[] = {
    { 0x0010, "pir_occupied_to_unoccupied_delay" },
    { 0x3C00, "pir_occupied_to_unoccupied_delay" },
};
constexpr ::zhc::generic::ZclAttrMap kOccMap{ kOccRows, 2 };
constexpr ::zhc::generic::ZclAttrRow kEweRows[] = { { 0x2018, "illumination_compensation_offset" } };
constexpr ::zhc::generic::ZclAttrMap kEweMap{ kEweRows, 1 };
constexpr FzConverter kFzPirDelay = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kOccMap);
constexpr FzConverter kFzIllumOffset = ::zhc::generic::zcl_attr_fz("manuSpecificWoolley", &kEweMap);   // 0xFC11
constexpr ::zhc::generic::ZclWriteSpec kDelaySpec{ "pir_occupied_to_unoccupied_delay", 0x0010, 0x21, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kOffsetSpec{ "illumination_compensation_offset", 0x2018, 0x29, 0, nullptr, 0 };
constexpr TzConverter kTzPirDelay = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kDelaySpec);
constexpr TzConverter kTzIllumOffset = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kOffsetSpec);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzIlluminance,
    &::zhc::generic::kFzBattery,
    &kFzPirDelay,
    &kFzIllumOffset,
};
const TzConverter* const kTz[] = {
    &kTzPirDelay,
    &kTzIllumOffset,
};
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"pir_occupied_to_unoccupied_delay", ExposeType::Numeric, Access::StateSet, "s", "Detection Duration", nullptr, 0, ExposeCategory::Config, 5, 60, 1},
    {"illumination_compensation_offset", ExposeType::Numeric, Access::StateSet, "lx", "Light intensity calibration offset", nullptr, 0, ExposeCategory::Config, -1000, 1000, 1},
};
constexpr const char* kM[] = { "SNZT-03P" };
constexpr BindingSpec kBind[] = {
    {1, 0x0001},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 10, 0},
};
}  // namespace

extern const PreparedDefinition kDef_SNZT_03P{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNZT-03P", .vendor="SONOFF",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::sonoff
