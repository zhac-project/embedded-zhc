// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SONOFF SNZB-02WD, graduated from the generated copy, which decoded the
// battery only. z2m v26.115.1 window: temperature and humidity reported every 5 s .. 1 h on a
// 0.2 C / 1 % change; the eWeLink 0xFC11 settings (screen unit, calibration in
// hundredths) and the genPollCtrl bind are ported with it.
// z2m-source: sonoff.ts #SNZB-02WD.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr ::zhc::generic::ZclWriteLookup kUnits[] = { {"celsius", 0}, {"fahrenheit", 1} };
constexpr ::zhc::generic::ZclAttrRow kEweRows[] = {
    { 0x0007, "temperature_units", 1, kUnits, 2 },
    { 0x2003, "temperature_calibration", 100 },
    { 0x2004, "humidity_calibration", 100 },
};
constexpr ::zhc::generic::ZclAttrMap kEweMap{ kEweRows, 3 };
constexpr FzConverter kFzEwe = ::zhc::generic::zcl_attr_fz("manuSpecificWoolley", &kEweMap);   // 0xFC11
constexpr ::zhc::generic::ZclWriteSpec kUnitsSpec{ "temperature_units", 0x0007, 0x21, 0, kUnits, 2 };
constexpr ::zhc::generic::ZclWriteSpec kTempCalSpec{ "temperature_calibration", 0x2003, 0x29, 0, nullptr, 0, 0, 100 };
constexpr TzConverter kTzUnits = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kUnitsSpec);
constexpr TzConverter kTzTempCal = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kTempCalSpec);
constexpr ::zhc::generic::ZclWriteSpec kHumCalSpec{ "humidity_calibration", 0x2004, 0x29, 0, nullptr, 0, 0, 100 };
constexpr TzConverter kTzHumCal = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kHumCalSpec);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &kFzEwe,
};
const TzConverter* const kTz[] = {
    &kTzUnits,
    &kTzTempCal,
    &kTzHumCal,
};
constexpr const char* kOpts4[] = { "celsius", "fahrenheit" };
constexpr Expose kExp[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature_units", ExposeType::Enum, Access::StateSet, nullptr, "Unit shown on the screen; wake the device (button on the back) before changing it", kOpts4, 2, ExposeCategory::Config},
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", "Offset to add/subtract to the reported temperature", nullptr, 0, ExposeCategory::Config, -50, 50, 0},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%", "Offset to add/subtract to the reported humidity", nullptr, 0, ExposeCategory::Config, -50, 50, 0},
};
constexpr const char* kM[] = { "SNZB-02WD" };
constexpr BindingSpec kBind[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
    {1, 0x0020},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 10, 0},
    {1, 0x0402, 0x0000, 0x29, 5, 3600, 20, 0},
    {1, 0x0001, 0x0020, 0x20, 3600, 65000, 10, 0},
    {1, 0x0405, 0x0000, 0x21, 5, 3600, 100, 0},
};
}  // namespace

extern const PreparedDefinition kDefSonoff_SNZB_02WD{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNZB-02WD", .vendor="SONOFF",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::sonoff
