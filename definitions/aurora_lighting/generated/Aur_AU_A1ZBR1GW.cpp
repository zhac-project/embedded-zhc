// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: AuroraLighting AU-A1ZBR1GW — auto-generated.
// AOne one gang wireless battery rotary dimmer
// z2m-source: aurora_lighting.ts #AU-A1ZBR1GW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {


constexpr const char* kModels_AU_A1ZBR1GW[] = { "1GBatteryDimmer50AU" };

}  // namespace


namespace {
const FzConverter* const kFz_min_AU_A1ZBR1GW[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_AU_A1ZBR1GW[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_AU_A1ZBR1GW[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_AU_A1ZBR1GW{
    .zigbee_models=kModels_AU_A1ZBR1GW, .zigbee_models_count=sizeof(kModels_AU_A1ZBR1GW)/sizeof(kModels_AU_A1ZBR1GW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZBR1GW", .vendor="AuroraLighting",
    .meta=nullptr, .exposes=kExp_min_AU_A1ZBR1GW, .exposes_count=sizeof(kExp_min_AU_A1ZBR1GW)/sizeof(kExp_min_AU_A1ZBR1GW[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_AU_A1ZBR1GW, .from_zigbee_count=sizeof(kFz_min_AU_A1ZBR1GW)/sizeof(kFz_min_AU_A1ZBR1GW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_AU_A1ZBR1GW,
    .bindings_count           = sizeof(kBind_min_AU_A1ZBR1GW)/sizeof(kBind_min_AU_A1ZBR1GW[0]),
};

}  // namespace zhc::devices::aurora_lighting
