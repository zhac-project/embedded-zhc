// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm mKomfy_Sensor — auto-generated.
// mKomfy, stove guard
// z2m-source: ctm.ts #mKomfy_Sensor.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
// mKomfy_Sensor z2m: fz.temperature + fz.battery + fz.ctm_stove_guard.
// ctmStoveGuard cluster (0xFFC9, mfg 0x1337) decoder is BLOCKED — every
// stove-guard expose (alarm_status, active, change_battery, stove_temp,
// runtime, etc.) needs a vendor fz_ctm_stove_guard decoder.
const FzConverter* const kFz_mKomfy_Sensor[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};

constexpr const char* kModels_mKomfy_Sensor[] = { "mKomfy", "mKomfy Tak" };

}  // namespace


// Manual: add temperature (msTemperatureMeasurement std). All
// ctmStoveGuard manuSpec exposes (alarm_status enum, active, change_
// battery, stove_temperature, runtime, dipSwitch, swVersion, etc.)
// stay TODO until ZHC gains a fz_ctm_stove_guard decoder.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0402},
};

extern const PreparedDefinition kDef_mKomfy_Sensor{
    .zigbee_models=kModels_mKomfy_Sensor, .zigbee_models_count=sizeof(kModels_mKomfy_Sensor)/sizeof(kModels_mKomfy_Sensor[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mKomfy_Sensor", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mKomfy_Sensor, .from_zigbee_count=sizeof(kFz_mKomfy_Sensor)/sizeof(kFz_mKomfy_Sensor[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
