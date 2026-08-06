// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital "Topazio" wall units (z2m v26.92.0 parity)
//   TO-DM-W/B   _TZE284_5yah8qx4  dimmer                     (added v26.83.0)
//   TO-WK-1W/B  _TZE284_3xnyj4ga  1 gang switch + socket     (added v26.82.0)
//   TO-WK-2W/B  _TZE284_exfilann  2 gang switch + socket     (added v26.89.0)
// z2m-source: tuya.ts #TO-DM-W/B / #TO-WK-1W/B / #TO-WK-2W/B
//
// Divisors resolved from the device-local `convLocal.novaDigital*` converters
// in tuya.ts, not guessed:
//   novaDigitalToDmBrightnessPercent   wire/10  <-> value*10   => divisor 10
//   energy   divideBy1000   current divideBy1000
//   power    divideBy10     voltage divideBy10
//   countdown = valueConverterBasic.raw()                      => divisor 1
//
// DEFERRED datapoints (deliberate, not overlooked):
//   * TO-DM DP2 `brightness` — convLocal.novaDigitalToDmBrightness is an
//     AFFINE map, round((wire-10)*254/990) with the inverse 10+(v*990)/254.
//     TuyaDpMapEntry can only express a divisor, so wiring it would report a
//     ~4% -to- 100% wrong brightness. Needs an affine DP factory first.
//   * TO-WK DP19 `inching_l1` / `inching_l2` — convLocal.novaDigitalToWkInching
//     decodes a packed 3-byte buffer (u8 enabled + u16BE seconds) into a single
//     value. Not a scalar DP; needs a struct-DP decoder.
// Everything else on all three devices is covered, including full metering.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kOnOff[]          = { {0, "OFF"}, {1, "ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kLightType[]      = { {0, "led"}, {1, "incandescent"}, {2, "halogen"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehav[]   = { {0, "off"}, {1, "on"}, {2, "previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kIndicatorDm[]    = { {0, "none"}, {1, "relay"}, {2, "pos"} };
constexpr ::zhc::tuya::TuyaEnumEntry kIndicatorWk[]    = { {0, "none"}, {1, "relay"}, {2, "pos"} };
constexpr ::zhc::tuya::TuyaEnumEntry kVibration[]      = { {0, "off"}, {1, "low"}, {2, "medium"}, {3, "high"} };

