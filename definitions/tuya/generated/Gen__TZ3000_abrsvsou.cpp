// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS004F smart knob family (ERS-10TZBVK-AA rebranded).
// Tier 2 header freezes this file against `generate_simple_vendor_ports.py`.
// The 8 manu names are also listed in
// `tools/{extract_tuya_datapoints,generate_tuya_ports}.py::HAND_PORTED_MANUS`
// so Tuya DP extract + fingerprint regen won't emit per-manu stubs.
//
// Consolidated port covering the 8 `_TZ3000_*` manu variants z2m
// groups under `ERS-10TZBVK-AA`. Lives under /generated/ because
// `refresh-parity` owns the slot; re-running it would overwrite this
// until the generator learns about the consolidated port.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tuya.ts
//             (ERS-10TZBVK-AA + ZG-101Z/D entries).
//
// Actions in this v1 port:
//   - single / double / hold          (tuyaAction  cmd 0xFD)
//   - rotate_right / rotate_left      (tuyaAction2 cmd 0xFC)
//   - toggle                          (genOnOff    cmd 0x02)
//
// Deferred (needs payload-parsing helpers not yet in zhc's shared lib):
//   - brightness_step_up/_down, color_temperature_step_*,
//     hue_move/stop, saturation_move.

#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

constexpr const char* kModels_TS004F_knob[] = { "TS004F" };

constexpr const char* kManus_TS004F_knob[] = {
    "_TZ3000_4fjiwweb",
    "_TZ3000_uri7ongn",
    "_TZ3000_ixla93vd",
    "_TZ3000_qja6nq5z",
    "_TZ3000_402vrq2i",
    "_TZ3000_abrsvsou",
    "_TZ3000_gwkzibhs",
    "_TZ3000_ugi8ky6u",
};

constexpr const char* kActions_TS004F_knob[] = {
    "single",
    "double",
    "hold",
    "rotate_left",
    "rotate_right",
    "toggle",
    "brightness_step_up",
    "brightness_step_down",
    "color_temperature_step_up",
    "color_temperature_step_down",
};

constexpr Expose kExposes_TS004F_knob[] = {
    { "action",                 ExposeType::Enum,    Access::State,
      nullptr, nullptr,
      kActions_TS004F_knob,
      sizeof(kActions_TS004F_knob)/sizeof(kActions_TS004F_knob[0]) },
    { "action_step_size",       ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "action_transition_time", ExposeType::Numeric, Access::State,
      "s",     nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
};

const FzConverter* const kFz_TS004F_knob[] = {
    &::zhc::tuya::kFzTuyaMultiAction,       // cmd 0xFD + 0xFC (event-mode rotate)
    &::zhc::generic::kFzCommandToggle,      // genOnOff cmd 0x02
    &::zhc::generic::kFzCommandStep,        // genLevelCtrl cmd 0x02 (cmd-mode brightness step)
    &::zhc::generic::kFzCommandStepColorTemp, // lightingColorCtrl cmd 0x4C (cmd-mode rotate)
    &::zhc::generic::kFzBattery,            // genPowerCfg 0x0020 + 0x0021
};

constexpr WhiteLabel kWhiteLabels_TS004F_knob[] = {
    { "Tuya",  "ZG-101Z_D_1" },
    { "Moes",  "ZG-101ZD"    },
    { "Immax", "07768L"      },
};

constexpr BindingSpec kBindings_TS004F_knob[] = {
    { 1, 0x0001 },   // genPowerCfg — battery reports
    { 1, 0x0006 },   // genOnOff — action events + operation-mode attr
    { 1, 0x0008 },   // genLevelCtrl — cmd-mode brightness step
    { 1, 0x0300 },   // lightingColorCtrl — cmd-mode rotate (stepColorTemp)
};

// Tuya magic-packet attrs (genBasic).
constexpr std::uint8_t kMagicAttrs_TS004F_knob[] = {
    0x04, 0x00,  0x00, 0x00,  0x01, 0x00,
    0x05, 0x00,  0x07, 0x00,  0xFE, 0xFF,
};

// Configure pipeline — bind-settle then magic packet. The
// operation_mode write (genOnOff attr 0x8004 = 1 "event") that z2m
// runs needs a Write op in the step walker; most firmware shipments
// default to event mode already, so rotations still emit without it.
constexpr ConfigStep kConfigSteps_TS004F_knob[] = {
    { ConfigStepOp::Wait, 0, 0,      0,    0, nullptr, 0, 600 },
    { ConfigStepOp::Read, 1, 0x0000, 0x00, 0,
      kMagicAttrs_TS004F_knob, sizeof(kMagicAttrs_TS004F_knob), 2000 },
};

}  // namespace

// Name preserved so registry.cpp's extern declaration resolves.
extern const PreparedDefinition kDefGen__TZ3000_abrsvsou{
    .zigbee_models            = kModels_TS004F_knob,
    .zigbee_models_count      = sizeof(kModels_TS004F_knob)/sizeof(kModels_TS004F_knob[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = kManus_TS004F_knob,
    .manufacturer_names_count = sizeof(kManus_TS004F_knob)/sizeof(kManus_TS004F_knob[0]),
    .model                    = "ERS-10TZBVK-AA",
    .vendor                   = "Tuya",
    .meta                     = nullptr,
    .exposes                  = kExposes_TS004F_knob,
    .exposes_count            = sizeof(kExposes_TS004F_knob)/sizeof(kExposes_TS004F_knob[0]),
    .white_labels             = kWhiteLabels_TS004F_knob,
    .white_labels_count       = sizeof(kWhiteLabels_TS004F_knob)/sizeof(kWhiteLabels_TS004F_knob[0]),
    .from_zigbee              = kFz_TS004F_knob,
    .from_zigbee_count        = sizeof(kFz_TS004F_knob)/sizeof(kFz_TS004F_knob[0]),
    .to_zigbee                = nullptr,
    .to_zigbee_count          = 0,
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kBindings_TS004F_knob,
    .bindings_count           = sizeof(kBindings_TS004F_knob)/sizeof(kBindings_TS004F_knob[0]),
    .reports                  = nullptr,
    .reports_count            = 0,
    .config_steps             = kConfigSteps_TS004F_knob,
    .config_steps_count       = sizeof(kConfigSteps_TS004F_knob)/sizeof(kConfigSteps_TS004F_knob[0]),
};

}  // namespace zhc::devices::tuya
