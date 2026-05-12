// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ewelink NAS-AB06B3 — hand-corrected, manuSpec siren writes.
// Outdoor sound and light alarm. Same 0xFC11 attribute layout as
// NAS-AB03B3, but volume is an enumLookup {high:3, medium:2, low:1}
// and melody is {1:0,2:1,3:2}. Both encoded as ZCL writeAttributes
// frames (no manuf code) via the canonical ZclWriteSpec runtime.
// z2m-source: ewelink.ts #NAS-AB06B3.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ewelink {
namespace {
const FzConverter* const kFz_NAS_AB06B3[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasZone,
    &::zhc::generic::kFzOnOff,
};

constexpr ::zhc::generic::ZclWriteLookup kLook_VolumeOutdoor[] = {
    {"low",    0x01},
    {"medium", 0x02},
    {"high",   0x03},
};
constexpr ::zhc::generic::ZclWriteLookup kLook_MelodyOutdoor[] = {
    {"1", 0x00}, {"2", 0x01}, {"3", 0x02},
};

constexpr ::zhc::generic::ZclWriteSpec kSpec_AlarmDuration{
    "alarm_duration", 0x2023, 0x23 /*UINT32*/, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpec_VolumeOutdoor{
    "volume",         0x2022, 0x20 /*UINT8*/,  0,
    kLook_VolumeOutdoor, sizeof(kLook_VolumeOutdoor)/sizeof(kLook_VolumeOutdoor[0]),
};
constexpr ::zhc::generic::ZclWriteSpec kSpec_MelodyOutdoor{
    "melody",         0x2021, 0x20 /*UINT8*/,  0,
    kLook_MelodyOutdoor, sizeof(kLook_MelodyOutdoor)/sizeof(kLook_MelodyOutdoor[0]),
};

extern const TzConverter kTz_AlarmDurationOutdoor{
    .key         = "alarm_duration",
    .cluster     = "customEwelinkSiren",
    .cluster_id  = 0xFC11,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_AlarmDuration,
};
extern const TzConverter kTz_VolumeOutdoor{
    .key         = "volume",
    .cluster     = "customEwelinkSiren",
    .cluster_id  = 0xFC11,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_VolumeOutdoor,
};
extern const TzConverter kTz_MelodyOutdoor{
    .key         = "melody",
    .cluster     = "customEwelinkSiren",
    .cluster_id  = 0xFC11,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSpec_MelodyOutdoor,
};

const TzConverter* const kTz_NAS_AB06B3[] = {
    &::zhc::generic::kTzOnOff,
    &kTz_AlarmDurationOutdoor,
    &kTz_VolumeOutdoor,
    &kTz_MelodyOutdoor,
};

constexpr const char* kModels_NAS_AB06B3[] = { "NAS-AB06B3" };

constexpr Expose kExp_NAS_AB06B3[] = {
    {"battery",        ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",        ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"battery_low",    ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"alarm",          ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_duration", ExposeType::Numeric, Access::StateSet, "s",  nullptr, nullptr, 0},
    {"volume",         ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"melody",         ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_NAS_AB06B3[] = {
    {1, 0x0001},
    {1, 0x0006},
    {1, 0x0500},
    {1, 0xFC11},
};
}  // namespace

extern const PreparedDefinition kDef_NAS_AB06B3{
    .zigbee_models=kModels_NAS_AB06B3, .zigbee_models_count=sizeof(kModels_NAS_AB06B3)/sizeof(kModels_NAS_AB06B3[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NAS-AB06B3", .vendor="Ewelink",
    .meta=nullptr, .exposes=kExp_NAS_AB06B3, .exposes_count=sizeof(kExp_NAS_AB06B3)/sizeof(kExp_NAS_AB06B3[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NAS_AB06B3, .from_zigbee_count=sizeof(kFz_NAS_AB06B3)/sizeof(kFz_NAS_AB06B3[0]),
    .to_zigbee=kTz_NAS_AB06B3, .to_zigbee_count=sizeof(kTz_NAS_AB06B3)/sizeof(kTz_NAS_AB06B3[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_NAS_AB06B3, .bindings_count=sizeof(kBind_NAS_AB06B3)/sizeof(kBind_NAS_AB06B3[0]),
};

}  // namespace zhc::devices::ewelink
