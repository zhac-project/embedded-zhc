// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: HaloSmartLabs HALO — auto-generated.
// Halo smart smoke & CO detector
// z2m-source: halo_smart_labs.ts #HALO.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::halo_smart_labs {
namespace {


constexpr const char* kModels_HALO[] = { "halo" };

}  // namespace


namespace {
const FzConverter* const kFz_min_HALO[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_HALO[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_HALO[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_HALO{
    .zigbee_models=kModels_HALO, .zigbee_models_count=sizeof(kModels_HALO)/sizeof(kModels_HALO[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HALO", .vendor="HaloSmartLabs",
    .meta=nullptr, .exposes=kExp_min_HALO, .exposes_count=sizeof(kExp_min_HALO)/sizeof(kExp_min_HALO[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_HALO, .from_zigbee_count=sizeof(kFz_min_HALO)/sizeof(kFz_min_HALO[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_HALO,
    .bindings_count           = sizeof(kBind_min_HALO)/sizeof(kBind_min_HALO[0]),
};

}  // namespace zhc::devices::halo_smart_labs
