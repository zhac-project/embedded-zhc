// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: IOTPerfect PF-PM02D-TYZ — Smart water/gas valve
//   (TS0601 / 6× _TZE2xx Tuya-MCU 0xEF00 DP-stream device).
//
// The auto extract mis-routed this Tuya-MCU (0xEF00 manuSpecificTuya) valve
// as a bare kFzOnOff / kTzOnOff stub bound to ZCL genOnOff cluster 0x0006 —
// a cluster a TS0601/_TZE* device never speaks. The single `state` expose was
// therefore DEAD: the valve reports its open/closed state and accepts its
// open/close command only through the 0xEF00 DP stream. z2m decodes via
// legacy.fromZigbee.tuya_switch / legacy.toZigbee.tuya_switch_state, which
// read/write a single boolean datapoint:
//   DP 1   state   bool -> "ON"/"OFF"   (value ? "ON" : "OFF")
// Single endpoint (no model.meta.multiEndpoint), so DP 1 == `state` directly.
//
// Graduated to a Tier-2 override wiring fz_tuya_datapoints + tz_tuya_datapoints
// over the 0xEF00 DP map (mirrors the GiEX QT06 valve pattern), bound to
// cluster 0xEF00 with the shared tuya_base_configure(). The state DP is fanned
// to the ON/OFF string label via kTuyaDpFlagBoolEnum, matching z2m's binary
// expose string publish.
//
// z2m-source: iotperfect.ts #PF-PM02D-TYZ + lib/legacy.ts tuya_switch /
//   tuya_switch_state (dataPoints.state == 1).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::iotperfect {
namespace {

// z2m: state = value ? "ON" : "OFF" (e.switch() binary publishes the string).
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_PF_PM02D_state_dp1[] = {
    { 0, "OFF" },
    { 1, "ON" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_PF_PM02D_TYZ[] = {
    { 1, "state", ::zhc::TuyaDpType::Bool, 1, kEnum_PF_PM02D_state_dp1,
      sizeof(kEnum_PF_PM02D_state_dp1)/sizeof(kEnum_PF_PM02D_state_dp1[0]),
      ::zhc::tuya::kTuyaDpFlagBoolEnum },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_PF_PM02D_TYZ{
    kEntries_PF_PM02D_TYZ,
    sizeof(kEntries_PF_PM02D_TYZ)/sizeof(kEntries_PF_PM02D_TYZ[0]) };

constexpr FzConverter kFzDp_PF_PM02D_TYZ{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap_PF_PM02D_TYZ,
};
const FzConverter* const kFz_PF_PM02D_TYZ[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_PF_PM02D_TYZ,
};
constexpr TzConverter kTzDp_PF_PM02D_TYZ{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_PF_PM02D_TYZ,
};
const TzConverter* const kTz_PF_PM02D_TYZ[] = { &kTzDp_PF_PM02D_TYZ };

constexpr const char* kModels_PF_PM02D_TYZ[] = { "TS0601" };
constexpr const char* kManus_PF_PM02D_TYZ[] = {
    "_TZE200_vrjkcam9", "_TZE200_d0ypnbvn", "_TZE204_v5xjyphj",
    "_TZE204_d0ypnbvn", "_TZE284_v5xjyphj", "_TZE284_d0ypnbvn" };

// z2m: exposes:[e.switch().setAccess("state", ea.STATE_SET)].
constexpr Expose kExp_PF_PM02D_TYZ[] = {
    { "state", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_PF_PM02D_TYZ[] = {
    { 1, 0xEF00 },
};
}  // namespace

extern const PreparedDefinition kDef_PF_PM02D_TYZ{
    .zigbee_models=kModels_PF_PM02D_TYZ, .zigbee_models_count=sizeof(kModels_PF_PM02D_TYZ)/sizeof(kModels_PF_PM02D_TYZ[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_PF_PM02D_TYZ, .manufacturer_names_count=sizeof(kManus_PF_PM02D_TYZ)/sizeof(kManus_PF_PM02D_TYZ[0]),
    .model="PF-PM02D-TYZ", .vendor="Iotperfect",
    .meta=nullptr, .exposes=kExp_PF_PM02D_TYZ, .exposes_count=sizeof(kExp_PF_PM02D_TYZ)/sizeof(kExp_PF_PM02D_TYZ[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PF_PM02D_TYZ, .from_zigbee_count=sizeof(kFz_PF_PM02D_TYZ)/sizeof(kFz_PF_PM02D_TYZ[0]),
    .to_zigbee=kTz_PF_PM02D_TYZ, .to_zigbee_count=sizeof(kTz_PF_PM02D_TYZ)/sizeof(kTz_PF_PM02D_TYZ[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_PF_PM02D_TYZ, .bindings_count=sizeof(kBind_PF_PM02D_TYZ)/sizeof(kBind_PF_PM02D_TYZ[0]),
};

}  // namespace zhc::devices::iotperfect
