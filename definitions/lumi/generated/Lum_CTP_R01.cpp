// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi CTP-R01 — auto-generated.
// Cube T1 Pro
// z2m-source: lumi.ts #CTP-R01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_CTP_R01[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_CTP_R01[] = { "lumi.remote.cagl02" };

}  // namespace

namespace {
const FzConverter* const kFz_min_CTP_R01[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiCTPR01Multistate,
    &::zhc::lumi::kFzLumiCubeAnalog,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_CTP_R01[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_CTP_R01[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_CTP_R01{
    .zigbee_models=kModels_CTP_R01, .zigbee_models_count=sizeof(kModels_CTP_R01)/sizeof(kModels_CTP_R01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="CTP-R01", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_CTP_R01, .exposes_count=sizeof(kExp_min_CTP_R01)/sizeof(kExp_min_CTP_R01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_CTP_R01, .from_zigbee_count=sizeof(kFz_min_CTP_R01)/sizeof(kFz_min_CTP_R01[0]),
    .to_zigbee=kTz_CTP_R01, .to_zigbee_count=sizeof(kTz_CTP_R01)/sizeof(kTz_CTP_R01[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
