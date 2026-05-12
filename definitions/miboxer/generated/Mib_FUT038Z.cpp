// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Miboxer FUT038Z — RGB+CCT LED controller.
//
// z2m-source: zigbee-herdsman-converters/src/devices/miboxer.ts (FUT038Z).
// The z2m entry expands `tuya.modernExtend.tuyaLight({colorTemp:{range:[153,500]},color:true})`,
// i.e. on/off + brightness + colour-temp (mireds 153-500) + colour (xy + hs).
// `tuya.modernExtend.tuyaLight` adds two Tuya quirks on top of vanilla
// `m.light(...)`: `tuyaFz.brightness` (level fallback report) and
// `tuyaTz.do_not_disturb`. We don't have generic-side equivalents in the
// shared library yet, so we ship the four-axis ZCL colour stack
// (kFz/kTz {OnOff, Brightness, ColorTemperature, Color}). The "do not
// disturb" toggle is a Tuya-specific extra and not safety-critical for
// SPA control; tracked under MISSING in docs/MIBOXER_PARITY.md.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::miboxer {
namespace {

constexpr const char* kModels_FUT038Z[] = { "TS0504B" };
constexpr const char* kManus_FUT038Z[]  = { "_TZ3210_ttkgurpb" };

constexpr Expose kExposes_FUT038Z[] = {
    { "state",      ExposeType::Binary,  Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_temp", ExposeType::Numeric, Access::StateSet,
      "mired", nullptr, nullptr, 0 },
    { "color_x",    ExposeType::Numeric, Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "color_y",    ExposeType::Numeric, Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "hue",        ExposeType::Numeric, Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
    { "saturation", ExposeType::Numeric, Access::StateSet,
      nullptr, nullptr, nullptr, 0 },
};

const FzConverter* const kFz_FUT038Z[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzColor,
};

const TzConverter* const kTz_FUT038Z[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &::zhc::generic::kTzColor,
};

constexpr BindingSpec kBindings_FUT038Z[] = {
    { 1, 0x0006 },   // genOnOff
    { 1, 0x0008 },   // genLevelCtrl
    { 1, 0x0300 },   // lightingColorCtrl
};

}  // namespace

extern const PreparedDefinition kDef_FUT038Z{
    .zigbee_models            = kModels_FUT038Z,
    .zigbee_models_count      = sizeof(kModels_FUT038Z)/sizeof(kModels_FUT038Z[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = kManus_FUT038Z,
    .manufacturer_names_count = sizeof(kManus_FUT038Z)/sizeof(kManus_FUT038Z[0]),
    .model                    = "FUT038Z",
    .vendor                   = "Miboxer",
    .meta                     = nullptr,
    .exposes                  = kExposes_FUT038Z,
    .exposes_count            = sizeof(kExposes_FUT038Z)/sizeof(kExposes_FUT038Z[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_FUT038Z,
    .from_zigbee_count        = sizeof(kFz_FUT038Z)/sizeof(kFz_FUT038Z[0]),
    .to_zigbee                = kTz_FUT038Z,
    .to_zigbee_count          = sizeof(kTz_FUT038Z)/sizeof(kTz_FUT038Z[0]),
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_FUT038Z,
    .bindings_count           = sizeof(kBindings_FUT038Z)/sizeof(kBindings_FUT038Z[0]),
};

}  // namespace zhc::devices::miboxer
