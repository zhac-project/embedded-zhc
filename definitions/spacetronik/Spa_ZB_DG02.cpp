// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Spacetronik ZB-DG02 gas leakage sensor (TS0601 / _TZE204_uc0iv1hb).
//
// Bug fixed: the auto stub lowered the generic kFzIasZone converter (cluster
// ssIasZone 0x0500) emitting bare alarm/tamper/battery_low. The device speaks
// NONE of that — z2m decodes it via tuya.modernExtend.tuyaBase({dp:true}) +
// a single Tuya datapoint [1, "gas", trueFalse0] on the 0xEF00 manuSpecificTuya
// DP stream. The IAS-vs-DP misroute left the only real channel (gas) dead and
// the phantom IAS surface unbacked. Graduated to a Tier-2 override wiring
// fz_tuya_datapoints with the DP map, the `gas` expose, and a 0xEF00 binding.
//
// z2m's `trueFalse0` reports gas==true when the wire DP value is 0, so the DP
// entry uses dp::binary_inv (kTuyaDpFlagInvertBool), NOT plain dp::binary.
//
// z2m-source: spacetronik.ts #ZB-DG02 (tuya.valueConverter.trueFalse0).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::spacetronik {
namespace {

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary_inv(1, "gas"),  // z2m [1,"gas",trueFalse0]
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{e, 1};
};
using FX = ::zhc::tuya::factory::TuyaOnOff<cfg>;

constexpr const char* kModels_ZB_DG02[] = { "TS0601" };
constexpr const char* kMfr_ZB_DG02[]    = { "_TZE204_uc0iv1hb" };

constexpr Expose kAutoExposes[] = {
    {"gas", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};

}  // namespace

extern const PreparedDefinition kDef_ZB_DG02{
    .zigbee_models=kModels_ZB_DG02, .zigbee_models_count=sizeof(kModels_ZB_DG02)/sizeof(kModels_ZB_DG02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kMfr_ZB_DG02, .manufacturer_names_count=sizeof(kMfr_ZB_DG02)/sizeof(kMfr_ZB_DG02[0]),
    .model="ZB-DG02", .vendor="Spacetronik",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::spacetronik
