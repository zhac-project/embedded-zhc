// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adeo IG-CDZOTAAG014RA-MAN — hand-rewritten for parity.
// ENKI LEXMAN E27 14W to 100W LED RGBW v2
// z2m-source: adeo.ts #IG-CDZOTAAG014RA-MAN.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {
const FzConverter* const kFz_IG_CDZOTAAG014RA_MAN[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_IG_CDZOTAAG014RA_MAN[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColor,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_IG_CDZOTAAG014RA_MAN[] = { "ZBEK-2" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
{"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

extern const PreparedDefinition kDef_IG_CDZOTAAG014RA_MAN{
    .zigbee_models=kModels_IG_CDZOTAAG014RA_MAN, .zigbee_models_count=sizeof(kModels_IG_CDZOTAAG014RA_MAN)/sizeof(kModels_IG_CDZOTAAG014RA_MAN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="IG-CDZOTAAG014RA-MAN", .vendor="Adeo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_IG_CDZOTAAG014RA_MAN, .from_zigbee_count=sizeof(kFz_IG_CDZOTAAG014RA_MAN)/sizeof(kFz_IG_CDZOTAAG014RA_MAN[0]),
    .to_zigbee=kTz_IG_CDZOTAAG014RA_MAN, .to_zigbee_count=sizeof(kTz_IG_CDZOTAAG014RA_MAN)/sizeof(kTz_IG_CDZOTAAG014RA_MAN[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adeo
