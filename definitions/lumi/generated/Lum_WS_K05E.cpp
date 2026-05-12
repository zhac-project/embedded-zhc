// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi WS-K05E — auto-generated.
// Dimmer Switch H2 US
// z2m-source: lumi.ts #WS-K05E.
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


const TzConverter* const kTz_WS_K05E[] = {
    &::zhc::lumi::kTzLumiPowerOutageMemory,
    &::zhc::lumi::kTzLumiLedDisabledNight,
    &::zhc::lumi::kTzLumiButtonLock,
};
constexpr const char* kModels_WS_K05E[] = { "lumi.switch.agl007" };

constexpr ::zhc::EndpointLabel kEndpoints_WS_K05E[] = { {"power", 1}, {"bright", 2}, {"dim", 3} };

}  // namespace

namespace {
const FzConverter* const kFz_min_WS_K05E[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
    &::zhc::lumi::kFzLumiManuSpecific,
};
constexpr Expose kExp_min_WS_K05E[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_WS_K05E[] = { { 1, 0x0001 }, { 1, 0x0006 } { 1, 0xFCC0 } };
}  // namespace
extern const PreparedDefinition kDef_WS_K05E{
    .zigbee_models=kModels_WS_K05E, .zigbee_models_count=sizeof(kModels_WS_K05E)/sizeof(kModels_WS_K05E[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WS-K05E", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_WS_K05E, .exposes_count=sizeof(kExp_min_WS_K05E)/sizeof(kExp_min_WS_K05E[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_WS_K05E, .from_zigbee_count=sizeof(kFz_min_WS_K05E)/sizeof(kFz_min_WS_K05E[0]),
    .to_zigbee=kTz_WS_K05E, .to_zigbee_count=sizeof(kTz_WS_K05E)/sizeof(kTz_WS_K05E[0]),
    .configure=nullptr, .on_event=nullptr,
    .endpoint_map       = kEndpoints_WS_K05E,
    .endpoint_map_count = sizeof(kEndpoints_WS_K05E)/sizeof(kEndpoints_WS_K05E[0]),
};

}  // namespace zhc::devices::lumi
