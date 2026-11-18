// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Girier TS0601_GIRIER ceiling presence sensor, TS0601
// _TZE28C1000000_ntqql63p (z2m v26.133.0 window).
// DP1 fans out like upstream's `from`: presence and occupancy are true for
// 1 (presence) or 2 (sensor_close), presence_state is the label. DP106
// presence_delay raw s. tuyaSendCommand "dataRequest" is cmd 0x00, the
// default here.
// NOT wired: DP117 detection_distance, a 10-byte zone mask <-> 0.7 m steps
// that the datapoint encoder cannot write (Raw); queryOnConfigure's DP query
// (tuya_base_configure is a no-op, as for Nov_ZCMR_1.cpp).
// z2m-source: girier.ts #TS0601_GIRIER.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::girier {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kPresent[] = { {1, "presence"}, {2, "sensor_close"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPresenceState[] = { {0, "absence"}, {1, "presence"}, {2, "sensor_close"} };

struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 1, "presence",  ::zhc::TuyaDpType::Enum, 1, kPresent, 2, ::zhc::tuya::kTuyaDpFlagEnumBool },
        { 1, "occupancy", ::zhc::TuyaDpType::Enum, 1, kPresent, 2, ::zhc::tuya::kTuyaDpFlagEnumBool },
        ::zhc::tuya::dp::enum_lookup(1, "presence_state", kPresenceState, 3),
        ::zhc::tuya::dp::numeric(106, "presence_delay"),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kPresenceStateOpts[] = { "absence", "presence", "sensor_close" };
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected occupancy", nullptr, 0},
    {"presence", ExposeType::Binary, Access::State, nullptr, "Indicates whether the device detected presence", nullptr, 0},
    {"presence_state", ExposeType::Enum, Access::State, nullptr, "Presence state reported by the sensor", kPresenceStateOpts, 3},
    {"presence_delay", ExposeType::Numeric, Access::StateSet, "s", "Delay before reporting absence", nullptr, 0,
     ExposeCategory::State, 0, 1800, 1},
};

constexpr const char* kM[] = { "TS0601" };
constexpr const char* kN[] = { "_TZE28C1000000_ntqql63p" };

}  // namespace

extern const PreparedDefinition kDef_TS0601_GIRIER{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]), .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0601_GIRIER", .vendor="Girier", .meta=nullptr,
    .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::girier
