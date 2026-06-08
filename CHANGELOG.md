# Changelog

All notable changes to **embedded-zhc** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to the `vYYYYMMDDVV` versioning scheme used
across the ZHAC platform.

## [Unreleased]

### Fixed

- Tuya soil-sensor temperature 10x scaling bug. The three soil parent defs
  (`definitions/tuya/TZE200_soil.cpp`, `TZE200_soil_ec.cpp`,
  `TZE200_soil_th.cpp`) decoded the temperature DP (dp 5) with the
  `dp::temperature` default `/10`, but z2m's `TS0601_soil` family decodes it
  as `tuya.valueConverter.raw` (`/1`) — a 10x error (a 25 C reading rendered
  as 2.5 C). Switched all three to `dp::temperature(5, 1)` for z2m parity.
  Covered by `tests/test_tze200_soil.cpp`.

### Added

- Tuya smart-plug attribute reporting + bindings (Phase-2b Batch 2).
  The generic Tuya plug parents `TS011F` (`TS011F_plug_1`) and `TS0121`
  (`TS0121_plug`) bound + decoded genOnOff, haElectricalMeasurement and
  seMetering but had `reports=nullptr`, so no Configure Reporting was set
  up. z2m's profile for both is
  `tuya.modernExtend.tuyaOnOff({electricalMeasurements: true})` +
  `electricityMeasurementPoll`, exposing state + voltage + current + power
  + energy. Added the matching `.reports` on EP1: onOff (genOnOff 0x0006 /
  0x0000, bool), rmsVoltage (haElectricalMeasurement 0x0B04 / 0x0505, u16),
  rmsCurrent (0x0B04 / 0x0508, u16), activePower (0x0B04 / 0x050B, s16) and
  currentSummationDelivered (seMetering 0x0702 / 0x0000, u48). Values mirror
  z2m's `lib/reporting.ts` exactly — `onOff` (min=0, max=3600, change=0);
  `rmsVoltage`/`rmsCurrent`/`activePower` (min=5, max=3600, change=1);
  `currentSummDelivered` (min=5, max=3600, change=257). Both defs' bindings
  were extended from genOnOff-only to also bind haElectricalMeasurement
  (0x0B04) and seMetering (0x0702) on EP1, since `run_configure` walks
  `.bindings[]`/`.reports[]` independently — a report on an unbound cluster
  has no route to the coordinator. Shared array `kReportsPlugVIPE_1ep`
  lives in `definitions/tuya/_shared.cpp` (decl in `_shared.hpp`). Covered
  by `tests/test_tuya_plug_reports.cpp`.

- Tuya switch family genOnOff attribute reporting (Phase-2b Batch 1).
  The nine generic Tuya switch parents — `TS0001`, `TS0002`, `TS0003`,
  `TS0004`, `TS0011`, `TS0012`, `TS0013`, `TS0014`, `TS000F` — already
  bound genOnOff via `kAutoBindings` but had `reports=nullptr`, so no
  Configure Reporting was set up. z2m's `configure:` for each calls
  `reporting.onOff(endpoint)` on every gang's endpoint. Added the
  matching `.reports` per gang count: 1-gang (`TS0001`/`TS0011`/`TS000F`)
  → EP1; 2-gang (`TS0002`/`TS0012`) → EP1+EP2; 3-gang (`TS0003`/`TS0013`)
  → EP1..3; 4-gang (`TS0004`/`TS0014`) → EP1..4. Gang counts verified
  against z2m `tuya.ts` endpoint maps, not the model number. Values
  mirror z2m's `reporting.onOff` exactly (lib/reporting.ts:
  `payload("onOff", 0, repInterval.HOUR, 0)` → min=0, max=3600,
  reportableChange=0) on genOnOff cluster 0x0006 / onOff attr 0x0000
  (bool, type 0x10). Shared arrays `kReportsOnOff_1ep..4ep` live in
  `definitions/tuya/_shared.cpp` (decls in `_shared.hpp`); `.bindings`
  were left untouched. Covered by `tests/test_tuya_switch_reports.cpp`.

