// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Pushok POK012 — 20 dBm Zigbee router with battery backup.
// z2m-source: pushok.ts #POK012. The TS definition does NOT bind the IAS
// zone cluster; previous auto-emit added a spurious kFzIasZone — removed.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK012[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_POK012[] = { "POK012" };

}  // namespace


// --- hand-authored exposes block (z2m: battery, battery_state) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    // pushokExtend: battery_state — genMultistateInput(presentValue), STATE_GET.
    // Exposed for UI; backing FZ not yet wired.
    {"battery_state", ExposeType::Enum, Access::State, nullptr,
        "Battery state", nullptr, 0, ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_POK012{
    .zigbee_models=kModels_POK012, .zigbee_models_count=sizeof(kModels_POK012)/sizeof(kModels_POK012[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK012", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK012, .from_zigbee_count=sizeof(kFz_POK012)/sizeof(kFz_POK012[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
