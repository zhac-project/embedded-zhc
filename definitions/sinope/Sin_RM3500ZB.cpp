// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: graduated to fix IAS dead-key + missing temperature.
// Calypso smart water-heater controller (with leak detection).
//
// z2m-source: sinope.ts #RM3500ZB — fromZigbee
//   [fzLocal.ias_water_leak_alarm, fzLocal.sinope, fz.temperature].
//   fzLocal.ias_water_leak_alarm emits {water_leak, tamper} from
//   ssIasZone zoneStatus bits 0 / 2.
//
// The generated def lowered the generic kFzIasZone (bare "alarm" key)
// behind an "alarm" expose, so the leak state never reached the shadow,
// and it omitted the on-board temperature sensor. Swap in the typed
// kFzIasWaterLeakAlarm (emits water_leak) and add kFzTemperature +
// the msTemperatureMeasurement (0x0402) bind. The relay (genOnOff) +
// energy metering (seMetering) paths are retained.
//
// z2m v26.133.0 (R153): keypad_lockout (manuSpecificSinope 0xFF01 attr 0x0002,
// enum8 unlock 0 / lock 1, mfg 0x119C) via the shared Sinope converters, plus its
// reporting (1 s .. 1 h, change 1) and the configure read of keypadLockout /
// drConfigWaterTempMin. NOT ported (pre-existing): low_water_temp_protection
// (0xFF01 attr 0x0076), so the 0x0076 read has no decoder yet.
#include "definitions/_generic/_shared.hpp"
#include "definitions/sinope/_shared.hpp"

namespace zhc::devices::sinope {
namespace {
const FzConverter* const kFz_RM3500ZB[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzMetering,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzIasWaterLeakAlarm,
    &::zhc::sinope::kFzSinopeManu,
};
const TzConverter* const kTz_RM3500ZB[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::sinope::kTzSinopeKeypadLockout,
};
constexpr const char* kModels_RM3500ZB[] = { "RM3500ZB" };
constexpr const char* kKeypadLockout[] = { "unlock", "lock" };

constexpr ReportingSpec kReports_RM3500ZB[] = {
    {1, 0xFF01, 0x0002, 0x30, 1, 3600, 1, 0x119C},  // keypadLockout
};
constexpr std::uint8_t kReadSinope_RM3500ZB[] = { 0x02, 0x00, 0x76, 0x00 };  // keypadLockout, drConfigWaterTempMin
constexpr ConfigStep kSteps_RM3500ZB[] = {
    { ConfigStepOp::Read, 1, 0xFF01, 0x00, 0, kReadSinope_RM3500ZB, sizeof(kReadSinope_RM3500ZB), 0, 0x119C },
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"keypad_lockout", ExposeType::Enum, Access::StateSet, nullptr, "Enables or disables the physical button",
     kKeypadLockout, sizeof(kKeypadLockout)/sizeof(kKeypadLockout[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0702},  // seMetering
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0500},  // ssIasZone
    {1, 0xFF01},  // manuSpecificSinope (z2m binds it for the keypadLockout report)
};

extern const PreparedDefinition kDef_RM3500ZB{
    .zigbee_models=kModels_RM3500ZB, .zigbee_models_count=sizeof(kModels_RM3500ZB)/sizeof(kModels_RM3500ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RM3500ZB", .vendor="Sinope",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RM3500ZB, .from_zigbee_count=sizeof(kFz_RM3500ZB)/sizeof(kFz_RM3500ZB[0]),
    .to_zigbee=kTz_RM3500ZB, .to_zigbee_count=sizeof(kTz_RM3500ZB)/sizeof(kTz_RM3500ZB[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=kReports_RM3500ZB, .reports_count=sizeof(kReports_RM3500ZB)/sizeof(kReports_RM3500ZB[0]),
    .config_steps=kSteps_RM3500ZB, .config_steps_count=sizeof(kSteps_RM3500ZB)/sizeof(kSteps_RM3500ZB[0]),
};

}  // namespace zhc::devices::sinope
