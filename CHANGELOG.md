# Changelog

All notable changes to **embedded-zhc** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to the `vYYYYMMDDVV` versioning scheme used
across the ZHAC platform.

## [Unreleased]

### Fixed

- **CEL (Cortet) CGW-Z-0010 range extender: phantom on/off control.** z2m wires
  this MeshConnect "Z10" router as a pure repeater
  (`fromZigbee:[fz.linkquality_from_basic]`, `toZigbee:[]`, `exposes:[]`;
  linkquality is stack-level metadata, not a device expose). The Tier-1
  auto-port mis-ported it as a controllable on/off switch — a phantom `state`
  expose plus `kFzOnOff`/`kTzOnOff` and a genOnOff (0x0006) bind — a dead
  control the extender never speaks. Graduated to a Tier-2 override stripped to
  the canonical router shape: no converters, no exposes, no bindings.

- **Atsmart Z6 3-gang wall switch: missing per-endpoint bindings + collapsed
  exposes.** z2m drives the Z6 with `m.deviceEndpoints({left:1,center:2,
  right:3})` + `m.onOff({endpointNames:["left","center","right"]})`, binding
  genOnOff (0x0006) on all three endpoints. The Tier-1 auto-port bound 0x0006
  on endpoint 1 only (so the center/right gangs never had reporting
  configured) and collapsed the three per-gang exposes into a single bare
  `state` — but with the endpoint_map present, dispatch suffixes inbound onOff
  keys to `state_<label>`, so the bare expose matched no runtime key. Graduated
  to a Tier-2 override that binds 0x0006 on endpoints 1/2/3 and declares
  `state_left`/`state_center`/`state_right`.
- **Letsled HLC929-Z-RGBW-4C-IA-OTA-3.0 RGBW down light: dropped color +
  colorTemp axes.** z2m models this as `m.light({colorTemp:{range:[153,370]},
  color:true})` (RGBW with both a CCT/mireds axis and an xy/hs color axis), but
  the auto-generator collapsed `m.light()` to on/off + brightness only, dropping
  the entire lightingColorCtrl (0x0300) channel. Graduated to a Tier-2 override:
  restored `kFzColorTemperature`+`kFzColor` / `kTzColorTemp`+`kTzColor`, the
  `color_temp`/`color_x`/`color_y`/`hue`/`saturation` exposes, and the 0x0300
  binding.

- **Enkin ZDM150 dimmer: mis-classified as battery-only on/off, brightness
  dropped.** z2m drives this 150W dimmer with `m.light()` +
  `m.forcePowerSource("Mains (single phase)")`, giving on/off + brightness
  (genLevelCtrl 0x0008) plus the matching writers. The auto-port wired
  `kFzBattery` + phantom `battery`/`voltage` exposes + a genPowerCfg (0x0001)
  binding and dropped brightness entirely. Graduated to a Tier-2 override:
  `kFzOnOff`+`kFzBrightness` / `kTzOnOff`+`kTzBrightness`, `state`+`brightness`
  exposes, bindings on genOnOff (0x0006)+genLevelCtrl (0x0008), no phantom
  battery, and `power_source_override` = Mains (single phase).
- **Eaton/Halo LED RL460WHZHA69 downlight: dropped colorTemp axis.** z2m drives
  this "Wireless Controlled LED retrofit downlight" (zigbeeModel `Halo_RL5601`)
  with `m.light({colorTemp:{range:[200,370]}})`, i.e. a CCT-only (tuneable white)
  bulb. The auto-generator collapsed `m.light()` to on/off + brightness only, so
  `color_temp` was both unexposed and undecoded (no `lightingColorCtrl` 0x0300
  binding). Graduated to a Tier-2 override restoring the `color_temp` expose, the
  0x0300 binding, the `kFzColorTemperature` decoder and the `kTzColorTemp` write
  path. No xy/hs color axis (z2m has no `color:{}`).
- **eZEX ECW-100-A03 3-gang switch: only the top gang was bound at join.** z2m
  drives it with `m.deviceEndpoints({top:1,center:2,bottom:3})` +
  `m.onOff({endpointNames:["top","center","bottom"]})`, whose configure binds
  genOnOff on every endpoint exposing the cluster (1/2/3). The auto-port bound
  only `{1,0x0006}`, so the center/bottom gangs were never bound and never
  reported state changes. Graduated to a Tier-2 override binding all three
  endpoints. The `{top,center,bottom}` `endpoint_map` was already present and
  correctly suffixes decoded `state` → `state_top`/`state_center`/`state_bottom`.

- **Sowilo DS L258 "Heimdall Pro" RGBWW controller: dropped colorTemp + colour
  axis.** z2m drives it with `m.light({colorTemp:{range:[150,575]},
  color:{modes:["xy","hs"]}})` (full RGB + tuneable white), but the
  auto-generated port wired only on/off + brightness, dropping the entire
  lightingColorCtrl (0x0300) channel. Graduated to a Tier-2 override that
  restores the `color_temp` / `color_xy` exposes, the colorTemp + colour
  converters, and the 0x0300 binding. Pinned by `test_sowilo_parity.cpp`.
- **EZVIZ CS-T2C open/close sensor: IAS dead-key + inverted contact.** The
  Tier-1 auto-port wired the generic `kFzIasZone`, which emits a bare,
  non-inverted `alarm` boolean. z2m ships this device via
  `m.iasZoneAlarm({zoneType:"contact", zoneAttributes:["alarm_1","alarm_2",
  "tamper","battery_low"]})`: `zoneType:"contact"` inverts the payload and both
  alarms present take z2m's `bothAlarms` branch, publishing two semantic INVERTED
  keys `contact_alarm_1` (=!bit0) + `contact_alarm_2` (=!bit1) plus tamper /
  battery_low. The auto-port's `alarm` key was both the wrong name and the wrong
  polarity. Added reusable two-key inverted contact converters
  `kFzIasContactAlarm1`/`kFzIasContactAlarm2` to `_generic/_shared` and graduated
  the def to a Tier-2 override using them, with matching `contact_alarm_1`/
  `contact_alarm_2` exposes.
- **Nexelec Open'R air-quality sensor: dropped temperature/humidity/CO2
  channels.** z2m drives it with `[m.temperature(), m.humidity(), m.co2(),
  m.battery(), m.identify()]` (msTemperatureMeasurement 0x0402 /100,
  msRelativeHumidity 0x0405 /100, msCO2 0x040D ppm, genPowerCfg 0x0001), but
  the auto-port wired ONLY `kFzBattery`, reducing the sensor to battery-only.
  Graduated to a Tier-2 override that re-wires `kFzTemperature` +
  `kFzHumidity` + `kFzCO2`, adds the `temperature`/`humidity`/`co2` exposes,
  and binds the three measurement clusters.
- **Direct Signs DS-Z-001DE RGB+CCT LED controller: dropped colorTemp + colour
  axis.** The `m.light({colorTemp:{range:[158,500]}, color:{modes:["xy","hs"],
  enhancedHue:true}})` def was auto-ported to on/off + brightness only, dropping
  the entire lightingColorCtrl (0x0300) channel (no color/color_temp decode,
  control, or binding). Graduated to a Tier-2 override that wires
  `kFzColorTemperature`/`kFzColor` + `kTzColorTemp`/`kTzColor`, adds the
  `color_temp` (range [158,500] mired) + `color_xy` exposes, and binds 0x0300.

- **Task Lighting T-TWAV-60W-PSR (WAV Smart Receiver): dropped colorTemp +
  colour axis.** The `m.light({colorTemp:{range:[150,500]}, color:true})` def was
  auto-ported to on/off + brightness only, dropping the entire lightingColorCtrl
  (0x0300) channel. Restored `color_temp` plus the xy colour surface
  (`color_x`/`color_y`; `color:true` defaults to `modes:["xy"]`, so no
  hue/saturation) via the generic `kFzColorTemperature`/`kFzColor` (+ matching
  Tz) converters and the 0x0300 binding. Graduated to Tier 2.
- **TIS Control BEE PIR 1 (ED6XX): IAS occupancy dead-key.** The auto-port
  lowered the generic `kFzIasZone` converter (bare `alarm` key) while z2m's
  `m.iasZoneAlarm({zoneType:"occupancy", zoneAttributes:["alarm_1","tamper",
  "battery_low"]})` (no `alarm_2`, so `bothAlarms=false`) publishes the semantic
  `occupancy` key from zoneStatus bit0. The expose was also mis-named `alarm`,
  so the motion state never reached the shadow. Graduated to a Tier-2 override
  swapping in `kFzIasMotionAlarm` (bare `occupancy` from bit0 + `tamper` bit2 +
  `battery_low` bit3) and renaming the expose `alarm` -> `occupancy`. Battery
  (`%`/`voltage` via `kFzBattery`) was already wired and is preserved.
- **TCL P2-WT water-leak sensor: dead IAS alarm key.** The auto-port wired the
  generic `kFzIasZone` (which emits a bare `alarm` key) against an `alarm`
  expose, so z2m's semantic `water_leak` key never populated. z2m drives it with
  `m.iasZoneAlarm({zoneType:"water_leak", zoneAttributes:["alarm_1","tamper",
  "battery_low"]})`, which (single alarm bit) publishes the bare `water_leak`
  key (zoneStatus bit 0, not inverted) + `tamper` + `battery_low`. Graduated to
  a Tier-2 override: repointed to the typed `kFzIasWaterLeakAlarm` converter and
  renamed the expose `alarm` → `water_leak`.

- **Silicon Labs RB-ElectricityDsp-061-3 metering plug: dropped the 0x0B04
  half.** z2m wires `m.onOff()` + `m.electricityMeter()`, and
  `electricityMeter()` defaults to `cluster:"both"` — energy from seMetering
  (0x0702) plus power/voltage/current from haElectricalMeasurement (0x0B04,
  `fz.electrical_measurement`). The auto-port wired only `kFzMetering` (0x0702)
  and exposed state/energy/power, dropping the 0x0B04 decoder, the
  voltage/current exposes, and the 0x0B04 binding, so RMSVoltage/RMSCurrent
  reports were never decoded. Graduated to a Tier-2 override that adds
  `kFzElectricalMeasurement`, the `voltage`+`current` exposes, and the 0x0B04
  binding.
- **Gumax LST103 lighting system: dropped colorTemp + colour axis.** The
  `m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"],
  enhancedHue:true}})` def was auto-ported to on/off + brightness only, dropping
  the entire lightingColorCtrl (0x0300) channel. Graduated to a Tier-2 override
  restoring `kFzColorTemperature`+`kFzColor` /`kTzColorTemp`+`kTzColor`, the
  `color_temp`/`color_x`/`color_y`/`hue`/`saturation` exposes, and the 0x0300
  binding. Covered by `tests/test_gumax_parity.cpp`.
- **GMY Smart Bulb B07KG5KF5R ("CCT box"): dropped colorTemp axis.** The
  `m.light({colorTemp:{range:undefined}})` def was auto-ported to on/off +
  brightness only, dropping the lightingColorCtrl (0x0300) colorTemperature
  channel. Graduated the def and restored `kFzColorTemperature` /
  `kTzColorTemp`, the `color_temp` expose, and the 0x0300 binding (CCT-only,
  no phantom colour axis since `color` is unset). Added `test_gmy_parity.cpp`.
- **Gidealed ZC05M (model "A11") RGB LED strip controller: dropped colour +
  colour-temperature axes.** z2m drives it with
  `m.light({colorTemp:{range:[153,370]}, color:{modes:["xy","hs"]}})` (full
  RGB+CCT), but the auto-port wired only on/off + brightness and bound only
  0x0006 + 0x0008, silently dropping every colour / colour-temperature report.
  Graduated to a Tier-2 override restoring `kFzColor`+`kFzColorTemperature` /
  `kTzColor`+`kTzColorTemp`, the `color_temp` (153–370 mired) + `color_xy`
  (hue/saturation/color_x/color_y) surface, and the lightingColorCtrl (0x0300)
  binding to match z2m.

- **Alchemy AL8TC13W-AP / AL8RGB13W-AP downlights: dropped colorTemp + colour
  axis.** Both `m.light()` defs were auto-ported to on/off + brightness only,
  dropping the entire lightingColorCtrl (0x0300) channel. z2m drives the
  AL8TC13W-AP with `m.light({colorTemp:{range:[153,370]}})` (tuneable white) and
  the AL8RGB13W-AP with `m.light({colorTemp:{range:[153,370]}, color:true})`
  (RGBW). Graduated both to Tier-2 overrides: TC restores `kFzColorTemperature`
  /`kTzColorTemp` + the `color_temp` expose; RGB restores
  `kFzColor`+`kFzColorTemperature` /`kTzColor`+`kTzColorTemp` + the `color_temp`
  /`color_xy` (hue/saturation/color_x/color_y) surface, plus the 0x0300 binding.
- **Jiawen K2RGBW01 RGBW bulb: dropped color + colorTemp axes.** z2m wires this
  E27 9W bulb as `m.light({colorTemp: {range: undefined}, color: true})`, i.e. an
  RGBW bulb with both a CCT (mireds) axis and an xy-color axis. The auto-generator
  collapsed `m.light()` to on/off + brightness only — `color_temp` and the xy
  color channels were unexposed and undecoded (no `lightingColorCtrl` 0x0300
  binding). Graduated to a Tier-2 override restoring the `color_temp` + `color_xy`
  exposes, the 0x0300 binding, the `kFzColorTemperature` + `kFzColor` decoders and
  the `kTzColorTemp` + `kTzColor` write paths. Sibling JW-A04-CT (plain
  `m.light()`) is correctly on/off + brightness only and stays unchanged.
- **Hoftronic HD300W-ZB rotary LED dimmer: dropped `power_on_behavior` axis.**
  z2m wires this dimmer as `m.light({powerOnBehavior:true, ...})`, which adds a
  `power_on_behavior` enum expose and the `fz.power_on_behavior` /
  `tz.power_on_behavior` converters (genOnOff 0x4003 `startUpOnOff`,
  off/on/toggle/previous). The Tier-1 auto-port collapsed the device to bare
  on/off + brightness, dropping the entire power-on-behavior axis. Graduated
  the def and restored read+write via generic `kFzPowerOnBehavior` /
  `kTzPowerOnBehavior1` plus the enum expose. Verified the FALSE colour
  suspect: z2m `m.light()` is called without `color`/`colorTemp` args, so there
  is no lightingColorCtrl 0x0300 axis — none added (asserted absent in test).
- **Tapestry THPZ1 sensor: temperature + humidity + occupancy channels dropped, mis-ported as an IAS-zone stub.**
  z2m (`tapestry.ts`) wires `fz.temperature` + `fz.humidity` + `fz.occupancy`
  and exposes occupancy/temperature/humidity. The auto-port instead wired the
  generic `kFzIasZone` (cluster 0x0500) with a phantom alarm/tamper/battery_low
  bundle, reducing a tri-channel sensor to a dead IAS stub. Re-ported to the
  three generic measurement decoders (`kFzTemperature` 0x0402, `kFzHumidity`
  0x0405, `kFzOccupancy` 0x0406) with matching exposes and binds.
- **Smart Home Pty HGZB-07A RGBW downlight: dropped colour + colour-temperature
  axes.** z2m models it as `m.light({colorTemp: {range: undefined}, color: true})`
  (on/off + level + colorTemp + colour), but the auto-generator collapsed
  `m.light()` to on/off + brightness only, dropping the lightingColorCtrl
  (0x0300) channels. Graduated to a parent override that restores
  `kFzColorTemperature`/`kTzColorTemp` + `kFzColor`/`kTzColor`, the
  `color_temp`/`color_x`/`color_y`/`hue`/`saturation` exposes and the 0x0300
  binding. The HGZB-20-DE plug (`m.onOff()`) is verified clean.
- **Roome HSC1-WD-0 contact sensor: IAS dead-key.** The auto-port lowered the
  generic `kFzIasZone` converter (bare `alarm` key) while z2m's
  `fz.ias_contact_alarm_1` publishes the semantic `contact` key (inverted:
  `contact = !(zoneStatus bit0)`) plus tamper/battery_low — so contact state
  never reached the shadow. Graduated the def to Tier 2, swapping in
  `kFzIasContactAlarm` and renaming the dead `alarm` expose to `contact`.

- **Quotra QV-RGBCCT (B07JHL6DRV) RGB WW LED strip: dropped colour + colour-temperature axes.**
  z2m defines this strip as `m.light({colorTemp:{range:[150,500]}, color:true,
  powerOnBehavior:false})` — a full RGB + CCT light. The auto-generated def
  reduced it to a plain on/off + brightness dimmer (exposes state/brightness
  only). Graduated to a Tier-2 override that restores the colour surface:
  `kFzColorTemperature` + `kFzColor` decoders, `kTzColorTemp` + `kTzColor`
  writers, the `color_temp`/`color_xy` exposes, and the lightingColorCtrl
  (0x0300) bind. The B07CVL9SZF plain dimmer is verified colourless. Pinned by
  `tests/test_quotra_parity.cpp`.
- **Oujiabao CR701-YZ gas + carbon-monoxide alarm: dead IAS keys.** The
  auto-port lowered the generic `kFzIasZone` (bare `alarm` key) plus a phantom
  `alarm` expose, while z2m wires `fz.ias_carbon_monoxide_alarm_1_gas_alarm_2`
  which decodes one ssIasZone notification into `carbon_monoxide` (zoneStatus
  bit 0) and `gas` (bit 1). Neither key ever reached the shadow. Graduated to a
  Tier-2 override wiring both typed converters — `kFzIasCoAlarm` (bit 0) and
  `kFzIasGasAlarm2` (bit 1) — and aligned exposes to
  carbon_monoxide/gas/tamper/battery_low.
- **Shyugj S901D-ZG door sensor: wrong IAS key + dropped alarm_2 channel.**
  z2m wires `m.iasZoneAlarm({zoneType:"generic", zoneAttributes:["alarm_1",
  "alarm_2","tamper","battery_low"]})`, which keeps the bare zone keys and, with
  both alarms requested, exposes BOTH alarm bits: `alarm_1` = zoneStatus bit0,
  `alarm_2` = bit1, `tamper` = bit2, `battery_low` = bit3. The auto-port wired
  the generic `kFzIasZone`, which emits a single `alarm` (bit0) + tamper +
  battery_low — the WRONG key (`alarm` vs `alarm_1`) AND dropping `alarm_2`
  (bit1) entirely. Graduated to a Tier-2 override wiring
  `kFzIasZoneStatusChange` (emits alarm_1/alarm_2/tamper/battery_low) and
  declaring both bare alarm exposes.
- **Scan Products 12501 push dimmer + 12502 switch: dropped power_on_behavior.**
  z2m wires 12501 as `m.light({configureReporting:true})` and 12502 as
  `m.onOff()`; both modernExtend helpers default `powerOnBehavior=true`, adding
  `fz.power_on_behavior` (genOnOff 0x4003 startUpOnOff enum8) + `tz.power_on_behavior`
  + a `power_on_behavior` enum expose. The Tier-1 auto-ports wired only on/off
  (+ brightness on 12501), dropping that axis. Graduated both to Tier-2 overrides
  restoring it via `kFzPowerOnBehavior` / `kTzPowerOnBehavior1` + the expose. The
  flagged "dropped color/CT" suspect is FALSE: the bare `m.light()` call passes no
  color/colorTemp args, so z2m never adds the lightingColorCtrl 0x0300 axis (no
  color expose is correct). Pinned by `test_scanproducts_parity`.

- **KAMI (Yi) N20 contact/motion sensor: phantom on/off + dead IAS decode.**
  The auto-port wired a phantom on/off (`kFzOnOff` + `kTzOnOff`, `genOnOff`
  0x0006 binding, controllable `state` expose) plus the generic `kFzIasZone`
  (alarm/tamper/battery_low keys) — but z2m has no on/off and the device runs
  non-standard Yi firmware. z2m's `fz.KAMI_contact` reads the raw `ssIasZone`
  frame byte 7 (`{contact: data[7] === 0}`) and `fz.KAMI_occupancy` reads the
  raw `msOccupancySensing` frame byte 7 (`{action: "motion"}` when
  `data[7] === 1`). Graduated `Kam_N20` to a Tier-2 parent, added vendor
  converters `kami::kFzKamiContact` / `kami::kFzKamiOccupancy`
  (`definitions/kami/_shared.{hpp,cpp}`), and fixed exposes to `contact` +
  `action: ["motion"]` (no TZ, no genOnOff binding).
- **Matcall BV LED dimmer drivers (ZG401224, ZG430700): dropped
  power_on_behavior axis.** Both are z2m `m.light()` with no arguments, which
  defaults `powerOnBehavior=true` — wiring `fz.power_on_behavior` (genOnOff
  0x4003 startUpOnOff enum8) + `tz.power_on_behavior` + a `power_on_behavior`
  enum expose. The Tier-1 auto-ports wired only `kFzOnOff` + `kFzBrightness`,
  dropping the entire power_on_behavior axis. Graduated both to Tier-2 overrides
  restoring it via `kFzPowerOnBehavior` / `kTzPowerOnBehavior1` + the expose.
  The suspected dropped color/colorTemp axis is a FALSE flag: `m.light()` adds
  the lightingColorCtrl 0x0300 axis only when called with color/colorTemp args,
  and these plain dimmer drivers pass neither — no colour axis exists to drop.
- **Brimate FZB8708HD-S1 motion sensor: IAS dead-key (motion never reached the
  shadow).** z2m wires `fz.ias_occupancy_alarm_1` and exposes `e.occupancy()` +
  `e.battery_low()`, decoding `occupancy` from zoneStatus bit 0 (plus `tamper`
  bit 2 / `battery_low` bit 3). The auto-port lowered the generic `kFzIasZone`,
  which emits a bare `alarm` key, and declared an `alarm` expose — so motion
  events landed under a key the SPA/shadow never reads. Graduated to a Tier-2
  override using the typed `kFzIasMotionAlarm` (`kLbl_Motion` = "occupancy",
  bit 0) and renamed the dead `alarm` expose to `occupancy`.
- **DQSmart dqhome.re4 4-gang switch: reporting bound only on gang 1.**
  The auto-port bound genOnOff (0x0006) on endpoint 1 only, so gangs
  l2/l3/l4 never had reporting configured and would not push their state
  changes. z2m's `m.onOff({endpointNames:["l1".."l4"]})` runs
  `setupAttributes` per endpoint carrying the cluster (`getEndpointsWithCluster`
  → ep1..ep4), binding 0x0006 on every gang. Graduated to a Tier-2 override
  binding 0x0006 on all four endpoints. Decode was already correct (the
  endpoint_map suffixes `state` → `state_l<n>` by source endpoint); only the
  binding table was short. Pinned by `tests/test_dqsmart_parity.cpp`.
- **Oz Smart Things DL41-03-10-R-ZB RGBW downlight: ported as a battery/on-off
  sensor stub.** z2m wires this TS0505B device as
  `tuya.modernExtend.tuyaLight({colorTemp:{range:[153,500]}, color:true})` →
  `modernExtend.light()` with both the colorTemp and color axes → on/off +
  brightness + colorTemp (mireds) + color (xy + hs). The auto-port collapsed it
  to a battery + on/off sensor stub (`kFzBattery` + `kFzOnOff`, genPowerCfg
  binding, phantom `battery`/`voltage` exposes), dropping the entire light.
  Graduated to a Tier-2 override restoring the full RGBW light bundle
  (`kFzOnOff` + `kFzBrightness` + `kFzColorTemperature` + `kFzColor` + the
  matching `kTz*` write path, bind 0x0006/0x0008/0x0300) with
  state/brightness/color_temp/color_x/color_y/hue/saturation exposes and no
  battery.
- **Philio PAT04-A (Evology PAT04-A) water leak detector: IAS dead-key.**
  The auto-port wired the generic `kFzIasZone` (bare `alarm` key from
  AttributeReport attr 0x0002) and exposed a bare `alarm` binary. But z2m wires
  `m.iasZoneAlarm({zoneType:"water_leak", zoneAttributes:["alarm_1","tamper",
  "battery_low"]})`, which publishes the semantic `water_leak` key (zoneStatus
  bit 0) via a `commandStatusChangeNotification` — so the exposed key was dead
  and the leak channel was missing. Now wires the typed `kFzIasWaterLeakAlarm`
  (bit0 → `water_leak`, bit2 → `tamper`, bit3 → `battery_low`) and exposes
  `water_leak`. Graduated the generated def to a Tier-2 override; added
  `test_philio_parity.cpp`.
- **Commercial Electric 53170161 CCT downlight: dropped colour-temperature axis.**
  z2m wires this "Zigbee CCT Downlight" as `m.light({colorTemp: {range: undefined}})`,
  exposing `state` + `brightness` + `color_temp`, binding `lightingColorCtrl` (0x0300),
  and decoding/setting the colour-temperature axis. The auto-port dropped the CCT
  axis entirely — it carried only on/off + brightness (`kFzOnOff`/`kFzBrightness`,
  exposes `state`/`brightness`, bindings 0x0006+0x0008), so colorTemperature reports
  were never decoded and the SPA could never set CCT. Restored
  `kFzColorTemperature`/`kTzColorTemp`, the `color_temp` expose (unit `mired`), and
  the `lightingColorCtrl` (0x0300) binding. Graduated to a Tier 2 override.

- **Soanalarm SNT858Z soil moisture sensor: dead, mis-classified Tuya-DP map.**
  z2m wires this TS0601 device as a Tuya-MCU sensor
  (`tuya.modernExtend.tuyaBase({dp:true})`) decoding the 0xEF00 datapoint stream:
  dp3 `soil_moisture` (raw /1), dp5 `temperature` (`valueConverter.raw`, /1 — not
  the /10 default), dp9 `temperature_unit` enum {celsius,fahrenheit}, dp15
  `battery` (raw /1). The auto-port mis-classified it as a phantom on/off +
  battery device (`kFzOnOff` + `kFzBattery`, `genOnOff`/`genPowerCfg` bindings,
  state/battery/voltage exposes) so it decoded nothing real and carried a dead
  controllable switch. Graduated to a Tier-2 override wiring the Tuya DP map via
  `factory::TuyaRw` (temperature_unit is STATE_SET) with the four sensor exposes.

- **IOTPerfect PF-PM02D-TYZ smart water/gas valve: Tuya-MCU misroute (dead
  state).** The auto-port wired this TS0601 / `_TZE2xx` valve as a bare
  genOnOff stub (`kFzOnOff` / `kTzOnOff`, bound to cluster 0x0006) — a cluster
  the device never speaks — leaving its single `state` expose dead in both
  directions. z2m decodes via `legacy.fz.tuya_switch` (0xEF00 DP 1 `state`,
  bool → "ON"/"OFF") and writes via `legacy.tz.tuya_switch_state`. Graduated to
  a Tier-2 override wiring `fz_tuya_datapoints` + `tz_tuya_datapoints` over the
  0xEF00 DP map (DP 1 → `state`, `kTuyaDpFlagBoolEnum`), bound to 0xEF00 with
  the shared `tuya_base_configure()`. Covered by `test_iotperfect_parity`.
- **Weiser SmartCode 10 / 10 Touch (9GED18000-009, 9GED21500-005) locks:
  dropped lock channel + wrong binding endpoint.** Both generated defs were
  battery-only stubs (`kFzBattery` + battery/voltage exposes, binding
  `{1,0x0001}`). z2m wires `m.lock({pinCodeCount:30,
  readPinCodeOnProgrammingEvent:true})` + `m.battery()` with
  `endpoint: () => ({default: 2})`, so the entire `closuresDoorLock` (0x0101)
  channel was missing and both clusters live on endpoint 2, not 1. Graduated
  to Tier-2: wired the generic lock-PIN runtime (`kFzLock` → `lock_state`,
  `kFzLockOperationEvent`/`kFzLockProgrammingEvent` → action stream,
  `kFzLockUserStatusResponse`, `kTzLock`/`kTzLockPinCode`/`kTzLockSoundVolume`/
  `kTzLockAutoRelockTime`), re-endpointed bindings ep1→ep2, and set
  `.default_endpoint=2`. Same hardware as the Kwikset SmartCode family.
