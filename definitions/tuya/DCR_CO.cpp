// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya DCR-CO smart air box / carbon-monoxide alarm (legacy tuya_CO).
//   DP1 carbon_monoxide (INVERTED: z2m `value ? OFF : ON`, so wire 0 = CO
//        detected/ON), DP2 co = value / 100 (ppm).
// BEST-EFFORT, not hardware-verified. Inverted alarm bit pinned in
// tests/test_tuya_alarms.cpp.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
namespace zhc::devices::tuya {
namespace {
struct cfg { static constexpr ::zhc::tuya::TuyaDpMapEntry e[]={
    ::zhc::tuya::dp::binary_inv(1,"carbon_monoxide"),  // DP1 trueFalse-style (0 = ON)
    ::zhc::tuya::dp::numeric(2,"co",100)};             // DP2 /100 ppm
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e,2}; };
using FX=::zhc::tuya::factory::TuyaOnOff<cfg>;
constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_7bztmfm1","_TZE204_7bztmfm1"};
}
extern const PreparedDefinition kDefDCR_CO{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=2,.model="DCR-CO",
    .vendor="Tuya",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
