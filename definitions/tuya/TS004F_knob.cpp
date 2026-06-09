// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS004F smart knob (z2m ERS-10TZBVK-AA) — manu-specific override.
//
// Covers the 8 `_TZ3000_*` variants z2m groups under `ERS-10TZBVK-AA`.
// Wins over the generic model-only `kDefTS004F` via manu disambiguation
// (the registry's collision audit treats a model+manu match as the
// disambiguator for a bare-model fallback).
//
// Supersedes the former generated hand-port
// `generated/Gen__TZ3000_abrsvsou.cpp` (deleted). A manual override in the
// parent vendor dir is hand-editable; files under `generated/` are blocked
// by the tools hook and overwritten by the zhac-tools sweep. The 8 manu
// names remain in `zhac-tools ::HAND_PORTED_MANUS`, so the generator keeps
// skipping per-manu stubs for them.
//
// Full z2m expose parity — see extra/docs/TS004F_KNOB_PARITY_TICKET.md.
// z2m-source: zigbee-herdsman-converters/src/devices/tuya.ts
//             (ERS-10TZBVK-AA + ZG-101Z/D entries).
//
// Actions:
//   - single / double / hold            (genOnOff cmd 0xFD tuyaAction)
//   - rotate_right / rotate_left        (genOnOff cmd 0xFC tuyaAction2)
//   - toggle                            (genOnOff cmd 0x02)
//   - brightness_step_up/_down          (genLevelCtrl cmd 0x02)
//   - color_temperature_step_up/_down   (lightingColorCtrl cmd 0x4C)
//   - hue_move / hue_stop + action_rate (lightingColorCtrl cmd 0x01)
//
// operation_mode (command/event): read+write expose on genOnOff attr
//   0x8004 — kFzTuyaOperationMode (report) + kTzTuyaOperationMode (write).
//
// Declared-only (z2m superset, no emitting converter for this device — the
// same as z2m, which lists it in `e.action()` but wires no producer here):
//   - saturation_move
//
// operation_mode is force-written to "event" (1) at configure time via a
// `ConfigStepOp::Write` step — genOnOff attr 0x8004 (tuyaOperationMode),
// enum8 0x30, value 0x01. This mirrors z2m's ERS-10TZBVK-AA `configure`
// (`endpoint.write("genOnOff", {tuyaOperationMode: 1})` in tuya.ts), which
// puts the knob into per-click "event" reporting. The tz converter still
// lets the user switch to "command" (group control) from the UI afterward.
// (Previously deferred because `ConfigStepOp` lacked a Write op — now done.)

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
    "hue_move",
    "hue_stop",
    "saturation_move",
};

constexpr const char* kOperationModes_TS004F_knob[] = {
    "command",
    "event",
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
    { "action_rate",            ExposeType::Numeric, Access::State,
      nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State,
      "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State,
      "mV", nullptr, nullptr, 0 },
    { "operation_mode", ExposeType::Enum, Access::StateSet,
      nullptr, "command = group control, event = clicks",
      kOperationModes_TS004F_knob,
      sizeof(kOperationModes_TS004F_knob)/sizeof(kOperationModes_TS004F_knob[0]),
      ExposeCategory::Config },
};

const FzConverter* const kFz_TS004F_knob[] = {
    &::zhc::tuya::kFzTuyaActionFlat,          // genOnOff 0xFD/0xFC (+0x00-02) → single/double/hold/rotate/toggle
    &::zhc::generic::kFzCommandToggle,        // genOnOff cmd 0x02 → toggle (explicit, z2m parity)
    &::zhc::generic::kFzCommandStep,          // genLevelCtrl cmd 0x02 → brightness_step_up/_down
    &::zhc::generic::kFzCommandStepColorTemp, // lightingColorCtrl cmd 0x4C → color_temperature_step_up/_down
    &::zhc::generic::kFzCommandMoveHue,        // lightingColorCtrl cmd 0x01 → hue_move / hue_stop + action_rate
    &::zhc::tuya::kFzTuyaOperationMode,        // genOnOff attr 0x8004 → operation_mode {command,event}
    &::zhc::generic::kFzBattery,              // genPowerCfg 0x0020 + 0x0021
};

const TzConverter* const kTz_TS004F_knob[] = {
    &::zhc::tuya::kTzTuyaOperationMode,        // genOnOff attr 0x8004 write — command/event
};

constexpr WhiteLabel kWhiteLabels_TS004F_knob[] = {
    { "Tuya",  "ZG-101Z_D_1" },
    { "Moes",  "ZG-101ZD"    },
    { "Immax", "07768L"      },
};

constexpr BindingSpec kBindings_TS004F_knob[] = {
    { 1, 0x0001 },   // genPowerCfg — battery reports
    { 1, 0x0006 },   // genOnOff — action events + operation_mode attr
    { 1, 0x0008 },   // genLevelCtrl — cmd-mode brightness step
    { 1, 0x0300 },   // lightingColorCtrl — cmd-mode rotate (step/move-hue)
};

// Tuya magic-packet attrs (genBasic).
constexpr std::uint8_t kMagicAttrs_TS004F_knob[] = {
    0x04, 0x00,  0x00, 0x00,  0x01, 0x00,
    0x05, 0x00,  0x07, 0x00,  0xFE, 0xFF,
};

// operation_mode = "event" (1). genOnOff attr 0x8004 (tuyaOperationMode),
// enum8 0x30 — see kSpecOperationMode in tuya/_shared.cpp.
constexpr std::uint8_t kOpModeEvent_TS004F_knob[] = { 0x01 };

// Configure pipeline — bind-settle, magic packet, then force operation_mode
// to "event" (z2m ERS-10TZBVK-AA: write tuyaOperationMode:1 then read back).
// Field order: op, endpoint, cluster_id, cmd_id, flags, payload, payload_len,
//              wait_ms, manu_code, attr_id, attr_type.
constexpr ConfigStep kConfigSteps_TS004F_knob[] = {
    { ConfigStepOp::Wait, 0, 0,      0,    0, nullptr, 0, 600 },
    { ConfigStepOp::Read, 1, 0x0000, 0x00, 0,
      kMagicAttrs_TS004F_knob, sizeof(kMagicAttrs_TS004F_knob), 2000 },
    { ConfigStepOp::Write, 1, 0x0006, 0, 0,
      kOpModeEvent_TS004F_knob, sizeof(kOpModeEvent_TS004F_knob),
      0, /*manu_code=*/0, /*attr_id=*/0x8004, /*attr_type=*/0x30 },
};

}  // namespace

extern const PreparedDefinition kDef_TS004F_knob{
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
    .to_zigbee                = kTz_TS004F_knob,
    .to_zigbee_count          = sizeof(kTz_TS004F_knob)/sizeof(kTz_TS004F_knob[0]),
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
