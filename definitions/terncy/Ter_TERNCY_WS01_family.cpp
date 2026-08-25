// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: TERNCY WS01 wall-switch family (z2m v26.98.0 parity).
//
// Seven models added upstream in v26.98.0, all one shape: N relay
// endpoints (l1..lN on ZCL endpoints 1..N) plus a shared button-action
// surface on the AduroSmart manufacturer cluster.
//
//   TERNCY-WS01-D1..D4  neutral-wire switches, 1..4 gang
//   TERNCY-WS01-S1..S3  no-neutral switches, 1..3 gang
//
// The S4 (4-gang no-neutral) already ships as `kDef_TERNCY_WS01` under
// generated/ and is NOT re-declared here.
//
// Relays wire to the stock generic on/off converters, suffixed per
// endpoint by the runtime through `endpoint_map`. Buttons decode via
// `kFzTerncyWs01Action` (see _shared.hpp) which emits `action` values
// carrying the endpoint (`single_l1`) plus `action_duration` on
// hold/release.
//
// DEFERRED: the per-gang config surface — `operation_mode`
// (control_relay / wireless), `wireless_led_status` (off / on),
// `led_feedback_mode` (positive / negative). z2m reads these from
// manufacturer-specific attributes cfgSwPureInput (0x001C, BOOLEAN),
// cfgButtonLedStatus (0x0020, UINT8) and cfgButtonLedPolarity (0x001F,
// UINT8) on cluster 0xFCCC, and writes them back through custom cluster
// COMMANDS (enablePureInput 0x1D, setButtonLedStatus 0x1F,
// setSwitchPolarity 0x1E) rather than attribute writes. There is no
// ::zhc::generic counterpart for a manufacturer-specific command-write
// tz, so the config trio waits for a dedicated terncy switch codec. The
// primary function (relays + buttons) is fully covered.
//
// z2m-source: terncy.ts #TERNCY-WS01-D1 .. #TERNCY-WS01-S3.
#include "definitions/_generic/_shared.hpp"
#include "definitions/terncy/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_WS01[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::terncy::kFzTerncyWs01Action,
};
const TzConverter* const kTz_WS01[] = {
    &::zhc::generic::kTzOnOff,
};

}  // namespace


