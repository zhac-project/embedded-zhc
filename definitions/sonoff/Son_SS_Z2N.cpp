// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SONOFF SS-Z2N soil temperature & moisture sensor (zigbeeModel SS-Z2N),
// new in z2m v26.126.0..v26.133.0. Partial port:
//   * m.battery, m.temperature (120 s .. 1 h, 1 C), m.soilMoisture (120 s .. 1 h,
//     3 %), the genPollCtrl bind;
//   * customClusterEwelink 0xFC11 (no manufacturer code, named
//     manuSpecificWoolley here): temperature/humidity calibration (int16 x100),
//     measure_interval (u16 enum), comfort temperature (int16 x100) / humidity
//     (u16 x100) min/max, frost_protection_temperature (int16 x100), the three
//     threshold enables (u8, ON/OFF -> Bool), the last-hour max/min/average
//     temperature and humidity (x100, read-only);
//   * refresh_measurements: command 0x00 on 0xFC11, no payload.
// Not ported: the derived temperature_status / humidity_status / frost_risk_status
// keys (computed from shadow state on every report) and the min < max pair check
// on the comfort writes (z2m refuses an inverted pair; here the write goes out
// as-is), battery-percentage reporting.
// z2m-source: sonoff.ts #SS-Z2N
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;
using ::zhc::generic::kZclAttrFlagBool;

constexpr ZclWriteLookup kIntervalLut[] = {
    {"2_min", 0}, {"5_min", 1}, {"15_min", 2}, {"30_min", 3}, {"1_hour", 4}, {"2_hours", 5},
};
constexpr ZclWriteLookup kOnOffLut[] = { {"OFF", 0}, {"ON", 1} };
constexpr ::zhc::generic::ZclAttrRow kEweRows[] = {
    { 0x2003, "temperature_calibration", 100 },
    { 0x2004, "humidity_calibration", 100 },
    { 0x202D, "measure_interval", 1, kIntervalLut, sizeof(kIntervalLut)/sizeof(kIntervalLut[0]) },
    { 0x0004, "comfort_temperature_min", 100 },
    { 0x0003, "comfort_temperature_max", 100 },
    { 0x0005, "comfort_humidity_min", 100 },
    { 0x0006, "comfort_humidity_max", 100 },
    { 0x6002, "frost_protection_temperature", 100 },
    { 0x202A, "temperature_threshold_enable", 1, nullptr, 0, kZclAttrFlagBool },
    { 0x202B, "humidity_threshold_enable", 1, nullptr, 0, kZclAttrFlagBool },
    { 0x202C, "frost_threshold_enable", 1, nullptr, 0, kZclAttrFlagBool },
    { 0x2008, "temperature_max_in_hour", 100 },
    { 0x2009, "temperature_min_in_hour", 100 },
    { 0x200A, "temperature_aver_in_hour", 100 },
    { 0x200B, "humidity_max_in_hour", 100 },
    { 0x200C, "humidity_min_in_hour", 100 },
    { 0x200D, "humidity_aver_in_hour", 100 },
};
constexpr ::zhc::generic::ZclAttrMap kEweMap{ kEweRows, sizeof(kEweRows)/sizeof(kEweRows[0]) };
constexpr FzConverter kFzEwe = ::zhc::generic::zcl_attr_fz("manuSpecificWoolley", &kEweMap);   // 0xFC11

