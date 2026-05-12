// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Diyruz DIYRuZ_rspm — hand-extended 2026-04-28.
// DIYRuZ relay switch power meter — single-EP firmware that publishes
// on/off + a `fz.diyruz_rspm` custom report carrying power/current/CPU
// temperature/action. z2m default endpoint is 8.
// z2m-source: diyruz.ts #DIYRuZ_rspm.
// PARTIAL: state/state-write wired via generic. fz.diyruz_rspm is custom
// (manuSpec attrs on a non-haElectricalMeasurement cluster) and BLOCKED;
// power/current/cpu_temperature/action exposes declared so the UI shape
// matches z2m, but reports drop until a diyruz/_shared.cpp lands.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::diyruz {
namespace {
const FzConverter* const kFz_DIYRuZ_rspm[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_DIYRuZ_rspm[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_DIYRuZ_rspm[] = { "DIYRuZ_rspm" };

constexpr ::zhc::EndpointLabel kEndpoints_DIYRuZ_rspm[] = { {"default", 8} };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state",           ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power",           ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"current",         ExposeType::Numeric, Access::State,    "A",     nullptr, nullptr, 0},
    {"cpu_temperature", ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"action",          ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {8, 0x0006},
};

extern const PreparedDefinition kDef_DIYRuZ_rspm{
    .zigbee_models=kModels_DIYRuZ_rspm, .zigbee_models_count=sizeof(kModels_DIYRuZ_rspm)/sizeof(kModels_DIYRuZ_rspm[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_rspm", .vendor="Diyruz",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIYRuZ_rspm, .from_zigbee_count=sizeof(kFz_DIYRuZ_rspm)/sizeof(kFz_DIYRuZ_rspm[0]),
    .to_zigbee=kTz_DIYRuZ_rspm, .to_zigbee_count=sizeof(kTz_DIYRuZ_rspm)/sizeof(kTz_DIYRuZ_rspm[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_DIYRuZ_rspm,
    .endpoint_map_count = sizeof(kEndpoints_DIYRuZ_rspm)/sizeof(kEndpoints_DIYRuZ_rspm[0]),
};

}  // namespace zhc::devices::diyruz