// ---- TERNCY-WS01-D1 ---------------------------------------------------
namespace {
constexpr const char* kModels_TERNCY_WS01_D1[] = { "TERNCY-WS01-D1" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01_D1[] = {
    {"l1", 1},
};

constexpr Expose kExposes_TERNCY_WS01_D1[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",          ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TERNCY_WS01_D1[] = {
    {1, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01_D1{
    .zigbee_models=kModels_TERNCY_WS01_D1,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01_D1)/sizeof(kModels_TERNCY_WS01_D1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01-D1", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_TERNCY_WS01_D1, .exposes_count=sizeof(kExposes_TERNCY_WS01_D1)/sizeof(kExposes_TERNCY_WS01_D1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=kTz_WS01, .to_zigbee_count=sizeof(kTz_WS01)/sizeof(kTz_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TERNCY_WS01_D1, .bindings_count=sizeof(kBindings_TERNCY_WS01_D1)/sizeof(kBindings_TERNCY_WS01_D1[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01_D1,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01_D1)/sizeof(kEndpoints_TERNCY_WS01_D1[0]),
};


// ---- TERNCY-WS01-D2 ---------------------------------------------------
namespace {
constexpr const char* kModels_TERNCY_WS01_D2[] = { "TERNCY-WS01-D2" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01_D2[] = {
    {"l1", 1},
    {"l2", 2},
};

constexpr Expose kExposes_TERNCY_WS01_D2[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",          ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TERNCY_WS01_D2[] = {
    {1, 0x0006},   // genOnOff
    {2, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01_D2{
    .zigbee_models=kModels_TERNCY_WS01_D2,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01_D2)/sizeof(kModels_TERNCY_WS01_D2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01-D2", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_TERNCY_WS01_D2, .exposes_count=sizeof(kExposes_TERNCY_WS01_D2)/sizeof(kExposes_TERNCY_WS01_D2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=kTz_WS01, .to_zigbee_count=sizeof(kTz_WS01)/sizeof(kTz_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TERNCY_WS01_D2, .bindings_count=sizeof(kBindings_TERNCY_WS01_D2)/sizeof(kBindings_TERNCY_WS01_D2[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01_D2,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01_D2)/sizeof(kEndpoints_TERNCY_WS01_D2[0]),
};


// ---- TERNCY-WS01-D3 ---------------------------------------------------
namespace {
constexpr const char* kModels_TERNCY_WS01_D3[] = { "TERNCY-WS01-D3" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01_D3[] = {
    {"l1", 1},
    {"l2", 2},
    {"l3", 3},
};

constexpr Expose kExposes_TERNCY_WS01_D3[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",          ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TERNCY_WS01_D3[] = {
    {1, 0x0006},   // genOnOff
    {2, 0x0006},   // genOnOff
    {3, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01_D3{
    .zigbee_models=kModels_TERNCY_WS01_D3,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01_D3)/sizeof(kModels_TERNCY_WS01_D3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01-D3", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_TERNCY_WS01_D3, .exposes_count=sizeof(kExposes_TERNCY_WS01_D3)/sizeof(kExposes_TERNCY_WS01_D3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=kTz_WS01, .to_zigbee_count=sizeof(kTz_WS01)/sizeof(kTz_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TERNCY_WS01_D3, .bindings_count=sizeof(kBindings_TERNCY_WS01_D3)/sizeof(kBindings_TERNCY_WS01_D3[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01_D3,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01_D3)/sizeof(kEndpoints_TERNCY_WS01_D3[0]),
};


// ---- TERNCY-WS01-D4 ---------------------------------------------------
namespace {
constexpr const char* kModels_TERNCY_WS01_D4[] = { "TERNCY-WS01-D4" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01_D4[] = {
    {"l1", 1},
    {"l2", 2},
    {"l3", 3},
    {"l4", 4},
};

constexpr Expose kExposes_TERNCY_WS01_D4[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l4",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",          ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TERNCY_WS01_D4[] = {
    {1, 0x0006},   // genOnOff
    {2, 0x0006},   // genOnOff
    {3, 0x0006},   // genOnOff
    {4, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01_D4{
    .zigbee_models=kModels_TERNCY_WS01_D4,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01_D4)/sizeof(kModels_TERNCY_WS01_D4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01-D4", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_TERNCY_WS01_D4, .exposes_count=sizeof(kExposes_TERNCY_WS01_D4)/sizeof(kExposes_TERNCY_WS01_D4[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=kTz_WS01, .to_zigbee_count=sizeof(kTz_WS01)/sizeof(kTz_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TERNCY_WS01_D4, .bindings_count=sizeof(kBindings_TERNCY_WS01_D4)/sizeof(kBindings_TERNCY_WS01_D4[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01_D4,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01_D4)/sizeof(kEndpoints_TERNCY_WS01_D4[0]),
};


// ---- TERNCY-WS01-S1 ---------------------------------------------------
namespace {
constexpr const char* kModels_TERNCY_WS01_S1[] = { "TERNCY-WS01-S1" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01_S1[] = {
    {"l1", 1},
};

constexpr Expose kExposes_TERNCY_WS01_S1[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",          ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TERNCY_WS01_S1[] = {
    {1, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01_S1{
    .zigbee_models=kModels_TERNCY_WS01_S1,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01_S1)/sizeof(kModels_TERNCY_WS01_S1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01-S1", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_TERNCY_WS01_S1, .exposes_count=sizeof(kExposes_TERNCY_WS01_S1)/sizeof(kExposes_TERNCY_WS01_S1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=kTz_WS01, .to_zigbee_count=sizeof(kTz_WS01)/sizeof(kTz_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TERNCY_WS01_S1, .bindings_count=sizeof(kBindings_TERNCY_WS01_S1)/sizeof(kBindings_TERNCY_WS01_S1[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01_S1,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01_S1)/sizeof(kEndpoints_TERNCY_WS01_S1[0]),
};


// ---- TERNCY-WS01-S2 ---------------------------------------------------
namespace {
constexpr const char* kModels_TERNCY_WS01_S2[] = { "TERNCY-WS01-S2" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01_S2[] = {
    {"l1", 1},
    {"l2", 2},
};

constexpr Expose kExposes_TERNCY_WS01_S2[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",          ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TERNCY_WS01_S2[] = {
    {1, 0x0006},   // genOnOff
    {2, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01_S2{
    .zigbee_models=kModels_TERNCY_WS01_S2,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01_S2)/sizeof(kModels_TERNCY_WS01_S2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01-S2", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_TERNCY_WS01_S2, .exposes_count=sizeof(kExposes_TERNCY_WS01_S2)/sizeof(kExposes_TERNCY_WS01_S2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=kTz_WS01, .to_zigbee_count=sizeof(kTz_WS01)/sizeof(kTz_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TERNCY_WS01_S2, .bindings_count=sizeof(kBindings_TERNCY_WS01_S2)/sizeof(kBindings_TERNCY_WS01_S2[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01_S2,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01_S2)/sizeof(kEndpoints_TERNCY_WS01_S2[0]),
};


// ---- TERNCY-WS01-S3 ---------------------------------------------------
namespace {
constexpr const char* kModels_TERNCY_WS01_S3[] = { "TERNCY-WS01-S3" };

constexpr ::zhc::EndpointLabel kEndpoints_TERNCY_WS01_S3[] = {
    {"l1", 1},
    {"l2", 2},
    {"l3", 3},
};

constexpr Expose kExposes_TERNCY_WS01_S3[] = {
    {"state_l1",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l3",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",          ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"action_duration", ExposeType::Numeric, Access::State, "s",     nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_TERNCY_WS01_S3[] = {
    {1, 0x0006},   // genOnOff
    {2, 0x0006},   // genOnOff
    {3, 0x0006},   // genOnOff
};
}  // namespace

extern const PreparedDefinition kDef_TERNCY_WS01_S3{
    .zigbee_models=kModels_TERNCY_WS01_S3,
    .zigbee_models_count=sizeof(kModels_TERNCY_WS01_S3)/sizeof(kModels_TERNCY_WS01_S3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-WS01-S3", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_TERNCY_WS01_S3, .exposes_count=sizeof(kExposes_TERNCY_WS01_S3)/sizeof(kExposes_TERNCY_WS01_S3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=kTz_WS01, .to_zigbee_count=sizeof(kTz_WS01)/sizeof(kTz_WS01[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TERNCY_WS01_S3, .bindings_count=sizeof(kBindings_TERNCY_WS01_S3)/sizeof(kBindings_TERNCY_WS01_S3[0]),
    .endpoint_map       = kEndpoints_TERNCY_WS01_S3,
    .endpoint_map_count = sizeof(kEndpoints_TERNCY_WS01_S3)/sizeof(kEndpoints_TERNCY_WS01_S3[0]),
};


}  // namespace zhc::devices::terncy
