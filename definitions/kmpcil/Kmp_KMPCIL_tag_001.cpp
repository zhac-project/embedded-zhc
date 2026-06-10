// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Kmpcil KMPCIL-tag-001 — arrival sensor (presence + vibration).
//
// z2m-source: kmpcil.ts #KMPCIL-tag-001 (zigbeeModel "tagv1").
//
// Graduated from generated/ to fix a dead `battery` expose: z2m's
// presence_power converter applies meta.battery.voltageToPercentage
// "3V_1500_2800" to publish `battery` (%), but the ported
// kFzKmpcilPresencePower previously emitted only `voltage`, so the
// declared e.battery() expose was never filled. The shared converter now
// also emits battery % (see definitions/kmpcil/_shared.cpp).
//
// Single-endpoint device on EP 1. Wires:
//   - msTemperatureMeasurement → temperature                  [generic kFzTemperature]
//   - genPowerCfg              → voltage + battery +
//                                presence=true                [shared kFzKmpcilPresencePower]
//   - genBinaryInput attr 0x55 → presence + power_state +
//                                vibration + occupancy        [shared kFzKmpcilPresenceBinaryInput]
//
// z2m's converter additionally schedules a host-side `setTimeout` to
// publish `presence: false` after `presence_timeout_dc` /
// `presence_timeout_battery` seconds of silence. That side-channel
// has no on-device equivalent in ZHC v6 (no per-device wall-clock
// timers in dispatch). Downstream rules in `simple_rules` can
// emulate the behaviour against the shadow's lastSeen timestamp.
#include "definitions/_generic/_shared.hpp"
#include "definitions/kmpcil/_shared.hpp"

namespace zhc::devices::kmpcil {
namespace {

const FzConverter* const kFz_KMPCIL_tag_001[] = {
    &::zhc::generic::kFzTemperature,
    &kFzKmpcilPresencePower,
    &kFzKmpcilPresenceBinaryInput,
};

constexpr const char* kModels_KMPCIL_tag_001[] = { "tagv1" };

constexpr Expose kExp_KMPCIL_tag_001[] = {
    { "battery",     ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0,
      ::zhc::ExposeCategory::Diagnostic },
    { "presence",    ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "power_state", ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "occupancy",   ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "vibration",   ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "°C", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_KMPCIL_tag_001[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x000F },   // genBinaryInput
    { 1, 0x0402 },   // msTemperatureMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_KMPCIL_tag_001{
    .zigbee_models=kModels_KMPCIL_tag_001,
    .zigbee_models_count=sizeof(kModels_KMPCIL_tag_001)/sizeof(kModels_KMPCIL_tag_001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="KMPCIL-tag-001", .vendor="KMPCIL",
    .meta=nullptr,
    .exposes=kExp_KMPCIL_tag_001,
    .exposes_count=sizeof(kExp_KMPCIL_tag_001)/sizeof(kExp_KMPCIL_tag_001[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_KMPCIL_tag_001,
    .from_zigbee_count=sizeof(kFz_KMPCIL_tag_001)/sizeof(kFz_KMPCIL_tag_001[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_KMPCIL_tag_001,
    .bindings_count=sizeof(kBind_KMPCIL_tag_001)/sizeof(kBind_KMPCIL_tag_001[0]),
};

}  // namespace zhc::devices::kmpcil
