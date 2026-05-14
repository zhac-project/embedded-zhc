// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: BOX ERC2202-Z (TS0601 / _TZE200_o7vduidq) — dual controller.
// Hand-spot-checked against z2m devices/box.ts during 2026-04-28 BOX sweep.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::box {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_o7vduidq_record_rf_dp102[] = {
    { 0, "record_l1" },
    { 1, "record_l2" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_o7vduidq_clear_rf_dp102[] = {
    { 2, "clear_l1" },
    { 3, "clear_l2" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_o7vduidq_rf_status_dp103[] = {
    { 0, "ok" },
    { 1, "error" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_o7vduidq[] = {
    { 19, "state_l1", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 20, "state_l2", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 102, "record_rf", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_o7vduidq_record_rf_dp102, sizeof(kEnum__TZE200_o7vduidq_record_rf_dp102)/sizeof(kEnum__TZE200_o7vduidq_record_rf_dp102[0]) },
    { 102, "clear_rf", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_o7vduidq_clear_rf_dp102, sizeof(kEnum__TZE200_o7vduidq_clear_rf_dp102)/sizeof(kEnum__TZE200_o7vduidq_clear_rf_dp102[0]) },
    { 103, "rf_status", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_o7vduidq_rf_status_dp103, sizeof(kEnum__TZE200_o7vduidq_rf_status_dp103)/sizeof(kEnum__TZE200_o7vduidq_rf_status_dp103[0]) },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_o7vduidq{ kEntries__TZE200_o7vduidq, 5 };
constexpr FzConverter kFzDp__TZE200_o7vduidq{
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
    .user_config       = &kMap__TZE200_o7vduidq,
};
const FzConverter* const kFz__TZE200_o7vduidq[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_o7vduidq,
};
constexpr TzConverter kTzDp__TZE200_o7vduidq{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_o7vduidq,
};
const TzConverter* const kTz__TZE200_o7vduidq[] = { &kTzDp__TZE200_o7vduidq };
constexpr const char* kM__TZE200_o7vduidq[] = { "TS0601" };
constexpr const char* kN__TZE200_o7vduidq[] = { "_TZE200_o7vduidq" };
}  // namespace

// --- hand-tuned exposes (BOX sweep 2026-04-28p) ---
constexpr Expose kAutoExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"record_rf", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"clear_rf", ExposeType::Enum, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rf_status", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0xEF00},
};
// --- end hand-tuned block ---

extern const PreparedDefinition kDefBox__TZE200_o7vduidq{
    .zigbee_models=kM__TZE200_o7vduidq,.zigbee_models_count=sizeof(kM__TZE200_o7vduidq)/sizeof(kM__TZE200_o7vduidq[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_o7vduidq,.manufacturer_names_count=1,
    .model="ERC2202-Z",.vendor="BOX",
    .meta=nullptr,.exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz__TZE200_o7vduidq,
    .from_zigbee_count=sizeof(kFz__TZE200_o7vduidq)/sizeof(kFz__TZE200_o7vduidq[0]),
    .to_zigbee=kTz__TZE200_o7vduidq,
    .to_zigbee_count=sizeof(kTz__TZE200_o7vduidq)/sizeof(kTz__TZE200_o7vduidq[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::box
