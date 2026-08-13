// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Excellux ZG-109TDS probe: temperature, EC, TDS, salinity, SG (z2m v26.95.0 parity).
// z2m-source: tuya.ts #ZG-109TDS (added v26.95.0)
// Datapoint map emitted from the upstream tuyaDatapoints triples; each
// divisor is the one that device's own valueConverter uses. Where upstream
// maps ONE datapoint to several keys with different lookups, the emitter
// names both tables after the DP and they collide -- those are suffixed _vN.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_DTS1XM9_112[] = { {0,"none"},{1,"low"},{2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_DTS1XM9_117[] = { {0,"none"},{1,"low"},{2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_DTS1XM9_122[] = { {0,"none"},{1,"low"},{2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_DTS1XM9_126[] = { {0,"none"},{1,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_DTS1XM9_130[] = { {0,"none"},{1,"low"},{2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_DTS1XM9_133[] = { {0,"freshwater"},{1,"seawater"} };

struct cfg_ZG_109TDS {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
    ::zhc::tuya::dp::numeric(1,"probe_temperature",10),
    ::zhc::tuya::dp::numeric(4,"battery",1),
    ::zhc::tuya::dp::temperature(5,100),
    ::zhc::tuya::dp::numeric(101,"sampling_interval",1),
    ::zhc::tuya::dp::numeric(108,"probe_temperature_calibration",10),
    ::zhc::tuya::dp::numeric(109,"probe_temperature_v0_set",10),
    ::zhc::tuya::dp::numeric(110,"probe_temperature_v1_set",10),
    ::zhc::tuya::dp::enum_lookup(112,"probe_temperature_warning",kEnum_DTS1XM9_112,3),
    ::zhc::tuya::dp::numeric(114,"temperature_calibration",100),
    ::zhc::tuya::dp::numeric(115,"temperature_v0_set",100),
    ::zhc::tuya::dp::numeric(116,"temperature_v1_set",100),
    ::zhc::tuya::dp::enum_lookup(117,"temperature_warning",kEnum_DTS1XM9_117,3),
    ::zhc::tuya::dp::numeric(118,"humidity",100),
    ::zhc::tuya::dp::numeric(119,"humidity_calibration",100),
    ::zhc::tuya::dp::numeric(120,"humidity_v0_set",100),
    ::zhc::tuya::dp::numeric(121,"humidity_v1_set",100),
    ::zhc::tuya::dp::enum_lookup(122,"humidity_warning",kEnum_DTS1XM9_122,3),
    ::zhc::tuya::dp::numeric(124,"tds",1),
    ::zhc::tuya::dp::numeric(125,"tds_warning_set",1),
    ::zhc::tuya::dp::enum_lookup(126,"tds_warning",kEnum_DTS1XM9_126,2),
    ::zhc::tuya::dp::numeric(127,"ec",1),
    ::zhc::tuya::dp::numeric(128,"ec_v0_set",1),
    ::zhc::tuya::dp::numeric(129,"ec_v1_set",1),
    ::zhc::tuya::dp::enum_lookup(130,"ec_warning",kEnum_DTS1XM9_130,3),
    ::zhc::tuya::dp::numeric(131,"salinity",10),
    ::zhc::tuya::dp::numeric(132,"sg",1000),
    ::zhc::tuya::dp::enum_lookup(133,"mode",kEnum_DTS1XM9_133,2)
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_ZG_109TDS>;

constexpr const char* kOpts_ZG_109TDS_probe_temperature_warning[] = { "none", "low", "high" };
constexpr const char* kOpts_ZG_109TDS_temperature_warning[] = { "none", "low", "high" };
constexpr const char* kOpts_ZG_109TDS_humidity_warning[] = { "none", "low", "high" };
constexpr const char* kOpts_ZG_109TDS_tds_warning[] = { "none", "high" };
constexpr const char* kOpts_ZG_109TDS_ec_warning[] = { "none", "low", "high" };
constexpr const char* kOpts_ZG_109TDS_mode[] = { "freshwater", "seawater" };

constexpr Expose kExp_ZG_109TDS[] = {
    {"probe_temperature", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0},
    {"sampling_interval", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"probe_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"probe_temperature_v0_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"probe_temperature_v1_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"probe_temperature_warning", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_109TDS_probe_temperature_warning, 3},
    {"temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature_v0_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature_v1_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"temperature_warning", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_109TDS_temperature_warning, 3},
    {"humidity", ExposeType::Numeric, ::zhc::Access::StateSet, "%", nullptr, nullptr, 0},
    {"humidity_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity_v0_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity_v1_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"humidity_warning", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_109TDS_humidity_warning, 3},
    {"tds", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"tds_warning_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"tds_warning", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_109TDS_tds_warning, 2},
    {"ec", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ec_v0_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ec_v1_set", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ec_warning", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_109TDS_ec_warning, 3},
    {"salinity", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sg", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"mode", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, nullptr, kOpts_ZG_109TDS_mode, 2},
};

constexpr const char* kM_ZG_109TDS[] = { "Excellux" };
constexpr const char* kN_ZG_109TDS[] = { "DTS1XM9" };
}  // namespace

extern const PreparedDefinition kDef_ZG_109TDS{
    .zigbee_models=kM_ZG_109TDS,.zigbee_models_count=sizeof(kM_ZG_109TDS)/sizeof(kM_ZG_109TDS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_ZG_109TDS,.manufacturer_names_count=sizeof(kN_ZG_109TDS)/sizeof(kN_ZG_109TDS[0]),
    .model="ZG-109TDS",.vendor="Excellux",.meta=nullptr,
    .exposes=kExp_ZG_109TDS,.exposes_count=sizeof(kExp_ZG_109TDS)/sizeof(kExp_ZG_109TDS[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
