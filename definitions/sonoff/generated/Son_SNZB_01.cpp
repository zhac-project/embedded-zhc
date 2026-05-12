// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sonoff SNZB-01 / eWeLink RHK07 wireless button.
//
// z2m-source: sonoff.ts #SNZB-01.  Exposes `action` with values
// single / double / long; uses `fz.ewelink_action` which reinterprets
// `genOnOff` command ids (0x01/0x00/0x02) as click types.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {

constexpr const char* kModels_SNZB_01[] = {
    "WB01", "WB-01", "SNZB-01", "CK-TLSR8656-SS5-01(7000)",
};

constexpr const char* kActions_SNZB_01[] = {
    "single",
    "double",
    "long",
};

constexpr Expose kExposes_SNZB_01[] = {
    { "action",  ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_SNZB_01, sizeof(kActions_SNZB_01)/sizeof(kActions_SNZB_01[0]) },
    { "battery", ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_SNZB_01[] = {
    &::zhc::generic::kFzEwelinkAction,
    &::zhc::generic::kFzBattery,
};

constexpr BindingSpec kBindings_SNZB_01[] = {
    { 1, 0x0001 },   // genPowerCfg
    { 1, 0x0006 },   // genOnOff (action source)
};

}  // namespace

extern const PreparedDefinition kDef_SNZB_01{
    .zigbee_models            = kModels_SNZB_01,
    .zigbee_models_count      = sizeof(kModels_SNZB_01)/sizeof(kModels_SNZB_01[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "SNZB-01",
    .vendor                   = "SONOFF",
    .meta                     = nullptr,
    .exposes                  = kExposes_SNZB_01,
    .exposes_count            = sizeof(kExposes_SNZB_01)/sizeof(kExposes_SNZB_01[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_SNZB_01,
    .from_zigbee_count        = sizeof(kFz_SNZB_01)/sizeof(kFz_SNZB_01[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_SNZB_01,
    .bindings_count           = sizeof(kBindings_SNZB_01)/sizeof(kBindings_SNZB_01[0]),
};

}  // namespace zhc::devices::sonoff
