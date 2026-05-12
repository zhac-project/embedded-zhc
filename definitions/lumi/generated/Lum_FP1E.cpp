// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi FP1E — auto-generated.
// Presence sensor
// z2m-source: lumi.ts #FP1E.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_FP1E[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_FP1E[] = { "lumi.sensor_occupy.agl1" };

}  // namespace

namespace {
const FzConverter* const kFz_min_FP1E[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_FP1E[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_FP1E[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_FP1E{
    .zigbee_models=kModels_FP1E, .zigbee_models_count=sizeof(kModels_FP1E)/sizeof(kModels_FP1E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FP1E", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_FP1E, .exposes_count=sizeof(kExp_min_FP1E)/sizeof(kExp_min_FP1E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_FP1E, .from_zigbee_count=sizeof(kFz_min_FP1E)/sizeof(kFz_min_FP1E[0]),
    .to_zigbee=kTz_FP1E, .to_zigbee_count=sizeof(kTz_FP1E)/sizeof(kTz_FP1E[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
