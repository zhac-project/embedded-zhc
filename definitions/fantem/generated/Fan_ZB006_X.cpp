// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Fantem ZB006-X — hand-rewritten 2026-04-29g sweep.
// Smart dimmer module.
//
// z2m-source: fantem.ts #ZB006-X.
//
// Wire format notes:
//   * Light surface (state + brightness) rides standard genOnOff /
//     genLevelCtrl read-back via kFzOnOff + kFzBrightness, and the
//     standard kTzOnOff + kTzBrightness write path.
//   * Action keys (`on`, `off`, `brightness_move_*`) come from
//     ClientToServer command frames on those clusters and are decoded
//     via the generic kFzCommandOn / kFzCommandOff /
//     kFzCommandMove / kFzCommandStop converters (action emit only).
//   * Tuya DP stream on manuSpecificTuya (0xEF00) carries dimmer
//     settings:
//       DP 101 fantemPowerSupplyMode  enum → power_supply_mode (R/O)
//       DP 103 fantemExtSwitchType    enum → switch_type        (R/W)
//       DP 105 fantemLoadDetectionMode enum → load_detection_mode (R/W)
//       DP 106 fantemExtSwitchStatus  numeric → switch_status   (R/O)
//       DP 109 fantemControlMode      enum → control_mode       (R/W)
//       DP 112 fantemLoadType         enum → load_type          (R/O)
//       DP 113 fantemLoadDimmable     enum → load_dimmable      (R/O)
//     DP 111 is silently ignored upstream (post-power-on housekeeping).

#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::fantem {
namespace {

using ::zhc::Access;
using ::zhc::BindingSpec;
using ::zhc::Expose;
using ::zhc::ExposeType;
using ::zhc::FzConverter;
using ::zhc::PreparedDefinition;
using ::zhc::TzConverter;

// ── Tuya DP map (fz + tz share the same table) ──────────────────────

constexpr ::zhc::tuya::TuyaEnumEntry kEnum_PowerSupplyMode[] = {
    {0, "unknown"},
    {1, "no_neutral"},
    {2, "with_neutral"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_SwitchType[] = {
    {0, "unknown"},
    {1, "toggle"},
    {2, "momentary"},
    {3, "rotary"},
    {4, "auto_config"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_LoadDetectionMode[] = {
    {0, "none"},
    {1, "first_power_on"},
    {2, "every_power_on"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_ControlMode[] = {
    {0, "ext_switch"},
    {1, "remote"},
    {2, "both"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_LoadType[] = {
    // z2m intentionally re-uses "unknown" for index 2 — see fantem.ts
    // source comment ("If you see 'unknown', pls. check with Tuya app").
    {0, "unknown"},
    {1, "resistive_capacitive"},
    {2, "unknown"},
    {3, "detecting"},
};
constexpr ::zhc::tuya::TuyaEnumEntry kEnum_LoadDimmable[] = {
    {0, "unknown"},
    {1, "dimmable"},
    {2, "not_dimmable"},
};

constexpr ::zhc::tuya::TuyaDpMapEntry kDpEntries_ZB006_X[] = {
    { 101, "power_supply_mode",   ::zhc::TuyaDpType::Enum,    1,
      kEnum_PowerSupplyMode,    sizeof(kEnum_PowerSupplyMode)   /sizeof(kEnum_PowerSupplyMode[0]),    0 },
    { 103, "switch_type",         ::zhc::TuyaDpType::Enum,    1,
      kEnum_SwitchType,         sizeof(kEnum_SwitchType)        /sizeof(kEnum_SwitchType[0]),         0 },
    { 105, "load_detection_mode", ::zhc::TuyaDpType::Enum,    1,
      kEnum_LoadDetectionMode,  sizeof(kEnum_LoadDetectionMode) /sizeof(kEnum_LoadDetectionMode[0]),  0 },
    { 106, "switch_status",       ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 109, "control_mode",        ::zhc::TuyaDpType::Enum,    1,
      kEnum_ControlMode,        sizeof(kEnum_ControlMode)       /sizeof(kEnum_ControlMode[0]),        0 },
    { 112, "load_type",           ::zhc::TuyaDpType::Enum,    1,
      kEnum_LoadType,           sizeof(kEnum_LoadType)          /sizeof(kEnum_LoadType[0]),           0 },
    { 113, "load_dimmable",       ::zhc::TuyaDpType::Enum,    1,
      kEnum_LoadDimmable,       sizeof(kEnum_LoadDimmable)      /sizeof(kEnum_LoadDimmable[0]),       0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kDpMap_ZB006_X{
    kDpEntries_ZB006_X,
    sizeof(kDpEntries_ZB006_X) / sizeof(kDpEntries_ZB006_X[0]),
};

constexpr FzConverter kFzDp_ZB006_X{
    .family            = ::zhc::FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kDpMap_ZB006_X,
};

constexpr TzConverter kTzDp_ZB006_X{
    .key         = nullptr,                       // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kDpMap_ZB006_X,
};

// ── Fz / Tz tables ──────────────────────────────────────────────────

const FzConverter* const kFz_ZB006_X[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ZB006_X,
};
const TzConverter* const kTz_ZB006_X[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &kTzDp_ZB006_X,
};

constexpr const char* kModels_ZB006_X[] = { "TS110F" };
constexpr const char* kManus_ZB006_X[]  = { "_TZ3210_lfbz816s", "_TZ3210_ebbfkvoy" };

constexpr Expose kExposes_ZB006_X[] = {
    {"state",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness",          ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action",              ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"control_mode",        ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_type",         ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_status",       ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
    {"load_detection_mode", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"load_type",           ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"load_dimmable",       ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"power_supply_mode",   ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB006_X[] = {
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
    {1, 0xEF00},  // manuSpecificTuya
};

}  // namespace

extern const PreparedDefinition kDef_ZB006_X{
    .zigbee_models=kModels_ZB006_X, .zigbee_models_count=sizeof(kModels_ZB006_X)/sizeof(kModels_ZB006_X[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZB006_X, .manufacturer_names_count=sizeof(kManus_ZB006_X)/sizeof(kManus_ZB006_X[0]),
    .model="ZB006-X", .vendor="Fantem",
    .meta=nullptr, .exposes=kExposes_ZB006_X, .exposes_count=sizeof(kExposes_ZB006_X)/sizeof(kExposes_ZB006_X[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZB006_X, .from_zigbee_count=sizeof(kFz_ZB006_X)/sizeof(kFz_ZB006_X[0]),
    .to_zigbee=kTz_ZB006_X,   .to_zigbee_count=sizeof(kTz_ZB006_X)/sizeof(kTz_ZB006_X[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBindings_ZB006_X, .bindings_count=sizeof(kBindings_ZB006_X)/sizeof(kBindings_ZB006_X[0]),
};

}  // namespace zhc::devices::fantem
