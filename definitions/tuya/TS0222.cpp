// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TS0222 illuminance sensor (non-DP).
// Standard msIlluminanceMeasurement cluster.
// z2m-source: tuya.ts #TS0222 (whitelabel HOBEIAN ZG-106Z fingerprint
// matches via the zigbeeModel "ZG-106Z" alias).
#include "definitions/lumi/_shared.hpp"
#include "zhc/modern_extend.hpp"
namespace zhc::devices::tuya {
namespace {
struct i_opts { static constexpr const char* name="illuminance",*unit="lux",
    *cluster="msIlluminanceMeasurement"; static constexpr std::uint16_t attr=0x0000;
    static constexpr std::uint32_t divisor=1; };
using Lux = ::zhc::m::Numeric<i_opts>;
const FzConverter* const kFz[] = { &Lux::converter };
constexpr const char* kModels[] = { "TS0222", "ZG-106Z" };
}

constexpr WhiteLabel kWhiteLabels_TS0222[] = {
    {"HOBEIAN", "ZG-106Z"},
};

extern const PreparedDefinition kDefTS0222{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="TS0222",.vendor="Tuya",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=kWhiteLabels_TS0222,
    .white_labels_count=sizeof(kWhiteLabels_TS0222)/sizeof(kWhiteLabels_TS0222[0]),
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr };
}
