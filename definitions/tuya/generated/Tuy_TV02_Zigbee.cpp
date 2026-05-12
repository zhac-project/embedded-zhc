// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TV02-Zigbee — auto-generated.
// Thermostat radiator valve
// z2m-source: tuya.ts #TV02-Zigbee.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TV02_Zigbee[] = { "TS0601" };
constexpr const char* kManus_TV02_Zigbee[] = { "_TZE200_sur6q7ko", "_TZE200_hue3yfsn", "_TZE200_e9ba97vf", "_TZE200_husqqvux", "_TZE200_lnbfnyxd", "_TZE200_fsow0qsk", "_TZE200_lllliz3p", "_TZE200_mudxchsu", "_TZE200_7yoranx2", "_TZE200_kds0pmmv", "_TZE200_py4cm3he", "_TZE200_wsbfwodu", "_TZE200_x9axofse", "_TZE200_lhzapfg9", "_TZE200_k1tumq4t" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TV02_Zigbee_Tuy_TV02_Zigbee[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TV02_Zigbee_Tuy_TV02_Zigbee[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TV02_Zigbee_Tuy_TV02_Zigbee[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TV02_Zigbee{
    .zigbee_models=kModels_TV02_Zigbee, .zigbee_models_count=sizeof(kModels_TV02_Zigbee)/sizeof(kModels_TV02_Zigbee[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TV02_Zigbee, .manufacturer_names_count=sizeof(kManus_TV02_Zigbee)/sizeof(kManus_TV02_Zigbee[0]),
    .model="TV02-Zigbee", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TV02_Zigbee_Tuy_TV02_Zigbee, .exposes_count=sizeof(kExp_min_TV02_Zigbee_Tuy_TV02_Zigbee)/sizeof(kExp_min_TV02_Zigbee_Tuy_TV02_Zigbee[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TV02_Zigbee_Tuy_TV02_Zigbee, .from_zigbee_count=sizeof(kFz_min_TV02_Zigbee_Tuy_TV02_Zigbee)/sizeof(kFz_min_TV02_Zigbee_Tuy_TV02_Zigbee[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
