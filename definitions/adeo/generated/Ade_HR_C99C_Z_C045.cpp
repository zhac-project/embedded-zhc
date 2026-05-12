// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adeo HR-C99C-Z-C045 — hand-rewritten for parity.
// RGB CTT LEXMAN ENKI remote control (5-button + scene cycle, Color+CT light).
// z2m-source: adeo.ts #HR-C99C-Z-C045 (zigbeeModel: ["LXEK-5","ZBEK-26"]).
//
// z2m bundle: fz.battery + fz.command_on/off/step/stop +
// fz.command_step_color_temperature + fz.command_step_hue +
// fz.command_step_saturation + fz.color_stop_raw +
// fz.scenes_recall_scene_65024.
//
// Mapped here: kFzCommandOn/Off/Step/Stop/StepColorTemp/Recall + kFzBattery.
// The hue/saturation step subcommands and the custom 0x65024 scene
// recall (vs vanilla genScenes) have no generic decoder yet — those
// `action` labels (color_hue_step_*, color_saturation_step_*,
// color_stop, scene_1..scene_4) are tracked as gaps in ADEO_PARITY.md.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adeo {
namespace {

constexpr const char* kModels_HR_C99C_Z_C045[] = { "LXEK-5", "ZBEK-26" };

constexpr const char* kActions_HR_C99C_Z_C045[] = {
    "on",
    "off",
    "scene_1",
    "scene_2",
    "scene_3",
    "scene_4",
    "color_saturation_step_up",
    "color_saturation_step_down",
    "color_stop",
    "color_hue_step_up",
    "color_hue_step_down",
    "color_temperature_step_up",
    "color_temperature_step_down",
    "brightness_step_up",
    "brightness_step_down",
    "brightness_stop",
};

constexpr Expose kExposes_HR_C99C_Z_C045[] = {
    { "action",  ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_HR_C99C_Z_C045,
      sizeof(kActions_HR_C99C_Z_C045)/sizeof(kActions_HR_C99C_Z_C045[0]) },
    { "battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_HR_C99C_Z_C045[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandStepColorTemp,
    &::zhc::generic::kFzCommandRecall,
};

constexpr BindingSpec kBindings_HR_C99C_Z_C045[] = {
    { 1, 0x0001 },  // genPowerCfg
    { 1, 0x0006 },  // genOnOff
    { 1, 0x0008 },  // genLevelCtrl
    { 1, 0x0300 },  // lightingColorCtrl
    { 1, 0x0005 },  // genScenes (best-effort; z2m uses adeo-private 65024)
};

}  // namespace

extern const PreparedDefinition kDef_HR_C99C_Z_C045{
    .zigbee_models=kModels_HR_C99C_Z_C045, .zigbee_models_count=sizeof(kModels_HR_C99C_Z_C045)/sizeof(kModels_HR_C99C_Z_C045[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HR-C99C-Z-C045", .vendor="Adeo",
    .meta=nullptr,
    .exposes=kExposes_HR_C99C_Z_C045, .exposes_count=sizeof(kExposes_HR_C99C_Z_C045)/sizeof(kExposes_HR_C99C_Z_C045[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HR_C99C_Z_C045, .from_zigbee_count=sizeof(kFz_HR_C99C_Z_C045)/sizeof(kFz_HR_C99C_Z_C045[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HR_C99C_Z_C045, .bindings_count=sizeof(kBindings_HR_C99C_Z_C045)/sizeof(kBindings_HR_C99C_Z_C045[0]),
};

}  // namespace zhc::devices::adeo
