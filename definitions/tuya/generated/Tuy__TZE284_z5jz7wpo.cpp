// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya _TZE284_z5jz7wpo — auto-generated.
// Ceiling fan control module
// z2m-source: tuya.ts #_TZE284_z5jz7wpo.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels__TZE284_z5jz7wpo[] = { "TS0601" };
constexpr const char* kManus__TZE284_z5jz7wpo[] = { "_TZE284_z5jz7wpo" };
}  // namespace

namespace {
const FzConverter* const kFz_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef__TZE284_z5jz7wpo{
    .zigbee_models=kModels__TZE284_z5jz7wpo, .zigbee_models_count=sizeof(kModels__TZE284_z5jz7wpo)/sizeof(kModels__TZE284_z5jz7wpo[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus__TZE284_z5jz7wpo, .manufacturer_names_count=sizeof(kManus__TZE284_z5jz7wpo)/sizeof(kManus__TZE284_z5jz7wpo[0]),
    .model="_TZE284_z5jz7wpo", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo, .exposes_count=sizeof(kExp_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo)/sizeof(kExp_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo, .from_zigbee_count=sizeof(kFz_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo)/sizeof(kFz_min__TZE284_z5jz7wpo_Tuy__TZE284_z5jz7wpo[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
