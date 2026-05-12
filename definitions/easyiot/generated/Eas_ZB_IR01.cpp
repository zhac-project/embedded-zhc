// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-IR01 — auto-generated.
// Infrared remote control equipped with local code library.
// z2m-source: easyiot.ts #ZB-IR01.
//
// RUNTIME BLOCKER: this device speaks the Easyiot data-point protocol
// over the seTunneling cluster (transferData / commandTransferData).
// ZHC has no generic seTunneling encoder, so all of:
//   send_command, ac_power, ac_temperature, ac_mode, ac_wind_speed,
//   ac_kfid_offset, last_received_command
// are exposed for the gateway UI but not yet wired to fz/tz
// converters. The full schema is preserved here so the firmware can
// surface controls; the actual encode/decode path is a v2 follow-up
// requiring a vendor-specific easyiot::seTunneling helper.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
constexpr const char* kModels_ZB_IR01[] = { "ZB-IR01" };

// TODO(easyiot/ZB-IR01): wire fz/tz against seTunneling cluster
// (0x1000) commands `transferData` / `commandTransferData` once a
// vendor-specific helper exists. Mirroring the protocol byte layout
// from easyiot.ts (`protocolUtils.buildCommand`, `getBrandFormatted*`).

constexpr Expose kExposes_ZB_IR01[] = {
    {"last_received_command", ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"send_command",          ExposeType::String,  Access::Set,   nullptr, nullptr, nullptr, 0},
    {"ac_power",              ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"ac_mode",               ExposeType::Enum,    Access::Set,   nullptr, nullptr, nullptr, 0},
    {"ac_temperature",        ExposeType::Numeric, Access::Set,   "\xC2\xB0""C", nullptr, nullptr, 0},
    {"ac_wind_speed",         ExposeType::Enum,    Access::Set,   nullptr, nullptr, nullptr, 0},
    {"ac_kfid_offset",        ExposeType::Numeric, Access::Set,   nullptr, nullptr, nullptr, 0},
    {"ac_brand",              ExposeType::Enum,    Access::Set,   nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_IR01[] = {
    {1, 0x1000},  // seTunneling
};

}  // namespace

extern const PreparedDefinition kDef_ZB_IR01{
    .zigbee_models=kModels_ZB_IR01, .zigbee_models_count=sizeof(kModels_ZB_IR01)/sizeof(kModels_ZB_IR01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-IR01", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_IR01, .exposes_count=sizeof(kExposes_ZB_IR01)/sizeof(kExposes_ZB_IR01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_IR01, .bindings_count=sizeof(kBindings_ZB_IR01)/sizeof(kBindings_ZB_IR01[0]),
};

}  // namespace zhc::devices::easyiot
