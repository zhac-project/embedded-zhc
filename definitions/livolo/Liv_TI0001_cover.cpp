// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Livolo TI0001-cover — custom-cluster roller blind motor.
// Zigbee roller blind motor
// z2m-source: livolo.ts #TI0001-cover — fz.livolo_cover_state decodes raw
// genPowerCfg frames (dp 0x0c/0x0f): reportType 0x04 -> position
// (100-data[13]) + state OPEN/CLOSE + moving; 0x12 -> motor_speed;
// 0x13 -> motor_direction. The auto-port wired generic kFzOnOff +
// kFzCoverPosition (standard 0x0006/0x0102) which never match the raw
// frame. Wired the custom decoder. Cover *control* (livolo_cover_state /
// _position / _options) uses three bespoke manuSpec encoders — INFRA,
// deferred; exposes are read-only here.
#include "definitions/_generic/_shared.hpp"
#include "definitions/livolo/_shared.hpp"

namespace zhc::devices::livolo {
namespace {
const FzConverter* const kFz_TI0001_cover[] = {
    &::zhc::devices::livolo::kFzLivoloCoverState,
};
constexpr const char* kMotorDirection[] = { "FORWARD", "REVERSE" };
constexpr const char* kModels_TI0001_cover[] = { "TI0001-cover" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",           ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"position",        ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"moving",          ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"motor_speed",     ExposeType::Numeric, Access::State, "rpm",   nullptr, nullptr, 0},
    {"motor_direction", ExposeType::Enum,    Access::State, nullptr, nullptr, kMotorDirection, 2},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0102},
};

extern const PreparedDefinition kDef_TI0001_cover{
    .zigbee_models=kModels_TI0001_cover, .zigbee_models_count=sizeof(kModels_TI0001_cover)/sizeof(kModels_TI0001_cover[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TI0001-cover", .vendor="Livolo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TI0001_cover, .from_zigbee_count=sizeof(kFz_TI0001_cover)/sizeof(kFz_TI0001_cover[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::livolo
