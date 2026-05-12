// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ysrsai YSR-MINI-01_wwcw — auto-generated.
// Zigbee LED controller (WW/CW)
// z2m-source: ysrsai.ts #YSR-MINI-01_wwcw.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ysrsai {
namespace {


constexpr const char* kModels_YSR_MINI_01_wwcw[] = { "ZB-CT01" };

}  // namespace


namespace {
const FzConverter* const kFz_min_YSR_MINI_01_wwcw[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_YSR_MINI_01_wwcw[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_YSR_MINI_01_wwcw[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_YSR_MINI_01_wwcw{
    .zigbee_models=kModels_YSR_MINI_01_wwcw, .zigbee_models_count=sizeof(kModels_YSR_MINI_01_wwcw)/sizeof(kModels_YSR_MINI_01_wwcw[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YSR-MINI-01_wwcw", .vendor="Ysrsai",
    .meta=nullptr, .exposes=kExp_min_YSR_MINI_01_wwcw, .exposes_count=sizeof(kExp_min_YSR_MINI_01_wwcw)/sizeof(kExp_min_YSR_MINI_01_wwcw[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_YSR_MINI_01_wwcw, .from_zigbee_count=sizeof(kFz_min_YSR_MINI_01_wwcw)/sizeof(kFz_min_YSR_MINI_01_wwcw[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_YSR_MINI_01_wwcw,
    .bindings_count           = sizeof(kBind_min_YSR_MINI_01_wwcw)/sizeof(kBind_min_YSR_MINI_01_wwcw[0]),
};

}  // namespace zhc::devices::ysrsai
