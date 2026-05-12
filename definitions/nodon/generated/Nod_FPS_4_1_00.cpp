// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon FPS-4-1-00 — hand-rewrite (added temp/humidity/PoB).
// Electrical heating actuator.
//
// z2m: `m.onOff({powerOnBehavior:true}) + m.electricityMeter({cluster:'metering'}) +
//       m.temperature() + m.humidity() + nodonPilotWire(true)`.
// Custom 0xFC00 PilotWire cluster handling is a TODO (see
// `nodon/_shared.hpp`); the rest is wired here.
//
// z2m-source: nodon.ts #FPS-4-1-00.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_FPS_4_1_00[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
const TzConverter* const kTz_FPS_4_1_00[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};
constexpr const char* kModels_FPS_4_1_00[] = { "FPS-4-1-00" };

}  // namespace


// Hand-aligned to z2m exposes.
constexpr Expose kAutoExposes[] = {
    {"state",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum,    Access::StateSet, nullptr,
        "Controls the behavior when the device is powered on after power loss",
        nullptr, 0, ExposeCategory::Config},
    {"energy",            ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power",             ExposeType::Numeric, Access::State, "W",   nullptr, nullptr, 0},
    {"temperature",       ExposeType::Numeric, Access::State, "C",   nullptr, nullptr, 0},
    {"humidity",          ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    // pilot_wire_mode requires custom 0xFC00 cluster — TODO.
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0402},
    {1, 0x0405},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_FPS_4_1_00{
    .zigbee_models=kModels_FPS_4_1_00, .zigbee_models_count=sizeof(kModels_FPS_4_1_00)/sizeof(kModels_FPS_4_1_00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FPS-4-1-00", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FPS_4_1_00, .from_zigbee_count=sizeof(kFz_FPS_4_1_00)/sizeof(kFz_FPS_4_1_00[0]),
    .to_zigbee=kTz_FPS_4_1_00, .to_zigbee_count=sizeof(kTz_FPS_4_1_00)/sizeof(kTz_FPS_4_1_00[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