// ---- TO-DM-W/B dimmer ---------------------------------------------------
struct cfg_dm {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        // DP2 brightness DEFERRED — affine, see file header.
        ::zhc::tuya::dp::numeric(3, "brightness_min", 10),
        ::zhc::tuya::dp::enum_lookup(4, "light_type", kLightType, 3),
        ::zhc::tuya::dp::numeric(5, "brightness_max", 10),
        ::zhc::tuya::dp::numeric(6, "countdown", 1),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOnBehav, 3),
        ::zhc::tuya::dp::enum_lookup(21, "indicator_mode", kIndicatorDm, 3),
        ::zhc::tuya::dp::binary(26, "backlight_switch"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXDM = ::zhc::tuya::factory::TuyaRw<cfg_dm>;

// ---- TO-WK-1W/B  (2 controllable outputs: gang + socket) ----------------
struct cfg_wk1 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state_l1", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(2, "state_l2", kOnOff, 2),
        ::zhc::tuya::dp::numeric(7, "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(8, "countdown_l2", 1),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOnBehav, 3),
        ::zhc::tuya::dp::enum_lookup(15, "indicator_mode", kIndicatorWk, 3),
        ::zhc::tuya::dp::binary(16, "backlight_switch"),
        // DP19 inching_l1 DEFERRED — packed buffer, see file header.
        ::zhc::tuya::dp::numeric(20, "energy", 1000),
        ::zhc::tuya::dp::numeric(21, "current", 1000),
        ::zhc::tuya::dp::numeric(22, "power", 10),
        ::zhc::tuya::dp::numeric(23, "voltage", 10),
        ::zhc::tuya::dp::binary(101, "induction"),
        ::zhc::tuya::dp::enum_lookup(102, "vibration", kVibration, 4),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXWK1 = ::zhc::tuya::factory::TuyaRw<cfg_wk1>;

// ---- TO-WK-2W/B  (3 controllable outputs) -------------------------------
struct cfg_wk2 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state_l1", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(2, "state_l2", kOnOff, 2),
        ::zhc::tuya::dp::enum_lookup(3, "state_l3", kOnOff, 2),
        ::zhc::tuya::dp::numeric(7, "countdown_l1", 1),
        ::zhc::tuya::dp::numeric(8, "countdown_l2", 1),
        ::zhc::tuya::dp::numeric(9, "countdown_l3", 1),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOnBehav, 3),
        ::zhc::tuya::dp::enum_lookup(15, "indicator_mode", kIndicatorWk, 3),
        ::zhc::tuya::dp::binary(16, "backlight_switch"),
        // DP19 inching_l1 + inching_l2 DEFERRED — packed buffer, see header.
        ::zhc::tuya::dp::numeric(20, "energy", 1000),
        ::zhc::tuya::dp::numeric(21, "current", 1000),
        ::zhc::tuya::dp::numeric(22, "power", 10),
        ::zhc::tuya::dp::numeric(23, "voltage", 10),
        ::zhc::tuya::dp::binary(101, "induction"),
        ::zhc::tuya::dp::enum_lookup(102, "vibration", kVibration, 4),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXWK2 = ::zhc::tuya::factory::TuyaRw<cfg_wk2>;

constexpr const char* kOnOffOpts[]        = { "OFF", "ON" };
constexpr const char* kLightTypeOpts[]    = { "led", "incandescent", "halogen" };
constexpr const char* kPowerOnBehavOpts[] = { "off", "on", "previous" };
constexpr const char* kIndicatorOpts[]    = { "none", "relay", "pos" };
constexpr const char* kVibrationOpts[]    = { "off", "low", "medium", "high" };

constexpr Expose kExpDm[] = {
    { "state",             ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Light state",             nullptr,            0 },
    { "brightness_min",    ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Minimum brightness",      nullptr,            0 },
    { "brightness_max",    ExposeType::Numeric, ::zhc::Access::StateSet, "%",     "Maximum brightness",      nullptr,            0 },
    { "light_type",        ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Lamp type",               kLightTypeOpts,     3 },
    { "countdown",         ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Countdown",               nullptr,            0 },
    { "power_on_behavior", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Power-on behaviour",      kPowerOnBehavOpts,  3 },
    { "indicator_mode",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Indicator light mode",    kIndicatorOpts,     3 },
    { "backlight_switch",  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Backlight",               nullptr,            0 },
};

#define ZHC_TO_WK_METERING_EXPOSES                                                                                     \
    { "energy",            ExposeType::Numeric, ::zhc::Access::State,    "kWh",   "Cumulative energy",       nullptr,           0 },  \
    { "power",             ExposeType::Numeric, ::zhc::Access::State,    "W",     "Active power",            nullptr,           0 },  \
    { "current",           ExposeType::Numeric, ::zhc::Access::State,    "A",     "Current",                 nullptr,           0 },  \
    { "voltage",           ExposeType::Numeric, ::zhc::Access::State,    "V",     "Voltage",                 nullptr,           0 },  \
    { "power_on_behavior", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Power-on behaviour",      kPowerOnBehavOpts, 3 },  \
    { "indicator_mode",    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Indicator light mode",    kIndicatorOpts,    3 },  \
    { "backlight_switch",  ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Backlight",               nullptr,           0 },  \
    { "induction",         ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Presence induction",      nullptr,           0 },  \
    { "vibration",         ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Vibration sensitivity",   kVibrationOpts,    4 }

constexpr Expose kExpWk1[] = {
    { "state_l1",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Output 1 state",     kOnOffOpts, 2 },
    { "state_l2",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Output 2 state",     kOnOffOpts, 2 },
    { "countdown_l1", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Output 1 countdown", nullptr,    0 },
    { "countdown_l2", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Output 2 countdown", nullptr,    0 },
    ZHC_TO_WK_METERING_EXPOSES,
};
constexpr Expose kExpWk2[] = {
    { "state_l1",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Output 1 state",     kOnOffOpts, 2 },
    { "state_l2",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Output 2 state",     kOnOffOpts, 2 },
    { "state_l3",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Output 3 state",     kOnOffOpts, 2 },
    { "countdown_l1", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Output 1 countdown", nullptr,    0 },
    { "countdown_l2", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Output 2 countdown", nullptr,    0 },
    { "countdown_l3", ExposeType::Numeric, ::zhc::Access::StateSet, "s",     "Output 3 countdown", nullptr,    0 },
    ZHC_TO_WK_METERING_EXPOSES,
};

#undef ZHC_TO_WK_METERING_EXPOSES

constexpr const char* kM[]    = { "TS0601" };
constexpr const char* kNDm[]  = { "_TZE284_5yah8qx4" };
constexpr const char* kNWk1[] = { "_TZE284_3xnyj4ga" };
constexpr const char* kNWk2[] = { "_TZE284_exfilann" };
}  // namespace

extern const PreparedDefinition kDef_TO_DM_W_B{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kNDm,.manufacturer_names_count=1,
    .model="TO-DM-W/B",.vendor="Nova Digital",
    .meta=nullptr,
    .exposes=kExpDm,.exposes_count=sizeof(kExpDm)/sizeof(kExpDm[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FXDM::fz_list,.from_zigbee_count=FXDM::fz_count,
    .to_zigbee=FXDM::tz_list,.to_zigbee_count=FXDM::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_TO_WK_1W_B{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kNWk1,.manufacturer_names_count=1,
    .model="TO-WK-1W/B",.vendor="Nova Digital",
    .meta=nullptr,
    .exposes=kExpWk1,.exposes_count=sizeof(kExpWk1)/sizeof(kExpWk1[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FXWK1::fz_list,.from_zigbee_count=FXWK1::fz_count,
    .to_zigbee=FXWK1::tz_list,.to_zigbee_count=FXWK1::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

extern const PreparedDefinition kDef_TO_WK_2W_B{
    .zigbee_models=kM,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kNWk2,.manufacturer_names_count=1,
    .model="TO-WK-2W/B",.vendor="Nova Digital",
    .meta=nullptr,
    .exposes=kExpWk2,.exposes_count=sizeof(kExpWk2)/sizeof(kExpWk2[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FXWK2::fz_list,.from_zigbee_count=FXWK2::fz_count,
    .to_zigbee=FXWK2::tz_list,.to_zigbee_count=FXWK2::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
