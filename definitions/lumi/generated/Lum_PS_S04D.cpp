// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi PS-S04D — auto-generated.
// Presence sensor FP300
// z2m-source: lumi.ts #PS-S04D.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_PS_S04D[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_PS_S04D[] = { "lumi.sensor_occupy.agl8" };

}  // namespace

namespace {
const FzConverter* const kFz_min_PS_S04D[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_PS_S04D[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_PS_S04D[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_PS_S04D{
    .zigbee_models=kModels_PS_S04D, .zigbee_models_count=sizeof(kModels_PS_S04D)/sizeof(kModels_PS_S04D[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PS-S04D", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_PS_S04D, .exposes_count=sizeof(kExp_min_PS_S04D)/sizeof(kExp_min_PS_S04D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_PS_S04D, .from_zigbee_count=sizeof(kFz_min_PS_S04D)/sizeof(kFz_min_PS_S04D[0]),
    .to_zigbee=kTz_PS_S04D, .to_zigbee_count=sizeof(kTz_PS_S04D)/sizeof(kTz_PS_S04D[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
