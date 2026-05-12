// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Shared Lumi (Aqara / Xiaomi) vendor converters. Every device that
// pulls from this header should carry a `// Tier 2: uses shared lumi
// converters` comment and a `// z2m-source:` provenance line.

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::lumi {

// Decode the Xiaomi `0xFF01` / MI-struct envelope carried on `genBasic`
// attribute reports. Well-known tags emitted into the output payload:
//
//   tag 0x01 (u16 mV)  → "voltage"  + derived "battery" (0-100 via
//                         2850-3000 mV curve — z2m default for coin cells)
//   tag 0x03 (i8 °C)   → "device_temperature"
//   tag 0x04 (u16)     → "power_outage_count"
//
// z2m-source: zigbee-herdsman-converters/src/lib/lumi.ts lumi_basic.
bool fz_lumi_basic(const DecodedMessage& msg,
                    const FzConverter& self,
                    const PreparedDefinition& def,
                    RuntimeContext& ctx,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// Sentinel converter descriptor that picks up `genBasic` attribute
// reports. Reused by every Lumi device that pulls in `fz_lumi_basic`.
extern const FzConverter kFzLumiBasic;

// Decode a `genMultistateInput` 0x0055 presentValue report into an
// `action` string. The raw→label map is supplied per device via the
// FzConverter's `user_config` as a `LumiActionMap*`. Raw values with
// no entry are silently dropped.
//
// Historical default map (kDefaultLumiActionMap) covers the five
// values z2m hardcoded for the simple-button family — used by
// devices that don't supply their own:
//   0  → "hold"
//   1  → "single"
//   2  → "double"
//   3  → "triple"
//   255 → "release"
//
// Increments the per-device `counter` in RuntimeStore so downstream
// tests can observe dispatch counts without extra instrumentation.
//
// z2m-source: zigbee-herdsman-converters/src/lib/lumi.ts
//             lumi_action_multistate.
struct LumiActionEntry {
    std::uint64_t value;
    const char*   action;
};
struct LumiActionMap {
    const LumiActionEntry* entries;
    std::uint8_t           count;
};

extern const LumiActionMap kDefaultLumiActionMap;

bool fz_lumi_action_multistate(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiActionMultistate;

// Xiaomi Mi Cube (MFKZQ01LM) action decoders.
//
// z2m-source: lumi.ts `lumi_action_multistate` + `lumi_action_analog`.
//
// kFzLumiCubeMultistate: cluster genMultistateInput, attr 0x0055 uint16.
// Decodes value ranges:
//   0           → shake
//   1           → throw
//   2           → wakeup
//   3           → fall
//   64..127     → flip90, from_side=(v-64)/8, to_side=v%8, side=v%8
//   128..255    → flip180, side=v-128
//   256..511    → slide, side=v-256
//   512+        → tap, side=v-512
//
// kFzLumiCubeAnalog: cluster genAnalogInput, attr 0x0055 float.
// Emits `action=rotate_left|rotate_right` + `angle` / `action_angle`.
bool fz_lumi_cube_multistate(const DecodedMessage& msg,
                              const FzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzLumiCubeMultistate;

bool fz_lumi_cube_analog(const DecodedMessage& msg,
                          const FzConverter& self,
                          const PreparedDefinition& def,
                          RuntimeContext& ctx,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzLumiCubeAnalog;

// z2m `lumi_power` — simple `presentValue` on genAnalogInput → watts.
// Exposed as `kFzLumiPowerAnalog` because this file's existing
// `kFzLumiPower` has a different meaning (Aqara haElectricalMeasurement
// voltage/current/power with per-device calibration divisors).
//
// z2m-source: lumi.ts `lumi_power` (cluster genAnalogInput).
bool fz_lumi_power_analog(const DecodedMessage& msg,
                           const FzConverter& self,
                           const PreparedDefinition& def,
                           RuntimeContext& ctx,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzLumiPowerAnalog;

// Decode manuSpecificLumi (0xFCC0) attribute reports. Each device
// supplies a `LumiSpecificMap*` in `user_config` mapping attr IDs to
// output keys + value types + optional enum lookup. Exposed as
// `kFzLumiManuSpecific` — `kFzLumiSpecific` already denotes this
// file's historical genBasic 0xFF01 MI-struct converter (legacy
// naming we don't want to break).
//
// z2m-source: lumi.ts `lumi_specific`
//             (numericAttributes2Payload switch — per-device subset).
enum class LumiSpecificType : std::uint8_t {
    Raw,       // passthrough as Int / Uint
    Bool,      // coerce to bool (non-zero = true)
    Enum,      // use enum_table
};
struct LumiSpecificEnumEntry {
    std::uint64_t value;
    const char*   label;
};
struct LumiSpecificEntry {
    std::uint16_t attr_id;
    const char*   out_key;
    LumiSpecificType type;
    const LumiSpecificEnumEntry* enum_table;
    std::uint8_t                 enum_count;
};
struct LumiSpecificMap {
    const LumiSpecificEntry* entries;
    std::uint8_t             count;
};

bool fz_lumi_manu_specific(const DecodedMessage& msg,
                            const FzConverter& self,
                            const PreparedDefinition& def,
                            RuntimeContext& ctx,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzLumiManuSpecific;

// Shared per-sensor-family maps. Every map entry was cross-checked
// against z2m's numericAttributes2Payload branch; devices that
// publish additional attrs add a per-device map instead.
extern const LumiSpecificMap kLumiManuMapMotion;    // RTCGQ*LM T1/T2
extern const LumiSpecificMap kLumiManuMapContact;   // MCCGQ*LM T1/T2
extern const LumiSpecificMap kLumiManuMapTempHum;   // WSDCGQ*LM T1
extern const LumiSpecificMap kLumiManuMapWaterLeak; // SJCGQ*LM T1

// Canonical FzConverters each pointing at one of the shared maps.
// Device cpps just wire the right one into their `fz_list`.
extern const FzConverter kFzLumiManuMotion;
extern const FzConverter kFzLumiManuContact;
extern const FzConverter kFzLumiManuTempHum;
extern const FzConverter kFzLumiManuWaterLeak;

// Canonical manu-specific tz writes (cluster 0xFCC0, manu 0x115F).
// Every Aqara device in z2m's "modern" list for these features can
// wire these directly — no per-device `ZclWriteSpec` needed.
//
// z2m attr ids:
//   power_outage_memory → 0x0201 bool
//   led_disabled_night  → 0x0203 bool
//   button_lock         → 0x0200 bool   (optional per-family)
//   flip_indicator_light→ 0x00F5 bool   (some variants only)
extern const TzConverter kTzLumiPowerOutageMemory;
extern const TzConverter kTzLumiLedDisabledNight;
extern const TzConverter kTzLumiButtonLock;
extern const TzConverter kTzLumiFlipIndicatorLight;

// (kFzLumiCurtainPosition already declared earlier in this header.)

// IAS Zone status — bit 0 = alarm active. The same decoder emits one
// of {water_leak, smoke, gas} based on the device's declared key,
// stored in `user_config` as a `const char*`.
bool fz_lumi_ias_alarm(const DecodedMessage& msg,
                        const FzConverter& self,
                        const PreparedDefinition& def,
                        RuntimeContext& ctx,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzLumiWaterLeak;
extern const FzConverter kFzLumiSmoke;
extern const FzConverter kFzLumiGas;

// Encode a writeAttributes frame for a single Lumi-manu-specific
// attribute on the 0xFCC0 cluster. The encoder is data-driven —
// every device that writes a FCC0 attribute just instantiates a
// `TzConverter` with a `LumiManuWriteSpec*` in `user_config`.
//
// Wire format: fc=0x14 (profile-wide + manu-specific + c→s + DDR)
//              manu_code_LE (0x115F), tsn=0x00 placeholder,
//              cmd=0x02 writeAttributes, [attr_id_LE, type, value].
//
// Input coercion:
//   Bool    input → type 0x10 writes 0/1
//   Uint    input → type 0x20 / 0x21 / 0x23 depending on declared
//                    `attr_type`
//   String  input → looks up via `lookup[]` (if provided)
//
// z2m-source: lumi.ts `tz.lumi_*` family.
struct LumiManuWriteLookup {
    const char*   label;
    std::uint32_t value;
};
struct LumiManuWriteSpec {
    const char*   key;            // payload key to claim
    std::uint16_t attr_id;
    std::uint8_t  attr_type;      // ZCL data-type byte
    std::uint16_t manufacturer_code;  // usually 0x115F
    const LumiManuWriteLookup* lookup;   // optional string → value map
    std::uint8_t               lookup_count;
};

bool tz_lumi_manu_write(std::string_view key,
                         const Value& input,
                         const TzConverter& self,
                         const PreparedDefinition& def,
                         RuntimeContext& ctx,
                         std::span<std::uint8_t> out,
                         std::size_t& out_size);

// Aqara Cube T1 Pro (CTP-R01) multistate decoder. Same z2m source file
// as MFKZQ01LM but per-model branch — different value map and +1 side
// offsets. Ranges:
//   0  → shake
//   1  → throw
//   2  → 1_min_inactivity
//   4  → hold
//   64..127   → flip90,  side=(v%8)+1, action_from_side=((v-64)/8)+1
//   128..255  → flip180, side=v-127, action_from_side=7-v+127
//   256..511  → slide,   side=v-255
//   512..1023 → tap,     side=v-511
//   1024+     → flip_to_side, side=v-1023
//
// z2m-source: lumi.ts `lumi_action_multistate` CTP-R01 branch.
bool fz_lumi_ctpr01_multistate(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);
extern const FzConverter kFzLumiCTPR01Multistate;

// Opple-family per-endpoint action converter. Emits
// `action = "button_<endpoint>_<label>"` from a user-config LumiActionMap
// on genMultistateInput presentValue. Used by Aqara Opple scene
// controllers (WXCJKG11/12/13LM, WXKG04LM).
//
// z2m-source: lumi.ts `lumi_action_multistate` WXKG04LM/WXCJKG* branch.
bool fz_lumi_action_opple(const DecodedMessage& msg,
                           const FzConverter& self,
                           const PreparedDefinition& def,
                           RuntimeContext& ctx,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// WXKG01LM-specific press/release state machine on `genOnOff`. Ports
// z2m's `lumi_action_WXKG01LM` but with a retroactive hold-detection
// (hold is published on release, not after 1 s of silence) — timer-
// based emission is a v2 feature. Covers all 7 exposed actions for
// the device: single / double / triple / quadruple / hold / release /
// many.
//
// Semantics:
//   - attr 0x8000 present (click count u8): 1..4 → single/double/triple/
//     quadruple, anything else → many.
//   - attr 0x0000 state=0: press. Stores `press_at_ms` in RuntimeStore.
//   - attr 0x0000 state=1: release. Duration = now - press_at_ms. If
//     duration >= 1000 → action=hold, else action=single. Always emits
//     a numeric `duration` alongside.
//
// z2m-source: zigbee-herdsman-converters/src/lib/lumi.ts
//             lumi_action_WXKG01LM.
bool fz_lumi_action_WXKG01LM(const DecodedMessage& msg,
                               const FzConverter& self,
                               const PreparedDefinition& def,
                               RuntimeContext& ctx,
                               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiActionWXKG01LM;

// Decode a `msOccupancySensing` 0x0000 attribute report into an
// `occupancy` bool. Trivial Tier-1-style mapping but lives here
// because most Aqara PIR sensors share the same shape.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             occupancy.
bool fz_occupancy(const DecodedMessage& msg,
                   const FzConverter& self,
                   const PreparedDefinition& def,
                   RuntimeContext& ctx,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzOccupancy;

// ── fz_lumi_specific + TagMap ──────────────────────────────────────
//
// Extends `fz_lumi_basic` to decode the richer Xiaomi / Aqara TLV tag
// set (energy/power on plugs, calibrated temperature/humidity on
// sensors, etc.). Tag semantics are not universal — tag 0x95 means
// energy on one device and a battery residue on another — so this
// converter resolves each tag via a per-device `LumiTagMap` carried
// in `PreparedDefinition::meta`.
//
// A device pulls this converter like so:
//
//   constexpr LumiTagEntry kZNCZ02LMTags[] = {
//       { 0x95, "energy", 1000 },   // energy kWh = raw / 1000
//       { 0x98, "power",  10 },     // power  W   = raw / 10
//   };
//   constexpr LumiTagMap kZNCZ02LMTagMap{
//       .entries = kZNCZ02LMTags,
//       .count   = sizeof(kZNCZ02LMTags) / sizeof(kZNCZ02LMTags[0]),
//   };
//   // PreparedDefinition::meta = &kZNCZ02LMTagMap;
//
// `divisor == 0` is treated as 1 (raw pass-through). Integer tags with
// a non-unit divisor are emitted as Float. Uint / Int / Bool inputs
// are all handled.

struct LumiTagEntry {
    std::uint16_t tag;        // MI-struct tag id
    const char*   key;        // output key to emit
    std::uint32_t divisor;    // 0 or 1 → integer pass-through
};

struct LumiTagMap {
    const LumiTagEntry* entries;
    std::uint8_t        count;
};

// Well-known tag ids — use these when building a device's map so the
// tag-value magic numbers don't drift across files.
constexpr std::uint16_t kLumiTagVoltage       = 0x01;  // u16 mV
constexpr std::uint16_t kLumiTagDeviceTemp    = 0x03;  // i8 °C
constexpr std::uint16_t kLumiTagOutageCount   = 0x04;  // u16
constexpr std::uint16_t kLumiTagState         = 0x64;  // varies (temp/state)
constexpr std::uint16_t kLumiTagHumidity      = 0x65;  // u16 %rh * 100
constexpr std::uint16_t kLumiTagPressure      = 0x66;  // u32 kPa * 100
constexpr std::uint16_t kLumiTagEnergy        = 0x95;  // float kWh
constexpr std::uint16_t kLumiTagPower         = 0x98;  // float W

bool fz_lumi_specific(const DecodedMessage& msg,
                       const FzConverter& self,
                       const PreparedDefinition& def,
                       RuntimeContext& ctx,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiSpecific;

// ── fz_lumi_power + fz_lumi_electricity_meter ──────────────────────
//
// `fz_lumi_power` decodes `haElectricalMeasurement`:
//   attr 0x0505 rmsVoltage  → "voltage"  (V)
//   attr 0x0508 rmsCurrent  → "current"  (A)
//   attr 0x050B activePower → "power"    (W)
//
// `fz_lumi_electricity_meter` decodes `seMetering`:
//   attr 0x0000 currentSummDelivered → "energy" (kWh)
//
// Each raw value is divided by a per-device divisor carried in
// `FzConverter::user_config`:
//
//   struct LumiPowerCalibration     { u32 voltage_div, current_div, power_div; };
//   struct LumiMeterCalibration     { u32 energy_div; };
//
// Rationale: real scaling comes from the device's own attrs 0x0301
// (multiplier) / 0x0302 (divisor) on `haElectricalMeasurement`, but
// those only arrive after an interview-time read. Until the interview
// flow lands, the per-device calibration is hard-coded in the device's
// .cpp (ZNCZ02LM → power /10, energy /1000 — matches z2m). When the
// calibration is null the converter emits raw integers so the adapter
// can still see activity, just without a unit.

struct LumiPowerCalibration {
    std::uint32_t voltage_div;   // 0 → raw pass-through
    std::uint32_t current_div;
    std::uint32_t power_div;
};

struct LumiMeterCalibration {
    std::uint32_t energy_div;
};

bool fz_lumi_power(const DecodedMessage& msg,
                    const FzConverter& self,
                    const PreparedDefinition& def,
                    RuntimeContext& ctx,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiPower;

bool fz_lumi_electricity_meter(const DecodedMessage& msg,
                                 const FzConverter& self,
                                 const PreparedDefinition& def,
                                 RuntimeContext& ctx,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiElectricityMeter;

// ── Remaining shared converters ────────────────────────────────────
//
// Thin Lumi-specific wrappers that show up across many device
// families. Each reads a single well-known attribute and maps to one
// canonical key. Per-device tweaks (endpoint label, action map) are
// threaded via `FzConverter::user_config`.

// `genOnOff` attr 0x0000 bool → "contact" inverted. Aqara contact
// sensors report 0=open / 1=closed. z2m emits `contact: true` when
// closed, matching home-assistant convention.
//
// z2m-source: lumi.ts `fz.lumi_contact`.
bool fz_lumi_contact(const DecodedMessage& msg,
                      const FzConverter& self,
                      const PreparedDefinition& def,
                      RuntimeContext& ctx,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiContact;

// Endpoint-aware `fz_lumi_on_off`. For single-endpoint devices it
// emits `state`; for multi-button wall switches the
// `DeviceEndpointLabels*` in user_config maps the frame's
// `src_endpoint` to a full output key.
//
// `label` must be the *complete* emitted key (e.g. "state_left") so
// the dispatch pipeline can stash it in FixedPayload as a pointer —
// there is no runtime string concatenation.
struct DeviceEndpointLabel {
    std::uint8_t endpoint;
    const char*  label;
};
struct DeviceEndpointLabels {
    const DeviceEndpointLabel* entries;
    std::uint8_t               count;
};

bool fz_lumi_on_off(const DecodedMessage& msg,
                     const FzConverter& self,
                     const PreparedDefinition& def,
                     RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiOnOff;

// `genOnOff` attr 0x8000 u8 click-count → "action". Per-device map in
// user_config (LumiActionMap*); falls back to kDefaultLumiActionMap
// when null.
bool fz_lumi_action(const DecodedMessage& msg,
                     const FzConverter& self,
                     const PreparedDefinition& def,
                     RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiAction;

// `genBasic` attr 0xFF22 u8 → "operation_mode". Raw 0/1/2 mapped via
// an attached LumiActionMap (keys reused — format is the same).
bool fz_lumi_operation_mode_basic(const DecodedMessage& msg,
                                    const FzConverter& self,
                                    const PreparedDefinition& def,
                                    RuntimeContext& ctx,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiOperationModeBasic;

// `genAnalogOutput` attr 0x0055 float → "position" (0-100).
// Aqara curtains report position as a percentage already; no scaling.
bool fz_lumi_curtain_position(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiCurtainPosition;

// `closuresDoorLock` (0x0101) attr 0x0000 u8 → "lock_state" via a
// canonical lookup table (1 = unlocked, 2 = locked, 0 = not_fully_locked).
bool fz_lumi_door_lock_report(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

extern const FzConverter kFzLumiDoorLockReport;

}  // namespace zhc::lumi
