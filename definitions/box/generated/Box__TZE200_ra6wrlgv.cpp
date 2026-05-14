// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: BOX ERC2206-Z (TS0601 / _TZE200_ra6wrlgv) — roller shutter controller.
// Hand-spot-checked against z2m devices/box.ts during 2026-04-28 BOX sweep.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::box {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ra6wrlgv_state_dp1[] = {
    { 0, "open" },
    { 1, "stop" },
    { 2, "close" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ra6wrlgv_direction_dp5[] = {
    { 0, "forward" },
    { 1, "back" },
};

// dp 0x65/0x66: z2m models these as bool-typed dpEnumLookups (record=true, stop=false).
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ra6wrlgv_record_rf_dp101[] = {
    { 1, "record" },
    { 0, "stop" },
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_ra6wrlgv_clear_rf_dp102[] = {
    { 1, "clear" },
    { 0, "stop" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_ra6wrlgv[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ra6wrlgv_state_dp1, sizeof(kEnum__TZE200_ra6wrlgv_state_dp1)/sizeof(kEnum__TZE200_ra6wrlgv_state_dp1[0]) },
    { 5, "direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ra6wrlgv_direction_dp5, sizeof(kEnum__TZE200_ra6wrlgv_direction_dp5)/sizeof(kEnum__TZE200_ra6wrlgv_direction_dp5[0]) },
    { 101, "record_rf", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ra6wrlgv_record_rf_dp101, sizeof(kEnum__TZE200_ra6wrlgv_record_rf_dp101)/sizeof(kEnum__TZE200_ra6wrlgv_record_rf_dp101[0]) },
    { 102, "clear_rf", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_ra6wrlgv_clear_rf_dp102, sizeof(kEnum__TZE200_ra6wrlgv_clear_rf_dp102)/sizeof(kEnum__TZE200_ra6wrlgv_clear_rf_dp102[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_ra6wrlgv{ kEntries__TZE200_ra6wrlgv, sizeof(kEntries__TZE200_ra6wrlgv)/sizeof(kEntries__TZE200_ra6wrlgv[0]) };
constexpr FzConverter kFzDp__TZE200_ra6wrlgv{
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
    .user_config       = &kMap__TZE200_ra6wrlgv,
};
const FzConverter* const kFz__TZE200_ra6wrlgv[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_ra6wrlgv,
};
constexpr TzConverter kTzDp__TZE200_ra6wrlgv{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_ra6wrlgv,
};
const TzConverter* const kTz__TZE200_ra6wrlgv[] = { &kTzDp__TZE200_ra6wrlgv };
constexpr const char* kM__TZE200_ra6wrlgv[] = { "TS0601" };
constexpr const char* kN__TZE200_ra6wrlgv[] = { "_TZE200_ra6wrlgv" };
}  // namespace

// --- hand-tuned exposes (BOX sweep 2026-04-28p) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"record_rf", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"clear_rf", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end hand-tuned block ---

extern const PreparedDefinition kDefBox__TZE200_ra6wrlgv{
    .zigbee_models=kM__TZE200_ra6wrlgv,.zigbee_models_count=sizeof(kM__TZE200_ra6wrlgv)/sizeof(kM__TZE200_ra6wrlgv[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_ra6wrlgv,.manufacturer_names_count=1,
    .model="ERC2206-Z",.vendor="BOX",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_ra6wrlgv,
    .from_zigbee_count=sizeof(kFz__TZE200_ra6wrlgv)/sizeof(kFz__TZE200_ra6wrlgv[0]),
    .to_zigbee=kTz__TZE200_ra6wrlgv,
    .to_zigbee_count=sizeof(kTz__TZE200_ra6wrlgv)/sizeof(kTz__TZE200_ra6wrlgv[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::box
