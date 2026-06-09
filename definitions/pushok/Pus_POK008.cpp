// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Pushok POK008 — battery powered thermostat relay.
// z2m-source: pushok.ts #POK008.
// Graduated from generated/: z2m wires m.temperature({reporting:null})
// (msTemperatureMeasurement) — the relay's measured temperature — but the
// generated def dropped both the `temperature` expose and its decoder, so
// every temperature report silently dropped. Wired generic kFzTemperature +
// the expose + the 0x0402 binding. The config numerics (tgt_temperature on
// genAnalogOutput, hysteresis on genAnalogValue, set_op_mode on
// genMultistateOutput) have no generic decoder → INFRA, deferred.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK008[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_POK008[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_POK008[] = { "POK008" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
};

extern const PreparedDefinition kDef_POK008{
    .zigbee_models=kModels_POK008, .zigbee_models_count=sizeof(kModels_POK008)/sizeof(kModels_POK008[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK008", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK008, .from_zigbee_count=sizeof(kFz_POK008)/sizeof(kFz_POK008[0]),
    .to_zigbee=kTz_POK008, .to_zigbee_count=sizeof(kTz_POK008)/sizeof(kTz_POK008[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
