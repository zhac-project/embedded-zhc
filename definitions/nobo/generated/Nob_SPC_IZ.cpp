// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nobo SPC-IZ — auto-generated.
// Control unit for panel heaters
// z2m-source: nobo.ts #SPC-IZ.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nobo {
namespace {


constexpr const char* kModels_SPC_IZ[] = { "SPC-IZ" };

}  // namespace


namespace {
const FzConverter* const kFz_min_SPC_IZ[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_SPC_IZ[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_SPC_IZ[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_SPC_IZ{
    .zigbee_models=kModels_SPC_IZ, .zigbee_models_count=sizeof(kModels_SPC_IZ)/sizeof(kModels_SPC_IZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SPC-IZ", .vendor="Nobo",
    .meta=nullptr, .exposes=kExp_min_SPC_IZ, .exposes_count=sizeof(kExp_min_SPC_IZ)/sizeof(kExp_min_SPC_IZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_SPC_IZ, .from_zigbee_count=sizeof(kFz_min_SPC_IZ)/sizeof(kFz_min_SPC_IZ[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_SPC_IZ,
    .bindings_count           = sizeof(kBind_min_SPC_IZ)/sizeof(kBind_min_SPC_IZ[0]),
};

}  // namespace zhc::devices::nobo
