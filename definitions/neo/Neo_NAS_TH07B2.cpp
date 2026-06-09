// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Neo NAS-TH07B2 — temperature & humidity sensor (SNZB-02 clone).
// Graduated from generated/ during the neo z2m<->embedded-zhc parity pass.
// This is the only neo device on standard ZCL clusters (model "SNZB-02",
// _TZ3000_utwgoauk), NOT Tuya-DP. z2m wires `m.temperature() + m.humidity()
// + m.battery()` (msTemperatureMeasurement 0x0402 / msRelativeHumidity 0x0405
// / genPowerCfg 0x0001). The generated def only carried kFzBattery + a
// {battery,voltage} expose set, so temperature & humidity never decoded.
// Added the standard temperature + humidity converters, exposes and binds.
// z2m-source: neo.ts #NAS-TH07B2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::neo {
namespace {
const FzConverter* const kFz_NAS_TH07B2[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_NAS_TH07B2[] = { "SNZB-02" };
constexpr const char* kManus_NAS_TH07B2[] = { "_TZ3000_utwgoauk" };

constexpr Expose kExposes_NAS_TH07B2[] = {
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_NAS_TH07B2[] = {
    {1, 0x0402},  // msTemperatureMeasurement
    {1, 0x0405},  // msRelativeHumidity
    {1, 0x0001},  // genPowerCfg (battery)
};
}  // namespace

extern const PreparedDefinition kDef_NAS_TH07B2{
    .zigbee_models=kModels_NAS_TH07B2, .zigbee_models_count=sizeof(kModels_NAS_TH07B2)/sizeof(kModels_NAS_TH07B2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_NAS_TH07B2, .manufacturer_names_count=sizeof(kManus_NAS_TH07B2)/sizeof(kManus_NAS_TH07B2[0]),
    .model="NAS-TH07B2", .vendor="Neo",
    .meta=nullptr, .exposes=kExposes_NAS_TH07B2, .exposes_count=sizeof(kExposes_NAS_TH07B2)/sizeof(kExposes_NAS_TH07B2[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_NAS_TH07B2, .from_zigbee_count=sizeof(kFz_NAS_TH07B2)/sizeof(kFz_NAS_TH07B2[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_NAS_TH07B2, .bindings_count=sizeof(kBindings_NAS_TH07B2)/sizeof(kBindings_NAS_TH07B2[0]),
};

}  // namespace zhc::devices::neo
