// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Phoscon Hive — hand-edited 2026-04-29c.
// Battery-powered smart LED light (color + CT + battery).
// z2m: m.light({colorTemp:[153,370], color:true}) + m.battery().
// Range mismatch: z2m caps CT at 370 mireds for this SKU; ZHC bundle
// uses the more permissive 153-500. Documented in DRESDEN_ELEKTRONIK_PARITY.md.
// z2m-source: dresden_elektronik.ts #Hive.
#include "definitions/dresden_elektronik/_shared.hpp"

namespace zhc::devices::dresden_elektronik {
namespace {

// Splice ColorCTLight bundle + generic battery channel.
const FzConverter* const kFz_Hive[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
    &::zhc::generic::kFzBattery,
};
const TzConverter* const kTz_Hive[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};
constexpr const char* kModels_Hive[] = { "Hive" };

constexpr Expose kHiveExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
    {"color_x",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue",        ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery",    ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"voltage",    ExposeType::Numeric, Access::State,    "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kHiveBindings[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0300}, {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_Hive{
    .zigbee_models=kModels_Hive, .zigbee_models_count=sizeof(kModels_Hive)/sizeof(kModels_Hive[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Hive", .vendor="Phoscon",
    .meta=nullptr,
    .exposes=kHiveExposes, .exposes_count=sizeof(kHiveExposes)/sizeof(kHiveExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Hive, .from_zigbee_count=sizeof(kFz_Hive)/sizeof(kFz_Hive[0]),
    .to_zigbee=kTz_Hive, .to_zigbee_count=sizeof(kTz_Hive)/sizeof(kTz_Hive[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kHiveBindings, .bindings_count=sizeof(kHiveBindings)/sizeof(kHiveBindings[0]),
};

}  // namespace zhc::devices::dresden_elektronik
