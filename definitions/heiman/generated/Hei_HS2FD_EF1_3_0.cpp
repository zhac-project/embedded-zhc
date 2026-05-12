// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Heiman HS2FD-EF1-3.0 — auto-generated.
// Fall Detection Sensor
// z2m-source: heiman.ts #HS2FD-EF1-3.0.
//
// TODO(heiman): HS2FD-EF1-3.0 is a fall-detection sensor that, in z2m,
// uses standard genOnOff for the trigger. If a future revision exposes
// heimanClusterSpecial sensorFaultState (mute / fault / pollution),
// add `kFzHeimanSmokeSpecial` from definitions/heiman/_shared.hpp.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {


constexpr const char* kModels_HS2FD_EF1_3_0[] = { "HS2FD-EF1-3.0" };

}  // namespace


namespace {
const FzConverter* const kFz_min_HS2FD_EF1_3_0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_HS2FD_EF1_3_0[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_HS2FD_EF1_3_0[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_HS2FD_EF1_3_0{
    .zigbee_models=kModels_HS2FD_EF1_3_0, .zigbee_models_count=sizeof(kModels_HS2FD_EF1_3_0)/sizeof(kModels_HS2FD_EF1_3_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS2FD-EF1-3.0", .vendor="Heiman",
    .meta=nullptr, .exposes=kExp_min_HS2FD_EF1_3_0, .exposes_count=sizeof(kExp_min_HS2FD_EF1_3_0)/sizeof(kExp_min_HS2FD_EF1_3_0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_HS2FD_EF1_3_0, .from_zigbee_count=sizeof(kFz_min_HS2FD_EF1_3_0)/sizeof(kFz_min_HS2FD_EF1_3_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_HS2FD_EF1_3_0,
    .bindings_count           = sizeof(kBind_min_HS2FD_EF1_3_0)/sizeof(kBind_min_HS2FD_EF1_3_0[0]),
};

}  // namespace zhc::devices::heiman
