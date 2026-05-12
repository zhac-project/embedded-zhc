// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: VSmart HS-SEOC00ZB-VNM — uses lumi shared kFzOccupancy.
// Occupancy sensor (battery + msOccupancySensing 0x0406).
// z2m-source: vsmart.ts #HS-SEOC00ZB-VNM (`extend: [m.battery(), m.occupancy()]`).
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::vsmart {
namespace {
const FzConverter* const kFz_HS_SEOC00ZB_VNM[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::lumi::kFzOccupancy,
};

constexpr const char* kModels_HS_SEOC00ZB_VNM[] = { "HS-SEOC00ZB-VNM" };

constexpr Expose kAutoExposes[] = {
    {"battery",   ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",   ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0406},
};

}  // namespace

extern const PreparedDefinition kDef_HS_SEOC00ZB_VNM{
    .zigbee_models=kModels_HS_SEOC00ZB_VNM, .zigbee_models_count=sizeof(kModels_HS_SEOC00ZB_VNM)/sizeof(kModels_HS_SEOC00ZB_VNM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS-SEOC00ZB-VNM", .vendor="VSmart",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS_SEOC00ZB_VNM, .from_zigbee_count=sizeof(kFz_HS_SEOC00ZB_VNM)/sizeof(kFz_HS_SEOC00ZB_VNM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vsmart
