// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Philips 8720169277243 — auto-generated.
// Hue Secure siren and chime
// z2m-source: philips.ts #8720169277243.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::philips {
namespace {


constexpr const char* kModels_D8720169277243[] = { "COM001" };

}  // namespace


namespace {
const FzConverter* const kFz_min_8720169277243[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_8720169277243[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_8720169277243[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_D8720169277243{
    .zigbee_models=kModels_D8720169277243, .zigbee_models_count=sizeof(kModels_D8720169277243)/sizeof(kModels_D8720169277243[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="8720169277243", .vendor="Philips",
    .meta=nullptr, .exposes=kExp_min_8720169277243, .exposes_count=sizeof(kExp_min_8720169277243)/sizeof(kExp_min_8720169277243[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_8720169277243, .from_zigbee_count=sizeof(kFz_min_8720169277243)/sizeof(kFz_min_8720169277243[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_8720169277243,
    .bindings_count           = sizeof(kBind_min_8720169277243)/sizeof(kBind_min_8720169277243[0]),
};

}  // namespace zhc::devices::philips