- Tuya multi-gang switch genOnOff bindings on every gang endpoint
  (Phase-2b Batch 1b). Batch 1 added `.reports` on EP1..N of the multi-gang
  switch defs, but those defs bound only EP1
  (`kAutoBindings = {{1, 0x0006}}`). The firmware's `run_configure` walks
  `.bindings[]` and `.reports[]` as **independent** loops (no implicit bind
  on a report's endpoint), so the EP2..N attribute reports had no route to
  the coordinator and were undeliverable. Extended `kAutoBindings` to bind
  genOnOff (`0x0006`) on every gang endpoint, matching z2m which binds each
  gang endpoint: 2-gang (`TS0002`/`TS0012`) → EP1+EP2; 3-gang
  (`TS0003`/`TS0013`) → EP1..3; 4-gang (`TS0004`/`TS0014`) → EP1..4. The
  single-gang defs (`TS0001`/`TS0011`/`TS000F`) were already correct and
  are untouched. `tests/test_tuya_switch_reports.cpp` now also asserts an
  onOff binding on every gang endpoint EP1..N (and none beyond), keeping
  bindings and reports in lockstep so the gap can't regress.

- Tuya TS0502B CCT light (z2m `TS0502B`, incl. MiBoxer `_TZB210_lmqquxus`)
  configure pipeline. The parent def `definitions/tuya/TS0502B.cpp` had
  `config_steps=nullptr`; z2m's `TS0502B` runs a `configure:` (tuyaLight
  `configureReporting: true` + `tuya.configureMagicPacket`). Ported to
  `config_steps` on both registrations (`kDefTS0502B` + `kDefTS0502B_v2`):
  a settle `Wait`, a genBasic (0x0000) magic-packet `Read`, and reporting
  `Read`s on genOnOff (0x0006), genLevelCtrl (0x0008) and lightingColorCtrl
  (0x0300). Binds were already declared in `bindings[]`. The z2m
  `saveClusterAttributeKeyValue(colorCapabilities)` is a host-cache write
  with no over-air traffic and has no step equivalent. Covered by
  `tests/test_ts0502b_config.cpp`.

- HOCH DIN-rail RCBO (z2m `TS0601_rcbo`, `_TZE200_hkdl5fmv`) write path.
  The parent def `definitions/tuya/hoch_din.cpp` had `to_zigbee=nullptr`;
  z2m uses `legacy.toZigbee.hoch_din`. Wired the four round-trippable
  scalar DPs through the generic Tuya-DP writer (`tz_tuya_datapoints`,
  manuSpecificTuya `0xEF00` cmd setData): `state` (DP1 bool), `child_lock`
  (DP29 bool), `countdown_timer` (DP9 value) and `power_on_behavior`
  (DP27 enum off/on/previous). z2m's momentary command DPs `trip` (DP116)
  and `clear_device_data` (DP115) are intentionally omitted (no expose
  state to round-trip). Covered by `tests/test_hoch_din_tz.cpp`.

- Tuya TS004F smart knob (z2m `ERS-10TZBVK-AA`) full expose parity. New
  manu-specific override `definitions/tuya/TS004F_knob.cpp`
  (`kDef_TS004F_knob`) graduates the port out of the now-deleted generated
  hand-port `generated/Gen__TZ3000_abrsvsou.cpp` (edits there are
  hook-blocked + sweep-overwritten). Adds `hue_move`/`hue_stop`
  (+ `action_rate`) via `kFzCommandMoveHue`, and a read+write
  `operation_mode` (command/event) expose via `kFzTuyaOperationMode` +
  `kTzTuyaOperationMode` (genOnOff attr `0x8004`). `saturation_move` is
  declared-only (z2m superset, no producer for this device). Covered by
  `tests/test_ts004f_knob.cpp`. See
  `extra/docs/TS004F_KNOB_PARITY_TICKET.md`.

### Tests

- Host suite restored to green (48/48). Dropped 9 smoke-table rows across
  `tests/test_wave_{a,b,c,d,e}.cpp` that referenced lumi defs which are
  never generated (no such z2m model): GZCGQ02LM, JYGZ02AQ, RTCGQ23LM,
  WXKG19LM, QBKG10LM, ZNLDP15LM, ZNLDP29LM, QBCZ12LM, WRDZG01LM. Each drop
  is annotated; the dangling `extern`s were a link error that blocked the
  whole host build.
- `test_generic` battery expectations corrected to the converted contract
  (voltage mV = raw×100, battery % = raw÷2) to match `fz_battery`,
  `test_foundation`, and `test_wxkg01lm`; the impl was always correct — the
  test asserted raw attribute ints. Case renamed to
  `test_battery_emits_voltage_mv_and_percent`.
