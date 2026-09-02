// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Moes SFD02-Z "Star feather" smart dimmer switch (`_TZE284_t88bjhfu`).
// New upstream in z2m v26.102.0 (#13045). Same board family as the SFL02-Z
// switches (induction / vibration / indicator datapoints) plus the standard
// Tuya dimmer datapoints.
//
// Brightness (dp2/3/5) is 0..1000 on the wire; z2m publishes 0..254
// (`scale0_254to0_1000`), reproduced here with a float divisor of 1000/254
// on both directions. `induction_mode` rides the plain onOff converter
// upstream (exposed as an ON/OFF enum there) — a Bool here.
//
// z2m-source: moes.ts #SFD02-Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::moes {
namespace {

constexpr float kBrightnessScale = 1000.0f / 254.0f;  // z2m scale0_254to0_1000

constexpr ::zhc::tuya::TuyaEnumEntry kLightType[] = { {0, "led"}, {1, "incandescent"}, {2, "halogen"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOn[]   = { {0, "off"}, {1, "on"}, {2, "previous"} };
constexpr ::zhc::tuya::TuyaEnumEntry kBacklight[] = { {0, "off"}, {1, "normal"}, {2, "inverted"} };
constexpr ::zhc::tuya::TuyaEnumEntry kIndicator[] = { {0, "off"}, {1, "relay"}, {2, "invert"} };
constexpr ::zhc::tuya::TuyaEnumEntry kVibration[] = { {0, "Gear 0"}, {1, "Gear 1"}, {2, "Gear 2"}, {3, "Gear 3"} };

constexpr const char* kLightTypeOpts[] = { "led", "incandescent", "halogen" };
constexpr const char* kPowerOnOpts[]   = { "off", "on", "previous" };
constexpr const char* kBacklightOpts[] = { "off", "normal", "inverted" };
constexpr const char* kIndicatorOpts[] = { "off", "relay", "invert" };
constexpr const char* kVibrationOpts[] = { "Gear 0", "Gear 1", "Gear 2", "Gear 3" };

struct cfg_sfd02 {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state"),
        { 2, "brightness",     ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        { 3, "min_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        ::zhc::tuya::dp::enum_lookup(4, "light_type", kLightType, 3),
        { 5, "max_brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        ::zhc::tuya::dp::numeric(6, "countdown", 1),
        ::zhc::tuya::dp::enum_lookup(14, "power_on_behavior", kPowerOn, 3),
        ::zhc::tuya::dp::enum_lookup(21, "backlight_mode", kBacklight, 3),
        ::zhc::tuya::dp::binary(26, "induction_mode"),
        ::zhc::tuya::dp::enum_lookup(101, "indicator_status", kIndicator, 3),
        ::zhc::tuya::dp::enum_lookup(102, "vibration_mode", kVibration, 4),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_sfd02>;

constexpr Expose kExposes[] = {
    { "state",          ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness",     ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 1 },
    { "min_brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Minimum brightness", nullptr, 0, ExposeCategory::Config, 0, 254, 1 },
    { "max_brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Maximum brightness", nullptr, 0, ExposeCategory::Config, 0, 254, 1 },
    { "light_type",     ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Type of light attached to the device", kLightTypeOpts, 3, ExposeCategory::Config },
    { "countdown",      ExposeType::Numeric, ::zhc::Access::StateSet, "s", "Toggle the device after a set duration (one time action)", nullptr, 0, ExposeCategory::Config, 0, 43200, 1 },
    { "power_on_behavior", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kPowerOnOpts, 3, ExposeCategory::Config },
    { "backlight_mode", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Mode of the backlight", kBacklightOpts, 3, ExposeCategory::Config },
    { "induction_mode", ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "Induction mode", nullptr, 0, ExposeCategory::Config },
    { "indicator_status", ExposeType::Enum,  ::zhc::Access::StateSet, nullptr, "Indicator status", kIndicatorOpts, 3, ExposeCategory::Config },
    { "vibration_mode", ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, "Vibration", kVibrationOpts, 4, ExposeCategory::Config },
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE284_t88bjhfu" };

}  // namespace

extern const PreparedDefinition kDef_SFD02_Z{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="SFD02-Z", .vendor="Moes",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::moes
