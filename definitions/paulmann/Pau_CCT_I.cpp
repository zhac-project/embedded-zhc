// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Paulmann CCT-I tunable white controller with integrated motion sensor (z2m v26.92.0 parity).
// z2m-source: paulmann.ts #CCT-I (added v26.84.0)
// Upstream colorTemp range [153, 370] mireds.
// Fingerprint is modelID + manufacturerName -- the modelID "CCT-I" alone is
// not unique. The integrated motion sensor is NOT exposed upstream either.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::paulmann {
namespace {
const FzConverter* const kFz_CCT_I[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_CCT_I[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_CCT_I[] = { "CCT-I" };
constexpr const char* kManus_CCT_I[] = { "Paulmann Licht GmbH" };

constexpr Expose kExposes_CCT_I[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_CCT_I[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_CCT_I{
    .zigbee_models=kModels_CCT_I, .zigbee_models_count=sizeof(kModels_CCT_I)/sizeof(kModels_CCT_I[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_CCT_I, .manufacturer_names_count=sizeof(kManus_CCT_I)/sizeof(kManus_CCT_I[0]),
    .model="CCT-I", .vendor="Paulmann",
    .meta=nullptr, .exposes=kExposes_CCT_I, .exposes_count=sizeof(kExposes_CCT_I)/sizeof(kExposes_CCT_I[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_CCT_I, .from_zigbee_count=sizeof(kFz_CCT_I)/sizeof(kFz_CCT_I[0]),
    .to_zigbee=kTz_CCT_I, .to_zigbee_count=sizeof(kTz_CCT_I)/sizeof(kTz_CCT_I[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_CCT_I, .bindings_count=sizeof(kBindings_CCT_I)/sizeof(kBindings_CCT_I[0]),
};

}  // namespace zhc::devices::paulmann
