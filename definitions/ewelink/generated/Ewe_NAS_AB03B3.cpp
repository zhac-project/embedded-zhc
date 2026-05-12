// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ewelink NAS-AB03B3 — hand-corrected, manuSpec siren writes.
// Indoor sound and light alarm. z2m carries:
//   - addCustomClusterSiren on 0xFC11 (alarmSoundMode 0x2021 u8,
//                                       alarmSoundLevel 0x2022 u8,
//                                       alarmSoundTime  0x2023 u32).
//   - sirenAlarm: alarm bool ↔ genOnOff on/off command.
//   - iasZoneAlarm({zoneType:"generic", zoneAttributes:["battery_low"]}).
//   - battery percentage.
// We map the three FC11 attrs as standard (non-manu) writeAttributes
// frames via the canonical ZclWriteSpec runtime. Alarm on/off uses
// the existing kFzOnOff/kTzOnOff which already drive genOnOff.
// z2m-source: ewelink.ts #NAS-AB03B3 + ewelinkExtend.sirenAlarm.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ewelink {
namespace {
const FzConverter* const kFz_NAS_AB03B3[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzOnOff,        // alarm bool ← genOnOff onOff
};

// FC11 manuSpec write specs (no manuf code — z2m's deviceAddCustomCluster
// for customEwelinkSiren has no manufacturerCode → standard fc=0x10).
constexpr ::zhc::generic::ZclWriteSpec kSpec_AlarmDuration{
    "alarm_duration", 0x2023, 0x23 /*UINT32*/, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpec_Volume{
    "volume",         0x2022, 0x20 /*UINT8*/,  0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpec_Melody{
    "melody",         0x2021, 0x20 /*UINT8*/,  0, nullptr, 0,
};

extern const TzConverter kTz_AlarmDuration{
    .key         = "alarm_duration",
    .cluster     = "customEwelinkSiren",
    .cluster_id  = 0xFC11,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_AlarmDuration,
};
extern const TzConverter kTz_Volume{
    .key         = "volume",
    .cluster     = "customEwelinkSiren",
    .cluster_id  = 0xFC11,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_Volume,
};
extern const TzConverter kTz_Melody{
    .key         = "melody",
    .cluster     = "customEwelinkSiren",
    .cluster_id  = 0xFC11,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_Melody,
};

const TzConverter* const kTz_NAS_AB03B3[] = {
    &::zhc::generic::kTzOnOff,         // alarm on/off
    &kTz_AlarmDuration,
    &kTz_Volume,
    &kTz_Melody,
};

constexpr const char* kModels_NAS_AB03B3[] = { "NAS-AB03B3" };

constexpr Expose kExp_NAS_AB03B3[] = {
    {"battery",        ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",        ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"battery_low",    ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"alarm",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_duration", ExposeType::Numeric, Access::StateSet, "s",  nullptr, nullptr, 0},
    {"volume",         ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"melody",         ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_NAS_AB03B3[] = {
    {1, 0x0001},   // genPowerCfg (battery)
    {1, 0x0006},   // genOnOff    (alarm)
    {1, 0x0500},   // ssIasZone   (battery_low + zone status)
    {1, 0xFC11},   // customEwelinkSiren
};
}  // namespace

extern const PreparedDefinition kDef_NAS_AB03B3{
    .zigbee_models=kModels_NAS_AB03B3, .zigbee_models_count=sizeof(kModels_NAS_AB03B3)/sizeof(kModels_NAS_AB03B3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NAS-AB03B3", .vendor="Ewelink",
    .meta=nullptr, .exposes=kExp_NAS_AB03B3, .exposes_count=sizeof(kExp_NAS_AB03B3)/sizeof(kExp_NAS_AB03B3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NAS_AB03B3, .from_zigbee_count=sizeof(kFz_NAS_AB03B3)/sizeof(kFz_NAS_AB03B3[0]),
    .to_zigbee=kTz_NAS_AB03B3, .to_zigbee_count=sizeof(kTz_NAS_AB03B3)/sizeof(kTz_NAS_AB03B3[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_NAS_AB03B3, .bindings_count=sizeof(kBind_NAS_AB03B3)/sizeof(kBind_NAS_AB03B3[0]),
};

}  // namespace zhc::devices::ewelink
