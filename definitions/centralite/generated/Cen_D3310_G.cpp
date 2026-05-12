// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Centralite 3310-G — hand-rewritten 2026-04-28.
// Temperature and humidity sensor.
// z2m-source: centralite.ts #3310-G.
//
// z2m bundle: fz.temperature + fzLocal.d3310_humidity + fz.battery
//   exposes [temperature, humidity, battery]
//   extend  centraliteExtend.addManuSpecificCentraliteHumidityCluster()
//
// Mapped:
//   kFzTemperature — msTemperatureMeasurement → "temperature"
//   kFzBattery     — genPowerCfg battery / voltage
//
// Runtime gap: humidity rides the manuSpecificCentraliteHumidity cluster
//   (0xFC45, manu 0x104E, attr 0x0000 — uint16, value%*100). ZHC has no
//   converter for this cluster yet (single-vendor + single-device need =
//   below the 5-port shared-converter threshold). Until the cluster lands,
//   only temperature/battery are emitted; humidity is a documented gap.
//   See docs/CENTRALITE_PARITY.md.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::centralite {
namespace {
const FzConverter* const kFz_D3310_G[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_D3310_G[] = { "3310-G" };

constexpr Expose kAutoExposes[] = {
    {"temperature", ExposeType::Numeric, Access::State, "\xc2\xb0""C", nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // humidity — pending manuSpecificCentraliteHumidity (0xFC45) cluster wiring.
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0402},  // msTemperatureMeasurement
    // 0xFC45 manuSpecificCentraliteHumidity — bind once cluster is supported.
};
}  // namespace

extern const PreparedDefinition kDef_D3310_G{
    .zigbee_models=kModels_D3310_G, .zigbee_models_count=sizeof(kModels_D3310_G)/sizeof(kModels_D3310_G[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3310-G", .vendor="Centralite",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D3310_G, .from_zigbee_count=sizeof(kFz_D3310_G)/sizeof(kFz_D3310_G[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::centralite
