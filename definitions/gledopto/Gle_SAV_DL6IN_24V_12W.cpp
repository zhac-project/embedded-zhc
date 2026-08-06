// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Savolar SAV-DL6IN-24V-12W 12W downlight RGB+CCT (z2m v26.92.0 parity).
// z2m-source: gledopto.ts #SAV-DL6IN-24V-12W (added v26.88.0)
// Upstream colorTemp range [158, 495] mireds.
// Upstream wraps this in gledoptoLight(); the Gledopto-specific quirks it
// adds are transport workarounds, so the device is modelled here on the
// standard ZCL light clusters plus power-on behaviour.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gledopto {
namespace {
const FzConverter* const kFz_SAV_DL6IN_24V_12W[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzPowerOnBehavior,
};
const TzConverter* const kTz_SAV_DL6IN_24V_12W[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_SAV_DL6IN_24V_12W[] = { "SAV-DL6IN-24V-12W" };

constexpr Expose kExposes_SAV_DL6IN_24V_12W[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_SAV_DL6IN_24V_12W[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_SAV_DL6IN_24V_12W{
    .zigbee_models=kModels_SAV_DL6IN_24V_12W, .zigbee_models_count=sizeof(kModels_SAV_DL6IN_24V_12W)/sizeof(kModels_SAV_DL6IN_24V_12W[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SAV-DL6IN-24V-12W", .vendor="Savolar",
    .meta=nullptr, .exposes=kExposes_SAV_DL6IN_24V_12W, .exposes_count=sizeof(kExposes_SAV_DL6IN_24V_12W)/sizeof(kExposes_SAV_DL6IN_24V_12W[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SAV_DL6IN_24V_12W, .from_zigbee_count=sizeof(kFz_SAV_DL6IN_24V_12W)/sizeof(kFz_SAV_DL6IN_24V_12W[0]),
    .to_zigbee=kTz_SAV_DL6IN_24V_12W, .to_zigbee_count=sizeof(kTz_SAV_DL6IN_24V_12W)/sizeof(kTz_SAV_DL6IN_24V_12W[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SAV_DL6IN_24V_12W, .bindings_count=sizeof(kBindings_SAV_DL6IN_24V_12W)/sizeof(kBindings_SAV_DL6IN_24V_12W[0]),
};

}  // namespace zhc::devices::gledopto
