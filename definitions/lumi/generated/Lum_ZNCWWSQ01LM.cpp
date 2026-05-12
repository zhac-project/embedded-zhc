// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi ZNCWWSQ01LM — auto-generated.
// Smart pet feeder C1
// z2m-source: lumi.ts #ZNCWWSQ01LM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_ZNCWWSQ01LM[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_ZNCWWSQ01LM[] = { "aqara.feeder.acn001" };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZNCWWSQ01LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_ZNCWWSQ01LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZNCWWSQ01LM[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_ZNCWWSQ01LM{
    .zigbee_models=kModels_ZNCWWSQ01LM, .zigbee_models_count=sizeof(kModels_ZNCWWSQ01LM)/sizeof(kModels_ZNCWWSQ01LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZNCWWSQ01LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_ZNCWWSQ01LM, .exposes_count=sizeof(kExp_min_ZNCWWSQ01LM)/sizeof(kExp_min_ZNCWWSQ01LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZNCWWSQ01LM, .from_zigbee_count=sizeof(kFz_min_ZNCWWSQ01LM)/sizeof(kFz_min_ZNCWWSQ01LM[0]),
    .to_zigbee=kTz_ZNCWWSQ01LM, .to_zigbee_count=sizeof(kTz_ZNCWWSQ01LM)/sizeof(kTz_ZNCWWSQ01LM[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
