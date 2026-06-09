// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: graduated to fix wrong-cluster valve position/state.
// Sedna smart water valve.
//
// z2m-source: sinope.ts #VA4220ZB — fromZigbee
//   [fz.ignore_iaszone_statuschange, fz.cover_position_via_brightness,
//    fz.cover_state_via_onoff, fz.battery, fz.metering],
//   toZigbee [tz.cover_via_brightness].
//
// See Sin_VA4200WZ.cpp for rationale: the generated def read/wrote the
// `closuresWindowCovering` cluster, but this valve uses genLevelCtrl
// (position) + genOnOff (open/closed). Swap in the Sinopé valve
// converters; energy metering (seMetering) is retained.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sinope/_shared.hpp"

namespace zhc::devices::sinope {
namespace {
const FzConverter* const kFz_VA4220ZB[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::sinope::kFzSinopeValvePosition,  // genLevelCtrl → position + state
    &::zhc::sinope::kFzSinopeValveState,     // genOnOff → state
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_VA4220ZB[] = {
    &::zhc::sinope::kTzSinopeValvePosition,  // position + state → genLevelCtrl
};
constexpr const char* kModels_VA4220ZB[] = { "VA4220ZB" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl (valve position)
    {1, 0x0702},  // seMetering
};

extern const PreparedDefinition kDef_VA4220ZB{
    .zigbee_models=kModels_VA4220ZB, .zigbee_models_count=sizeof(kModels_VA4220ZB)/sizeof(kModels_VA4220ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VA4220ZB", .vendor="Sinope",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VA4220ZB, .from_zigbee_count=sizeof(kFz_VA4220ZB)/sizeof(kFz_VA4220ZB[0]),
    .to_zigbee=kTz_VA4220ZB, .to_zigbee_count=sizeof(kTz_VA4220ZB)/sizeof(kTz_VA4220ZB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sinope
