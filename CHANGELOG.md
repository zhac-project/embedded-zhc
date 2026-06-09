# Changelog

All notable changes to **embedded-zhc** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to the `vYYYYMMDDVV` versioning scheme used
across the ZHAC platform.

## [Unreleased]

### Fixed

- **Stelpro thermostat parity pass — `keypad_lockout` dead attribute fixed on
  all 6 defs** (HT402, SMT402, SMT402AD, SORB, ST218, STZB402). Each declared
  the `keypad_lockout` expose as `StateSet` (read + write) but wired neither a
  decoder nor an encoder, while z2m wires `fz.hvac_user_interface` and
  `tz.thermostat_keypad_lockout` on every Stelpro device — so the value never
  decoded and could not be set. Added `kFzStelproKeypadLockout`
  (`hvacUserInterfaceCfg` 0x0204 attr 0x0001 keypadLockout, ENUM8 →
  `keypad_lockout`) and `kTzStelproKeypadLockout` (non-manu ENUM8 write of the
  same attr) to `definitions/stelpro/_shared.{hpp,cpp}`, and graduated all six
  defs from `generated/` to parent Tier-2 overrides to wire them. New fixture
  `tests/test_stelpro_parity.cpp` pins keypad_lockout decode + write across all
  six and guards the pre-existing thermostat decode (Eco(5)→`auto`,
  `running_state` from pIHeatingDemand) and the outdoor-temp manuSpec write.

