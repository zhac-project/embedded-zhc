// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya AE-5503-S-H-ZIGBEE sauna thermostat (z2m v26.111.0, #13225).
// DP1 is a boolean system_mode {off: false, heat: true}.
// z2m-source: tuya.ts #AE-5503-S-H-ZIGBEE.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSystemMode1[] = { {0,"off"}, {1,"heat"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "system_mode", ::zhc::TuyaDpType::Bool, 1, kSystemMode1, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
        ::zhc::tuya::dp::numeric(16, "current_heating_setpoint", 1),
        ::zhc::tuya::dp::numeric(24, "local_temperature", 1),
        ::zhc::tuya::dp::binary(40, "child_lock"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts1[] = { "off", "heat" };
constexpr Expose kExp[] = {
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Enables/disables physical input on the device", nullptr, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, "Mode of this device", kOpts1, 2},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", "Temperature setpoint", nullptr, 0, ExposeCategory::State, 0, 120, 1},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", "Current temperature measured on the device", nullptr, 0},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE204_eaasry7v" };
}  // namespace

extern const PreparedDefinition kDef_AE_5503_S_H_ZIGBEE{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="AE-5503-S-H-ZIGBEE",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::tuya
