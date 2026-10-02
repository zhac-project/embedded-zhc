// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Mowe MW836P presence sensor with relay, 24 GHz radar + PIR (z2m v26.110.0, #13190).
// DP107-109 (hardware_version / soft_version / radar_id) are string datapoints and
// are not ported: this port surfaces no string datapoint.
// z2m-source: mowe.ts #MW836P.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::mowe {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kRelayDelay112[] = { {0,"0"}, {1,"1"}, {2,"5"}, {3,"10"}, {4,"20"}, {5,"30"} };
constexpr ::zhc::tuya::TuyaEnumEntry kDetectionRange120[] = { {0,"2m"}, {1,"3m"} };
constexpr ::zhc::tuya::TuyaEnumEntry kNobodyTime133[] = { {0,"10s"}, {1,"20s"}, {2,"30s"}, {3,"60s"}, {4,"180s"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "presence"),
        ::zhc::tuya::dp::binary(101, "auto_monitoring"),
        ::zhc::tuya::dp::numeric(103, "illuminance", 1),
        ::zhc::tuya::dp::binary(104, "infrared"),
        ::zhc::tuya::dp::binary(105, "induction_switch"),
        ::zhc::tuya::dp::binary(111, "relay"),
        ::zhc::tuya::dp::enum_lookup(112, "relay_delay", kRelayDelay112, 6),
        ::zhc::tuya::dp::binary(113, "relay_off_trigger"),
        ::zhc::tuya::dp::binary(114, "micro_motion_detection"),
        ::zhc::tuya::dp::binary(115, "self_test"),
        ::zhc::tuya::dp::enum_lookup(120, "detection_range", kDetectionRange120, 2),
        ::zhc::tuya::dp::numeric(122, "motion_sensitivity", 1),
        ::zhc::tuya::dp::numeric(123, "static_sensitivity", 1),
        ::zhc::tuya::dp::enum_lookup(133, "nobody_time", kNobodyTime133, 5),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kOpts4[] = { "2m", "3m" };
constexpr const char* kOpts5[] = { "10s", "20s", "30s", "60s", "180s" };
constexpr const char* kOpts11[] = { "0", "1", "5", "10", "20", "30" };
constexpr Expose kExp[] = {
    {"presence", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"motion_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Radar sensitivity to a moving person", nullptr, 0, ExposeCategory::State, 1, 3, 1},
    {"static_sensitivity", ExposeType::Numeric, Access::StateSet, nullptr, "Radar sensitivity to a still person", nullptr, 0, ExposeCategory::State, 1, 3, 1},
    {"detection_range", ExposeType::Enum, Access::StateSet, nullptr, "Maximum detection distance", kOpts4, 2},
    {"nobody_time", ExposeType::Enum, Access::StateSet, nullptr, "How long presence is held after the last detection", kOpts5, 5},
    {"micro_motion_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"infrared", ExposeType::Binary, Access::StateSet, nullptr, "Infrared (PIR) trigger element", nullptr, 0, ExposeCategory::Config},
    {"induction_switch", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"auto_monitoring", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"relay", ExposeType::Binary, Access::StateSet, nullptr, "Enable the built-in relay output", nullptr, 0, ExposeCategory::Config},
    {"relay_delay", ExposeType::Enum, Access::StateSet, nullptr, "Relay switch-off delay once the room reads vacant", kOpts11, 6, ExposeCategory::Config},
    {"relay_off_trigger", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"self_test", ExposeType::Binary, Access::State, nullptr, "Radar power-on self-test in progress", nullptr, 0, ExposeCategory::Diagnostic},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_mexuq6lm" };
}  // namespace

extern const PreparedDefinition kDef_MW836P{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="MW836P",.vendor="Mowe",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::mowe
