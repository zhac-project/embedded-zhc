// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ozsmartthings DL41-03-10-R-ZB — auto-generated.
// Oz Smart RGBW Zigbee downlight 10w
// z2m-source: ozsmartthings.ts #DL41-03-10-R-ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ozsmartthings {
namespace {


constexpr const char* kModels_DL41_03_10_R_ZB[] = { "TS0505B" };
constexpr const char* kManus_DL41_03_10_R_ZB[] = { "_TZ3210_mcm6m1ma", "_TZ3210_klsm24op", "_TZ3210_pdqu9pot" };
}  // namespace


namespace {
const FzConverter* const kFz_min_DL41_03_10_R_ZB[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_DL41_03_10_R_ZB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_DL41_03_10_R_ZB[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_DL41_03_10_R_ZB{
    .zigbee_models=kModels_DL41_03_10_R_ZB, .zigbee_models_count=sizeof(kModels_DL41_03_10_R_ZB)/sizeof(kModels_DL41_03_10_R_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_DL41_03_10_R_ZB, .manufacturer_names_count=sizeof(kManus_DL41_03_10_R_ZB)/sizeof(kManus_DL41_03_10_R_ZB[0]),
    .model="DL41-03-10-R-ZB", .vendor="Ozsmartthings",
    .meta=nullptr, .exposes=kExp_min_DL41_03_10_R_ZB, .exposes_count=sizeof(kExp_min_DL41_03_10_R_ZB)/sizeof(kExp_min_DL41_03_10_R_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_DL41_03_10_R_ZB, .from_zigbee_count=sizeof(kFz_min_DL41_03_10_R_ZB)/sizeof(kFz_min_DL41_03_10_R_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_DL41_03_10_R_ZB,
    .bindings_count           = sizeof(kBind_min_DL41_03_10_R_ZB)/sizeof(kBind_min_DL41_03_10_R_ZB[0]),
};

}  // namespace zhc::devices::ozsmartthings
