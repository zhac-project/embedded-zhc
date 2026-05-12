// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi A6121 — auto-generated.
// Vima Smart Lock
// z2m-source: lumi.ts #A6121.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_A6121[] = { "lumi.lock.v1" };

}  // namespace

namespace {
const FzConverter* const kFz_min_A6121[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_A6121[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_A6121[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_A6121{
    .zigbee_models=kModels_A6121, .zigbee_models_count=sizeof(kModels_A6121)/sizeof(kModels_A6121[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="A6121", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_A6121, .exposes_count=sizeof(kExp_min_A6121)/sizeof(kExp_min_A6121[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_A6121, .from_zigbee_count=sizeof(kFz_min_A6121)/sizeof(kFz_min_A6121[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
