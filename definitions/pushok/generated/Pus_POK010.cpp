// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Pushok POK010 — water level + temperature + humidity sensor.
// z2m-source: pushok.ts #POK010. Contact comes from m.binary on
// genBinaryInput(presentValue) — NOT IAS-zone. Previous port wrongly
// bundled kFzIasZone + IAS sub-attrs (tamper/battery_low) and dropped
// the m.humidity({access: STATE}) expose. Fixed.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::pushok {
namespace {
const FzConverter* const kFz_POK010[] = {
    // contact lives on genBinaryInput(presentValue); no generic FZ wired yet.
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_POK010[] = { "POK010" };

}  // namespace


// --- hand-authored exposes block (z2m: m.binary{genBinaryInput contact},
//     m.temperature, m.humidity{STATE}, m.battery,
//     pushokExtend.pulseCounter,
//     m.enumLookup{operating_mode on genMultistateInput}) ---
constexpr Expose kAutoExposes[] = {
    {"contact",     ExposeType::Binary,  Access::State, nullptr,
        "Indicates if the contact is closed (= true) or open (= false)", nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    // pushokExtend.pulseCounter — vendor counter on genMultistateValue.
    // Exposed for UI; specialised FZ/TZ not yet wired in static library.
    {"pulse_counter", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    // pushokExtend operating_mode — genMultistateInput(presentValue),
    // {contact:1, counter:2}.
    {"operating_mode", ExposeType::Enum, Access::StateSet, nullptr,
        "Operating mode: contact sensor or pulse counter", nullptr, 0,
        ExposeCategory::Config},
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x000F},  // genBinaryInput (contact)
};
// --- end hand-authored block ---

extern const PreparedDefinition kDef_POK010{
    .zigbee_models=kModels_POK010, .zigbee_models_count=sizeof(kModels_POK010)/sizeof(kModels_POK010[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="POK010", .vendor="Pushok",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_POK010, .from_zigbee_count=sizeof(kFz_POK010)/sizeof(kFz_POK010[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::pushok
