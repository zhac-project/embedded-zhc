// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ls 756200030 — hand-ported (LS sweep, 2026-04-29c).
// Mec Driver module 4-channel Zigbee (12V)
// z2m-source: ls.ts #756200030.
//
// z2m fingerprint match: modelID= + softwareBuildID
//   in {"3.12.25-026", "4.09.03-027"}. ZHC matches on modelID alone;
//   the same modelID is also used by 756200027 (1-channel), so the
//   4-channel binding/expose surface lands as a strict superset and
//   the 1-channel device's exposes/bindings remain a no-op subset.
//
// z2m extend chain:
//   m.deviceEndpoints({endpoints: {11:11,12:12,13:13,14:14}})
//   m.light({endpointNames:["11","12","13","14"], colorTemp:{range:[153,500]}})
//
// ZHC mirrors this with an `endpoint_map` of four labels (l11..l14)
// pointing at EPs 11..14, and per-EP bindings for the three light
// clusters. The dispatcher rewrites bare `state` / `brightness` /
// `color_temp` keys into `state_l<N>` / `brightness_l<N>` /
// `color_temp_l<N>` based on the source endpoint, matching z2m's
// `withEndpoint("${ep}")` shape.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ls {
namespace {
const FzConverter* const kFz_D756200030[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_D756200030[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_D756200030[] = { "Mec Driver module" };

constexpr ::zhc::EndpointLabel kEndpoints_D756200030[] = {
    {"l11", 11}, {"l12", 12}, {"l13", 13}, {"l14", 14},
};

constexpr Expose kExposes_D756200030[] = {
    {"state_l11",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"brightness_l11", ExposeType::Numeric, Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"color_temp_l11", ExposeType::Numeric, Access::StateSet, "mired", "153..500", nullptr, 0},
    {"state_l12",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"brightness_l12", ExposeType::Numeric, Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"color_temp_l12", ExposeType::Numeric, Access::StateSet, "mired", "153..500", nullptr, 0},
    {"state_l13",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"brightness_l13", ExposeType::Numeric, Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"color_temp_l13", ExposeType::Numeric, Access::StateSet, "mired", "153..500", nullptr, 0},
    {"state_l14",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"brightness_l14", ExposeType::Numeric, Access::StateSet, nullptr, nullptr,    nullptr, 0},
    {"color_temp_l14", ExposeType::Numeric, Access::StateSet, "mired", "153..500", nullptr, 0},
};

constexpr BindingSpec kBind_D756200030[] = {
    {11, 0x0006}, {11, 0x0008}, {11, 0x0300},
    {12, 0x0006}, {12, 0x0008}, {12, 0x0300},
    {13, 0x0006}, {13, 0x0008}, {13, 0x0300},
    {14, 0x0006}, {14, 0x0008}, {14, 0x0300},
};

constexpr WhiteLabel kWhiteLabels_D756200030[] = {
    {"L&S Lighting", "756200031"},   // 24V Mec Driver 4ch variant
};

}  // namespace

extern const PreparedDefinition kDef_D756200030{
    .zigbee_models=kModels_D756200030, .zigbee_models_count=sizeof(kModels_D756200030)/sizeof(kModels_D756200030[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="756200030", .vendor="Ls",
    .meta=nullptr, .exposes=kExposes_D756200030, .exposes_count=sizeof(kExposes_D756200030)/sizeof(kExposes_D756200030[0]),
    .white_labels=kWhiteLabels_D756200030, .white_labels_count=sizeof(kWhiteLabels_D756200030)/sizeof(kWhiteLabels_D756200030[0]),
    .from_zigbee=kFz_D756200030, .from_zigbee_count=sizeof(kFz_D756200030)/sizeof(kFz_D756200030[0]),
    .to_zigbee=kTz_D756200030, .to_zigbee_count=sizeof(kTz_D756200030)/sizeof(kTz_D756200030[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_D756200030, .bindings_count=sizeof(kBind_D756200030)/sizeof(kBind_D756200030[0]),
    .endpoint_map       = kEndpoints_D756200030,
    .endpoint_map_count = sizeof(kEndpoints_D756200030)/sizeof(kEndpoints_D756200030[0]),
};

}  // namespace zhc::devices::ls
