// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Msh msh.ds18b20psm — temperature channel restored.
// MSH outdoor thermometer with DS18B20
// z2m-source: msh.ts #msh.ds18b20psm
//   extend: [m.battery(), m.temperature()]
//
// Bug fixed: the Tier-1 port reduced this temp sensor to battery-only.
// z2m m.temperature() binds msTemperatureMeasurement (0x0402, /100). Wired
// the generic kFzTemperature decoder + expose + binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::msh {
namespace {
const FzConverter* const kFz_msh_ds18b20psm[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_msh_ds18b20psm[] = { "msh.ds18b20psm" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
};

extern const PreparedDefinition kDef_msh_ds18b20psm{
    .zigbee_models=kModels_msh_ds18b20psm, .zigbee_models_count=sizeof(kModels_msh_ds18b20psm)/sizeof(kModels_msh_ds18b20psm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="msh.ds18b20psm", .vendor="Msh",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_msh_ds18b20psm, .from_zigbee_count=sizeof(kFz_msh_ds18b20psm)/sizeof(kFz_msh_ds18b20psm[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::msh
