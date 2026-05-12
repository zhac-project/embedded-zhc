// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 81949 — hand-rewritten to use Adu_LightEM bundle.
// ERIA built-in dimmer module (with power measurements).
// z2m extend: m.light({configureReporting: true}) + m.electricityMeter({cluster: "electrical"}) + 9x adurosmart.extend.dimmer*.
// PARTIAL: 9 vendor-specific extends (dimmerLoadControlMode, dimmerSwitchMode, dimmerInvertSwitch, dimmerSceneActivation, dimmerS1DoubleClickScene, dimmerS2DoubleClickScene, dimmerMinBrightnessLevel, dimmerMaxBrightnessLevel, dimmerManualDimmingStepSize/Time) read/write genBasic 0x7600/0x7700/0x7701/etc. Not yet exposed. See ADUROSMART_PARITY.md.
// z2m-source: adurosmart.ts #81949.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81949[] = { "DimmerM3002" };

}  // namespace

extern const PreparedDefinition kDef_D81949{
    .zigbee_models=kModels_D81949,
    .zigbee_models_count=sizeof(kModels_D81949)/sizeof(kModels_D81949[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="81949", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduLightEM, .exposes_count=kExposesAduLightEMCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduLightEM, .from_zigbee_count=kFzAduLightEMCount,
    .to_zigbee=kTzAduLightEM, .to_zigbee_count=kTzAduLightEMCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduLightEM, .bindings_count=kBindingsAduLightEMCount,
};

}  // namespace zhc::devices::adurosmart
