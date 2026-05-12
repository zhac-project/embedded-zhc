// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: KeenHome RS-THP-MP-1.0 — battery + temperature + humidity + pressure sensor.
// z2m wires fz.battery + fz.temperature (z2m source comment notes
// `lumi.fromZigbee.lumi_temperature` "looks like a mistake, probably just
// fz.temperature") + fz.humidity + fz.keen_home_smart_vent_pressure (which is
// plain msPressureMeasurement attr 0x0000).
// z2m-source: keen_home.ts #RS-THP-MP-1.0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::keen_home {
namespace {
const FzConverter* const kFz_RS_THP_MP_1_0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzPressure,
};

constexpr const char* kModels_RS_THP_MP_1_0[] = { "RS-THP-MP-1.0" };

constexpr Expose kExp_RS_THP_MP_1_0[] = {
    {"battery",     ExposeType::Numeric, ::zhc::Access::State, "%",   nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, ::zhc::Access::State, "mV",  nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, ::zhc::Access::State, "C",   nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",   nullptr, nullptr, 0},
    {"pressure",    ExposeType::Numeric, ::zhc::Access::State, "hPa", nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_RS_THP_MP_1_0[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0405},   // msRelativeHumidity
    {1, 0x0403},   // msPressureMeasurement
};
}  // namespace

extern const PreparedDefinition kDef_RS_THP_MP_1_0{
    .zigbee_models=kModels_RS_THP_MP_1_0,
    .zigbee_models_count=sizeof(kModels_RS_THP_MP_1_0)/sizeof(kModels_RS_THP_MP_1_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RS-THP-MP-1.0", .vendor="KeenHome",
    .meta=nullptr,
    .exposes=kExp_RS_THP_MP_1_0,
    .exposes_count=sizeof(kExp_RS_THP_MP_1_0)/sizeof(kExp_RS_THP_MP_1_0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RS_THP_MP_1_0,
    .from_zigbee_count=sizeof(kFz_RS_THP_MP_1_0)/sizeof(kFz_RS_THP_MP_1_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_RS_THP_MP_1_0,
    .bindings_count=sizeof(kBind_RS_THP_MP_1_0)/sizeof(kBind_RS_THP_MP_1_0[0]),
};

}  // namespace zhc::devices::keen_home