constexpr ZclWriteSpec kTempCalSpec{ "temperature_calibration", 0x2003, 0x29, 0, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kHumCalSpec{ "humidity_calibration", 0x2004, 0x29, 0, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kIntervalSpec{ "measure_interval", 0x202D, 0x21, 0, kIntervalLut, sizeof(kIntervalLut)/sizeof(kIntervalLut[0]) };
constexpr ZclWriteSpec kComfortTMinSpec{ "comfort_temperature_min", 0x0004, 0x29, 0, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kComfortTMaxSpec{ "comfort_temperature_max", 0x0003, 0x29, 0, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kComfortHMinSpec{ "comfort_humidity_min", 0x0005, 0x21, 0, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kComfortHMaxSpec{ "comfort_humidity_max", 0x0006, 0x21, 0, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kFrostSpec{ "frost_protection_temperature", 0x6002, 0x29, 0, nullptr, 0, 0, 100 };
constexpr ZclWriteSpec kTempThrSpec{ "temperature_threshold_enable", 0x202A, 0x20, 0, kOnOffLut, sizeof(kOnOffLut)/sizeof(kOnOffLut[0]) };
constexpr ZclWriteSpec kHumThrSpec{ "humidity_threshold_enable", 0x202B, 0x20, 0, kOnOffLut, sizeof(kOnOffLut)/sizeof(kOnOffLut[0]) };
constexpr ZclWriteSpec kFrostThrSpec{ "frost_threshold_enable", 0x202C, 0x20, 0, kOnOffLut, sizeof(kOnOffLut)/sizeof(kOnOffLut[0]) };
constexpr TzConverter kTzTempCal = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kTempCalSpec);
constexpr TzConverter kTzHumCal = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kHumCalSpec);
constexpr TzConverter kTzInterval = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kIntervalSpec);
constexpr TzConverter kTzComfortTMin = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kComfortTMinSpec);
constexpr TzConverter kTzComfortTMax = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kComfortTMaxSpec);
constexpr TzConverter kTzComfortHMin = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kComfortHMinSpec);
constexpr TzConverter kTzComfortHMax = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kComfortHMaxSpec);
constexpr TzConverter kTzFrost = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kFrostSpec);
constexpr TzConverter kTzTempThr = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kTempThrSpec);
constexpr TzConverter kTzHumThr = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kHumThrSpec);
constexpr TzConverter kTzFrostThr = ::zhc::generic::zcl_write_tz("manuSpecificWoolley", 0xFC11, &kFrostThrSpec);

// sonoffExtend.ssZ2nRefreshMeasurements: cluster command 0x00, empty payload.
bool tz_refresh_measurements(std::string_view key, const Value&, const TzConverter&, const PreparedDefinition&,
                             RuntimeContext&, std::span<std::uint8_t> out, std::size_t& n) {
    n = 0;
    if (key != "refresh_measurements" || out.size() < 3) return false;
    out[0] = 0x11;   // cluster-specific, client -> server, no default response
    out[1] = 0x00;   // tsn placeholder
    out[2] = 0x00;   // refreshMeasurements
    n = 3;
    return true;
}
constexpr TzConverter kTzRefresh{
    .key = "refresh_measurements", .cluster = "manuSpecificWoolley", .cluster_id = 0xFC11,
    .command_id = 0x00, .fn = tz_refresh_measurements, .user_config = nullptr,
};

