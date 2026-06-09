// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman HS1MIS-3.0 — hand-maintained parity override.
//
// Graduated from generated/Hei_HS1MIS_3_0.cpp, which mis-ported this as an
// IAS Zone sensor (kFzIasZone + ssIasZone 0x0500 binding, emitting "alarm").
// Unlike the rest of the Heiman PIR family this model is NOT an IAS device:
// z2m decodes it with `fz.occupancy` (msOccupancySensing 0x0406, attr 0x0000
// bit 0) and exposes only `e.occupancy()` + `e.battery()`. The generic
// kFzOccupancy decoder applies the identical `(raw & 1) > 0` mapping, so it
// is at parity. Bindings corrected to genPowerCfg + msOccupancySensing; the
// IAS-only tamper / battery_low exposes are dropped (never produced by the
// occupancy cluster).
//
// z2m-source: heiman.ts #HS1MIS-3.0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS1MIS_3_0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOccupancy,
};

constexpr const char* kModels_HS1MIS_3_0[] = { "PIRILLSensor-EF-3.0" };

constexpr Expose kExposes_HS1MIS_3_0[] = {
    {"battery",   ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",   ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"occupancy", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_HS1MIS_3_0[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0406},   // msOccupancySensing
};

}  // namespace

extern const PreparedDefinition kDef_HS1MIS_3_0{
    .zigbee_models=kModels_HS1MIS_3_0, .zigbee_models_count=sizeof(kModels_HS1MIS_3_0)/sizeof(kModels_HS1MIS_3_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS1MIS-3.0", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS1MIS_3_0, .exposes_count=sizeof(kExposes_HS1MIS_3_0)/sizeof(kExposes_HS1MIS_3_0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS1MIS_3_0, .from_zigbee_count=sizeof(kFz_HS1MIS_3_0)/sizeof(kFz_HS1MIS_3_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HS1MIS_3_0, .bindings_count=sizeof(kBindings_HS1MIS_3_0)/sizeof(kBindings_HS1MIS_3_0[0]),
};

}  // namespace zhc::devices::heiman
