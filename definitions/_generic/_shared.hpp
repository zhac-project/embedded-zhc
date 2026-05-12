// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Generic, vendor-neutral converters. Every device that pulls from here
// should carry a `// Tier 1: generic converters only` comment so
// reviewers see coverage shape at a glance.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::generic {

// Decode a `genOnOff` attribute report (attr 0x0000, bool) into
//   "state" → bool (true = on).
//
// Raw mapping only; vendor-specific helpers (retroactive hold
// detection, double-press decoding, etc.) live elsewhere.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             on_off.
bool fz_on_off(const DecodedMessage& msg,
                const FzConverter& self,
                const PreparedDefinition& def,
                RuntimeContext& ctx,
                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzOnOff;

// Decode `genPowerCfg` battery reports. Raw pass-through:
//   attr 0x0020 (u8, 100 mV units) → "voltage"  as u8
//   attr 0x0021 (u8, half-percent) → "battery"  as u8
//
// No derivation: per-device `meta.battery` curves live in the
// meta-aware path. z2m's behaviour is to derive battery % from voltage
// when percent is missing; that stays a v2 concern.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             battery.
bool fz_battery(const DecodedMessage& msg,
                 const FzConverter& self,
                 const PreparedDefinition& def,
                 RuntimeContext& ctx,
                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzBattery;

// genLevelCtrl attr 0x0000 u8 → "brightness" (0-254 raw).
bool fz_brightness(const DecodedMessage& msg,
                    const FzConverter& self,
                    const PreparedDefinition& def,
                    RuntimeContext& ctx,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzBrightness;

// lightingColorCtrl attr 0x0007 u16 → "color_temp" (mireds, raw).
bool fz_color_temperature(const DecodedMessage& msg,
                            const FzConverter& self,
                            const PreparedDefinition& def,
                            RuntimeContext& ctx,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzColorTemperature;

// lightingColorCtrl color report decoder. Handles every attribute
// flavour z2m's `fz.color_colortemp` covers in one converter:
//   attr 0x0000 currentHue            (u8 [0,254])  → "hue"
//   attr 0x0001 currentSaturation     (u8 [0,254])  → "saturation"
//   attr 0x0003 currentX              (u16)          → "color_x" Float [0,1]
//   attr 0x0004 currentY              (u16)          → "color_y" Float [0,1]
//   attr 0x0007 colorTemperature      (u16, mireds)  → "color_temp" (raw)
//   attr 0x0008 colorMode             (enum8)        → "color_mode"
//                                                       ("hs" / "xy" / "color_temp")
//   attr 0x4000 enhancedCurrentHue    (u16)          → "enhanced_hue"
// XY are encoded by z2m as `raw / 65535` per the ZCL CIE 1931 spec.
bool fz_color(const DecodedMessage& msg,
               const FzConverter& self,
               const PreparedDefinition& def,
               RuntimeContext& ctx,
               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzColor;

// genOnOff attr 0x4003 startUpOnOff (enum8) → "power_on_behavior"
// ("off" / "on" / "toggle" / "previous").
bool fz_power_on_behavior(const DecodedMessage& msg,
                            const FzConverter& self,
                            const PreparedDefinition& def,
                            RuntimeContext& ctx,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzPowerOnBehavior;

// Drop-through converter that matches and returns true without
// emitting anything. Exists so devices that receive noisy reports on a
// cluster they don't care about can still claim the frame as handled —
// prevents the adapter's `unhandled` hook from firing.
//
// Use via the `ZHC_FZ_IGNORE(name, cluster_str)` macro below; the
// factory is the macro + this shared body.
bool fz_ignore(const DecodedMessage& msg,
                const FzConverter& self,
                const PreparedDefinition& def,
                RuntimeContext& ctx,
                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Pre-stamped ignore descriptors for the two z2m cases that ship
// today (`fz.ignore_occupancy_report`, `fz.ignore_illuminance_report`).
// Add more via the macro when a device asks for them.
extern const FzConverter kFzIgnoreOccupancyReport;
extern const FzConverter kFzIgnoreIlluminanceReport;

// ── Cluster-command action converters (for battery-powered remotes) ─
//
// Each emits `action = "<label>"` when the device sends the matching
// cluster-specific command. Direction is Client→Server because a
// remote control is the client of On/Off / LevelCtrl / ColorCtrl.
// Wire them into a device's `from_zigbee` list and pair with
// `e.action(...)` exposes to publish button events.
//
// z2m-source: command_on / command_off / command_toggle /
//             command_move_to_level / command_move_to_color_temp /
//             command_move_to_hue_and_saturation in fromZigbee.ts.
extern const FzConverter kFzCommandOn;
extern const FzConverter kFzCommandOff;
// `genOnOff.commandOffWithEffect` (cluster 0x0006, cmd 0x40) — same
// semantics as plain command_off for the action stream (z2m emits
// `action: "off"`); the trailing effect_id / variant payload bytes
// are ignored. Used by Insta InstaRemote (Gira/Jung ZLL transmitters)
// + Hue dimmer-switch and a handful of OEM-rebadged ZLL remotes.
// z2m-source: fromZigbee.ts `fz.command_off_with_effect`.
extern const FzConverter kFzCommandOffWithEffect;
extern const FzConverter kFzCommandToggle;
extern const FzConverter kFzCommandMoveToLevel;
extern const FzConverter kFzCommandMoveToColorTemp;
extern const FzConverter kFzCommandMoveToHueAndSaturation;

// `genLevelCtrl.commandStep` (cluster 0x0008, cmd 0x02) — used by
// rotary / scroll-wheel remotes (TS004F smart-knob family). Payload:
//   step_mode u8 (0 = up, 1 = down), step_size u8,
//   transition_time u16 LE (units of 1/10 s).
// Emits:
//   action                 = "brightness_step_up" or "_down"
//   action_step_size       = step_size (0..254)
//   action_transition_time = transition_time / 10  (seconds, as Float)
extern const FzConverter kFzCommandStep;

// `genLevelCtrl.commandMove` (cluster 0x0008, cmd 0x01) — continuous
// brightness move while button is held. Payload: move_mode u8
// (0 = up, 1 = down), rate u8 (units per second).
// Emits:
//   action      = "brightness_move_up" or "_down"
//   action_rate = rate
extern const FzConverter kFzCommandMove;

// `genLevelCtrl.commandStop` (cluster 0x0008, cmd 0x03 or 0x07) —
// fired when the user releases a held brightness button. No payload.
// Emits: action = "brightness_stop"
extern const FzConverter kFzCommandStop;
extern const FzConverter kFzCommandStopWithOnOff;    // variant cmd 0x07

// `closuresWindowCovering` (cluster 0x0102) cluster-specific commands
// on window-cover remotes:
//   cmd 0x00 upOpen    → action = "open"
//   cmd 0x01 downClose → action = "close"
//   cmd 0x02 stop      → action = "stop"
extern const FzConverter kFzCommandCoverOpen;
extern const FzConverter kFzCommandCoverClose;
extern const FzConverter kFzCommandCoverStop;

// `msTemperatureMeasurement` (cluster 0x0402) attribute 0x0000
// `measuredValue` (s16 in 0.01 °C). Emits
//   temperature = raw / 100.0  (Float, °C)
// z2m-source: fromZigbee.ts `fz.temperature`.
extern const FzConverter kFzTemperature;

// `msRelativeHumidity` (cluster 0x0405) attribute 0x0000 (u16 0.01 %).
//   humidity = raw / 100.0
// z2m-source: fromZigbee.ts `fz.humidity`.
extern const FzConverter kFzHumidity;

// `msPressureMeasurement` (cluster 0x0403) attr 0x0000 (s16 hPa).
//   pressure = raw  (hPa direct)
// z2m-source: fromZigbee.ts `fz.pressure`.
extern const FzConverter kFzPressure;

// `msIlluminanceMeasurement` (cluster 0x0400) attr 0x0000 (u16 raw).
//   illuminance = raw
// z2m-source: fromZigbee.ts `fz.illuminance`.
extern const FzConverter kFzIlluminance;

// `ssIasZone` (cluster 0x0500) commandStatusChangeNotification (cmd 0x00):
// Payload = { zoneStatus:u16, extendedStatus:u8, zoneId:u8, delay:u16 }.
// zoneStatus bit decoding:
//   bit 0  alarm_1       → occupancy / contact / water_leak / smoke / …
//   bit 1  alarm_2       → secondary alarm
//   bit 2  tamper        → tamper
//   bit 3  battery_low   → battery_low
// We emit all four as booleans; the hardware type (motion vs contact vs …)
// is a per-device label choice that lives in the `exposes` list.
extern const FzConverter kFzIasZoneStatusChange;

// Type-specific IAS status-change decoders — same wire shape as
// `kFzIasZoneStatusChange` but emit semantic labels that match the
// device's zone type instead of the generic `alarm_1` / `alarm_2`.
//
// Pick ONE of these per device:
//   - `kFzIasContactAlarm`   zoneType 0x0015 — door/window contact
//   - `kFzIasMotionAlarm`    zoneType 0x000D — motion / occupancy
//   - `kFzIasWaterLeakAlarm` zoneType 0x002A — water leak
//   - `kFzIasSmokeAlarm`     zoneType 0x0028 — smoke
//   - `kFzIasCoAlarm`        zoneType 0x002B — carbon monoxide
//   - `kFzIasVibrationAlarm` zoneType 0x002D — vibration / glass break
//
// Non-alarm bits stay unchanged: tamper (bit 2), battery_low (bit 3).
// z2m-source: fromZigbee.ts `fz.ias_{contact,occupancy,water_leak,
// smoke,co,vibration}_alarm_1`.
extern const FzConverter kFzIasContactAlarm;
extern const FzConverter kFzIasMotionAlarm;
// Variant of `kFzIasMotionAlarm` reading zoneStatus bit 1 instead of
// bit 0. Matches z2m's `fz.ias_occupancy_alarm_2`. Used by iris
// IL07_1, iris 3326-L, centralite 3328-G.
extern const FzConverter kFzIasMotionAlarm2;

// ColorCtrl move-style command decoders (z2m fz.command_move_hue /
// fz.command_move_to_color / fz.command_move_color_temperature).
//   kFzCommandMoveHue            — cmd 0x01 → action = "hue_<move|down|stop>" + action_rate.
//   kFzCommandMoveToColor        — cmd 0x07 → action = "color_move" + action_color_x/y + action_transition_time.
//   kFzCommandMoveColorTemperature — cmd 0x4B → action = "color_temperature_move_<up|down|stop>"
//                                                + action_rate + action_minimum + action_maximum.
extern const FzConverter kFzCommandMoveHue;
extern const FzConverter kFzCommandMoveToColor;
extern const FzConverter kFzCommandMoveColorTemperature;

// ssIasAce arm / panic / emergency command decoders.
//   kFzIasAceArm       — cmd 0x00 → action="<armMode>" + action_code + action_zone.
//   kFzIasAcePanic     — cmd 0x04 → action="panic".
//   kFzIasAceEmergency — cmd 0x02 → action="emergency". z2m fz.command_emergency.
extern const FzConverter kFzIasAceArm;
extern const FzConverter kFzIasAcePanic;
extern const FzConverter kFzIasAceEmergency;

// ssIasWd shorthand siren/squawk encoders.
//   kTzIasWdWarningSimple — cmd 0x00 (startWarning). Accepts `alarm`
//                            (Bool) or StringRef "stop"/"burglar"/
//                            "fire"/"emergency"/"police_panic"/etc.
//                            Mirrors z2m tz.warning_simple shorthand.
//   kTzIasWdSquawk        — cmd 0x02 (squawk). Accepts Bool
//                            (true=armed/false=disarmed) or
//                            StringRef "system_is_armed" /
//                            "system_is_disarmed".
extern const TzConverter kTzIasWdWarningSimple;
extern const TzConverter kTzIasWdSquawk;

// ColorCtrl extras for vendor remotes.
//   kFzCommandEnhancedMoveToHueAndSat — cmd 0x43 → action +
//     action_enhanced_hue + action_hue (mapped 0..360) +
//     action_saturation + action_transition_time.
//   kFzCommandColorLoopSet            — cmd 0x44 → action lookup
//     (deactivate / activate_from_color_loop_start_enhanced_hue /
//      activate_from_enhanced_current_hue) + action_update_flags.
extern const FzConverter kFzCommandEnhancedMoveToHueAndSat;
extern const FzConverter kFzCommandColorLoopSet;

// genBasic write of attr 0x4005 (16389) → action="scene_<n>". z2m
// `fz.tint_scene` equivalent. Used by Tint / Paul Neuhaus remotes
// that surface scene picks via attribute writes.
extern const FzConverter kFzTintScene;
extern const FzConverter kFzIasWaterLeakAlarm;
extern const FzConverter kFzIasSmokeAlarm;
extern const FzConverter kFzIasCoAlarm;
extern const FzConverter kFzIasVibrationAlarm;

// ── genOnOff power-on behaviour (standard ZCL attribute 0x4003) ─────
//
// z2m-source: fromZigbee.ts `fz.power_on_behavior_1` + toZigbee.ts
// `tz.power_on_behavior_1`. Tuya's 0x8002 alternate lives in
// `tuya/_shared.{hpp,cpp}` as `kFzTuyaPowerOnBehavior` /
// `kTzTuyaPowerOnBehavior` — this pair is the non-Tuya variant that
// uses the spec-defined StartUpOnOff attribute.
//
// Values emitted / accepted as `power_on_behavior`:
//   0 → "off"      1 → "on"       2 → "toggle"     255 → "previous"
extern const FzConverter kFzPowerOnBehavior1;
extern const TzConverter kTzPowerOnBehavior1;

// ── Sonoff / eWeLink button action (SNZB-01 family) ──────────────────
//
// z2m-source: fromZigbee.ts `fz.ewelink_action`. genOnOff command
// codes are repurposed as click types:
//   0x00 off    → "double"
//   0x01 on     → "single"
//   0x02 toggle → "long"
// Direction is Client→Server; payload is empty (body is the cmd_id
// alone). Used by any device that decorates genOnOff commands with
// eWeLink's 3-click convention.
extern const FzConverter kFzEwelinkAction;

// ── IKEA TRADFRI arrow buttons (genScenes custom cmds) ───────────────
//
// z2m-source: ikea.ts `ikeaArrowClick`. IKEA adds three custom
// genScenes (0x0005) client→server commands:
//   0x07 tradfriArrowSingle   — u16 value + u16 value2
//                                 value 257 → arrow_left_click
//                                 value 256 → arrow_right_click
//                                 value   2 → ignored (toggle hold)
//   0x08 tradfriArrowHold     — u16 value
//                                 value 3329 → arrow_left_hold
//                                 else       → arrow_right_hold
//   0x09 tradfriArrowRelease  — u16 value (duration in ms)
//                                 emits bare `arrow_release` plus
//                                 `action_duration` (seconds). Direction
//                                 is not carried on the wire — z2m
//                                 recovers it from globalStore; we do
//                                 not, since fz converters here are
//                                 stateless. Consumers can pair the
//                                 release with the preceding hold.
extern const FzConverter kFzIkeaArrowClick;
extern const FzConverter kFzIkeaArrowHold;
extern const FzConverter kFzIkeaArrowRelease;

// ── lightingColorCtrl StepColorTemperature (cmd 0x4C) ───────────────
//
// z2m-source: `fz.command_step_color_temperature`. Payload (LE):
//   step_mode u8  — 0x01 = up, 0x03 = down
//   step_size u16
//   transition_time u16
//   color_temperature_minimum_mireds u16 (unused by us)
//   color_temperature_maximum_mireds u16 (unused by us)
// Emits `action = "color_temperature_step_up"` / `_down` plus
// `action_step_size` (raw) and `action_transition_time` (seconds).
// Used by TS004F-class rotary knobs in "command mode".
extern const FzConverter kFzCommandStepColorTemp;

// ── Philips Hue dimmer switch (manuSpecificPhilips 0xFC00) ───────────
//
// z2m-source: philips.ts `philips.fz.hue_dimmer_switch`. The Hue
// dimmer RWL020/021 (gen 1) and RWL022 (gen 2) emit
// `hueNotification` (cmd 0x00) commandsResponse frames on the
// manu-specific `manuSpecificPhilips` cluster. Payload (LE):
//   button  u8  — 1 on, 2 up, 3 down, 4 off
//   unknown u24 — skipped
//   type    u8  — 0 press, 1 hold, 2 press_release, 3 hold_release
//   unknown u8, time u8, unknown u8
// Emitted action: `"{button}_{type}"` e.g. "on_press" / "up_hold".
// Direction is server→client (device is the cluster server). z2m's
// stateful duration + simulated-brightness paths are not replicated —
// consumers pair press/release events themselves.
extern const FzConverter kFzHueDimmerNotification;

// ── genScenes Recall (cmd 0x05) ──────────────────────────────────────
//
// z2m-source: `fz.command_recall`. Payload: group u16 LE, scene u8.
// Emits `action: "recall_<scene>"` (e.g. "recall_0"). Used by Hue
// RWL022 gen-2 dimmer for its lower-right "off" scene-cycle button
// and by assorted scene-controllers that want a single-button scene
// select.
extern const FzConverter kFzCommandRecall;

// ── genScenes 0x04 store-scene → "store_<scene>" action ──────────────
//
// z2m-source: fromZigbee.ts `fz.command_store`.
//
// Mirrors `kFzCommandRecall` but for command 0x04 (store) instead of
// 0x05 (recall). Used by icasa KPD14S / KPD18S keypads for their
// long-press scene-store buttons.
extern const FzConverter kFzCommandStore;

// ── Window covering encoders ─────────────────────────────────────────
//
// z2m-source: toZigbee.ts `tz.cover_state` / `tz.cover_position_tilt`.
//
// `kTzCoverState` accepts input key `state` with string values
// "OPEN" / "CLOSE" / "STOP" (case-insensitive in the common devices).
// Emits `closuresWindowCovering` (0x0102) cmd 0x00 / 0x01 / 0x02 —
// same cluster + commands as the cover-remote decoders above but in
// the outbound direction.
extern const TzConverter kTzCoverState;

// `kTzCoverPositionLift` — `closuresWindowCovering` cmd 0x05
// (goToLiftPercentage). Accepts key `position` with numeric 0..100.
extern const TzConverter kTzCoverPositionLift;

// `kTzCoverPositionTilt` — `closuresWindowCovering` cmd 0x08
// (goToTiltPercentage). Accepts key `tilt` with numeric 0..100.
// Mirrors z2m tz.cover_position_tilt. Used by tilt-aware blinds
// (somfy 1245600, 1245602, insta 57008000).
extern const TzConverter kTzCoverPositionTilt;

// Standard-cluster generic converters shared by vendors that layer
// `fz.cover_position_tilt` / `fz.metering` / `fz.electrical_measurement`
// / `fz.thermostat` over vanilla ZCL (no manuspecific quirks).
extern const FzConverter kFzCoverPosition;
extern const FzConverter kFzMetering;
extern const FzConverter kFzElectricalMeasurement;
extern const FzConverter kFzThermostat;
extern const FzConverter kFzIasZone;
extern const FzConverter kFzFanMode;
extern const FzConverter kFzLock;

// ── Outbound encoders ───────────────────────────────────────────────
//
// Every encoder below writes a cluster-specific ZCL command frame into
// `out_frame`:
//
//   [0] frame control: 0x11
//       (bit0 cluster-specific, bit4 disable-default-response)
//   [1] transaction sequence number — left at 0; the platform adapter
//       patches the current per-device counter before TX. Ownership of
//       TSN lives in the adapter, not here.
//   [2] command id
//   [3..] command payload (little-endian multi-byte fields)
//
// The adapter reads `TzConverter::cluster_id` for radio routing and
// supplies the endpoint out-of-band (user request + device config).
// Returns `false` and leaves `out_size = 0` on malformed input.

// genOnOff (0x0006) "state" setter. Accepts:
//   Value::Bool        — true → 0x01 on, false → 0x00 off
//   Value::StringRef   — "ON" / "OFF" / "TOGGLE" (case-sensitive; z2m
//                        sends uppercase)
bool tz_on_off(std::string_view key,
                const Value& input,
                const TzConverter& self,
                const PreparedDefinition& def,
                RuntimeContext& ctx,
                std::span<std::uint8_t> out_frame,
                std::size_t& out_size);

extern const TzConverter kTzOnOff;

// genLevelCtrl (0x0008) "brightness" setter. Emits the
// `moveToLevelWithOnOff` (0x04) command so the device wakes from off
// when a non-zero brightness arrives. `transition_time` is fixed at 0
// in v1; per-device curves can override via `user_config` later.
//
// Accepts `Value::Uint` with 0 ≤ level ≤ 254.
bool tz_brightness(std::string_view key,
                    const Value& input,
                    const TzConverter& self,
                    const PreparedDefinition& def,
                    RuntimeContext& ctx,
                    std::span<std::uint8_t> out_frame,
                    std::size_t& out_size);

extern const TzConverter kTzBrightness;

// lightingColorCtrl (0x0300) "color_temp" setter. Emits the
// `moveToColorTemp` (0x0A) command. Accepts `Value::Uint` mireds.
bool tz_color_temp(std::string_view key,
                    const Value& input,
                    const TzConverter& self,
                    const PreparedDefinition& def,
                    RuntimeContext& ctx,
                    std::span<std::uint8_t> out_frame,
                    std::size_t& out_size);

extern const TzConverter kTzColorTemp;

// lightingColorCtrl (0x0300) wildcard color setter. Single TzConverter
// claiming any of `color_x` / `color_y` / `hue` / `saturation`, emitting
// the matching ZCL command:
//
//   key="color_x" or "color_y"   → moveToColor (0x07)
//        payload: x u16 LE, y u16 LE, transition u16 LE
//        Value can be Float in [0,1] (z2m shape) or Uint raw u16
//        ([0, 65535]). Missing axis defaults to 0x8000 (centre).
//   key="hue"                    → moveToHue (0x00)
//        payload: hue u8 [0,254], direction u8 (0 shortest), transition u16
//   key="saturation"             → moveToSaturation (0x03)
//        payload: saturation u8 [0,254], transition u16
//
// `key=nullptr` makes the converter wildcard: its own body decides
// which keys to honour and rejects the rest cleanly so other Tz
// converters in the array can still match.
//
// z2m-source: toZigbee.ts `tz.light_color` /
//             `tz.light_hue_saturation_move`.
bool tz_color(std::string_view key,
               const Value& input,
               const TzConverter& self,
               const PreparedDefinition& def,
               RuntimeContext& ctx,
               std::span<std::uint8_t> out_frame,
               std::size_t& out_size);

extern const TzConverter kTzColor;

// closuresWindowCovering (0x0102) position/state setter.
extern const TzConverter kTzCoverPosition;

// ssIasWd (0x0502) warning/siren startWarning (cmd 0x00) encoder.
extern const TzConverter kTzWarning;

// closuresDoorLock (0x0101) lock/unlock command encoder.
extern const TzConverter kTzLock;

// hvacThermostat (0x0201) writeAttributes — supports
// occupied/current_heating_setpoint (attr 0x0012) + system_mode
// (0x001C). Additional attrs via per-device `tz_zcl_write_attr`.
extern const TzConverter kTzThermostat;

// Data-driven ZCL writeAttributes encoder. Handles both standard
// (fc=0x10) and manufacturer-specific (fc=0x14) writes via
// `manufacturer_code` — zero means non-manu.
//
// Devices wire per-attribute TzConverters with a
// `ZclWriteSpec*` in `user_config`. Supports Bool/Uint8/Uint16/Uint32
// /Int8/Int16 + optional string-label → uint lookup.
struct ZclWriteLookup {
    const char*   label;
    std::uint32_t value;
};
// `flags` bit 0 (`kZclWriteFlagInvertBool`) inverts a Bool input before
// encoding — matches z2m's `valueOn=[true,0]` / `valueOff=[false,1]`
// pattern where the device's register is the opposite polarity from the
// user-facing label (e.g. zigbeetlc enable_display: 0=on, 1=off).
constexpr std::uint8_t kZclWriteFlagInvertBool = 0x01;

struct ZclWriteSpec {
    const char*   key;                  // input payload key
    std::uint16_t attr_id;
    std::uint8_t  attr_type;            // ZCL data-type byte
    std::uint16_t manufacturer_code;    // 0 = non-manu-specific
    const ZclWriteLookup* lookup;       // optional string-lookup
    std::uint8_t          lookup_count;
    std::uint8_t          flags = 0;    // kZclWriteFlag*
};

bool tz_zcl_write_attr(std::string_view key,
                        const Value& input,
                        const TzConverter& self,
                        const PreparedDefinition& def,
                        RuntimeContext& ctx,
                        std::span<std::uint8_t> out_frame,
                        std::size_t& out_size);

// closuresDoorLock command 0x20 (programming-event notification).
// Decodes program_event_code into z2m's `action` enum
// ("master_code_changed" / "pin_code_added" / …) plus `action_user`,
// `action_source`, `action_source_name`. Use cross-vendor on locks.
extern const FzConverter kFzLockProgrammingEvent;

// closuresDoorLock command 0x21 (operation-event notification).
// Decodes op_event_code into `action` ("lock" / "unlock" /
// "key_unlock" / …) plus the same auxiliary fields as above.
extern const FzConverter kFzLockOperationEvent;

// closuresDoorLock setPinCode (cmd 0x05) / clearPinCode (cmd 0x07).
// Input format is a `StringRef` of `"<user_id>:<pin_digits>"`. An
// empty pin (e.g. `"5:"`) emits clearPinCode for that slot. user
// status defaults to enabled (1), user type to unrestricted (0).
extern const TzConverter kTzLockPinCode;

// closuresDoorLock soundVolume attribute write (0x0024 ENUM8).
// Accepts string label "silent" / "low" / "high".
extern const TzConverter kTzLockSoundVolume;

// closuresDoorLock autoRelockTime attribute write (0x0023 UINT32).
// Accepts numeric seconds OR a Bool (true/false → 1/0). The Bool
// coercion in `tz_zcl_write_attr` lets a single converter cover both
// z2m's `tz.lock_auto_relock_time` and `tz.easycode_auto_relock`.
extern const TzConverter kTzLockAutoRelockTime;

// closuresDoorLock commandGetUserStatusRsp decoder (cmd 0x05).
// Emits `user_id` (Uint), `user_status_raw` (Uint), and
// `user_status` (StringRef "available"/"enabled"/"disabled" when
// recognised). z2m fz.lock_user_status_response equivalent.
extern const FzConverter kFzLockUserStatusResponse;

// ssIasWd `maxDuration` (cluster 0x0502 attr 0x0000, UINT16 seconds).
// Read-side decoder + write-side encoder. Closes z2m
// `tz.ias_max_duration` + `tz.ts0216_duration` (latter writes the
// same attr) and the matching attribute-report decode.
extern const FzConverter kFzIasWdMaxDuration;
extern const TzConverter kTzIasWdMaxDuration;

// msOccupancySensing 0x0000 — emits `occupancy: bool` from bit 0
// of the bitmap8 attribute. z2m fz.occupancy equivalent.
extern const FzConverter kFzOccupancy;

// hvacThermostat setpoint-limit attribute writes (INT16, 0.01 °C).
// Caller supplies the already-scaled int (e.g. 1500 = 15.00 °C).
extern const TzConverter kTzMinHeatSetpointLimit;   // attr 0x0015
extern const TzConverter kTzMaxHeatSetpointLimit;   // attr 0x0016
extern const TzConverter kTzMinCoolSetpointLimit;   // attr 0x0017
extern const TzConverter kTzMaxCoolSetpointLimit;   // attr 0x0018

// hvacThermostat setpointRaiseLower (cluster cmd 0x00). Input:
// `Int`/`Uint` amount in 0.1 °C steps (mode defaults to both), or
// `StringRef` "heat:<steps>" / "cool:<steps>" / "both:<steps>" /
// "<steps>".
extern const TzConverter kTzSetpointRaiseLower;

}  // namespace zhc::generic

// Declare an ignore-style FzConverter at namespace scope. Lives in
// `.rodata` because every field is constexpr-initialised.
#define ZHC_FZ_IGNORE(var_name, cluster_str)                          \
    extern const ::zhc::FzConverter var_name{                         \
        .family            = ::zhc::FrameFamily::Zcl,                 \
        .cluster           = cluster_str,                             \
        .type_mask         = ::zhc::type_bit(                         \
                                 ::zhc::MessageType::AttributeReport) \
                           | ::zhc::type_bit(                         \
                                 ::zhc::MessageType::ReadResponse),   \
        .command_id        = ::zhc::WILDCARD_CMD_ID,                  \
        .attr_id           = ::zhc::WILDCARD_ATTR_ID,                 \
        .endpoint          = ::zhc::WILDCARD_ENDPOINT,                \
        .frame_flags_mask  = 0,                                       \
        .frame_flags_value = 0,                                       \
        .direction         = ::zhc::Direction::ServerToClient,        \
        .fn                = { .zcl_fn = ::zhc::generic::fz_ignore }, \
        .user_config       = nullptr,                                 \
    }
