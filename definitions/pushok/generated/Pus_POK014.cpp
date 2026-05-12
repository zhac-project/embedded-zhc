// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Pushok POK014 — external probe temperature sensor (k-type).
// z2m-source: pushok.ts #POK014 (uses pushokExtend.extendedTemperature()).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
// extendedTemperature in z2m reads measuredValue + a vendor offset 0xF001.
// Generic kFzTemperature handles measuredValue; the 0xF001 fine-offset is
// device-specific and not yet emulated here. Marked PARTIAL in PARITY doc.
const FzConverter* const kFz_POK014[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_POK014[] = { "POK014" };

}  // namespace


// --- hand-authored exposes block ---
constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C",
        "Measured temperature value", nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_POK014{
    .zigbee_models=kModels_POK014, .zigbee_models_count=sizeof(kModels_POK014)/sizeof(kModels_POK014[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK014", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK014, .from_zigbee_count=sizeof(kFz_POK014)/sizeof(kFz_POK014[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
