# MOES devices — ZHAC support status

Source of truth: zigbee-herdsman-converters `devices/moes.ts` (snapshot date 2026-04-27, 2 406 LOC, 41 device records / 90 fingerprints).  
ZHC port library: `embedded/zhc/definitions/moes/generated/` (160 generated `.cpp` files).

This document is a per-device parity map between z2m's MOES coverage and the ZHC ports compiled into the firmware. It complements `docs/parity/MOES_PARITY.md` (which is the audit + landing record) by giving a flat per-device view.

## Status legend

- ✅ **FULL** — every z2m fz/tz/configure has a ZHC equivalent wired and the expose set matches. *Does NOT mean verified on real hardware.*
- 🟡 **PARTIAL** — some functionality works (typically read-only or missing one expose family). Specific gap is documented.
- ❌ **MIS-CLASSIFIED** — port exists but advertises wrong functionality (wrong fz/tz family) for the z2m device.
- 🚧 **BLOCKED** — port intentionally minimal (battery-only stub) pending runtime additions.
- ⛔ **MISSING** — z2m has the device but ZHC has no port at all.

## Summary

- z2m records: **41**
- z2m fingerprints: **90**
- ZHC ports: **160**
- Status counts: ✅ 30 · 🟡 9 · ❌ 1 · 🚧 1

Phase 1 + Phase 2 parity work landed 2026-04-27 (see `docs/parity/MOES_PARITY.md` for the per-port edit log).

## Devices

<!-- ============ FULL (30) ============ -->

### `ADCBZI01` — Curtain Robot
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZ3210_sxtfesc6`  
**ZHC port files (3):** `Gen__TZ3210_sxtfesc6.cpp`, `Moe_ADCBZI01.cpp`, `Moes__TZ3210_sxtfesc6.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `fz.cover_position_tilt`, `tuya.fz.datapoints` | `::zhc::generic::kFzCoverPosition`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZ3210_sxtfesc6` |
| toZigbee | `tz.cover_position_tilt`, `tz.cover_state`, `tuya.tz.datapoints` | `::zhc::generic::kTzCoverPosition`, `kTzDp__TZ3210_sxtfesc6` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `battery`, `charging_status`, `close_threshold`, `cover_position`, `curtain_status`, `custom_week_prog_1`, `custom_week_prog_2`, `custom_week_prog_3`, `custom_week_prog_4`, `enum`, `factory_test`, `fault`, *…(+9 more)* | `action`, `battery`, `close_threshold`, `curtain_status`, `custom_week_prog_1`, `custom_week_prog_2`, `custom_week_prog_3`, `custom_week_prog_4`, `factory_test`, `illuminance`, `open_threshold`, `position`, *…(+3 more)* |

**Notes:** DP engine + tz + configure all present.

### `ERS-10TZBVB-AA` — Smart button
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (6):** `_TZ3000_ja5osu5g`, `_TZ3000_kjfzuycl`, `_TZ3000_egvb1p2g`, `_TZ3000_kaflzta4`, `_TZ3000_lrfvzq1e`, `ZG-101ZL` (zigbeeModel)  
**ZHC port files (7):** `Moe_ERS_10TZBVB_AA.cpp`, `Moes_ZG_101ZL.cpp`, `Moes__TZ3000_egvb1p2g.cpp`, `Moes__TZ3000_ja5osu5g.cpp`, `Moes__TZ3000_kaflzta4.cpp`, `Moes__TZ3000_kjfzuycl.cpp`, `Moes__TZ3000_lrfvzq1e.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `fz.command_step`, `fz.command_on`, `fz.command_off`, `fz.command_move_to_color_temp`, `fz.command_move_to_level`, `tuya.fz.multi_action`, `tuya.fz.operation_mode`, `fz.battery` | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzCommandMoveToColorTemp`, `::zhc::generic::kFzCommandMoveToLevel`, `::zhc::generic::kFzCommandOff`, `::zhc::generic::kFzCommandOn`, `::zhc::generic::kFzCommandStep`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `::zhc::tuya::kFzTuyaMultiAction`, *…(+2 more)* |
| toZigbee | `tuya.tz.operation_mode` | `::zhc::tuya::kTzTuyaOperationMode` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `battery` | `action`, `battery`, `operation_mode`, `voltage` |
| extend | `tuya.clusters.addTuyaGenOnOffCluster` | — |

**Notes:** Phase B (2026-04-27): `kFzCommandStep`/`On`/`Off`/`MoveToColorTemp`/`MoveToLevel` + `kFzTuyaMultiAction` + `kFzTuyaOperationMode` + `kFzBattery`; tz: `kTzTuyaOperationMode`; `tuya_base_configure` wired.

### `FWJZCEH18A001` — Roller blind motor 17mm/25mm/28mm
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE284_qoi1aqxg`  
**ZHC port files (3):** `Gen__TZE284_qoi1aqxg.cpp`, `Moe_FWJZCEH18A001.cpp`, `Moes__TZE284_qoi1aqxg.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzCoverPosition`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE284_qoi1aqxg` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzCoverPosition`, `kTzDp__TZE284_qoi1aqxg` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `battery`, `border`, `cover_position`, `enum`, `motor_direction` | `action`, `border`, `motor_direction`, `position`, `state` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `GM25TEQ-TYZ-2/25` — Roller Shade Blinds Motor for 38mm Tube
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE204_xtrnjaoz`, `_TZE200_xtrnjaoz`, `_TZE284_8whfphjv`  
**ZHC port files (7):** `Gen__TZE200_xtrnjaoz.cpp`, `Gen__TZE204_xtrnjaoz.cpp`, `Gen__TZE284_8whfphjv.cpp`, `Moe_GM25TEQ_TYZ_2_25.cpp`, `Moes__TZE200_xtrnjaoz.cpp`, `Moes__TZE204_xtrnjaoz.cpp`, `Moes__TZE284_8whfphjv.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzCoverPosition`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_xtrnjaoz`, `kFzDp__TZE204_xtrnjaoz`, `kFzDp__TZE284_8whfphjv` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzCoverPosition`, `kTzDp__TZE200_xtrnjaoz`, `kTzDp__TZE204_xtrnjaoz`, `kTzDp__TZE284_8whfphjv` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `cover_position`, `enum`, `motor_direction` | `action`, `motor_direction`, `position`, `state` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `MS-104BZ` — Smart light switch module (2 gang)
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (2):** `_TZ3000_pmz6mjyu`, `_TZ3000_iv6ph5tr`  
**ZHC port files (3):** `Moe_MS_104BZ.cpp`, `Moes__TZ3000_iv6ph5tr.cpp`, `Moes__TZ3000_pmz6mjyu.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `battery`, `state`, `voltage` |
| extend | `tuya.modernExtend.tuyaOnOff` | — |

