# Changelog

All notable changes to **embedded-zhc** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to the `vYYYYMMDDVV` versioning scheme used
across the ZHAC platform.

## [Unreleased]

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