// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lellki WK34-EU — metering channel restored.
// Power socket EU (with power monitoring)
//
// z2m uses tuya.modernExtend.tuyaOnOff({electricalMeasurements: true}) which
// pushes fz.electrical_measurement (haElectricalMeasurement 0x0B04) + fz.metering
// (seMetering 0x0702) and exposes power/current/voltage/energy. The auto port
// was lowered to bare on/off, dropping the entire metering half. Restore both
// generic converters + the four numeric exposes + the 0x0702/0x0B04 binds.
//
// z2m-source: lellki.ts #WK34-EU; lib/tuya.ts tuyaOnOff(electricalMeasurements).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lellki {
namespace {
const FzConverter* const kFz_WK34_EU[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_WK34_EU[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_WK34_EU[] = { "TS011F" };
constexpr const char* kManus_WK34_EU[] = { "_TZ3000_0yxeawjt" };
}  // namespace


// --- Tier 2: on/off + metering (seMetering 0x0702 + haElectricalMeasurement 0x0B04) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end block ---

extern const PreparedDefinition kDef_WK34_EU{
    .zigbee_models=kModels_WK34_EU, .zigbee_models_count=sizeof(kModels_WK34_EU)/sizeof(kModels_WK34_EU[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_WK34_EU, .manufacturer_names_count=sizeof(kManus_WK34_EU)/sizeof(kManus_WK34_EU[0]),
    .model="WK34-EU", .vendor="Lellki",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WK34_EU, .from_zigbee_count=sizeof(kFz_WK34_EU)/sizeof(kFz_WK34_EU[0]),
    .to_zigbee=kTz_WK34_EU, .to_zigbee_count=sizeof(kTz_WK34_EU)/sizeof(kTz_WK34_EU[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lellki