- `test_wxkg01lm` press/release action state-machine cases parked
  (commented, defs kept `[[maybe_unused]]`) pending the v2 action FSM —
  see `extra/docs/WXKG_ACTION_FSM_TICKET.md`. Click-count action mapping
  and battery/white-label cases remain active.

### Fixed (Critical)

- **C-1** Static 3-4 slot ring buffers in `kTuyaDpFlagScheduleDay`
  decode, `fz_tint_scene`, and `fz_ias_ace_arm` replaced with
  `RuntimeContext::alloc_str` (per-dispatch arena). Burst frames within
  one dispatch no longer overwrite earlier `StringRef` slots.
  (`definitions/tuya/_shared.cpp`, `definitions/_generic/_shared.cpp`,
  `include/zhc/runtime/dispatch.hpp`)
- **C-2** `parse_report_attributes` and `parse_read_attr_response` no
  longer drop already-decoded attributes when the unknown-key scratch
  buffer is exhausted; the affected unknown attribute is skipped while
  the rest of the multi-attribute report still surfaces.
  (`src/zcl/foundation.cpp`)

### Fixed (High)

- **H-1** `run_configure` only invokes the legacy `def.configure` hook
  when the declarative phases all succeeded; transition-period devices
  no longer double-configure on transport failure.
  (`src/runtime/dispatch.cpp`)
- **H-2** `fz_tuya_action` retransmit dedup moved off the file-scope
  static LRU onto `DeviceRuntimeState`, eliminating cross-test state
  leakage and cross-device interference. The test-only
  `reset_action_dedup_cache_for_test` is kept as a no-op for source
  compatibility. (`definitions/tuya/_shared.cpp`,
  `include/zhc/runtime/store.hpp`)
- **H-3** `parse_tuya_dp_stream` returns success with a truncated
  `count` on capacity overflow instead of discarding every DP decoded
  before the overflow. (`src/zcl/foundation.cpp`)
- **H-4** `tz_thermostat` setpoint unit contract documented in both
  the implementation comment and the header, calling out the
  intentional asymmetry with `kTzMinHeatSetpointLimit` & co. (which
  take pre-scaled deci-°C). (`definitions/_generic/_shared.cpp`,
  `definitions/_generic/_shared.hpp`)
- **H-5** ZCL long char string (type `0x44`) now decoded as
  `BytesRef` rather than `StringRef`; the raw radio buffer is
  length-prefixed and not NUL-terminated, so a `StringRef` would tempt
  consumers into reading past the end. (`src/zcl/foundation.cpp`)

### Fixed (Medium)

- **M-1** `static_assert` on `ZHC_FIXED_PAYLOAD_CAP` strengthened to
  `< 255` so `uint8_t` loop iterators can't wrap. (`include/zhc/types.hpp`)
- **M-2** `find_definition` Pass 2b now enforces manufacturer match
  before returning a Tuya-styled candidate with a non-TS0601 zigbeeModel,
  preventing collisions between two defs sharing the same model with
  different manufacturers. (`src/runtime/definition_runtime.cpp`)
- **M-3** `decode_enum_attr` formatter rewritten to use `std::snprintf`,
  matching the shape of `foundation.cpp::format_decimal_key` so both
  sites stay in sync. (`definitions/tuya/_shared.cpp`)
- **M-4** `zhc_core` `-Werror` wrapped in `$<BUILD_INTERFACE:…>` so
  downstream consumers (firmware, host tooling) don't inherit it.
  (`CMakeLists.txt`)
- **M-5** Mutual-exclusion between `fz_tuya_switch_scene` and
  `fz_tuya_action` (both match cmd `0xFD` on `genOnOff`) documented in
  both converter headers. (`definitions/tuya/_shared.cpp`)
- **M-6** `lint_no_new_legacy_tuya_action.py` normalises path
  separators after `os.path.relpath` so the SHARED_EXEMPT / BASELINE
  lookups succeed on Windows CI. (`tools/lint_no_new_legacy_tuya_action.py`)
- **M-7** `ConfigStep::wait_ms` documented as Wait-op only; the
  dispatcher now force-zeros the field locally for Read/Cmd/Callback
  before invoking platform hooks. WXKG01LM's stale `wait_ms = 1500` on
  its Read step zeroed. (`include/zhc/runtime/definition.hpp`,
  `src/runtime/dispatch.cpp`, `definitions/lumi/WXKG01LM.cpp`)