- **VAV (NodOn-derived) modules: dropped `power_on_behavior` + `pilot_wire_mode`.**
  z2m extends both VAV-256215-MOD1 (pilot-wire heater) and VAV-256215-MOD2
  (metering relay) with `m.onOff({powerOnBehavior:true})`, which decodes the
  genOnOff `startUpOnOff` attribute (0x4003) into `power_on_behavior` — the
  auto-port dropped it on both. MOD1 additionally extends with
  `...nodonPilotWire(true)`, which decodes the NodOn custom pilot-wire cluster
  (0xFC00 attr 0x0000 `mode`, UINT8) into a `pilot_wire_mode` enum
  (off/comfort/eco/frost_protection/comfort_-1/comfort_-2) — that whole cluster
  was dropped. Graduated both defs out of `generated/`, wired generic
  `kFzPowerOnBehavior`/`kTzPowerOnBehavior1` and a new `vav::kFzPilotWireMode`
  (in `definitions/vav/_shared.{hpp,cpp}`), and added `power_on_behavior` /
  `pilot_wire_mode` exposes. Metering stays energy+power only (z2m
  `cluster:"metering"` = seMetering 0x0702, no 0x0B04 current/voltage half).
- **Aldi (MEGOS) 141L100RC switch/dimming remote: dead action, phantom
  control.** z2m models this remote as `e.action([on, off, brightness_stop,
  brightness_step_up/down, brightness_move_up/down])`, decoding genOnOff +
  genLevelCtrl COMMANDS via `fz.command_on/off/step/move/stop` with an empty
  `toZigbee`. The auto-port mis-modelled it as a settable on/off `state`
  (`kFzOnOff` + `kTzOnOff`) — a dead control whose button presses produced no
  `action`. Graduated to a Tier-2 override wiring the command decoder set
  (`kFzCommandOn/Off/Step/Move/Stop`) + an `action` enum, dropping the bogus
  to_zigbee and binding genLevelCtrl (0x0008) alongside genOnOff (0x0006).
- **XAL "Just MOVE IT 25" luminaires (050-0131558M, 050-0511558F,
  050-1212558H): dropped `power_on_behavior` channel.** All three are z2m
  `m.light()` (no color/colorTemp args) = plain dimmable lights, so the
  reported "RGBW/CCT color axis" was a false flag (bare `m.light()` emits
  neither a color expose nor `fz.color_colortemp`). The real gap: `m.light()`
  also exposes `power_on_behavior` (`e.power_on_behavior(["off","on","toggle",
  "previous"])` backed by `fz`/`tz.power_on_behavior` on genOnOff attr 0x4003
  startUpOnOff), which the auto-port dropped. Graduated the three defs out of
  `generated/` and wired the generic `kFz`/`kTzPowerOnBehavior1` pair plus the
  `power_on_behavior` expose.
- **BYUN M415-5C gas / M415-6C smoke sensors: dead alarm key + dropped
  alarm-clear channel.** These sensors abuse standard clusters and z2m drives
  them with four bespoke converters (`byun.ts` → `fz.byun_{smoke,gas}_{true,
  false}`) producing a single boolean expose (`smoke` / `gas`). The auto-ports
  wired the generic `kFzIasZone`, which emits the unrelated keys
  `alarm`/`tamper`/`battery_low` (dead-key) and — fatally — never reads the
  alarm-CLEAR channel, so the sensor could latch ON forever (dropped channel).
  Graduated both to Tier-2 + a shared `definitions/byun/_shared.{hpp,cpp}` that
  reproduces z2m exactly: ALARM = `ssIasZone` commandStatusChangeNotification
  fired only when `zoneStatus == 33`; CLEAR = `pHMeasurement` (0x0409)
  `measuredValue == 0` (smoke) / raw cluster 0x040A frame-control `0x18` (gas).
  Exposes corrected to the single boolean `gas` / `smoke`. Added clusters
  0x0409/0x040A to the cluster-name table.

