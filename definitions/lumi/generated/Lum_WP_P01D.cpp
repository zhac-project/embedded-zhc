// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi WP-P01D — auto-generated.
// Smart wall outlet H2 EU
// z2m-source: lumi.ts #WP-P01D.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_WP_P01D[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_WP_P01D[] = { "lumi.plug.aeu001" };

}  // namespace

namespace {
const FzConverter* const kFz_min_WP_P01D[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_WP_P01D[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_WP_P01D[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_WP_P01D{
    .zigbee_models=kModels_WP_P01D, .zigbee_models_count=sizeof(kModels_WP_P01D)/sizeof(kModels_WP_P01D[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WP-P01D", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_WP_P01D, .exposes_count=sizeof(kExp_min_WP_P01D)/sizeof(kExp_min_WP_P01D[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_WP_P01D, .from_zigbee_count=sizeof(kFz_min_WP_P01D)/sizeof(kFz_min_WP_P01D[0]),
    .to_zigbee=kTz_WP_P01D, .to_zigbee_count=sizeof(kTz_WP_P01D)/sizeof(kTz_WP_P01D[0]),
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
