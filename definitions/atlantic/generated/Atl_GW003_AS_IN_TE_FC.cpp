// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Atlantic GW003-AS-IN-TE-FC — hand-rewritten 2026-04-29a sweep.
// Interface Naviclim for Takao air conditioners.
//
// z2m-source: atlantic.ts #GW003-AS-IN-TE-FC.
//
// Wire format notes:
//   * Default device endpoint is 1 for all hvacFanCtrl/hvacThermostat
//     traffic. z2m additionally binds endpoint 232 → haDiagnostic
//     (a Philips manuSpecificPhilips2 quirk for IRP companion); the
//     ZDO bind is registered, but no Fz on this firmware emits anything
//     interesting from it.
//   * Four manuSpec writes target manufacturerCode = 0x125B
//     (ATLANTIC_GROUP):
//        - hvacFanCtrl  attr 0x1000 (4096) BOOL  → quiet_fan
//        - hvacThermostat attr 0x4273 (17011) ENUM8 → ac_louver_position
//        - hvacThermostat attr 0x4274 (17012) BOOL  → swing_mode
//        - hvacThermostat attr 0x4270 (17008) BOOL  → preset[boost]
//     The full TS `preset` converter coordinates three writes
//     (activity attr 0x4275, programingOperMode std attr, boost attr
//     0x4270). Only the boost leg is wired here — see PARITY doc.

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::atlantic {

namespace {

using ::zhc::Access;
using ::zhc::BindingSpec;
using ::zhc::Expose;
using ::zhc::ExposeType;
using ::zhc::FzConverter;
using ::zhc::PreparedDefinition;
using ::zhc::TzConverter;
using ::zhc::generic::ZclWriteSpec;

constexpr std::uint16_t kAtlanticMfgCode = 0x125B;

// hvacThermostat attribute ids (manuSpec, ATLANTIC_GROUP).
constexpr std::uint16_t kAttrPresetBoost   = 0x4270;  // 17008
constexpr std::uint16_t kAttrLouverPos     = 0x4273;  // 17011
constexpr std::uint16_t kAttrSwingMode     = 0x4274;  // 17012
// hvacFanCtrl attribute id (manuSpec, ATLANTIC_GROUP).
constexpr std::uint16_t kAttrQuietFan      = 0x1000;  // 4096

// ── enum / lookup tables ───────────────────────────────────────────

// ac_louver_position (TS thermostatPositions): 1..4.
// SPA-friendly labels also accepted; SPA may submit either label or u8.
constexpr ::zhc::generic::ZclWriteLookup kLookupLouverPos[] = {
    {"quarter_open",        1},
    {"half_open",            2},
    {"three_quarters_open",  3},
    {"fully_open",           4},
};

// ── ZclWriteSpec table ──────────────────────────────────────────────

constexpr ZclWriteSpec kSpecQuietFan{
    "quiet_fan",         kAttrQuietFan,    0x10, kAtlanticMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecLouverPos{
    "ac_louver_position", kAttrLouverPos,  0x30, kAtlanticMfgCode,
    kLookupLouverPos, sizeof(kLookupLouverPos)/sizeof(kLookupLouverPos[0]),
};
constexpr ZclWriteSpec kSpecSwingMode{
    "swing_mode",        kAttrSwingMode,   0x10, kAtlanticMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecPresetBoost{
    "preset_boost",      kAttrPresetBoost, 0x10, kAtlanticMfgCode, nullptr, 0,
};
// 2026-04-29c: 2nd + 3rd legs of z2m's `preset` Tz now wired
// (was BLOCKED on multi-frame Tz). Activity is mfg ENUM8 attr
// 0x4275; eco rides standard programingOperMode (attr 0x0025).
// Caller toggles each leg independently — atomic preset
// switching (which clears other legs) requires runtime
// multi-frame Tz support (deferred).
constexpr std::uint16_t kAttrPresetActivity     = 0x4275;
constexpr std::uint16_t kAttrProgramingOperMode = 0x0025;
constexpr ZclWriteSpec kSpecPresetActivity{
    "preset_activity",   kAttrPresetActivity, 0x30 /* ENUM8 */, kAtlanticMfgCode,
    nullptr, 0,
};
constexpr ZclWriteSpec kSpecPresetEcoOper{
    "preset_eco_oper_mode", kAttrProgramingOperMode, 0x30 /* ENUM8 */, 0,
    nullptr, 0,
};

// ── per-attribute Tz converters ─────────────────────────────────────

extern const TzConverter kTzAtlanticQuietFan{
    .key         = "quiet_fan",
    .cluster     = "hvacFanCtrl",
    .cluster_id  = 0x0202,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecQuietFan,
};

extern const TzConverter kTzAtlanticLouverPos{
    .key         = "ac_louver_position",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecLouverPos,
};

extern const TzConverter kTzAtlanticSwingMode{
    .key         = "swing_mode",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecSwingMode,
};

extern const TzConverter kTzAtlanticPresetBoost{
    .key         = "preset_boost",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecPresetBoost,
};

extern const TzConverter kTzAtlanticPresetActivity{
    .key         = "preset_activity",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecPresetActivity,
};

extern const TzConverter kTzAtlanticPresetEcoOper{
    .key         = "preset_eco_oper_mode",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpecPresetEcoOper,
};

// ── enum value strings for exposes ──────────────────────────────────

constexpr const char* kSystemModeValues[] = {
    "off", "heat", "cool", "auto", "dry", "fan_only",
};
constexpr const char* kFanModeValues[] = {
    "off", "low", "medium", "high", "auto",
};
constexpr const char* kSwingModeValues[] = {
    "on", "off",
};
constexpr const char* kPresetBoostValues[] = {
    "true", "false",
};
constexpr const char* kLouverPosValues[] = {
    "quarter_open", "half_open", "three_quarters_open", "fully_open",
};

// ── converter tables ────────────────────────────────────────────────

const FzConverter* const kFz_GW003_AS_IN_TE_FC[] = {
    &::zhc::generic::kFzThermostat,
    &::zhc::generic::kFzFanMode,
};
const TzConverter* const kTz_GW003_AS_IN_TE_FC[] = {
    // kTzThermostat covers occupied_heating_setpoint +
    // current_heating_setpoint + system_mode (off/auto/cool/heat).
    // Cool setpoint, dry/fan_only modes, programming_operation_mode
    // and tz.fan_mode are not yet covered by generic Tz — see PARITY
    // doc for the gap list.
    &::zhc::generic::kTzThermostat,
    &kTzAtlanticQuietFan,
    &kTzAtlanticLouverPos,
    &kTzAtlanticSwingMode,
    &kTzAtlanticPresetBoost,
    &kTzAtlanticPresetActivity,
    &kTzAtlanticPresetEcoOper,
};
constexpr const char* kModels_GW003_AS_IN_TE_FC[] = { "Adapter Zigbee FUJITSU" };

// ── exposes — hand-aligned to z2m ──────────────────────────────────

constexpr Expose kAutoExposes[] = {
    {"local_temperature",          ExposeType::Numeric, Access::State,    "C",
        nullptr, nullptr, 0},
    {"current_heating_setpoint",   ExposeType::Numeric, Access::StateSet, "C",
        nullptr, nullptr, 0},
    {"occupied_cooling_setpoint",  ExposeType::Numeric, Access::StateSet, "C",
        "z2m: setpoint 18..30 step 0.5 — outbound write not yet wired (kTzThermostat is heat-only)",
        nullptr, 0},
    {"system_mode",                ExposeType::Enum,    Access::StateSet, nullptr,
        "off / heat / cool / auto / dry / fan_only — kTzThermostat covers off/heat/cool/auto only",
        kSystemModeValues, sizeof(kSystemModeValues)/sizeof(kSystemModeValues[0])},
    {"fan_mode",                   ExposeType::Enum,    Access::StateSet, nullptr,
        "low/medium/high/on/auto — outbound tz.fan_mode (hvacFanCtrl::fanMode write) not yet generic; only kFzFanMode reads",
        kFanModeValues, sizeof(kFanModeValues)/sizeof(kFanModeValues[0])},
    {"swing_mode",                 ExposeType::Enum,    Access::StateSet, nullptr,
        "Atlantic manuSpec attr 0x4274 (BOOL on/off)",
        kSwingModeValues, sizeof(kSwingModeValues)/sizeof(kSwingModeValues[0])},
    {"ac_louver_position",         ExposeType::Enum,    Access::StateSet, nullptr,
        "Atlantic manuSpec attr 0x4273 (ENUM8 1..4)",
        kLouverPosValues, sizeof(kLouverPosValues)/sizeof(kLouverPosValues[0])},
    {"quiet_fan",                  ExposeType::Binary,  Access::StateSet, nullptr,
        "Fan quiet mode — Atlantic manuSpec attr 0x1000 on hvacFanCtrl",
        nullptr, 0},
    {"preset_boost",               ExposeType::Binary,  Access::StateSet, nullptr,
        "Boost leg of TS preset (attr 0x4270). Activity (attr 0x4275) + eco (programingOperMode) legs not wired",
        nullptr, 0},
    {"programming_operation_mode", ExposeType::Enum,    Access::State,    nullptr,
        "z2m exposes programmingOperationMode but ZHC has no generic Tz for it",
        nullptr, 0},
};

// ── bindings ────────────────────────────────────────────────────────
//
// ep1: hvacFanCtrl + genIdentify + hvacThermostat — the standard ZCL
// reporting set z2m configures with reporting.bind().
// ep232: haDiagnostic — Philips IRP companion endpoint that ships with
// firmwares that include manuSpecificPhilips2. The bind is preserved so
// downstream firmware mirrors z2m, even though no decoder uses the
// reports yet.
//
// `manuSpecificPhilips2` (extend-only) is intentionally NOT bound on
// ep1 — z2m's `philips.m.addManuSpecificPhilips2Cluster()` only adds
// the cluster definition for discovery; no exposes/converters depend
// on it in the GW003 path.
constexpr BindingSpec kAutoBindings[] = {
    {1,   0x0201},   // hvacThermostat
    {1,   0x0202},   // hvacFanCtrl
    {1,   0x0003},   // genIdentify (z2m bind list)
    {232, 0x0B05},   // haDiagnostic on companion ep
};

}  // namespace

extern const PreparedDefinition kDef_GW003_AS_IN_TE_FC{
    .zigbee_models=kModels_GW003_AS_IN_TE_FC, .zigbee_models_count=sizeof(kModels_GW003_AS_IN_TE_FC)/sizeof(kModels_GW003_AS_IN_TE_FC[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GW003-AS-IN-TE-FC", .vendor="Atlantic",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_GW003_AS_IN_TE_FC, .from_zigbee_count=sizeof(kFz_GW003_AS_IN_TE_FC)/sizeof(kFz_GW003_AS_IN_TE_FC[0]),
    .to_zigbee=kTz_GW003_AS_IN_TE_FC, .to_zigbee_count=sizeof(kTz_GW003_AS_IN_TE_FC)/sizeof(kTz_GW003_AS_IN_TE_FC[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::atlantic