- **M-8** `TuyaOnOff` / `TuyaLight` factories now carry a
  `static constexpr DeviceKind kind` so the previously-invisible
  light-vs-switch distinction is observable at runtime without
  template-type inspection. (`definitions/tuya/factories.hpp`)

### Device support

- **`_TZE200_dwcarsat` / `_TZE204_dwcarsat`** "Smart Air Housekeeper"
  air-quality monitor ported (temperature, humidity, voc, co2, pm25). The
  generator had emitted an empty placeholder because z2m decodes this device
  via `legacy.fromZigbee.tuya_air_quality` (no declarative
  `meta.tuyaDatapoints`), so every cluster-0xEF00 DP report logged
  `(no match)` and the device showed no states. The legacy converter remaps
  two DPs for this variant — DP22→co2 (not formaldehyde), DP2→pm25 (not co2)
  — captured in the hand-port. Also removed the prior `TZE200_car_pres.cpp`,
  which had mislabeled `_TZE200_dwcarsat` as a "car-presence detector".
  (`definitions/tuya/TS0601_air_house_keeper.cpp`,
  `definitions/tuya/registry.cpp`, `tests/test_dwcarsat.cpp`; generator
  follow-up in `extra/docs/ZHAC_TOOLS_TICKET_legacy_converter_stubs.md`)
- **`_TZE200_mja3fuja`** "Smart Air Box" air-quality monitor ported
  (temperature, humidity, voc, co2, formaldehyde). Same `tuya_air_quality`
  legacy-converter family as dwcarsat, but its per-manufacturer branch maps
  DP22→co2 and DP2→formaldehyde (where dwcarsat maps DP2→pm25). Verified
  against captured device frames. (`definitions/tuya/TZE200_mja3fuja.cpp`,
  `definitions/tuya/registry.cpp`, `tests/test_dwcarsat.cpp`)
- **Tuya TS0601 air-quality sensors** (default `tuya_air_quality` branch)
  ported as one shared def covering 9 fingerprints —
  `_TZE200/_TZE204_3ejwxpmu`, `_TZE200/_TZE204_ogkdpgy2`, `_TZE200_blfcpsxz`,
  `_TZE200/_TZE204_c2fmom5z`, `_TZE200/_TZE204_yvx5lh6k` (z2m models
  TS0601_air_quality_sensor / _co2_sensor / _temperature_humidity_co2_sensor).
  Default mapping: DP2→co2, DP22→formaldehyde, DP21→voc, DP18 temp/10,
  DP19 hum/10. Replaces generator placeholders. `_TZE200_ryfmq5rl` /
  `_TZE200_8ygsuhe1` deliberately excluded — already ported as radar/presence,
  pending a separate mislabel review. (`definitions/tuya/TS0601_air_quality.cpp`,
  `definitions/tuya/registry.cpp`, `tests/test_dwcarsat.cpp`)
- **Tuya alarms (best-effort, no hardware frames):** `_TZE200_ux5v4dbd`
  photoelectric smoke detector (TS0601_smoke_3) and the `ZG-225Z` gas sensor
  (`_TZE200_aj0oxo1i` / `_TZE200_p6fuhvez`, zigbeeModel TS0225). Alarm bit uses
  the inverted-bool path (z2m `trueFalse0`; new `dp::binary_inv` helper +
  `kTuyaDpFlagInvertBool`) so wire 0 = alarm. Derived from z2m converters, not
  hardware-verified. (`definitions/tuya/TS0601_smoke_3.cpp`,
  `definitions/tuya/ZG_225Z.cpp`, `definitions/tuya/dp.hpp`,
  `tests/test_tuya_alarms.cpp`)
