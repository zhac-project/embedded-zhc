// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: uses shared stelpro converter (peak-demand-icon Tz).
// Stello STLO-23 — Hilo water heater controller.
// z2m-source: stello.ts #STLO-23. onOff + seMetering "metering" cluster
// extend; mfgcode 0x1185 peak-demand-icon (also lifted from stelpro).
// Note: z2m flags an unimplemented manuSpec FC02 cluster
// (CCRDureeSalubre/Salubre/TempEau/TempFc) — same gap here.
#include "definitions/_generic/_shared.hpp"
#include "definitions/stelpro/_shared.hpp"

namespace zhc::devices::stello {
namespace {
const FzConverter* const kFz_STLO_23[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
};
const TzConverter* const kTz_STLO_23[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::stelpro::kTzStelproPeakDemandIcon,
};
constexpr const char* kModels_STLO_23[] = { "STLO-23" };

}  // namespace


// --- hand-authored exposes block (z2m-parity for STLO-23). ---
constexpr Expose kAutoExposes[] = {
    {"state",            ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy",           ExposeType::Numeric, Access::State,    "kWh",   nullptr, nullptr, 0},
    {"power",            ExposeType::Numeric, Access::State,    "W",     nullptr, nullptr, 0},
    {"peak_demand_icon", ExposeType::Numeric, Access::Set,      "hours",
        "Set peak demand event icon for the specified number of hours", nullptr, 0,
        ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0702},  // seMetering
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_STLO_23{
    .zigbee_models=kModels_STLO_23, .zigbee_models_count=sizeof(kModels_STLO_23)/sizeof(kModels_STLO_23[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STLO-23", .vendor="Stello",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STLO_23, .from_zigbee_count=sizeof(kFz_STLO_23)/sizeof(kFz_STLO_23[0]),
    .to_zigbee=kTz_STLO_23, .to_zigbee_count=sizeof(kTz_STLO_23)/sizeof(kTz_STLO_23[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::stello
