// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya ZP01 PIR motion sensor running the pvvx/ZigbeeTLc open
// firmware (`TS202PIR1-z`). New upstream in z2m v26.100.0.
//
// The reflashed firmware turns a plain TS0202 into a standards-clean device:
// occupancy on msOccupancySensing, battery on genPowerCfg, and a genOnOff
// endpoint used as a REMOTE BINDING SOURCE rather than a load — when enabled,
// occupancy sends `on` and clearing sends `off` to whatever the sensor is
// bound to. That is why `state` here is a switch the user can toggle (it gates
// the behaviour), and why the command decoders are wired: the sensor is the
// one issuing on/off.
//
// `pir_timeout` is the standard msOccupancySensing `pirOToUDelay` attribute
// (0x0010), read/write, in seconds.
//
// z2m-source: zigbeetlc.ts #TS202PIR1-z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::zigbeetlc {
namespace {

const FzConverter* const kFz_TS202PIR1_z[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOccupancy,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
};
const TzConverter* const kTz_TS202PIR1_z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
};

constexpr const char* kModels_TS202PIR1_z[] = { "TS202PIR1-z" };

constexpr const char* kPobOpts[] = { "off", "on", "toggle", "previous" };

constexpr Expose kExposes_TS202PIR1_z[] = {
    {"occupancy",   ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"action",      ExposeType::String,  Access::State,    nullptr,
     "on / off issued to bound devices on occupancy change", nullptr, 0},
    {"state",       ExposeType::Binary,  Access::StateSet, nullptr,
     "Enable remote on/off binding on occupancy change", nullptr, 0, ExposeCategory::Config},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Power-on behavior for state (remote on/off enable)",
     kPobOpts, sizeof(kPobOpts)/sizeof(kPobOpts[0]), ExposeCategory::Config},
    {"pir_timeout", ExposeType::Numeric, Access::StateSet, "s",
     "PIR occupancy timeout (firmware default ~35 s)", nullptr, 0,
     ExposeCategory::Config, 0, 65535, 1},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0,
     ExposeCategory::Diagnostic},
};

constexpr BindingSpec kBindings_TS202PIR1_z[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0006},   // genOnOff
    {1, 0x0406},   // msOccupancySensing
};

}  // namespace

extern const PreparedDefinition kDef_TS202PIR1_z{
    .zigbee_models=kModels_TS202PIR1_z,
    .zigbee_models_count=sizeof(kModels_TS202PIR1_z)/sizeof(kModels_TS202PIR1_z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TS202PIR1-z", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes_TS202PIR1_z,
    .exposes_count=sizeof(kExposes_TS202PIR1_z)/sizeof(kExposes_TS202PIR1_z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TS202PIR1_z, .from_zigbee_count=sizeof(kFz_TS202PIR1_z)/sizeof(kFz_TS202PIR1_z[0]),
    .to_zigbee=kTz_TS202PIR1_z, .to_zigbee_count=sizeof(kTz_TS202PIR1_z)/sizeof(kTz_TS202PIR1_z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TS202PIR1_z,
    .bindings_count=sizeof(kBindings_TS202PIR1_z)/sizeof(kBindings_TS202PIR1_z[0]),
};

}  // namespace zhc::devices::zigbeetlc
