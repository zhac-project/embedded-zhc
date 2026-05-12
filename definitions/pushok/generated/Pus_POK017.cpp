// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Pushok POK017 — battery powered greenhouse vent.
// z2m-source: pushok.ts #POK017. Battery + vendor multistate enums; previous
// auto-emit incorrectly bundled a coverPosition transport.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK017[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_POK017[] = { "POK017" };

}  // namespace


// --- hand-authored exposes block (z2m: battery + heat_sensitivity enum +
//     status enum). The two vendor enums sit on genMultistateOutput and
//     genMultistateInput respectively; exposed for UI. ---
constexpr Expose kAutoExposes[] = {
    {"heat_sensitivity", ExposeType::Enum, Access::StateSet, nullptr,
        "Heat sensitivity level for automatic vent opening", nullptr, 0,
        ExposeCategory::Config},
    {"status", ExposeType::Enum, Access::State, nullptr,
        "Actual window status", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_POK017{
    .zigbee_models=kModels_POK017, .zigbee_models_count=sizeof(kModels_POK017)/sizeof(kModels_POK017[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK017", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK017, .from_zigbee_count=sizeof(kFz_POK017)/sizeof(kFz_POK017[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
