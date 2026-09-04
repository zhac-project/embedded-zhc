// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Lincukoo PZE2612 outdoor smart plug, two sockets
// (zigbeeModel "CK-BL702-MSW-01(7011)"; z2m v26.105.0, #13076).
//
// Plain genOnOff on endpoints 1 and 2; upstream names them "1" and "2", so
// the keys are `state_1` / `state_2`. No power-on behaviour upstream.
// z2m-source: lincukoo.ts #PZE2612.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lincukoo {
namespace {
const FzConverter* const kFz_PZE2612[] = { &::zhc::generic::kFzOnOff };
const TzConverter* const kTz_PZE2612[] = { &::zhc::generic::kTzOnOff };
constexpr const char* kModels_PZE2612[] = { "CK-BL702-MSW-01(7011)" };
constexpr ::zhc::EndpointLabel kEndpoints_PZE2612[] = { {"1", 1}, {"2", 2} };
constexpr Expose kExp_PZE2612[] = {
    {"state_1", ExposeType::Binary, Access::StateSet, nullptr, "Socket 1", nullptr, 0},
    {"state_2", ExposeType::Binary, Access::StateSet, nullptr, "Socket 2", nullptr, 0},
};
constexpr BindingSpec kBind_PZE2612[] = { {1, 0x0006}, {2, 0x0006} };
}  // namespace

extern const PreparedDefinition kDef_PZE2612{
    .zigbee_models=kModels_PZE2612, .zigbee_models_count=sizeof(kModels_PZE2612)/sizeof(kModels_PZE2612[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PZE2612", .vendor="Lincukoo",
    .meta=nullptr, .exposes=kExp_PZE2612, .exposes_count=sizeof(kExp_PZE2612)/sizeof(kExp_PZE2612[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PZE2612, .from_zigbee_count=sizeof(kFz_PZE2612)/sizeof(kFz_PZE2612[0]),
    .to_zigbee=kTz_PZE2612, .to_zigbee_count=sizeof(kTz_PZE2612)/sizeof(kTz_PZE2612[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_PZE2612, .bindings_count=sizeof(kBind_PZE2612)/sizeof(kBind_PZE2612[0]),
    .endpoint_map       = kEndpoints_PZE2612,
    .endpoint_map_count = sizeof(kEndpoints_PZE2612)/sizeof(kEndpoints_PZE2612[0]),
};

}  // namespace zhc::devices::lincukoo
