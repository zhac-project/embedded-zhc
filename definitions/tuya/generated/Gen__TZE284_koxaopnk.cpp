// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated from z2m devices/tuya.ts (fingerprint TS0601 / _TZE284_koxaopnk).
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE284_koxaopnk_state_dp1[] = {
    { 2, "OPEN" },
    { 1, "STOP" },
    { 0, "CLOSE" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE284_koxaopnk[] = {
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE284_koxaopnk_state_dp1, sizeof(kEnum__TZE284_koxaopnk_state_dp1)/sizeof(kEnum__TZE284_koxaopnk_state_dp1[0]) },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE284_koxaopnk{ kEntries__TZE284_koxaopnk, 3 };
constexpr FzConverter kFzDp__TZE284_koxaopnk{
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
    .user_config       = &kMap__TZE284_koxaopnk,
};
const FzConverter* const kFz__TZE284_koxaopnk[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE284_koxaopnk,
};
constexpr TzConverter kTzDp__TZE284_koxaopnk{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE284_koxaopnk,
};
const TzConverter* const kTz__TZE284_koxaopnk[] = { &kTzDp__TZE284_koxaopnk };
constexpr const char* kM__TZE284_koxaopnk[] = { "TS0601" };
constexpr const char* kN__TZE284_koxaopnk[] = { "_TZE284_koxaopnk" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts__TZE284_koxaopnk_0[] = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kAutoOpts__TZE284_koxaopnk_2[] = {"normal", "reversed"};
constexpr Expose kAutoExposes__TZE284_koxaopnk[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_koxaopnk_0, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0, ExposeCategory::State, 0, 100, 0},
    {"motor_direction", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts__TZE284_koxaopnk_2, 2, ExposeCategory::Config},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
};
// --- end auto-generated exposes ---
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE284_koxaopnk[] = {
    {"Nova Digital","ZC-GM42"},
    {"Moes","ZC-LS02"},
};
extern const PreparedDefinition kDefGen__TZE284_koxaopnk{
    .zigbee_models=kM__TZE284_koxaopnk,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE284_koxaopnk,.manufacturer_names_count=1,
    .model="TS0601__TZE284_koxaopnk",.vendor="Tuya",
    .meta=nullptr,.exposes=kAutoExposes__TZE284_koxaopnk,.exposes_count=sizeof(kAutoExposes__TZE284_koxaopnk)/sizeof(kAutoExposes__TZE284_koxaopnk[0]),
    .white_labels=kWhiteLabels_Gen_TZE284_koxaopnk, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE284_koxaopnk)/sizeof(kWhiteLabels_Gen_TZE284_koxaopnk[0]),
    .from_zigbee=kFz__TZE284_koxaopnk,
    .from_zigbee_count=sizeof(kFz__TZE284_koxaopnk)/sizeof(kFz__TZE284_koxaopnk[0]),
    .to_zigbee=kTz__TZE284_koxaopnk,
    .to_zigbee_count=sizeof(kTz__TZE284_koxaopnk)/sizeof(kTz__TZE284_koxaopnk[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr };
}  // namespace zhc::devices::tuya
