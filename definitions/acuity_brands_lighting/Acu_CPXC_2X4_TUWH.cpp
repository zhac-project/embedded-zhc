// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Acuity Brands Lighting CPXC 2X4 TUWH (zigbeeModel
// ABL-LIGHTSENSOR-Z-001), Lithonia CPX Connect 2x4 tunable-white troffer with
// a motion sensor, new in z2m v26.126.0..v26.133.0.
// m.light({colorTemp: {range: [200, 370], startup: false}, effect: false}):
// state, brightness, color_temp 200-370 mired, power_on_behavior (the m.light
// default). m.occupancy({reporting: false, pirConfig: [otu_delay, uto_delay,
// uto_threshold]}): occupancy plus msOccupancySensing 0x0010
// occupancy_timeout (u16 s), 0x0011 pir_uto_delay (u16), 0x0012
// pir_uto_threshold (u8), all writable. No occupancy binding or reporting:
// the cluster rejects configureReporting and reports on its own (upstream).
// Not ported: identify, the configure-time read of the three pir attributes.
// z2m-source: acuity_brands_lighting.ts #CPXC 2X4 TUWH.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::acuity_brands_lighting {
namespace {

constexpr ::zhc::generic::ZclAttrRow kPirRows[] = {
    { 0x0010, "occupancy_timeout" },
    { 0x0011, "pir_uto_delay" },
    { 0x0012, "pir_uto_threshold" },
};
constexpr ::zhc::generic::ZclAttrMap kPirMap{ kPirRows, sizeof(kPirRows)/sizeof(kPirRows[0]) };
constexpr FzConverter kFzPir = ::zhc::generic::zcl_attr_fz("msOccupancySensing", &kPirMap);

constexpr ::zhc::generic::ZclWriteSpec kOtuDelaySpec{ "occupancy_timeout", 0x0010, 0x21, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kUtoDelaySpec{ "pir_uto_delay", 0x0011, 0x21, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kUtoThresholdSpec{ "pir_uto_threshold", 0x0012, 0x20, 0, nullptr, 0 };
constexpr TzConverter kTzOtuDelay = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kOtuDelaySpec);
constexpr TzConverter kTzUtoDelay = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kUtoDelaySpec);
constexpr TzConverter kTzUtoThreshold = ::zhc::generic::zcl_write_tz("msOccupancySensing", 0x0406, &kUtoThresholdSpec);

const FzConverter* const kFz_CPXC_2X4_TUWH[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzOccupancy,
    &kFzPir,
};
const TzConverter* const kTz_CPXC_2X4_TUWH[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzPowerOnBehavior1,
    &kTzOtuDelay,
    &kTzUtoDelay,
    &kTzUtoThreshold,
};

constexpr const char* kModels_CPXC_2X4_TUWH[] = { "ABL-LIGHTSENSOR-Z-001" };

constexpr Expose kExposes_CPXC_2X4_TUWH[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 200, 370, 1},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_timeout", ExposeType::Numeric, Access::StateSet, "s",
     "Time in seconds before occupancy is cleared after the last detected movement.", nullptr, 0,
     ExposeCategory::Config, 0, 65534, 0},
    {"pir_uto_delay", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config, 0, 65534, 0},
    {"pir_uto_threshold", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0,
     ExposeCategory::Config, 1, 254, 0},
};

constexpr BindingSpec kBindings_CPXC_2X4_TUWH[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_CPXC_2X4_TUWH{
    .zigbee_models=kModels_CPXC_2X4_TUWH, .zigbee_models_count=sizeof(kModels_CPXC_2X4_TUWH)/sizeof(kModels_CPXC_2X4_TUWH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CPXC 2X4 TUWH", .vendor="AcuityBrandsLighting",
    .meta=nullptr, .exposes=kExposes_CPXC_2X4_TUWH, .exposes_count=sizeof(kExposes_CPXC_2X4_TUWH)/sizeof(kExposes_CPXC_2X4_TUWH[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CPXC_2X4_TUWH, .from_zigbee_count=sizeof(kFz_CPXC_2X4_TUWH)/sizeof(kFz_CPXC_2X4_TUWH[0]),
    .to_zigbee=kTz_CPXC_2X4_TUWH, .to_zigbee_count=sizeof(kTz_CPXC_2X4_TUWH)/sizeof(kTz_CPXC_2X4_TUWH[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_CPXC_2X4_TUWH, .bindings_count=sizeof(kBindings_CPXC_2X4_TUWH)/sizeof(kBindings_CPXC_2X4_TUWH[0]),
};

}  // namespace zhc::devices::acuity_brands_lighting
