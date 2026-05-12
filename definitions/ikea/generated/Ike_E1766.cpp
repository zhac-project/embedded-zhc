// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: IKEA TRADFRI open/close remote (E1766). Two-button battery
// remote for window covers: emits closuresWindowCovering commands.
//
// z2m-source: ikea.ts #E1766 — commandsWindowCovering + ikeaBattery.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {

constexpr const char* kModels_E1766[] = { "TRADFRI open/close remote" };

constexpr const char* kActions_E1766[] = {
    "open",
    "close",
    "stop",
};

constexpr Expose kExposes_E1766[] = {
    { "action",  ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_E1766, sizeof(kActions_E1766)/sizeof(kActions_E1766[0]) },
    { "battery", ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_E1766[] = {
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_E1766[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0102 },   // closuresWindowCovering
};

}  // namespace

extern const PreparedDefinition kDef_E1766{
    .zigbee_models            = kModels_E1766,
    .zigbee_models_count      = sizeof(kModels_E1766)/sizeof(kModels_E1766[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "E1766",
    .vendor                   = "IKEA",
    .meta                     = nullptr,
    .exposes                  = kExposes_E1766,
    .exposes_count            = sizeof(kExposes_E1766)/sizeof(kExposes_E1766[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_E1766,
    .from_zigbee_count        = sizeof(kFz_E1766)/sizeof(kFz_E1766[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_E1766,
    .bindings_count           = sizeof(kBindings_E1766)/sizeof(kBindings_E1766[0]),
};

}  // namespace zhc::devices::ikea
