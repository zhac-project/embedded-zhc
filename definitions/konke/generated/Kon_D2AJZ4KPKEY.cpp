// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Konke 2AJZ4KPKEY — multi-function button (hand-rewritten).
// z2m-source: konke.ts #2AJZ4KPKEY (fz.konke_action + fz.battery).
//   genOnOff attr 0x0000 value 128/129/130 → action single/double/hold.
#include "definitions/_generic/_shared.hpp"
#include "definitions/konke/_shared.hpp"

namespace zhc::devices::konke {
namespace {
const FzConverter* const kFz_D2AJZ4KPKEY[] = {
    &::zhc::devices::konke::kFzKonkeAction,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D2AJZ4KPKEY[] = { "3AFE170100510001", "3AFE280100510001" };

constexpr Expose kAutoExposes[] = {
    {"action",      ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
};

}  // namespace

extern const PreparedDefinition kDef_D2AJZ4KPKEY{
    .zigbee_models=kModels_D2AJZ4KPKEY, .zigbee_models_count=sizeof(kModels_D2AJZ4KPKEY)/sizeof(kModels_D2AJZ4KPKEY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="2AJZ4KPKEY", .vendor="Konke",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D2AJZ4KPKEY, .from_zigbee_count=sizeof(kFz_D2AJZ4KPKEY)/sizeof(kFz_D2AJZ4KPKEY[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::konke