const FzConverter* const kFz_SS_Z2N[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzSoilMoisture,
    &kFzEwe,
};
const TzConverter* const kTz_SS_Z2N[] = {
    &kTzRefresh,
    &kTzTempCal,
    &kTzHumCal,
    &kTzInterval,
    &kTzComfortTMin,
    &kTzComfortTMax,
    &kTzComfortHMin,
    &kTzComfortHMax,
    &kTzFrost,
    &kTzTempThr,
    &kTzHumThr,
    &kTzFrostThr,
};
constexpr const char* kModels_SS_Z2N[] = { "SS-Z2N" };
constexpr const char* kRefreshOpts[] = { "refresh" };
constexpr const char* kIntervalOpts[] = { "2_min", "5_min", "15_min", "30_min", "1_hour", "2_hours" };
constexpr Expose kExposes_SS_Z2N[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0, ExposeCategory::State, -20, 60, 0},
    {"soil_moisture", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"refresh_measurements", ExposeType::Enum, Access::Set, nullptr,
     "Take new soil temperature and moisture measurements; the device reports them once done.",
     kRefreshOpts, sizeof(kRefreshOpts)/sizeof(kRefreshOpts[0])},
    {"temperature_calibration", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C",
     "Offset to add/subtract to the reported temperature.", nullptr, 0, ExposeCategory::Config, -10, 10, 0},
    {"humidity_calibration", ExposeType::Numeric, Access::StateSet, "%",
     "Offset added to or subtracted from the reported soil moisture value.", nullptr, 0, ExposeCategory::Config, -100, 100, 1},
    {"measure_interval", ExposeType::Enum, Access::StateSet, nullptr,
     "Set the interval for scheduled measurements and reports. Default: 5 minutes.",
     kIntervalOpts, sizeof(kIntervalOpts)/sizeof(kIntervalOpts[0]), ExposeCategory::Config},
    {"comfort_temperature_min", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C",
     "Soil temperature below this value is classified as low. Default: 8 \xC2\xB0""C.", nullptr, 0,
     ExposeCategory::Config, -20, 60, 1},
    {"comfort_temperature_max", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C",
     "Soil temperature above this value is classified as high. Default: 32 \xC2\xB0""C.", nullptr, 0,
     ExposeCategory::Config, -20, 60, 1},
    {"comfort_humidity_min", ExposeType::Numeric, Access::StateSet, "%",
     "Soil moisture below this value is classified as low. Default: 15%.", nullptr, 0, ExposeCategory::Config, 1, 99, 1},
    {"comfort_humidity_max", ExposeType::Numeric, Access::StateSet, "%",
     "Soil moisture above this value is classified as high. Default: 30%.", nullptr, 0, ExposeCategory::Config, 1, 99, 1},
    {"frost_protection_temperature", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C",
     "Soil temperature below this value indicates a risk of frost. Default: 5 \xC2\xB0""C.", nullptr, 0,
     ExposeCategory::Config, -10, 20, 1},
    {"temperature_threshold_enable", ExposeType::Binary, Access::StateSet, nullptr,
     "Enables/disables the temperature threshold alarm (wake the device first).", nullptr, 0, ExposeCategory::Config},
    {"humidity_threshold_enable", ExposeType::Binary, Access::StateSet, nullptr,
     "Enables/disables the humidity threshold alarm (wake the device first).", nullptr, 0, ExposeCategory::Config},
    {"frost_threshold_enable", ExposeType::Binary, Access::StateSet, nullptr,
     "Enables/disables the frost protection threshold alarm (wake the device first).", nullptr, 0, ExposeCategory::Config},
    {"temperature_max_in_hour", ExposeType::Numeric, Access::State, "\xC2\xB0""C", "Maximum temperature recorded in the last hour", nullptr, 0},
    {"temperature_min_in_hour", ExposeType::Numeric, Access::State, "\xC2\xB0""C", "Minimum temperature recorded in the last hour", nullptr, 0},
    {"temperature_aver_in_hour", ExposeType::Numeric, Access::State, "\xC2\xB0""C", "Average temperature recorded in the last hour", nullptr, 0},
    {"humidity_max_in_hour", ExposeType::Numeric, Access::State, "%", "Maximum humidity recorded in the last hour", nullptr, 0},
    {"humidity_min_in_hour", ExposeType::Numeric, Access::State, "%", "Minimum humidity recorded in the last hour", nullptr, 0},
    {"humidity_aver_in_hour", ExposeType::Numeric, Access::State, "%", "Average humidity recorded in the last hour", nullptr, 0},
};
constexpr BindingSpec kBindings_SS_Z2N[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0408},
    {1, 0x0020},
};
constexpr ReportingSpec kReports_SS_Z2N[] = {
    {1, 0x0402, 0x0000, 0x29, 120, 3600, 100, 0},
    {1, 0x0408, 0x0000, 0x21, 120, 3600, 300, 0},
};
}  // namespace

extern const PreparedDefinition kDef_SS_Z2N{
    .zigbee_models=kModels_SS_Z2N, .zigbee_models_count=sizeof(kModels_SS_Z2N)/sizeof(kModels_SS_Z2N[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SS-Z2N", .vendor="Sonoff",
    .meta=nullptr, .exposes=kExposes_SS_Z2N, .exposes_count=sizeof(kExposes_SS_Z2N)/sizeof(kExposes_SS_Z2N[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SS_Z2N, .from_zigbee_count=sizeof(kFz_SS_Z2N)/sizeof(kFz_SS_Z2N[0]),
    .to_zigbee=kTz_SS_Z2N, .to_zigbee_count=sizeof(kTz_SS_Z2N)/sizeof(kTz_SS_Z2N[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_SS_Z2N, .bindings_count=sizeof(kBindings_SS_Z2N)/sizeof(kBindings_SS_Z2N[0]),
    .reports=kReports_SS_Z2N, .reports_count=sizeof(kReports_SS_Z2N)/sizeof(kReports_SS_Z2N[0]),
};

}  // namespace zhc::devices::sonoff
