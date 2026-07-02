// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Handshake Finland 2619839 — 2 gang smart push dimmer (z2m v26.77.0 parity).
// z2m-source: handshake_finland.ts #2619839.
//
// extend: [deviceEndpoints({endpoints: {l1: 1, l2: 2}}),
//          light({endpointNames: ["l1","l2"], configureReporting: true})].
// Two independent dimmable gangs: on/off (genOnOff 0x0006) + brightness
// (genLevelCtrl 0x0008), no colour. `endpoint_map` suffixes every emitted
// key per gang (state -> state_l1/state_l2, brightness -> brightness_l1/_l2)
// and routes outbound Tz frames back to the matching endpoint. All decode
// via the shared generic on/off + brightness converters; z2m matches by
// zigbeeModel "AGGE Zigbee 2 gang smart push dimmer" (modelID alone).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::handshake_finland {
namespace {

// z2m modernExtend light() core = fz.on_off + fz.brightness (no colour).
const FzConverter* const kFz_2619839[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};

// z2m modernExtend light() core = tz.light_onoff_brightness.
const TzConverter* const kTz_2619839[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};

constexpr const char* kModels_2619839[] = { "AGGE Zigbee 2 gang smart push dimmer" };

// deviceEndpoints({endpoints: {l1: 1, l2: 2}}) → per-gang key suffixing.
constexpr ::zhc::EndpointLabel kEndpoints_2619839[] = { {"l1", 1}, {"l2", 2} };

// Two dimmable gangs. state_* = on/off (Binary), brightness_* = 0-254 raw
// level (Numeric, unitless per z2m). Access::StateSet — readable + settable.
constexpr Expose kExposes_2619839[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

// z2m light({configureReporting: true}) binds genOnOff + genLevelCtrl per gang.
constexpr BindingSpec kBindings_2619839[] = {
    {1, 0x0006},  // l1 genOnOff
    {1, 0x0008},  // l1 genLevelCtrl
    {2, 0x0006},  // l2 genOnOff
    {2, 0x0008},  // l2 genLevelCtrl
};

// configureReporting: true → onOff (bool 0x10) + currentLevel (u8 0x20) reports
// on each gang endpoint.
constexpr ReportingSpec kReports_2619839[] = {
    { 1, 0x0006, 0x0000, 0x10, 0, 3600, 1, 0 },  // l1 genOnOff onOff
    { 1, 0x0008, 0x0000, 0x20, 5, 3600, 1, 0 },  // l1 genLevelCtrl currentLevel
    { 2, 0x0006, 0x0000, 0x10, 0, 3600, 1, 0 },  // l2 genOnOff onOff
    { 2, 0x0008, 0x0000, 0x20, 5, 3600, 1, 0 },  // l2 genLevelCtrl currentLevel
};

}  // namespace

extern const PreparedDefinition kDef_2619839{
    .zigbee_models=kModels_2619839, .zigbee_models_count=sizeof(kModels_2619839)/sizeof(kModels_2619839[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="2619839", .vendor="Handshake Finland",
    .meta=nullptr, .exposes=kExposes_2619839, .exposes_count=sizeof(kExposes_2619839)/sizeof(kExposes_2619839[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_2619839, .from_zigbee_count=sizeof(kFz_2619839)/sizeof(kFz_2619839[0]),
    .to_zigbee=kTz_2619839, .to_zigbee_count=sizeof(kTz_2619839)/sizeof(kTz_2619839[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_2619839, .bindings_count=sizeof(kBindings_2619839)/sizeof(kBindings_2619839[0]),
    .reports=kReports_2619839, .reports_count=sizeof(kReports_2619839)/sizeof(kReports_2619839[0]),
    .endpoint_map       = kEndpoints_2619839,
    .endpoint_map_count = sizeof(kEndpoints_2619839)/sizeof(kEndpoints_2619839[0]),
};

}  // namespace zhc::devices::handshake_finland
