// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lytko L101Ze-DLN — auto-generated.
// Dual channel thermostat without display
// z2m-source: lytko.ts #L101Ze-DLN.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lytko {
namespace {


constexpr const char* kModels_L101Ze_DLN[] = { "L101Ze-DLN" };

constexpr ::zhc::EndpointLabel kEndpoints_L101Ze_DLN[] = { {"3", 3}, {"4", 4} };

}  // namespace


namespace {
const FzConverter* const kFz_min_L101Ze_DLN[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_L101Ze_DLN[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_L101Ze_DLN[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_L101Ze_DLN{
    .zigbee_models=kModels_L101Ze_DLN, .zigbee_models_count=sizeof(kModels_L101Ze_DLN)/sizeof(kModels_L101Ze_DLN[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-DLN", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_min_L101Ze_DLN, .exposes_count=sizeof(kExp_min_L101Ze_DLN)/sizeof(kExp_min_L101Ze_DLN[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_L101Ze_DLN, .from_zigbee_count=sizeof(kFz_min_L101Ze_DLN)/sizeof(kFz_min_L101Ze_DLN[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_L101Ze_DLN,
    .bindings_count           = sizeof(kBind_min_L101Ze_DLN)/sizeof(kBind_min_L101Ze_DLN[0]),
    .endpoint_map       = kEndpoints_L101Ze_DLN,
    .endpoint_map_count = sizeof(kEndpoints_L101Ze_DLN)/sizeof(kEndpoints_L101Ze_DLN[0]),
};

}  // namespace zhc::devices::lytko
