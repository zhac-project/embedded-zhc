// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lytko L101Ze-SLM — auto-generated.
// Single channel thermostat without display
// z2m-source: lytko.ts #L101Ze-SLM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lytko {
namespace {


constexpr const char* kModels_L101Ze_SLM[] = { "L101Ze-SLM" };

constexpr ::zhc::EndpointLabel kEndpoints_L101Ze_SLM[] = { {"3", 3} };

}  // namespace


namespace {
const FzConverter* const kFz_min_L101Ze_SLM[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_L101Ze_SLM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_L101Ze_SLM[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_L101Ze_SLM{
    .zigbee_models=kModels_L101Ze_SLM, .zigbee_models_count=sizeof(kModels_L101Ze_SLM)/sizeof(kModels_L101Ze_SLM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="L101Ze-SLM", .vendor="Lytko",
    .meta=nullptr, .exposes=kExp_min_L101Ze_SLM, .exposes_count=sizeof(kExp_min_L101Ze_SLM)/sizeof(kExp_min_L101Ze_SLM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_L101Ze_SLM, .from_zigbee_count=sizeof(kFz_min_L101Ze_SLM)/sizeof(kFz_min_L101Ze_SLM[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_L101Ze_SLM,
    .bindings_count           = sizeof(kBind_min_L101Ze_SLM)/sizeof(kBind_min_L101Ze_SLM[0]),
    .endpoint_map       = kEndpoints_L101Ze_SLM,
    .endpoint_map_count = sizeof(kEndpoints_L101Ze_SLM)/sizeof(kEndpoints_L101Ze_SLM[0]),
};

}  // namespace zhc::devices::lytko
