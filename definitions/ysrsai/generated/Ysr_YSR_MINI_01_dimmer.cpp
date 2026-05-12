// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ysrsai YSR-MINI-01_dimmer — auto-generated.
// Zigbee LED controller (Dimmer)
// z2m-source: ysrsai.ts #YSR-MINI-01_dimmer.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ysrsai {
namespace {


constexpr const char* kModels_YSR_MINI_01_dimmer[] = { "ZB-DL01" };

}  // namespace


namespace {
const FzConverter* const kFz_min_YSR_MINI_01_dimmer[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_YSR_MINI_01_dimmer[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_YSR_MINI_01_dimmer[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_YSR_MINI_01_dimmer{
    .zigbee_models=kModels_YSR_MINI_01_dimmer, .zigbee_models_count=sizeof(kModels_YSR_MINI_01_dimmer)/sizeof(kModels_YSR_MINI_01_dimmer[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YSR-MINI-01_dimmer", .vendor="Ysrsai",
    .meta=nullptr, .exposes=kExp_min_YSR_MINI_01_dimmer, .exposes_count=sizeof(kExp_min_YSR_MINI_01_dimmer)/sizeof(kExp_min_YSR_MINI_01_dimmer[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_YSR_MINI_01_dimmer, .from_zigbee_count=sizeof(kFz_min_YSR_MINI_01_dimmer)/sizeof(kFz_min_YSR_MINI_01_dimmer[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_YSR_MINI_01_dimmer,
    .bindings_count           = sizeof(kBind_min_YSR_MINI_01_dimmer)/sizeof(kBind_min_YSR_MINI_01_dimmer[0]),
};

}  // namespace zhc::devices::ysrsai
