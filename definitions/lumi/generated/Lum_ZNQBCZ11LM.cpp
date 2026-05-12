// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi ZNQBCZ11LM — auto-generated.
// Smart wall outlet H2 (USB)
// z2m-source: lumi.ts #ZNQBCZ11LM.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_ZNQBCZ11LM[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_ZNQBCZ11LM[] = { "lumi.plug.acn005" };

constexpr ::zhc::EndpointLabel kEndpoints_ZNQBCZ11LM[] = { {"relay", 1}, {"usb", 2} };

}  // namespace

namespace {
const FzConverter* const kFz_min_ZNQBCZ11LM[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_ZNQBCZ11LM[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZNQBCZ11LM[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_ZNQBCZ11LM{
    .zigbee_models=kModels_ZNQBCZ11LM, .zigbee_models_count=sizeof(kModels_ZNQBCZ11LM)/sizeof(kModels_ZNQBCZ11LM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZNQBCZ11LM", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_ZNQBCZ11LM, .exposes_count=sizeof(kExp_min_ZNQBCZ11LM)/sizeof(kExp_min_ZNQBCZ11LM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZNQBCZ11LM, .from_zigbee_count=sizeof(kFz_min_ZNQBCZ11LM)/sizeof(kFz_min_ZNQBCZ11LM[0]),
    .to_zigbee=kTz_ZNQBCZ11LM, .to_zigbee_count=sizeof(kTz_ZNQBCZ11LM)/sizeof(kTz_ZNQBCZ11LM[0]),
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_ZNQBCZ11LM,
    .endpoint_map_count = sizeof(kEndpoints_ZNQBCZ11LM)/sizeof(kEndpoints_ZNQBCZ11LM[0]),
};

}  // namespace zhc::devices::lumi