**Notes:** Phase C (2026-04-27): `tuya_base_configure` attached (was nullptr).

### `MS-104Z` — Smart light switch module (1 gang)
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TYZB01_iuepbmpv`, `_TZ3000_zmy1waw6`, `_TZ3000_bkfe0bab`  
**ZHC port files (3):** `Moe_MS_104Z.cpp`, `Moes__TZ3000_bkfe0bab.cpp`, `Moes__TZ3000_zmy1waw6.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `battery`, `state`, `voltage` |
| extend | `tuya.modernExtend.tuyaOnOff` | — |

**Notes:** Phase C (2026-04-27): `tuya_base_configure` attached (was nullptr).

### `SFL02-Z-1` — Star feather smart switch 1 gang
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE200_stvgmdjz`, `_TZE200_ydkqbmpt`, `_TZE200_z3u99qxt`  
**ZHC port files (7):** `Gen__TZE200_stvgmdjz.cpp`, `Gen__TZE200_ydkqbmpt.cpp`, `Gen__TZE200_z3u99qxt.cpp`, `Moe_SFL02_Z_1.cpp`, `Moes__TZE200_stvgmdjz.cpp`, `Moes__TZE200_ydkqbmpt.cpp`, `Moes__TZE200_z3u99qxt.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_stvgmdjz`, `kFzDp__TZE200_ydkqbmpt`, `kFzDp__TZE200_z3u99qxt` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `kTzDp__TZE200_stvgmdjz`, `kTzDp__TZE200_ydkqbmpt`, `kTzDp__TZE200_z3u99qxt` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `power_on_behavior`, `switch` | `action`, `backlight_mode`, `countdown_l1`, `indicator_status`, `induction_mode`, `mode_l1`, `momentary_1`, `power_on_behavior`, `state`, `state_l1` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `SFL02-Z-2` — Star feather smart switch 2 gangs
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE200_uenof8jd`, `_TZE200_tzyy0rtq`, `_TZE200_hktk6hze`  
**ZHC port files (7):** `Gen__TZE200_hktk6hze.cpp`, `Gen__TZE200_tzyy0rtq.cpp`, `Gen__TZE200_uenof8jd.cpp`, `Moe_SFL02_Z_2.cpp`, `Moes__TZE200_hktk6hze.cpp`, `Moes__TZE200_tzyy0rtq.cpp`, `Moes__TZE200_uenof8jd.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_hktk6hze`, `kFzDp__TZE200_tzyy0rtq`, `kFzDp__TZE200_uenof8jd` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `kTzDp__TZE200_hktk6hze`, `kTzDp__TZE200_tzyy0rtq`, `kTzDp__TZE200_uenof8jd` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `power_on_behavior`, `switch` | `action`, `backlight_mode`, `countdown_l1`, `countdown_l2`, `indicator_status`, `induction_mode`, `mode_l1`, `mode_l2`, `momentary_1`, `momentary_2`, `power_on_behavior`, `state`, *…(+2 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `SFL02-Z-3` — Star feather smart switch 3 gangs
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE200_rd8cdssd`, `_TZE200_wv9ukqca`, `_TZE200_zo0cfekv`  
**ZHC port files (7):** `Gen__TZE200_rd8cdssd.cpp`, `Gen__TZE200_wv9ukqca.cpp`, `Gen__TZE200_zo0cfekv.cpp`, `Moe_SFL02_Z_3.cpp`, `Moes__TZE200_rd8cdssd.cpp`, `Moes__TZE200_wv9ukqca.cpp`, `Moes__TZE200_zo0cfekv.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_rd8cdssd`, `kFzDp__TZE200_wv9ukqca`, `kFzDp__TZE200_zo0cfekv` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `kTzDp__TZE200_rd8cdssd`, `kTzDp__TZE200_wv9ukqca`, `kTzDp__TZE200_zo0cfekv` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `power_on_behavior`, `switch` | `action`, `backlight_mode`, `countdown_l1`, `countdown_l2`, `countdown_l3`, `indicator_status`, `induction_mode`, `mode_l1`, `mode_l2`, `mode_l3`, `momentary_1`, `momentary_2`, *…(+6 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `SFL02-Z-4` — Star feather smart switch 4 gangs
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE200_dq8bu0pt`, `_TZE200_hmabvy81`, `_TZE200_9dhenr94`  
**ZHC port files (7):** `Gen__TZE200_9dhenr94.cpp`, `Gen__TZE200_dq8bu0pt.cpp`, `Gen__TZE200_hmabvy81.cpp`, `Moe_SFL02_Z_4.cpp`, `Moes__TZE200_9dhenr94.cpp`, `Moes__TZE200_dq8bu0pt.cpp`, `Moes__TZE200_hmabvy81.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_9dhenr94`, `kFzDp__TZE200_dq8bu0pt`, `kFzDp__TZE200_hmabvy81` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `kTzDp__TZE200_9dhenr94`, `kTzDp__TZE200_dq8bu0pt`, `kTzDp__TZE200_hmabvy81` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `power_on_behavior`, `switch` | `action`, `backlight_mode`, `countdown_l1`, `countdown_l2`, `countdown_l3`, `countdown_l4`, `indicator_status`, `induction_mode`, `mode_l1`, `mode_l2`, `mode_l3`, `mode_l4`, *…(+10 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `SH4-ZB` — Thermostatic radiator valve
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE200_fhn3negr`  
**ZHC port files (3):** `Gen__TZE200_fhn3negr.cpp`, `Moe_SH4_ZB.cpp`, `Moes__TZE200_fhn3negr.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_fhn3negr` |
| toZigbee | *(via extend)* | `kTzDp__TZE200_fhn3negr` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `auto_setpoint_override`, `battery`, `binary`, `boost_heating`, `boost_heating_countdown`, `child_lock`, `comfort_temperature`, `eco_temperature`, `numeric`, `online`, `open_window_temperature`, `window_detection_bool`, *…(+1 more)* | `action`, `auto_setpoint_override`, `battery`, `boost_heating`, `boost_heating_countdown`, `child_lock`, `comfort_temperature`, `current_heating_setpoint`, `eco_temperature`, `error_status`, `local_temperature`, `local_temperature_calibration`, *…(+7 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZC-HM` — Carbon monoxide alarm
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE200_hr0tdd47`, `_TZE200_rjxqso4a`, `_TZE284_rjxqso4a`  
**ZHC port files (7):** `Gen__TZE200_hr0tdd47.cpp`, `Gen__TZE200_rjxqso4a.cpp`, `Gen__TZE284_rjxqso4a.cpp`, `Moe_ZC_HM.cpp`, `Moes__TZE200_hr0tdd47.cpp`, `Moes__TZE200_rjxqso4a.cpp`, `Moes__TZE284_rjxqso4a.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzIasZone`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_hr0tdd47`, `kFzDp__TZE200_rjxqso4a`, `kFzDp__TZE284_rjxqso4a` |
| toZigbee | *(via extend)* | `kTzDp__TZE200_hr0tdd47`, `kTzDp__TZE200_rjxqso4a`, `kTzDp__TZE284_rjxqso4a` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `battery`, `carbon_monoxide`, `co` | `action`, `alarm`, `battery`, `battery_low`, `carbon_monoxide`, `co`, `self_test_result`, `silence`, `state`, `tamper` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZC-LP01` — Smart sliding window pusher
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZ3210_5rta89nj`  
**ZHC port files (3):** `Gen__TZ3210_5rta89nj.cpp`, `Moe_ZC_LP01.cpp`, `Moes__TZ3210_5rta89nj.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`, `tuya.modernExtend.dpBattery`)* | `::zhc::generic::kFzCoverPosition`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZ3210_5rta89nj` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzCoverPosition`, `kTzDp__TZ3210_5rta89nj` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `binary`, `button_position`, `charging`, `cover_position`, `enum` | `action`, `charging`, `position`, `state` |
| extend | `tuya.modernExtend.tuyaBase`, `tuya.modernExtend.dpBattery` | — |

**Notes:** DP engine + tz + configure all present.

### `ZHT-PT01-M-MS` — Smart thermostat for pilot wire heating radiator
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE204_x9usygq1`  
**ZHC port files (3):** `Gen__TZE204_x9usygq1.cpp`, `Moe_ZHT_PT01_M_MS.cpp`, `Moes__TZE204_x9usygq1.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE204_x9usygq1` |
| toZigbee | *(via extend)* | `kTzDp__TZE204_x9usygq1` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `binary`, `boost_duration`, `child_lock`, `elec_statistics_day`, `elec_statistics_month`, `elec_statistics_year`, `electricity_statistics`, `enum`, `fault`, `fault_alarm`, `lower_temp`, `numeric`, *…(+4 more)* | `action`, `air_pressure_index`, `app_features`, `average_power`, `battery`, `boost_duration`, `child_lock`, `current_heating_setpoint`, `elec_statistics_day`, `elec_statistics_month`, `elec_statistics_year`, `electricity_statistics`, *…(+26 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZHT-S01` — Zigbee wall thermostat (air/internal temperature priority)
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE284_rlytpmij`  
**ZHC port files (3):** `Gen__TZE284_rlytpmij.cpp`, `Moe_ZHT_S01.cpp`, `Moes__TZE284_rlytpmij.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE284_rlytpmij` |
| toZigbee | *(via extend)* | `kTzDp__TZE284_rlytpmij` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(dynamic function)* | `action`, `backlight_brightness`, `battery`, `child_lock`, `current_heating_setpoint`, `deadzone_temperature`, `eco_temperature`, `factory_reset`, `floor_temperature`, `local_temperature`, `local_temperature_calibration`, `max_temperature_limit`, *…(+9 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** z2m exposes built dynamically; ZHC port has DP engine + tz + configure.

### `ZHT-S03` — Zigbee wall thermostat
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE204_zxkwaztm`  
**ZHC port files (3):** `Gen__TZE204_zxkwaztm.cpp`, `Moe_ZHT_S03.cpp`, `Moes__TZE204_zxkwaztm.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE204_zxkwaztm` |
| toZigbee | *(via extend)* | `kTzDp__TZE204_zxkwaztm` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `binary`, `child_lock`, `enum`, `frost_protection`, `schedule_holiday`, `schedule_weekday`, `text`, `working_day` | `action`, `battery`, `child_lock`, `current_heating_setpoint`, `frost_protection`, `local_temperature`, `local_temperature_calibration`, `preset`, `running_state`, `schedule_holiday`, `schedule_weekday`, `state`, *…(+3 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZK-EU-2U` — Zigbee 3.0 dual USB wireless socket plug
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `TS0112` (zigbeeModel)  
**ZHC port files (2):** `Moe_ZK_EU_2U.cpp`, `Moes_TS0112.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `m.onOff`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `::zhc::tuya::kFzTuyaOnOffAction` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff` |
| configure | *(none)* | *(nullptr)* |
| exposes | *(via extend: `m.onOff`)* | `action`, `state` |
| extend | `m.onOff` | — |

**Notes:** z2m uses `extend: [m.onOff()]`; ZHC parent port `Moe_ZK_EU_2U.cpp` uses `kFzOnOff`+`kTzOnOff`; the additional `Moes_TS0112.cpp` sibling is from a stale generation pass (it advertises Tuya scene-action converters which do not match z2m — harmless because the parent definition shadows it for the runtime fingerprint match).

### `ZP-LZ-FR2U` — Zigbee 3.0 dual USB wireless socket plug
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZ3000_cymsnfvf`, `_TZ3000_2xlvlnez`, `_TZ3210_2uk4z8ce`  
**ZHC port files (4):** `Moe_ZP_LZ_FR2U.cpp`, `Moes__TZ3000_2xlvlnez.cpp`, `Moes__TZ3000_cymsnfvf.cpp`, `Moes__TZ3210_2uk4z8ce.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaIndicatorMode`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `::zhc::tuya::kFzTuyaPowerOnBehavior` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `::zhc::tuya::kTzTuyaChildLock`, `::zhc::tuya::kTzTuyaIndicatorMode`, `::zhc::tuya::kTzTuyaPowerOnBehavior` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `child_lock`, `indicator_mode`, `power_on_behavior`, `state` |
| extend | `tuya.modernExtend.tuyaOnOff` | — |

**Notes:** Phase C (2026-04-27): `power_on_behavior` / `indicator_mode` / `child_lock` fz+tz + `tuya_base_configure` attached.

### `ZS-D1` — Intelligent dimming switch - 1 gang
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (2):** `_TZE284_a1ovdobn`, `_TZE200_a1ovdobn`  
**ZHC port files (5):** `Gen__TZE200_a1ovdobn.cpp`, `Gen__TZE284_a1ovdobn.cpp`, `Moe_ZS_D1.cpp`, `Moes__TZE200_a1ovdobn.cpp`, `Moes__TZE284_a1ovdobn.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_a1ovdobn`, `kFzDp__TZE284_a1ovdobn` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `kTzDp__TZE200_a1ovdobn`, `kTzDp__TZE284_a1ovdobn` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(dynamic function)* | `action`, `brightness_1`, `brightness_max_1`, `brightness_min_1`, `countdown_1`, `light_mode`, `relay_status`, `state`, `state_l1`, `switch_backlight` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** z2m exposes built dynamically; ZHC port has DP engine + tz + configure.

### `ZS-D2` — Intelligent dimming switch - 2 gang
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE200_rlqamjhp`  
**ZHC port files (3):** `Gen__TZE200_rlqamjhp.cpp`, `Moe_ZS_D2.cpp`, `Moes__TZE200_rlqamjhp.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_rlqamjhp` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `kTzDp__TZE200_rlqamjhp` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(dynamic function)* | `action`, `brightness_1`, `brightness_2`, `brightness_max_1`, `brightness_max_2`, `brightness_min_1`, `brightness_min_2`, `countdown_1`, `countdown_2`, `light_mode`, `relay_status`, `state`, *…(+3 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** z2m exposes built dynamically; ZHC port has DP engine + tz + configure.

### `ZS-D3` — Intelligent dimming switch - 3 gang
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE284_vizxbhco`  
**ZHC port files (3):** `Gen__TZE284_vizxbhco.cpp`, `Moe_ZS_D3.cpp`, `Moes__TZE284_vizxbhco.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE284_vizxbhco` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff`, `kTzDp__TZE284_vizxbhco` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(dynamic function)* | `action`, `brightness_1`, `brightness_2`, `brightness_3`, `brightness_max_1`, `brightness_max_2`, `brightness_max_3`, `brightness_min_1`, `brightness_min_2`, `brightness_min_3`, `countdown_1`, `countdown_2`, *…(+8 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** z2m exposes built dynamically; ZHC port has DP engine + tz + configure.

### `ZS-EUB_1gang` — Wall light switch (1 gang)
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZ3000_hhiodade`  
**ZHC port files (2):** `Moe_ZS_EUB_1gang.cpp`, `Moes__TZ3000_hhiodade.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(via extend: `tuya.modernExtend.tuyaOnOff`)* | `battery`, `state`, `voltage` |
| extend | `tuya.modernExtend.tuyaOnOff` | — |

**Notes:** Phase C (2026-04-27): `tuya_base_configure` attached (was nullptr).

### `ZS-SF-EUC-WH-MS` — Star feather Zigbee curtain switch
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE284_upt8lzi0`  
**ZHC port files (3):** `Gen__TZE284_upt8lzi0.cpp`, `Moe_ZS_SF_EUC_WH_MS.cpp`, `Moes__TZE284_upt8lzi0.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzCoverPosition`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE284_upt8lzi0` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzCoverPosition`, `kTzDp__TZE284_upt8lzi0` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `cover_position` | `action`, `position`, `state` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZS-SR-EUC` — Star ring - smart curtain switch
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE204_srmahpwl`  
**ZHC port files (3):** `Gen__TZE204_srmahpwl.cpp`, `Moe_ZS_SR_EUC.cpp`, `Moes__TZE204_srmahpwl.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzCoverPosition`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE204_srmahpwl` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzCoverPosition`, `kTzDp__TZE204_srmahpwl` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `calibration`, `cover_position`, `enum`, `motor_steering` | `action`, `calibration`, `motor_steering`, `position`, `state` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZSS-HM-SSD01` — Smoke sensor
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE200_vawy74yh`, `_TZE204_vawy74yh`, `_TZE284_vawy74yh`  
**ZHC port files (5):** `Gen__TZE204_vawy74yh.cpp`, `Gen__TZE284_vawy74yh.cpp`, `Moe_ZSS_HM_SSD01.cpp`, `Moes__TZE204_vawy74yh.cpp`, `Moes__TZE284_vawy74yh.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzIasZone`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE204_vawy74yh`, `kFzDp__TZE284_vawy74yh` |
| toZigbee | *(via extend)* | `kTzDp__TZE204_vawy74yh`, `kTzDp__TZE284_vawy74yh` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `battery`, `binary`, `enum`, `self_test`, `silence`, `smoke` | `action`, `alarm`, `battery`, `battery_low`, `battery_state`, `silence`, `smoke`, `state`, `tamper` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZT-B-EU1` — Scene remote with 1 key
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZ3000_axpdxqgu`  
**ZHC port files (2):** `Moe_ZT_B_EU1.cpp`, `Moes__TZ3000_axpdxqgu.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `tuya.fz.on_off_action`, `fz.battery`, `tuya.fz.datapoints` | `::zhc::generic::kFzBattery`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `::zhc::tuya::kFzTuyaOnOffAction`, `kFzDp_ZT_B_EU1`, `kFzDp__TZ3000_axpdxqgu` |
| toZigbee | *(via extend)* | `kTzDp_ZT_B_EU1`, `kTzDp__TZ3000_axpdxqgu` |
| configure | `tuya.configureMagicPacket` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `battery` | `action`, `battery`, `voltage` |
| extend | `tuya.clusters.addTuyaGenOnOffCluster` | — |

**Notes:** Phase A (2026-04-27): scene-remote DP map (DP1+optional DP2/DP3 → action enum) + `kFzTuyaOnOffAction` + `kFzBattery` + `tuya_base_configure` wired.

### `ZT-B-EU2` — Scene remote with 2 keys
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZ3000_5e235jpa`  
**ZHC port files (2):** `Moe_ZT_B_EU2.cpp`, `Moes__TZ3000_5e235jpa.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `tuya.fz.on_off_action`, `fz.battery`, `tuya.fz.datapoints` | `::zhc::generic::kFzBattery`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `::zhc::tuya::kFzTuyaOnOffAction`, `kFzDp_ZT_B_EU2`, `kFzDp__TZ3000_5e235jpa` |
| toZigbee | *(via extend)* | `kTzDp_ZT_B_EU2`, `kTzDp__TZ3000_5e235jpa` |
| configure | `tuya.configureMagicPacket` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `battery` | `action`, `battery`, `voltage` |
| extend | `tuya.clusters.addTuyaGenOnOffCluster` | — |

**Notes:** Phase A (2026-04-27): scene-remote DP map (DP1+optional DP2/DP3 → action enum) + `kFzTuyaOnOffAction` + `kFzBattery` + `tuya_base_configure` wired.

### `ZT-B-EU3` — Scene remote with 3 keys
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZ3000_gbm10jnj`  
**ZHC port files (2):** `Moe_ZT_B_EU3.cpp`, `Moes__TZ3000_gbm10jnj.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `tuya.fz.on_off_action`, `fz.battery`, `tuya.fz.datapoints` | `::zhc::generic::kFzBattery`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `::zhc::tuya::kFzTuyaOnOffAction`, `kFzDp_ZT_B_EU3`, `kFzDp__TZ3000_gbm10jnj` |
| toZigbee | *(via extend)* | `kTzDp_ZT_B_EU3`, `kTzDp__TZ3000_gbm10jnj` |
| configure | `tuya.configureMagicPacket` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `action`, `battery` | `action`, `battery`, `voltage` |
| extend | `tuya.clusters.addTuyaGenOnOffCluster` | — |

**Notes:** Phase A (2026-04-27): scene-remote DP map (DP1+optional DP2/DP3 → action enum) + `kFzTuyaOnOffAction` + `kFzBattery` + `tuya_base_configure` wired.

### `ZTRV-S01` — Zigbee temperature control valve
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE200_ivdc0kwl`  
**ZHC port files (3):** `Gen__TZE200_ivdc0kwl.cpp`, `Moe_ZTRV_S01.cpp`, `Moes__TZE200_ivdc0kwl.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp__TZE200_ivdc0kwl` |
| toZigbee | *(via extend)* | `kTzDp__TZE200_ivdc0kwl` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `battery`, `binary`, `child_lock`, `eco`, `eco_temperature`, `enum`, `frost_protection`, `max_temperature`, `min_temperature`, `position`, `screen_orientation`, `window`, *…(+1 more)* | `action`, `battery`, `current_heating_setpoint`, `eco`, `eco_temperature`, `holiday_temperature`, `local_temperature`, `local_temperature_calibration`, `max_temperature`, `min_temperature`, `position`, `preset`, *…(+11 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** DP engine + tz + configure all present.

### `ZWV-YC` — Water valve
**Status:** ✅ FULL  
**z2m vendor:** `Moes`  
**Fingerprints (4):** `_TZ3000_cjfmu5he`, `_TZ3000_mq4wujmp`, `_TZ3000_5af5r192`, `_TZ3000_ogjpfoyn`  
**ZHC port files (5):** `Moe_ZWV_YC.cpp`, `Moes__TZ3000_5af5r192.cpp`, `Moes__TZ3000_cjfmu5he.cpp`, `Moes__TZ3000_mq4wujmp.cpp`, `Moes__TZ3000_ogjpfoyn.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `m.battery`, `m.onOff`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff` |
| configure | *(none)* | *(nullptr)* |
| exposes | *(via extend: `m.battery`, `m.onOff`)* | `battery`, `state`, `voltage` |
| extend | `m.battery`, `m.onOff` | — |

**Notes:** z2m uses `extend: [m.battery(), m.onOff()]`; ZHC parent + 4 fp variants wire `kFzOnOff`+`kFzBattery`+`kTzOnOff` (sometimes plus `kFzTuyaMcuSyncTime`). configure step missing — but z2m has none either.

<!-- ============ PARTIAL (9) ============ -->

### `BHT-002` — Moes BHT series Thermostat
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (9):** `_TZE200_aoclfnxz`, `_TZE200_ztvwu4nk`, `_TZE204_5toc8efa`, `_TZE200_5toc8efa`, `_TZE200_ye5jkfsb`, `_TZE284_ye5jkfsb`, `_TZE204_aoclfnxz`, `_TZE200_u9bfwha0`, `_TZE204_u9bfwha0`  
**ZHC port files (10):** `Moe_BHT_002.cpp`, `Moes__TZE200_5toc8efa.cpp`, `Moes__TZE200_aoclfnxz.cpp`, `Moes__TZE200_u9bfwha0.cpp`, `Moes__TZE200_ye5jkfsb.cpp`, `Moes__TZE200_ztvwu4nk.cpp`, `Moes__TZE204_5toc8efa.cpp`, `Moes__TZE204_aoclfnxz.cpp`, `Moes__TZE204_u9bfwha0.cpp`, `Moes__TZE284_ye5jkfsb.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `legacy.fz.moes_thermostat` | `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp_BHT_002` |
| toZigbee | `legacy.tz.moes_thermostat_child_lock`, `legacy.tz.moes_thermostat_current_heating_setpoint`, `legacy.tz.moes_thermostat_mode`, `legacy.tz.moes_thermostat_standby`, `legacy.tz.moes_thermostat_sensor`, `legacy.tz.moes_thermostat_calibration`, `legacy.tz.moes_thermostat_deadzone_temperature`, `legacy.tz.moes_thermostat_max_temperature_limit`, *…(+2 more)* | `kTzDp_BHT_002` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(dynamic function)* | `child_lock`, `current_heating_setpoint`, `deadzone_temperature`, `local_temperature`, `local_temperature_calibration`, `max_temperature_limit`, `min_temperature_limit`, `preset`, `running_state`, `sensor`, `state` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** Phase D (2026-04-27): 12-DP map ported from `legacy.fz.moes_thermostat`. DP 101 packed-weekday schedule unsupported (would need raw decoder). `_TZE200_5toc8efa`/`_TZE204_5toc8efa` use /10 setpoint scaling (handled in fp-specific files). Verified vs legacy.ts source, NOT against real device.

### `BRT-100-TRV` — Thermostatic radiator valve
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (3):** `_TZE200_b6wax7g0`, `_TZE200_qsoecqlk`, `_TZE200_6y7kyjga`  
**ZHC port files (4):** `Moe_BRT_100_TRV.cpp`, `Moes__TZE200_6y7kyjga.cpp`, `Moes__TZE200_b6wax7g0.cpp`, `Moes__TZE200_qsoecqlk.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `fz.ignore_tuya_set_time`, `legacy.fz.moesS_thermostat` | `::zhc::tuya::kFzIgnoreTuyaSetTime`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp_BRT_100` |
| toZigbee | `legacy.tz.moesS_thermostat_current_heating_setpoint`, `legacy.tz.moesS_thermostat_child_lock`, `legacy.tz.moesS_thermostat_window_detection`, `legacy.tz.moesS_thermostat_temperature_calibration`, `legacy.tz.moesS_thermostat_boost_heating`, `legacy.tz.moesS_thermostat_boostHeatingCountdownTimeSet`, `legacy.tz.moesS_thermostat_eco_temperature`, `legacy.tz.moesS_thermostat_max_temperature`, *…(+5 more)* | `kTzDp_BRT_100` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `battery`, `binary`, `boost_heating_countdown`, `child_lock`, `eco_mode`, `eco_temperature`, `max_temperature`, `min_temperature`, `numeric`, `position`, `valve_state`, `window`, *…(+1 more)* | `battery`, `boost_heating`, `boost_heating_countdown`, `boost_heating_countdown_time_set`, `child_lock`, `current_heating_setpoint`, `eco_mode`, `eco_temperature`, `local_temperature`, `local_temperature_calibration`, `max_temperature`, `min_temperature`, *…(+5 more)* |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** Phase E (2026-04-27): 17-DP map ported from `legacy.fz.moesS_thermostat`. DP 101 packed schedule (12 items + program-mode flags) unsupported. fz chain mirrors z2m's `[fz.ignore_tuya_set_time, legacy.fz.moesS_thermostat]`.

### `SR-ZS` — Smart switch (light + sence)
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZ3002_vaq2bfcu`  
**ZHC port files (2):** `Moe_SR_ZS.cpp`, `Moes__TZ3002_vaq2bfcu.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaMagicPacket`, `m.deviceEndpoints`, `tuya.modernExtend.tuyaOnOff`, `m.actionEnumLookup`, `tuya.TuyaGenOnOff`, `tuya.modernExtend.tuyaLedIndicator`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff`, `::zhc::tuya::kFzTuyaMcuSyncTime`, `::zhc::tuya::kFzTuyaSwitchScene` |
| toZigbee | *(via extend)* | `::zhc::generic::kTzOnOff` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | *(via extend: `tuya.modernExtend.tuyaMagicPacket`, `m.deviceEndpoints`, `tuya.modernExtend.tuyaOnOff`, `m.actionEnumLookup`, `tuya.TuyaGenOnOff`, `tuya.modernExtend.tuyaLedIndicator`)* | `action`, `action_scene`, `battery`, `state`, `voltage` |
| extend | `tuya.modernExtend.tuyaMagicPacket`, `m.deviceEndpoints`, `tuya.modernExtend.tuyaOnOff`, `m.actionEnumLookup`, `tuya.TuyaGenOnOff`, `tuya.modernExtend.tuyaLedIndicator` | — |

**Notes:** Phase A (2026-04-27): 0xEF00 binding switched to `genOnOff`; `kFzTuyaSwitchScene` + `kFzOnOff` + `kTzOnOff` + `tuya_base_configure` wired. z2m's per-button `buttonLookup` labels (`1_up`/`1_down`/...) NOT decoded; multi-endpoint state mapping pending.

### `ZSS-ZK-THL` — Smart temperature and humidity meter with display
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (2):** `_TYZB01_kvwjujy9`, `_TYZB01_ftdkanlj`  
**ZHC port files (1):** `Moe_ZSS_ZK_THL.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `fz.battery`, `fz.humidity`, `fz.temperature` | `::zhc::generic::kFzBattery` |
| toZigbee | *(via extend)* | *(nullptr)* |
| configure | *(none)* | *(nullptr)* |
| exposes | `battery`, `humidity`, `temperature` | `battery`, `voltage` |
| extend | `m.illuminance` | — |

**Notes:** z2m uses `[fz.battery, fz.humidity, fz.temperature]` + `extend: [m.illuminance()]`. ZHC port `Moe_ZSS_ZK_THL.cpp` only wires `kFzBattery` — temperature/humidity/illuminance are MISSING. Read-only battery only.

### `ZTS-EU_1gang` — Wall touch light switch (1 gang)
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (2):** `_TZE200_amp6tsvy`, `_TZE200_tviaymwx`  
**ZHC port files (3):** `Moe_ZTS_EU_1gang.cpp`, `Moes__TZE200_amp6tsvy.cpp`, `Moes__TZE200_tviaymwx.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `legacy.fz.tuya_switch`, `legacy.fz.moes_switch` | `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp_ZTS_EU`, `kFzDp_ZTS_EU_1gang` |
| toZigbee | `legacy.tz.tuya_switch_state`, `legacy.tz.moes_switch` | `kTzDp_ZTS_EU`, `kTzDp_ZTS_EU_1gang` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `enum`, `indicate_light`, `switch` | `indicate_light`, `power_on_behavior`, `state` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** Phase F (2026-04-27): per-gang DP map (DP 1-4 state + DP 14 power_on_behavior + DP 15 indicate_light) from `legacy.fz.tuya_switch + legacy.fz.moes_switch`. Multi-endpoint state attrs (`state_lN`) emit but device tree doesn't auto-create per-endpoint kids.

### `ZTS-EU_2gang` — Wall touch light switch (2 gang)
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE200_g1ib5ldv`  
**ZHC port files (2):** `Moe_ZTS_EU_2gang.cpp`, `Moes__TZE200_g1ib5ldv.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `legacy.fz.tuya_switch`, `legacy.fz.moes_switch` | `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp_ZTS_EU`, `kFzDp_ZTS_EU_2gang` |
| toZigbee | `legacy.tz.tuya_switch_state`, `legacy.tz.moes_switch` | `kTzDp_ZTS_EU`, `kTzDp_ZTS_EU_2gang` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `enum`, `indicate_light`, `switch` | `indicate_light`, `power_on_behavior`, `state_l1`, `state_l2` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** Phase F (2026-04-27): per-gang DP map (DP 1-4 state + DP 14 power_on_behavior + DP 15 indicate_light) from `legacy.fz.tuya_switch + legacy.fz.moes_switch`. Multi-endpoint state attrs (`state_lN`) emit but device tree doesn't auto-create per-endpoint kids.

### `ZTS-EU_3gang` — Wall touch light switch (3 gang)
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE200_tz32mtza`  
**ZHC port files (2):** `Moe_ZTS_EU_3gang.cpp`, `Moes__TZE200_tz32mtza.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `legacy.fz.tuya_switch`, `legacy.fz.moes_switch` | `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp_ZTS_EU`, `kFzDp_ZTS_EU_3gang` |
| toZigbee | `legacy.tz.tuya_switch_state`, `legacy.tz.moes_switch` | `kTzDp_ZTS_EU`, `kTzDp_ZTS_EU_3gang` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `enum`, `indicate_light`, `switch` | `indicate_light`, `power_on_behavior`, `state_l1`, `state_l2`, `state_l3` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** Phase F (2026-04-27): per-gang DP map (DP 1-4 state + DP 14 power_on_behavior + DP 15 indicate_light) from `legacy.fz.tuya_switch + legacy.fz.moes_switch`. Multi-endpoint state attrs (`state_lN`) emit but device tree doesn't auto-create per-endpoint kids.

### `ZTS-EU_4gang` — Wall touch light switch (4 gang)
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE200_1ozguk6x`  
**ZHC port files (2):** `Moe_ZTS_EU_4gang.cpp`, `Moes__TZE200_1ozguk6x.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `legacy.fz.tuya_switch`, `legacy.fz.moes_switch` | `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp_ZTS_EU`, `kFzDp_ZTS_EU_4gang` |
| toZigbee | `legacy.tz.tuya_switch_state`, `legacy.tz.moes_switch` | `kTzDp_ZTS_EU`, `kTzDp_ZTS_EU_4gang` |
| configure | `INLINE_FN` | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `enum`, `indicate_light`, `switch` | `indicate_light`, `power_on_behavior`, `state_l1`, `state_l2`, `state_l3`, `state_l4` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** Phase F (2026-04-27): per-gang DP map (DP 1-4 state + DP 14 power_on_behavior + DP 15 indicate_light) from `legacy.fz.tuya_switch + legacy.fz.moes_switch`. Multi-endpoint state attrs (`state_lN`) emit but device tree doesn't auto-create per-endpoint kids.

### `ZTS-EUR-C` — Zigbee + RF curtain switch
**Status:** 🟡 PARTIAL  
**z2m vendor:** `Moes`  
**Fingerprints (4):** `_TZE200_nhyj64w2`, `_TZE200_127x7wnl`, `_TZE204_5slehgeo`, `_TZE284_5slehgeo`  
**ZHC port files (5):** `Moe_ZTS_EUR_C.cpp`, `Moes__TZE200_127x7wnl.cpp`, `Moes__TZE200_nhyj64w2.cpp`, `Moes__TZE204_5slehgeo.cpp`, `Moes__TZE284_5slehgeo.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `legacy.fz.moes_cover` | `::zhc::tuya::kFzTuyaMcuSyncTime`, `kFzDp_EURC` |
| toZigbee | `legacy.tz.moes_cover` | `kTzDp_EURC` |
| configure | *(none)* | `::zhc::tuya::extend::tuya_base_configure()` |
| exposes | `backlight`, `calibration`, `cover_position`, `enum`, `motor_reversal` | `backlight`, `calibration`, `motor_reversal`, `position`, `state` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** Phase G (2026-04-27): 5-DP map ported from `legacy.fz.moes_cover`. coverPositionInvert flag unused — 4 affected manus are NOT in z2m's invert list, default non-inverted is correct.

<!-- ============ MIS-CLASSIFIED (1) ============ -->

### `ZHT-SR` — Smart ring thermostat
**Status:** ❌ MIS-CLASSIFIED  
**z2m vendor:** `Moes`  
**Fingerprints (1):** `_TZE204_lpedvtvr`  
**ZHC port files (1):** `Moe_ZHT_SR.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | *(via extend: `tuya.modernExtend.tuyaBase`)* | `::zhc::generic::kFzBattery`, `::zhc::generic::kFzOnOff` |
| toZigbee | *(via extend)* | *(nullptr)* |
| configure | *(none)* | *(nullptr)* |
| exposes | `binary`, `child_lock`, `deadzone_temperature`, `max_temperature_limit`, `min_temperature_limit`, `rgb_light` | `battery`, `state`, `voltage` |
| extend | `tuya.modernExtend.tuyaBase` | — |

**Notes:** z2m uses `tuya.modernExtend.tuyaBase` with thermostat exposes (child_lock, max/min_temperature_limit, deadzone_temperature, rgb_light, etc.). ZHC port `Moe_ZHT_SR.cpp` is misclassified as battery+OnOff stub — exposes `state`/`battery`/`voltage` instead of any thermostat field. Needs full DP map similar to BHT-002.

<!-- ============ FULL — verified against z2m source, hardware-test pending (1) ============ -->

### `UFO-R11` — Universal smart IR remote control
**Status:** ✅ FULL (verified against z2m source — hardware test pending; see Caveats)
**z2m vendor:** `Moes`  
**Fingerprints (7):** `_TZ3290_j37rooaxrcdcqo5n`, `_TZ3290_ot6ewjvmejq5ekhl`, `_TZ3290_xjpbcxn92aaxvmlz`, `_TZ3290_gnl5a6a5xvql7c2a`, `_TZ3290_yyax9ajf`, `_TZ3290_nkpxapoz`, `_TZ3290_785fbxik`  
**ZHC port files (8):** `Moe_UFO_R11.cpp`, `Moes__TZ3290_785fbxik.cpp`, `Moes__TZ3290_gnl5a6a5xvql7c2a.cpp`, `Moes__TZ3290_j37rooaxrcdcqo5n.cpp`, `Moes__TZ3290_nkpxapoz.cpp`, `Moes__TZ3290_ot6ewjvmejq5ekhl.cpp`, `Moes__TZ3290_xjpbcxn92aaxvmlz.cpp`, `Moes__TZ3290_yyax9ajf.cpp`

| Aspect | z2m | ZHC |
|---|---|---|
| fromZigbee | `fzZosung.zosung_send_ir_code_00..05`, `fz.battery` | `::zhc::zosung::kFzZosungSendIrCode_00..05`, `::zhc::generic::kFzBattery` |
| toZigbee | `tzZosung.zosung_ir_code_to_send`, `tzZosung.zosung_learn_ir_code` | `::zhc::zosung::kTzZosungIrCodeToSend`, `::zhc::zosung::kTzZosungLearnIrCode` |
| configure | `INLINE_FN` (binds genPowerCfg + IRTransmit + IRControl) | declarative `bindings[]` (same three clusters; `_TZ3290_gnl5a6a5xvql7c2a` skips genPowerCfg per z2m) |
| exposes | `learn_ir_code` (binary set), `learned_ir_code` (text state), `ir_code_to_send` (text set), `battery`, `voltage` | identical (ExposeType::String introduced for the two text exposes) |
| extend | `zosung.zosungExtend.addZosungIRTransmitCluster`, `addZosungIRControlCluster` | clusters 0xED00 / 0xE004 added to `cluster_id_to_name`; runtime in `definitions/zosung/_shared.{hpp,cpp}` |

**Notes:** Zosung IR runtime landed 2026-04-27. Six fz handlers + two tz handlers cover the full chunked-file-transfer state machine. **Hardware testing required** — protocol is faithful to z2m's `lib/zosung.ts` reference but has not been exercised on a real UFO-R11 yet. See `docs/parity/MOES_PARITY.md#zosung-ir-runtime-2026-04-27` for the protocol diagram, limitations (single global session, no raw cluster-command body access for received-chunk content yet), and the hardware test checklist.

## Orphan ports (ZHC has no z2m equivalent)

*None — every ZHC port matches at least one z2m fingerprint or zigbeeModel.*

## Missing ports (z2m has, ZHC doesn't)

*None — every z2m model has at least one ZHC port file.*

---

*Generated 2026-04-27 from a parser pass over `moes.ts` and the 160 `.cpp` ports under `embedded/zhc/definitions/moes/generated/`. Status counts mirror Phase 1 + Phase 2 work landed; see `docs/parity/MOES_PARITY.md` for the per-port edit log.*