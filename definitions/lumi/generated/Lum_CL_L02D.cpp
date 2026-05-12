// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi CL-L02D — auto-generated.
// Ceiling light T1M
// z2m-source: lumi.ts #CL-L02D.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_CL_L02D[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_CL_L02D[] = { "lumi.light.acn032", "lumi.light.acn031" };

}  // namespace

namespace {
const FzConverter* const kFz_min_CL_L02D[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_CL_L02D[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_CL_L02D[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_CL_L02D{
    .zigbee_models=kModels_CL_L02D, .zigbee_models_count=sizeof(kModels_CL_L02D)/sizeof(kModels_CL_L02D[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CL-L02D", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_CL_L02D, .exposes_count=sizeof(kExp_min_CL_L02D)/sizeof(kExp_min_CL_L02D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_CL_L02D, .from_zigbee_count=sizeof(kFz_min_CL_L02D)/sizeof(kFz_min_CL_L02D[0]),
    .to_zigbee=kTz_CL_L02D, .to_zigbee_count=sizeof(kTz_CL_L02D)/sizeof(kTz_CL_L02D[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
