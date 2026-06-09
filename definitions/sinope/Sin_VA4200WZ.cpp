// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: graduated to fix wrong-cluster valve position/state.
// Zigbee smart water valve (3/4").
//
// z2m-source: sinope.ts #VA4200WZ — fromZigbee
//   [fz.cover_position_via_brightness, fz.cover_state_via_onoff, fz.battery],
//   toZigbee [tz.cover_via_brightness], exposes [valve_switch (state),
//   valve_position (position), battery].
//
// The generated def used the generic kFzCoverPosition / kTzCoverPosition,
// which read/write `closuresWindowCovering` (0x0102). This valve never
// reports on that cluster: position lives on genLevelCtrl currentLevel
// and open/closed on genOnOff. As written, position decode was dead and
// the open/closed `state` was absent entirely. Swap in the Sinopé valve
// converters (genLevelCtrl + genOnOff) and bind those clusters.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sinope/_shared.hpp"

namespace zhc::devices::sinope {
namespace {
const FzConverter* const kFz_VA4200WZ[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::sinope::kFzSinopeValvePosition,  // genLevelCtrl → position + state
    &::zhc::sinope::kFzSinopeValveState,     // genOnOff → state
};
const TzConverter* const kTz_VA4200WZ[] = {
    &::zhc::sinope::kTzSinopeValvePosition,  // position + state → genLevelCtrl
};
constexpr const char* kModels_VA4200WZ[] = { "VA4200WZ" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl (valve position)
};

extern const PreparedDefinition kDef_VA4200WZ{
    .zigbee_models=kModels_VA4200WZ, .zigbee_models_count=sizeof(kModels_VA4200WZ)/sizeof(kModels_VA4200WZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VA4200WZ", .vendor="Sinope",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VA4200WZ, .from_zigbee_count=sizeof(kFz_VA4200WZ)/sizeof(kFz_VA4200WZ[0]),
    .to_zigbee=kTz_VA4200WZ, .to_zigbee_count=sizeof(kTz_VA4200WZ)/sizeof(kTz_VA4200WZ[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sinope
