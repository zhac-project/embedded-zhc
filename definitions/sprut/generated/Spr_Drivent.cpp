// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Sprut.device Drivent — uses shared sprut converters.
// Drivent window drive.
// z2m-source: zigbee-herdsman-converters/src/devices/sprut.ts #Drivent
//
// Drivent extends closuresWindowCovering with manuSpec attrs and a
// set of cluster-specific commands tagged with mfg=0x6666:
//   0x6605 calibrate     (bool, ALL)
//   0x6606 speed         (u8 0..100 step 25, ALL)
//   0x6609 drive_state   (enum8, STATE_GET)
//   0x660A blocked_jam   (bool,  STATE_GET)
//   0x660B blocked_many  (bool,  STATE_GET)
//   cmd 0x00 reset_limit
//   cmd 0x01 open_limit
//   cmd 0x02 close_limit
//   cmd 0x03 reset_block
// Plus a separate cluster 0x6600 with a single attr 0x6600 boolean for
// the WiFi AP toggle.

#include "definitions/_generic/_shared.hpp"
#include "definitions/sprut/_shared.hpp"

namespace zhc::devices::sprut {
namespace {

const FzConverter* const kFz_Drivent[] = {
    &::zhc::sprut::kFzSprutDriventCovering,   // position + manuSpec readouts
    &::zhc::sprut::kFzSprutWifi,              // cluster 0x6600 attr 0x6600
};

const TzConverter* const kTz_Drivent[] = {
    // Standard cover up/down/stop/position via the generic combined Tz.
    &::zhc::generic::kTzCoverPosition,
    // manuSpec writes
    &::zhc::sprut::kTzSprutSpeed,
    &::zhc::sprut::kTzSprutCalibrate,
    &::zhc::sprut::kTzSprutWifi,
    // Cluster commands (resetLimit / openLimit / closeLimit / resetBlock)
    &::zhc::sprut::kTzSprutResetLimit,
    &::zhc::sprut::kTzSprutOpenLimit,
    &::zhc::sprut::kTzSprutCloseLimit,
    &::zhc::sprut::kTzSprutResetBlock,
};

constexpr const char* kModels_Drivent[] = { "Drivent" };

constexpr Expose kAutoExposes[] = {
    {"position",     ExposeType::Numeric, Access::StateSet, "%",   nullptr, nullptr, 0},
    {"speed",        ExposeType::Numeric, Access::StateSet, "%",   nullptr, nullptr, 0},
    {"drive_state",  ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"blocked_jam",  ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"blocked_many", ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"calibrate",    ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"wifi",         ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    // Cluster-command actions (write-only triggers)
    {"reset_limit",  ExposeType::Enum,    Access::Set,      nullptr, nullptr, nullptr, 0},
    {"open_limit",   ExposeType::Enum,    Access::Set,      nullptr, nullptr, nullptr, 0},
    {"close_limit",  ExposeType::Enum,    Access::Set,      nullptr, nullptr, nullptr, 0},
    {"reset_block",  ExposeType::Enum,    Access::Set,      nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},     // closuresWindowCovering
    {1, 0x6600},     // Sprut WiFi cluster
};

}  // namespace

extern const PreparedDefinition kDef_Drivent{
    .zigbee_models=kModels_Drivent,
    .zigbee_models_count=sizeof(kModels_Drivent)/sizeof(kModels_Drivent[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Drivent", .vendor="Sprut",
    .meta=nullptr,
    .exposes=kAutoExposes,
    .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Drivent,
    .from_zigbee_count=sizeof(kFz_Drivent)/sizeof(kFz_Drivent[0]),
    .to_zigbee=kTz_Drivent,
    .to_zigbee_count=sizeof(kTz_Drivent)/sizeof(kTz_Drivent[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,
    .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::sprut
