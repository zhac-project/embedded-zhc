// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lincukoo W04-Z10T — auto-generated.
// Smart water leakage alarm sensor
// z2m-source: lincukoo.ts #W04-Z10T.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {


constexpr const char* kModels_W04_Z10T[] = { "TS0601" };
constexpr const char* kManus_W04_Z10T[] = { "_TZE284_vbgmewta", "_TZE284_iunyuzwe" };
}  // namespace

namespace {
const FzConverter* const kFz_min_W04_Z10T[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_W04_Z10T[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_W04_Z10T[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_W04_Z10T{
    .zigbee_models=kModels_W04_Z10T, .zigbee_models_count=sizeof(kModels_W04_Z10T)/sizeof(kModels_W04_Z10T[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_W04_Z10T, .manufacturer_names_count=sizeof(kManus_W04_Z10T)/sizeof(kManus_W04_Z10T[0]),
    .model="W04-Z10T", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_min_W04_Z10T, .exposes_count=sizeof(kExp_min_W04_Z10T)/sizeof(kExp_min_W04_Z10T[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_W04_Z10T, .from_zigbee_count=sizeof(kFz_min_W04_Z10T)/sizeof(kFz_min_W04_Z10T[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lincukoo