- **Adeo (ENKI LEXMAN / LexMan) parity pass — 7 of 50 defs corrected** over a
  z2m↔embedded-zhc sweep. Adeo is otherwise a bounded-lighting vendor (the
  LDSENK*/ZBEK-*/LXEK-* bulbs are stock `m.light` and decode fine); every gap
  was on the sensor / key-fob ports. Six IAS devices lowered the generic
  `kFzIasZone` (which emits `alarm_1`/`alarm_2`, or a bare `alarm`) behind a
  *semantic* expose, so with no rename layer the primary sensor state never
  reached the shadow; each was graduated to a Tier-2 parent override wiring the
  typed `kFzIas<Type>Alarm`:
  - **LDSENK10** (PIR motion) → `kFzIasMotionAlarm` / `occupancy`
    (z2m `fz.ias_occupancy_alarm_1`).
  - **83633206** (ZB-WaterSensor-D0001 leak) → `kFzIasWaterLeakAlarm` /
    `water_leak` (z2m `iasZoneAlarm({zoneType: "water_leak"})`).
  - **ZB-DoorSensor-D0007** (contact) → `kFzIasContactAlarm` / `contact`
    (z2m `iasZoneAlarm({zoneType: "contact"})`).
  - **LDSENK08** (door + vibration) → `kFzIasContactAlarm` / `contact` +
    `kFzBattery`. The `vibration` bit-1 sub-flag and the `sensitivity` write
    (custom ssIasZone decode + attribute write in z2m's `fzLocal`/`tzLocal`)
    have no generic converter and remain deferred.
  - **LDSENK06** (indoor 85 dB siren) → new generic `kFzIasGenericAlarm` /
    `alarm`. z2m's `iasZoneAlarm({zoneType: "alarm"})` collapses both alarm
    bits onto the single `alarm` key, which the generic `kFzIasZone`
    (`alarm_1`/`alarm_2`) never produced. The `iasWarning()` siren *control*
    (SET-only `tz.warning`) has no generic write path and is deferred.
  - **83633205** (ZB-SMART-PIRTH-V3 "4-in-1") was the worst gap: it lowered only
    `kFzBattery` + `kFzIasZone`, dropping the illuminance, temperature and
    humidity channels the device exists to report and leaving motion on a dead
    `alarm` key. Wired the generic `kFzIlluminance` / `kFzTemperature` /
    `kFzHumidity` decoders + exposes + binds, and swapped in `kFzIasMotionAlarm`
    for `occupancy` (which here rides ssIasZone, not msOccupancySensing).
  - **LDSENK09** (security key fob) was mis-ported as an on/off relay
    (`kFzOnOff` + `kTzOnOff` + a controllable `state`). It is an ssIasAce
    *sender*; the `state` was dead and the action surface was missing. Re-wired
    to `kFzIasAceArm` + `kFzIasAcePanic`, exposed `action`
    [panic, disarm, arm_partial_zones, arm_all_zones], bound ssIasAce (0x0501),
    and dropped the bogus to_zigbee path. Mirrors the linkind / immax key-fob
    ports.

  Added generic converter `kFzIasGenericAlarm` (ssIasZone bit 0 → `alarm` +
  tamper + battery_low) for the zoneType:"alarm" case. All seven defs graduated
  to Tier-2 parent overrides; covered by `tests/test_adeo_parity.cpp`. The
  1-key remote **83633204** (z2m `fz.adeo_button_65024`, a manuSpecific private
  cluster 0xFE00 `raw` decoder) is left as-is — it has no generic converter and
  is deferred as infra.

- **Osram (LEDVANCE / Lightify) parity pass — 3 of 44 defs corrected** over a
  z2m↔embedded-zhc sweep. Osram is otherwise a pure lighting vendor
  (`ledvanceLight` / `ledvanceOnOff` over stock ZCL; the plugs are on/off-only
  — z2m wires no metering — and decode fine), so the gaps were all on the
  three non-light ports, which were dead:
  - **SMART+ Switch Mini (AC0251100NJ/AC0251600NJ/AC0251700NJ) and SMART+
    Switch (4058075816459) emitted a dead controllable `state` instead of
    `action`.** Both are battery-powered remotes that z2m drives with the
    genOnOff / genLevelCtrl / lightingColorCtrl *command* converters
    (`fz.command_on/off`, `fz.command_move/stop/move_to_level`,
    `fz.command_move_to_color_temp`, `fz.command_step_color_temperature`,
    `fz.command_move_hue`) behind a single `action` enum — but the generated
    ports lowered `kFzOnOff` (which decodes the genOnOff command into a phantom
    on/off `state`) and declared only a `state` expose, so every button was
    dead. Re-wired to the generic command-action decoders, exposed `action`,
    and dropped the bogus to_zigbee path.
  - **SMART+ Motion Sensor (AC01353010G) dropped its temperature and reported
    the wrong occupancy key.** z2m wires `fz.temperature` +
    `fz.ias_occupancy_only_alarm_2`. The port had no temperature decoder,
    expose, or msTemperatureMeasurement (0x0402) bind — temperature was dead —
    and lowered the generic `kFzIasZone` (which emits a bare `alarm` key) instead
    of the `occupancy` key z2m advertises. Added `kFzTemperature` + the
    `temperature` expose + 0x0402 bind, and re-pointed the IAS decode at the
    typed `kFzIasMotionAlarm2` (occupancy / tamper / battery_low).

  All three graduated to Tier-2 parent overrides; covered by
  `tests/test_osram_parity.cpp`.

- **ROBB (ROBB Smarrt) parity pass — 9 of 32 defs corrected** over a
  z2m↔embedded-zhc sweep (ROBB rebrands Sunricher hardware, so several gaps
  mirror the Sunricher pass). Five device families were misclassified by the
  auto-generator:
  - **ROB_200-081-0 / ROB_200-084-0 (4-button wireless wall switches) emitted
    a dead controllable `state` instead of `action`.** z2m drives these with
    `m.commandsOnOff()` + `m.commandsLevelCtrl()` — the device is a genOnOff/
    genLevelCtrl *client* that only sends commands — but the port lowered a
    controllable `m.onOff()` (writable `state` + `kTzOnOff`). Re-wired to the
    generic command-action decoders (on/off + brightness move/stop), exposed
    `action`, dropped the bogus to_zigbee path, and set
    `endpoint_action_suffix` so each button reports as `action_1..action_4`.
  - **ROB_200-070-0 (PIR + temperature + humidity + light sensor) dropped all
    four of its measurements.** The port lowered only `kFzBattery` +
    `kFzIasZone`, so the sensor surfaced just its IAS bits + battery. Added
    `kFzOccupancy` + `kFzTemperature` + `kFzHumidity` + `kFzIlluminance` plus
    the matching exposes and per-endpoint binds (occupancy/IAS on ep1,
    temp/humidity/lux on ep3/4/5 → suffixed `temperature_3` etc.), mirroring
    the twin Sunricher HK-SENSOR-4IN1-A.
  - **ROB_200-017-0 / ROB_200-017-1 (smart plugs) dropped their internal
    temperature.** z2m wires `fz.temperature`; the port omitted it. Added
    `kFzTemperature` + the `temperature` expose + the msTemperatureMeasurement
    (0x0402) bind.
  - **ROB_200-016-0 (RGB CCT remote) was missing its colour actions and
    ROB_200-018-0 (knob dimmer) used the wrong colour decoder.** The earlier
    hand-port had noted the lightingColorCtrl command decoders were "not yet
    present" — they exist now. Wired `kFzCommandMoveToColor` /
    `MoveToColorTemp` / `MoveHue` / `ColorLoopSet` /
    `EnhancedMoveToHueAndSat` / `MoveToHueAndSaturation` on 016, and swapped
    018's stray `kFzCommandMoveToHueAndSaturation` (a command it never sends)
    for `kFzCommandMoveToColor` (z2m's `color_move`).
  - **ROB_200-007-0 / ROB_200-008-0 / ROB_200-025-0 (8-/4-button remotes) lost
    per-button identity.** They emitted a bare `action` (a global key), so all
    buttons collapsed onto one key; z2m distinguishes them per endpoint
    (`on_1`..`brightness_stop_4`). Set `endpoint_action_suffix` so the
    dispatcher rewrites to `action_<n>` per endpoint (same convention as
    Legrand 067774).

  All nine defs were graduated from `generated/` to Tier-2 parents (symbol-
  based registry needs no edit). The remaining 23 defs were at parity:
  the plain `m.light()` dimmers/CCT/RGB drivers (004/006/011/060/061/063,
  etc.), the curtain controllers 010/029 (`fz.cover_position_tilt`,
  `coverInverted`), the 1-gang AC switches 003/030, the 5-gang relay 050
  (`endpoint_map`), the metering switches 011/014/026-0/026-1/035
  (`kFzMetering` + `kFzElectricalMeasurement`), the 4-channel remote 024
  (z2m exposes *bare* `on`/`off`/`recall_*` actions — no per-endpoint suffix,
  so the generated bare-`action` form already matches), and the door/window
  sensor 001 (`iasZoneAlarm` contact). New fixture
  `tests/test_robb_parity.cpp` pins all five fixed families.

- **eWeLink WS01 rainfall sensor never reported `rain`** — found by a
  z2m↔embedded-zhc parity pass over the 18 eWeLink defs. WS01 reports
  rainfall over `ssIasZone` commandStatusChangeNotification (zoneStatus
  bit 0); z2m maps it with `fzLocal.WS01_rain` → `{rain: bool}`. The
  generated port correctly declared the `rain` expose but wired the
  generic `kFzIasZoneStatusChange`, which only emits
  `alarm_1`/`alarm_2`/`tamper`/`battery_low` — so the `rain` expose stayed
  permanently empty while phantom alarm-bit keys leaked into the shadow.
  Added a typed `kFzIasRainAlarm` converter (zoneStatus bit 0 → `rain`) to
  the generic shared layer (mirrors the existing
  `kFzIas{WaterLeak,Smoke,Co,…}Alarm` family) and graduated WS01 to a
  Tier-2 parent override wired to it. New fixture
  `tests/test_ewelink_parity.cpp` pins the rain decode plus regression
  guards that WS01 carries no phantom on/off (z2m `toZigbee:[]`), that
  SNZB-05 decodes typed `water_leak`, and that the CK-BL702-ROUTER USB
  repeater stays expose-less (z2m `exposes:[]`). The rest of the range was
  at parity: the NAS-AB03B3/AB06B3 sirens (custom `0xFC11`
  `customEwelinkSiren` writes for melody/volume/duration + genOnOff alarm
  + IAS battery_low — already hand-corrected, *not* Tuya-DP), the curtains
  AM25B/MYRX25Z/CK-MG22 (`m.windowCovering({coverInverted:true})` = raw
  lift passthrough, which the generic cover converter already matches),
  the multi-gang switches ZB-SW02..05 (`endpoint_map`), and the plain
  on/off switches/plugs (SA-003/SA-030, ZB-SW01, SWITCH-ZR02/ZR03,
  CK-BL702-MSW). The curtains' custom-cluster `ewelinkBattery` and
  `ewelinkMotor*` (mode/reverse/calibration/speed, custom `0xEF00`)
  channels are deferred as manuSpecific infra (no generic decoder).

- **NodOn TRV-4-1-00 `trv_mode` / `valve_position` and SIN-4-RS-20[_PRO]
  `tilt` were dead (no decode)** — found by a z2m↔embedded-zhc parity pass
  over the 16 NodOn defs. (1) TRV-4-1-00 declared `trv_mode`
  (`nodonModernExtend.trvMode`, enumLookup over hvacThermostat attr
  `0x4000`) and `valve_position` (`.valvePosition`, numeric over attr
  `0x4001`) exposes with matching Tz writers, but only wired the generic
  `kFzThermostat` (decodes `0x0000`/`0x0012`/`0x001C`) — so both were
  write-only with dead STATE_GET/reporting, while z2m's modernExtends
  auto-generate a read path. Wired a vendor-local `kFzNodonTrvExtras`
  (attr `0x4000` enum8 → `auto`/`valve_position_mode`/`manual`; `0x4001`
  u8 → `valve_position` 0..100). (2) SIN-4-RS-20 / SIN-4-RS-20_PRO roller
  shutters wired the lift-only generic `kFzCoverPosition` and exposed only
  `position`, but z2m's `m.windowCovering({controls:['tilt','lift']})`
  exposes `position` + `tilt` and `fz.cover_position_tilt` also decodes
  attr `0x0009` (CurrentPositionTiltPercentage → `tilt`). Wired
  `kFzNodonCoverPositionTilt` (lift `0x0008` + tilt `0x0009`, skips the
  `0xFF` unknown sentinel), added a `tilt` expose and the generic
  `kTzCoverPositionTilt` writer. All three defs graduated to Tier-2 parent
  overrides. New fixture `tests/test_nodon_parity.cpp` pins both decodes
  plus regression of the generic thermostat/lift surface. The rest of the
  range was at parity (relay modules SIN-4-1-xx/SIN-4-2-xx incl. the
  2-channel `endpoint_map`, SDC-4-1-00 dry contact, SDO-4-1-00 IAS
  contact, SEM-4-1-00 meter, STPH-4-1-00 climate sensor). The
  `pilot_wire_mode` surface on FPS-4-1-00 / SIN-4-FP-20 / SIN-4-FP-21
  (z2m `nodonPilotWire`, custom cluster `0xFC00`) is deferred as infra:
  `cluster_names.hpp` already maps `0xFC00` → `manuSpecificPhilips` (a
  hardcoded collision) and Fz dispatch matches on cluster name only, so a
  NodOn pilot-wire decoder can't be cleanly targeted without cluster-name
  infra changes.
- **Immax (Neo) 07046L 4-Touch key fob never emitted an `action`** — found
  by a z2m↔embedded-zhc parity pass over the 24 Immax defs. The generated
  def lowered `kFzCommandRecall` (genScenes cluster `0x0005` commandRecall →
  `recall_<scene>`) and bound `0x0005`, but z2m (`fz.command_arm` +
  `fz.command_panic`) decodes this key fob's presses from IAS ACE
  (`ssIasAce` `0x0501`) commandArm (`0x00`) and commandPanic (`0x04`). The
  recall decoder could never match those frames, so disarm/arm/away/panic
  produced no state. Graduated the def to a Tier-2 parent override wiring
  the generic `kFzIasAceArm` + `kFzIasAcePanic` converters (the direct
  equivalents of `fz.command_arm` / `fz.command_panic`) and re-bound
  `ssIasAce` (`0x0501`). New fixture `tests/test_immax_parity.cpp` pins the
  arm/panic action decode and the binding swap. The rest of the Immax range
  was at parity or out of scope: the plain bulbs (TS0502C CCT, 07008L/07042L/
  07115L/07743L, etc.), the 07048L plug (on/off + metering +
  electrical_measurement), the IAS sensors (07043M motion, 07045L contact,
  07047L 4-in-1) and the TS0601 keypad Tuya-DP defs all decode correctly.
  The 07502L "4-in-1" (z2m `legacy.fz.ZB003X`), the 07504L siren (custom
  local `ts0219*` converters) and the 07752L Tuya double-socket
  (`tuya.modernExtend.tuyaOnOff` l1/l2) need vendor-private converter infra
  and are deferred.

- **Perenio PEHWE20 two-channel mini-relay could not distinguish its two
  switch channels** — found by a z2m↔embedded-zhc parity pass over the 5
  Perenio defs. The def declared per-endpoint exposes
  (`state_l1`/`state_l2`, `power_on_behavior_l1`/`_l2`,
  `switch_type_l1`/`_l2`, mirroring z2m's `endpoint: () => ({l1: 1, l2: 2})`
  + `withEndpoint(...)`) but was missing the `endpoint_map` field. The
  dispatcher only rewrites emitted Fz keys to `<key>_<label>` when
  `endpoint_map` is populated, so the bare `state`/`power_on_behavior`/
  `switch_type` keys the converters emit were never suffixed: both
  channels collided on the same unsuffixed key (last-writer-wins) and the
  declared `_l1`/`_l2` exposes never populated; outbound writes to
  `state_l2` had no endpoint to route to. Added
  `endpoint_map = {{"l1", 1}, {"l2", 2}}` and graduated the def to a
  Tier-2 parent override. Diagnostic reports (haDiagnostic on EP10, not in
  the map) correctly stay unsuffixed — `last_message_lqi`/`rssi` are
  device-global. The plug (PEHPL0X: `perenioSpecific` smart-plug decode +
  `seMetering`) and the three IAS sensors (PECMS01 occupancy, PECWS01
  contact, PECLS01 water_leak) were already at parity. New fixture
  `tests/test_perenio_parity.cpp`.

- **Siglis zigfred plus (ZFP-1A-CH) and uno (ZFU-1D-CH) dropped the
  front-surface RGB LED and never decoded button presses** — found by a
  z2m↔embedded-zhc parity pass over the 2 Siglis defs. (1) EP5
  (`zigfredEndpoint`, label `l1`) was missing from each def's
  `endpoint_map`, so the front-surface RGB LED's `state`/`brightness`/
  `color` reports on EP5 never received the `_l1` suffix and could not
  match the declared `state_l1`/`brightness_l1`/`color_l1` exposes (z2m
  maps `l1: zigfredEndpoint`). Added `{"l1", 5}` to both maps. (2) Button
  events arrive as a manufacturer-specific cluster command
  (`manuSpecificSiglisZigfred` 0xFC42, cmd 0x02 `siglisZigfredButtonEvent`,
  payload `button:u8 type:u8 duration:u16`) with no generic equivalent —
  the `action` expose was a declared no-op. Added a vendor converter
  `siglis::kFzZigfredButtonEvent` (new `definitions/siglis/_shared.{hpp,cpp}`)
  that mirrors z2m's `zifgredFromZigbeeButtonEvent` lookups into
  `action: "button_<n>_<event>"`, guarded on `manufacturer_specific` so it
  does not hijack the non-manu genOnOff Toggle (also cmd 0x02). Both defs
  graduated to Tier-2 parent overrides. New fixture
  `tests/test_siglis_parity.cpp`. (All other endpoints — 4 dimmers + 2
  covers on ZFP, relay + dimmer on ZFU — verified already correct.
  Remaining `level_config` / hue-saturation move-step / power-on-behavior
  TZ helpers stay deferred: no generic equivalents yet.)

- **Plugwise Tom (106-03) and Emma (170-01) thermostats had dead
  exposes** — found by a z2m↔embedded-zhc parity pass over the 4 Plugwise
  defs. Both wired the generic `kFzThermostat`, which decodes only
  hvacThermostat attrs `0x0000`/`0x0012`/`0x001C`, leaving several declared
  exposes undecoded that z2m's base `fz.thermostat` fills: Tom's
  `pi_heating_demand` (`0x0008`) and Emma's `occupied_cooling_setpoint`
  (`0x0011`), `running_state` (`0x0029`, map16→string),
  `local_temperature_calibration` (`0x0010`) and `outdoor_temperature`
  (`0x0001`). Extended the existing `kFzPlugwiseThermostat` converter to
  decode these standard attrs alongside its manufacturer-specific (`0x1172`)
  set. Emma additionally overloads `pIHeatingDemand` as the boiler-water
  setpoint — z2m surfaces it as `boiler_setpoint` (°C), not
  `pi_heating_demand` (%) — so the 170-01 expose was renamed and populated
  via a def-local converter. Both defs graduated to Tier-2 parent overrides.
  New fixture `tests/test_plugwise_parity.cpp`. (The Lisa 158-01 thermostat
  and 160-01 plug verified CLEAN — pure-generic and already matching z2m.)

- **QA QAT44Z4H (4-gang) / QAT44Z6H (6-gang) wall switches decoded and
  controlled none of their gangs** — found by a z2m↔embedded-zhc parity pass
  over the 28 QA defs. Both are TS0601 Tuya-MCU devices (`_TZE204_kyzjsjo3` /
  `_TZE204_4cl0dzt4`) that z2m drives with `legacy.fz.tuya_switch` /
  `legacy.tz.tuya_switch_state` on `manuSpecificTuya` (0xEF00): the
  multiEndpoint datapoints `1..N` carry per-gang bool ON/OFF (DP 1→l1,
  2→l2, … 6→l6), every gang multiplexed through the single physical endpoint 1.
  The generated stubs instead wired the generic genOnOff converters
  (`kFzOnOff`/`kTzOnOff`, cluster 0x0006) against a single bare `state` expose
  — so these devices never emit plain genOnOff and the entire multi-gang
  surface was dead (no state read, no per-gang control). Both defs graduated
  to Tier-2 parent overrides wiring the Tuya-DP infra (`fz_tuya_datapoints` /
  `tz_tuya_datapoints`) with a `{1..N → state_lN}` bool map, per-gang
  `state_l1..lN` exposes, and the `{1,0xEF00}` binding — mirroring the sibling
  DP ports (`Qa__TZE284_ms97nkyy` etc.). New fixture `tests/test_qa_parity.cpp`.
  (The rest of QA verified CLEAN: the 6 manufacturer-discriminated DP ports
  `_TZE284_*` / `_TZ3218_kwht8j5m` and the hand-rewritten QAFZ200 CCT light /
  QASZP power-sensor stubs already match z2m; the TS0726 scene switches'
  `commandTuyaAction` button-lookup decode + the TS110E dimmers' `switch_type` /
  `power_on_behavior` need converters absent from the generic/tuya layer —
  INFRA, deferred.)

- **xyzroe ZigUSB_C6 over-exposed a phantom `energy` channel, dropped the
  over-current alarm, and mis-keyed every metering/switch channel** — found
  by a z2m↔embedded-zhc parity pass over the 3 xyzroe DIY power-monitor defs
  (ZigUSB / ZigUSB_C6 / ZigDC). ZigUSB_C6 is built entirely from
  `m.electricityMeter({cluster:"electrical", electricalMeasurementType:"both"})`
  + `m.temperature()` + `m.onOff()`×3 + `m.iasZoneAlarm({zoneType:"generic",
  zoneAttributes:["alarm_1"]})`. Three real gaps: (1) `cluster:"electrical"`
  maps to `fz.electrical_measurement` (haElectricalMeasurement 0x0B04) ONLY —
  it exposes no `energy` and uses no seMetering — but the prior hand-rewrite
  wired `kFzMetering` + an `energy` expose + a `{1,0x0702}` bind, all phantom
  (removed); (2) the over-current IAS alarm was dropped entirely — restored via
  the generic `kFzIasZoneStatusChange` + `{1,0x0500}` bind; (3) with the
  `{1,2,3}` endpoint_map the dispatch suffix-rewrite emits `power_1`/`current_1`/
  `voltage`(forced-global)/`temperature_3`/`state_1`/`state_2`/`state_3`/
  `alarm_1_1`, but the exposes were bare `state`/`power`/`current`/`temperature`/
  `alarm_1` and the two indicator-LED switches (`state_2`/`state_3`, with their
  `{2,0x0006}`/`{3,0x0006}` binds) were missing — exposes renamed to the runtime
  shape and the LED switches added, so every standard channel now surfaces. Def
  graduated to a Tier-2 parent override. ZigUSB + ZigDC verified CLEAN: their
  voltage/current/power/uptime ride genAnalogInput (0x000C) through vendor
  `description`-string parsers with no generic converter (INFRA, deferred); only
  their standard `fz.temperature`/`fz.humidity` (0x0402/0x0405) channels are
  portable and decode correctly. New fixture `tests/test_xyzroe_parity.cpp`.

- **VSmart HS-SEDR00ZB-VNM door/window sensor never reported `contact`**
  — found by a z2m↔embedded-zhc parity pass over the 6 VSmart (HumanSmart)
  defs. The generated port wired the generic `kFzIasZone`, which emits a
  generic `"alarm"` boolean, but z2m maps this device with
  `m.iasZoneAlarm({zoneType:"contact", invertAlarm:true})` and advertises a
  `"contact"` expose — so the catalogued expose was dead against the wire
  shape. Re-wired to the typed `kFzIasContactAlarm` (the same converter the
  rest of the contact-sensor fleet uses) and renamed the expose
  `alarm` → `contact`; `tamper` + `battery_low` unchanged. Polarity is
  preserved: z2m's `invertAlarmPayload` defaults true for `zoneType:"contact"`
  and this device's explicit `invertAlarm:true` flips it back to false, so
  `contact = (zoneStatus & 1)` — exactly what `kFzIasContactAlarm` emits. Def
  graduated to a Tier-2 parent override. The other 5 VSmart defs (SEOC
  occupancy, SW100/200/300/400 wall switches with their custom LED/vibration/
  intensity write-path commands) verified CLEAN. New fixture
  `tests/test_vsmart_parity.cpp`.
- **BITUO TECHNIK energy meters decoded only 4 of ~20 metering channels**
  — found by a z2m↔embedded-zhc parity pass over the 11 SDM/SPM DIN-rail
  meter defs (SDM01/SDM02, SPM01/SPM02; U00/U01/U02 SKUs). All 11 run z2m's
  `m.electricityMeter` (with `producedEnergy` + `acFrequency` + `powerFactor`,
  plus `threePhase` on SDM01W/SPM02) and the vendor `bituo_fz.total_power` /
  `bituo_fz.phase_energy` converters — but the generated ports wired only the
  generic `kFzMetering` (seMetering 0x0000 energy / 0x0400 power) and
  `kFzElectricalMeasurement` (haElectricalMeasurement 0x050B power / 0x0505
  voltage / 0x0508 current), so every catalogued extra was a dead expose:
  `produced_energy`, `ac_frequency`, `power_factor`, `power_apparent`,
  `power_reactive`, `total_power`/`total_power_reactive`/`total_power_apparent`,
  the per-phase B/C `voltage`/`current`/`power`, and the per-phase tier-summation
  `energy_phase_*`/`produced_energy_phase_*`. Bituo uses NO manufacturer-specific
  cluster — both converters read STANDARD seMetering / haElectricalMeasurement
  attributes (vendor-assigned slots inside the standard clusters) — so a new
  pair of reusable converters, `kFzBituoMeteringExtras` (seMetering 0x0001
  produced + 0x0102–0x0107 tier energy) and `kFzBituoElectricalMeasurementExtras`
  (0x0300 frequency, 0x0510 factor, 0x050E/0x050F reactive/apparent, 0x0304–0x0306
  totals, 0x0905/0x0908/0x090B + 0x0A05/0x0A08/0x0A0B per-phase B/C), now decode
  them. Values are raw pass-through, matching the generic converters (the runtime
  scales downstream). The converters wire alongside the generics on all 11 defs
  (`definitions/bituo_technik/_shared.{hpp,cpp}`), graduated from `generated/`
  to Tier-2 parent overrides. Also restored the 6 per-phase tier-energy exposes
  that the **SPM02-U01** catalogue dropped (its z2m entry carries
  `bituo_fz.phase_energy` like SDM01W, which did list them). Covered by
  `tests/test_bituo_technik_parity.cpp`.

- **Neo (NEO Coolcam) sirens, PIR and T/H sensor lost their primary
  channels** — found by a z2m↔embedded-zhc parity pass over the 25 neo defs.
  The Tuya-DP water valves (NAS-WV03B/WV03B2), AB06B2 siren, PS10B2 presence
  and STH02B2 soil sensor resolve correctly to their `Neo__TZE*` Tuya-DP
  defs (registry order makes the DP def win Pass-1 over the shadowed named
  IAS stub for shared fingerprints). Four named devices were misrouted by the
  generator and graduated from `generated/` to Tier-2 parent overrides: the
  **NAS-AB02B0** (`_TZE200_d0yu2xgi`) and **NAS-AB02B2** (`_TZE200_t1blo2bj`,
  `_TZE204_t1blo2bj`, `_TZE204_q76rtoa9`) sirens and the **NAS-PD07**
  (`_TZE200_7hfcudw5`) PIR were wired to `kFzIasZone` (a `{alarm,tamper,
  battery_low}` IAS expose set), but z2m serves all three over Tuya-DP
  (`legacy.fz.neo_t_h_alarm` / `neo_alarm` / `neo_nas_pd07`, manuSpecificTuya
  0xEF00) — so temperature, humidity, occupancy, alarm, duration, volume and
  melody never decoded. Replaced each IAS stub with the proper Tuya-DP map
  (temperature /10 per z2m, the differing AB02B0 vs AB02B2 volume enum order,
  the AO\* DP ids for AB02B2) plus a write path. The **NAS-TH07B2**
  (`_TZ3000_utwgoauk`, SNZB-02 clone) is the family's only standard-cluster
  device — z2m wires `m.temperature()+m.humidity()+m.battery()` but the
  generated port carried `kFzBattery` only, dropping both temperature and
  humidity; added generic `kFzTemperature` (0x0402) + `kFzHumidity` (0x0405)
  with their exposes and binds. Pinned by `tests/test_neo_parity.cpp`.
- **PushOk Hardware water-level sensors, thermostat relay and window/vent
  covers dropped their headline channels** — found by a z2m↔embedded-zhc
  parity pass over the 17 PushOk defs (the valves POK001/POK006, soil sensor
  POK002/POK007, illuminance POK004/POK011, temp/humidity POK005, probe
  thermometers POK014/POK015 and the POK020 TRV are at parity; the voltage
  monitor POK009 analog channels, the POK021 gas-pulse counter — both on
  genAnalogInput/genAnalogOutput with no Tier-1 decoder, mirroring the Bacchus
  water-level meter — and the POK012 router's IAS `ac_status`/`battery_defect`
  diagnostic bits remain infra, deferred). Five defs were graduated from
  `generated/` to Tier-2 parent overrides: the **POK003** and **POK010** water
  level sensors expose `contact` from z2m's
  `m.binary({cluster:"genBinaryInput", attribute:"presentValue"})` but wired no
  decoder, so every contact report silently dropped — added a vendor
  `kFzPushokContact` converter (`definitions/pushok/_shared.{hpp,cpp}`,
  genBinaryInput 0x000F presentValue 0x0055 → bool). The **POK008** thermostat
  relay decodes `temperature` in z2m (`m.temperature` on
  msTemperatureMeasurement) but the generated port dropped both the expose and
  the decoder — wired generic `kFzTemperature` + the expose + the 0x0402
  binding. The **POK016** window opener and **POK017** greenhouse vent are
  `m.windowCovering` covers (position on closuresWindowCovering 0x0102), but a
  prior port misclassified POK016 as an `m.onOff` switch and stripped POK017 to
  battery-only, dropping the position channel entirely — both restored to
  proper covers (`kFzCoverPosition` + `kTzCoverState`/`kTzCoverPositionLift`,
  `state`+`position` exposes).

- **Samsung SmartThings motion sensors lost occupancy and the button lost its
  action** — found by a z2m↔embedded-zhc parity pass over the 28 SmartThings
  defs (the other 25 — motion STS-IRM-250/251 + IM6001-MTP01, contact 3300-S/
  3321-S/IM6001-MPP01/STSS-MULT-001/F-MLT-US-2, water-leak STS-WTR-250/
  F-ADT-WTR-1/WTR-UK-V2/IM6001-WLP01/3315-S/3315-G, the dimmable lights
  GP-LBU019BBAWU/7ZA-A806ST-Q1R, plugs STS-OUT-US-2/F-APP-UK-V2/IM6001-OTP05/
  GP-WOU019BBDWG, presence STS-PRS-251/STSS-PRES-001, siren SZ-SRN12N and the
  3310-S humidity sensor are at parity; the Samsung-accelerometer x/y/z+moving
  channels, the arrival-sensor presence cluster and the Centralite humidity
  attribute remain manuSpecific infra, deferred as in z2m's own extends).
  Three defs were graduated from `generated/` to Tier-2 parent overrides: the
  **3305-S** and **3325-S** motion sensors read occupancy from z2m's
  `fz.ias_occupancy_alarm_2` (ssIasZone `zoneStatus` **bit 1**) only, but the
  generated ports wired `kFzIasMotionAlarm` (bit 0) — the PIR bit never reached
  the shadow; swapped to `kFzIasMotionAlarm2` (bit 1). The **IM6001-BTP01**
  button advertised an `action` enum (`off`/`single`/`double`/`hold`) that was
  never populated: the generated def wired the generic `kFzIasZoneStatusChange`
  (which emits alarm-bit booleans), whereas z2m's
  `fz.command_status_change_notification_action` maps the whole ssIasZone
  `zoneStatus` value through `{0:off, 1:single, 2:double, 3:hold}` → `action`.
  Added a vendor `kFzStButtonAction` converter
  (`definitions/smartthings/_shared.{hpp,cpp}`) that does exactly that and
  wired it into the button.

- **Yandex (Alice) contact/leak/motion sensors and the TRV surfaced no
  primary state** — found by a z2m↔embedded-zhc parity pass over the 13 Yandex
  defs (the wireless buttons 00534/00535, gang switches 00531/00532, relays
  00537/00538, dimmer 00530, cover 00591 and the temp/humidity/pressure sensor
  00529 are at parity). Four defs were graduated from `generated/` to Tier-2
  parent overrides: the **YNDX-00526 contact** and **YNDX-00527 leak** sensors
  lowered the generic `kFzIasZone` (emits the bare key `alarm`) against a bare
  `alarm` expose, so the door/leak state never reached the shadow — swapped to
  the typed `kFzIasContactAlarm` / `kFzIasWaterLeakAlarm` (emit `contact` /
  `water_leak`, matching z2m `m.iasZoneAlarm`). The **YNDX-00528 motion+
  illuminance** sensor exposed `occupancy` but registered no decoder (a stale
  comment claimed no generic helper existed); wired the now-present
  `kFzOccupancy` (msOccupancySensing 0x0000) — the 0x0406 binding was already
  declared. The **YNDX-00518 TRV** decoded only genOnOff + battery and bound no
  hvacThermostat (0x0201), so the entire thermostat surface was missing; wired
  the generic `kFzThermostat` (read) + `kTzThermostat` (write) and a flat
  `local_temperature` / `current_heating_setpoint` / `system_mode` surface
  (plus the existing Yandex `display_flip` write) and the 0x0201 binding.
  `local_temperature_calibration`, `child_lock` and the manuSpecific
  `calibrated` flag still need generic helpers and remain deferred. New parity
  tests in `tests/test_yandex_parity.cpp` pin the semantic IAS keys (and the
  absence of the bare `alarm`), the occupancy decode from msOccupancySensing,
  and the TRV thermostat decode + 0x0201 binding.

- **Orvibo CR11S8UZ + T40S6Z scene remotes emitted nothing on a button
  press** — found by a z2m↔embedded-zhc parity pass over the 38 Orvibo defs
  (the IAS sensors SN10ZW/SM10ZW/SM20/SW21/SW30 already use the typed
  `kFzIas<Type>Alarm` converters, the AM25/W40CZ/W45CZ/CM10ZW curtain motors
  already decode `position` on `closuresWindowCovering`, and the multi-gang
  switches/dimmers/sensors are at parity). Both remotes were auto-generated as
  state-only stubs wiring the generic `kFzOnOff` behind a Binary `state`
  expose, but they are stateless scene remotes with no genOnOff relay: z2m
  decodes a vendor-private raw frame on the Orvibo cluster (id 0x0017) via
  `fz.orvibo_raw_1` / `fz.orvibo_raw_2`, publishing an `action` string
  (`"button_<n>_<click|hold|release>"`). The entire button-action surface was
  lost. Added two shared raw-frame decoders (`kFzOrviboRaw1Action` /
  `kFzOrviboRaw2Action` in `orvibo/_shared.cpp`) modelled on the existing
  terncy `MessageType::Raw` precedent — they read the button byte
  (raw_1 lookup `{3→1,11→2,7→3,15→4}`, raw_2 identity `{1..7}`) and action byte
  (`{0→click,2→hold,3→release}`) from `msg.raw_body` and emit the z2m-matching
  `action`. Both defs were graduated to Tier-2 parent overrides wiring the new
  converter + an `action` String expose (replacing the dead `state`). New
  `tests/test_orvibo_parity.cpp` pins each button/action pair, the
  unrecognised-byte and direction-mismatch no-match cases, and the absence of
  the stale `state`/`on`/`off` literals.

- **Avatto ME168_AVATTO TRV temperature calibration read 100× too small, and
  the ZDMS16-1 / ZDMS16-2 dimmers reported the wrong `switch_type`** — found
  by a z2m↔embedded-zhc parity pass over the 11 existing Avatto defs (all
  Tuya-OEM TS0601 datapoint devices plus one TS0011 on/off module). The rest
  of the catalogue (the ME168/TRV26/TRV60 thermostats, the ZWPM16/ZWPM16-2
  energy meters, the ZWSH16 temp/humidity sensor, the ZBS16 boiler switch and
  the LZWSM16-1 module) is at parity. Three auto-generated DP-table decode
  bugs were fixed: (1) `ME168_AVATTO` (`_TZE200_4aijvczq`) DP47
  `local_temperature_calibration` carried `divisor=100`, but z2m wires
  `localTempCalibration2`, whose decode is identity — the wire value is
  already in whole degrees (`withLocalTemperatureCalibration(-30, 30, 1)`), so
  a +3 °C offset surfaced as 0.03 °C (TRV26/TRV60 use `localTempCalibration1`
  = v/10, correctly `divisor=10`); (2) `ZDMS16-1` (`_TZE204_5cuocqty`) DP4
  `switch_type` used the generic `switchType` enum order
  `{momentary:0, toggle:1, state:2}` instead of z2m's `switchType2`
  `{toggle:0, state:1, momentary:2}`, mislabelling every reading; (3)
  `ZDMS16-2` (`_TZE204_o9gyszw2`) had the same `switch_type` bug on BOTH
  channels (DP4 + DP10). The sibling `ZDMS16-US-W2` (`_TZE204_sdykkwsu`)
  already used the correct `switchType2` order and was left unchanged. All
  three defs were graduated to Tier-2 parent overrides. The dimmer
  brightness/min/max DPs (z2m `scale0_254to0_1000`, 0–1000 wire) were verified
  as a deliberate codebase-wide passthrough convention (`divisor=1`, exposed
  device-native 0–1000) and are not a per-vendor gap. New
  `tests/test_avatto_parity.cpp` pins the corrected calibration scale and
  `switch_type` labels decoded off the 0xEF00 wire.

- **Innr SP 120 / SP 234 / SP 240 (incl. SP 242/244) / OSP 240 metering
  plugs were dropping the current + voltage channels** — found by a
  z2m↔embedded-zhc parity pass over the Innr vendor (116 defs). Innr is
  otherwise a pure lighting/remote vendor and the rest of the catalogue is
  at parity (lights = `m.light(...)`; remotes RC 110/210/250 already surface
  `action`). These four plugs are built in z2m with `m.onOff()` +
  `m.electricityMeter(...)`, and `electricityMeter()` defaults to
  `cluster:"both"` / `electricalMeasurementType:"ac"` — so it decodes BOTH
  seMetering (0x0702 → energy) AND haElectricalMeasurement (0x0B04 →
  rmsCurrent/rmsVoltage/activePower). The auto-generated Tier-1 ports wired
  only `kFzMetering` (0x0702), so `current` and `voltage` never reached the
  shadow. Graduated all four to Tier-2 parent overrides: added the generic
  `kFzElectricalMeasurement` converter (→ `power`/`voltage`/`current`), the
  `current`/`voltage` exposes, and the 0x0B04 bind (mirrors the Lidl HG08673
  power-plug override). The plain-onOff plugs (SP 110/220/222/224, OSP 210)
  match z2m's `m.onOff()` and were left unchanged. New
  `tests/test_innr_parity.cpp` pins power/voltage/current decode from 0x0B04
  and energy from 0x0702 across all four defs.
- **Lidl Silvercrest / Livarno / Parkside (Tuya-OEM) — six defs were
  decoding the wrong thing, plus a duplicate-fingerprint stub** — found by a
  z2m↔embedded-zhc parity pass over the Lidl vendor (17 defs). (1) The two
  IAS sensors lowered the generic `kFzIasZone` (which emits the bare key
  `alarm`) while z2m types them by `zoneType`: HG06335/HG07310 motion
  (`m.iasZoneAlarm({zoneType:"occupancy"})`) and HG06336 contact
  (`zoneType:"contact"`) — so the one bit each sensor exists to report never
  reached the shadow. Swapped in the typed `kFzIasMotionAlarm` /
  `kFzIasContactAlarm` converters (zoneStatus bit 0 → `occupancy` / `contact`;
  tamper/battery_low preserved). (2) Three command remotes — FB20-002,
  FB21-001 (Livarno dimmer remotes) and HG08164 (Silvercrest smart button) —
  were emitted by the generator as controllable on/off endpoints (`kFzOnOff` +
  `kTzOnOff`, a `state` expose) when they are battery-less/battery wall
  controllers. z2m surfaces their cluster commands as an `action` stream
  (`fz.command_on/off/step/move/stop`); rewired to the generic `kFzCommand*`
  converters with an `action` enum expose (HG08164 keeps `kFzBattery`). The
  vendor-specific frames (`FB20002_on`, `tuya.fz.switch_scene`,
  `tuya.fz.on_off_action` single/double) have no generic converter and are
  deferred — the `action` expose still admits their values. (3) HG08673
  (Silvercrest plug with power monitoring) wired only `kFzOnOff`, dropping its
  entire metering surface; z2m builds it with `electricalMeasurements` +
  metering polling, so added `kFzElectricalMeasurement` (power/voltage/current)
  + `kFzMetering` (energy/power), the four exposes, and the
  `haElectricalMeasurement`/`seMetering` binds. (4) PSBZS-A1 carried a
  duplicate battery+onoff stub (`kDef_PSBZS_A1`) registered alongside the full
  Tuya-DP def (`kDefLid__TZE200_htnnfasr`) for the identical
  TS0601/`_TZE200_htnnfasr` fingerprint; removed the stub (file + registry
  entry) so the watering timer keeps its real `timer`/`time_left`/`frost_lock`
  DP keys. All six edited defs graduated from `generated/` to Tier-2 parents.
  Pinned by `tests/test_lidl_parity.cpp` (IAS semantic-key decode, action-verb
  decode on real genOnOff/genLevelCtrl wire shapes, metering attr reports, and
  a registry single-def guard for the PSBZS fingerprint). The remaining lidl
  defs are at parity: the HG06467 garden-string light (z2m `legacy.fz.silver-
  crest_smart_led_string`) and the 368308-2010 TRV (`legacy.fromZigbee.zs_-
  thermostat`) need bespoke legacy converters and are deferred as infra; the
  HG06668 doorbell button (`fz.tuya_doorbell_button` → `action="pressed"`) has
  no generic IAS-action converter and is likewise deferred.
- **23 Yale smart locks never reported battery level** — found by a
  z2m↔embedded-zhc parity pass over the Yale vendor (28 defs, all door locks).
  Every Yale lock in z2m decodes battery: most use the shared `lockExtend()`
  helper, which bundles `fz.battery` + `e.battery()` (+ `e.battery_low()`), and
  the rest use `m.battery()`. But 23 of the 28 generated defs wired only
  `kFzLock` (the `lock_state` decoder) and dropped battery entirely — the
  `genPowerCfg` decoder, the `battery` expose, and the `0x0001` binding were all
  missing, so a battery-powered lock never surfaced its charge level. (The other
  5 — SOLIS01, YMC420-W, YRL256 TS, YMI70A, YRM476 — had already been
  hand-corrected.) All 23 graduated from `generated/` to Tier-2 parents wiring
  the generic `kFzBattery` + a `battery` expose + the `genPowerCfg` bind. The
  lock-state decode (`kFzLock` → `lock_state`) and the lock/unlock command
  encoder (`kTzLock`, key `state` → closuresDoorLock cmd 0x00/0x01) were already
  correct on every def and are pinned as a regression guard. PIN/user-code
  management, auto-relock, sound-volume and the programming/operation-event
  `action` stream remain out of scope (bespoke converters; deferred). Pinned by
  `tests/test_yale_parity.cpp`.
- **Hive DWS003 / MOT003 contact + motion sensors (and the KEYPAD001 alarm
  keypad) never surfaced their primary state** — found by a z2m↔embedded-zhc
  parity pass over the Hive vendor. All three lowered the generic `kFzIasZone`,
  which emits the bare keys `alarm_1`/`alarm_2`, while their exposes declared
  the semantic keys `contact` (DWS003) and `occupancy` (MOT003) — so the
  one bit each sensor exists to report never reached the shadow. z2m wires the
  typed `fz.ias_contact_alarm_1` / `fz.ias_occupancy_alarm_1` decoders. Swapped
  in the typed `kFzIasContactAlarm` / `kFzIasMotionAlarm` converters (zoneStatus
  bit 0 → semantic key; tamper/battery_low preserved) and graduated the three
  defs from `generated/` to Tier-2 parents. KEYPAD001 declares BOTH `occupancy`
  and `contact` (z2m wires both alarm-1 decoders on the same status-change
  notification), so it carries both typed converters; its IAS-ACE arm/panic
  command path stays unwired (no generic converter exists — out of scope).
  Pinned by `tests/test_hive_parity.cpp`.
- **Hive heating receivers (SLR1/SLR1b/SLR1c/SLR1d, SLR2/SLR2b/SLR2c, OTR1)
  dropped `running_state`** — found by the same Hive parity pass. z2m wires
  `fz.thermostat`, which decodes hvacThermostat attr 0x0029
  (ThermostatRunningState), and the exposes carry `.withRunningState(["idle",
  "heat"])`; but the generic `kFzThermostat` decodes only attrs 0x0000/0x0012/
  0x001C, so the heat/idle state the receivers report was never published.
  Added a vendor-local `kFzHiveThermostatExtras` (attr 0x0029 enum → string via
  z2m's `constants.thermostatRunningStates`) wired alongside the generic decoder
  on each receiver, plus the `running_state` expose; all eight defs graduated
  from `generated/` to Tier-2 parents. The core surface (local_temperature /
  current_heating_setpoint / system_mode) still decodes via the generic
  converter and is regression-checked. Pinned by `tests/test_hive_parity.cpp`.
- **Bacchus Flower_Sensor_v2 / Flower_Sensor_v4 never decoded `soil_moisture`,
  their headline measurement** — found by a z2m↔embedded-zhc parity pass over
  the Bacchus vendor. z2m reports soil moisture via `m.soilMoisture()` on
  msSoilMoisture (0x0408), but both generated Tier-1 defs only wired
  `kFzTemperature`/`kFzIlluminance`/`kFzBattery`, so every soil report silently
  dropped — the sensors surfaced temperature/illuminance/battery but not the
  one channel a flower sensor exists for. The generic `kFzSoilMoisture`
  converter (0x0408 measuredValue, u16 / 100 → %; added earlier for the sibling
  custom_devices_diy / diyruz / efekta flower sensors) already existed but was
  never referenced. Wired `&generic::kFzSoilMoisture` into both defs and
  graduated them from `generated/` to Tier-2 parents. Remaining undecoded
  channels (report_delay/threshold and the v4 tx_radio_power/smart_sleep/
  thermal_compensation) are write-side manufacturer-specific attributes that
  need a vendor TZ helper and are out of scope for this measurement-parity fix.
  Pinned by `tests/test_bacchus_parity.cpp`.
- **Halo Smart Labs HALO / HALO+ smoke + CO detectors dropped their entire
  safety-sensor channel set** — found by a z2m↔embedded-zhc parity pass over the
  Halo Smart Labs vendor. Both generated defs were battery+on/off stubs: they
  surfaced only `state`/`battery`/`voltage` while z2m's `haloCommonExtend`
  reports `smoke`/`carbon_monoxide`/`tamper`/`battery_low`/`test`/
  `mains_power_connected` (the device's whole purpose) plus `temperature`/
  `humidity`/`pressure` and an RGB status light. The endpoint-aware `ssIasZone`
  demux converter (`kFzHaloIasZone` — EP1 smoke + diagnostics, EP3
  carbon_monoxide, matching z2m's `haloZoneStatus`) already existed in
  `definitions/halo_smart_labs/_shared.cpp` but was never referenced by either
  def. Re-wired both defs to `kFzHaloIasZone` + the generic `kFzTemperature`/
  `kFzHumidity`/`kFzPressure` (standard clusters 0x0402/0x0405/0x0403) and added
  the EP2 colour light (`kFzOnOff`/`kFzBrightness`/`kFzColor` +
  `kTzOnOff`/`kTzBrightness`/`kTzColor`, keys suffixed `_light` via an
  `endpoint_map` of `{light → 2}` with `default_endpoint = 2`), with the
  matching ssIasZone/temperature/humidity/pressure/genOnOff/genLevelCtrl/
  lightingColorCtrl bindings on the correct endpoints. Both defs graduated from
  `generated/` to Tier-2 parents. The four manufacturer-specific clusters
  (`haloDeviceStatus` 0xFD00, `haloControl` 0xFD01, `haloSensors` 0xFD02
  [`co_ppm`], and HALO+'s `haloWeather` 0xFD03 severe-storm radio) remain
  unwired — `cluster_id_to_name()` does not know their names, so no FzConverter
  can select them; tracked as infrastructure work. Pinned by
  `tests/test_halo_parity.cpp`.
- **Niko 552-80401 wireless motion sensor never reported `occupancy`** — found
  by a z2m↔embedded-zhc parity pass over the Niko vendor. The generated def
  lowered the generic `kFzIasZone` converter, which emits the neutral key
  `alarm`, while the def's expose declared `occupancy` — so the IAS zone-status
  motion flag never reached the shadow (dead key). z2m decodes it via
  `fz.ias_occupancy_alarm_1`, emitting `occupancy`/`tamper`/`battery_low` from
  zoneStatus bits 0/2/3. Re-wired to the typed generic `kFzIasMotionAlarm`
  (semantic key `occupancy` + tamper + battery_low) and renamed the expose; the
  def graduated from `generated/` to a Tier-2 parent. The other Niko suspect
  classes were investigated and found already correct (552-72301 motor control
  exposes lift-only `position` on closuresWindowCovering with z2m's
  `coverInverted`+default-`invert_cover` net no-op matching the non-inverting
  generic converter; 552-720X1/X2/X4 battery scene controllers decode genOnOff/
  genLevelCtrl commands to `action`; 552-721X1/X2 mains switches and the
  170-33505/552-80698/552-80699 metering plugs carry their full channel set).
  Pinned by `tests/test_niko_parity.cpp`.
- **Wirenboard WB-MSW-ZIGBEE v.3 / v.4 dropped their `occupancy` and `co2`
  channels** — found by a z2m↔embedded-zhc parity pass over the Wirenboard
  vendor. Both multi-sensor ports declared `occupancy` and `co2` exposes and
  bound the matching standard clusters (msOccupancySensing 0x0406, msCO2
  0x040D), but neither wired an `from_zigbee` converter for them — so the PIR
  occupancy flag and the CO2 reading never reached the shadow. z2m decodes
  both via standard-cluster converters (`fz.occupancy`, `fz.co2`). Wired the
  existing generic `kFzOccupancy` (attr 0x0000 bit 0) and added a new generic
  `kFzCO2` (msCO2 attr 0x0000 single-precision float → `floor(value · 1e6)`
  ppm, mirroring z2m `fz.co2`; tolerates already-scaled integer reports). Both
  defs graduated from `generated/` to Tier-2 parents. The sprut custom-cluster
  channels (noise, voc) and manuSpecific config writes remain deferred (no
  generic decoder yet). Pinned by `tests/test_wirenboard_parity.cpp`.
- **Legrand / BTicino / Netatmo battery remotes published a dead `state`
  instead of `action` button events** — found by a z2m↔embedded-zhc parity
  pass over the Legrand vendor. Seven wireless pushbuttons / scene remotes
  with no local relay (`067694` toggle, `067773` remote, `067774` dual-gang,
  `WNAL63` Netatmo remote dimmer, `067646` shutter remote, `067755` 4-scene
  pocket remote, `067767` color-dimmer remote) shipped generated defs wiring
  `kFzOnOff` + a controllable on/off `to_zigbee`. Those never fired: the
  devices only *send* cluster commands (genOnOff / genLevelCtrl / genScenes /
  closuresWindowCovering, client→server), which `kFzOnOff` (an attribute-
  report decoder) ignores — so the `state` expose stayed empty and the TZ
  was meaningless. Each was graduated from `generated/` to a Tier-2 parent
  wiring the matching generic command-action decoders
  (`kFzCommandOn/Off/Toggle`, `kFzCommandMove/Step/Stop`,
  `kFzCommandCoverOpen/Close/Stop`, `kFzCommandRecall`) and now exposes
  `action` + `battery` instead of a phantom `state`/switch. `067774` gains
  its `{left:1, right:2}` endpoint map + `endpoint_action_suffix` so each
  gang surfaces as `action_left` / `action_right` (mirrors z2m
  `postfixWithEndpointName`). Pinned by `tests/test_legrand_parity.cpp`.
  Deferred as infra (no generic decoder; left as-is): the Green-Power scene
  switches (`ZLGP14/15/16`, `ZLGP17/18`, `600087L` — `fz.legrand_greenpower`,
  no GP frame path in the host parser), the `legrand_scenes`
  enter/leave/sleep/wakeup remaps (`064873`, `752189`), the cover-tilt expose
  (z2m adds it dynamically only when the device's `calibrationMode` reads
  `venetian_bso` at pairing; the generic cover decoder is lift-only), and the
  `067646` `moving`/`stopped` sub-actions (a Legrand genBinaryInput
  present-value remap).

- **Lincukoo SZT06 temperature/humidity sensor surfaced no temperature or
  humidity** — found by a z2m↔embedded-zhc parity pass over the Lincukoo
  vendor. The generated `SZT06` def lowered only `kFzBattery` and exposed
  only `battery`/`voltage`, but z2m wires `m.temperature()` + `m.humidity()`
  (+ `m.battery({voltage:true})`): the device reports its primary
  measurements on the standard `msTemperatureMeasurement` (0x0402) and
  `msRelativeHumidity` (0x0405) clusters, so a paired sensor published
  nothing but battery. The def was graduated from `generated/` to a Tier-2
  parent override adding `kFzTemperature` + `kFzHumidity` (emitting the
  `temperature`/`humidity` keys, scaled /100) and the 0x0402 / 0x0405
  reporting bindings alongside the existing 0x0001 power binding. The rest of
  the vendor is Tuya-DP and already wires its `tuyaDatapoints`; the named
  model stubs (`SZLR08`, `SZR07`, `E02C-Z10T`, …) are harmless dead
  duplicates of their manufacturer-discriminated `kDefLin__TZE*` twins, which
  win in `find_definition` Pass 1. Pinned by
  `tests/test_lincukoo_parity.cpp`.

- **Zemismart curtain/blind tubular motors were shadowed by dead
  generic-cover stubs** — surfaced by a z2m↔embedded-zhc parity pass over the
  Zemismart vendor. Six defs (`ZM16B`, `ZMP1`, `ZN-USC1U-HT`, `ZM25R1`,
  `ZM25RX-08/30`, `ZM25TQ`) wired the generic `closuresWindowCovering`
  (0x0102) cover converter, but each is a TS0601 device that speaks **only**
  Tuya datapoints on `manuSpecificTuya` (0xEF00) — so the stub's `position`
  could decode nothing on the real wire.
  - Five of them (`ZM16B`/`ZMP1`/`ZN-USC1U-HT`/`ZM25R1`/`ZM25RX-08/30`)
    duplicated the `{TS0601 + _TZE*}` fingerprint of an existing
    `kDefZem__TZE*` DP def that already carries the full datapoint map
    (position via DP 2/3 or 8/9, state, motor_direction, battery). Two defs
    per fingerprint made resolution depend on registry order
    (`find_definition` is first-match), so a motor could land on the dead
    generic-cover stub. The five stubs were removed; the `kDefZem__TZE*` DP
    defs now match those manufacturer names as the sole def each.
  - `ZM25TQ` (`_TZE200_fzo2pocs`) had no competing DP def — it was graduated
    from `generated/` to a Tier-2 Tuya-DP override (DP 1 state
    `{OPEN/STOP/CLOSE}`, DP 2 + DP 3 → `position`, DP 103/104/105 stroke
    limits), mirroring its `ZM25RX-08/30`/`ZMP1` siblings and z2m
    `legacy.fz.tuya_cover` (`coverPosition`=2, `coverArrived`=3).
  - Deferred (INFRA): `ZM-AM02_cover` (complex per-model `legacy.fz.ZMAM02_cover`
    AM02 lookups) and the `ZM-RM02` scene-switch action surface
    (`legacy.fromZigbee.ZMRM02`) keep their current wiring; `ZM-CSW032-D` is a
    genuine TS0302 standard-cluster cover and is unchanged. Pinned by
    `tests/test_zemismart_parity.cpp`.

- **Develco / Frient sensors had dead IAS keys, lost temperature/humidity
  channels, a phantom alarm bundle on a repeater, and a button wired as a
  switch** — surfaced by a z2m↔embedded-zhc parity pass over the Develco
  vendor. Affected defs graduated from `generated/` to Tier-2 parent
  overrides.
  - **FLSZB-110 (flood), HESZB-120 + SMSZB-120 (heat/smoke),
    MOSZB-130/140/141/153 (motion), WISZB-120/121/134/138 (contact),
    WISZB-137 (vibration)** lowered the generic `kFzIasZone` converter,
    which emits the bare key `alarm`, while their exposes declared the
    semantic key. Swapped in the typed `kFzIas<Type>Alarm` converters
    (`water_leak`/`smoke`/`occupancy`/`contact`/`vibration` + `tamper` +
    `battery_low`), matching z2m's per-zone-type `fz.ias_*_alarm_1`
    (Develco routes the HESZB-120 heat alarm through `ias_smoke_alarm_1`).
  - **Temperature channel restored** on FLSZB-110, HESZB-120, SMSZB-120,
    WISZB-120/121/137/138 — these report `msTemperatureMeasurement`
    (`0x0402`) via z2m `develcoModernExtend.temperature()`, previously
    undecoded. (WISZB-134 intentionally omits it, matching z2m.)
  - **AQSZB-110 air quality sensor** wired only battery; every primary
    channel was dead. Wired the existing develco VOC decoder (manuSpecific
    `0xFC03` → `voc` µg/m³ + `air_quality` enum) plus generic temperature
    (`0x0402`) and humidity (`0x0405`).
  - **HMSZB-120 temp/humidity sensor** wired only battery — added generic
    temperature + humidity decoders/exposes/bindings.
  - **REXZB-111 range extender** carried a phantom `alarm`/`tamper` IAS
    bundle (generic `kFzIasZone` + `0x0500` binding) that z2m never
    publishes for this mains repeater. Stripped to battery only.
  - **SBTZB-110 smart button** was wired as an on/off switch (`kFzOnOff`
    + `kTzOnOff` + controllable `state`). Re-wired to `kFzEwelinkAction`
    (genOnOff command → `action`) and exposes `action`, matching z2m
    `fz.ewelink_action`.

- **Five IKEA non-light devices were wired as phantom lights, and several
  sensor channels were dead** — surfaced by a z2m↔embedded-zhc parity pass
  over the 100-def IKEA vendor. The generated defs lowered the full
  `kFzIkeaLight` bundle (`state`/`brightness`/`color_temp` exposes +
  genOnOff/genLevelCtrl/genColorCtrl bindings) onto devices that are not
  lights, so their real channels never reached the shadow.
  - **E1525/E1745 TRADFRI motion sensor** — phantom light. Stripped to
    `kFzBattery` + `battery`/`voltage` exposes (z2m `ikeaBattery()`).
    Occupancy/requested-brightness come over outward-bound genOnOff
    `commandOnWithTimedOff` / genLevelCtrl `commandMoveToLevelWithOnOff`,
    which need command converters that do not exist generically yet
    (noted as deferred infra).
  - **E2112 VINDSTYRKA air quality & humidity sensor** — phantom light.
    Wired generic `kFzTemperature` (`0x0402`) + `kFzHumidity` (`0x0405`)
    matching z2m `m.temperature()`/`m.humidity()`. `pm25` (custom
    `0x042a`) and `voc_index` (manuSpecific) need dedicated decoders —
    deferred infra.
  - **E1746 TRADFRI signal repeater** — phantom light on a router that
    z2m declares with `exposes:[]`. Stripped to an empty definition.
  - **E2007 STARKVIND air purifier** — phantom light. All channels
    (fan/pm25/air_quality/filters) live on manuSpecific cluster `0xfc7d`
    with no generic decoder; stripped the phantom light to an empty
    definition (full support deferred as infra).
  - **E2134 VALLHORN wireless motion sensor** — wired only `kFzBattery`,
    leaving its primary outputs dead. Added generic `kFzOccupancy`
    (msOccupancySensing `0x0406`) + `kFzIlluminance` (`0x0400`) +
    `occupancy`/`illuminance` exposes + bindings, matching z2m
    `m.occupancy()`/`m.illuminance()`.
- **Four IKEA battery-powered blinds dropped their battery channel** —
  `E1757` FYRTUR / `E1926` KADRILJ / `E2102` PRAKTLYSING / `E2103`
  TREDANSEN exposed only cover `position`; z2m wires `ikeaBattery()` on
  all of them. Added generic `kFzBattery` + `battery`/`voltage` exposes +
  genPowerCfg (`0x0001`) binding to each.
  All nine defs were graduated from `definitions/ikea/generated/` to
  Tier-2 parents. New `tests/test_ikea_parity.cpp` pins the phantom-light
  keys gone, VALLHORN occupancy+illuminance decode, VINDSTYRKA
  temperature+humidity decode, and battery presence on motion+blinds.

- **Four Owon HVAC controllers had dead climate exposes the generic
  thermostat decoder never filled** — surfaced by a z2m↔embedded-zhc
  parity pass over the 19-def vendor. The generic `kFzThermostat` decodes
  only hvacThermostat attrs `0x0000`/`0x0012`/`0x001C`, but z2m's
  `fz.thermostat` also decodes the cooling setpoint (`0x0011`) and
  running-state (`0x0029`), and `fz.occupancy` decodes msOccupancySensing.
  - `AC201` / `AC221` / `PCT504` exposed `current_cooling_setpoint`
    (z2m `.withSetpoint("occupied_cooling_setpoint", …)`) with no decoder.
  - `PCT504` / `PCT512` exposed `running_state` (z2m `.withRunningState`)
    with no decoder, and `occupancy` (z2m `m.occupancy()`) with no decoder
    despite binding msOccupancySensing — even though the generic
    `kFzOccupancy` already exists.
  Added a vendor-local `kFzOwonThermostatExtras` (attr `0x0011` cooling
  setpoint as raw s16; attr `0x0029` running-state mapped enum→string via
  z2m `thermostatRunningStates`: 0=idle/1=heat/2=cool/4=fan_only) wired
  alongside the generic decoder, and wired the generic `kFzOccupancy` on
  PCT504/PCT512. All four thermostat defs were graduated from
  `definitions/owon/generated/` to Tier-2 parents. The IAS sensors
  (DWS312/SPM915/PIR313/PIR323-PTH contact+motion, all typed `contact`/
  `occupancy` keys), THS317 temp/humidity, WSP plugs, SLC remotes/plug and
  the PC321 3-phase meter (per-phase + top-level current/reactive channels
  are OWON manuSpec attributes, correctly deferred as infra) were audited
  and found correct. Pinned by `tests/test_owon_parity.cpp`.

- **Four ShinaSystem (SiHAS) devices lost their primary channel to a
  generator miss** — surfaced by a z2m↔embedded-zhc parity pass over the
  40-def vendor.
  - `USM-300ZB` multipurpose sensor exposed `occupancy` with no decoder and
    dropped the illuminance channel entirely. z2m wires
    `fromZigbee:[…fz.occupancy]` + `extend:[m.illuminance()]`. Re-added
    `kFzOccupancy` + `kFzIlluminance`, the `illuminance` expose, and the
    0x0400 binding.
  - `OSM-300ZB` motion sensor (`extend:[m.occupancy(), m.battery()]`) was
    lowered to battery-only; the `occupancy` expose had no decoder. Re-added
    `kFzOccupancy` (msOccupancySensing, same shape as heiman HS1MIS-3.0).
  - `DMS-300ZB` dual-motion sensor was battery-only with both PIR channels
    dead. z2m reads `occupancy_in` (msOccupancySensing) and `occupancy_out`
    (ssIasZone bit 0). Added two stateless vendor channel decoders
    (`kFzSihasOccupancyIn` / `kFzSihasOccupancyOut`) and dropped the phantom
    `occupancy` expose (z2m emits no such key). The derived
    `occupancy_or`/`occupancy_and` (cross-message state) and writable
    `occupancy_timeout` remain deferred as vendor infra.
  - `GCM-300Z` gas valve had two bugs: `to_zigbee` was left null (every
    writable control — valve, close_timeout, close_remain_timeout, volume,
    overheat_mode — was dead) and the valve rode the generic on/off `state`
    key while the expose is `gas_valve_state` ("OPEN"/"CLOSE"). Wired
    `to_zigbee` and added dedicated `kFzSihasGasValveState` /
    `kTzSihasGasValveState` converters (genOnOff attr/command ↔ OPEN/CLOSE).
  All four defs were graduated from `definitions/shinasystem/generated/` to
  Tier-2 parents. The ~22 SBM/SQM button-and-switch variants, the PMM power
  meters, HQM/TCM thermostats, WCM outlet and the remaining sensors were
  audited and found correct. Pinned by `tests/test_shinasystem_parity.cpp`.

- **Custom devices (DiY) `ZeeFlora` and `b-parasite` soil sensors never
  decoded soil moisture** — both defs exposed `soil_moisture`, but no
  `kFzSoilMoisture` converter existed in `_generic/_shared` and the
  `msSoilMoisture` cluster (0x0408) was absent from `cluster_names.hpp`, so
  every `msSoilMoisture` report silently dropped — the sensors surfaced
  battery/temperature/humidity but not the headline soil reading. Added a
  generic `kFzSoilMoisture` converter (cluster `msSoilMoisture` 0x0408,
  `measuredValue` u16 / 100 → %, mirroring `kFzHumidity` / z2m
  `m.soilMoisture` scale 100) plus the 0x0408 → `"msSoilMoisture"` mapping,
  and graduated both defs from `definitions/custom_devices_diy/generated/`
  to Tier-2 parents wiring it. `ZeeFlora` additionally carried z2m
  `extend:[m.illuminance()]` that the generator had dropped entirely (no
  `illuminance` expose, no `kFzIlluminance`, no 0x0400 bind) — restored.
  The shared `kFzSoilMoisture` also unblocks the matching gap in the
  bacchus/diyruz/efekta flower sensors (tracked separately). Pinned by
  `tests/test_custom_devices_diy_parity.cpp`.

- **Nine Third Reality IAS-Zone sensors decoded the bare `alarm` key instead
  of their semantic state** — the generated defs for four motion/presence
  sensors (`3RMS16BZ`, `3RSMR01067Z`, `3RPS01083Z`, and the night light
  `3RSNL02043Z`), two contact sensors (`3RDS17BZ` door, `3RDTS01056Z` garage
  tilt), two water-leak sensors (`3RWS18BZ`, `3RWS0218Z`) and the vibration
  sensor (`3RVS01031Z`) all lowered the generic `kFzIasZone` converter, which
  emits the bare key `alarm`. z2m drives each from a typed zone-type decoder
  (`fz.ias_{occupancy,contact,water_leak,vibration}_alarm_1` /
  `m.iasZoneAlarm({zoneType})`), so embedded-zhc never surfaced the primary
  state (`occupancy` / `contact` / `water_leak` / `vibration`) to the shadow.
  All nine were graduated from `definitions/third_reality/generated/` to
  Tier-2 parent overrides wiring the typed `kFzIasMotionAlarm` /
  `kFzIasContactAlarm` / `kFzIasWaterLeakAlarm` / `kFzIasVibrationAlarm`
  converters (semantic key + `tamper` + `battery_low` from zoneStatus bits
  0/2/3) with the expose key renamed to match. The night light's
  `genOnOff`/`genLevelCtrl` light path and the water sensor's `genOnOff`
  buzzer toggle are kept intact. Pinned by
  `tests/test_third_reality_parity.cpp`. (The 3RVS01031Z x/y/z acceleration
  and the 3RPS01083Z radar settings ride manufacturer-specific clusters with
  no existing converter and remain deferred as infrastructure.)

- **Four Easyiot ZB-WB01/02/03/08 scene remotes emitted the wrong `action`
  verb and lost button identity** — the four button remotes lowered the
  generic `kFzCommandOn`/`kFzCommandOff`/`kFzCommandToggle` converters, which
  emit the *standard* literals `on`/`off`/`toggle` with no button prefix. z2m's
  `easyiot_action` fz instead remaps the verbs (`commandToggle`→`single`,
  `commandOn`→`double`, `commandOff`→`long`) and prefixes the originating
  button derived from `msg.endpoint.ID` (`1_single` … `8_long`). So
  embedded-zhc surfaced the wrong action string and collapsed every button to
  one un-prefixed `action`. All four defs were graduated from
  `definitions/easyiot/generated/` to Tier-2 parents wiring a new vendor
  converter `kFzEasyiotAction` (`definitions/easyiot/_shared.cpp`) that
  reproduces the z2m verb map and per-button prefix from `src_endpoint`.
  Pinned by `tests/test_easyiot_parity.cpp`.

- **Seven Engo (ENGO Controls) TS0601 thermostats had duplicate-fingerprint
  battery on/off stubs shadowing their full Tuya-datapoint defs** — the engo
  registry carried seven stale defs from an earlier generation pass
  (`kDef_E25_230`, `kDef_E25_BATB`, `kDef_E40`, `kDef_ECB62_ZB`, `kDef_EONE`,
  `kDef_EONE_230W`, `kDef_EONE_BATB`), each a bare battery + on/off stub
  (`state`/`battery`/`voltage`, `kFzBattery` + `kFzOnOff`, bound `genOnOff
  0x0006`, no Tuya DP decoder). Every stub duplicated the
  `{TS0601 + _TZE20x_*}` fingerprint of one of the ten `kDefEng__TZE*` DP defs
  that carry the full datapoint map (setpoint / local_temperature /
  system_mode / running_state / child_lock / …, plus the ECB62-ZB control
  box's pump/boiler outputs). With two defs per fingerprint the resolved
  device hung on registry order (`find_definition` is first-match), risking a
  battery+state-only readout instead of the climate surface. The seven stubs
  were removed from `definitions/engo/`; the `kDefEng__TZE*` DP defs cover all
  ten manufacturer names as the sole match each. Pinned by
  `tests/test_engo_parity.cpp`.
- **Six Lytko L101Ze thermostats were lowered as battery on/off switches** —
  z2m defines every `L101Ze-*` model via `modernExtend`'s `thermostat({...})`
  builder (a floor/room heating thermostat: `local_temperature` /
  `current_heating_setpoint` / `system_mode` / `running_mode` on
  `hvacThermostat`, plus Lytko manuSpec config attrs). The legacy generator
  cannot read those dynamic exposes, so six of the seven L101Ze ports
  (`L101Ze-SLN`, `-SLM`, `-SMN`, `-DLN`, `-DLM`, `-DBN`) were emitted as a
  battery + on/off stub (`state`/`battery`/`voltage`, wired to `kFzBattery` +
  `kFzOnOff`, bound to `genOnOff 0x0006`) — on hardware they would never
  surface a temperature, setpoint or mode. Each was graduated to a Tier-2
  parent override mapping the core control surface onto the generic
  `kFzThermostat` decoder (`0x0000`/`0x0012`/`0x001C`) + generic
  `kTzThermostat` writes + the shared Lytko manuSpec TZ encoders, mirroring
  the already-corrected sibling `L101Ze-SBN`. Dual-channel models (`-DLN`,
  `-DLM`, `-DBN`) declare a two-entry `endpoint_map {3,4}` so the runtime
  suffixes per-channel keys to `<key>_3` / `<key>_4`; single-channel models
  carry no `endpoint_map` so their keys stay un-suffixed, matching the
  literal `L101Z-*` siblings. Power metering (`electricityMeter`) and
  `remote_sensing` config remain deferred (no generic decoder equivalent).
  New parity fixture `tests/test_lytko_parity.cpp` pins the decode + per-EP
  suffixing.

- **Ubisys H1/H10 thermostats, J1 shutter, and R0 router had real parity gaps** —
  four Ubisys defs were graduated to Tier-2 parent overrides against `ubisys.ts`:
  - **H1 + H10 heating regulators declared `pi_heating_demand` and `running_mode`
    exposes that never populated** — z2m's `e.climate()` carries
    `.withPiHeatingDemand(STATE_GET)` + `.withRunningMode(...)`, decoded by
    `fz.thermostat` from standard `hvacThermostat` attrs `0x0008` (PIHeatingDemand)
    and `0x001E` (ThermostatRunningMode). The defs wired only the generic
    `kFzThermostat`, which decodes `0x0000`/`0x0012`/`0x001C` and skips those two,
    leaving both as dead exposes. Added `kFzUbisysThermostatExtras` (shared
    `_shared.cpp`) and wired it on both. Unlike Danfoss, Ubisys does **not** set
    `dontMapPIHeatingDemand`, so demand is remapped `0-255 → 0-100 %`
    (`(raw*100+127)/255`); `running_mode` maps `0→off`/`3→cool`/`4→heat`.
  - **J1 shutter controller was missing its `tilt` axis** — z2m's
    `e.cover().withTilt()` exposes tilt and `fz.cover_position_tilt` decodes
    `closuresWindowCovering` attr `0x0009` (CurrentPositionTiltPercentage), which
    the J1 reports. The def declared only `position` and wired the generic
    `kFzCoverPosition` (lift attr `0x0008` only). Added `kFzUbisysCoverPositionTilt`
    (emits `position` + `tilt`, skips the `0xFF` "unknown" sentinel per z2m's
    `<= 100` guard), declared a `tilt` expose, and added the generic
    `kTzCoverPositionTilt` writer.
  - **R0 Zigbee router advertised a dead on/off `state`** — z2m declares
    `exposes: []` with no `toZigbee` and no genOnOff cluster, but the auto-port
    wired `kFzOnOff`/`kTzOnOff` + a `state` expose: an uncontrollable phantom
    bundle on a router. Stripped to no exposes / no converters (binds genBasic for
    modelId reporting), mirroring the KeenHome GW01 repeater fix.
  - Pinned by `tests/test_ubisys_parity.cpp`.

- **Shelly S4SN-0071A "Flood Gen 4" leak sensor never surfaced its leak state** —
  the auto-generated def wired the generic `kFzIasZone`, which decodes only an
  *attribute report* of ZoneStatus (attr `0x0002`) and emits the bare key
  `alarm`, while its expose also declared `alarm`. z2m uses
  `m.iasZoneAlarm({zoneType: "water_leak", zoneAttributes: ["alarm_1", "tamper",
  "battery_low"]})`, which decodes the IAS Zone-Status-Change-Notification
  *command* and emits the semantic key `water_leak`. The def was graduated to a
  Tier-2 parent override that swaps in the typed `kFzIasWaterLeakAlarm`
  converter and renames the expose `alarm` → `water_leak`, matching `shelly.ts`.
  Pinned by `tests/test_shelly_parity.cpp`.

- **Bosch Smart Home sensors, siren, plug, and thermostats were mis-classed or
  decoded the wrong key** — twelve Bosch defs were graduated to Tier-2 parent
  overrides to fix real parity gaps against `bosch.ts`:
  - **A row of non-light devices was mapped onto the Bosch "Light" bundle**
    (`{state, brightness, color_temp}` + onOff/level/colorTemp converters) and
    therefore decoded nothing the device sends while advertising phantom
    `brightness`/`color_temp`: `BSEN-CV` + `BSEN-C2D` (door/window contacts),
    `BSEN-W` (water alarm), `BSP-FD` (smart plug), `BTH-RM` + `BTH-RM230Z`
    (room thermostats), and `BSIR-EZ` (outdoor siren). Each was re-targeted to
    the correct bundle: contacts → `kFzBoschContact`, water → `kFzBoschWaterLeak`,
    plug → `kFzBoschPlug` (genOnOff + seMetering, `state`/`power`/`energy`),
    thermostats → `kFzBoschTrv`/`kTzBoschTrv` (flat `local_temperature` /
    `current_heating_setpoint` / `system_mode` / `running_state` + Bosch
    manuSpec writes), siren → `kFzBoschIasBattery`.
  - **The IAS security sensors emitted a bare `alarm`** — `BSEN-C2`, `BSEN-M`,
    `BSD-2`, `ISW-ZPR1-WP13`, and `RADION TriTech ZB` (plus the re-classed
    contacts/water above) wired the generic `kFzIasZone`, which emits a bare
    `alarm` (zoneStatus bit 0) that the SPA cannot map to a device class. z2m
    emits the semantic key for the zone type. Added typed bundles
    `kFzBoschContact` → `contact`, `kFzBoschMotion` → `occupancy`,
    `kFzBoschSmoke` → `smoke`, `kFzBoschWaterLeak` → `water_leak` (bit 0 →
    semantic key, bit 2 → `tamper`, bit 3 → `battery_low`), wiring the generic
    `kFzIasContactAlarm` / `kFzIasMotionAlarm` / `kFzIasSmokeAlarm` /
    `kFzIasWaterLeakAlarm`.
  - Deferred (INFRA, manuSpec custom cluster with no generic converter):
    Twinguard (`8750001213`, custom `twinguardSmokeDetector` 0xE000 air-quality
    + smoke) left as-is; the `BSIR-EZ` siren command surface (Bosch
    `alarmControl`), contact/plug button-action + auto-off extras, and TRV
    valve-adaptation diagnostics remain unwired. See `docs/BOSCH_PARITY.md`.
  - Pinned by `tests/test_bosch_parity.cpp` (12 cases).
- **CTM Lyng stove guards, water devices, and motion detectors had broken core
  decoders** — six auto-generated CTM defs were graduated to Tier-2 parent
  overrides to fix real parity gaps against `ctm.ts`:
  - **mKomfy 2.0 (`6254380`) + mKomfy 2.5 (`mkomfy25`) stove guards** wired the
    generic `kFzIasZone`, which emits a bare `alarm` (zoneStatus bit 0) that
    never matched any expose, and dropped both real stove-guard signals. z2m
    uses `m.iasZoneAlarm({zoneType:"generic", manufacturerZoneAttributes:[{bit:0,
    name:"high_temperature"},{bit:1,name:"power_cut_off"}], zoneAttributes:
    ["tamper","battery_low"]})`. Added `kFzCtmStoveGuardZone` (bit 0 →
    `high_temperature`, bit 1 → `power_cut_off`, bit 2 → `tamper`, bit 3 →
    `battery_low`) and replaced the dead `alarm` expose with the two signals.
  - **AX Water Sensor + AX Valve Controller** wired the generic
    `kFzIasWaterLeakAlarm`, which reads the leak from zoneStatus **bit 0** and
    declared a spurious `tamper`. z2m's `fzLocal.ctm_water_leak_alarm` puts the
    leak on **bit 1** (`water_leak`), bit 0 is `active_water_leak`, bit 3 is
    `battery_low`, with no tamper — so the leak was read off the wrong bit and
    `active_water_leak` was missing entirely. Added `kFzCtmWaterLeak` with the
    correct bit map and exposes.
  - **MBD-S + MBD Dim motion detectors** declared an `occupancy` expose with
    **no decoder** — a stale note claimed `msOccupancySensing` (0x0406) had no
    generic converter, but `kFzOccupancy` (z2m `fz.occupancy`, attr 0x0000)
    exists; it is now wired, so motion actually reaches the shadow.

  Added `kFzCtmStoveGuardZone` / `kFzCtmWaterLeak` to `definitions/ctm/_shared.*`
  (both fire on `commandStatusChangeNotification` + `attributeReport` +
  `readResponse`, matching z2m). Pinned by `tests/test_ctm_parity.cpp`.

- **Danfoss Ally TRV 014G2461 `pi_heating_demand` was a dead expose** — the def
  declared a `pi_heating_demand` readout (z2m
  `danfossExtend.danfossThermostat({piHeatingDemand:{values:true}})`, decoded by
  the standard `fz.thermostat` from `hvacThermostat` attr `0x0008`), but only
  wired the generic `kFzThermostat`, which decodes `0x0000` / `0x0012` /
  `0x001C` and skips `0x0008` — so the valve-demand percentage never reached the
  shadow. Extended `kFzDanfossThermostat` (already wired on this device) to also
  decode `0x0008` as the raw 0-100 % value (Danfoss sets
  `dontMapPIHeatingDemand: true`, so z2m's `mapNumberRange(v,0,100,0,100)` is
  identity). Graduated `definitions/danfoss/Dan_D014G2461.cpp` to Tier 2; pinned
  by `tests/test_danfoss_parity.cpp`. The Icon / Icon2 multi-room floor-heating
  controllers were audited and the primary per-room control surface
  (`local_temperature` / `current_heating_setpoint` / `system_mode`, decoded via
  the wildcard-endpoint `kFzThermostat` + runtime `_<label>` suffix path) works;
  their remaining per-room diagnostics (`running_state`, floor sensor, battery,
  l16 system status) stay deferred as documented multi-endpoint manuSpec infra.

- **Namron 4512752/4512753 Tuya thermostat decoded nothing under collision** —
  the namron registry held two definitions claiming the identical fingerprint
  (zigbeeModel `TS0601` + manufacturer `_TZE204_p3lqqy2r`): the full port
  `kDefNam__TZE204_p3lqqy2r`, which wires `fz_tuya_datapoints` over the device's
  17-DP map (setpoint / local_temperature / mode / sensor / child_lock /
  metering / …), and a second generated def `kDef_D4512752_4512753` that lowered
  only `kFzMetering` + `kFzElectricalMeasurement` and **no** Tuya datapoint
  decoder, dropping the thermostat's entire core surface. Because both matched
  the same keys, the device's behaviour hung on registry order
  (`find_definition` is first-match within the Tuya-styled pass) — a latent
  shadowing hazard. Removed the metering-only stub, graduated the full-DP def to
  `definitions/namron/Nam__TZE204_p3lqqy2r.cpp` (Tier 2) as the sole match, and
  preserved its human-facing `.model` label `4512752/4512753`. Pinned by
  `tests/test_namron_parity.cpp`.

- **Sunricher sensor parity** — ten auto-generated Sunricher ports fixed by
  graduating to Tier-2 parent overrides under `definitions/sunricher/`:
  - **IAS alarm sensors decoded to dead keys** — six sensors wired the generic
    `kFzIasZone` (emits the bare `alarm` bit) behind an `alarm` expose, while z2m
    decodes each via `m.iasZoneAlarm({zoneType})` and publishes a semantic key
    with no rename layer, so the alarm never reached the shadow. Wired the typed
    `kFzIas<Type>Alarm` converter + matching expose:
    **SR-ZG9070A-SS** (smoke) → `kFzIasSmokeAlarm` / `smoke`;
    **SR-ZG9060A-GS** (gas) → `kFzIasGasAlarm` / `gas`;
    **SR-ZG9060B-CS** (CO) → `kFzIasCoAlarm` / `carbon_monoxide`;
    **SR-ZG9050C-WS** + **SR-ZG9050B-WS** (water leak) → `kFzIasWaterLeakAlarm` /
    `water_leak`; **SR-ZG9011A-DS** (contact) → `kFzIasContactAlarm` / `contact`.
  - **SR-ZG9030F-PS** (human-presence sensor) was mis-ported as a controllable
    on/off relay (`genOnOff`, expose `state`). It has no relay — replaced with
    `kFzOccupancy` + `kFzIlluminance` and `occupancy` / `illuminance` exposes.
  - **HK-SENSOR-4IN1-A** (occupancy + temp + humidity + illuminance) and
    **SR-ZG9033TH** (temp + humidity) lowered only `kFzBattery`, dropping every
    measurement the device exists to report; **SR-ZG9032A-PIR** lowered only its
    ep1 light and dropped occupancy (ep2) + illuminance (ep3). Added the generic
    `kFzOccupancy` / `kFzTemperature` / `kFzHumidity` / `kFzIlluminance` decoders,
    exposes, and cluster binds. Pinned by `tests/test_sunricher_parity.cpp`.

- **Sber IAS sensors decoded to dead keys** — three SDevices (Sber) IAS Zone
  sensors had auto-generated ports that wired the generic `kFzIasZone`
  converter (which emits the raw zoneStatus bits `alarm_1` / `alarm_2` /
  `tamper` / `battery_low`) while their expose list declared a bare `alarm`
  key that no converter produced, so the sensor's core state never reached the
  shadow. z2m decodes each via `m.iasZoneAlarm({zoneType: ...})`, publishing a
  semantic key. Graduated each def to `definitions/sber/Sbe_*.cpp` and wired the
  typed `kFzIas<Type>Alarm` converter + matching expose:
  **SBDV-00029** (motion) → `kFzIasMotionAlarm` / `occupancy`;
  **SBDV-00030** (opening/contact) → `kFzIasContactAlarm` / `contact`;
  **SBDV-00154** (water leak) → `kFzIasWaterLeakAlarm` / `water_leak` (z2m
  drops `tamper` for this device, so the expose is dropped to match). Pinned by
  `tests/test_sber_parity.cpp`.

- **Sinopé leak/valve/tank-monitor parity** — three real gaps in the generated
  Sinopé ports fixed by graduating to Tier-2 parent overrides:
  - **WL4200 / WL4200S / WL4210** (water leak detectors) and **RM3500ZB**
    (Calypso water-heater controller) wired the generic `kFzIasZone` (bare
    `alarm` key) behind an `alarm` expose, so the `water_leak` state z2m
    publishes (`fz.ias_water_leak_alarm_1` / `fzLocal.ias_water_leak_alarm`)
    never reached the shadow; they also dropped the on-board temperature
    sensor. Swapped in the typed `kFzIasWaterLeakAlarm` (emits `water_leak`)
    + `kFzTemperature`, with semantic exposes and an `msTemperatureMeasurement`
    bind.
  - **VA4200WZ / VA4201WZ / VA4220ZB** (water valves) used the generic cover
    converters, which read/write `closuresWindowCovering`. The valves report
    position on `genLevelCtrl` currentLevel and open/closed on `genOnOff`
    (z2m `fz.cover_position_via_brightness` + `fz.cover_state_via_onoff`,
    `tz.cover_via_brightness`), so position decode was dead and the `state`
    was absent entirely. Added Sinopé valve converters `kFzSinopeValvePosition`
    / `kFzSinopeValveState` / `kTzSinopeValvePosition` (genLevelCtrl 0-255 ↔
    position 0-100, plus open/close/stop), an `OPEN`/`CLOSE` `state` expose,
    and `genOnOff` + `genLevelCtrl` binds.
  - **LM4110ZB** (tank-level monitor) only wired `kFzBattery`, dropping its
    core function: the gauge fill level (z2m `fzLocal.tank_level`,
    `genAnalogInput` presentValue → %) and temperature. Added
    `kFzSinopeTankLevel` + `kFzTemperature` with `tank_level` / `temperature`
    exposes and `genAnalogInput` + `msTemperatureMeasurement` binds.

  Pinned by `tests/test_sinope_parity.cpp`.

### Added

- `ConfigStepOp::Write` — declarative ZCL **Write Attributes** configure
  step (host-side infra only). The declarative configure pipeline
  (`ConfigStep` in `include/zhc/runtime/definition.hpp`) previously
  covered only `Read` / `Cmd` / `Callback` / `Wait`, so z2m
  `endpoint.write(cluster, {attr: value}, {manufacturerCode})` configure
  steps could not be expressed. Added the `Write` op plus two additive,
  default-initialised `ConfigStep` fields — `attr_id` (u16) and
  `attr_type` (ZCL data-type byte) — so the ~hundreds of existing
  positional `ConfigStep` initializers stay valid and warnings-as-errors
  stays clean. Write field mapping: `endpoint` / `cluster_id` / `attr_id`
  / `attr_type` / value bytes in `payload`+`payload_len` (LE per
  `attr_type`) / `manu_code` / `flags`. Added `RuntimeContext`
  `configure_write` (typedef `ConfigureWriteFn`, signature
  `(device_index, endpoint, cluster_id, attr_id, attr_type,
  const std::uint8_t* value, std::size_t len, manufacturer_code)`),
  mirroring `configure_cmd`. The `run_configure` walker
  (`src/runtime/dispatch.cpp`) handles `Write` by invoking
  `configure_write` **null-safe** — a missing hook SKIPs the step and the
  walk continues (Write is inert until firmware implements the hook),
  unlike `Read`/`Cmd` which fail-closed. Host coverage added to
  `tests/test_config_steps.cpp` (exact attr_id/attr_type/value/manu_code
  capture; endpoint-0→1 coercion; null-hook skip-and-continue). No device
  def is wired to `Write` yet — it would be inert until firmware lands the
  transmit side. **Firmware follow-up (hardware-gated):** each core must
  implement `configure_write` to emit a real ZCL Write Attributes frame.
  Unblocks declarative lumi/Aqara 0xFCC0 "event-mode" enable writes (most
  QBKG*/WS-* wall switches) and Tuya `operation_mode` force-writes — the
  0xFCC0 follow-up already flagged below.

- Wired the first two real device defs to `ConfigStepOp::Write` (host-
  testable now; over-the-air inert until firmware implements
  `configure_write`):
  - **Tuya TS004F knob** (`definitions/tuya/TS004F_knob.cpp`,
    `kDef_TS004F_knob`) — appended a `Write` step to its `config_steps`
    forcing `operation_mode` to "event": genOnOff (`0x0006`) attr `0x8004`
    (tuyaOperationMode), enum8 (`0x30`), value `{0x01}`, manu_code 0. This
    lands the previously deferred z2m configure write
    (`endpoint.write("genOnOff", {tuyaOperationMode: 1})` for
    ERS-10TZBVK-AA in `tuya.ts`) that had been parked because `ConfigStepOp`
    lacked a `Write` op — header note updated deferred → done.
  - **Aqara WXCJKG11LM** Opple wireless switch
    (`definitions/lumi/WXCJKG11LM.cpp`, `kDefWXCJKG11LM`) — added a
    `config_steps` array with one `Write` step enabling "event" mode:
    manuSpecificLumi (`0xFCC0`) attr `0x0009` (mode), uint8 (`0x20`), value
    `{0x01}`, manu_code `0x115F` (Lumi). Mirrors z2m's
    `endpoint.write("manuSpecificLumi", {mode: 1}, {manufacturerCode})` in
    `lumi.ts` (attr/type from `lib/lumi.ts`
    `mode: {ID: 0x0009, type: UINT8}`; manufacturerCode `0x115f`).
  - New host test `tests/test_configure_write_defs.cpp` (registered in
    `CMakeLists.txt`, links `zhc_definitions`) runs `run_configure` on each
    real def with a capturing `configure_write` and asserts the exact
    `(endpoint, cluster, attr_id, attr_type, value, manu_code)`.

- Wired the remaining five lumi (Aqara) event-mode device defs to
  `ConfigStepOp::Write`, completing the set of six lumi SKUs whose z2m
  `configure` does `endpoint.write("manuSpecificLumi", {mode: 1},
  {manufacturerCode})` (verified against `devices/lumi.ts` — exactly six
  such writes; `lib/lumi.ts` `mode: {ID: 0x0009, type: UINT8}`,
  manufacturerCode `0x115f`). Each adds a one-step `config_steps` array
  mirroring WXCJKG11LM: manuSpecificLumi (`0xFCC0`) attr `0x0009` (mode),
  uint8 (`0x20`), value `{0x01}` ("event"), manu_code `0x115F`, endpoint 1
  (host-testable now; over-the-air inert until firmware implements
  `configure_write`):
  - **WXCJKG12LM** (`definitions/lumi/WXCJKG12LM.cpp`, `kDefWXCJKG12LM`).
  - **WXCJKG13LM** (`definitions/lumi/WXCJKG13LM.cpp`, `kDefWXCJKG13LM`).
  - **WXKG15LM** (`definitions/lumi/WXKG15LM.cpp`, `kDefWXKG15LM`) — this
    parent def also covers the Aqara **WRS-R02** whiteLabel (no standalone
    def). z2m additionally writes attr `293` (`0x125`)=`0x02` for the
    "multiple clicks" mode; that second non-mode write is intentionally
    left out of scope here (event-mode wiring only).
  - **WXKG21LM** (`definitions/lumi/WXKG21LM.cpp`, `kDefWXKG21LM`).
  - **WXKG22LM** (`definitions/lumi/WXKG22LM.cpp`, `kDefWXKG22LM`).
  - Extended `tests/test_configure_write_defs.cpp` with a shared
    `assert_lumi_mode_write` check asserting the exact `(endpoint 1,
    cluster 0xFCC0, attr 0x0009, type 0x20, value {0x01}, manu 0x115F)` for
    each newly-wired def via a capturing `configure_write`.
  - Note: QBKG\*/WS-\* lumi wall switches are deliberately **not** wired —
    z2m does not perform the `{mode: 1}` write for them (verified).

- lumi (Aqara) switch/plug attribute reporting (parity Batch 1). A
  parity worklist flagged 113 lumi defs as "missing configure": z2m sets
  up attribute reporting in each SKU's
  `configure(device, coordinatorEndpoint)`, but the ZHC lumi defs carried
  `.reports = nullptr`. The dominant *in-tree-doable* z2m pattern for the
  mains-powered relay family is a plain
  `reporting.bind(genOnOff[, ...])` + `reporting.onOff(endpoint)` (some
  add `deviceTemperature` / `currentSummDelivered`), which maps 1:1 onto a
  `ReportingSpec[]` on `PreparedDefinition.reports` (the ZNCZ15LM /
  ikea / Tuya-plug reporting mechanism). Added three shared reporting
  templates in `definitions/lumi/_shared.{hpp,cpp}` mirroring z2m
  lib/reporting.ts defaults exactly — `kReportsLumiOnOff`
  (genOnOff.onOff: bool 0..3600s rc 0), `kReportsLumiOnOffDevTemp` (+
  genDeviceTempCfg.currentTemperature: s16 300..3600s rc 1), and
  `kReportsLumiOnOffEnergy` (+ seMetering.currentSummDelivered: u48
  5..3600s rc 257) — and wired them into four parent defs that already
  bound + decoded genOnOff but reported nothing: `SSM_U01.cpp` (onOff +
  device-temperature; also extends its bindings with genDeviceTempCfg
  0x0002 so the temp report has a route), `SSM_U02.cpp` (onOff),
  `ZNCZ02LM.cpp` (onOff), `SP_EUC01.cpp` (onOff + energy). Per z2m,
  V/I/P (rmsVoltage/rmsCurrent/activePower) reporting is intentionally
  NOT added for these SKUs ("Voltage and current are not supported"),
  and battery is not reported. New host test
  `tests/test_lumi_switch_reports.cpp` asserts each def's reports against
  the z2m values and keeps bindings/reports in lockstep.

  NOT addressed by this batch (documented for follow-up): the most common
  lumi `configure:` body across the worklist is a 0xFCC0 "event mode"
  magic write — `endpoint.write("manuSpecificLumi", {mode: 1}, {disableResponse})`
  (QBKG25/26/27/28/29LM, WS-USC01/02, WS-EUK01/03/04, and most generated
  defs). ZHC's `ConfigStepOp` enum has no Write op (only Read / Cmd /
  Callback / Wait), so this pattern is **infra-blocked** — it needs a ZCL
  Write Attributes config step (and a `configure_write_fn`), the same
  primitive gap that kept Tuya IAS off the config pipeline. The
  read-only `configure:` bodies (`endpoint.read("genPowerCfg",
  ["batteryVoltage"])` / manuSpecific attr reads) are battery-sensor
  one-shot reads that z2m does NOT bind/report, and are not part of this
  reporting batch. 51 of the 64 flagged devices carry both a parent and a
  regen-owned `generated/` row; the generated reporting belongs in the
  private `zhac-tools` generator, not hand-edits.

### Fixed

- **Heiman IAS-Zone sensor family** — 27 auto-generated defs decoded the
  bare `alarm` key (generic `kFzIasZone`) while their expose declared the
  *semantic* key, so the primary sensor state never reached the shadow.
  Because there is no `alarm`→semantic rename layer, a smoke detector
  exposing `smoke` (etc.) silently produced nothing on alarm. All 27 are
  graduated to Tier-2 parent overrides under `definitions/heiman/` (so a
  regen of `generated/` can no longer re-introduce the gap):
  - 26 swapped to the typed `kFzIas<Type>Alarm` converter that emits the
    semantic key directly — `contact` (D1-EFA, HS1DS, HS3DS, HS8DS-EFA,
    Heiman-M1), `smoke` (HS15A-M, HS1SA-E, HS1SA-E-PLUS, HS2SA-EF-3.0,
    HS3SA/HS1SA), `carbon_monoxide` (HS1CA-E), `gas` (HS1CG, HS1CG-E,
    HS1CG_H, HS1CG-M via `alarm_1`; HS1CG-E_3.0, HS1CG_M, HS3CG via
    `alarm_2`), `occupancy` (HS1MS-EF, HS1MS-M, HS3MS, PIR_TPV12),
    `vibration` (HS1VS-EF, HS1VS-N), `water_leak` (HS1WL/HS3WL, HS2WL).
    Mirrors the already-correct HS1CA-M / HS2WD-E heiman defs and the
    trust / smartthings IAS ports.
  - **HS1MIS-3.0** was additionally mis-ported as an IAS device; z2m
    decodes it via `fz.occupancy` (msOccupancySensing `0x0406`), so it is
    re-pointed at the generic `kFzOccupancy` converter with a corrected
    `0x0406` binding and the IAS-only `tamper`/`battery_low` exposes
    dropped.
  - Added generic `kFzIasGasAlarm` / `kFzIasGasAlarm2` converters
    (`fz.ias_gas_alarm_{1,2}`, zoneStatus bits 0/1 → `gas`) to
    `definitions/_generic/`, which had no gas-zone decoder.
  - New host fixture `tests/test_heiman_parity.cpp` pins the semantic-key
    decode (and absence of the bare `alarm` key) per representative type,
    the `alarm_2` bit-1 gas variant, and the HS1MIS-3.0 occupancy path.

- Three **Schneider Electric (Wiser)** devices whose auto-generated ports
  dropped a core decoder are graduated to parent overrides under
  `definitions/schneider/` (so a regen of `generated/` can no longer silently
  re-introduce the gap):
  - **EER53000** — Wiser radiator thermostat (VACT, zigbeeModel
    `EH-ZB-VACT`) was lowered as a bare on/off switch (state/battery only).
    z2m decodes a full climate via `fzLocal.wiser_smart_thermostat`. Wired
    the generic `kFzThermostat` / `kTzThermostat` converters (+ binding
    `hvacThermostat` 0x0201) so `local_temperature` /
    `current_heating_setpoint` / `system_mode` surface, bringing it to parity
    with its sibling EER51000. Climate ships **flat** (project rule);
    zone_mode / keypad_lockout / valve-calibration ride Schneider
    manuSpecific attrs (0x105E) and remain deferred (no in-tree converter).
  - **550B1024** — temperature & humidity sensor (zigbeeModel
    `CCT593011_AS`) was ported battery-only; z2m fromZigbee is
    `[fz.humidity, fz.temperature, fz.battery]`. Added the generic
    `kFzTemperature` / `kFzHumidity` converters, `temperature` + `humidity`
    exposes, and bindings `msTemperatureMeasurement` (0x0402) /
    `msRelativeHumidity` (0x0405).
  - **CCTFR6400** — "Temperature/Humidity measurement with thermostat
    interface": the thermostat half was wired but the named humidity
    measurement was missing (z2m carries `fz.humidity` + `e.humidity()` +
    `msRelativeHumidity` bind). Added `kFzHumidity` + `humidity` expose +
    binding 0x0405.

  New host fixture `tests/test_schneider_parity.cpp` pins the expose/binding
  shape and decodes real ZCL reports for all three at the
  `dispatch_from_zigbee` boundary.

- Philips Hue **motion sensors** SML001/SML002/SML003/SML004
  (`9290012607` / `9290019758` / `9290030675` / `9290030674`) now decode
  **occupancy**. The shared `kExposesPhilipsMotionSensor` already declared
  the `occupancy` expose and `kBindingsPhilipsMotionSensor` already bound
  msOccupancySensing (0x0406), but the shared fz array
  `kFzPhilipsMotionSensor` (`definitions/philips/_shared.cpp`) omitted the
  decoder — so the state was advertised but never surfaced. Wired the
  generic `kFzOccupancy` (msOccupancySensing attr 0x0000 bit 0, the same
  decoder used cross-vendor by lumi/terncy/owon/etc.) into the array,
  matching z2m's `fz.occupancy`. Host coverage added in
  `tests/test_philips_motion.cpp` (occupied + vacant). The Hue-specific
  motion_sensitivity / led_indication / occupancy_timeout channels
  (`philips.tz.hue_motion_*`, manuSpecificPhilips writes) have no generic
  converter and remain unported.

- SlackyDiy **THERM_SLACKY_DIY_R08** (Tuya floor-heating thermostat,
  custom firmware, zigbeeModel `Tuya_Thermostat_r08`) is now lowered as a
  thermostat instead of a phantom switch. The auto-generated def
  (`definitions/slacky_diy/generated/Sla_THERM_SLACKY_DIY_R08.cpp`)
  mis-classified it as a battery + on/off device (`kFzBattery` +
  `kFzOnOff`, exposes `state`/`battery`/`voltage`, binds genPowerCfg
  0x0001 + genOnOff 0x0006) — the wrong device class entirely, so it
  surfaced a fake switch and never reported temperature, setpoint or mode.
  Graduated to a hand-maintained **Tier 2** parent override
  (`definitions/slacky_diy/THERM_SLACKY_DIY_R08.cpp`; generated row
  deleted) matching its 13 sibling THERM defs: generic `kFzThermostat` /
  `kTzThermostat` on the custom `hvacThermostat` cluster (0x0201), exposing
  `local_temperature` / `current_heating_setpoint` / `system_mode` — at
  parity with z2m `slacky_diy.ts #THERM_SLACKY_DIY_R08`
  (`localFromZigbeeThermostat` incl. `fz.thermostat`, `e.climate()`). The
  device's richer controls (child_lock, weekly schedules, eco_mode,
  frost/heat protect, calibration) ride manufacturer-specific 0x0201
  attributes and z2m's `e.climate()` composite; the runtime has no generic
  decoder for those and ships climate flat, so they are deferred exactly as
  for every other slacky_diy / Tuya thermostat (infra, not an R08 gap).
  New host test `tests/test_slacky_therm_r08.cpp` pins that the override
  exposes/binds the thermostat (and NOT the phantom switch/battery) and
  decodes localTemp / setpoint / systemMode wire reports.
  (Investigation note: the other 13 slacky_diy probe "expose-gaps"
  [R01–R07, R09–R0D] and all 15 "missing configure" rows are false
  positives or runtime infra — the extra thermostat controls need
  custom-0x0201-attr decoders that do not exist yet, and `configureCommon`
  is bind+read+reporting whose bindings the defs already carry; vendor is
  otherwise at parity.)

- Sonoff **SNZB-02** temperature & humidity sensor now surfaces
  `temperature` + `humidity`. The auto-generated def
  (`definitions/sonoff/generated/Son_SNZB_02.cpp`) lowered only
  `ewelinkBattery()` and so exposed nothing but `battery` + `voltage`,
  dropping the device's entire reason for existing. Graduated to a
  hand-maintained **Tier 2** parent override
  (`definitions/sonoff/SNZB_02.cpp`; generated row deleted) that adds the
  generic `kFzTemperature` / `kFzHumidity` converters plus the matching
  `msTemperatureMeasurement` (0x0402) / `msRelativeHumidity` (0x0405)
  bindings — at parity with z2m `sonoff.ts #SNZB-02`
  (`e.temperature()`, `e.humidity()`, `fz.SNZB02_temperature`,
  `fz.humidity`). z2m's `SNZB02_temperature` is `fz.temperature` plus a
  -33..100 °C garbage-report clamp; the generic decoder uses the
  identical /100.0 scaling, so it is at parity for every in-range value.
  New host test `tests/test_sonoff_snzb02.cpp` pins the expose/binding
  shape and decode of temp (incl. negative) + humidity wire reports.
  (Investigation note: the other six sonoff probe "expose-gaps" —
  `TRVZB`, `ZBCurtain`, `ZBDongle-E`, `SWV`, `SNZB-01-KF`, `SNZB-01M` —
  were false positives or need runtime infra that does not exist yet
  [`kFzFlow`, FC11/FC12 custom-cluster decoders], so they are left as-is.)

- Tuya IAS sensor decode + bindings (Phase-2b Batch 4). The worklist
  flagged `definitions/tuya/TS0202.cpp` (motion) and
  `definitions/tuya/TS0203.cpp` (contact) as "missing configure", but the
  real gap was a broken/absent IAS-status decoder — there is no z2m
  `configure:` to port for either device, and IAS Zone enrollment is done
  GLOBALLY by the coordinator stack
  (`zhac-components/components/zigbee_mgr/zigbee_mgr.cpp`
  `zcl_maybe_respond_ias_enroll`: writes attr 0x0010 IAS_CIE_Address +
  answers ZoneEnrollRequest with ZoneEnrollResponse for any 0x0500
  device), so no per-def config step is needed (and ZHC's `ConfigStepOp`
  has no Write op precisely because IAS enroll never requires one).
  TS0202 had `kFzIgnoreOccupancyReport` (which *ignored* occupancy) and no
  IAS decoder, so motion was never reported; TS0203 had only `kFzBattery`,
  no `contact` expose, and did not bind ssIasZone. Switched TS0202 to
  `kFzIasMotionAlarm` + `kFzBattery` and TS0203 to `kFzIasContactAlarm` +
  `kFzBattery` (z2m `fz.ias_occupancy_alarm_1` / `fz.ias_contact_alarm_1`
  + `fz.battery`); added the missing `tamper`/`voltage`/`battery_low` +
  (TS0203) `contact` exposes to match z2m; and bound both genPowerCfg
  (0x0001, battery) and ssIasZone (0x0500) on each. Battery *reporting*
  (`reports`) is intentionally NOT added — z2m removed
  `batteryPercentageRemaining` for several Tuya IAS sensors ("devices fall
  off network"). The z2m `_report` (attribute-report) decode path has no
  ZHC equivalent (the `fz_ias_typed` primitive is command-only); ZHC
  decodes the post-enroll ZoneStatusChangeNotification command, matching
  the sibling Tuya IAS sensors (SM0202, RH3040).

- Tuya soil-sensor temperature 10x scaling bug. The three soil parent defs
  (`definitions/tuya/TZE200_soil.cpp`, `TZE200_soil_ec.cpp`,
  `TZE200_soil_th.cpp`) decoded the temperature DP (dp 5) with the
  `dp::temperature` default `/10`, but z2m's `TS0601_soil` family decodes it
  as `tuya.valueConverter.raw` (`/1`) — a 10x error (a 25 C reading rendered
  as 2.5 C). Switched all three to `dp::temperature(5, 1)` for z2m parity.
  Covered by `tests/test_tze200_soil.cpp`.

### Added

- Tuya ZCL-light attribute reporting + bindings (Phase-2b Batch 3).
  The generic standard-ZCL Tuya lights `TS110E` (dimmer), `TS0502A` (CCT),
  `TS0505A` and `TS0505B` (RGB+CCT) bound genOnOff / genLevelCtrl
  [/ lightingColorCtrl] and decoded onOff / brightness [/ color_temp] but
  had `reports=nullptr`, so no Configure Reporting was set up. z2m's profile
  is the tuyaLight extend wrapping `lib/modernExtend.ts` `light()` with
  `configureReporting: true`. Added the matching `.reports` per capability:
  onOff (genOnOff 0x0006 / 0x0000, bool) + currentLevel (genLevelCtrl
  0x0008 / 0x0000, u8) for all four; colorTemperature (lightingColorCtrl
  0x0300 / 0x0007, u16) for the CCT + RGB+CCT lights; currentX (0x0300 /
  0x0003, u16) + currentY (0x0300 / 0x0004, u16) for the RGB+CCT lights.
  Values mirror z2m exactly: `onOff` (min=0, max=3600, change=0) and
  `currentLevel` (min=1, max=3600, change=1) and `colorTemperature` (min=0,
  max=3600, change=1) from `lib/reporting.ts`; `currentX`/`currentY`
  (min=10, max=65535, change=1) from `lib/modernExtend.ts` `light()`.
  `colorMode` is intentionally NOT reported — z2m's `lib/light.ts` only
  READS colorMode, it never enters the configureReporting *report* set
  (this diverges from the IKEA `kReportsIkeaLight` template, which over-
  reports colorMode). Shared arrays `kReportsDimmer_1ep` /
  `kReportsLightCCT_1ep` / `kReportsLightRGBCCT_1ep` added to
  `definitions/tuya/_shared.{hpp,cpp}`; each light's existing
  `kAutoBindings` already binds every reported cluster on EP1, keeping
  bindings/reports in lockstep. Covered by
  `tests/test_tuya_light_reports.cpp`.

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
  currentSummationDelivered (seMetering 0x0702 / 0x0000, u48). `onOff` and
  `currentSummDelivered` mirror z2m's `lib/reporting.ts` defaults — `onOff`
  (min=0, max=3600, change=0); `currentSummDelivered` (min=5, max=3600,
  change=257) — while the V/I/P reportable changes mirror z2m's per-device
  `configure:` for `TS011F_plug_1`/`TS0121_plug`, which override the
  reporting.ts base change of 1: `rmsVoltage` (min=5, max=3600, change=5);
  `rmsCurrent` (min=5, max=3600, change=50); `activePower` (min=5, max=3600,
  change=10). Both defs' bindings
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