// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy QUAD-ZIG-SW — auto-generated.
// FUGA compatible switch from Smarthjemmet.dk
// z2m-source: custom_devices_diy.ts #QUAD-ZIG-SW.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: same converters as MULTI-ZIG-SW (fzLocal.multi_zig_sw_*, tzLocal.multi_zig_sw_*).
// Different physical switch (FUGA-compatible 4-button) but identical Zigbee surface.
// TODO(parity): same as MULTI-ZIG-SW - per-EP genMultistateInput action + switch_type
// enums require custom fz/tz converters not in shared helpers.
const FzConverter* const kFz_QUAD_ZIG_SW[] = {
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_QUAD_ZIG_SW[] = { "QUAD-ZIG-SW" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {2, 0x0007}, {3, 0x0007}, {4, 0x0007}, {5, 0x0007},
    {2, 0x0012}, {3, 0x0012}, {4, 0x0012}, {5, 0x0012},
};

extern const PreparedDefinition kDef_QUAD_ZIG_SW{
    .zigbee_models=kModels_QUAD_ZIG_SW, .zigbee_models_count=sizeof(kModels_QUAD_ZIG_SW)/sizeof(kModels_QUAD_ZIG_SW[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="QUAD-ZIG-SW", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_QUAD_ZIG_SW, .from_zigbee_count=sizeof(kFz_QUAD_ZIG_SW)/sizeof(kFz_QUAD_ZIG_SW[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::custom_devices_diy
