// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Mill MFTWIFI — hand-fixed wrong-class port.
// Smart floor thermostat WiFi & Zigbee.
//
// z2m-source: mill.ts #MFTWIFI — m.thermostat({
//   setpoints.occupiedHeatingSetpoint {5..35 step 0.5}, systemMode ["off","heat"] }).
//
// Same wrong-class bug as Mill-gen-4: the auto-port wired bare genOnOff + battery
// (phantom state/battery/voltage, bind 0x0001/0x0006) for a hvacThermostat
// (0x0201). m.thermostat() exposes local_temperature (attr 0x0000) + a settable
// occupied/current_heating_setpoint (0x0012) + system_mode (0x001C). Wired the
// generic kFzThermostat read + kTzThermostat write and bound 0x0201.
// Models: z2m's null-form "Mill International\0Threa" truncates to
// "Mill International" on the wire; the concatenated form is kept verbatim.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::mill {
namespace {


constexpr const char* kModels_MFTWIFI[] = { "Mill International", "Mill InternationalThrea" };

const FzConverter* const kFz_MFTWIFI[] = {
    &::zhc::generic::kFzThermostat,
};
const TzConverter* const kTz_MFTWIFI[] = {
    &::zhc::generic::kTzThermostat,
};
constexpr Expose kExp_MFTWIFI[] = {
    { "local_temperature",        ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "current_heating_setpoint", ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "system_mode",              ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_MFTWIFI[] = {
    { 1, 0x0201 },
};

}  // namespace

extern const PreparedDefinition kDef_MFTWIFI{
    .zigbee_models=kModels_MFTWIFI, .zigbee_models_count=sizeof(kModels_MFTWIFI)/sizeof(kModels_MFTWIFI[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MFTWIFI", .vendor="Mill",
    .meta=nullptr, .exposes=kExp_MFTWIFI, .exposes_count=sizeof(kExp_MFTWIFI)/sizeof(kExp_MFTWIFI[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MFTWIFI, .from_zigbee_count=sizeof(kFz_MFTWIFI)/sizeof(kFz_MFTWIFI[0]),
    .to_zigbee=kTz_MFTWIFI, .to_zigbee_count=sizeof(kTz_MFTWIFI)/sizeof(kTz_MFTWIFI[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_MFTWIFI,
    .bindings_count           = sizeof(kBind_MFTWIFI)/sizeof(kBind_MFTWIFI[0]),
};

}  // namespace zhc::devices::mill
