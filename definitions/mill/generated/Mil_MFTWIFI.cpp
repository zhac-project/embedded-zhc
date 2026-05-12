// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Mill MFTWIFI — auto-generated.
// Smart floor thermostat WiFi & Zigbee
// z2m-source: mill.ts #MFTWIFI.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mill {
namespace {


constexpr const char* kModels_MFTWIFI[] = { "Mill International", "Mill InternationalThrea" };

}  // namespace


namespace {
const FzConverter* const kFz_min_MFTWIFI[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_MFTWIFI[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_MFTWIFI[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_MFTWIFI{
    .zigbee_models=kModels_MFTWIFI, .zigbee_models_count=sizeof(kModels_MFTWIFI)/sizeof(kModels_MFTWIFI[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MFTWIFI", .vendor="Mill",
    .meta=nullptr, .exposes=kExp_min_MFTWIFI, .exposes_count=sizeof(kExp_min_MFTWIFI)/sizeof(kExp_min_MFTWIFI[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_MFTWIFI, .from_zigbee_count=sizeof(kFz_min_MFTWIFI)/sizeof(kFz_min_MFTWIFI[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_MFTWIFI,
    .bindings_count           = sizeof(kBind_min_MFTWIFI)/sizeof(kBind_min_MFTWIFI[0]),
};

}  // namespace zhc::devices::mill
