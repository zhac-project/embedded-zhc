// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Multiterm ZC0101 — ZeeFan fan coil unit controller.
//
// The auto-port modelled this as a bare single-binary `fan_mode` device and
// dropped the entire multi-endpoint binary-output half. z2m exposes:
//   * a fan control (hvacFanCtrl 0x0202) → fan_mode / fan_state, and
//   * three genBinaryOutput (0x0010) channels on endpoints 8/9/10:
//       ep8 silent_mode, ep9 heating_cooling, ep10 electric_valve
//     via `fzLocal.binary_output` ({state: presentValue===1 ?
//     activeText : inactiveText}) under m.deviceEndpoints({8,9,10}) +
//     meta.multiEndpoint.
//
// Gaps fixed:
//   (b) MISSING decoder — no genBinaryOutput converter was wired, so all
//       three channel states were dead. Added the generic kFzBinaryOutput
//       (presentValue → boolean `state`). z2m's per-device activeText/
//       inactiveText enum strings are runtime-only attributes, so the
//       static port emits the decodable boolean half (INFRA note below).
//   (f) MISSING endpoint_map — the three channels all share cluster
//       genBinaryOutput, so without a map their decoded `state` keys
//       collide (last-writer-wins). endpoint_map suffixes ep9→state_9,
//       ep10→state_10. ep8 is deliberately LEFT OUT of the map: the fan
//       control's hvacFanCtrl is bound on ep8 and z2m keeps fan_mode/
//       fan_state unsuffixed; mapping ep8 would rewrite the fan key to
//       `fan_mode_8`. ep8's silent_mode binary output therefore keeps the
//       bare `state` key — the three channels stay distinct
//       (state / state_9 / state_10) and the fan key stays bare.
//
// INFRA-deferred: the genBinaryOutput→enum-label mapping (active/inactive
// text per channel) and the binary_output toZigbee write path depend on
// per-device runtime attributes (activeText/inactiveText) with no static
// converter; the boolean presentValue decode is the faithful portable half.
//
// z2m-source: multiterm.ts #ZC0101.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::multiterm {
namespace {
const FzConverter* const kFz_ZC0101[] = {
    &::zhc::generic::kFzFanMode,
    &::zhc::generic::kFzBinaryOutput,
};

constexpr const char* kModels_ZC0101[] = { "ZC0101" };

constexpr const char* kSilentModeStates[] = { "inactive", "active" };
constexpr const char* kHeatingCoolingStates[] = { "heating", "cooling" };
constexpr const char* kElectricValveStates[] = { "off", "on" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    // Fan control (hvacFanCtrl). z2m e.fan().withState("fan_state").
    {"fan_state", ExposeType::Binary, Access::StateSet, nullptr, "Fan Control", nullptr, 0},
    // Three multi-endpoint binary-output channels. Exposes stay bare-keyed;
    // dispatch suffixes the runtime decode per endpoint_map.
    {"silent_mode", ExposeType::Enum, Access::StateSet, nullptr, "Silent mode",
     kSilentModeStates, 2, ExposeCategory::Config},
    {"heating_cooling", ExposeType::Enum, Access::StateSet, nullptr, "Heating/Cooling",
     kHeatingCoolingStates, 2, ExposeCategory::Config},
    {"electric_valve", ExposeType::Enum, Access::StateSet, nullptr, "Electric Valve",
     kElectricValveStates, 2, ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {8, 0x0202},   // hvacFanCtrl on the fan endpoint (z2m fanEp)
    {8, 0x0010},   // genBinaryOutput silent_mode
    {9, 0x0010},   // genBinaryOutput heating_cooling
    {10, 0x0010},  // genBinaryOutput electric_valve
};

// ep8 omitted on purpose — keeps fan_mode/fan_state + silent_mode `state`
// unsuffixed; ep9/ep10 binary outputs become state_9 / state_10.
constexpr ::zhc::EndpointLabel kEndpoints_ZC0101[] = { {"9", 9}, {"10", 10} };

extern const PreparedDefinition kDef_ZC0101{
    .zigbee_models=kModels_ZC0101, .zigbee_models_count=sizeof(kModels_ZC0101)/sizeof(kModels_ZC0101[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZC0101", .vendor="Multiterm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZC0101, .from_zigbee_count=sizeof(kFz_ZC0101)/sizeof(kFz_ZC0101[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_ZC0101,
    .endpoint_map_count = sizeof(kEndpoints_ZC0101)/sizeof(kEndpoints_ZC0101[0]),
};

}  // namespace zhc::devices::multiterm
