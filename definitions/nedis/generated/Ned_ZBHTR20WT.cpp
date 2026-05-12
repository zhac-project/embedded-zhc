// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nedis ZBHTR20WT — auto-generated.
// Thermostat radiator valve
// z2m-source: nedis.ts #ZBHTR20WT.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nedis {
namespace {


constexpr const char* kModels_ZBHTR20WT[] = { "TS0601" };
constexpr const char* kManus_ZBHTR20WT[] = { "_TZE200_ne4pikwm", "_TZE284_ne4pikwm", "_TZE284_hcs66axl" };
}  // namespace


namespace {
const FzConverter* const kFz_min_ZBHTR20WT[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZBHTR20WT[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZBHTR20WT[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_ZBHTR20WT{
    .zigbee_models=kModels_ZBHTR20WT, .zigbee_models_count=sizeof(kModels_ZBHTR20WT)/sizeof(kModels_ZBHTR20WT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZBHTR20WT, .manufacturer_names_count=sizeof(kManus_ZBHTR20WT)/sizeof(kManus_ZBHTR20WT[0]),
    .model="ZBHTR20WT", .vendor="Nedis",
    .meta=nullptr, .exposes=kExp_min_ZBHTR20WT, .exposes_count=sizeof(kExp_min_ZBHTR20WT)/sizeof(kExp_min_ZBHTR20WT[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZBHTR20WT, .from_zigbee_count=sizeof(kFz_min_ZBHTR20WT)/sizeof(kFz_min_ZBHTR20WT[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_ZBHTR20WT,
    .bindings_count           = sizeof(kBind_min_ZBHTR20WT)/sizeof(kBind_min_ZBHTR20WT[0]),
};

}  // namespace zhc::devices::nedis
