// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya ZRF01 Zigbee RF cloner with 8 slots, TS0601 _TZE284_tdg4ckyh
// (z2m v26.125.0 window). 117 state / 118 pack bools, 119 illuminance,
// 101-108 button_N {Learn 0, Saved 1, Delete 2}, 109-116 start_N {Transmit 0}
// (write-only upstream; a report would decode to the same label).
// z2m-source: tuya.ts #ZRF01.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {
constexpr ::zhc::tuya::TuyaEnumEntry kButton[] = { {0, "Learn"}, {1, "Saved"}, {2, "Delete"} };
constexpr ::zhc::tuya::TuyaEnumEntry kStart[] = { {0, "Transmit"} };
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::binary(117, "state"),
        ::zhc::tuya::dp::binary(118, "pack"),
        ::zhc::tuya::dp::numeric(119, "illuminance", 1),
        ::zhc::tuya::dp::enum_lookup(101, "button_1", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(102, "button_2", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(103, "button_3", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(104, "button_4", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(105, "button_5", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(106, "button_6", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(107, "button_7", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(108, "button_8", kButton, 3),
        ::zhc::tuya::dp::enum_lookup(109, "start_1", kStart, 1),
        ::zhc::tuya::dp::enum_lookup(110, "start_2", kStart, 1),
        ::zhc::tuya::dp::enum_lookup(111, "start_3", kStart, 1),
        ::zhc::tuya::dp::enum_lookup(112, "start_4", kStart, 1),
        ::zhc::tuya::dp::enum_lookup(113, "start_5", kStart, 1),
        ::zhc::tuya::dp::enum_lookup(114, "start_6", kStart, 1),
        ::zhc::tuya::dp::enum_lookup(115, "start_7", kStart, 1),
        ::zhc::tuya::dp::enum_lookup(116, "start_8", kStart, 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;
constexpr const char* kButtonOpts[] = { "Learn", "Delete", "Saved" };
constexpr const char* kStartOpts[] = { "Transmit" };
constexpr Expose kExp[] = {
    {"illuminance", ExposeType::Numeric, Access::State, "lx", nullptr, nullptr, 0},
    {"state", ExposeType::Binary, Access::StateSet, nullptr, "Enable RF learning mode; needed before Learn on any button", nullptr, 0},
    {"pack", ExposeType::Binary, Access::StateSet, nullptr, "Long packet mode (on) or short packet mode (off)", nullptr, 0},
    {"button_1", ExposeType::Enum, Access::StateSet, nullptr, "Button 1 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"button_2", ExposeType::Enum, Access::StateSet, nullptr, "Button 2 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"button_3", ExposeType::Enum, Access::StateSet, nullptr, "Button 3 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"button_4", ExposeType::Enum, Access::StateSet, nullptr, "Button 4 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"button_5", ExposeType::Enum, Access::StateSet, nullptr, "Button 5 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"button_6", ExposeType::Enum, Access::StateSet, nullptr, "Button 6 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"button_7", ExposeType::Enum, Access::StateSet, nullptr, "Button 7 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"button_8", ExposeType::Enum, Access::StateSet, nullptr, "Button 8 code learning: Learn, Delete; Saved after a successful clone", kButtonOpts, 3},
    {"start_1", ExposeType::Enum, Access::Set, nullptr, "Button 1: Transmit sends the saved code", kStartOpts, 1},
    {"start_2", ExposeType::Enum, Access::Set, nullptr, "Button 2: Transmit sends the saved code", kStartOpts, 1},
    {"start_3", ExposeType::Enum, Access::Set, nullptr, "Button 3: Transmit sends the saved code", kStartOpts, 1},
    {"start_4", ExposeType::Enum, Access::Set, nullptr, "Button 4: Transmit sends the saved code", kStartOpts, 1},
    {"start_5", ExposeType::Enum, Access::Set, nullptr, "Button 5: Transmit sends the saved code", kStartOpts, 1},
    {"start_6", ExposeType::Enum, Access::Set, nullptr, "Button 6: Transmit sends the saved code", kStartOpts, 1},
    {"start_7", ExposeType::Enum, Access::Set, nullptr, "Button 7: Transmit sends the saved code", kStartOpts, 1},
    {"start_8", ExposeType::Enum, Access::Set, nullptr, "Button 8: Transmit sends the saved code", kStartOpts, 1},
};
constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE284_tdg4ckyh" };
}  // namespace

extern const PreparedDefinition kDef_ZRF01{
    .zigbee_models=kM,.zigbee_models_count=sizeof(kM)/sizeof(kM[0]),.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="ZRF01",.vendor="Tuya",.meta=nullptr,
    .exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };

}  // namespace zhc::devices::tuya
