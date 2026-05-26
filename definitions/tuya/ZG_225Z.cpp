// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: ZG-225Z gas sensor (_TZE200_aj0oxo1i / _TZE200_p6fuhvez, zigbeeModel
// TS0225). z2m-source: tuya.ts meta.tuyaDatapoints —
//   [1 "gas" trueFalse0 (INVERTED: wire 0 = gas detected),
//    2 "gas_value" raw (ppm), 101 "sensitivity" {low,medium,high},
//    6 "ring" {ring1,ring2}].
// sensitivity/ring are STATE_SET in z2m; read-path only here (no tz wired).
// BEST-EFFORT: derived from the z2m converter, NOT verified against hardware
// frames. Inverted alarm bit via dp::binary_inv — pinned by test_tuya_alarms.cpp.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kSens_ZG225Z[]={ {0,"low"},{1,"medium"},{2,"high"} };
constexpr ::zhc::tuya::TuyaEnumEntry kRing_ZG225Z[]={ {0,"ring1"},{1,"ring2"} };
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary_inv(1,"gas"),                            // DP1  trueFalse0
    ::zhc::tuya::dp::numeric(2,"gas_value",1),                       // DP2  raw (ppm)
    ::zhc::tuya::dp::enum_lookup(101,"sensitivity",kSens_ZG225Z,3),  // DP101
    ::zhc::tuya::dp::enum_lookup(6,"ring",kRing_ZG225Z,2)};          // DP6
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,4}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0225"};
constexpr const char* kN[]={"_TZE200_aj0oxo1i","_TZE200_p6fuhvez"};
}
extern const PreparedDefinition kDefZG_225Z{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=2,.model="ZG-225Z",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
