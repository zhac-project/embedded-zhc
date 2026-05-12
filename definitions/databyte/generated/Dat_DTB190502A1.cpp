// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Databyte DTB190502A1 — auto-generated.
// CC2530 based IO Board
// z2m-source: databyte.ts #DTB190502A1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::databyte {
namespace {


constexpr const char* kModels_DTB190502A1[] = { "DTB190502A1" };

}  // namespace


namespace {
const FzConverter* const kFz_min_DTB190502A1[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_DTB190502A1[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_DTB190502A1[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_DTB190502A1{
    .zigbee_models=kModels_DTB190502A1, .zigbee_models_count=sizeof(kModels_DTB190502A1)/sizeof(kModels_DTB190502A1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DTB190502A1", .vendor="Databyte",
    .meta=nullptr, .exposes=kExp_min_DTB190502A1, .exposes_count=sizeof(kExp_min_DTB190502A1)/sizeof(kExp_min_DTB190502A1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_DTB190502A1, .from_zigbee_count=sizeof(kFz_min_DTB190502A1)/sizeof(kFz_min_DTB190502A1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_DTB190502A1,
    .bindings_count           = sizeof(kBind_min_DTB190502A1)/sizeof(kBind_min_DTB190502A1[0]),
};

}  // namespace zhc::devices::databyte
