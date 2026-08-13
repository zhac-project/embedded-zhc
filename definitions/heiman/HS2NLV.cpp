// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HEIMAN HS2NLV smart notifier — colour light plus T/H sensing
// (z2m v26.95.0 parity, added v26.94.0).
// z2m-source: heiman.ts #HS2NLV  (m.light({color: true}) + temperature + humidity)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS2NLV[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
const TzConverter* const kTz_HS2NLV[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_HS2NLV[] = { "HS2NLV" };

constexpr Expose kExposes_HS2NLV[] = {
    {"state",       ExposeType::Binary,  Access::StateSet, nullptr, "Light state",                nullptr, 0},
    {"brightness",  ExposeType::Numeric, Access::StateSet, nullptr, "Brightness",                 nullptr, 0},
    {"color_x",     ExposeType::Numeric, Access::StateSet, nullptr, "Colour x",                   nullptr, 0},
    {"color_y",     ExposeType::Numeric, Access::StateSet, nullptr, "Colour y",                   nullptr, 0},
    {"hue",         ExposeType::Numeric, Access::StateSet, nullptr, "Hue",                        nullptr, 0},
    {"saturation",  ExposeType::Numeric, Access::StateSet, nullptr, "Saturation",                 nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State,    "\u00b0C",    "Measured temperature",       nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State,    "%",     "Measured relative humidity", nullptr, 0},
};

constexpr BindingSpec kBindings_HS2NLV[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0300}, {1, 0x0402}, {1, 0x0405},
};
}  // namespace

extern const PreparedDefinition kDef_HS2NLV{
    .zigbee_models=kModels_HS2NLV, .zigbee_models_count=sizeof(kModels_HS2NLV)/sizeof(kModels_HS2NLV[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS2NLV", .vendor="HEIMAN",
    .meta=nullptr, .exposes=kExposes_HS2NLV, .exposes_count=sizeof(kExposes_HS2NLV)/sizeof(kExposes_HS2NLV[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS2NLV, .from_zigbee_count=sizeof(kFz_HS2NLV)/sizeof(kFz_HS2NLV[0]),
    .to_zigbee=kTz_HS2NLV, .to_zigbee_count=sizeof(kTz_HS2NLV)/sizeof(kTz_HS2NLV[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HS2NLV, .bindings_count=sizeof(kBindings_HS2NLV)/sizeof(kBindings_HS2NLV[0]),
};

}  // namespace zhc::devices::heiman
