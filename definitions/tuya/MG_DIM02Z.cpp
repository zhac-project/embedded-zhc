// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya MG-DIM02Z dimmer module with power monitoring (`_TZE284_da26abzz`).
// New upstream in z2m v26.102.0 (#13039).
//
// Brightness (dp142) is 0..1000 on the wire, published 0..254 like z2m
// (`scale0_254to0_1000`), float divisor 1000/254 both ways. One upstream
// nicety is NOT reproduced: z2m's brightness write also drives dp141
// (brightness 0 → state off; brightness > 0 with no `state` in the same
// message → state on first). Here `brightness` writes dp142 only and `state`
// is its own key, as on every other datapoint dimmer in this tree.
// min/max_brightness (dp143/144) are raw 10..1000 upstream and stay raw.
//
// z2m-source: tuya.ts #MG-DIM02Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr float kBrightnessScale = 1000.0f / 254.0f;

constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnState[] = {
    {0, "power_off"}, {1, "power_on"}, {2, "memory"} };
constexpr const char* kPowerOnStateOpts[] = { "memory", "power_on", "power_off" };

struct cfg_mg_dim02z {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(21, "current", 1000),
        ::zhc::tuya::dp::numeric(22, "power",   10),
        ::zhc::tuya::dp::numeric(23, "voltage", 10),
        ::zhc::tuya::dp::binary(141, "state"),
        { 142, "brightness", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        ::zhc::tuya::dp::numeric(143, "min_brightness", 1),
        ::zhc::tuya::dp::numeric(144, "max_brightness", 1),
        ::zhc::tuya::dp::enum_lookup(146, "power_on_state", kPowerOnState, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_mg_dim02z>;

constexpr Expose kExposes[] = {
    { "state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 1 },
    { "min_brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Minimum brightness limit", nullptr, 0, ExposeCategory::Config, 10, 1000, 10 },
    { "max_brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, "Maximum brightness limit", nullptr, 0, ExposeCategory::Config, 10, 1000, 10 },
    { "power_on_state", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Power-on state after power loss", kPowerOnStateOpts, 3, ExposeCategory::Config },
    { "power",   ExposeType::Numeric, ::zhc::Access::State, "W", nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0 },
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE284_da26abzz" };

}  // namespace

extern const PreparedDefinition kDef_MG_DIM02Z{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="MG-DIM02Z", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
