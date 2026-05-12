// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi WT-A03E — auto-generated.
// Radiator thermostat W600
// z2m-source: lumi.ts #WT-A03E.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_WT_A03E[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_WT_A03E[] = { "lumi.airrtc.aeu005" };

}  // namespace

namespace {
const FzConverter* const kFz_min_WT_A03E[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_WT_A03E[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_WT_A03E[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_WT_A03E{
    .zigbee_models=kModels_WT_A03E, .zigbee_models_count=sizeof(kModels_WT_A03E)/sizeof(kModels_WT_A03E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WT-A03E", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_WT_A03E, .exposes_count=sizeof(kExp_min_WT_A03E)/sizeof(kExp_min_WT_A03E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_WT_A03E, .from_zigbee_count=sizeof(kFz_min_WT_A03E)/sizeof(kFz_min_WT_A03E[0]),
    .to_zigbee=kTz_WT_A03E, .to_zigbee_count=sizeof(kTz_WT_A03E)/sizeof(kTz_WT_A03E[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
