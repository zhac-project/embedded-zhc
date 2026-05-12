// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: IKEA TRADFRI shortcut button (E1812). Single-button remote
// emitting cluster commands on genOnOff.
//
// z2m-source: ikea.ts #E1812.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E1812[] = { "TRADFRI SHORTCUT Button" };

constexpr const char* kActions_E1812[] = {
    "on",
    "off",
};

constexpr Expose kExposes_E1812[] = {
    { "action",  ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_E1812, sizeof(kActions_E1812)/sizeof(kActions_E1812[0]) },
    { "battery", ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_E1812[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_E1812[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff
};

}  // namespace

extern const PreparedDefinition kDef_E1812{
    .zigbee_models            = kModels_E1812,
    .zigbee_models_count      = sizeof(kModels_E1812)/sizeof(kModels_E1812[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "E1812",
    .vendor                   = "IKEA",
    .meta                     = nullptr,
    .exposes                  = kExposes_E1812,
    .exposes_count            = sizeof(kExposes_E1812)/sizeof(kExposes_E1812[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_E1812,
    .from_zigbee_count        = sizeof(kFz_E1812)/sizeof(kFz_E1812[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_E1812,
    .bindings_count           = sizeof(kBindings_E1812)/sizeof(kBindings_E1812[0]),
};

}  // namespace zhc::devices::ikea
