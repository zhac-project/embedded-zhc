// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Zemismart ZMD-208-2 screen dimmer switch, 2 gang, TS0601
// _TZE28C1000000_gheewd2t (z2m v26.133.0 window). tuyaMagicPacket (no
// tuyaBase, so no time answer) + two dpLight gangs on EP1: DP1/7 state_l1/l2,
// DP2/8 brightness_l1/l2, 0..1000 on the wire, published 0..254 like z2m
// (`scale: [0, 254, 0, 1000]`) via float divisor 1000/254 both ways, as
// tuya/MG_DIM02Z.cpp.
// NOT wired: the per-gang screen names (DP106/107, `name_l1` / `name_l2`),
// latin1 text in a Raw DP that the datapoint encoder cannot write. Not
// reproduced either: dpLight's skip of the state DP when one message sets
// state + brightness; here each key is its own write.
// z2m-source: zemismart.ts #ZMD-208-2.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::zemismart {
namespace {

constexpr float kBrightnessScale = 1000.0f / 254.0f;  // z2m scale [0, 254, 0, 1000]

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(1, "state_l1"),
        { 2, "brightness_l1", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
        ::zhc::tuya::dp::binary(7, "state_l2"),
        { 8, "brightness_l2", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, kBrightnessScale },
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr Expose kExp[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l1", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 1},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_l2", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::State, 0, 254, 1},
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE28C1000000_gheewd2t" };

}  // namespace

extern const PreparedDefinition kDef_ZMD_208_2{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]), .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZMD-208-2", .vendor="Zemismart", .meta=nullptr,
    .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=nullptr, .on_event=nullptr,
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount };

}  // namespace zhc::devices::zemismart
