// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Elko EKO05806 — hand-rewritten (was wrong-bundle: kFz/kTzOnOff).
// Elko ESH 316 Endevender RF (battery dimmer remote).
// z2m-source: elko.ts #EKO05806 — fz.command_toggle, fz.command_step;
//             toZigbee:[]; exposes: action(toggle, brightness_step_up,
//             brightness_step_down). Bind genOnOff + genLevelCtrl on ep1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::elko {
namespace {
const FzConverter* const kFz_EKO05806[] = {
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandStep,
};
constexpr const char* kModels_EKO05806[] = { "ElkoDimmerRemoteZHA" };

constexpr const char* kActionEnum_EKO05806[] = {
    "toggle",
    "brightness_step_up",
    "brightness_step_down",
};

constexpr Expose kExp_EKO05806[] = {
    {
        "action", ExposeType::Enum, Access::State,
        nullptr, nullptr,
        kActionEnum_EKO05806,
        sizeof(kActionEnum_EKO05806) / sizeof(kActionEnum_EKO05806[0]),
    },
};

constexpr BindingSpec kBind_EKO05806[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
};

}  // namespace

extern const PreparedDefinition kDef_EKO05806{
    .zigbee_models=kModels_EKO05806, .zigbee_models_count=sizeof(kModels_EKO05806)/sizeof(kModels_EKO05806[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EKO05806", .vendor="Elko",
    .meta=nullptr, .exposes=kExp_EKO05806, .exposes_count=sizeof(kExp_EKO05806)/sizeof(kExp_EKO05806[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EKO05806, .from_zigbee_count=sizeof(kFz_EKO05806)/sizeof(kFz_EKO05806[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_EKO05806, .bindings_count=sizeof(kBind_EKO05806)/sizeof(kBind_EKO05806[0]),
};

}  // namespace zhc::devices::elko
