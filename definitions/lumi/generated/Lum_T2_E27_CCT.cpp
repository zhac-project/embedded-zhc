// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi T2_E27_CCT — auto-generated.
// E27 CCT led bulb
// z2m-source: lumi.ts #T2_E27_CCT.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_T2_E27_CCT[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_T2_E27_CCT[] = { "lumi.light.agl002", "lumi.light.agl004", "lumi.light.agl006", "lumi.light.agl008" };

}  // namespace

namespace {
const FzConverter* const kFz_min_T2_E27_CCT[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_T2_E27_CCT[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_T2_E27_CCT[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_T2_E27_CCT{
    .zigbee_models=kModels_T2_E27_CCT, .zigbee_models_count=sizeof(kModels_T2_E27_CCT)/sizeof(kModels_T2_E27_CCT[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T2_E27_CCT", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_T2_E27_CCT, .exposes_count=sizeof(kExp_min_T2_E27_CCT)/sizeof(kExp_min_T2_E27_CCT[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_T2_E27_CCT, .from_zigbee_count=sizeof(kFz_min_T2_E27_CCT)/sizeof(kFz_min_T2_E27_CCT[0]),
    .to_zigbee=kTz_T2_E27_CCT, .to_zigbee_count=sizeof(kTz_T2_E27_CCT)/sizeof(kTz_T2_E27_CCT[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
