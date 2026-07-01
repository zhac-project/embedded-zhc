// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Terncy DIM003 color-temperature dimmer (z2m v26.76.0 parity).
//
// z2m-source: terncy.ts #DIM003. Fingerprint modelID "DIM003" +
// manufacturerName "Xiaoyan". The primary function is a CT light —
// z2m `m.light({colorTemp: {range: [142, 625]}})` — wired here with the
// generic on/off + level + color_temp converters (same shape as the
// Terncy DL001 downlight), bound on genOnOff / genLevelCtrl /
// lightingColorCtrl.
//
// DEFERRED: the vendor config surface rides on two manufacturer-specific
// clusters that have no generic equivalent —
//   * terncyDimmerPower       -> rated_max_current_ma (powerCalibration cmd),
//                                startup_depth_calibration (adjustStartLevel)
//   * terncyDimmerLightEffect -> color_temperature_range_{min,max}_kelvin,
//                                color_temperature_io_reversed, light_up_curve
//                                (fromZigbee dim003_light_effect_attributes)
// Those fzLocal/tzLocal converters decode/encode custom-cluster attributes
// and commands with no ::zhc::generic counterpart, so they are left for a
// dedicated terncy dimmer codec if a user asks. The primary light (on/off,
// brightness, color temperature) is fully covered.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

const FzConverter* const kFz_DIM003[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_DIM003[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_DIM003[] = { "DIM003" };
constexpr const char* kManu_DIM003[]   = { "Xiaoyan" };

constexpr Expose kExposes_DIM003[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_DIM003[] = {
    {1, 0x0006},   // genOnOff
    {1, 0x0008},   // genLevelCtrl
    {1, 0x0300},   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_DIM003{
    .zigbee_models=kModels_DIM003, .zigbee_models_count=sizeof(kModels_DIM003)/sizeof(kModels_DIM003[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManu_DIM003, .manufacturer_names_count=sizeof(kManu_DIM003)/sizeof(kManu_DIM003[0]),
    .model="DIM003", .vendor="TERNCY",
    .meta=nullptr,
    .exposes=kExposes_DIM003, .exposes_count=sizeof(kExposes_DIM003)/sizeof(kExposes_DIM003[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIM003, .from_zigbee_count=sizeof(kFz_DIM003)/sizeof(kFz_DIM003[0]),
    .to_zigbee=kTz_DIM003, .to_zigbee_count=sizeof(kTz_DIM003)/sizeof(kTz_DIM003[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_DIM003, .bindings_count=sizeof(kBindings_DIM003)/sizeof(kBindings_DIM003[0]),
};

}  // namespace zhc::devices::terncy
