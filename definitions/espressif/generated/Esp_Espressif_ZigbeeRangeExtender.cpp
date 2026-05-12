// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Espressif Espressif-ZigbeeRangeExtender — auto-generated.
// ESP32-C6/H2 router
// z2m-source: espressif.ts #Espressif-ZigbeeRangeExtender.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::espressif {
namespace {


constexpr const char* kModels_Espressif_ZigbeeRangeExtender[] = { "ZigbeeRangeExtender" };

}  // namespace


namespace {
const FzConverter* const kFz_min_Espressif_ZigbeeRangeExtender[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_Espressif_ZigbeeRangeExtender[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_Espressif_ZigbeeRangeExtender[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_Espressif_ZigbeeRangeExtender{
    .zigbee_models=kModels_Espressif_ZigbeeRangeExtender, .zigbee_models_count=sizeof(kModels_Espressif_ZigbeeRangeExtender)/sizeof(kModels_Espressif_ZigbeeRangeExtender[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Espressif-ZigbeeRangeExtender", .vendor="Espressif",
    .meta=nullptr, .exposes=kExp_min_Espressif_ZigbeeRangeExtender, .exposes_count=sizeof(kExp_min_Espressif_ZigbeeRangeExtender)/sizeof(kExp_min_Espressif_ZigbeeRangeExtender[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_Espressif_ZigbeeRangeExtender, .from_zigbee_count=sizeof(kFz_min_Espressif_ZigbeeRangeExtender)/sizeof(kFz_min_Espressif_ZigbeeRangeExtender[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_Espressif_ZigbeeRangeExtender,
    .bindings_count           = sizeof(kBind_min_Espressif_ZigbeeRangeExtender)/sizeof(kBind_min_Espressif_ZigbeeRangeExtender[0]),
};

}  // namespace zhc::devices::espressif
