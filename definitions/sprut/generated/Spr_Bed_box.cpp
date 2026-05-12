// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sprut.device Bed.box — uses shared sprut converters.
// Sprut bed.box ergomotion controller.
// z2m-source: zigbee-herdsman-converters/src/devices/sprut.ts #Bed.box
//
// 17 named endpoints. Each endpoint surfaces either a `state` (OnOff)
// toggle, a `presentValue` numeric (positions / massage intensities /
// duration), or a STATE_GET feedback. The endpoint_map drives suffix
// rewrite on Fz and suffix-strip on Tz so a single base-key converter
// fans out across all matching endpoints.

#include "definitions/_generic/_shared.hpp"
#include "definitions/sprut/_shared.hpp"

namespace zhc::devices::sprut {
namespace {

// Writable presentValue (genMultistateInput attr 0x0055, u16). One
// generic write-attr Tz handles every writable multistate endpoint —
// the adapter strips the `_<endpoint-label>` suffix before dispatch
// and routes to the matching EP.
constexpr ::zhc::generic::ZclWriteSpec kSpec_PresentValue{
    "presentValue",
    /*attr_id=*/0x0055,
    /*attr_type=*/0x21,                 // UINT16
    /*manufacturer_code=*/0,
    nullptr, 0,
};
constexpr TzConverter kTz_PresentValue{
    .key         = "presentValue",
    .cluster     = "genMultistateInput",
    .cluster_id  = 0x0012,
    .command_id  = 0x02,                // writeAttributes
    .fn          = ::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_PresentValue,
};

const FzConverter* const kFz_Bed_box[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::sprut::kFzSprutMultistateInput,
    &::zhc::sprut::kFzSprutMultistateOutput,
};
const TzConverter* const kTz_Bed_box[] = {
    &::zhc::generic::kTzOnOff,
    &kTz_PresentValue,
};
constexpr const char* kModels_Bed_box[] = { "Bed.box" };

constexpr ::zhc::EndpointLabel kEndpoints_Bed_box[] = {
    {"light", 1}, {"head_control", 2}, {"foot_control", 3},
    {"head_massage_intensity", 4}, {"foot_massage_intensity", 5},
    {"massage_duration", 6}, {"flat", 7}, {"zero_gravity", 8},
    {"reading", 9}, {"tv", 10}, {"head_feedback", 11},
    {"foot_feedback", 12}, {"clear_angles_settings", 16},
    {"massage", 21}, {"anti_snoring", 24}, {"move", 26}, {"stop", 27},
};

constexpr Expose kAutoExposes[] = {
    // 10 OnOff endpoints surface as "state_<label>" via endpoint_map
    // suffix rewrite. Listed once with the base key — the SPA reads
    // them per-EP off the shadow.
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // Multistate writable percent / enum reads on input cluster.
    {"presentValue", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    // OnOff endpoints
    {1,  0x0006}, {7,  0x0006}, {8,  0x0006}, {9,  0x0006},
    {10, 0x0006}, {16, 0x0006}, {21, 0x0006}, {24, 0x0006},
    {26, 0x0006}, {27, 0x0006},
    // genMultistateInput (positions, intensities, duration)
    {2, 0x0012}, {3, 0x0012}, {4, 0x0012}, {5, 0x0012}, {6, 0x0012},
    // genMultistateOutput (head/foot feedback)
    {11, 0x0013}, {12, 0x0013},
};

}  // namespace

extern const PreparedDefinition kDef_Bed_box{
    .zigbee_models=kModels_Bed_box,
    .zigbee_models_count=sizeof(kModels_Bed_box)/sizeof(kModels_Bed_box[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Bed.box", .vendor="Sprut",
    .meta=nullptr,
    .exposes=kAutoExposes,
    .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Bed_box,
    .from_zigbee_count=sizeof(kFz_Bed_box)/sizeof(kFz_Bed_box[0]),
    .to_zigbee=kTz_Bed_box,
    .to_zigbee_count=sizeof(kTz_Bed_box)/sizeof(kTz_Bed_box[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,
    .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_Bed_box,
    .endpoint_map_count = sizeof(kEndpoints_Bed_box)/sizeof(kEndpoints_Bed_box[0]),
};

}  // namespace zhc::devices::sprut