- **Tuya TS0601 DP switches (best-effort, states-only):** ported
  `TS0601_switch_1_gang` (3 mfgs), `_2_gang` (6), and multi-gang `TS0601_switch`
  (2) from the legacy `tuya_switch` converter — DP n → `state_l n` (bool).
  Decode via the tested `fz_tuya_datapoints` path. **On/off control (tz) is not
  yet wired** for DP switches — tracked as a cross-cutting follow-up in
  `extra/docs/STUB_COVERAGE_PLAN.md`. (`definitions/tuya/TS0601_switch_1g.cpp`,
  `TS0601_switch_2g.cpp`, `TS0601_switch_4g.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya TS0601 temp/humidity sensors:** ported the legacy
  `tuya_temperature_humidity_sensor` family — `_TZE200_44af8vyi`,
  `_TZE200/_TZE204/_TZE284_qyflbnbj` (humidity raw) and `_TZE200_zl1kmjqx`
  (humidity /10). DP1 temp/10, DP2 humidity, DP3 battery_level
  {low,middle,high}, DP4 battery %. `battery_low` (z2m-derived) omitted.
  Best-effort, not hardware-verified. (`definitions/tuya/TS0601_th_sensor_1.cpp`,
  `TS0601_th_sensor_zl.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya TS0601 illuminance + CO:** `XFY-CGQ-ZIGB` illuminance sensor
  (`_TZE200/_TZE204_khx7nnka`, `_TZE200_yi4jtqq1`; DP1 brightness_state
  {low,middle,high,strong}, DP2 illuminance) and `DCR-CO` carbon-monoxide alarm
  (`_TZE200/_TZE204_7bztmfm1`; DP1 carbon_monoxide inverted, DP2 co/100), from
  legacy `tuya_illuminance_sensor` / `tuya_CO`. CO invert pinned in
  test_tuya_alarms.cpp. Best-effort, not hardware-verified.
  (`definitions/tuya/TS0601_illuminance.cpp`, `definitions/tuya/DCR_CO.cpp`,
  `tests/test_tuya_alarms.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya TS0601 misc sensors:** water-leak `_TZE200_qq9mpfhw` (DP101), NOUS LCD
  temp/humidity `_TZE200_whkgqxse` (DP1 temp/10, DP2 hum, DP4 battery; settable
  config/alarm DPs omitted), and illuminance+temp+humidity `_TZE200_vzqtvljm`
  (DP8 temp/10, DP9 hum, DP3 battery, DP7 illuminance). From legacy
  `tuya_water_leak` / `nous_lcd_*` / `tuya_illuminance_temperature_humidity_sensor`.
  Best-effort, not hardware-verified. (`definitions/tuya/TS0601_water_sensor.cpp`,
  `TS0601_nous_lcd_th.cpp`, `TS0601_ith_sensor.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya TS0601 presence + contact:** human-presence radar `hpsz`
  (`_TZE200_0u3bj3rc`/`mx6u6l4y`/`v6ossqfy`; DP1 presence, DP101/102
  attendance/absence duration, DP103 led_state) and `TM001-ZA/TM081` door/window
  contact (`_TZE200_fwoorn8y`/`kltffuzl`; DP1 contact inverted, DP2 battery).
  Best-effort, not hardware-verified. (`definitions/tuya/TS0601_hpsz.cpp`,
  `definitions/tuya/TM081.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya TS0601 vibration + DIN-rail meter:** vibration sensor `_TZE200_kzm5w4iz`
  (DP1 contact inverted, DP3 battery, DP10 vibration) and DDS238-2 DIN energy
  meter `_TZE200_byzdayie`/`ewxhg6o9` (DP1 state, DP17 energy/100, DP18
  current/1000, DP19 power/10, DP20 voltage/10; relay control deferred). From
  legacy `tuya_smart_vibration_sensor` / `tuya_dinrail_switch`. `hoch_din` meter
  deferred (packed multi-byte buffer decode). Best-effort, not hardware-verified.
  (`definitions/tuya/TS0601_vibration.cpp`, `definitions/tuya/TS0601_din.cpp`,
  `definitions/tuya/registry.cpp`)
- **Tuya TS0601 mmWave presence radar:** ported the `tuya_smart_human_presense_sensor`
  (tshps) family — 11 mfgs (`_TZE200_holel4dk`/`jva8ink8`/`lyetpprm`/`sgpeacqp`/
  `wukb7rhc`/`xpq2rzhq`, `_TZE204_fwondbzy`/`xpq2rzhq`/`xsm7l9xa`/`ztc6ggyl`,
  `_TZE284_fwondbzy`) → one def. DP1 presence, DP2 sensitivity, DP3/4 min/max
  range /100, DP9 target_distance /100, DP101/102 detection_delay/fading_time /10,
  DP104 illuminance. self_test/cli skipped; config DPs read-only. Best-effort,
  not hardware-verified. (`definitions/tuya/TS0601_tshps_radar.cpp`,
  `definitions/tuya/registry.cpp`)
- **More Tuya TS0601:** ZG-205Z/A 5.8/24GHz presence radar `_TZE200_crq3r3la`
  (declarative — presence, target_distance, illuminance, multi-zone
  sensitivities/distances; motion_state enum omitted) and 3-gang switch
  `TS0601_switch_3_gang` (`_TZE200_2hf7x9n3`/`atpwqgml`/`bynnczcb`/`kyfqmmyl`,
  `_TZE204_atpwqgml`; states-only, control deferred). Best-effort, not
  hardware-verified. (`definitions/tuya/TS0601_zg205z.cpp`,
  `definitions/tuya/TS0601_switch_3g.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya TS0601 thermostats/TRVs (states-only):** ported the legacy
  `tuya_thermostat` family — 17 mfgs → one def (shared DP layout, no per-mfg
  branching). Readable subset: local_temperature, current_heating_setpoint
  (both /10), child_lock, valve_detection, battery, calibration, min/max/
  comfort/eco temperatures, boost_time, battery_low, window_open. Setpoint/mode
  **control + schedules deferred** (→ task #83). Best-effort, not hardware-
  verified. (`definitions/tuya/TS0601_thermostat.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya DP device control (write path):** added `factory::TuyaRw` (fz +
  `tz_tuya_datapoints` on one map) and wired on/off / setpoint control for the
  ported DP switches (1/2/3/4-gang), the DIN relay, and thermostat
  `current_heating_setpoint` + `child_lock` (exposed `StateSet`). Write encode
  is host-tested (`tests/test_tuya_alarms.cpp`). **On-device control is
  UNVERIFIED without hardware** — the DIN unit drives a mains breaker, the
  thermostat drives heating; validate before relying on remote control. Covers
  (position inversion), garage, PIR (no DP map) and scene-cube (custom
  converter) remain deferred (task #83). (`definitions/tuya/factories.hpp`,
  `TS0601_switch_1g/2g/3g/4g.cpp`, `TS0601_din.cpp`, `TS0601_thermostat.cpp`)
- **Tuya TS0601 covers (#83):** curtain/roller motors from legacy `tuya_cover` —
  non-inverted (8 mfgs, `TS0601_cover.cpp`): state {open,stop,close} + position
  0-100, read + set via `TuyaRw`; inverted-position variants (3 mfgs listed in
  z2m `coverPositionInvert`, `TS0601_cover_inv.cpp`): open/stop/close only
  (position omitted — needs a percent-invert converter feature). On-device
  control unverified. (`definitions/tuya/registry.cpp`)
- **Tuya TS0601 garage openers (#83):** `TS0601_garage_door_opener`
  (nklqjk62 ×3 + jktmrpoj, modernExtend): trigger (open/close, **control**),
  garage_door_contact, countdown, run_time, open_alarm_time; Matsee variant
  (wfxuhoea ×2, legacy): garage_door_contact (inverted) read-only. *** SECURITY:
  garage trigger control is host-tested only, UNVERIFIED on hardware — validate
  + review remote exposure before enabling. *** (`definitions/tuya/TS0601_garage.cpp`,
  `TS0601_garage_matsee.cpp`, `definitions/tuya/registry.cpp`)
- **Tuya converter features (#83):** (1) cover position **inversion write**:
  `encode_numeric` now honours `kTuyaDpFlagInvertPosition` + new `dp::position_inv`
  helper, so the 3 inverted-position covers (`nogaemzt`/`xuzcvlku`/`r0jdjrvi`)
  get full read+set position (host-tested both directions). (2) custom
  `fz_hoch_din` byte-slice converter for the HOCH DIN meter `_TZE200_hkdl5fmv`
  (voltage/current/power packed buffers + state/energy/temperature, host-tested).
  PIR (`f1pvdgoh`/`me6wtiqs`/`ghynnvos`) and the scene-cube `convLocal` action
  remain unportable (no extractable z2m DP map). (`definitions/tuya/_shared.cpp`,
  `dp.hpp`, `TS0601_cover_inv.cpp`, `hoch_din.cpp`, `tests/test_tuya_alarms.cpp`)

### Added

- **Unmapped Tuya DP passthrough.** `dispatch_from_zigbee` now has a fallback:
  when a Tuya DP report matches no converter in the device's def (placeholder /
  un-ported / unknown device), the raw scalar datapoints are surfaced as
  `dp_<id>` keys (Bool/Int) instead of being dropped silently. Turns the old
  `(no match)` devices into diagnosable + rule-usable state — an installer sees
  `dp_1`, `dp_104`, … and can identify/port them. Fires ONLY on no-match, so
  fully-ported devices are unaffected. (`src/runtime/dispatch.cpp`,
  `tests/test_tuya_alarms.cpp`)
</content>
</invoke>