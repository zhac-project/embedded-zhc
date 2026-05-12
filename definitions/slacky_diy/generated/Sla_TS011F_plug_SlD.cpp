// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy TS011F_plug-SlD — auto-generated.
// Plug with power monitoring
// z2m-source: slacky_diy.ts #TS011F_plug-SlD.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {
const FzConverter* const kFz_TS011F_plug_SlD[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzElectricalMeasurement,
};
const TzConverter* const kTz_TS011F_plug_SlD[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_TS011F_plug_SlD[] = { "TS011F-SlD" };

}  // namespace


// z2m wires m.onOff({powerOnBehavior}) + m.electricityMeter() (seMetering 0x0702 +
// haElectricalMeasurement 0x0B04). The custom 0xFXXX seMetering attrs (key_lock/led_ctrl/
// switch_current_max/switch_power_max/switch_time_reload/switch_protect_ctrl/switch_auto_restart)
// require manuf-specific decoders we do not have. Standard energy/power/voltage/current cover the
// wire-monitoring surface.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
    {1, 0x0702},
    {1, 0x0B04},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_TS011F_plug_SlD{
    .zigbee_models=kModels_TS011F_plug_SlD, .zigbee_models_count=sizeof(kModels_TS011F_plug_SlD)/sizeof(kModels_TS011F_plug_SlD[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TS011F_plug-SlD", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TS011F_plug_SlD, .from_zigbee_count=sizeof(kFz_TS011F_plug_SlD)/sizeof(kFz_TS011F_plug_SlD[0]),
    .to_zigbee=kTz_TS011F_plug_SlD, .to_zigbee_count=sizeof(kTz_TS011F_plug_SlD)/sizeof(kTz_TS011F_plug_SlD[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::slacky_diy
