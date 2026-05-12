// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: SlackyDiy AirQ_Monitor_S01 — auto-generated.
// Air quality monitor
// z2m-source: slacky_diy.ts #AirQ_Monitor_S01.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::slacky_diy {
namespace {


constexpr const char* kModels_AirQ_Monitor_S01[] = { "AirQ_Monitor_S01" };

}  // namespace


namespace {
// z2m AirQ_Monitor_S01 also wires m.co2(), m.numeric(voc_index), m.numeric(formaldehyd),
// air_extend.led_brightness(), m.binary(enabling_sound), m.numeric(life_time/screen_brightness/etc.)
// against msCO2 (0x040D), genAnalogInput (0x000C), hvacUserInterfaceCfg, genLevelCtrl and a heap of
// custom 0xFXXX manuf-specific attributes. The ZHC runtime currently lacks decoders for these clusters
// so only the standard temperature/humidity reports survive parity.
const FzConverter* const kFz_min_AirQ_Monitor_S01[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
constexpr Expose kExp_min_AirQ_Monitor_S01[] = {
    { "state",       ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery",     ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage",     ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
    { "temperature", ExposeType::Numeric, ::zhc::Access::State,    "C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_AirQ_Monitor_S01[] = {
    { 1, 0x0001 }, { 1, 0x0006 }, { 1, 0x0402 }, { 1, 0x0405 },
};
}
extern const PreparedDefinition kDef_AirQ_Monitor_S01{
    .zigbee_models=kModels_AirQ_Monitor_S01, .zigbee_models_count=sizeof(kModels_AirQ_Monitor_S01)/sizeof(kModels_AirQ_Monitor_S01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AirQ_Monitor_S01", .vendor="SlackyDiy",
    .meta=nullptr, .exposes=kExp_min_AirQ_Monitor_S01, .exposes_count=sizeof(kExp_min_AirQ_Monitor_S01)/sizeof(kExp_min_AirQ_Monitor_S01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_AirQ_Monitor_S01, .from_zigbee_count=sizeof(kFz_min_AirQ_Monitor_S01)/sizeof(kFz_min_AirQ_Monitor_S01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_AirQ_Monitor_S01,
    .bindings_count           = sizeof(kBind_min_AirQ_Monitor_S01)/sizeof(kBind_min_AirQ_Monitor_S01[0]),
};

}  // namespace zhc::devices::slacky_diy