- **Chacon ZB-ERSM-01 roller shutter: missing window-cover action.** z2m wires
  `m.windowCovering` PLUS `m.commandsWindowCovering({commands:["open","close",
  "stop"]})`, the latter adding an `action` enum decoded from
  `closuresWindowCovering` cluster-specific commands 0x00 upOpen / 0x01
  downClose / 0x02 stop (the module's own button presses). The auto-port kept
  only the position half and dropped the command block, so those frames decoded
  to nothing. Graduated to a Tier-2 override wiring
  `kFzCommandCoverOpen/Close/Stop` + an `action` expose (single channel → bare
  action). Position decode retained. `coverMode` (write-only Composite via
  `tz.cover_mode`) deferred as config infra.
- **iLightsIn HSSA18-Z-MID (Zhaga module): dropped illuminance + occupancy
  channels and dead IAS key.** The auto-port dropped `m.illuminance()`
  (msIlluminanceMeasurement 0x0400) and `m.occupancy()` (msOccupancySensing
  0x0406) entirely, and wired the IAS alarm to the generic `kFzIasZone` (bare
  `alarm`, bit 0 only). z2m's `iasZoneAlarm({zoneType:"occupancy"})` with both
  alarm_1+alarm_2 publishes the split keys `occupancy_alarm_1` (zoneStatus
  bit 0) + `occupancy_alarm_2` (bit 1). Graduated the def to a Tier-2 override
  wiring `kFzIlluminance` + `kFzOccupancy` + the new typed
  `kFzIasOccupancyAlarm1`/`kFzIasOccupancyAlarm2` converters, with the
  0x0400/0x0406 bindings restored. (HLC610 plain `m.light()` is correct.)
- **HomeSeer DS150ZB door sensor: IAS dead-key + phantom exposes.** z2m wires
  `m.iasZoneAlarm({zoneType:"contact", zoneAttributes:["alarm_1","battery_low"]})`,
  publishing the semantic, inverted key `contact` (= `!zoneStatus` bit0, since
  `invertAlarmPayload = zoneType==="contact"`). The auto-port instead lowered the
  generic `kFzIasZone` (bare key `alarm`), so the contact state never reached the
  shadow, and declared phantom `alarm`/`tamper`/`voltage` exposes (z2m has none:
  `m.battery()` defaults `voltage=false`, and there is no `tamper` attribute).
  Graduated to a Tier-2 override using typed `kFzIasContactAlarm` and aligned
  exposes to `battery`/`contact`/`battery_low`. Pinned by `test_homeseer_parity`.
- **BSEED BSEED_TS0601_cover curtain switch: Tuya-DP misroute (dead
  position).** This TS0601 `_TZE200_yenbr4om` / `_TZE204_bdblidq3` /
  `_TZE200_bdblidq3` curtain switch is decoded by z2m via
  `legacy.fz.tuya_cover` over the `manuSpecificTuya` (0xEF00) DP stream
  (dp 2 coverPosition + dp 3 coverArrived → `position`, dp 105 coverSpeed →
  `motor_speed`), but the auto-port wired it to the generic ZCL
  windowCovering cluster (`kFzCoverPosition`/`kTzCoverPosition` on 0x0102)
  with a 0x0102 binding. A TS0601 Tuya device never speaks ZCL 0x0102, so the
  generic converter matched nothing and `position` was a dead key. Graduated
  to a Tier-2 override wiring the Tuya-DP map (`fz_tuya_datapoints` /
  `tz_tuya_datapoints`) and the 0xEF00 binding, adding the `motor_speed`
  expose. No position inversion (none of the three manufacturerNames are in
  z2m's `coverPositionInvert` list).
- **Novo C10-3E-1.2 curtain switch (TS0601 / _TZE200_swhwv3k3): Tuya-MCU
  misroute, cover dead.** z2m decodes this device via `legacy.fz.tuya_cover` /
  `legacy.tz.tuya_cover_control` — the 0xEF00 (manuSpecificTuya) datapoint
  stream — but the auto-port wired the GENERIC windowCovering converters
  (`kFzCoverPosition`/`kTzCoverPosition`, cluster 0x0102) and bound endpoint 1
  to 0x0102. A TS0601 Tuya MCU never speaks 0x0102, so position, state and the
  set path were all dead. Graduated to a Tier-2 override that ports the DP map
  to `fz_tuya_datapoints`/`tz_tuya_datapoints`: DP1 state {0:OPEN,1:STOP,2:CLOSE},
  DP2/DP3 position (coverPosition/coverArrived), DP105 motor_speed; binds 0xEF00.
- **TNCE TNCE_CLIMATE temperature & humidity sensor: missing decoders.** z2m
  wires `m.temperature()` + `m.humidity()` + `m.battery()` and the TELINK
  TLSR82xx fingerprint declares inputClusters `[0,3,32,1026,1029,1]`
  (1026=`msTemperatureMeasurement`, 1029=`msRelativeHumidity`,
  1=`genPowerCfg`). The auto-port lowered only `kFzBattery`, dropping BOTH
  environmental channels (their decoders, exposes and bindings). Graduated to a
  Tier-2 override adding `kFzTemperature` (0x0402) + `kFzHumidity` (0x0405),
  their `temperature`/`humidity` exposes and the 0x0402/0x0405 bindings.
- **Girier JR-ZDS01 1-gang mini switch: dropped `power_on_behavior` +
  `switch_type` config channels.** z2m builds this device with
  `tuya.modernExtend.tuyaOnOff({switchType: true})`, which surfaces three
  states: `state` (genOnOff on/off), `power_on_behavior` (genOnOff attr
  0x8002 `moesStartUpOnOff`) and `switch_type` (manuSpecificTuya2/0xE001 attr
  0xD030 `switchType`, `{0:toggle,1:state,2:momentary}`). The auto-port carried
  only `state`, so both config-attr decoders and their write paths were dead.
  Graduated to a Tier-2 override wiring the existing `tuya/_shared` converters
  (`kFzTuyaPowerOnBehavior`/`kFzTuyaSwitchType` + `kTz*` twins) and adding the
  two enum exposes. (Not a TS0601 0xEF00 Tuya-DP device — config rides standard
  attribute reports.)

- **Seastar Intelligence 020B0B (Fischer & Honsel LED Tischleuchte Beta Zig):
  dropped color/colorTemp axis.** z2m drives this bulb via
  `m.light({colorTemp:{range:[153,370]}, color:true})` (on/off + brightness +
  colorTemp mireds + XY/HS color), but the auto-port collapsed it to on/off +
  brightness only, dropping the entire `lightingColorCtrl` (0x0300) axis.
  Restored `kFzColorTemperature` + `kFzColor` (+ `kTzColorTemp`/`kTzColor`),
  the `color_temp`/`color_x`/`color_y`/`hue`/`saturation` exposes, and the
  0x0300 bind. Graduated to a parent override.
- **Villeroy & Boch C5850000 + C0040000 (Subway 3.0 / mirror kits): dropped
  colour-temperature axis.** Both are tunable-white (CCT) lights driven in z2m
  by `m.light({colorTemp:{range:[160,450]}})` — on/off + brightness +
  colorTemperature on `lightingColorCtrl` (0x0300). The auto-port wired only
  `kFzOnOff` + `kFzBrightness` and dropped the entire 0x0300 channel, so
  `color_temp` never decoded and was missing from the exposes/bindings.
  Graduated both to Tier-2 overrides that restore `kFzColorTemperature` +
  `kTzColorTemp`, the `color_temp` expose ([160,450] mireds), and the 0x0300
  binding. No colour (xy/hs) axis is added — z2m has no `color:` arg.
- **Hilux DZ8 ("Spot 7W") LED bulb: dropped color-temperature axis.** z2m models
  this as `m.light({colorTemp: {range: [153, 370]}})` (a CCT-tunable bulb), but
  the auto-generator collapsed `m.light()` to on/off + brightness only — leaving
  `color_temp` unexposed, undecoded and unbound. Restored the CCT axis:
  `kFzColorTemperature` + `kTzColorTemp`, the `color_temp` (mired) expose and the
  `lightingColorCtrl` (0x0300) binding. D160-ZG (plain `m.light()`) is correctly
  left as on/off + brightness. Added `tests/test_hilux_parity.cpp`.
- **Spacetronik ZB-DG02 gas leakage sensor: IAS-vs-Tuya-DP misroute.** The
  TS0601 / `_TZE204_uc0iv1hb` was auto-ported as a generic IAS-zone
  (`ssIasZone` 0x0500) emitting bare `alarm`/`tamper`/`battery_low`. The device
  speaks none of that — z2m decodes it via `tuya.modernExtend.tuyaBase({dp:true})`
  + a single datapoint `[1, "gas", trueFalse0]` on the 0xEF00 manuSpecificTuya DP
  stream. The misroute left the only real channel (`gas`) dead. Graduated to a
  Tier-2 override wiring `fz_tuya_datapoints` with `dp::binary_inv(1,"gas")`
  (matching z2m `trueFalse0`: wire 0 → gas detected), the `gas` expose, and a
  0xEF00 binding.

- **Current Products Corp CP180335E-01 ("E-Wand") tilt blind: wrong cover
  channel.** z2m decodes this hybrid blind via the tilt channel
  (`fz.cover_position_tilt` + `meta.coverStateFromTilt`, `currentPositionTilt`
  attr 0x0009) and exposes `e.cover_tilt()` = state + tilt, controlling it with
  `tz.cover_state` + `tz.cover_position_tilt` (goToTiltPercentage 0x08). The
  auto-port wired the LIFT channel instead (`kFzCoverPosition`/`kTzCoverPosition`
  → "position", goToLiftPercentage 0x05), so the real tilt reports never
  decoded and the device exposed a phantom `position`. Graduated to a Tier-2
  override that swaps to `kFzCoverTilt` + `kTzCoverState` + `kTzCoverPositionTilt`
  and exposes state + tilt (battery/voltage retained).
- **Smartwings WM25L-Z roller shade: battery level reported 2x low.** z2m sets
  meta `{battery:{dontDividePercentage:true}}` for this model, so it reports
  `batteryPercentageRemaining` as a whole 0..100 % value, but the auto-port
  wired the default `kFzBattery` which always halves the ZCL half-percent
  encoding. Added a reusable generic `kFzBatteryNoDivide` converter and pointed
  the graduated WM25L-Z override at it (cover position via `kFzCoverPosition`
  already matched z2m's `coverInverted:true` raw-percentage path).

- **TCI "TCI - Mini ZLL I" Dali driver: controller action stream dropped.**
  z2m wires `m.light() + m.commandsOnOff() + m.commandsLevelCtrl()`, so the
  driver is both a settable light AND a controller that emits genOnOff /
  genLevelCtrl commands as a composite `action`. The auto-port kept only the
  light half (state + brightness) and dropped the `action` expose plus every
  command converter, so the device's controller role was dead. Graduated the
  def and additively wired the generic genOnOff/genLevelCtrl command
  converters (`on`/`off`/`toggle`/`brightness_move_to_level`/`*_move_*`/
  `*_step_*`/`brightness_stop`) + an `action` expose, keeping the settable
  state/brightness write path. The three plain TCI drivers (122576, 151570,
  676-00301024955Z) are bare `m.light()` (dimmable only) and were verified
  FALSE for the suspected dropped color/colorTemp axis (regression-guarded).
- **Acuity Brands Lighting (Juno) downlights: restored dropped colour axes.**
  All three luminaires were auto-ported as plain dimmable lights (on/off +
  brightness only) while z2m wires `m.light({colorTemp[, color]})`: `WF4C_WF6C`
  (wafer downlight) now exposes `color_temp`; `RB56SC` and `RB56AC`
  (Retrobasics downlights) now expose `color_temp` + `color_x`/`color_y`. All
  three wire the generic `kFzColorTemperature`/`kTzColorTemp` (and, for the two
  colour bulbs, `kFzColor`/`kTzColor`) and bind `lightingColorCtrl` (0x0300);
  `color_temp` carries the z2m mireds range 200-370.
- **iStar SCCV2403-2 (CCT) / SCCV2401-4 (RGBW) LED controllers: colour axes
  dropped.** The auto-port collapsed both z2m `m.light(...)` bundles to on/off +
  brightness only. SCCV2403-2 lost its `colorTemp:{range:[153,370]}` channel;
  SCCV2401-4 lost both `colorTemp:{range:[275,295]}` and `color:{modes:[xy,hs]}`.
  Restored `kFzColorTemperature`/`kFzColor` (+ tz peers), the
  `color_temp`/`color_x`/`color_y`/`hue`/`saturation` exposes and the 0x0300
  binding. SCCV2401-1 (plain dimmer) was already correct and is unchanged.
- **Brun Holding "Fire Fence" stove guard: relays collided, temperature
  dropped, phantom voltage.** z2m maps `m.deviceEndpoints({main_switch:1,
  short_override:2})` but the auto-port placed `short_override` on EP1, so both
  relays collided on the bare `state` key. Restored EP2 (`state_main_switch` /
  `state_short_override`). The port also dropped `m.temperature()`
  (msTemperatureMeasurement 0x0402) — re-added the decoder, expose, and bind
  (EP1 report suffixes to `temperature_main_switch`). Finally removed a phantom
  `voltage` expose: z2m's `m.electricityMeter` sets `voltage:false` +
  `current:false`, so only power + energy stream from seMetering 0x0702
  (tagged to main_switch -> `power_main_switch` / `energy_main_switch`); no
  0x0B04 electrical half exists on this device. Graduated to Tier-2 with a
  parity test.
- **Smart9 S9ZGBRC01 (TS0215) key fob: arm/emergency actions dead, phantom
  on/off control.** The "Smart remote controller" is a battery panic remote —
  z2m wires `fz.command_arm + fz.command_emergency + fz.battery` and exposes
  `action([disarm, arm_day_zones, arm_night_zones, arm_all_zones, exit_delay,
  emergency])`. The auto-port mis-ported it as a controllable on/off switch
  (`kFzOnOff`/`kTzOnOff` + a phantom `state` Binary StateSet + a spurious
  genOnOff 0x0006 bind): the IAS ACE actions never decoded and the on/off state
  was a dead sink. Graduated to Tier-2 wiring `kFzIasAceArm` (commandArm 0x00 →
  `action=<armMode>` + `action_code` + `action_zone`) + `kFzIasAceEmergency`
  (commandEmergency 0x02 → `action="emergency"`) + `kFzBattery`, replaced
  `state` with the `action` enum, and narrowed the bind to genPowerCfg 0x0001 to
  match z2m configure. (z2m uses `command_emergency` cmd 0x02, NOT `command_panic`
  cmd 0x04 — so `kFzIasAceEmergency`, not `kFzIasAcePanic`.) Same key-fob shape
  as the securifi Almond Click / linkind / climax keypad fixes.
- **M-ELEC "Stitchy" family (ML-ST-D200 / -D200-NF / -BP-DIM dimmers +
  ML-ST-R200 relay): dropped `power_on_behavior` axis.** All four use z2m
  `m.light()` / `m.onOff()`, which default `powerOnBehavior=true` and wire
  `fz.power_on_behavior` (genOnOff `0x4003` startUpOnOff enum8) +
  `tz.power_on_behavior` + the `power_on_behavior` enum expose. The auto-ports
  wired only on/off (+ brightness on the dimmers), dropping the axis entirely.
  Graduated all four to Tier-2 overrides with `kFzPowerOnBehavior` /
  `kTzPowerOnBehavior1` + the expose. The suspected colour/colorTemp drop is a
  FALSE flag: `m.light()` adds the `0x0300` axis only when passed
  `color`/`colorTemp` args, and none of these do; the parity test pins the
  absence so a future colour sweep cannot bolt one on.
- **Jasco / GE 43095 smart plug-in switch: physical button presses dropped.**
  z2m wires `fz.command_on_state` / `fz.command_off_state`, which fold genOnOff
  *commands* (commandOn 0x01 / commandOff 0x00) into the `state` property — the
  43095 reports button presses as cluster commands, not onOff attribute reports.
  The auto-port carried only `kFzOnOff` (attribute 0x0000) + `kFzMetering`, so
  every toggle was dead. Graduated the def to Tier-2 and added jasco
  `kFzCommandOnState` / `kFzCommandOffState` (genOnOff cmd → `state` Bool,
  converging on the same key as the attribute path). 43132 (metering = 0x0702
  only) and ZB3102 dimmer verified unaffected.
- **Wyze WLCKG1 ("Ford") lock: dropped lock operation-event decoder.** z2m wires
  `[fz.lock, fz.lock_operation_event, fz.battery]`, but the auto-port wired only
  `fz.lock` + `fz.battery` and dropped `fz.lock_operation_event` — the
  closuresDoorLock (0x0101) cluster-specific command 0x21 (ServerToClient) that
  reports how the lock was operated (`action`/`action_user`/`action_source`/
  `action_source_name`). Graduated to a Tier-2 parent override wiring the generic
  `kFzLockOperationEvent` alongside the existing converters.

- **Echostar SAGE206611 / SAGE206612 (SAGE by Hughes) remotes: dead on/off
  control instead of `action`.** Both are battery controllers that EMIT
  genOnOff On/Off commands and surface them as an `action` (z2m toZigbee
  empty), but the auto-port mis-modelled both as a settable on/off `state`
  (kFzOnOff + kTzOnOff) and dropped the action. Graduated both to Tier-2:
  SAGE206611 (switch) wires generic kFzCommandOn/Off → `action` "on"/"off";
  SAGE206612 (doorbell) wires a new vendor `kFzSageBellAction`
  (echostar/_shared.cpp, mirroring z2m `SAGE206612_state`: commandOn→"bell1",
  commandOff→"bell2") + kFzBattery. Single endpoint → no endpoint_map.
- **Rademacher addZ colour LEDs (35104001 / 35144001 / RDM-35274001): restored
  dropped colour-temperature + colour axes.** All three z2m defs are full
  RGB+CCT bulbs (`m.light({colorTemp:{range:[153,555]}, color:...})`) but the
  auto-port collapsed each `m.light()` to on/off + brightness only, dropping
  both the CCT and colour channels. Graduated all three to Tier-2, wiring the
  generic `kFzColorTemperature`/`kTzColorTemp` + `kFzColor`/`kTzColor` decoders,
  restoring the `color_temp`/`color_x`/`color_y`/`hue`/`saturation` exposes and
  the `lightingColorCtrl` (0x0300) binding. Same dropped-axes class as the
  SLV/Feibit colour-light fixes.
- **Hommyn MS-20-Z / WS-20-Z sensors: primary IAS state never reached the
  shadow (dead-key).** Both auto-ports wired the generic `kFzIasZone` (which
  emits the bare key `alarm`) while z2m decodes the semantic state via a typed
  `fz.ias_*_alarm_1` (zoneStatus bit 0): MS-20-Z's
  `ias_occupancy_alarm_1_with_timeout` → `occupancy`, WS-20-Z's
  `ias_water_leak_alarm_1` → `water_leak`. Graduated both to Tier-2, swapped to
  `kFzIasMotionAlarm` (emits `occupancy`) / `kFzIasWaterLeakAlarm` (emits
  `water_leak`), and repointed the expose `alarm` → the semantic key. tamper +
  battery_low co-decode unchanged; MS-20-Z keeps `kFzBattery`, WS-20-Z (no
  `fz.battery` in z2m) keeps `battery_low` only — both at parity.
- **TubesZB `tubeszb.router` (CC2652 Router): stripped a phantom on/off bundle
  from a pure repeater.** z2m defines this as a pure router — `exposes:[]`,
  `toZigbee:[]`, `fromZigbee:[fz.linkquality_from_basic]` — whose only
  `configure()` binds `genBasic` on endpoint 8. The Tier-1 auto-port carried a
  phantom `kFzOnOff`/`kTzOnOff` pair, a `state` Binary expose, and a `genOnOff`
  (0x0006) bind on ep1, creating a dead controllable toggle in the shadow on a
  device with no on/off endpoint. Graduated to Tier-2: no exposes, no fz/tz,
  only the `{8,0x0000}` genBasic bind. Same shape as the Espressif
  ZigbeeRangeExtender / SMLIGHT router phantom fixes.
- **LDS lights/sensor: dropped colour axis + missing motion sensor.**
  `ZBT-RGBWLight-A0000` is z2m `m.light({colorTemp:{range:[153,555]}, color:true})`
  but the auto-port collapsed it to on/off + brightness, dropping the colorTemp +
  XY/HS colour channels — restored `kFzColorTemperature` + `kFzColor`
  (`kTzColorTemp`/`kTzColor`), colour exposes, and the `lightingColorCtrl` (0x0300)
  binding. `PFMOT001` (Hive Pet Friendly Motion sensor, `m.battery()` +
  `m.iasZoneAlarm({zoneType:"occupancy", zoneAttributes:["alarm_1","tamper",
  "battery_low"]})`) was dropped entirely by the auto-port — added as battery +
  `kFzIasMotionAlarm` (semantic `occupancy` on zoneStatus bit 0, plus
  tamper/battery_low) and registered. `FWBulb03UK` (bare `m.light()`) is
  dimmable-only and verified correct.

- **Spotmau SP-PS1-02 / SP-WS-02 single-gang switches: control + reads dead on
  the wrong endpoint.** z2m declares `endpoint:()=>({default:16})` with a single
  `m.onOff()`, so genOnOff lives on endpoint 16 and publishes a bare `state`.
  The auto-port bound genOnOff on ep1 and left `default_endpoint=0`, so
  reporting was configured on the wrong endpoint and outbound on/off routed to
  ep1. Graduated both to Tier-2 with `default_endpoint=16` + `{16,0x0006}`
  binding and NO endpoint_map (the default endpoint is unsuffixed; a 1-entry
  map would wrongly yield `state_default`). Same shape as the ge PTAPT-WH02 /
  frient SMRZB-153 default_endpoint fix. (SP-PS2-02/PS3-02 multi-gang were
  already correct.)

- **SLV (VALETO) family: restored dropped colour axes + stripped a phantom
  remote bundle.** The four LED luminaires were auto-ported as plain dimmable
  lights (on/off + brightness only) while z2m wires `m.light({colorTemp[,
  color]})`: `1001248` (CCT driver) and `1005314` (QPAR111) now expose
  `color_temp`; `1001923` (GU10 RGBW) and `1005318` (E27 RGBW) now expose
  `color_temp` + `color_xy`. All four wire the generic
  `kFzColorTemperature`/`kTzColorTemp` (and, for RGBW, `kFzColor`/`kTzColor`)
  converters and bind `lightingColorCtrl` (0x0300). The `1002994` VALETO remote
  — a pure binder that z2m declares with `fromZigbee:[], toZigbee:[],
  exposes:[]` — had been given a phantom controllable `state` plus `battery`/
  `voltage` exposes and decoders; stripped to nothing to match z2m.
- **YSRSAI LED controllers: rescued the dimmer + WW/CW from phantom-battery
  mis-ports.** z2m drives all three (`YSR-MINI-01_dimmer` ZB-DL01,
  `YSR-MINI-01_wwcw` ZB-CT01, `YSR-MINI-01_rgbcct`) with
  `tuya.modernExtend.tuyaLight()` — a mains-powered ZCL light, not a Tuya
  0xEF00 DP device and not battery-powered. The auto-port mis-classified the
  dimmer and the WW/CW as battery sensors (`kFzBattery` + bare `kFzOnOff`,
  phantom `battery`/`voltage` exposes) and dropped the brightness channel
  (and `color_temp` on the CCT). Re-wired both as real lights: dimmer →
  state + brightness (genOnOff + genLevelCtrl); WW/CW → state + brightness +
  color_temp (+ lightingColorCtrl 0x0300), with `kTzOnOff`/`kTzBrightness`/
  `kTzColorTemp` write-back, bindings, and configure-reporting. rgbcct was
  already correct and is now regression-guarded.
- **FrankEver FK_V02 water valve: rescued from misrouted genOnOff.** The
  TS0601 `_TZE200_wt9agwf3` / `_5uodvhgc` / `_1n2zev06` smart water valve was
  auto-ported onto bare `genOnOff` (cluster 0x0006) with only a `state` expose,
  but z2m drives it entirely over the 0xEF00 (manuSpecificTuya) DP stream
  (`legacy.fz.frankever_valve`) — the genOnOff wiring never matched, so nothing
  decoded and the `threshold` + `timer` channels were dropped entirely. Re-wired
  onto the Tuya-DP map (DP1→state, DP101→threshold raw, DP9→timer with the
  seconds→minutes `/60` scale z2m applies) plus the matching write path, restored
  the two lost StateSet exposes, and bound 0xEF00 instead of 0x0006.
- **Xenon X7726 curtain motor: removed misrouted dup-fingerprint stub.** The
  single z2m def (TS0601 / `_TZE284_hbjwgkdh`, a Tuya-MCU 0xEF00 datapoint
  device) had been emitted as TWO defs for the same fingerprint: the correct
  full Tuya-DP port plus a stub that misrouted to `genWindowCovering` (0x0102)
  via `kFzCoverPosition` and dropped state/calibration/temperature. The firmware
  only surfaces this device over 0xEF00 DPs, so the 0x0102 route was dead. The
  stub was removed and the DP def graduated to `definitions/xenon/X7726.cpp` with
  the z2m identity (model `X7726` / vendor `Xenon Smart`), the phantom `action`
  expose dropped, and exposes typed to match z2m (state/calibration enums,
  position/temperature numerics).
- **Plaid Systems Spruce `PS-SPRZMS-SLP3`: restored dropped temperature +
  humidity channels.** z2m wires this temp/moisture sensor as
  `fromZigbee:[fz.temperature, fz.humidity, fz.plaid_battery]` (exposes
  temperature + humidity + battery + battery_voltage), but the auto-port
  emitted a phantom on/off bundle (`kFzOnOff` + a `state` expose + a 0x0006
  genOnOff bind) plus battery-only, dropping BOTH sensor channels and leaving
  the moisture sensor unable to report temperature or humidity. Re-wired onto
  generic `kFzTemperature` (0x0402, /100) + `kFzHumidity` (0x0405, /100) and a
  new shared `kFzPlaidBattery` that reads `mainsVoltage` (genPowerCfg attr
  0x0000, raw mV — NOT batteryVoltage, which the generic kFzBattery expects)
  and derives `battery` via z2m's `voltageToPercentage {min:2500, max:3000}`.
  Bindings corrected to genPowerCfg + msTemperatureMeasurement +
  msRelativeHumidity; phantom genOnOff bind + `state` expose removed.
- **Danalock V3-BTZB/V3-BTZBE smartlock: restored the lock action stream +
  PIN write that the generated port dropped.** The auto-port wired only
  `kFzLock` + `kFzBattery` (battery/voltage/lock_state). z2m also wires
  `fz.lock_operation_event` + `fz.lock_programming_event` (the `action` /
  `action_user` / `action_source` / `action_source_name` stream) and
  `tz.pincode_lock`. Graduated the def to Tier 2 and wired the existing
  generic closuresDoorLock helpers `kFzLockOperationEvent` (cmd 0x21),
  `kFzLockProgrammingEvent` (cmd 0x20) and `kTzLockPinCode`, plus the
  `action*`/`pin_code` exposes. The `fz.lock_pin_code_response`
  (commandGetPinCodeRsp) read-back has no generic decoder yet and is
  deferred (INFRA). Covered by `tests/test_danalock_parity.cpp`.
- **Wally WallyHome multi-sensor (`U02I007C.01`): restored four broken channels.**
  The generated def degraded the device to an on/off relay + battery + dead
  bare-`alarm` IAS. z2m decodes it very differently: `fz.temperature` +
  `fz.humidity` (both DROPPED in the port), `fz.command_on`/`fz.command_off`
  (genOnOff On/Off *commands* → `action`, not a controllable `state`; the
  phantom `kFzOnOff`/`kTzOnOff` + `state` were removed, no toZigbee), and two
  PER-ENDPOINT custom IAS converters over `ssIasZone` status_change — endpoint 1
  → `contact = !(zoneStatus bit0)`, endpoint 2 → `water_leak = (bit0)`. Added
  endpoint-pinned `kFzContactEp1`/`kFzWaterLeakEp2` (`definitions/wally/_shared.cpp`)
  so each zone routes to exactly one semantic key — the generic wildcard-endpoint
  typed IAS converters would cross-fire both `contact` and `water_leak` on every
  report. Graduated the generated def to a Tier 2 override; added
  `tests/test_wally_parity.cpp`.
- **Blaupunkt SCM-S1 roller shutter: wrong-cluster cover fix.** z2m drives this
  shutter through the brightness/on-off proxy (`fz.cover_position_via_brightness`
  + `fz.cover_state_via_onoff` + `tz.cover_via_brightness`), but the auto-port
  wired the generic `kFzCoverPosition`/`kTzCoverPosition` against
  closuresWindowCovering (0x0102) — a cluster this device never speaks — so
  position decode AND control were both dead, and the `state` channel was
  dropped. Rewired to the generic via-brightness converters (genLevelCtrl 0x0008
  currentLevel → position+state, genOnOff 0x0006 → state, moveToLevelWithOnOff
  for writes) + bindings 0x0008/0x0006, and added the `state` expose. Mirrors the
  Climax SCM-5ZBS fix (same SCM roller-shutter family).
- **Evanell EZ200 TRV: rescued from a dead genThermostat port.** The
  `TS0601` + `_TZE200_dmfguuli`/`_TZE200_rxypyjkw` thermostatic radiator valve
  was auto-ported onto the generic genThermostat converter (cluster 0x0201)
  with a phantom 0x0201 binding, but z2m wires it as a Tuya-MCU device on the
  0xEF00 (manuSpecificTuya) DP stream (`legacy.fz.evanell_thermostat`) — the
  port decoded NOTHING. Re-wired onto the Tuya-DP read+write infra
  (`factory::TuyaRw`): DP2 → `system_mode` enum {0:auto,2:heat,3:off}, DP4 →
  `current_heating_setpoint` /10, DP5 → `local_temperature` /10, DP6 →
  `battery`, DP8 → `child_lock`. Exposes are now flat (no `Climate`) with a
  proper enum `system_mode` (was a bare Binary), setpoint range 5..30 °C.
- **Senoro.Win window alarm (TS0601 / `_TZE200_ytx9fudw`): restored the missing
  `opening_state` channel and removed a misrouted IAS duplicate.** The fingerprint
  carried two defs: a Tuya-DP port that dropped DP 101 `opening_state` (the
  device's primary channel) and exposed two phantom keys (`state`, `action`) no
  DP drives, plus a same-fingerprint stub misrouted onto the generic IAS-zone
  (ssIasZone 0x0500) emitting bare alarm/tamper/battery_low. z2m has no IAS path —
  it is a pure Tuya-MCU 0xEF00 DP-stream device. Graduated the Tuya-DP def to a
  Tier-2 override (DP 101 → `opening_state` enum {open/closed/tilted}, DP 16 →
  `alarm` Bool to match z2m `e.binary`, DP 2 → `battery`), corrected the exposes,
  and deleted the dead IAS duplicate.

- **Evology PSE03-V1.1.0 siren: dead IAS state decode.** The auto-port wired the
  generic `kFzIasZone`, which reads ZoneStatus from an AttributeReport (attr
  0x0002). But z2m decodes this siren via `fz.ias_siren` — an ssIasZone
  `commandStatusChangeNotification` (Command frame, cmd 0x00) where ZoneStatus
  rides the command body, not attr 0x0002 — so `kFzIasZone` matched nothing on
  the real wire and the exposed `alarm`/`tamper`/`battery_low` keys were dead.
  Re-wired onto `kFzIasGenericAlarm` (the zoneType:"alarm" siren converter):
  bit0 → `alarm`, bit2 → `tamper`, bit3 → `battery_low`. (`fz.ias_enroll` is
  GLOBAL IAS enrollment; `fz.ias_wd` / `tz.warning` are ssIasWd 0x0502 to-zigbee
  warning control — both INFRA, deferred.)
- **Schlage BE468 Connect deadbolt: restored the dropped lock-action stream +
  PIN programming.** The auto-port wired only `kFzLock` + `kFzBattery` and
  exposed `battery`/`voltage`/`lock_state`, dropping z2m's
  `fz.lock_operation_event` / `fz.lock_programming_event` (closuresDoorLock cmds
  0x21/0x20) → `action` / `action_user` / `action_source` /
  `action_source_name` and `tz.pincode_lock` → `pin_code`. Wired the generic
  `kFzLockOperationEvent` + `kFzLockProgrammingEvent` + `kTzLockPinCode` and
  restored the matching exposes (kwikset/onesti/datek precedent), and corrected
  `lock_state` from `Binary` to `Enum` (z2m emits the
  not_fully_locked/locked/unlocked enum). Graduated the def to a Tier-2 parent.
  PIN/user-code *read* response (`fz.lock_pin_code_response` → users map) stays
  INFRA-deferred.
- **Espressif `ZigbeeRangeExtender` (ESP32-C6/H2 router): stripped phantom
  on/off + battery bundle.** z2m defines this as a pure router — `exposes: []`,
  `fromZigbee: []`, `toZigbee: []`, with `configure()` only binding genBasic.
  The Tier-1 auto-port carried a phantom bundle (`kFzOnOff` + `kFzBattery`
  converters; `state`/`battery`/`voltage` exposes; genPowerCfg `0x0001` +
  genOnOff `0x0006` binds) that exists nowhere in z2m, so a mains-powered range
  extender surfaced a dead toggle and a phantom battery/voltage in the shadow.
  Graduated out of `generated/` and reduced to z2m parity (no exposes/fz/tz,
  genBasic bind only); also added the `manufacturerName: "Espressif"` fingerprint
  gate z2m requires (the port was model-only).
- **WMUN ZS05 universal IR remote (TS1201): rescued from battery-only port.**
  The auto-port reduced this IR remote to a battery-only device — it kept only
  `kFzBattery` and dropped the entire Zosung IR channel that z2m wires
  (`wmun.ts` #ZS05: `addZosungIRTransmitCluster`/`addZosungIRControlCluster` +
  the six `fzZosung.zosung_send_ir_code_00..05` fz handlers + the two `tzZosung`
  handlers, exposing `learn_ir_code`/`learned_ir_code`/`ir_code_to_send`).
  Graduated to a parent override wired against the shared Zosung IR runtime
  (`definitions/zosung/_shared`, the same infra the Moes UFO-R11 uses): restored
  all 6 IRTransmit fz handlers + 2 tz handlers + the 3 IR exposes, bound both
  Zosung clusters (0xED00 / 0xE004), and restored the missing
  `_TZ3290_u9xac5rv` manufacturer fingerprint. (Hardware testing of the Zosung
  state machine still pending, per the shared runtime's standing note.)
- **PEQ 3300-P contact sensor: dead IAS contact key + dropped temperature
  channel.** The auto-port lowered the generic `kFzIasZone` converter (emits
  the bare `alarm` key, which never matched the declared expose) and dropped
  the z2m `fz.temperature` channel entirely. Graduated to a parent override
  wiring the typed `kFzIasContactAlarm` (emits `contact`, z2m-inverted
  `contact = !(zoneStatus bit0)`) and restoring the `kFzTemperature` decoder
  (msTemperatureMeasurement 0x0402, /100), the `temperature` expose, and the
  0x0402 binding — matching z2m `[fz.temperature, fz.ias_contact_alarm_1,
  fz.battery]`.
- **Swann security family: rescued from dead IAS/on-off ports.** All three
  defs had dead channels. `SWO-MOS1PA` (motion) and `SWO-WDS1PA` (window/door)
  were auto-ported onto generic `kFzIasZone`, which emits a bare `alarm` key
  while the declared exposes are `occupancy`/`contact` — so those semantic
  channels never updated; now wired to typed `kFzIasMotionAlarm` /
  `kFzIasContactAlarm` (the latter applies z2m's contact = !bit0 inversion),
  matching z2m `fz.ias_occupancy_alarm_1` / `fz.ias_contact_alarm_1`.
  `SWO-KEF1PA` (key fob) was mis-ported as a *controllable* genOnOff switch
  (kFzOnOff + kTzOnOff + writable `state` + 0x0006 binding) when it is an
  ssIasAce (0x0501) action source; now wired to `kFzIasAceArm` +
  `kFzIasAcePanic` with `action`/`action_code`/`action_zone` exposes, no
  toZigbee, and an ssIasAce binding, mirroring z2m `fz.command_arm` +
  `fz.command_panic`.
- **Aubess ZXZIR-02 IR remote: rescued from a phantom on/off + battery port.**
  The auto-generated def mis-classified this pure Zosung IR blaster/learner
  (z2m `zosung.zosungExtend` + `fzZosung.zosung_send_ir_code_00..05` +
  `tzZosung` + `presetsZosung`) as a bare on/off + battery device — phantom
  `kFzOnOff`/`kFzBattery`, dead `state`/`battery`/`voltage` exposes, and
  bindings to genOnOff (0x0006) + genPowerCfg (0x0001) the device never emits.
  Graduated to a Tier-2 override wiring the existing Zosung IR runtime
  (clusters 0xED00 IRTransmit + 0xE004 IRControl): the six IR-transfer
  decoders + `ir_code_to_send`/`learn_ir_code` writers + the three IR exposes,
  matching the Moes UFO-R11 sibling but without battery (z2m's Aubess def
  exposes none). HARDWARE TESTING REQUIRED — the Zosung protocol is faithful
  to z2m but not yet exercised on real hardware.
- **Titan Products TPZRCO2HT-Z3: restored dropped temperature/humidity/CO2
  channels.** The auto-generated port had been reduced to battery-only
  (`from_zigbee:[kFzBattery]`, exposes `battery`/`voltage`). z2m wires
  `fz.battery + fz.humidity + fz.temperature + fz.co2`. Graduated to a Tier-2
  override wiring `kFzTemperature` (0x0402), `kFzHumidity` (0x0405, ep2) and
  `kFzCO2` (0x040D) plus the `temperature`/`humidity`/`co2`/`battery_low`
  exposes and the matching bindings.

- **Somgoms TS0601 / legacy-DP family: rescued from dead standard-cluster
  ports.** All four defs (`ZSTY-SM-11ZG-US-W` 1-gang switch, `ZSTY-SM-1DMZG-US-W`
  dimmer, `ZSTY-SM-1CTZG-US-W` + `SM-1CTW-EU` curtain motors) were auto-ported
  onto generic `genOnOff`/`genLevelCtrl`/`windowCovering` converters and bindings,
  but z2m wires every one as a Tuya-MCU device on the 0xEF00 (manuSpecificTuya)
  DP stream (`legacy.fz.tuya_switch`/`tuya_cover`/`tuya_dimmer`) — the standard
  clusters are never used, so all four ports were DEAD (no state/position/
  brightness ever decoded, writes went nowhere). Re-wired onto the Tuya-DP
  read+write infra: switch DP1 → state; covers DP2+DP3 → position; dimmer DP1
  → state + DP3 → brightness. (Dimmer brightness keeps the raw 0..1000 DP value,
  per the existing ported-Tuya-dimmer convention; z2m's affine 0..254 remap is an
  unsupported DP-map scaling — documented INFRA gap.)

- **Calex colour/colour-temperature + remote-action parity.** `421792`
  (LED A60 RGB lamp, z2m `m.light({colorTemp:{range:[153,370]},
  color:{modes:["xy","hs"]}})`) had its entire lightingColorCtrl (0x0300)
  axis dropped — ported as on/off+brightness only. Restored colour +
  colour-temperature read/write (`kFzColor`/`kFzColorTemperature` +
  `kTzColor`/`kTzColorTemp`, the hue/saturation/color_x/color_y/color_temp
  exposes, and a 0x0300 bind). `421782` ("Smart Wall Switch" RGB wall
  controller, z2m `toZigbee:[]`, action only) was mis-ported as a settable
  on/off switch (dead button presses, phantom relay); rewired to the seven
  generic command converters (on/off/step/move/stop/move_to_color_temp/
  enhanced_move_to_hue_and_saturation) + an `action` expose.
- **TP-Link IAS sensor parity (MS100, CS100).** Both defs lowered the generic
  `kFzIasZone` (bare `alarm` key), leaving their semantic exposes dead. MS100
  (z2m `fz.ias_occupancy_alarm_1` + `m.illuminance()`) now lowers the typed
  `kFzIasMotionAlarm` (`occupancy` from zoneStatus bit0) and the previously
  dropped `kFzIlluminance` channel (0x0400). CS100 (z2m `fz.ias_contact_alarm_1`,
  inverted) now lowers `kFzIasContactAlarm` (`contact = !bit0`). Exposes
  realigned to z2m (occupancy/tamper/battery_low/battery/illuminance for MS100;
  contact/battery_low/battery for CS100).
- **BlitzWolf BW-IS3 / BW-IS9 decode misroutes.** `BW-IS3` (PIR motion sensor)
  was auto-ported as a generic IAS zone (`kFzIasZone`, ssIasZone 0x0500) with
  phantom `alarm`/`tamper`/`battery_low` exposes and a dead 0x0500 bind, but
  z2m decodes it via `legacy.fz.blitzwolf_occupancy_with_timeout` — a Tuya
  0xEF00 DP stream (DP3 → occupancy). Graduated to a Tier-2 Tuya-DP override
  (`fz_tuya_datapoints`, `dp::binary(3,"occupancy")`, `occupancy` expose, 0xEF00
  bind). `BW-IS9` (water-leak sensor) used `kFzIasZone` (bare `alarm`) while
  z2m's `m.iasZoneAlarm({zoneType:"water_leak", zoneAttributes:["alarm_1",
  "alarm_2",…]})` bothAlarms config exposes `water_leak_alarm_1` (zoneStatus
  bit 0) and `water_leak_alarm_2` (bit 1); repointed to two new typed
  `kFzIasWaterLeakAlarm1`/`kFzIasWaterLeakAlarm2` generic converters with
  matching exposes. The BW-SS7 1-gang/2-gang relays were verified correct
  (2-gang already carries the `{l1,l2}` endpoint_map).
- **Cleverio SA100 siren + SS300 sensor parity.** `SS300` (SM0201
  temp/humidity sensor) had both primary channels dropped — wired battery-only
  — when z2m decodes `fz.temperature` + `fz.humidity` + `fz.battery`; restored
  `kFzTemperature`/`kFzHumidity` converters, the temperature/humidity exposes,
  and the 0x0402/0x0405 binds. `SA100` (TS0219 smart siren) used the broad
  `kFzIasZone` (extra tamper + battery_low) with phantom `tamper`/`battery_low`
  exposes, while z2m's `fz.ias_alarm_only_alarm_1` (generic zoneType) emits a
  bare `alarm` only; re-pointed at `kFzIasZoneAlarmOnly` and dropped the two
  phantom exposes (mirrors sibling Woox R7051). Volume (ssIasWd attr 0x0002)
  and power_source remain INFRA-deferred (no generic converter yet).
- **IT Commander contact + button parity.** `ITCMDR_Contact` wired the
  generic `kFzIasZone`, emitting a bare `alarm` (zoneStatus bit 0) that
  never matched the `contact` expose, so the door/window state was dead.
  Swapped to `kFzIasContactAlarm` (z2m `fz.ias_contact_alarm_1`: inverted
  bit-0 → `contact`, bit 2 → `tamper`, bit 3 → `battery_low`) and
  re-pointed the expose to `contact`. `ITCMDR_Click` is a button but was
  mis-ported as a controllable genOnOff switch (`kFzOnOff` + `kTzOnOff` +
  a dead `state` toggle); re-ported to a new vendor `kFzItcmdrClicks`
  converter (genMultistateInput 0x0012 PresentValue → `action`:
  single/double/triple/quadruple/hold/release/many, per z2m
  `fz.itcmdr_clicks`) with an `action` enum expose and no to_zigbee.
- **LeTV 8-key scene remote action parity.** `LeTV.8KEY`
  (`qlwz.letv8key.10`) was mis-ported as a controllable on/off switch —
  phantom genOnOff in/out (`kFzOnOff`/`kTzOnOff`), a settable `state`
  Binary expose, a 0x0006 binding and `to_zigbee` — when z2m's role is
  action-only (`fromZigbee:[fz.qlwz_letv8key_switch]`, `toZigbee:[]`). The
  entire button channel was dropped: z2m decodes genMultistateInput
  (0x0012) presentValue (attr 0x0055) keyed by the *source endpoint* into
  `<gesture>_<button>` (e.g. `single_up`, `hold_center`). Restored with a
  new `kFzLetv8KeyAction` converter (`definitions/letv/_shared.{hpp,cpp}`)
  that mirrors z2m's buttonLookup `{4:up,2:down,5:left,3:right,8:center,
  1:back,7:play,6:voice}` x actionLookup `{0:hold,1:single,2:double,
  3:tripple}`, replaced the phantom on/off with an `action` enum, and
  dropped the bogus `to_zigbee` + binding. Button is encoded in the action
  string so the def carries no endpoint_map.
- **D-Link DCH-B112 contact/vibration IAS dead-key.** The door/window +
  vibration sensor was auto-ported with the generic `kFzIasZone`, which
  emits a bare `alarm` (zoneStatus bit0, NOT inverted) and never produces
  the `contact` or `vibration` channels the device reports. z2m's
  `fzLocal.DCH_B112` decodes the `ssIasZone` ZoneStatusChangeNotification
  into four semantic keys: `contact = !(bit0)` (inverted), `vibration =
  bit1`, `tamper = bit2`, `battery_low = bit3`. Graduated to a Tier-2
  override wiring a dedicated zone-status decoder and restoring the
  `contact` + `vibration` exposes (the phantom `alarm` key is gone).
- **Ecolink sensors: IAS contact dead-key + dropped temperature channel.**
  The `4655BC0-R` contact sensor lowered the generic `kFzIasZone` (bare
  `alarm` key) while its expose declared `contact`, so the door state never
  reached the shadow; swapped in `kFzIasContactAlarm` (z2m polarity:
  `contact = !zoneStatus bit0`, plus tamper/battery_low) to match
  `fz.ias_contact_alarm_1`. Both `4655BC0-R` and `FFZB1-SM-ECO` dropped the
  temperature channel that z2m wires via `fz.temperature` / `m.temperature()`;
  restored `kFzTemperature` (msTemperatureMeasurement 0x0402) + the
  `temperature` expose + the 0x0402 binding on each. `FFZB1-SM-ECO`'s bare
  `alarm` key is left unchanged (z2m `m.iasZoneAlarm({zoneType:"alarm"})`).
- **Automaton AUT000069: restored 5-zone multi-endpoint routing.** The TS011F
  underfloor-heating / irrigation valve controller (`_TZ3000_j0ktmul1`) is a
  z2m `tuya.modernExtend.tuyaOnOff({endpoints:["l1".."l5"]})` device with
  `endpoint:{l1:1..l5:5}` + `meta.multiEndpoint` — five independent genOnOff
  channels. The auto-port collapsed it to a single bare `state` expose with
  one EP1 binding and no `endpoint_map`, so all five zones collided on one key
  and zones 2..5 were never bound. Graduated the def and added the
  `endpoint_map` (l1..l5), per-EP `state_l1..state_l5` exposes and genOnOff
  bindings on EP1..EP5 so each zone routes to its own suffixed key. (The Tuya
  manuSpecific `power_on_behavior`/`child_lock`/`countdown` extras lack a
  generic decoder and are deferred.)

- **Mill heating thermostat parity.** Both `Mill-gen-4` (WiFi heating panel)
  and `MFTWIFI` (smart floor thermostat) are z2m `m.thermostat()` devices
  (hvacThermostat `0x0201`: `occupiedHeatingSetpoint` 5..35 + `systemMode`
  `["off","heat"]`) but were auto-ported as bare genOnOff switches with phantom
  `state`/`battery`/`voltage` and a `0x0001`/`0x0006` bind. Re-wired both to the
  generic `kFzThermostat` read + `kTzThermostat` write, exposing flat
  `local_temperature` / `current_heating_setpoint` / `system_mode`, bound
  `0x0201`, and dropped the phantom on/off + battery channels.
- **Akuvox M423-9E temperature & humidity parity.** The generated def for the
  `TS0201` / `_TYZB01_ujfk3xd9` sensor was reduced to battery-only (only
  `kFzBattery`, exposing `battery`/`voltage`), dropping z2m's `fz.temperature`
  + `fz.humidity` decoders and the `temperature` + `humidity` exposes — the
  entire purpose of the sensor. Graduated to a Tier-2 override adding
  `kFzTemperature` (0x0402, /100) + `kFzHumidity` (0x0405, /100) + the
  temperature & humidity exposes and the matching `msTemperatureMeasurement`
  (ep1) / `msRelativeHumidity` (ep2) bindings.

- **Qmotion QZR-ZIG2400 remote: stop modelling it as a controllable cover.**
  The "5 channel remote" (z2m `fromZigbee:[fz.identify, fz.cover_position_tilt]`,
  `toZigbee:[]`) was auto-ported as an on/off cover — `kFzOnOff`, a writable
  `state` Binary expose, and phantom `kTzOnOff` + `kTzCoverPosition` write
  paths. Rewired to match z2m: a vendor-local `kFzIdentifyAction`
  (genIdentify 0x0003 → `action="identify"`) + the read-only
  `kFzCoverPosition` decoder, no `toZigbee`, exposes `action` + read-only
  `position`, and the phantom genOnOff (0x0006) binding dropped. HDM40PV620
  (roller blind) was verified already correct (its single `kTzCoverPosition`
  already claims both `position` and `state`, covering z2m's
  `[tz.cover_state, tz.cover_position_tilt]`).
- **EDP re:dy plug bind-endpoint parity.** `PLUG EDP RE:DY` (z2m `fz.on_off`
  + `fz.metering`) configures genOnOff + seMetering on `device.getEndpoint(85)`
  ("re:dy plug uses ep relay"), but the auto-port bound both clusters on
  endpoint 1, so the coordinator never bound the device's relay/metering
  endpoint and state/power/energy never reported. Repointed both bindings to
  endpoint 85. Metering stays 0x0702-only — z2m does not use
  `fz.electrical_measurement`, so there is no 0x0B04 (voltage/current) channel.
- **Hampton Bay fan + light parity.** `#99432` (universal ceiling-fan
  remote, z2m `fromZigbee:[fz.fan]`) decoded only `fan_mode` via the generic
  `kFzFanMode` and declared it as a bare Binary with no `fan_state` expose —
  so the z2m-derived on/off `fan_state` was dead. Replaced with an inlined
  vendor converter (`kFzHamptonFan`) emitting `fan_mode` (u8) + `fan_state`
  (Bool: off/0 → false), an Enum `fan_mode` expose with the z2m mode labels,
  and a Binary `fan_state` expose (mirrors the Mercator SSWF01G precedent).
  `#54668161` (12 in. LED smart puff, z2m `m.light({colorTemp})`) had its
  color_temp axis dropped — restored `kFzColorTemperature`/`kTzColorTemp`,
  the `color_temp` expose, and the lightingColorCtrl (0x0300) bind.
- **Netvox ZB02A remote-action parity.** The `ZB02A` wireless wall switch
  (z2m `fromZigbee:[fz.command_toggle]`, `toZigbee:[]`,
  `exposes:[e.action(["toggle"])]`) was mis-ported as a controllable on/off
  switch — `kFzOnOff` + `kTzOnOff` + a dead `state` expose — so button
  presses were dropped and the UI showed a non-functional toggle target.
  Rewired to the generic `kFzCommandToggle` publishing `action: toggle`,
  with no write path and no phantom state. `Z809A` (metering socket) was
  verified correct: z2m's `fz.electrical_measurement` is the 0x0B04 half
  only (no seMetering/energy), matching the existing port.
- **EasyAccess EasyCode903G2.1 (EasyFinger V2) door-lock parity.** The
  auto-port carried a phantom genOnOff bundle (`kFzOnOff` + `kTzOnOff` +
  a `{1,0x0006}` binding) on a device that has no on/off cluster, and
  dropped z2m's entire `fz.easycode_action` event stream plus the
  `sound_volume` / `auto_relock` config. Graduated to Tier 2: removed the
  phantom on/off, ported the RAW closuresDoorLock unlock decoder
  (`kFzEasycodeAction` → `action` = lock / zigbee_unlock / rfid_unlock /
  keypad_unlock), added the `soundVolume` (0x0024) read decoder with z2m's
  `lockSoundVolume` labels + the `sound_volume` / `auto_relock` writers,
  and corrected bindings to endpoint 11 closuresDoorLock (0x0101) +
  genPowerCfg (0x0001). The lock toggle now routes through the generic
  `kTzLock`.
- **Net2Grid N2G-SP command-action parity.** The SP31 metering outlet is
  both a controllable on/off plug and a command source — z2m wires
  `fz.command_on`/`fz.command_off` alongside `fz.on_off`/`fz.metering`. The
  auto-port kept only the attribute-report on/off + seMetering 0x0702
  energy/power and dropped both command converters, so the plug's genOnOff
  On/Off commands (physical button / bound group) never surfaced. Added the
  generic `kFzCommandOn`/`kFzCommandOff` + an `action` enum while keeping the
  on/off write path and metering channel. (No 0x0B04 gap: z2m uses
  `fz.metering`, 0x0702 only.) Graduated `Net_N2G_SP.cpp` out of `generated/`.
- **Bega LED-luminaire colour/colour-temperature parity.** `13557`
  (E27 RGBW bulb, z2m `m.light({colorTemp:{range:[153,556]}, color:true})`)
  and `85000` (Garden Spotlight RGBW, z2m
  `m.light({colorTemp:{range:[50,1000]}, color:{modes:["xy","hs"],
  enhancedHue:true}})`) had their entire lightingColorCtrl (0x0300) axis
  dropped — both were ported as on/off+brightness only. Restored colour +
  colour-temperature read/write (`kFzColorTemperature`/`kFzColor` +
  `kTzColorTemp`/`kTzColor`), the `color_temp`/`color_xy` exposes and the
  `{1,0x0300}` bind. Graduated to Tier 2; pinned by `test_bega_parity`.
  (Bega 70049 DALI control module remains unported: z2m matches it by
  fingerprint with an empty modelID + endpoint signature, which the
  `PreparedDefinition` schema cannot express — INFRA defer.)

- **EVN colour/colour-temperature + remote-action parity.** `ZB24100VS`
  (z2m `m.light({colorTemp:{range:[160,450]}, color:{modes:["xy","hs"]}})`)
  had its entire lightingColorCtrl (0x0300) axis dropped — ported as
  on/off+brightness only. Restored colour + colour-temperature read/write
  (`kFzColor`/`kFzColorTemperature` + `kTzColor`/`kTzColorTemp`, the colour
  exposes, and a 0x0300 bind). `ZBHS4RGBW`, a 4-channel RGBW battery remote
  (`toZigbee:[]`, action only), was wrong-bundled as a settable on/off
  switch (dead button presses, phantom relay); rewired to the genOnOff /
  genLevelCtrl / lightingColorCtrl command decoders + an `action` expose
  with `endpoint_map {ep1..ep4}` + `endpoint_action_suffix` (z2m
  multiEndpoint). `ZBPD23400` (plain dimmer) was already correct.
- **Ajax Online LED bulbs: restored dropped colour / colour-temperature
  axes.** All six z2m `m.light(...)` / `tuyaLight(...)` defs ship colour
  and/or colour-temperature, but every auto-port wired only
  `kFzOnOff + kFzBrightness` and exposed just state/brightness, dropping the
  whole `lightingColorCtrl` (0x0300) channel. Graduated all six to Tier 2:
  the full-colour bulbs (`Aj_Zigbee_Led_Strip`, `AJ_ZB_GU10`,
  `AJ_ZIGPROA60`, `ZB_A60_RGBCW`, and the Tuya `AJ_RGBCCT_CTRL`) now wire
  `kFzColor + kFzColorTemperature` / `kTzColor + kTzColorTemp` with
  `color_x/color_y/hue/saturation/color_temp` exposes; the CCT-only
  `ZB-CCT_Filament` wires `kFzColorTemperature` / `kTzColorTemp` +
  `color_temp` (and regains its z2m `ZB/Ajax Online` manufacturer gate).
  `AJ_RGBCCT_CTRL` was additionally a phantom-battery sensor (`kFzBattery` +
  battery/voltage exposes, no `to_zigbee`) on a mains RGB+CCT light — rebuilt
  as a full controllable light and the genPowerCfg (0x0001) battery binding
  removed.
- **J.XUAN sensor/button IAS dead-keys + WSZ01 phantom on/off.** PRZ01
  (occupancy) and DSZ01 (contact) wired the generic `kFzIasZone` (bare
  `alarm_1`/`alarm_2`) while z2m decodes them via
  `fz.ias_occupancy_alarm_1_with_timeout` / `fz.ias_contact_alarm_1` into
  semantic `occupancy` / `contact` keys — the advertised state was dead.
  Now wired to typed `kFzIasMotionAlarm` / `kFzIasContactAlarm` (the latter
  emitting z2m-inverted `contact = !bit0`). WSZ01 is a battery button
  (z2m `fz.WSZ01_on_off_action`, manuSpecific cluster `0xFE05` attr 1 →
  `action`) but was mis-ported as a controllable genOnOff switch with a
  phantom `state` and no action; now decodes `action`
  (release/single/double/hold) via the new `jxuan::kFzWsz01Action` over the
  newly-named `manuSpecificJxuan` cluster, with the phantom on/off + its
  toZigbee dropped and the truncated `JD-SWITCH02` model id restored.
- **Securifi Almond Click (B01M7Y8BP9 / ZB2-BU01): mis-ported as a dead
  on/off switch.** The auto-port wired `kFzOnOff` + `kTzOnOff` + a bare
  `state` expose, but the button never speaks genOnOff — it reports clicks
  on `ssIasAce` `commandArm`, reusing the `armmode` byte with a
  device-specific lookup (z2m `fz.almond_click`: 3 = single, 0 = double,
  2 = long; NOT the standard IAS-ACE arm-mode names). Graduated to a parent
  override wired onto a new vendor `kFzAlmondClick` decoder that emits
  `action`, with no on/off state and no writeable path (z2m `toZigbee:[]`).
  The Peanut Smart Plug (PP-WHT-US) was verified clean (z2m
  `fz.electrical_measurement`, 0x0B04 power/voltage/current, no 0x0702
  energy — already matched).

- **Nobø (Glen Dimplex) SWT-IZ / SPC-IZ thermostats decoded (wrong class).**
  Both electric-heater thermostats use z2m `m.thermostat()` but were
  auto-ported as a bare on/off + battery class (`kFzBattery` + `kFzOnOff`,
  exposing phantom `state`/`battery`/`voltage`), leaving the entire
  thermostat surface dead. Graduated both defs out of `generated/` and
  added `kFzNoboThermostat` (hvacThermostat 0x0201) emitting z2m's climate
  keys: `local_temperature` (0x0000), `occupied_heating_setpoint` (0x0012,
  NOT the generic dead key `current_heating_setpoint`),
  `unoccupied_heating_setpoint` (0x0014), `system_mode` (0x001C) and
  `running_mode` (0x001E, off/heat). Flat exposes; setpoints + system_mode
  writable via generic `kTzThermostat`.
- **Airzone Aidoo AZAI6ZBEMHI (Mitsubishi Heavy AC controller): dead cooling
  setpoint.** z2m's `e.climate()...withSetpoint("occupied_cooling_setpoint", ...)`
  is decoded by `fz.thermostat` from hvacThermostat attr 0x0011, but the generic
  `kFzThermostat` decodes only 0x0000/0x0012/0x001C, so the cooling setpoint was
  absent/dead. Graduated the def to a Tier-2 override and wired a new
  `kFzAirzoneThermostatExtras` (attr 0x0011 → `current_cooling_setpoint`)
  alongside the generic decoder, mirroring the Owon HVAC pattern. Also added the
  `current_cooling_setpoint` expose and corrected `system_mode` / `fan_mode` from
  Binary to Enum (z2m `withSystemMode` / `withFanMode`). `fan_mode` already
  matched the generic `kFzFanMode` key.
- **Javis JS-MC-SENSOR-ZB / JS-SLK2-ZB parity.** The microwave/presence
  sensor (TS0601 `_TZE200_lgstepha`/`_kagkgk0i`/`_i0b1dbqu`) was mis-wired
  to the IAS-zone converter with phantom `alarm`/`tamper`/`battery_low`
  exposes and a 0x0500 bind; z2m decodes it over the 0xEF00 Tuya DP stream
  (`legacy.fz.javis_microwave_sensor`). Ported the real DP→key map
  (presence_state + occupancy on DP1, illuminance, sensitivity, keep_time,
  led_enable, illuminance_calibration) and split the `_TZE200_kagkgk0i`
  DP102/106/107 remap into its own def. The JS-SLK2-ZB biometric lock was
  mis-ported as a controllable on/off switch (kFzOnOff + kTzOnOff + StateSet
  `state` + genOnOff bind); z2m has an empty toZigbee and exposes
  `action(["unlock"])` + battery — dropped the phantom control. The raw
  genBasic 0x4200 lock-report decode (custom UTF-8/timer shape) is deferred
  as infra.

- **Light Solutions: restored dropped color/colorTemp axes on two bulbs.** The
  auto-ported `42-032` CCT LED driver (z2m `m.light({colorTemp})`) and `91-943`
  RGBW+CCT LED controller (z2m `m.light({color, colorTemp})`) shipped as bare
  OnOff+Brightness dimmers. Graduated both to Tier 2 overrides wiring the
  generic color/colorTemp converters, the `color_temp` (+ `color_xy` for 91-943)
  exposes and the `lightingColorCtrl` (0x0300) binding.
- **Smartenit ZBHT-1 + 4040B parity.** The ZBHT-1 temperature & humidity
  sensor was auto-ported to battery-only, silently dropping `fz.temperature`
  (0x0402) and `fz.humidity` (0x0405); restored both decoders, exposes, and
  report bindings. The 4040B dual-load metering switch lost the z2m endpoint
  map `{l1:1, l2:2}`, so genOnOff reports from both relays collided on a
  single bare `state`; added the `endpoint_map` (per-load `state_l1`/
  `state_l2`) plus ep2 genOnOff+seMetering bindings. `power`/`energy` are now
  in the dispatch always-global blocklist so multi-load metering switches
  keep z2m's single un-suffixed `e.power()`/`e.energy()` exposes.

- **Domraem DOM-Z-105P LED controllers regained their color/colorTemp axes.**
  The auto-generator collapsed every `m.light(...)` variant to on/off +
  brightness only. Restored the dropped 0x0300 axes per modelID to match z2m:
  `RGB` (hs color) regained hue/saturation; `RGBW` + `RGBCCT` regained
  colorTemp (158–495 mired) + xy + hs; `CCT` (158–495) + `WW/CW` (158–500)
  regained colorTemp. `DIMMER` was already correct. Five generated defs
  graduated to Tier 2 with `kFzColor`/`kFzColorTemperature` +
  `kTzColor`/`kTzColorTemp`; pinned by `tests/test_domraem_parity.cpp`.
- **Contact-sensor polarity matched to z2m (systemic).** The generic
  `kFzIasContactAlarm` emitted the raw IAS zoneStatus bit 0 as `contact`,
  but z2m publishes `contact = !(bit0)` for every `zoneType:"contact"`
  device (a closed door = magnet present = bit0 clear = `contact:true`),
  via both `fz.ias_contact_alarm_1` and the modernExtend `iasZoneAlarm`
  (`invertAlarmPayload = zoneType==="contact"`). All ~82 contact defs that
  wire the converter were therefore inverted vs z2m. Added an `invert` flag
  to the typed-IAS label; `kFzIasContactAlarm` now inverts. The lone z2m
  `invertAlarm:true` device (VSmart HS-SEDR, which double-inverts back to
  raw bit0) uses the new non-inverting `kFzIasContactAlarmNI`. Only contact
  inverts — motion/smoke/water/gas/CO/vibration stay raw bit0.

- **Technicolor XHK1-TC (Xfinity security keypad): phantom on/off + IAS dead-key
  + dropped temperature + dead `action_transaction`.** Sibling of the
  UniversalElectronicsInc XHK1-UE. The auto-port wired a phantom `kFzOnOff`/
  `kTzOnOff` (`state` expose, bound 0x0006) the keypad has no such cluster, the
  generic dead-key `kFzIasZone` (emitting a bare `alarm` while exposes declare
  `occupancy`/`contact`), dropped temperature entirely, and used `kFzIasAceArm`
  which drops the ZCL TSN — leaving the declared `action_transaction` expose
  dead. Rewired to `kFzBattery` + `kFzTemperature` + typed `kFzIasMotionAlarm`
  (→`occupancy`) + `kFzIasContactAlarm` (→`contact`) + `kFzIasAceArmWithTransaction`
  so the ZCL transaction sequence number now reaches `action_transaction`,
  mirroring z2m `fz.command_arm_with_transaction`. Graduated to a Tier-2 hand
  def. (`fz.ias_ace_occupancy_with_timeout` getPanelStatus and `tz.arm_mode` /
  `m.iasGetPanelStatusResponse` armRsp encoders remain runtime gaps, as for the
  XHK1-UE sibling.)
- **eCozy 1TST-EU thermostat: dead pi_heating_demand / running_state /
  local_temperature_calibration.** z2m's `ecozy.ts` wires `fz.thermostat`,
  which decodes the full hvacThermostat attribute set, and the climate block
  exposes `pi_heating_demand` (0x0008), `running_state` (0x0029) and
  `local_temperature_calibration` (0x0010). The auto-port wired only the
  generic `kFzThermostat`, which decodes just 0x0000/0x0012/0x001C — so all
  three keys were exposed-but-dead, and `system_mode` was a `Binary` stub.
  Graduated to Tier 2 with `kFzEcozyThermostatExtras` (delegates to the
  generic decoder, then adds pi_heating_demand mapped 0-255→0-100 %, lt_cal
  raw/10 °C as float, running_state {idle,heat}) and upgraded the exposes to
  the real climate-flat surface (Enum `system_mode`/`running_state` + the
  three keys). keypad_lockout (hvacUserInterfaceCfg 0x0204) is bound by z2m
  but never decoded/exposed there, so it is not a gap.
- **Lutron remotes: dead `action`, phantom relay.** Both Lutron ZHA entries —
  `LZL4BWHL01` (Connected Bulb Remote) and `Z3-1BRL` (Aurora rotary dimmer) —
  are battery remotes that drive a paired bulb by emitting `genLevelCtrl`
  commands (`fz.command_step` / `command_move_to_level` / `command_stop`) with
  `toZigbee: []`, exposing only `action`. The auto-generator wrong-bundled both
  as settable on/off lights (`kFzOnOff` + `kTzOnOff` + a writable `state`),
  dropping the `action` decode entirely so every rotary turn / button press was
  dead and the device falsely advertised a relay. Graduated both to Tier 2:
  wired `kFzCommandStep` / `kFzCommandMoveToLevel` / `kFzCommandStop`, added an
  `action` expose + the `{1,0x0008}` genLevelCtrl bind, dropped the phantom
  state and toolbox; kept the `Z3-1BRL` battery.

- **Databyte Touch4 + DTB190502A1: dropped custom genOnOff manufacturer-attr decoders.**
  Both databyte.ch devices hang vendor attributes off the standard genOnOff
  cluster (0xA191..0xA194) and the auto-ports dropped them. `Touch4`
  (DTB-ED2011-014) was battery-only so the four touch keys never surfaced; it
  now wires a vendor converter emitting `key_1`..`key_4` (z2m `fzLocal.DTB2011014`).
  `DTB190502A1` (CC2530 IO board) fabricated a controllable on/off `state` + a
  phantom battery/voltage bundle; it now wires `fz.DTB190502A1` emitting
  `led_state`/`key_state`/`cpu_temperature` and carries z2m's exposes (no
  battery, no `state`). New shared converters in `definitions/databyte/_shared.{hpp,cpp}`;
  both defs graduated to Tier-2 parent overrides. (The suspected 4-gang
  endpoint_map collision was a false flag — z2m uses four manufacturer
  attributes on one endpoint, not per-endpoint genOnOff state.)
- **Nordtronic metering dimmers/relays: dropped 0x0B04 half + dead remote.**
  The four metering devices `98424072` (rotary), `98425271` (Box Dimmer G2),
  `98425033` (ceiling micro) and `98425034` (DIN rail) are z2m `m.light()` +
  `m.electricityMeter()`. z2m's `electricityMeter()` defaults to
  `cluster:"both"` (AC), layering `haElectricalMeasurement` 0x0B04
  (power/voltage/current) on top of `seMetering` 0x0702 (energy) and binding
  both. The auto-ports wired only `kFzMetering` (0x0702) and exposed only
  energy+power — dropping the entire 0x0B04 half, so voltage and current never
  decoded. Graduated all four to Tier 2: added `kFzElectricalMeasurement`,
  `voltage`/`current` exposes and the `{1,0x0B04}` bind. The `98426061`
  "Remote Control" is, in z2m, a command client (`m.commandsOnOff` +
  `m.commandsLevelCtrl` + `m.commandsColorCtrl` + `m.battery`) but was
  mis-ported as a controllable on/off switch (`kFzOnOff` + `kTzOnOff` + a
  `state` expose) with a dead action stream; graduated to Tier 2 wiring the
  generic `kFzCommand*` converters + an `action` expose, dropping the phantom
  on/off state and `to_zigbee` setter. (`sunricher.externalSwitchType` on
  `98425033` is a config-only manuSpecific enum — noted, deferred as infra.)
- **Aeotec Pico switches/shutter + range extender: metering half, cover
  tilt, device temp, scene-button channels, and a phantom router on/off.**
  `WG001` ("Range extender Zi") is a router (z2m `exposes:[]`,
  `fromZigbee:[fz.linkquality_from_basic]`) but was auto-ported as a
  controllable on/off (`kFzOnOff`/`kTzOnOff` + a `state` expose + 0x0006
  bind) — stripped to an exposeless def. `ZGA002`/`ZGA003` use
  `m.electricityMeter()` (default `cluster:"both"`) but the port wired only
  `kFzMetering` (seMetering 0x0702 → energy), dropping the
  haElectricalMeasurement 0x0B04 half (voltage/current) — added
  `kFzElectricalMeasurement` + voltage/current exposes + the 0x0B04 bind.
  `ZGA004` ("Pico shutter") is `m.windowCovering({controls:["lift","tilt"]})`
  but the port wired only `kFzCoverPosition`/`kTzCoverPosition` (lift),
  dropping the tilt channel (currentPositionTiltPercentage 0x0009) and
  carrying a phantom on/off — added `kFzCoverTilt` + `kTzCoverPositionTilt`
  + a `tilt` expose and removed the dead `state`/0x0006. All three `ZGA*`
  also dropped `m.deviceTemperature()` (genDeviceTempCfg 0x0002) and the
  `m.commandsOnOff`/`commandsLevelCtrl`/`commandsWindowCovering`
  scene-button endpoints — added `kFzDeviceTemperature` +
  `device_temperature` and the generic `kFzCommand*` converters + an
  `action` enum with `endpoint_action_suffix` so each button endpoint
  surfaces as `action_<n>`. Graduated all four defs to Tier 2.
- **Repenic RD-250ZG: dropped electrical-measurement half of the meter.** The
  dimmer is z2m `m.light()` + `m.electricityMeter()`, whose default
  `cluster:"both"` decodes both seMetering 0x0702 (energy) and
  haElectricalMeasurement 0x0B04 (power/voltage/current), with z2m sourcing
  `power` from 0x0B04 (it deletes `seMetering.power` in "both" mode). The
  auto-port wired only the generic `kFzMetering` (0x0702) and exposed
  energy+power, so `voltage`/`current` had neither decoder nor expose and
  `power` had no live decoder. Graduated `Rep_RD_250ZG` to Tier 2, added the
  generic `kFzElectricalMeasurement` (0x0B04) converter, the `voltage`/
  `current` exposes, and the 0x0B04 reporting bind. (The genLevelCtrl
  manufacturer-specific config attributes — min/max/start brightness, boost,
  dimming_mode, default_move_rate — need read/write of arbitrary attribute
  IDs with no generic converter; left as INFRA defer.)
- **Onenuo 288WZ smoke detector: dead alarm + self-test channels.** The
  TS0601 detector (`_TZE204_kgaxpvxr` / `_TZE284_n4ttsck2`) is a Tuya-DP
  (0xEF00) device, but its auto-ported defs kept only 3 of z2m's 5
  datapoints — dropping DP1 (the actual `smoke` alarm + `smoke_state`) and
  DP101 (`self_test_result`) — and declared phantom `state`/`action` binary
  exposes the device never reports. A sibling descriptive stub also
  mis-wired the detector to ssIasZone (0x0500) instead of the DP map.
  Completed both DP maps to match z2m: DP1 enum {alarm,normal,detecting,
  unknown} now fans into both a string `smoke_state` and a boolean `smoke`
  (true only in the alarm state), DP101 boolean fans to failure/success,
  and the exposes now mirror z2m (`smoke`, `smoke_state`, `self_test_result`,
  `battery`, `silence`, `sensitivity`). Added a reusable
  `kTuyaDpFlagEnumBool` to the Tuya DP infra for the enum→boolean alarm
  fan-out. Graduated both defs out of `generated/` and added
  `tests/test_onenuo_parity.cpp`.

- **Prolight bulbs/remote: dropped colour axis + dead remote.** The E27
  (`5412748727371`) and GU10 (`5412748727401`) white-and-colour bulbs are
  z2m `m.light({colorTemp:{range:[153,555]}, color:true})` but were
  auto-ported as on/off + brightness only — the 0x0300 lightingColorCtrl
  axis (`color_temp` + `color_xy` exposes and the cluster bind) was missing,
  so colour and colour-temperature reports decoded to nothing. The remote
  (`5412748727388`) is an action-only ZLL transmitter (`toZigbee:[]`) but
  was mis-ported as a controllable on/off switch (`kFzOnOff` + `kTzOnOff` +
  a `state` expose), leaving the action stream dead. Graduated all three to
  Tier 2: wired `kFzColorTemperature`/`kFzColor` (+ `kTzColorTemp`/`kTzColor`)
  on the bulbs, and the generic Client→Server command converters
  (`kFzCommandOn`/`Off`/`MoveToLevel`/`Move`/`Stop`/`MoveToColorTemp`/
  `MoveToColor`/`MoveColorTemperature` + `kFzBattery`) plus an `action` enum
  on the remote, dropping its phantom on/off state and `to_zigbee` setter.
- **LUX KONOz KN-Z-WH1-B04 thermostat: dead cooling-setpoint + running
  state.** The HVAC thermostat's climate expose declares
  `occupied_cooling_setpoint` and `withRunningState(["idle","heat","cool"])`,
  both decoded by z2m's `fz.thermostat` from standard hvacThermostat attrs
  0x0011 (OccupiedCoolingSetpoint) and 0x0029 (ThermostatRunningState). The
  auto-port wired only the generic `kFzThermostat` (0x0000/0x0012/0x001C),
  so both were dead exposes; `running_mode` (0x001E, also z2m-decoded) was
  likewise dropped. Added a vendor `_shared.{hpp,cpp}` with
  `kFzLuxThermostatExtras`, graduated the def to Tier 2 (added the
  `occupied_cooling_setpoint` / `running_state` / `running_mode` exposes),
  and authored `tests/test_lux_parity.cpp`. The `fan_mode` channel was
  already correctly decoded by the generic `kFzFanMode` (regression-tested).
- **iHORN sensors: IAS dead-key + dropped temp/humidity channels.** All seven
  iHORN defs lowered the generic `kFzIasZone` (bare key `alarm`) while exposing
  a semantic key, so the primary state never reached the shadow. Graduated each
  to a Tier-2 override with the typed converter matching z2m's `fz.ias_*_alarm_1`
  (all bit 0): `kFzIasMotionAlarm`→`occupancy` (LH-992ZB/LH-990ZB/LH-990F),
  `kFzIasContactAlarm`→`contact` (HO-09ZB/LH03121), `kFzIasSmokeAlarm`→`smoke`
  (LH-09521 siren; `tz.warning` write path unchanged). The LH-32ZB
  temperature+humidity sensor was reduced to battery-only — restored
  `kFzTemperature` (0x0402) + `kFzHumidity` (0x0405) decoders, both exposes, and
  the 0x0402/0x0405 bindings. Added `tests/test_ihorn_parity.cpp`.
- **Qoto QT-05M watering timer: dead on/off+battery stub, all 7 watering
  datapoints dropped.** The solar garden watering timer (TS0601 /
  `_TZE200_arge1ptm`, `_TZE200_anv5ujhv`, `_TZE200_xlppj4f5`) was auto-ported
  as a bare `kFzBattery` + `kFzOnOff` stub binding ZCL `0x0001`/`0x0006` it
  never speaks, with phantom `state`/`voltage` exposes. It is a Tuya-MCU
  (`0xEF00`) device — z2m decodes via `legacy.fromZigbee.watering_timer`.
  Graduated to a Tier-2 DP-map override (`fz_tuya_datapoints` +
  `tz_tuya_datapoints`) restoring `water_flow` (DP 3), `last_watering_duration`
  (DP 107), `remaining_watering_time` (DP 101), `valve_state` (DP 102),
  `valve_state_auto_shutdown` (DP 2), `shutdown_timer` (DP 11) and `battery`
  (DP 110). Per the giex precedent, the float-divisor suspect was FALSE: z2m
  passes every value DP through raw, so all DPs decode at divisor 1.
- **Airam CTR.U / CTR.UBX remotes: dead `action`, mis-ported as on/off
  switches.** Both command-style dimmer remotes (`ZBT-Remote-EU-DIMV1A2`
  / `ZBT-Remote-EU-DIMV2A2`) were auto-ported as controllable on/off
  switches (`kFzOnOff` + `kTzOnOff` + a dead `state` expose), dropping
  every button press. z2m wires `fz.command_on/off/step/move/stop/recall`
  and exposes a composite `action`. Fixed by graduating both defs and
  wiring the generic genOnOff/genLevelCtrl/genScenes command converters
  (`kFzCommandOn/Off/Step/Move/Stop/Recall`) so they publish `action`
  (`on`/`off`/`brightness_step_*`/`brightness_move_*`/`brightness_stop`/
  `recall_<n>`), with no write path.

- **FireAngel CO sensors: dead/missing carbon-monoxide channels.** The
  W2-Module (`Alarm_SD_Device`) was auto-ported battery-only — z2m wires
  `fz.W2_module_carbon_monoxide` and reports CO on ssIasZone zoneStatus
  *bit 8* (not the standard bit 0 the generic `kFzIasCoAlarm` reads). The
  ZBCO-AE-10X-EUR CO alarm was missing from the registry entirely. Added a
  vendor `_shared.{hpp,cpp}` with `kFzW2ModuleCarbonMonoxide` (bit 8) +
  `kFzFireangelCoTest` (test = bit 5 OR bit 9), graduated W2-Module to
  Tier 2 (restored carbon_monoxide expose + 0x0500 binding), and authored
  the manufacturer-gated ZBCO def (generic `kFzIasCoAlarm` for
  carbon_monoxide/tamper/battery_low + the test decoder). The two defs share
  model `Alarm_SD_Device`; the mfr-gated ZBCO wins find_definition Pass 1 for
  FireAngel hardware, W2-Module is the model-only fallback.
- **Frient (Develco rebrand) sensors/meters: IAS dead-key + lost
  temperature + missing 0x0B04 electrical half.** `WISZB-131` lowered the
  generic `kFzIasZone` (bare `alarm`) and dropped the temperature channel —
  z2m wires `m.iasZoneAlarm({zoneType:"contact"})` + `m.temperature()`, so
  it now uses `kFzIasContactAlarm` (semantic `contact`) + `kFzTemperature`
  (0x0402). `SMRZB-153` and `EMIZB-151` wired only `kFzMetering` (0x0702)
  but z2m's `m.electricityMeter()` default cluster is `"both"`, so the
  0x0B04 electrical half (`voltage`/`current`) was dead — added
  `kFzElectricalMeasurement` + the exposes + the 0x0B04 binding. All three
  graduated generated -> Tier 2, mirroring the develco fixes. Both also set
  their metering/onOff bindings to **endpoint 2** to match z2m's
  `endpoint:()=>({default:2})`; `SMRZB-153` additionally sets
  `default_endpoint=2` so its outbound on/off control routes to the real
  endpoint (was dead on EP1). (EMIZB-151 three-phase/produced-energy/tariff
  attrs remain INFRA-deferred: no generic decoder. SIRZB-112 siren on EP43
  stays INFRA-deferred — warning control is to-zigbee-only and its EP43
  bindings are a documented follow-up.)
- **Zunzunbee SSWZ8T scene switch: remote mis-ported as a settable
  on/off.** The 8-button battery "Slate switch" was auto-ported as a
  controllable on/off `state` (`kFzOnOff` + `kTzOnOff`), a dead control,
  which also dropped the entire button-press decode and the temperature
  channel. z2m has no on/off; it packs the pressed button + press type
  into the `ssIasZone` zoneStatus word (bit 0 = short/long, bits 1..8 =
  button number one-hot) and surfaces a single composite `action`.
  Graduated the def to Tier 2, added the vendor `kFzZunzunbeeSlateSwitch`
  zoneStatus decoder (emits `button_<n>_<short|long>_press`), wired
  `kFzBattery` + `kFzTemperature`, swapped the phantom `state`/to_zigbee
  for an `action` enum + `temperature` expose, dropped the bogus genOnOff
  bind, and pinned the manufacturer name (`zunzunbee`).
- **Woolley BSD29/BSD59 smart plug: dropped metering channels.** The
  generated def wired the generic `kFzElectricalMeasurement`
  (haElectricalMeasurement 0x0B04) for its power/voltage/current exposes,
  but z2m's `fzLocal.BSD29` decodes a manufacturer-specific cluster 0xFC11
  (decimal 64529) — attrs 0x7006/0x7005/0x7004 (power/voltage/current),
  each scaled `/1000`. Against 0x0B04 every metering report was dropped on
  read. Named 0xFC11 as `manuSpecificWoolley` in `cluster_names.hpp`,
  graduated the def to Tier 2 with an inline `kFzWoolleyElectricity` Fz
  that decodes the three vendor attrs as Float = raw/1000, and dropped the
  phantom 0x0B04 binding (z2m's `configure()` binds only genOnOff).

- **Alecto SMART-HEAT10 / SMART-SMOKE10: Tuya-DP misroute (IAS/standard-cluster instead of 0xEF00).**
  Both TS0601 Tuya-MCU devices were auto-ported to the wrong decode path:
  the SMART-HEAT10 TRV to the generic `kFzThermostat` (standard hvacThermostat
  0x0201) and the SMART-SMOKE10 to a generic IAS zone (ssIasZone 0x0500). Both
  speak only the 0xEF00 manuSpecificTuya DP stream, so every channel was dead.
  z2m decodes them via `legacy.fz.tuya_thermostat` / `legacy.fz.tuya_alecto_smoke`.
  Graduated both defs to Tier 2 and wired `fz_tuya_datapoints` + `tz_tuya_datapoints`
  with the full DP maps — HEAT10: setpoint (DP2 /10), local_temperature (DP3 /10),
  system_mode (DP4 enum off/auto/heat), child_lock (DP7), window_detection (DP18),
  battery (DP21); SMOKE10: smoke_state (DP1), smoke_value (DP2), self_checking (DP8),
  checking_result (DP9), lifecycle (DP12), battery_state (DP14), battery (DP15),
  silence (DP16). New `tests/test_alecto_parity.cpp` covers both.
- **Purmo/Radson Yali Parada Plus thermostat: dead thermostat extras.**
  The `PUMM01102` electric oil-filled radiator was auto-ported with only
  the generic `kFzThermostat`, which decodes just hvacThermostat
  0x0000/0x0012/0x001C. z2m wires `fz.thermostat` and exposes the full
  climate surface, so `unoccupied_heating_setpoint` (0x0014),
  `local_temperature_calibration` (0x0010), `running_mode` (0x001E),
  `running_state` (0x0029), `max_heat_setpoint_limit` (0x0016) and
  `occupancy` (0x0002) were exposed-but-dead. Graduated the def to Tier 2,
  added `kFzPurmoThermostatExtras` (delegates to the generic decoder then
  adds the missing attrs), wired the matching write path (unoccupied
  setpoint, max-heat limit, local-temp calibration, occupancy), restored
  the full flat expose surface, re-typed `system_mode` from Binary to
  Enum, and pinned the manufacturer name (`computime`).
- **Casaia CTHS-317-ET parity: dropped temperature channel.** The
  generated def for this remote temperature probe was reduced to
  battery-only (`kFzBattery` + battery/voltage exposes), dropping z2m's
  `fz.temperature` decoder and `e.temperature()` expose — the entire
  purpose of the device — plus the `e.battery_low()` expose. Graduated to
  a Tier-2 override adding `kFzTemperature` (msTemperatureMeasurement
  0x0402, /100), the `temperature` + `battery_low` exposes, and the 0x0402
  binding on endpoint 3. `battery_low` already decodes via `kFzBattery`
  (batteryAlarmState attr 0x0035). New `test_casaia_parity`. The two AC
  relay modules (CSLC601-D-E, CSAC451-WTC-E) are plain `m.onOff()` in z2m
  and the CCB432 energy meter already wires both metering halves
  (`kFzMetering` 0x0702 + `kFzElectricalMeasurement` 0x0B04) — verified
  correct, no change.

- **Lupus (LUPUSEC) roller-shutter parity: wrong-cluster cover.** The `12031`
  and `LS12128` SCM-family roller shutters were ported with the generic
  `kFzCoverPosition` / `kTzCoverPosition` converters against
  `closuresWindowCovering` (0x0102) and bound 0x0102 — a cluster these devices
  never speak. z2m drives them through the brightness/on-off proxy
  (`fz.cover_position_via_brightness` + `fz.cover_state_via_onoff` +
  `tz.cover_via_brightness`), so position decode and control were both dead and
  the `cover_position()` `state` expose was dropped. Rewired both to the generic
  via-brightness converters (genLevelCtrl 0x0008 + genOnOff 0x0006), restored
  the `state` expose, and corrected bindings (mirrors the Climax SCM-5ZBS fix).
  Added `tests/test_lupus_parity.cpp` (also pins the `12127` two-channel relay
  l1/l2 endpoint_map suffixing).
- **smarli. parity: dropped 0x0B04 electrical-measurement half + cover tilt.**
  All three smarli. devices use z2m `m.electricityMeter()`, which defaults to
  `cluster:"both"` — `seMetering` 0x0702 (energy) **plus**
  `haElectricalMeasurement` 0x0B04 (power/voltage/current). The auto-ports
  wired only `kFzMetering` (0x0702) and dropped the 0x0B04 half, so on
  `S-ZB-PDM1-R251` (phase dimmer), `S-ZB-1RE1-R251` (2ch relay) and
  `S-ZB-COV1-R251` (curtain) the `current`/`voltage` channels were absent and
  `power` had no decoder. Added `kFzElectricalMeasurement` + `current`/`voltage`
  exposes + the 0x0B04 binding to all three (on 1RE1/COV1 the meter lives on
  endpoint 3, so its runtime keys are suffixed `_3` via the endpoint_map; the
  meter bindings were re-pointed to ep3). `S-ZB-COV1-R251` additionally declared
  z2m `m.windowCovering({controls:["lift","tilt"]})` but was wired with only
  `kFzCoverPosition` (attr 0x0008), dropping the tilt channel — added
  `kFzCoverTilt` (attr 0x0009) + `kTzCoverPositionTilt` + the `tilt` expose. The
  `curtain_type` enum (manuSpecific attr 0x1000) and the sunricher motor/PWM
  config extends remain INFRA (no generic converter) and are deferred. New
  `test_smarli_parity`.
- **MSH (MySmartHouse) DIY sensor parity: dropped channels + wrong class.**
  All three generated `msh` defs were reduced to non-functional ports.
  `msh.bme280psm` (z2m `[battery, temperature, humidity, pressure]`) and
  `msh.ds18b20psm` (`[battery, temperature]`) were stripped to battery-only —
  restored the generic `kFzTemperature`/`kFzHumidity`/`kFzPressure` decoders,
  exposes, and bindings (`msTemperatureMeasurement` 0x0402, `msRelativeHumidity`
  0x0405, `msPressureMeasurement` 0x0403). `msh.AirQMon` was mis-ported as a
  bare on/off switch with a phantom battery; rewired to its real
  `co2` (`fz.co2` on `msCO2` 0x040D, ppm = floor(value·1e6)) and
  `temperature` (`msTemperatureMeasurement` on endpoint 3, suffixed
  `temperature_l3` via `endpoint_map`) channels. The PTVO UART/analog
  (`genAnalogInput` 0x000C) action/calibrate channels remain deferred INFRA.
  Added `tests/test_msh_parity.cpp`.

- **HZC Electric sensor parity: phantom on/off, IAS dead-keys, dropped
  channels.** Three generated sensor defs were mis-ported. `S093TH-ZG`
  (temp/humidity) was wired as a phantom on/off switch (`kFzOnOff` +
  `state` expose + `kTzOnOff`) instead of z2m's `fz.temperature` +
  `fz.humidity` — restored `kFzTemperature` + `kFzHumidity`, the
  temperature/humidity exposes and 0x0402 / 0x0405 bindings, dropped the
  dead write path. `S900W-ZG` (water leak) and `S902M-ZG` (motion) lowered
  the generic `kFzIasZone` (bare key `alarm`) while their exposes declared
  the semantic key — graduated to the typed `kFzIasWaterLeakAlarm` /
  `kFzIasMotionAlarm` (`water_leak` / `occupancy`, zoneStatus bit 0).
  `S902M-ZG` also dropped z2m's `m.illuminance()` — restored
  `kFzIlluminance` + the `illuminance` expose + 0x0400 binding. New
  `test_hzc_electric_parity`.
- **LivingWise LVS parity: dead IAS keys + scene controller mis-ported as a switch.**
  The LVS-SM10ZW contact and LVS-SN10ZW_SN11 occupancy sensors were
  auto-ported with the generic `kFzIasZone` (which emits the bare `alarm`
  key) while exposing the semantic `contact` / `occupancy` keys, so the
  alarm signal never reached the shadow. Swapped to the typed
  `kFzIasContactAlarm` / `kFzIasMotionAlarm` converters (matching z2m
  `fz.ias_contact_alarm_1` / `fz.ias_occupancy_alarm_1_with_timeout`,
  zoneStatus bit 0 → semantic key + bit 2 tamper + bit 3 battery_low) and
  renamed the `alarm` expose to `contact` / `occupancy`. The LVS-SC7 7-button
  scene controller was generated as a controllable on/off switch (phantom
  `state` + `kFzOnOff`/`kTzOnOff`); it is a stateless remote that z2m
  decodes via `fz.orvibo_raw_2` (vendor-private raw frame on cluster 0x0017
  → `action` = `button_<n>_<click|hold|release>` for buttons 1..7). Reused
  the existing shared `kFzOrviboRaw2Action` converter, dropped the dead
  `state`, and added an `action` expose. All three defs graduated to Tier-2
  parents; new `tests/test_livingwise_parity.cpp` fixture.

- **IMOU (Dahua) sensor family parity: IAS dead-keys + dropped channels.**
  All six IMOU battery sensors were auto-ported onto the generic
  `kFzIasZone` (emits `alarm_1`/`alarm_2`) or battery-only, so the primary
  channels never reached the shadow. Graduated each to a Tier-2 override:
  ZP1-EN PIR now decodes `occupancy` (`kFzIasMotionAlarm`); ZR1-EN siren +
  ZD1-EN contact decode the bare `alarm` key (`kFzIasGenericAlarm`, matching
  z2m's `alarm` zoneType); ZGA1-EN gas now emits the split `gas_alarm_1`/
  `gas_alarm_2` + `test` keys and drops the phantom `battery_low` (new
  imou-local `kFzImouGasAlarm`); ZTM1-EN re-wires the dropped `temperature`
  (0x0402) + `humidity` (0x0405) channels; ZE1-EN wireless switch decodes
  `action:"press"` on zoneStatus==2 (new imou-local `kFzImouAlarmButton`).
  New `definitions/imou/_shared.{hpp,cpp}` + `test_imou_parity`. (z2m
  `m.iasWarning()` siren control on ZR1-EN deferred — to-zigbee infra.)
- **Weten PCI E parity: misrouted/split Tuya-DP def + truncated enum.** The
  single z2m "PCI E" (Remote Control PCI E Card, Tuya TS0601 0xEF00 DP) device
  had been auto-ported as three colliding artifacts — a misrouted bare genOnOff
  def (cluster 0x0006) carrying both `_TZE204_6fk3gewc`/`_TZE284_6fk3gewc`
  names, plus two model-only DP defs split one-per-manufacturer-name (so neither
  matched a manufacturer fingerprint). Consolidated into one `kDef_PCI_E`
  carrying both manufacturer names, the "Weten / Tuya PRO" white-label and the
  real model "PCI E", wired to the DP map. Real decode fix: DP101 `restart_mode`
  shipped only `{0:"restart"}` while z2m maps `{restart:0, "force restart":1,
  "–":2}`, so `force restart` and `–` reports silently dropped.

- **Zen Zen-01-W parity: dead cooling setpoint / running_state / calibration.**
  The heat+cool+fan thermostat (z2m `fz.thermostat` + `fz.fan` + `fz.battery`)
  was auto-ported with only the generic `kFzThermostat`, which decodes just
  0x0000 / 0x0012 / 0x001C — leaving `occupied_cooling_setpoint` (0x0011),
  `running_state` (0x0029) and `local_temperature_calibration` (0x0010) dead,
  and omitting those three exposes entirely while mistyping `system_mode` /
  `fan_mode` as Binary. Graduated to a Tier-2 override: added the vendor
  `kFzZenThermostat` (`definitions/zen/_shared.cpp`) wired alongside the generic
  decoder + `kFzFanMode`, restored the missing exposes (enums for
  system_mode/fan_mode/running_state), and fixed the expose types. New
  `test_zen_parity`.
- **JetHome WS7 parity: phantom on/off, dead discrete-input channel.** The
  3-channel battery discrete-input module was auto-ported with a phantom
  genOnOff in/out (settable `state` expose + `kFzOnOff`/`kTzOnOff` +
  0x0006 binding) and dropped the entire `genMultistateInput` channel that
  z2m decodes via `jetHome.fz.multiStateAction`. Graduated to a Tier-2
  override: wired `kFzBattery` + new `kFzJetHomeMultiStateAction`
  (presentValue 0x0055 → release/single/double/triple/hold), restored the
  `JetHome` manufacturer gate, replaced `state` with an `action` enum, and
  set `endpoint_action_suffix` so each input surfaces as
  `action_in1`/`action_in2`/`action_in3`. New `test_jethome_parity`.
- **LED-Trading 9133 parity: Green Power pushbutton mis-ported as on/off relay.**
  The `9133` "Pushbutton transmitter module" (fingerprint modelID
  `GreenPower_2`) is a self-powered Green Power transmitter — z2m gives it
  `toZigbee: []` and a single `e.action([press_1, hold_1, … hold_4])` decoded
  via `cluster:"greenPower"` `commandNotification`. The auto-port mis-classified
  it as a controllable relay (`kFzOnOff`/`kTzOnOff` + dead `state` Binary +
  genOnOff 0x0006 bind). Graduated to Tier 2 and re-shaped to the z2m role: a
  semantic `action` enum, non-controllable, no bindings. The commandID→action
  decode stays a documented Green Power INFRA defer (no Green Power frame
  family in the parser yet), mirroring the enocean PTM 21x precedent. The 9134
  5-gang powerstrip (l1..l5 `endpoint_map`), 9135 cover and HK-LN-DIM-A dimmer
  were verified parity-correct and pinned with regression tests.
- **Schwaiger parity: restored dropped LED colour/CCT channels.** The
  HAL300/HAL550/HAL800 (`m.light({colorTemp, color})`) and HAL500
  (`m.light({colorTemp})`) bulbs were auto-ported as bare on/off +
  brightness, dropping the entire `lightingColorCtrl` (0x0300) channel.
  Graduated to Tier 2 + wired `kFzColorTemperature`/`kTzColorTemp` (all
  four) and `kFzColor`/`kTzColor` (the three RGB models), added the
  `color_temp`/`color_x`/`color_y`/`hue`/`saturation` exposes and the
  0x0300 binding. ZHS-15 verified already correct (z2m
  `fz.electrical_measurement` = 0x0B04 power/voltage/current only, no
  0x0702 energy) and pinned by a regression test.
- **Intuis parity: dead thermostat extras, occupancy and keypad UI.** The
  `intuisradiator` electric heater (Muller Intuitiv) wires z2m `fz.thermostat`
  + `fz.hvac_user_interface` + `m.occupancy` + `m.electricityMeter`, but the
  auto-port used only generic `kFzMetering` + `kFzThermostat`. The generic
  thermostat decoder emits attr 0x0012 as `current_heating_setpoint` (the def
  declares `occupied_heating_setpoint`) and never touches 0x0014
  (`unoccupied_heating_setpoint`) or 0x0029 (`running_state`); occupancy
  (`msOccupancySensing` 0x0406) and `hvacUserInterfaceCfg` 0x0204
  (`keypad_lockout`, `temperature_display_mode`) had no decoder — all dead
  exposes. Added `kFzIntuisThermostat` + `kFzIntuisHvacUi` (definitions/intuis/
  _shared) and wired generic `kFzOccupancy`; graduated the def out of
  `generated/`, corrected the exposes (dropped the phantom `power` — z2m sets
  metering `power:false`, so only `energy` via seMetering 0x0702 is reported;
  the "0x0B04 half" suspect is a false flag) and added the 0x0204 / 0x0406
  bindings.
- **Simpla Home Soil Pro parity: dropped temperature / illuminance / dual
  soil_moisture channels.** The auto-generated `Soil Pro` def was a
  battery-only stub. z2m exposes temperature (msTemperatureMeasurement
  0x0402) + illuminance (msIlluminanceMeasurement 0x0400) on the default
  endpoint plus TWO soil_moisture channels (msSoilMoisture 0x0408) on the
  z1_top (ep 2) and z2_bottom (ep 3) endpoints. Graduated to a Tier-2 def
  wiring `kFzTemperature` / `kFzIlluminance` / `kFzSoilMoisture` /
  `kFzBattery`, with the endpoint_map corrected to `{z1_top: 2,
  z2_bottom: 3}` (was both at ep 1, which would have collided the zones)
  so the soil zones decode to suffixed keys `soil_moisture_z1_top` /
  `soil_moisture_z2_bottom` while temperature/illuminance stay bare.
  `measurement_interval` (genAnalogOutput) and `linear_mode`
  (genBinaryOutput) config writes are deferred (no generic converter).

- **Envilar parity: metering 0x0B04 half, dead remote, missing CCT channel.**
  `1CH-HP-RELAY-7853` + `7859` use z2m `m.electricityMeter()` (default
  `cluster:"both"`, so seMetering 0x0702 energy **and**
  haElectricalMeasurement 0x0B04 power/voltage/current); the auto-ports wired
  only `kFzMetering` and exposed energy+power, dropping the 0x0B04 half —
  restored `kFzElectricalMeasurement` + voltage/current exposes + the 0x0B04
  binding. `ZGR904-S` is a battery scene/dimmer remote (a sender) that z2m
  drives via `command_recall/on/off/move/stop` → `action`, but the auto-port
  mis-ported it as a controllable on/off load (`kFzOnOff`/`kTzOnOff` + phantom
  `state`) so every press was dropped — rewired to the generic command
  converters and an `action` enum, dropped the phantom state. `ZG50CC-CCT-
  DRIVER` + `5463` + `5491` use `m.light({colorTemp})` but were ported as
  on/off+brightness only, dropping the lightingColorCtrl 0x0300 colour-temp
  channel — restored `kFzColorTemperature`/`kTzColorTemp` + `color_temp`
  expose + 0x0300 binding. New `tests/test_envilar_parity.cpp`.
- **Meazon BIZY_PLUG / DINRAIL metering plugs reported no power/voltage/current/
  energy.** Both run z2m's vendor `fz.meazon_meter`, which decodes
  manufacturer-specific (MEAZON_S_A) attributes carried *inside* the seMetering
  (0x0702) cluster — proprietary IDs (power 0x2001, voltage 0x2004/0x2015,
  current 0x2007/0x2018, energy 0x3000) that do not overlap the standard
  metering slots. The auto-ports wired the generic `kFzMetering` (0x0000/0x0400)
  + `kFzElectricalMeasurement` (0x0B04) instead, so every electrical channel was
  a dead expose (and the device reports on no 0x0B04 cluster at all). Graduated
  both defs to Tier-2 and replaced the generic pair with a shared `kFzMeazonMeter`
  (`definitions/meazon/_shared.cpp`); raw pass-through matching z2m. Bindings
  realigned to endpoint 10 (genOnOff + seMetering) per z2m `configure`, dropping
  the phantom 0x0B04 bind.
- **Nyce NCZ-30xx IAS / multi-sensors emitted wrong keys and dropped channels.**
  All five generated defs were lowered as battery + the generic `kFzIasZone`,
  which emits a bare `alarm` and reads zoneStatus bit 0. z2m wires the contact
  sensors (NCZ-3010 hinge, NCZ-3011-HA door/window) to `fz.ias_contact_alarm_1`
  (bit 0 → `contact`) — re-wired to typed `kFzIasContactAlarm` and corrected the
  exposes to contact/battery_low(/tamper on 3011)/battery. The motion sensors
  (NCZ-3041/3043/3045-HA) are multi-sensors: z2m decodes occupancy two ways —
  `fz.occupancy` (msOccupancySensing 0x0406) **and** `fz.ias_occupancy_alarm_2`
  (zoneStatus **bit 1** → `occupancy`) — plus `fz.temperature` (0x0402, /100)
  and `fz.humidity` (0x0405, /100). The auto-port dropped occupancy/temperature/
  humidity entirely and read the wrong IAS bit; re-wired `kFzOccupancy` +
  `kFzTemperature` + `kFzHumidity` + the bit-1 `kFzIasMotionAlarm2`, added the
  0x0402/0x0405/0x0406 bindings, and corrected the exposes. (Known deferred:
  z2m's `dontDividePercentage` battery meta on 3041/3045 is unmodelled platform-
  wide, so battery % reads 2× low on those two — same gap as vesternet.)
- **Nedis ZBHTR20WT (TS0601 radiator-valve thermostat) reported local-temperature
  calibration 100× too small.** DP27 `local_temperature_calibration` was baked with
  divisor=100, but z2m wires `valueConverter.localTempCalibration2` whose decode is
  identity (`from: (v) => v`) — the wire value is already in whole degrees (expose
  range −6..6, step 1). A +3 °C offset surfaced as 0.03 °C. Corrected to divisor=1
  (raw signed-int pass-through) across all three manufacturer-name defs
  (`_TZE200_ne4pikwm`, `_TZE284_ne4pikwm`, `_TZE284_hcs66axl`); the three generated
  DP tables were graduated to Tier-2. The rest of the DP map (running_state enum,
  local_temperature & setpoint /10, child_lock, battery_low, window/frost/scale/leave
  booleans) was already at parity; the model-only battery+onOff stub is a harmless
  dead dup (loses Pass-1 ordering) and was left untouched.

- **GS (Linkind-OEM) family: IAS dead-keys, dropped socket metering half, and
  incomplete bulbs.** The 6 IAS sensors lowered the generic `kFzIasZone` (bare
  key `alarm`, bit 0) while exposing a semantic key, so the primary sensor
  state never reached the shadow; graduated each to the typed
  `kFzIas<Type>Alarm` converter — `occupancy` (SMHM-I1), `contact` (SOHM-I1),
  `smoke` (SSHM-I1), `water_leak` (SWHM-I1), and `gas` for the methane/propane
  sensors (SGMHM-I1 / SGPHM-I1, which read `alarm_2`/zoneStatus bit 1 via
  `kFzIasGasAlarm2`). The SKHMP30-I1 smart socket dropped the `electricityMeter()`
  `cluster:"both"` 0x0B04 half — restored `kFzElectricalMeasurement` plus the
  `voltage`/`current` exposes and the 0x0B04 binding. The BRHM8E27W70-I1 (RGB+CT)
  and BDHM8E27W70-I1 (CT) bulbs were collapsed to on/off+brightness — restored
  `color_temp` (and `hue`/`saturation` on BRHM) plus the lightingColorCtrl
  (0x0300) binding.
- **Siterwell GS361A-H04 (TS0601 radiator-valve thermostat) decoded nothing.**
  The auto-port misrouted it to the generic genThermostat pair (`kFzThermostat`
  / `kTzThermostat`) bound on ZCL 0x0201, but this is a Tuya-MCU TRV that z2m
  decodes entirely over the 0xEF00 DP stream (`legacy.fz.tuya_thermostat`), so
  setpoint / local_temperature / system_mode / child_lock / window+valve
  detection / battery / valve-position were all dead. Graduated to a Tier-2
  Tuya DP map (`factory::TuyaRw`) with z2m-matching per-DP scale (setpoint DP2
  + local_temperature DP3 both `/10`) and enum order (system_mode DP4
  `{0:off,1:auto,2:heat}` per `thermostatSystemModes4`); dropped the phantom
  0x0201 binding. Flat thermostat exposes replace the genThermostat shape.
- **Ynoa (Lytimages) remotes were dead and bulbs lost their colour axes.**
  The two action-only remotes (LA-5KEY-RGBW, 8718801528334) were auto-ported
  as settable on/off lights — kFzOnOff + kTzOnOff + a writable `state` — so
  every key press was dead and they falsely advertised a relay; z2m drives a
  bound light via genOnOff/genLevelCtrl/lightingColorCtrl commands surfaced on
  `action` (toZigbee:[]). The four bulbs (8718801528204/8718801528273/
  LA-A60-CCT CCT, LA-GU10-RGBW RGB+CCT) wired only OnOff + Brightness and
  dropped the colour-temperature axis (and xy/hs colour on the RGBW). Graduated
  all six defs to Tier 2: remotes → the command decoders + an `action` expose,
  no toolbox; bulbs → kFzColorTemperature (+ kFzColor on the RGBW) + the
  color_temp/color_xy exposes + the lightingColorCtrl (0x0300) binding. Also
  extended the generic `kFzBattery` to decode genPowerCfg batteryAlarmState
  (attr 0x0035 → `battery_low`, mirroring z2m fz.battery) so the 5KEY's
  `battery_low` is no longer a dead expose. The LA-PLUG-10Amp metering plug was
  already correct (z2m electricityMeter "both" = 0x0702 + 0x0B04, both wired)
  and is pinned as a regression guard.
- **Hej (Goqual) KKZ-DO021 door contact and KKZ-MO021 PIR sensors emitted a
  dead `alarm` key instead of their declared `contact` / `occupancy` state.**
  Both auto-ports lowered the generic `kFzIasZone` converter (bare key
  `alarm`) while exposing the semantic key, so the primary sensor signal
  never reached the shadow. z2m decodes these via `fz.ias_contact_alarm_1` /
  `fz.ias_occupancy_alarm_1` (both zoneStatus bit 0); graduated each def to a
  Tier-2 override wiring the typed `kFzIasContactAlarm` / `kFzIasMotionAlarm`
  converter (emitting `contact` / `occupancy` directly). Mirrors the heiman
  IAS contact/occupancy ports. Added `tests/test_hej_parity.cpp` (also pins
  the GLSK multi-gang `endpoint_map` state-suffix path is intact).
- **GE PTAPT-WH02 ("Quirky smart switch") on/off routed to the wrong
  endpoint.** z2m declares `endpoint: () => ({default: 2})` — the genOnOff
  cluster lives on endpoint 2 — but the auto-port bound genOnOff on EP1 and
  left `default_endpoint=0`, so outbound on/off frames went to EP1 (dead
  toggle) and reporting bound the wrong endpoint. Graduated the def to
  Tier 2, re-endpointed the binding ep1→ep2, and set `default_endpoint=2`.
  Confirmed the GE metering devices (45853/45856 `fz.metering`, 45857
  `m.electricityMeter({cluster:"metering"})`) are seMetering 0x0702 only
  with no 0x0B04 voltage/current half, and the bulbs/dimmers are plain
  `m.light()` (on/off + brightness, no color_temp) — both regression-guarded
  in the new `test_ge_parity` suite.
- **Samotech metering relays/dimmers dropped the 0x0B04 electrical half.**
  SM309-S, SM323_v2 (SM323 / HK_DIM_A) and SM308-2CH use z2m
  `m.electricityMeter()` with no args, which defaults to `cluster:"both"` +
  `electricalMeasurementType:"ac"` — decoding both seMetering (0x0702: energy)
  AND haElectricalMeasurement (0x0B04: power/voltage/current). The auto-ports
  wired only `kFzMetering` and exposed just energy/power, so voltage and
  current never decoded. Graduated all three to Tier-2 overrides adding
  `kFzElectricalMeasurement`, the voltage/current exposes and the 0x0B04 bind;
  SM308-2CH also gained its missing 2nd-channel genOnOff bind. Its
  (suspected-missing) per-channel endpoint_map was already present and is
  retained.

- **Multiterm ZC0101 (ZeeFan fan coil unit controller) dropped its entire
  multi-endpoint binary-output half.** The auto-port modelled it as a bare
  single `fan_mode` binary and never decoded the three genBinaryOutput
  (0x0010) channels on endpoints 8/9/10 (silent_mode/heating_cooling/
  electric_valve, z2m `fzLocal.binary_output` under
  `m.deviceEndpoints({8,9,10})`), which would also have collided on a bare
  `state` key. Added a reusable generic `kFzBinaryOutput` (presentValue →
  boolean `state`, mirror of `kFzBinaryInput`), graduated the def to Tier 2
  with the z2m exposes and an endpoint_map for ep9/ep10 (ep8 left unmapped so
  the fan's hvacFanCtrl key stays unsuffixed → channels resolve to
  `state`/`state_9`/`state_10`). The fan control is exposed as `fan_mode`
  (Enum off/low/medium/high/on) — the key `kFzFanMode` actually decodes; the
  earlier draft declared `fan_state`, which nothing decoded (dead). z2m's
  derived `fan_state` (on iff fan_mode≠off) needs a fan converter that emits
  it (see Mercator `kFzMercatorFan`) and is deferred. The per-channel
  activeText/inactiveText enum labels + the binary_output write path remain
  runtime-attribute-dependent and are deferred as infra.
- **Saswell TRV legacy `_TYST11_*` variants decoded nothing.** The
  SEA801/SEA802 thermostatic radiator valve (z2m `legacy.fz.saswell_thermostat`,
  a Tuya-MCU 0xEF00 DP device) had two ez defs sharing the same model: a correct
  full DP-map def plus a duplicate that was the *only* def carrying the legacy
  `_TYST11_*` fingerprints (GbxAXL2 / uhszj9s / 88teujp / w7cahqs / aj4jz0i) and
  wired the generic genThermostat (0x0201) decoder — a cluster the device never
  speaks, so those variants decoded nothing (dead def). Folded the `_TYST11_*`
  fingerprints into the Tuya-DP def (now routed through `fz_tuya_datapoints` with
  setpoint/local-temperature /10 and identity calibration) and removed the dead
  duplicate plus its registry reference. Added `tests/test_saswell_parity.cpp`.
- **Hornbach (FLAIR Viyu) LED bulbs lost their colour-temperature / colour
  axis.** All 14 generated defs were wired with only `kFzOnOff` +
  `kFzBrightness` (clusters 0x0006/0x0008), dropping the `lightingColorCtrl`
  (0x0300) channel that z2m's `m.light({colorTemp, color?})` exposes — every
  bulb lost `color_temp`, and the six `*_RGBW_*` / "RGB" variants (10011725,
  10297666, 10297667, 10454466, 10454467, 10454471) lost the hue/saturation
  colour axis too. Graduated all 14 to two shared bundles in
  `definitions/hornbach/_shared.cpp`: CTLight (on/off + brightness +
  `kFzColorTemperature`/`kTzColorTemp` + `color_temp` expose + 0x0300 bind) for
  the 8 CCT models, and ColorCTLight (additionally `kFzColor`/`kTzColor` +
  `color_xy`/`color_hs` exposes) for the 6 RGBW models.

- **EGLO connect family (AwoX rebadge): five bulbs lost their colour
  channels and two remotes were ported as on/off switches.** All five EGLO
  bulbs are z2m `m.light({colorTemp, …})` — 12242 + 98847 tunable-white,
  300686 + 900091 + 900024/12253 full RGBW — but the auto-generator lowered
  every one onto on/off + brightness only, leaving the lightingColorCtrl
  (0x0300) `color_temp` and colour (hue/sat/xy) reports dead and dropping the
  colour set-path. Graduated to Tier-2 parents wired to a new `eglo/_shared`
  CT and full-RGBW bundle (kFzColorTemperature / kFzColor + matching TZ +
  0x0300 bind + the colour exposes). The 99099 (3-groups remote) and 99106
  (TLSR82xx) were both modelled by z2m as pure command transmitters exposing
  `action`, but were ported as `kFzOnOff` + on/off TZ + a dead settable
  `state`; re-wired to the generic command-action decoders (on/off, brightness
  step/move/move_to_level/stop, scene recall, color-temp move/step) with an
  `action` expose (plus `action_group` on 99099) and no TZ. The AwoX-specific
  raw colour/refresh frames (awox_colors / awox_refresh / awox_refreshColored)
  have no generic equivalent and are intentionally deferred (INFRA), mirroring
  the AwoX 33952 precedent.
- **EssentielB bulbs dropped colour, smart button was a dead switch.** All
  nine `m.light()` LED bulbs (CCT + RGBW) were ported as on/off + brightness
  only — the auto-generator collapsed `m.light({colorTemp[, color]})` and
  dropped the colour-temp axis on every bulb plus the full colour axis on the
  three RGBW SKUs (lightingColorCtrl 0x0300 neither decoded nor bound). The
  EB-SB-1B smart button was mis-ported as a controllable on/off switch (dead
  `state` + on/off TZ) instead of a `fz.command_*` remote, dropping its entire
  seven-value `action` surface. Graduated all ten defs to Tier-2: bulbs now
  wire `kFzColorTemperature`(+`kFzColor`) + colour exposes + the 0x0300 bind;
  the button wires the generic command-action decoders (`action` =
  on/off/brightness_step_up/down/stop/color_temperature_step_up/down) + battery.

- **GiEX water-irrigation valves (QT06_1 / QT06_2) decoded nothing.** Both are
  Tuya-MCU (0xEF00) DP-stream devices (z2m `legacy.fromZigbee.giexWaterValve`)
  but the auto-generator ported them as bare `kFzBattery` + `kFzOnOff` stubs
  binding ZCL clusters 0x0001/0x0006 the devices never speak, dropping 8 of
  z2m's datapoints and carrying a phantom `voltage`. Graduated both to Tier-2
  overrides wiring `fz_tuya_datapoints` + `tz_tuya_datapoints` with the full DP
  map: mode (1) and state (2) as bool→string via `kTuyaDpFlagBoolEnum`, the
  irrigation start/end time + last-duration String DPs (101/102/114), and the
  raw numeric channels cycle_irrigation_num_times (103), irrigation_target
  (104), cycle_irrigation_interval (105), battery (108) and water_consumed
  (111). The suspected per-DP scale/float-divisor was confirmed FALSE — z2m
  passes every value DP straight through with no scaling. DP 106
  (currentTemperature) stays unmapped to match z2m (it explicitly ignores it).
- **EnOcean PTM 215Z / 215ZE / 216Z Green Power transmitters were ported as
  controllable on/off relays.** All three self-powered pushbutton modules are
  pure action remotes in z2m (`toZigbee: []` + a single `e.action([...])`),
  but the auto-generator emitted `kFzOnOff` + `kTzOnOff`, a dead settable
  `state` Binary expose, and a genOnOff (0x0006) coordinator bind — so a
  transmitter falsely advertised a relay and offered nothing meaningful.
  Graduated all three defs out of `generated/`, replaced `state` with the
  semantic `action` enum carrying z2m's full per-model action list, and
  removed the phantom converters/binding. The action *decoder* stays a
  documented defer: z2m reads these over `cluster: "greenPower"`
  /`commandNotification`, a Green Power frame family the embedded-zhc parser
  does not yet produce (216Z further needs the compound `commandID_<rawByte0>`
  key). Schema/role parity is what the shadow + SPA consume.
- **Vimar thermostat dropped its cooling setpoint and the RemoteControl was
  mis-ported as an on/off switch.** The 02973.B IoT thermostat wires z2m's
  full `fz.thermostat` and exposes both heating and cooling setpoints plus
  `system_mode` off/heat/cool, but the auto-port wired only the generic
  `kFzThermostat` (decodes 0x0000/0x0012/0x001C), leaving the
  `occupied_cooling_setpoint` (attr 0x0011) expose dead and declaring
  `system_mode` as a phantom Binary. Added a vendor `kFzVimarThermostatExtras`
  converter (0x0011 → occupied_cooling_setpoint) alongside the generic decoder,
  exposed the cooling setpoint, and corrected system_mode to an Enum.
  RemoteControl_v1.0 is a battery transmitter (`m.commandsOnOff()`,
  `m.commandsWindowCovering()`, `m.commandsLevelCtrl()`) but was mapped as a
  controllable on/off switch (`kFzOnOff` + `kTzOnOff` + a writable `state`);
  rewired to the generic command converters (action emitter), dropped the
  phantom `state` and toZigbee.
- **Halemeier S-Mitter / X-Mitter remotes were mis-ported as on/off
  switches.** HA-ZSM-MW2, HA-ZBM-MW2 and HA-ZX1 are battery remotes that emit
  genOnOff / genLevelCtrl / lightingColorCtrl / genScenes cluster commands, but
  the auto-generator wired `kFzOnOff` + `kTzOnOff` (a controllable switch with a
  dead `state` attribute). Rewired to `kFzCommand*` converters with an `action`
  expose (on/off, brightness_step/move, color_temperature_step, recall, stop)
  and dropped the phantom state + toZigbee path.

- **Shenzhen Homa LED drivers dropped the 0x0300 color/colorTemp axis.** The
  auto-generator collapsed every `m.light()` to on/off + brightness, but three
  drivers request the lightingColorCtrl axis that z2m wires: HOMA1001_RGB and
  HOMA1001_RGBW (`m.light({color:true})`) regained xy color via `kFzColor` +
  `color_x`/`color_y` exposes + 0x0300 bind (RGBW on its rgb endpoint 11, so the
  endpoint_map suffixes the keys to `color_x_rgb`/`color_y_rgb`); HLD503-Z-CT
  (`m.light({colorTemp:{range:[150,500]}})`) regained `color_temp` via
  `kFzColorTemperature` + 0x0300 bind. Plain `m.light()` drivers (CT/SC/HLC610/
  HLC821/HLC833/HLD812) carry neither axis in z2m and were left unchanged.
- **The Light Group (SLC SmartOne) remotes were wrong-bundled as settable
  lights.** S57003 (4-channel wall remote) and S57007 (3-button remote
  control) are battery-powered scene/dimmer remotes with z2m `toZigbee: []`,
  but the auto-generator emitted `kFzOnOff` + `kTzOnOff` + a writable `state`
  expose, so every button press decoded to nothing and both falsely
  advertised a relay. S57003 also dropped its endpoint_map, collapsing all
  four rockers onto one key. Graduated both defs to the genOnOff/genLevelCtrl
  (S57003) and genOnOff/genScenes (S57007) command decoders, exposed `action`,
  dropped the phantom `state` + toolbox, and added endpoint_map +
  endpoint_action_suffix so each button keeps identity (action_1..action_N).
- **Gewiss ports dropped contact channels and the electrical-meter half.**
  GWA1201 used `m.electricityMeter()` (default cluster "both") but the port
  wired only `seMetering` (0x0702) energy, dropping the
  `haElectricalMeasurement` (0x0B04) voltage/current half — added
  `kFzElectricalMeasurement` + voltage/current exposes + 0x0B04 bind. GWA1502
  (mains 230V contact interface) was mis-ported as a phantom battery + phantom
  on/off; it is actually two `genBinaryInput.presentValue` channels — rewired
  to a new generic `kFzBinaryInput` (0x000F attr 0x0055 → `input`) on ep1/ep2
  with an endpoint_map (no battery, no on/off). GWA1501 kept its battery but
  dropped the same two `input` channels — added `kFzBinaryInput` on both
  endpoints. New reusable `::zhc::generic::kFzBinaryInput` added for any
  genBinaryInput presentValue device.

- **MultIR (MIR series) IAS dead-keys, dropped channels, and a phantom
  switch.** All seven MultIR sensor ports were corrected against z2m: the
  contact (MIR-MC100), smoke (MIR-SM200), and water-leak (MIR-WA100) sensors
  wired the generic `kFzIasZone` (bare `alarm`) against a semantic expose key
  — swapped to the typed `kFzIasContactAlarm` / `kFzIasSmokeAlarm` /
  `kFzIasWaterLeakAlarm` (→ `contact`/`smoke`/`water_leak`); MIR-SM100-E
  (z2m zoneType "generic" + both alarms) now uses `kFzIasZoneStatusChange`
  for `alarm_1`/`alarm_2`; MIR-IR100 PIR regained its `occupancy` IAS key
  (`kFzIasMotionAlarm`) plus the dropped illuminance channel
  (msIlluminanceMeasurement 0x0400); MIR-TE600 regained temperature (0x0402)
  + humidity (0x0405), which the port had reduced to battery-only; and the
  MIR-SO100 SOS button — mis-modelled as a settable genOnOff switch (phantom
  `state` + write path + 0x0006 binding) — now decodes its raw ssIasZone
  press command (byte 0/1/128 → single/double/hold) to `action` via the new
  vendor `kFzMultirSosAction`. (Deferred infra: the MIR-IR100 `sensitivity`
  enum over ssIasZone attr 0x0013 — no generic IAS-attribute config
  converter exists yet.)
- **MakeGood MG-AUZG01 metering plug dropped its whole power-monitoring
  surface.** The TS011F `_TZ3000_dd8wwzcy` double power point uses z2m
  `tuya.modernExtend.tuyaOnOff({electricalMeasurements: true})`, whose
  `electricalMeasurements` branch adds `fz.electrical_measurement` +
  `fz.metering` and exposes power/current/voltage/energy. The auto-port
  did not expand that modernExtend, so the generated def wired on/off
  only — dropping the entire metering surface (`seMetering` 0x0702 energy
  + `haElectricalMeasurement` 0x0B04 power/voltage/current). Graduated to
  a Tier-2 parent override wiring the generic metering + electrical
  decoders, the four exposes, and the 0x0702/0x0B04 (+EP2 genOnOff)
  bindings. The dual-endpoint (l1/l2) state split stays INFRA-deferred
  (z2m `multiEndpointSkip` keeps metering global; the runtime suffix
  rewrite has no per-def metering-skip hook), matching the Honyar
  U86Z223A10-ZJU01(GD) precedent. The sibling MG-GPO01
  (`_TZ3210_bep7ccew`) carries the *same* `multiEndpointSkip` set but the
  generated def kept a `{right:1,left:2}` endpoint_map — the runtime would
  then suffix power/current/energy (`_right`), diverging from z2m's bare
  metering keys. Graduated MG-GPO01 too, dropping the endpoint_map (same
  INFRA defer): single bare `state` + single EP1 metering surface.

- **Visonic (PowerMax/Tyco) IAS sensors lost their primary state + temperature.**
  All 8 auto-generated Visonic defs lowered the generic `kFzIasZone`
  converter (bare key `alarm`) while exposing the semantic key, so the
  occupancy (MP-840/MP-841), contact (MCT-370/350/340 E/340 SMA/302 SMA) or
  vibration (GB-540) state never reached the shadow. Graduated all 8 to
  Tier-2 overrides using the typed `kFzIasMotionAlarm` / `kFzIasContactAlarm`
  / `kFzIasVibrationAlarm` converters (zoneStatus bit 0). The five
  temperature-capable models (MP-840, MCT-350 SMA, MCT-340 E, MCT-340 SMA,
  MCT-302 SMA) additionally dropped z2m's `fz.temperature` / `m.temperature()`
  decoder and `e.temperature()` expose; restored `kFzTemperature`
  (`msTemperatureMeasurement` 0x0402, /100) + the `temperature` expose +
  the 0x0402 binding.
- **LifeControl (MCLH series) ports dropped whole channels.** Three
  auto-generated defs lost half their z2m functionality: MCLH-02 "vivi
  ZLight" colour bulb (`m.light({colorTemp, color: true})`) wired only
  on/off + brightness, dropping the entire `lightingColorCtrl` (0x0300)
  half (color_temp / color_x/y / hue / saturation); MCLH-03 "RICI01"
  smart socket (`m.electricityMeter()` default cluster "both") wired only
  `seMetering` (0x0702) energy, dropping the `haElectricalMeasurement`
  (0x0B04) voltage/current; MCLH-08 "VOC_Sensor" air-quality sensor wired
  only battery, dropping LifeControl's custom `airQuality()` decode that
  packs temperature/humidity/eco2/voc into ONE `msTemperatureMeasurement`
  (0x0402) report. Graduated all three to Tier 2: MCLH-02 + MCLH-03 wire
  the generic colour / electrical-measurement converters; MCLH-08 gets a
  vendor `kFzLifecontrolAirQuality` converter (temperature/humidity divided
  /100 to Float to match z2m + the generic temp/humidity converters; eco2/
  voc raw; incl. the negative-temperature wrap). Covered by
  `tests/test_lifecontrol_parity.cpp`.
- **Fantem ZB003-X 4-in-1 multi sensor never reported `occupancy`.** The IAS
  motion path lowered the generic `kFzIasZoneStatusChange`, which emits the
  bare key `alarm_1` for zoneStatus bit 0, while the expose declared the
  semantic key `occupancy`. z2m's `fz.ZB003X_occupancy` maps bit 0 →
  `occupancy` (bit 2 → `tamper`), so motion events never reached the shadow.
  Re-pointed the def at the typed `kFzIasMotionAlarm` (emits `occupancy` from
  bit 0, `tamper`/`battery_low` from bits 2/3) and graduated it out of
  `generated/`. Tuya-DP temp/humidity/calibration path was already correct.
- **BTicino F20T60A measured via the wrong cluster + K4003C dropped its
  button action.** The F20T60A DIN power module was ported with
  `kFzMetering` (seMetering 0x0702) and a phantom `energy` expose, but z2m
  uses `m.electricityMeter({cluster: "electrical"})` which is
  haElectricalMeasurement-only (0x0B04) → power/voltage/current and NO
  energy; re-wired to `kFzElectricalMeasurement` with the correct
  binding/exposes (voltage + current were previously missing entirely). The
  K4003C/L4003C/N4003C/NT4003C light switch reports its button on
  `genBinaryInput` (0x000F) presentValue via z2m's `fz.K4003C_binary_input`
  (`action: on/off` + state) but was ported as bare on/off, leaving the
  `action` surface dead; added shared `bticino::kFzK4003CBinaryInput` +
  the `action` expose + 0x000F binding. Both defs graduated out of
  `generated/`. (Legrand 0xFC01 LED-mode config = manuSpecific INFRA,
  deferred.)

- **Enbrighten (GE/Jasco) switches/outlets/dimmers dropped their physical
  button `action`.** z2m pairs every device with
  `m.commandsOnOff({commands:["on","off"], bind:true})` and (the dimmers)
  `m.commandsLevelCtrl({commands:["brightness_move_up","brightness_move_down",
  "brightness_stop"], bind:true})`, which bind genOnOff/genLevelCtrl output
  and decode the device's physical presses into an `action` enum. The
  generated ports kept only the controllable surface (on/off, brightness,
  metering) so the `action` expose and its decoders were missing. Graduated
  all four defs (43076 switch + whitelabels, 43078 switch+energy, 43080
  dimmer + whitelabels, 43082 dimmer+energy) out of `generated/` and wired
  the generic `kFzCommandOn`/`kFzCommandOff` (+ `kFzCommandMove`/
  `kFzCommandStop` for dimmers) + an `action` expose. `electricityMeter`
  stays `cluster:"metering"` (seMetering 0x0702 only — no phantom
  voltage/current half).
- **Inovelli Blue-series switches/dimmers/fan dropped their button-tap
  `action`.** z2m marks VZM30-SN, VZM31-SN, VZM32-SN and VZM35-SN
  `supportsButtonTaps: true`, decoding a manufacturer-specific raw frame on
  endpoint 2 (`manuSpecificInovelli` 0xFC31, cmd 0x00) into
  `action: "<button>_<click>"` (e.g. `up_double`, `config_held`). The
  generated ports wired only on/off/brightness/metering, so both the
  `action` expose and its decoder were missing — every single/double/triple/
  held/release tap was dead. Added `inovelli::kFzInovelliSceneAction`
  (button 1-6 → down/up/config + aux variants × click 0-6, guarded on
  endpoint 2 + `manufacturer_specific` so a plain command-0x00 frame can't
  mint a bogus action) and graduated the four defs out of `generated/` to
  wire the converter + `action` expose. VZM36 (`supportsButtonTaps: false`)
  is intentionally left untouched.
- **Stello STLO-34 (Hilo thermostat) `keypad_lockout` was a dead attribute.**
  The auto-generated port declared `keypad_lockout` as a read+write expose but
  wired neither a decoder nor an encoder, so the value never decoded and could
  not be set — z2m wires `fz.hvac_user_interface` (hvacUserInterfaceCfg 0x0204
  attr 0x0001 keypadLockout ENUM8) and `tz.thermostat_keypad_lockout`. Graduated
  the def to the parent dir and wired the shared `kFzStelproKeypadLockout` /
  `kTzStelproKeypadLockout` (same gap previously fixed for the Stelpro siblings).
  Pinned by the new `tests/test_stello_parity.cpp`.

- **iCasa ICZB-IW21D metering dimmer dropped voltage + current.** z2m drives
  it with `m.electricityMeter()`, whose default `cluster:"both"` decodes both
  `seMetering` (0x0702 energy/power) and `haElectricalMeasurement` (0x0B04
  power/voltage/current). The auto-generated port wired only `kFzMetering`
  (the 0x0702 half), exposing just energy + power, so RMSVoltage (0x0505) and
  RMSCurrent (0x0508) were dead. Graduated to Tier 2 and added
  `kFzElectricalMeasurement` + the 0x0B04 binding + `voltage`/`current`
  exposes. Added `tests/test_icasa_parity.cpp` (also regression-guards the
  KPD12/KPD14S/RM11S command-action keypad/remote ports).
- **Acova electric-radiator thermostats reported dead setpoint / running-state
  keys.** All three Acova defs (ALCANTARA2, TAFFETAS2/PERCALE2, IHC-Enki) routed
  inbound through the generic `kFzThermostat`, which decodes only hvacThermostat
  `0x0000`/`0x0012`/`0x001C` and emits attr `0x0012` under the key
  `current_heating_setpoint`. But z2m's `fz.thermostat` (and the Acova exposes)
  use `occupied_heating_setpoint`, so the occupied setpoint never updated; the
  unoccupied setpoint (`0x0014`), `running_state` (`0x0029`) and
  `local_temperature_calibration` (`0x0010`) were not decoded at all. Graduated
  the three defs to Tier 2 and routed them through a new
  `kFzAcovaThermostatExtras` (in `definitions/acova/_shared`) that decodes the
  full Acova surface with z2m's keys (`0x0029` → `idle`/`heat`). Outbound
  vendor `system_mode` enum and `fz.hvac_user_interface` remain PARTIAL. Added
  `tests/test_acova_parity.cpp`.

- **Paul Neuhaus Q-Home remotes dropped/mis-decoded colour-wheel actions.**
  Both remotes carried stale "converter absent" port comments from before the
  generic command bundle gained `kFzCommandColorLoopSet` /
  `kFzCommandEnhancedMoveToHueAndSat` (lightingColorCtrl cmd 0x44 / 0x43).
  `100.462.31` (Q-REMOTE) never wired them, so `color_loop_set` and
  `enhanced_move_to_hue_and_saturation` were dead enum entries; `E0040006`
  (JZ-RC-J4R) substituted `kFzCommandMoveToHueAndSaturation` (wrong action
  string) for z2m's `command_enhanced_move_to_hue_and_saturation` and dropped
  `command_color_loop_set` entirely (plus carried phantom move/stop/recall
  decoders z2m never lists). Graduated both to Tier 2 and wired the correct
  decoders. `scene_*` (z2m `fz.tint_scene`) remains an INFRA defer — the
  genBasic Write-Attributes parser does not populate `msg.payload`, so
  `kFzTintScene` would be inert. Added `tests/test_paul_neuhaus_parity.cpp`.
- **Netica FRZ1 ("FreezBee") thermostat carried a phantom battery and four
  dead exposes.** z2m's `netica.ts` wires `m.temperature() + m.humidity() +
  m.thermostat({runningState, ctrlSeqeOfOper, ...})` plus three manuSpec
  hvacThermostat attrs and **no** `m.battery()` / genPowerCfg. The auto-port
  invented `battery`/`voltage` exposes (+ `kFzBattery` + a 0x0001 binding),
  and the generic `kFzThermostat` (0x0000/0x0012/0x001C only) never populated
  `running_state` (0x0029) or `control_sequence_of_operation` (0x001B), while
  the manuSpec `remote_temperature` (0x4000) and `target_water_temperature`
  (0x4002) were wired write-only — all four stayed dead despite being declared
  exposes in z2m. Graduated `Net_FRZ1.cpp` to Tier 2, removed the phantom
  battery, and added an inline `kFzNeticaThermostatExtras` converter decoding
  running_state, control_sequence_of_operation and the two manuSpec read-backs
  alongside the generic decoder. Added `tests/test_netica_parity.cpp`.
- **Somfy Ysia 5 remote (1871154) collided all five channels onto one
  `action`, and the 1871215B plug dropped its electrical-measurement half.**
  z2m wires the Ysia 5 via `m.commandsOnOff` + `m.commandsWindowCovering`, both
  with `endpointNames ["1".."5"]`, so each command is postfixed per endpoint
  (`on_1`, `open_2`, …); the auto-port carried an `endpoint_map` for EPs 1-5 but
  not `endpoint_action_suffix`, so dispatch kept `action` global and the
  per-button identity was lost — set the flag so it emits `action_<n>`. The
  1871215B plug uses `m.electricityMeter()` (defaults to `cluster:"both"`,
  decoding seMetering 0x0702 energy **and** haElectricalMeasurement 0x0B04
  power/voltage/current), but the port wired only `kFzMetering`, dropping
  `voltage`/`current`; added `kFzElectricalMeasurement`, the two exposes and the
  0x0B04 binding. Both files graduated to Tier 2; added
  `tests/test_somfy_parity.cpp`.

- **Dresden Elektronik scene remotes lost button presses (BN-600085 &
  BN-600087).** *BN-600085 ("Scene Switch")* — the 3-part battery scene remote
  wires z2m's `m.commandsScenes()`, which decodes genScenes (0x0005)
  `commandRecall`/`commandStore` into `action: "recall_<scene>"` /
  `"store_<scene>"`. The auto-port wired only the OnOff/LevelCtrl/ColorCtrl
  command stream plus battery and carried a stale "no generic decoder yet"
  comment — but `kFzCommandRecall`/`kFzCommandStore` already exist in
  `_generic/_shared`, so the scene buttons were silent. Wired both (single
  endpoint, actions stay bare). *BN-600087 ("Lighting Switch")* — the
  2-endpoint remote (z2m `m.commandsOnOff/LevelCtrl/ColorCtrl(["1","2"])`
  postfix actions per button) carried an `endpoint_map` but not
  `endpoint_action_suffix`; since `action` is a kAlwaysGlobalKey both rockers
  collapsed onto one bare `action` (a 2-button remote looked like 1). Set
  `endpoint_action_suffix` so the key is rewritten `action_1`/`action_2`.
  Graduated both defs to Tier 2; added `tests/test_dresden_elektronik_parity.cpp`.
- **L&S Lighting SEMOTE (756200643) remote dropped its `stop_move_step`
  action.** z2m's `m.commandsColorCtrl({commands:[..., "stop_move_step"]})`
  decodes a lightingColorCtrl `commandStopMoveStep` (cmd 0x47) to
  `action="stop_move_step"`, but the generic catalogue had no decoder for
  0x47, so the auto-port declared the action in its enum while emitting it
  from no converter — a dead action key. Graduated `Ls_D756200643.cpp` to
  Tier 2 and added the bespoke `kFzLsCommandStopMoveStep`
  (`definitions/ls/_shared.{hpp,cpp}`).

- **Leviton RC-2000WH (Omnistat2) thermostat left three exposes dead.** z2m
  wires the full `fz.thermostat`, which publishes the entire hvacThermostat
  surface, but the auto-port only wired the generic `kFzThermostat`
  (0x0000/0x0012/0x001C). The device's `pi_heating_demand` (attr 0x0008),
  `local_temperature_calibration` (0x0010) and `occupied_cooling_setpoint`
  (0x0011) exposes were therefore never populated. Graduated `Lev_RC_2000WH.cpp`
  to Tier 2 and wired a new vendor converter `kFzLevitonThermostat`
  (`definitions/leviton/_shared.{hpp,cpp}`) decoding those three alongside the
  generic decoder + `kFzFanMode`. Unlike Danfoss, Leviton does not set
  `dontMapPIHeatingDemand`, so `pi_heating_demand` is mapped 0-255→0-100 %
  (`mapNumberRange`, round-half-up). Added `tests/test_leviton_parity.cpp`.
- **Universal Electronics Inc (Xfinity) XHK1-UE & UEHK2AZ0 security keypads
  dropped `action_transaction`.** z2m wires `fz.command_arm_with_transaction`,
  which copies the ZCL transaction sequence number into `action_transaction`;
  the ports wired `kFzIasAceArm`, which decodes `action`/`action_code`/
  `action_zone` but not the TSN, so the declared `action_transaction` expose
  stayed dead. Added a reusable `kFzIasAceArmWithTransaction` generic converter
  (emits the arm fields plus `action_transaction` from
  `DecodedMessage.transaction_sequence`), wired both keypads to it, added the
  `action_transaction` expose, and graduated both defs out of `generated/`.

- **Atlantic Group GW003-AS-IN-TE-FC (Naviclim AC interface) dropped two
  exposed thermostat reads.** The def exposes `occupied_cooling_setpoint` and
  `programming_operation_mode`, but the generic `kFzThermostat` only decodes
  hvacThermostat attrs `0x0000`/`0x0012`/`0x001C`, so both keys were dead on
  read (z2m's `fz.thermostat` decodes `occupiedCoolingSetpoint` attr `0x0011`
  and `programingOperMode` attr `0x0025`). Graduated the generated def to a
  Tier-2 override adding `kFzAtlanticThermostatExtras` (raw values, matching
  `kFzThermostat`'s convention) and gave `programming_operation_mode` its z2m
  enum labels. New `tests/test_atlantic_parity.cpp`.
- **Linptech ES1ZZ(TY) mmWave presence sensor decoded illuminance wrong and
  under a non-z2m key.** z2m's `fzLocal.TS0225_illuminance` reads the whole raw
  ZCL frame (`buffer = msg.data`), takes the little-endian u16 measuredValue at
  full-frame offset 6/7, and publishes `illuminance = round(10^((raw-1)/10000))`
  (lux). The ZHC decoder read `raw_body[6]/[7]` — offset by the 3-byte ZCL
  header, so it read past the value into garbage — and emitted the raw u16 under
  the non-z2m key `illuminance_raw` with no lux curve. Graduated
  `Lin_ES1ZZ_TY_.cpp` to Tier 2, fixed the decoder to read `raw_data[6]/[7]`
  (== z2m's full-frame offsets), apply the lux curve, and emit the standard
  `illuminance` (lx) key + expose. Added `tests/test_linptech_parity.cpp`.
  (The `manuSpecificTuya2` cluster-ID mapping 0xE001-vs-0xE002 is a documented
  cross-vendor decision in `cluster_names.hpp` and left untouched.)
- **Lellki metering sockets XF-EU-S100-1-M and WK34-EU dropped their entire
  power-monitoring half.** z2m wires both via
  `tuya.modernExtend.tuyaOnOff({electricalMeasurements: true})`, which pushes
  `fz.metering` (seMetering 0x0702) + `fz.electrical_measurement`
  (haElectricalMeasurement 0x0B04) and exposes power/current/voltage/energy, but
  the auto port lowered each to a bare on/off with only a `state` expose.
  Graduated both to Tier 2 and wired `kFzMetering` + `kFzElectricalMeasurement`,
  the four numeric exposes, and the 0x0702/0x0B04 binds. Added
  `tests/test_lellki_parity.cpp`.
- **Futurehome TS0601 thermostat (Co020 / FH-clones) exposed a dead
  `running_state` channel.** z2m maps DP 104 `running_state` via
  `lookup({idle: false, heat: true})` and exposes it through
  `withRunningState(["idle", "heat"])`, but the hand-ported Tier-2 DP map
  dropped DP 104 entirely — the thermostat declared a running-state surface
  that never decoded. Graduated the def out of `generated/` and restored DP
  104 as a boolean DP fanned to an `idle`/`heat` string via
  `kTuyaDpFlagBoolEnum`, plus the `running_state` expose. Added
  `tests/test_futurehome_parity.cpp`.
- **Insta 57008000 blinds actuator exposed a dead `tilt` channel.** z2m's
  `cover_position_tilt` reports both `currentPositionLiftPercentage` (attr
  0x0008) and `currentPositionTiltPercentage` (attr 0x0009) on EP6, and the def
  declared a `tilt` expose, but only the generic `kFzCoverPosition` (lift) was
  wired so tilt reports decoded to nothing. Graduated to Tier 2 and wired
  `kFzCoverTilt` (0x0009 → tilt) plus `default_endpoint=6` so outbound cover
  commands route to the cover EP (matching z2m `endpoint:{default:6}`). Added
  `tests/test_insta_parity.cpp`.
- **Essentials thermostat (TS0601 / _TZE200_i48qyn9s) dropped its primary
  setpoint + open-window channels.** The auto extract mapped only 7 of z2m's
  18 active datapoints, omitting the PRIMARY heating setpoint (DP 16
  `current_heating_setpoint`, z2m `divideBy(2)`) and the open-window detection
  set (DP 107 `window_open`, DP 116 `open_window_temperature` `divideBy(2)`,
  DP 117 `detect_window_time_minute` raw) — leaving a thermostat with a
  visible mode but no settable/readable target temperature. Graduated the
  generated def to a Tier-2 override and restored these DPs with z2m-matching
  scale; `window_open` fans the boolean DP to a "YES"/"NO" label via
  `kTuyaDpFlagBoolEnum`. DP 34 battery (custom `(v-70)*1.7` curve), DP 45
  `fault_code`, and DP 103 `away_setting` (composite) remain deferred — no
  DP-map infra for those shapes.
- **HONYAR multi-gang switches collided all gangs on a single `state` key
  and never bound the upper endpoints.** z2m gives U2-86K21ND10-ZD (HY0096,
  2-gang `{left:1, right:2}`), U2-86K31ND10-ZD (HY0097) and U86K31ND6
  (00500c35, both 3-gang `{left:1, center:2, right:3}`) a switch on every
  endpoint with genOnOff bound per endpoint (`e.switch().withEndpoint(...)`
  / `m.deviceEndpoints` + `m.onOff`). The generated defs carried a single
  bare `state` expose and a single EP1 binding (HY0096/HY0097 also lacked
  the `endpoint_map` entirely), so the runtime suffix rewrite could not
  separate the gangs — they overwrote each other on the bare `state` key —
  and EP2/EP3 were never bound. Graduated each to a Tier-2 parent override
  with per-gang suffixed exposes (`state_left`/`state_center`/`state_right`),
  per-endpoint genOnOff bindings, and the `endpoint_map`. (The HY0157
  dual-socket metering plug's per-endpoint split is INFRA-deferred — its
  `multiEndpointSkip` metering keys have no per-def runtime hook.)
- **CLIMAX SCM-5ZBS cover and SRAC-23B-ZBSR siren had dead decode/control
  surfaces.** The SCM-5ZBS roller shutter was wired to the
  `closuresWindowCovering` cluster (0x0102) it never speaks; z2m proxies
  position through `genLevelCtrl` `currentLevel` + `genOnOff` (z2m
  `fz.cover_position_via_brightness` / `fz.cover_state_via_onoff` /
  `tz.cover_via_brightness`), so both position decode and control were dead.
  Added reusable generic `kFzCoverPositionViaBrightness` /
  `kFzCoverStateViaOnOff` / `kTzCoverViaBrightness` converters and rewired the
  shutter (bindings genLevelCtrl 0x0008 + genOnOff 0x0006). The SRAC-23B-ZBSR
  siren wired the generic `kFzIasZone`, which only reads the `ssIasZone`
  ZoneStatus *attribute* report — but z2m `fz.ias_siren` decodes the
  `commandStatusChangeNotification` *command*, so the `alarm` key was dead on a
  real siren trigger. Swapped to `kFzIasGenericAlarm` (command-path bit0 →
  `alarm`) and regained the dropped `ias_wd` `max_duration` channel
  (`kFzIasWdMaxDuration` / `kTzIasWdMaxDuration`, ssIasWd 0x0502) plus the
  missing `warning` / `squawk` / `max_duration` exposes.
- **KWIKSET SmartCode locks bound reporting on the wrong endpoint (no lock-state
  / battery reports).** All 7 Kwikset defs in z2m configure via
  `device.getEndpoint(2)` and bind `closuresDoorLock` (0x0101) + `genPowerCfg`
  (0x0001) on endpoint 2. The generated defs bound both clusters on endpoint 1,
  and `run_configure()` passes the bind endpoint verbatim to `configure_bind`,
  so the coordinator bound an absent endpoint and the lock never set up
  lock-state or battery reporting. Graduated all 7 to Tier-2 parents,
  re-endpointed bindings to `{2, 0x0001}` + `{2, 0x0101}`, and added
  `.default_endpoint=2` (the onesti S4RX-110 precedent). The fz/tz surface
  (lock_state, battery, action stream, lock/unlock + PIN encoders) was already
  correct and is now pinned by `tests/test_kwikset_parity.cpp`.

- **TERNCY raw-frame action/motion decoder read past the body (dead action +
  occupancy surface).** z2m's `fzLocal.terncy_raw` reads the WHOLE raw frame
  buffer (`msg.data`, ZCL header included): `msg.data[4]` discriminates action
  vs motion, `msg.data[6]`/`[7]` carry the action/side value. The AduroSmart
  cluster (0xFCCC) frame is manufacturer-specific so its header is 5 bytes and
  ZHC's `raw_body` begins at full-frame index 5. The shared `kFzTerncyRaw*`
  decoder re-used z2m's full-frame offsets against `raw_body` (`raw_body[4]/[6]/
  [7]`) AND required `raw_body.size() >= 8`, so on real (~3-byte body) frames it
  matched nothing — PP01 awareness switch / SD01 knob / LS01 socket emitted no
  `action` and PP01 emitted no motion `occupancy`/`action_side`. Fixed to
  discriminate on `msg.command_id` (z2m's `msg.data[4]` IS the command id) and
  read the values at `raw_body[1]`/`raw_body[2]`. Also corrected the converter
  direction from `ClientToServer` to `ServerToClient` — the device-originated
  frames carry the server→client bit (z2m wire byte fc=0x0D), so even with
  correct offsets they would never have matched.
- **Sylvania 74388 — contact state decoded to the dead `alarm` key.** The
  Smart+ contact + temperature sensor lowered the generic `kFzIasZone`
  converter (emits the bare `alarm` key) while z2m's
  `fz.ias_contact_alarm_1` publishes the semantic key `contact` (zoneStatus
  bit 0; tamper bit 2; battery_low bit 3). The expose declared `alarm`, so
  the door/window state never reached the `contact` shadow key. Graduated to
  a parent override that swaps in the typed `kFzIasContactAlarm` and renames
  the expose to `contact`; the temperature axis (msTemperatureMeasurement
  0x0402) was already wired. The OSRAM `set_transition`/`remember_state`
  manuSpecific commands (cluster 0xfc0f) on the light family remain
  INFRA-deferred.
- **Konke 2AJZ4KPFT / KK-ES-J01W — sensor channels reduced to battery-only.**
  Both temperature/humidity sensors were auto-ported with only `kFzBattery`
  wired, silently dropping every measurement channel. z2m surfaces
  `fz.temperature` + `fz.humidity` on 2AJZ4KPFT and additionally
  `m.illuminance()` on KK-ES-J01W. Graduated both defs and restored the generic
  msTemperatureMeasurement (0x0402), msRelativeHumidity (0x0405) and
  msIlluminanceMeasurement (0x0400) decoders + the matching exposes/bindings.
  (Per-model fingerprint gating of KK-ES-J01W's `TS0222` to
  `_TYZB01_fi5yftwv` is left INFRA-deferred — the matcher's manufacturer
  gating is whole-def, so it cannot be applied to one model without also
  gating the ungated `3AFE090103021000` variant.)
- **MAZDA TR-M2Z — truncated + mis-scaled `local_temperature_calibration`
  (DP47).** The auto-ported Tuya DP map for both fingerprints
  (`_TZE204_k6rdmisz` / `_TZE284_k6rdmisz`) declared `count=23` while the entry
  table held 24 rows, silently truncating the last row so DP47 was never
  decoded. Once restored to 24, DP47 still carried `divisor=1`, but z2m wires
  `localTempCalibration3` whose decode is `v/10` (signed s32, tenths of a
  degree). Set `divisor=10` and `count=24` on both variants; graduated the two
  generated files to the parent `definitions/mazda/` dir as Tier 2 ports.

- **AduroSmart 81848 / 81998 / 81949 — phantom `energy` channel + seMetering
  (0x0702) on electrical-only power devices.** All three z2m defs use
  `electricityMeter({cluster:"electrical"})` / `fz.electrical_measurement`,
  which reads the haElectricalMeasurement cluster (0x0B04) ONLY —
  power/voltage/current with NO `energy`. The shared `kFzAdu*EM` bundles also
  wired the generic `kFzMetering` (seMetering 0x0702) decoder, declared a
  phantom `energy` (kWh) expose and bound 0x0702, none of which z2m surfaces.
  Made the EM bundles electrical-only (kFzElectricalMeasurement only; no
  `energy` expose; no 0x0702 binding). The 81949 dimmer's 9 vendor-specific
  genBasic config extends remain INFRA-deferred (manuSpecific attributes, no
  existing converter).
- **Sercomm SZ-PIR04N — dropped illuminance channel.** z2m wires this PIR
  motion/temperature sensor with `extend: [m.illuminance()]` on top of
  `ias_occupancy_alarm_1` + temperature + battery, exposing `illuminance`
  (`msIlluminanceMeasurement` 0x0400 measuredValue). The auto-port dropped the
  illuminance channel entirely — no `kFzIlluminance` decoder, no `illuminance`
  expose, no 0x0400 binding — so ambient-light readings never reached the
  shadow. Graduated to a Tier-2 parent override wiring the generic
  `kFzIlluminance` converter, the `illuminance` expose, and a 0x0400 binding.

- **Livolo TI0001-* family — every custom-cluster decoder was dead, plus
  phantom/mismatched exposes.** Livolo devices speak a bespoke raw protocol
  over genPowerCfg (0x0001) whose body starts with the magic header
  `[0x7A 0xD1]`. Those frames decode to `MessageType::Raw` (frame control
  0x7A is not cluster-specific), but every Livolo `FzConverter` gated only on
  `MessageType::Command`, so none ever matched — added `type_bit(Raw)` to all
  of them (matching the terncy/orvibo raw-cluster precedent). The 4-gang
  TI0001 decoded `state_left/_right/_bottom_left/_bottom_right` but exposed
  only a bare `state`; corrected to the four keyed switches. TI0001-pir
  carried a phantom IAS bundle (`alarm`/`tamper`/`battery_low` + ssIasZone
  binding) while its decoder emits `occupancy`; fixed to occupancy +
  genPowerCfg binding. TI0001-curtain-switch / -dimmer / -cover were wired to
  dead generic on/off / brightness / cover-position converters (standard
  0x0006/0x0008/0x0102) that never match the raw frame; ported the custom
  `livolo_curtain_switch_state` (state_left/state_right, non-bitmask
  polarity), `livolo_dimmer_state` (state + brightness) and
  `livolo_cover_state` (position/state/moving/motor_speed/motor_direction)
  decoders. Cover/dimmer *write* paths (livolo_cover_* / livolo_dimmer_level
  bespoke manuSpec encoders) remain INFRA-deferred — those exposes are
  read-only. Graduated the five affected defs to Tier-2 parents. Covered by
  `tests/test_livolo_parity.cpp`.
- **KMPCIL-tag-001 arrival sensor — dead `battery` expose.** The def declares
  `e.battery()` and z2m's `presence_power` converter applies
  `meta.battery.voltageToPercentage: "3V_1500_2800"` to publish `battery`, but
  the ported `kFzKmpcilPresencePower` emitted only `voltage` (mV) from
  `genPowerCfg` batteryVoltage — the battery expose was never filled. The
  shared converter now also derives battery % from the z2m-faithful non-linear
  curve `round(clamp(235 - 370000/(voltage_mV+1), 0, 100))`. Graduated
  `Kmp_KMPCIL_tag_001.cpp` to a Tier-2 parent. The presentValue bitfield
  decoders (presence/power_state/vibration/occupancy) and the RES005
  genBinaryInput/Output occupancy/state bespoke decoders are pinned as
  regression guards in `tests/test_kmpcil_parity.cpp`.

- **Woox R7051 smart siren (TS0219) — phantom `tamper` / `battery_low`
  exposes and over-broad IAS decode.** z2m decodes the alarm via
  `fz.ias_alarm_only_alarm_1` (ssIasZone attr 0x0002 bit 0 → `alarm` only),
  but the generated def lowered the broader `kFzIasZone` (which also emits
  `tamper` + `battery_low`) and declared phantom `tamper` / `battery_low`
  exposes z2m never surfaces for this device. Added a generic
  `kFzIasZoneAlarmOnly` converter and graduated R7051 to a parent override
  using it with the two phantom exposes removed. (volume/duration write and
  `ac_connected` from genBasic.powerSource remain INFRA-deferred — no generic
  converter yet.)
- **RGB Genie ZB-5028 4-EP scene remote — colliding per-endpoint actions.**
  z2m marks the ZB-5028 (and sibling ZB-3008) `meta: {multiEndpoint: true}`
  with no `endpoint()` map, so `postfixWithEndpointName` suffixes every action
  with the raw endpoint ID and the four buttons emit distinct events. The port
  declared an `endpoint_map` (ep1..ep4) but omitted `endpoint_action_suffix`,
  so the dispatch kept `action` global (it is in `kAlwaysGlobalKeys`) and all
  four endpoints collapsed onto one bare `action` key — ZB-3008 had the flag,
  ZB-5028 was missed. Added `.endpoint_action_suffix = true` (graduated
  `Rgb_ZB_5028.cpp` to a Tier-2 parent). Also corrected stale "command_move_*
  not yet wired in _generic" port notes (those ColorCtrl move decoders are
  wired and fire). Covered by the new `tests/test_rgb_genie_parity.cpp`.
- **Linkind ZS230002 5-key smart-bulb dimmer remote — wrong color
  converters.** The hand-port wired `kFzCommandStepColorTemp`
  (lightingColorCtrl cmd 0x4C → `color_temperature_step_*`, a converter z2m
  does not use here) and `kFzCommandMoveToHueAndSaturation` (cmd 0x06 → a
  phantom `move_to_hue_and_saturation` action), with a stale comment claiming
  the correct generic converters were missing. z2m's actual fz are
  `command_move_color_temperature` (cmd 0x4B → `color_temperature_move_up/_down/_stop`)
  and `command_move_to_color` (cmd 0x07 → `color_move`); both generic
  converters (`kFzCommandMoveColorTemperature`, `kFzCommandMoveToColor`)
  already exist and are now wired, so the discrete color-temp move + color_move
  actions decode and the phantom action is dropped from the expose enum.
  Graduated `Lin_ZS230002.cpp` to a parent override. Added
  `tests/test_linkind_parity.cpp`.

- **Viessmann ViCare radiator TRV (ZK03840) — false `window_open` flag and
  dead `occupied_heating_setpoint`.** Two decode bugs against z2m's
  `fzLocal.viessmann_thermostat`: (1) the manuSpec attr 0x4000
  `viessmannWindowOpenInternal` was flattened to `window_open = (raw != 0)`,
  but z2m treats only raw 3/4 as window-open (codes 0-2/5 are "unknown" → not
  open), so codes 1/2/5 false-flagged the window as open; (2) the def declared
  the z2m expose `occupied_heating_setpoint`, but the wired generic
  `kFzThermostat` emits attr 0x0012 as `current_heating_setpoint`, leaving the
  setpoint readout dead. `kFzViessmannThermostat` now decodes the standard
  surface (local_temperature 0x0000, occupied_heating_setpoint 0x0012,
  system_mode 0x001C) with z2m-faithful keys plus the manuSpec trio with the
  `raw==3||raw==4` window rule; generic `kFzThermostat` dropped from the def
  (no phantom `current_heating_setpoint`). Graduated `Vie_ZK03840.cpp` to a
  Tier-2 parent. Covered by `tests/test_viessmann_parity.cpp`.

- **Salus Controls RE600 Zigbee router — phantom on/off + battery bundle.**
  z2m declares the RE600 with `fromZigbee:[]`, `toZigbee:[]`, `exposes:[]` (a
  pure mains repeater with no reportable state), but the auto-generated port
  carried a phantom `state` (on/off) plus a `battery`/`voltage` bundle, wired
  `kFzOnOff`/`kFzBattery`, and bound `genOnOff` (0x0006) + `genPowerCfg`
  (0x0001). Graduated `Sal_RE600.cpp` to a Tier-2 parent stripping all
  exposes/decoders/toZigbee/bindings to match z2m exactly. Added
  `tests/test_salus_controls_parity.cpp`, which also guards the already-correct
  SW600/OS600/WLS600 typed-IAS decode, the FC600 flat thermostat surface, and
  the SP600/SPE600/SX885ZB metering plugs (seMetering 0x0702 only, no phantom
  haElectricalMeasurement 0x0B04).
- **Profalux covers + remotes — phantom on/off, dropped tilt, dead
  position.** Three z2m-parity fixes graduated out of `generated/`: **NB102**
  is an empty cover remote in z2m (`fromZigbee:[]`/`toZigbee:[]`/`exposes:[]`)
  but the port hallucinated an on/off `state` + battery bundle — stripped to
  an empty definition. **MOT-C1ZxxC/F** is a lift+tilt cover on EP2
  (`closuresWindowCovering`) but the port wired `genOnOff` behind a phantom
  `state` and dropped the tilt half — now decodes `position` (attr 0x0008) +
  `tilt` (attr 0x0009), encodes via cover-state/lift/tilt, binds 0x0102 on
  EP2 with `default_endpoint=2`, and carries no `state` expose. **NSAV061**
  is an older cover driven via `genLevelCtrl currentLevel` (position) +
  `genOnOff` (OPEN/CLOSE) but the port used the generic `kFzBrightness` (raw
  `brightness` key, no 0..100 scale), so `position` decoded to nothing —
  added `profalux/_shared.{hpp,cpp}` with cover-via-brightness converters
  (scaled `position` + OPEN/CLOSE `state` decode, `position`→genLevelCtrl
  level encode) mirroring the iluminize 5128.10 precedent. Covered by
  `tests/test_profalux_parity.cpp`.

- **EcoDim ED-10011/12/13/14/15 wall remotes — buttons collapsed onto one
  `action` key.** z2m decodes these 2-/4-/8-button remotes with
  `fz.command_on/off/move/stop` + `fz.battery` and distinguishes each
  button-pair per endpoint (`on_1` … `brightness_stop_4`). The auto-port emitted
  a bare `action` (a `kAlwaysGlobalKey`), so every button on every endpoint
  collapsed onto a single key and the originating endpoint was lost. Graduated
  the five defs out of `generated/` and added an `endpoint_map` (label = the
  digit) + `endpoint_action_suffix` so the dispatcher rewrites the key to
  `action_<n>` per endpoint — same convention as robb / vesternet. ED-10010 is
  intentionally left unsuffixed (z2m exposes `action` with no `_n` suffix
  there). Covered by `tests/test_ecodim_parity.cpp`.
- **Mercator Ikuü SSWF01G AC fan controller — dead `fan_state` key.** z2m's
  `fz.fan` decodes `hvacFanCtrl` attr 0x0000 (fanMode) and publishes BOTH
  `fan_mode` and `fan_state` ("OFF"/"ON"), but the generated port wired the
  generic `kFzFanMode` (which emits `fan_mode` only) while still declaring a
  `fan_state` expose — so that key never populated. Graduated `Mer_SSWF01G.cpp`
  to a Tier-2 parent with an inlined vendor converter (`kFzMercatorFan`) that
  emits `fan_mode` (u8) plus `fan_state` (Bool: off/0 → false) from the same
  report, mirroring z2m. Covered by `tests/test_mercator_parity.cpp`.
- **TECH Controllers VNTH-T2 radiator valves — dropped `system_mode`
  channel.** Both TS0601 TRVs (`VNTH-T2_v1` `_TZE204_r7brscr6`, `VNTH-T2_v2`
  `_TZE204_p1qrtljn`) map z2m DP 101 to BOTH `state` (on/off) AND `system_mode`
  (`lookup({heat: true, off: false})`), but the auto-generated DP extract kept
  only the `state` row, so the heat/off mode was never decoded or exposed.
  Added a reusable `kTuyaDpFlagBoolEnum` flag to the Tuya DP infra (a boolean
  DP fanned to a string label via its `enum_table`), graduated both ports out
  of `generated/` and added a second DP-101 row + `system_mode` enum expose to
  each. The signed-int32 numeric decode already matches z2m's
  `localTempCalibration1` sign-correction (no separate fix needed).
- **Imhotep Creation E-Ctrl + BRI4P thermostats — dead setpoint-limit /
  cooling-setpoint exposes.** Both defs surface standard `hvacThermostat`
  attributes that z2m's `fz.thermostat` decodes but the generic
  `kFzThermostat` (0x0000/0x0012/0x001C only) drops. The E-Ctrl heater
  thermostat (and whitelabels RSS/RPH E-Ctrl) exposed
  `min_heat_setpoint_limit` / `max_heat_setpoint_limit` (attrs
  0x0015/0x0016) with no decoder — dead keys. The BRI4P 16-channel
  underfloor-heating bridge carried only local_temperature /
  current_heating_setpoint / system_mode, while z2m exposes per endpoint a
  cooling setpoint (0x0011) plus min/max heat and cool setpoint limits —
  entirely missing exposes + decoder. Added a vendor-local
  `kFzImhotepThermostatExtras` (raw s16 pass-through, runtime /100) that
  read-decodes the three keys z2m's `fz.thermostat` actually reports
  (0x0011/0x0015/0x0016) wired alongside the generic decoder; added the
  missing BRI4P exposes + the generic min/max heat/cool setpoint-limit tz
  converters; and graduated both ports to Tier-2 parent overrides. The
  min/max cool setpoint-limit keys (0x0017/0x0018) are write-path-only in
  z2m (no `fz.thermostat` report branch), so their settable exposes are
  driven by the tz write/get path — not read-decoded.

- **Iluminize 5128.10 roller shutter + 5715/5717 metering dimmers — dead
  decoders.** The 5128.10 "switch shutter with level control" is a
  brightness-driven cover in z2m (`fz.cover_position_via_brightness` +
  `fz.cover_state_via_onoff` + `fz.cover_position_tilt`), but the
  auto-generated port wired only the generic `kFzCoverPosition`
  (`closuresWindowCovering` attr 0x0008) and exposed `position` alone — a real
  device reports its open percentage on `genLevelCtrl.currentLevel` and
  OPEN/CLOSE on `genOnOff`, so nothing decoded and `state` never surfaced.
  Added vendor-shared `kFzIluCoverViaBrightness` / `kFzIluCoverStateViaOnOff`
  (+ `kTzIluCoverViaBrightness`), graduated `Ilu_D5128_10.cpp` to a Tier-2
  override wiring those legs + the windowCovering lift fallback + a `state`
  expose (matching `e.cover_position()`) and binding 0x0006/0x0008/0x0102.
  The 5715/5717 dimmers use `m.electricityMeter()` (default `cluster:"both"`)
  but dropped the entire `haElectricalMeasurement` (0x0B04) half — `current`
  and `voltage` never decoded; graduated both to add
  `kFzElectricalMeasurement` + `current`/`voltage` exposes + the 0x0B04 bind.
  New fixture `tests/test_iluminize_parity.cpp` pins the cover
  position/state/lift decode and the metering current/voltage/energy decode.
  The remotes (511.324/.524/.541/.544/.557, 5144.\*) and the light/onoff
  bundles were verified CLEAN; sunricher `externalSwitchType`/`minimumPWM`
  manuSpecific config attributes were deferred (no generic converter).
- **Onesti S4RX-110 ("Relax" smart plug) — internal die-temperature channel
  was dropped.** z2m wires `fz.device_temperature` + `e.device_temperature()`
  and binds `genDeviceTempCfg` (0x0002), but the auto-generated port carried
  only `kFzOnOff` + `kFzMetering` + `kFzElectricalMeasurement`, so the
  `device_temperature` value never surfaced (same gap class as Dawon
  PM-B540). Graduated `One_S4RX_110.cpp` from `generated/` to a Tier-2 parent
  override wiring the generic `kFzDeviceTemperature` + the `device_temperature`
  expose + the 0x0002 bind; bindings retargeted to endpoint 2 and
  `default_endpoint=2` set to mirror z2m's `endpoint: () => ({default: 2})`.
  New fixture `tests/test_onesti_parity.cpp` pins the device_temperature
  decode (positive + negative, no `temperature` leak), the
  metering/electrical-measurement regression, the endpoint-2 binds, and the
  lock family's static surface. The two Onesti door locks (easyCodeTouch_v1,
  Nimly) are CLEAN on their generic core (lock/battery/action/pincode); their
  vendor-specific `nimly_pro_lock_actions` (closuresDoorLock attrs 256/257)
  and `easycodetouch_action` (raw-cluster lookup) decoders are INFRA-deferred.

- **Feibit (Heiman-OEM) family — IAS dead-keys, dropped sensor channels,
  missing cover tilt, and dead-control remotes.** A vendor-wide parity pass
  fixed 17 definitions. Six IAS sensors (SBM01ZB motion→`occupancy`, SSA01ZB
  →`smoke`, SCA01ZB→`carbon_monoxide`, SGA01ZB→`gas` [alarm_2/bit 1], SWA01ZB
  →`water_leak`, SDM01ZB→`contact`) and the SEB01ZB SOS button (→`sos`,
  alarm_2/bit 1) wired the generic `kFzIasZone` (bare `alarm`) against semantic
  exposes the converter never produced; swapped to the typed
  `kFzIas<Type>Alarm` converters and added a new generic `kFzIasSosAlarm2`.
  STH01ZB regained `temperature`/`humidity` (only `kFzBattery` was lowered);
  TSKT222W-H4 regained the `haElectricalMeasurement` (0x0B04) half
  (`voltage`/`current`) of `m.electricityMeter()` `cluster:"both"`. Two
  lift+tilt curtain motors (SZT211_AW-P1, TCUR218W-V1) regained their `tilt`
  channel via a new generic `kFzCoverTilt` (attr 0x0009); TCUR218W-V1 also had
  its endpoint IDs corrected to z2m's `{top:15, bottom:14}`. Five
  remotes/scene-switches mis-modelled as dead settable on/off switches were
  rebuilt as action devices: SZSN325W-Q (`command_recall`), NZRC106W-M2
  (IAS-ACE arm/panic + battery), TZSN408W-V1 / NZSN421W-Q (4-gang
  `commandsOnOff` → `action_1..4`), FMRC209W (`commandsOnOff` +
  `commandsLevelCtrl`); SSS401ZB regained its scene-recall `action` while
  keeping its controllable switch.
- **Datek HSE2927E "Motion Sensor" (Eva motion) — illuminance channel was
  dropped.** z2m wires `extend: [m.illuminance()]`, adding a
  `msIlluminanceMeasurement` (0x0400) `measuredValue` -> `illuminance`
  decoder + expose, but the generated port carried only
  battery/temperature/IAS-motion/`led_on_motion`. Wired the existing
  generic `kFzIlluminance`, added the `illuminance` expose and the 0x0400
  bind, and graduated the def to a Tier-2 parent.
- **Datek HSE2905E "Meter Reader" (Eva AMS HAN power-meter) — produced
  energy and the per-phase B/C voltage/current channels were dropped.**
  z2m stacks two `m.electricityMeter` bundles: `metering`
  (`producedEnergy: true`) and `electrical` (`threePhase: true,
  power: false`), giving `produced_energy` and per-phase B/C voltage/current
  on top of the phase-A core. (`electricityMeter()` defaults
  `acFrequency: false` / `powerFactor: false` and this bundle does not
  override them, so z2m exposes neither — nor do we.) The generated port
  wired only the generic `kFzMetering` (energy 0x0000 / power 0x0400) +
  `kFzElectricalMeasurement` (power 0x050B / voltage 0x0505 / current
  0x0508), so the extra channels were dead exposes. Added Datek
  `kFzMeteringExtras` (seMetering `produced_energy` 0x0001) +
  `kFzElectricalMeasurementExtras` (haElectricalMeasurement
  `voltage/current_phase_b` 0x0905/0x0908, `voltage/current_phase_c`
  0x0A05/0x0A08) in `definitions/datek/_shared.cpp`, wired ALONGSIDE the
  generics, added the missing exposes, and graduated the def to a Tier-2
  parent. Mirrors the `definitions/bituo_technik` approach (raw
  pass-through; runtime scales).
- **Dawon DNS PM-B540-ZB (16 A metering plug) — internal die-temperature
  channel was dropped.** Alone among the Dawon metering plugs, the PM-B540
  also reports its own temperature on `genDeviceTempCfg` (0x0002); z2m wires
  `fz.device_temperature` + `e.device_temperature()` and binds the cluster,
  but the auto-generated port carried only `kFzOnOff` + `kFzMetering`, so the
  `device_temperature` value never surfaced. There was no generic decoder for
  cluster 0x0002 at all. Added `0x0002 → "genDeviceTempCfg"` to the cluster
  name table and a new generic `kFzDeviceTemperature` converter (attr 0x0000
  `currentTemperature`, s16 whole °C, emitted unscaled — distinct from
  `kFzTemperature`'s 0.01 °C `msTemperatureMeasurement` path), then graduated
  `Daw_PM_B540_ZB.cpp` from `generated/` to a Tier-2 parent override wiring
  the converter + `device_temperature` expose + 0x0002 bind. New fixture
  `tests/test_dawon_dns_parity.cpp` pins the device_temperature decode
  (positive + negative, no `temperature` leak), the seMetering energy/power
  regression, and structural guards over the rest of the family (a sibling
  PM-B530 plug that must *not* grow the channel, TH-110 temp/humidity, the
  multi-gang endpoint_maps). The remaining Dawon devices were verified CLEAN
  against z2m.

- **Lonsonho X701A (1-gang switch with backlight) — one hardware variant
  never matched.** z2m fingerprints the device as
  `tuya.fingerprint("TS0001", ["_TZ3000_t3s9qmmg", "_TZ3000_ehgouyvu"])`,
  but the generated def carried only `_TZ3000_t3s9qmmg`, so units
  reporting `_TZ3000_ehgouyvu` matched no definition at all. Graduated
  `Lon_X701A.cpp` from `generated/` to a Tier-2 parent override restoring
  both manufacturer names. New fixture `tests/test_lonsonho_parity.cpp`
  pins the fingerprint fix (resolved through the registry) plus structural
  guards over the rest of the family: multi-gang switch endpoint_maps,
  the attribute-based TS130F covers exposing `position` on
  closuresWindowCovering (0x0102, not a Tuya-DP curtain), the dual-cover
  per-endpoint suffixing, and the RGBCW colour surface. The Tuya-DP
  devices (X711A/X712A/X713A via `legacy.fz.tuya_switch`, VM-Zigbee-S02
  via `tuyaDatapoints`) remain INFRA-deferred — their exposes/endpoint
  maps mirror z2m but DP decode awaits the Tuya-DP dispatch layer.

- **Müller-Licht (tint) battery remotes — advertised colour-wheel and
  store actions were dead enum entries with no decoder.** The three tint
  remotes shipped `action` enums matching z2m but only wired the
  genOnOff/genLevelCtrl command decoders, so several advertised events
  could never surface. Graduated all three from `generated/` to Tier-2
  parent overrides and wired the generic command decoders that already
  exist (used by paulmann/legrand):
  - **MLI-404011/MLI-404049** — added `kFzCommandMoveToColorTemp`
    (`color_temperature_move`, z2m `fz.tint404011_move_to_color_temp` — the
    bare string matches; z2m's `_up`/`_down` suffix is internal
    state-tracking, not on the wire) and `kFzCommandMoveToColor`
    (`color_move`, `fz.command_move_to_color`).
  - **404022/404049C** — added `kFzCommandMoveToColor` (`color_move`);
    `color_temperature_move` was already wired.
  - **404002** — added `kFzCommandStore` (`store_1`, `fz.command_store`);
    `recall_1` already decoded via `kFzCommandRecall`.

  `scene_<n>` (z2m `fz.tint_scene`) stays a deliberately unwired INFRA
  gap: it decodes a genBasic *Write Attributes* of attr 0x4005, but the
  foundation Write-attribute parser does not populate `DecodedMessage`'s
  payload, so the generic `kFzTintScene` converter can never fire — wiring
  it would be inert. Tracked for a shared Write-attr parser fix. New
  fixture `tests/test_muller_licht_parity.cpp` pins the restored
  colour-wheel + store decodes (and the recall sanity path) and pins the
  scene gap so a future parser fix flips it intentionally.

- **MindY Leleka air-quality monitor — the `co2` channel was silently
  dropped.** The def declared a `co2` expose and bound the standard msCO2
  cluster (0x040D) but omitted the decoder from its from-zigbee list; its
  header comment claiming "Tier-1 has no kFzCO2" was stale (the generic
  `kFzCO2` already exists, matching z2m `fz.co2` =
  `floor(measuredValue * 1e6)` → ppm). Graduated the def `generated/` →
  Tier-2 parent override and wired `generic::kFzCO2`. The sibling
  temperature / humidity / pressure / illuminance channels already decoded
  via the generic converters. The manuf-specific config knobs
  (read_interval, night_*, co2_*, lux_*, offset_*, temperature_sensor) and
  the per-device last_boot / wifi surfaces have no generic decoder and stay
  deferred (write-side infra). New fixture `tests/test_mindy_parity.cpp`
  pins the restored CO2 ppm decode (exclusive — no phantom sibling keys)
  and regression-checks the four sibling channels. Mirrors the
  wirenboard / diyruz CO2 fixes.

- **Aurora "AOne" non-light family — sensors decoded the wrong IAS key and
  the remote + wireless dimmers had their entire action stream dropped.**
  Five Aurora defs were graduated from `generated/` to Tier-2 parent
  overrides:
  - **AU-A1ZBPIRS** (PIR) decoded the generic `kFzIasZone` bare key `alarm`
    instead of z2m's `fz.ias_occupancy_alarm_1` → `occupancy`, and dropped
    the `m.illuminance()` lux channel entirely. Re-pointed at
    `kFzIasMotionAlarm` (emits `occupancy`) + added `kFzIlluminance`, the
    `illuminance` expose and the `msIlluminanceMeasurement` (0x0400) binding.
  - **AU-A1ZBDWS** (door/window contact) had the same `kFzIasZone` → `alarm`
    bug vs `fz.ias_contact_alarm_1` → `contact`. Swapped to
    `kFzIasContactAlarm`; battery channel unchanged.
  - **AU-A1ZBRC** (smart remote) was wrong-bundled as a settable on/off light
    (`kFzOnOff` + `kTzOnOff` + writable `state`), so every button press was
    dead and it falsely advertised a relay. Rewired to the genOnOff /
    genLevelCtrl(step) / genScenes(recall, store) command decoders, exposing
    `action`; phantom `state` and toolbox removed.
  - **AU-A1ZBR1GW** (1-gang wireless rotary dimmer, single endpoint) had its
    action stream dropped the same way. Rewired to genOnOff / genLevelCtrl /
    lightingColorCtrl(stepColorTemp) command decoders + `action`.
  - **AU-A1ZBR2GW** (2-gang wireless rotary dimmer, multi-EP {right:1,
    left:2}) had its action stream explicitly deferred. Wired the command
    decoders and enabled `endpoint_action_suffix` so each gang keeps identity
    (`action_right` / `action_left`); battery stays a global key.
  New fixture `tests/test_aurora_lighting_parity.cpp` pins the typed sensor
  keys (no bare `alarm`), illuminance decode, the action labels per command,
  and the 2-gang per-endpoint suffixing. Mirrors the heiman / paulmann fixes.

- **Sengled IAS sensor family — motion and contact sensors shipped with
  their primary state dead.** Four Sengled IAS-Zone defs lowered (or, for
  E13-N11, half-rewrote to) the generic `kFzIasZone` converter — which emits
  the bare key `alarm` — while z2m's `m.iasZoneAlarm({zoneType})` publishes a
  *semantic* key (`occupancy` / `contact`). With no rename layer the decoded
  alarm never reached the matching expose, so the sensor's whole reason for
  existing was dead in the shadow. Each was graduated to a Tier-2 parent and
  re-pointed at the typed `kFzIas{Motion,Contact}Alarm` converter (which
  emits the semantic key directly, matching `fz.ias_<type>_alarm_1` on
  zoneStatus bit 0), mirroring the heiman IAS family fix:
  - **E1M-G7H** (motion sensor) → `kFzIasMotionAlarm`, expose `occupancy`.
  - **E1D-G73WNA** (window/door contact) → `kFzIasContactAlarm`, expose `contact`.
  - **E2D-G73** (window/door contact G2) → `kFzIasContactAlarm`, expose `contact`.
  - **E13-N11** (flood light + motion) → the 2026-04-28 rewrite had already
    renamed the expose to `occupancy` but left the decoder as `kFzIasZone`,
    so occupancy stayed dead; swapped the decoder to `kFzIasMotionAlarm`. The
    light channel is unchanged.

  New `tests/test_sengled_parity.cpp` pins each device's semantic key decode
  (and the absence of the bare `alarm` key), tamper/battery_low passthrough,
  and that E13-N11's on/off light channel still decodes alongside occupancy.

- **Paulmann remote family — all five battery remotes shipped with every
  button dead.** Paulmann is otherwise a clean lighting vendor, but its
  scene/RGB remotes (501.37, 501.34, 500.67, 501.40, 501.41) each carry
  `toZigbee: []` (they cannot be controlled) yet the generator wrong-bundled
  every one as a settable on/off light — `kFzOnOff` + `kTzOnOff` + a writable
  `state` expose — so the `action` decode was dropped and each device falsely
  advertised a relay. All five generated Tier-1 defs were graduated to Tier-2
  parents and re-wired to the generic genOnOff / genLevelCtrl /
  lightingColorCtrl / genScenes command-action decoders, exposing `action`
  and dropping the phantom state + toolbox:
  - **501.37 / 501.34** (4-button white switches, z2m `multiEndpoint`): two
    rocker pairs on EP1/EP2 emitting `command_on/off/move/stop`. Added an
    endpoint label map `{1,2}` + `endpoint_action_suffix`, so presses surface
    as `action_1` / `action_2` (z2m on_1..off_2 / brightness_move_*_N). 501.34
    is the `brightness_stop` variant of 501.37.
  - **500.67** (RGB remote, single endpoint): `command_on/off/toggle/step/
    move/stop/move_to_color_temp/move_to_color/color_loop_set/
    enhanced_move_to_hue_and_saturation`. The Tint-specific `fz.tint_scene`
    (`scene_*`) is a genBasic write recall with no generic ZHC equivalent
    (same gap as innr's `rc_110_level_to_scene`) and is deferred.
  - **501.40** (4-button RGB remote, z2m `deviceEndpoints{1..4}`): full
    commandsOnOff/LevelCtrl/ColorCtrl/Scenes set across EP1-4 with an
    endpoint label map `{1,2,3,4}` + `endpoint_action_suffix` (`action_1`..
    `action_4`); battery stays a global key.
  - **501.41** (white remote, single endpoint): commandsOnOff/LevelCtrl/
    ColorCtrl(color-temp move+step)/Scenes(store,recall). The custom
    `paulmann50141ColorTemperatureStopCommand` only recovers
    `color_temperature_move_stop` from a malformed 5-byte raw frame; the
    normal move-stop is already decoded, so the raw quirk is deferred to a
    vendor fz. New `tests/test_paulmann_parity.cpp` pins the per-endpoint
    action suffix on the multi-button remotes, the bare-`action` stream on
    the single-endpoint remotes (incl. color-temp move and genScenes
    `recall_<n>`), the absence of a settable `state`, and the empty toolbox.

- **AwoX / EGLO non-light family — a PIR sensor and two remotes shipped
  with their primary signal dead.** Three generated Tier-1 defs were
  mis-modelled and graduated to Tier-2 parents:
  - **EPIR_Zm** (Connect-Z PIR motion sensor, EGLO 99106) wired only
    `kFzOnOff` + `kFzBattery` and exposed `state`/`battery`/`voltage` —
    `occupancy`, the whole reason the device exists, was dropped, so the
    `msOccupancySensing` (0x0406) report was dead. z2m models it as
    `m.battery() + m.occupancy() + …`. Added the generic `kFzOccupancy`
    decoder, an `occupancy` Binary expose, and the 0x0406 binding. (No
    illuminance channel exists in z2m despite the "motion + light"
    branding.)
  - **ERCU_WS_Zm** (Connect-Z wall-mount light remote, EGLO 900116) and
    **33952 / ERCU_Zm** (remote controller) were both stubbed as on/off
    switches (`kFzOnOff` + a controllable on/off TZ, both dead — they are
    pure command transmitters with no relay) exposing a meaningless
    `state`. z2m exposes `action` driven by genOnOff / genLevelCtrl /
    lightingColorCtrl (/genScenes) client commands. Re-wired both to the
    generic command-action decoders and exposed `action`; ERCU_WS_Zm is
    multi-endpoint (`{"1":1,"3":3}`), so it carries an endpoint label map
    + `endpoint_action_suffix` and surfaces `action_1` / `action_3`. The
    AwoX-specific `raw`-frame magic-byte sub-actions (`awox_color_ctrl`
    colours, `awox_level_ctrl` refresh, `awox_scenes_raw`) have no generic
    decoder and are deferred, mirroring the legrand remote precedent. New
    `tests/test_awox_parity.cpp` pins the restored PIR occupancy decode,
    the per-endpoint action suffix on the wall remote (incl. level + colour
    command families), and the command-action stream on the 33952 remote
    (incl. genScenes `recall_<n>`).

- **Vesternet REM-013 / WAL-011 / WAL-012 multi-button scene remotes — every
  button collapsed onto one `action`.** These battery remotes (12-, 4- and
  8-button; rebranded Sunricher `ZGRC-KEY-013` / `ZG2833K4_EU06` /
  `ZG2833K8_EU05`) are genOnOff/genLevelCtrl/genScenes clients that emit bare
  command actions (`"on"`, `"brightness_move_up"`, `"recall_<scene>"`). Because
  `action` is a `kAlwaysGlobalKey`, the dispatcher never suffixed it, so a press
  on endpoint 1 and a press on endpoint 4 both surfaced as the same
  `action: "on"` — the originating button was discarded and an N-button remote
  was indistinguishable from a 1-button one. z2m distinguishes them per
  endpoint (`on_1..on_4`, …, `recall_1_1..recall_2_4`). Graduated all three defs
  from `generated/` to Tier-2 parents, added the per-endpoint label map
  (`{1,2,3,4}` / `{1,2}`) plus the EP2-4 binds, and set `endpoint_action_suffix`
  so the dispatcher rewrites the key to `action_<n>` per endpoint (the recall
  scene rides in the value, so endpoint *and* scene both survive). Identical fix
  to the robb/sunricher rebrand of the same hardware (`Rob_ROB_200_007_0` /
  `Rob_ROB_200_008_0`). The single-endpoint 2-button WAL-006 is unaffected (bare
  `action`, no map — matching the robb `ROB_200-009-0` precedent). New
  `tests/test_vesternet_parity.cpp` pins `action_<n>` per endpoint (incl.
  genScenes recall) for the three remotes, plus regression guards that WAL-006
  stays single-EP, SWI-015 still demuxes `state_l1`/`state_l2`, and PIR-21 keeps
  its IAS occupancy wiring.

- **SMaBiT (Bitron Video) AV2010/33 vibration sensor — vibration never fired
  (wrong IAS zoneStatus bit).** z2m wires this sensor to
  `fz.ias_occupancy_alarm_2`, which reads zoneStatus **bit 1** (alarm_2). The
  generated Tier-1 def lowered the generic `kFzIasVibrationAlarm`, whose label
  reads **bit 0** (alarm_1) — correct for the other four vibration sensors
  (develco WISZB-137, heiman HS1VS-EF/N, third_reality 3RVS01031Z, all on
  `fz.ias_vibration_alarm_1`), but wrong here: AV2010/33 signals vibration on
  alarm_2, so the event was watched on the wrong bit and never reached the
  shadow. Added a bit-1 variant `kFzIasVibrationAlarm2` (emitting the same
  semantic `vibration` key, mirroring the existing `kFzIasMotionAlarm2`
  precedent) in `definitions/_generic/_shared.{hpp,cpp}`, and graduated
  `Bit_AV2010_33.cpp` from `generated/` to a parent Tier-2 override pointing at
  it. The four bit-0 vibration sensors are unaffected. New
  `tests/test_bitron_parity.cpp` pins that AV2010/33 decodes `vibration` from
  bit 1 (and not bit 0) with shared tamper/battery_low, plus a regression that
  the bit-0 occupancy path (AV2010/22) is untouched.

- **Amina amina S EV Charger — AC metering channels dropped or unexposed.**
  The charger drives its AC metering through z2m's
  `m.electricityMeter({cluster: "electrical", acFrequency: true, threePhase:
  true})` plus a standalone `total_active_power` numeric (attribute
  `totalActivePower` 0x0304, kW). The hand-rewritten Tier-2 def leaned solely on
  the generic `kFzElectricalMeasurement` (activePower 0x050B / rmsVoltage 0x0505
  / rmsCurrent 0x0508 only) and only exposed `power`/`energy`, so `voltage` and
  `current` were decoded-but-unexposed and `ac_frequency`, `total_active_power`,
  and all six per-phase B/C channels (`power_phase_b/c`, `voltage_phase_b/c`,
  `current_phase_b/c`) were dropped on the floor. The device's PRIMARY
  instantaneous-power reading is `total_active_power` (0x0304) — z2m's
  `fzLocal.poll_energy` keys its energy poll off `msg.data.totalActivePower`,
  confirming the firmware reports there rather than via activePower 0x050B.
  Added `kFzAminaElectricalMeasurementExtras` (new
  `definitions/amina/_shared.{hpp,cpp}`) decoding ac_frequency 0x0300,
  totalActivePower 0x0304, and the per-phase B/C power/voltage/current attrs,
  wired alongside the generic `kFzElectricalMeasurement`; added the missing
  exposes. Graduated `Ami_amina_S.cpp` from `generated/` to a parent Tier-2
  override. `power_factor` stays unexposed (z2m default `powerFactor: false`,
  not overridden). The EV-specific state/alarms/energy/offline-config channels
  on the 0xFEE7 manuSpecific cluster were already decoded by the per-device
  converters; new `tests/test_amina_parity.cpp` pins the restored standard
  channels plus a regression on the manuSpecific decoders.

- **DIYRuZ parity pass — `soil_moisture`, `co2` and `contact` were exposed but
  silently dropped on three sensors.** All three headline channels were declared
  as exposes by the generated Tier-1 ports but had no decoder wired, because the
  matching converter did not exist when the ports were generated. (1) **Flower**
  (soil/temp/humidity/pressure/illuminance/battery) exposed `soil_moisture` from
  z2m `fz.soil_moisture` (msSoilMoisture 0x0408 measuredValue / 100) with no
  decoder — wired generic `kFzSoilMoisture` and added the 0x0408 binding. (2)
  **AirSense** (CO2/temp/humidity/pressure) exposed `co2` from z2m `fz.co2`
  (msCO2 0x040D measuredValue × 1e6 → ppm) with no decoder — wired generic
  `kFzCO2`. (3) **magnet** exposed `contact` from z2m `fz.diyruz_contact`
  (genOnOff `onOff` 0x0000 != 0 — *not* IAS) with no decoder; the generic
  `kFzOnOff` decodes the same attribute but emits `state`, so added a vendor
  `kFzDiyruzContact` (genOnOff onOff → bool `contact`) in new
  `definitions/diyruz/_shared.{hpp,cpp}`. Graduated all three defs from
  `generated/` to parent Tier-2 overrides. New fixture
  `tests/test_diyruz_parity.cpp` decodes real attribute-report wire shapes
  through the dispatcher and pins each restored channel (Flower's multi-endpoint
  `*_bme`/`*_ds` suffixing included) plus per-channel exclusivity. The other 9
  DIYRuZ defs were verified: RT (state + temperature), R4_5 / R8_8 (multi-relay
  on/off with endpoint maps) decode fully; FreePad / FreePad_LeTV_8 / KEYPAD20
  (`action` via genMultistateInput presentValue), rspm, Geiger and Zintercom
  carry only manufacturer-specific / multistate channels with no generic
  converter and stay as-is.

- **Nous (Tuya-OEM) parity pass — E6 and SZ-T04 LCD temp/humidity sensors were
  dead on every primary channel.** Both are TS0601 Tuya-DP devices (z2m serves
  them via `legacy.fz.nous_lcd_temperature_humidity_sensor` on manuSpecificTuya
  0xEF00), but the generated defs wired a genPowerCfg-only battery converter
  (`kFzBattery`) — so temperature, humidity *and* battery never decoded (these
  devices send nothing on genPowerCfg/msTemperature). Graduated both from
  `generated/` to parent Tier-2 overrides and wired `kFzTuyaDatapoints` with the
  z2m DP map (temperature DP1 /10, humidity DP2, battery DP4, min/max temp
  DP10/11 /10, min/max humidity DP12/13, temp/humidity alarm DP14/15, unit DP9;
  SZ-T04 adds report intervals DP17/18 and sensitivity DP19 /10 / DP20). Added
  the inverse `tz_tuya_datapoints` write path so the STATE_SET config keys
  round-trip. New fixture `tests/test_nous_parity.cpp` decodes real 0xEF00 DP
  frames through the dispatcher for both. The other 10 nous defs were verified
  correct (E5 standard-cluster T/H; A11Z metering plug; the four `_TZE*` Tuya-DP
  defs full; E9/E12/E13/D5Z model-only stubs are shadowed by their DP twins).

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