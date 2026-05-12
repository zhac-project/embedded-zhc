// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Nodon SDC-4-1-00 — hand-rewrite (added dry_contact).
// Dry contact sensor.
//
// z2m: `m.battery({voltageReporting:true}) + nodonModernExtend.dryContact()`.
// `dryContact` is an `enumLookup` over `genBinaryInput` attr 0x0055
// → "contact_closed" / "contact_open". Wire `kFzNodonDryContact`
// (defined in nodon/_shared.cpp) to surface the value.
//
// z2m-source: nodon.ts #SDC-4-1-00.
#include "definitions/_generic/_shared.hpp"
#include "definitions/nodon/_shared.hpp"

namespace zhc::devices::nodon {
namespace {
const FzConverter* const kFz_SDC_4_1_00[] = {
    &::zhc::generic::kFzBattery,
    &kFzNodonDryContact,
};

constexpr const char* kModels_SDC_4_1_00[] = { "SDC-4-1-00" };

}  // namespace


// Hand-aligned to z2m exposes (added `dry_contact`).
constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"dry_contact", ExposeType::Enum,    Access::State, nullptr,
        "State of the contact, closed or open.", nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x000F},   // genBinaryInput
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_SDC_4_1_00{
    .zigbee_models=kModels_SDC_4_1_00, .zigbee_models_count=sizeof(kModels_SDC_4_1_00)/sizeof(kModels_SDC_4_1_00[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SDC-4-1-00", .vendor="Nodon",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SDC_4_1_00, .from_zigbee_count=sizeof(kFz_SDC_4_1_00)/sizeof(kFz_SDC_4_1_00[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::nodon
