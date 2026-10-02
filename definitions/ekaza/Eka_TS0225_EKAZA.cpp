// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Ekaza TS0225_EKAZA 24 GHz presence sensor (z2m v26.115.1 window, new
// vendor). TS0225 `_TZ3210_eep3fewj`.
//
// Ported: presence from the IAS zone (alarm_1 -> occupancy) and the two Tuya
// datapoints, 101 presence_delay (s) and 104 illuminance (raw).
// Not ported, see the R8 worklist: z2m writes these datapoints with the Tuya
// `sendData` command (0x04), which the datapoint encoder here does not emit,
// so presence_delay is read-only; and detection_distance lives on
// manuSpecificTuya2 (0xE002) attribute 0xE00B, read on demand only.
// z2m-source: ekaza.ts #TS0225_EKAZA.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::ekaza {
namespace {
struct cfg {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(101, "presence_delay", 1),
        ::zhc::tuya::dp::numeric(104, "illuminance", 1),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e)/sizeof(e[0]) };
};
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::tuya::factory::detail::BoundFz<cfg>::converter,
};
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, nullptr, "Raw illuminance reported by the sensor", nullptr, 0},
    {"presence_delay", ExposeType::Numeric, Access::State, "s",
     "Delay before reporting absence after presence is no longer detected", nullptr, 0,
     ExposeCategory::State, 1, 300, 1},
};
constexpr const char* kM[] = { "TS0225" };
constexpr const char* kN[] = { "_TZ3210_eep3fewj" };
constexpr std::uint8_t kReadZoneStatus[] = { 0x02, 0x00 };
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0x0500, 0x00, 0, kReadZoneStatus, sizeof(kReadZoneStatus), 0 },
};
}  // namespace

extern const PreparedDefinition kDef_TS0225_EKAZA{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="TS0225_EKAZA", .vendor="Ekaza",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::ekaza
