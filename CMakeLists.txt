# SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20)
project(zhc_core CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core library is currently header-only. Later tasks add sources under
# src/zcl/, src/runtime/, and definitions/ and switch this to STATIC.
add_library(zhc_core INTERFACE)
target_include_directories(zhc_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Warnings-as-errors enforced from day one. Easier to keep clean than
# to retrofit once hundreds of definitions have landed. Wrapped in
# BUILD_INTERFACE so downstream consumers (firmware, host tooling) are
# not forced to inherit -Werror — only this tree's own build trips on
# warnings.
target_compile_options(zhc_core INTERFACE
    $<BUILD_INTERFACE:$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic -Werror>>
    $<BUILD_INTERFACE:$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>>
)

add_library(zhc_parser STATIC
    src/zcl/header.cpp
    src/zcl/foundation.cpp
    src/zcl/decoder.cpp
    src/zcl/tuya_dp.cpp
)
target_link_libraries(zhc_parser PUBLIC zhc_core)

add_library(zhc_runtime STATIC
    src/runtime/dispatch.cpp
    src/runtime/definition_runtime.cpp
)
target_link_libraries(zhc_runtime PUBLIC zhc_core)

add_library(zhc_definitions OBJECT
    definitions/_generic/_shared.cpp
    definitions/lumi/_shared.cpp
    definitions/lumi/registry.cpp
    definitions/tuya/_shared.cpp
    definitions/tuya/registry.cpp
    definitions/tuya/TZE200_bjawzodf.cpp
    definitions/tuya/TZE200_0nmiizme.cpp
    definitions/tuya/TZE200_cjbofhxw.cpp
    definitions/tuya/TZE200_temp.cpp
    definitions/tuya/TZE200_smoke.cpp
    definitions/tuya/TZE200_co2.cpp
    definitions/tuya/TZE200_motion.cpp
    definitions/tuya/TZE200_alarm.cpp
    definitions/tuya/TZE200_plug_metering.cpp
    definitions/tuya/TZE200_switch_3gang.cpp
    definitions/tuya/TZE200_trv.cpp
    definitions/tuya/TZE200_radar.cpp
    definitions/tuya/TS0001.cpp
    definitions/tuya/gq8b1uv.cpp
    definitions/tuya/U86KWF_ZPSJ.cpp
    definitions/tuya/U86KCJ_ZP.cpp
    definitions/tuya/TT001ZAV20.cpp
    definitions/tuya/SNTZ009.cpp
    definitions/tuya/SNTZ007.cpp
    definitions/tuya/SM0212.cpp
    definitions/tuya/SM0202.cpp
    definitions/tuya/SM0001.cpp
    definitions/tuya/RH3040.cpp
    definitions/tuya/EPJ_ZB.cpp
    definitions/tuya/E220_KR4N0Z0_HA.cpp
    definitions/tuya/D3_DPWK_TY.cpp
    definitions/tuya/TS0002.cpp
    definitions/tuya/TS0011.cpp
    definitions/tuya/TS011F.cpp
    definitions/tuya/TS0501B.cpp
    definitions/tuya/TS110E.cpp
    definitions/tuya/TZE200_vzekyi4c.cpp
    definitions/tuya/TZE200_co_alarm.cpp
    definitions/tuya/TZE200_illuminance.cpp
    definitions/tuya/TZE200_formaldehyde.cpp
    definitions/tuya/TZE200_voc.cpp
    definitions/tuya/TZE200_smoke2.cpp
    definitions/tuya/TZE200_vibration.cpp
    definitions/tuya/TZE200_airquality.cpp
    definitions/tuya/TZE200_pm25.cpp
    definitions/tuya/TZE200_switch_countdown.cpp
    definitions/tuya/TZE200_switch_2gang.cpp
    definitions/tuya/TZE200_switch_4gang.cpp
    definitions/tuya/TZE204_plug.cpp
    definitions/tuya/TZE200_scene_4btn.cpp
    definitions/tuya/TZE200_thermostat.cpp
    definitions/tuya/TZE200_trv_v2.cpp
    definitions/tuya/TZE200_fancoil.cpp
    definitions/tuya/TZE200_radar_v2.cpp
    definitions/tuya/TZE204_radar.cpp
    definitions/tuya/TS0043.cpp
    definitions/tuya/TS0044.cpp
    definitions/tuya/TS004F.cpp
    definitions/tuya/TS004F_knob.cpp
    definitions/tuya/TS0202.cpp
    definitions/tuya/TS0205.cpp
    definitions/tuya/TZE200_pir_illum.cpp
    definitions/tuya/TZE200_th_slim.cpp
    definitions/tuya/TZE200_presence_zone.cpp
    definitions/tuya/TZE200_door2.cpp
    definitions/tuya/TZE200_gas.cpp
    definitions/tuya/TZE200_volume_alarm.cpp
    definitions/tuya/TZE200_smoke3.cpp
    definitions/tuya/TZE200_window.cpp
    definitions/tuya/TZE200_flood.cpp
    definitions/tuya/TZE200_switch_2g_cd.cpp
    definitions/tuya/TZE200_dimmer.cpp
    definitions/tuya/TZE200_relay_power.cpp
    definitions/tuya/TZE200_outdoor_plug.cpp
    definitions/tuya/TZE200_garage.cpp
    definitions/tuya/TS0003.cpp
    definitions/tuya/TS0004.cpp
    definitions/tuya/TS0012.cpp
    definitions/tuya/TS0013.cpp
    definitions/tuya/TS0121.cpp
    definitions/tuya/TS130F.cpp
    definitions/tuya/TZE200_cover.cpp
    definitions/tuya/TS0222.cpp
    definitions/tuya/TS0211.cpp
    definitions/tuya/TS0210.cpp
    definitions/tuya/TZE200_soil.cpp
    definitions/tuya/TZE200_rain.cpp
    definitions/tuya/TZE200_pressure.cpp
    definitions/tuya/TZE200_air_pressure.cpp
    definitions/tuya/TZE200_uv.cpp
    definitions/tuya/TZE200_battery_mon.cpp
    definitions/tuya/TZE200_accel.cpp
    definitions/tuya/TZE200_flow.cpp
    definitions/tuya/TZE200_switch_6gang.cpp
    definitions/tuya/TZE200_multi_outlet.cpp
    definitions/tuya/TZE200_pir_switch.cpp
    definitions/tuya/TZE200_ir_blaster.cpp
    definitions/tuya/TZE200_touch_panel.cpp
    definitions/tuya/TZE200_underfloor.cpp
    definitions/tuya/TZE200_floor_thermo.cpp
    definitions/tuya/TZE200_pool_thermo.cpp
    definitions/tuya/TZE200_hvac.cpp
    definitions/tuya/TZE200_ceiling_radar.cpp
    definitions/tuya/TZE204_hires_radar.cpp
    definitions/tuya/TS0042.cpp
    definitions/tuya/TS0215A.cpp
    definitions/tuya/TZE200_rgbww.cpp
    definitions/tuya/TS0041.cpp
    definitions/tuya/TZE200_pet_feeder.cpp
    definitions/tuya/TZE200_fall.cpp
    definitions/tuya/TZE200_sleep.cpp
    definitions/tuya/TZE200_motion_lux.cpp
    definitions/tuya/TZE200_contact_t1.cpp
    definitions/tuya/TZE200_light_sensor.cpp
    definitions/tuya/TZE200_radar_v5.cpp
    definitions/tuya/TZE200_noise.cpp
    definitions/tuya/TZE200_presence_th.cpp
    definitions/tuya/TZE200_relay_1g.cpp
    definitions/tuya/TZE200_outdoor_switch.cpp
    definitions/tuya/TZE200_scenario.cpp
    definitions/tuya/TZE200_timer_switch.cpp
    definitions/tuya/TZE200_floor_ctrl.cpp
    definitions/tuya/TZE200_trv_bt.cpp
    definitions/tuya/TZE200_thermostat_v3.cpp
    definitions/tuya/TZE200_ac_remote.cpp
    definitions/tuya/TZE200_fan_speed.cpp
    definitions/tuya/TZE200_roller_shade.cpp
    definitions/tuya/TZE200_tilt_blinds.cpp
    definitions/tuya/TZE200_motor_curtain.cpp
    definitions/tuya/TZE200_dual_curtain.cpp
    definitions/tuya/TZE200_siren.cpp
    definitions/tuya/TZE200_lock.cpp
    definitions/tuya/TS0203.cpp
    definitions/tuya/TZE200_clamp.cpp
    definitions/tuya/TZE200_soil_th.cpp
    definitions/tuya/TZE200_greenhouse.cpp
    definitions/tuya/TS0601_air_house_keeper.cpp
    definitions/tuya/TS0601_air_quality.cpp
    definitions/tuya/TZE200_mja3fuja.cpp
    definitions/tuya/TS0601_smoke_3.cpp
    definitions/tuya/ZG_225Z.cpp
    definitions/tuya/TS0601_switch_1g.cpp
    definitions/tuya/TS0601_switch_2g.cpp
    definitions/tuya/TS0601_switch_4g.cpp
    definitions/tuya/TS0601_th_sensor_1.cpp
    definitions/tuya/TS0601_th_sensor_zl.cpp
    definitions/tuya/TS0601_illuminance.cpp
    definitions/tuya/DCR_CO.cpp
    definitions/tuya/TS0601_water_sensor.cpp
    definitions/tuya/TS0601_nous_lcd_th.cpp
    definitions/tuya/TS0601_ith_sensor.cpp
    definitions/tuya/TS0601_hpsz.cpp
    definitions/tuya/TM081.cpp
    definitions/tuya/TS0601_vibration.cpp
    definitions/tuya/TS0601_din.cpp
    definitions/tuya/TS0601_tshps_radar.cpp
    definitions/tuya/TS0601_zg205z.cpp
    definitions/tuya/TS0601_switch_3g.cpp
    definitions/tuya/TS0601_thermostat.cpp
    definitions/tuya/TS0601_cover.cpp
    definitions/tuya/TS0601_cover_inv.cpp
    definitions/tuya/TS0601_garage.cpp
    definitions/tuya/TS0601_garage_matsee.cpp
    definitions/tuya/hoch_din.cpp
    definitions/tuya/TZE200_wall_airq.cpp
    definitions/tuya/TZE200_body.cpp
    definitions/tuya/TZE200_vanadium.cpp
    definitions/tuya/TZE200_scene_2g.cpp
    definitions/tuya/TZE200_switch_3g_cd.cpp
    definitions/tuya/TZE200_dual_dimmer.cpp
    definitions/tuya/TZE200_rf_ctrl.cpp
    definitions/tuya/TZE200_usb_sw.cpp
    definitions/tuya/TZE200_water_boiler.cpp
    definitions/tuya/TZE200_ir_heater.cpp
    definitions/tuya/TZE200_fan_thermo.cpp
    definitions/tuya/TZE200_dehumid.cpp
    definitions/tuya/TZE200_evap_cool.cpp
    definitions/tuya/TZE200_proj_screen.cpp
    definitions/tuya/TZE200_garage_opener.cpp
    definitions/tuya/TZE200_vent.cpp
    definitions/tuya/TZE200_tubular_motor.cpp
    definitions/tuya/TZE200_doorbell.cpp
    definitions/tuya/TS004A.cpp
    definitions/tuya/TZE200_sprinkler.cpp
    definitions/tuya/TZE200_hcho_co2.cpp
    definitions/tuya/TZE200_multi_gas.cpp
    definitions/tuya/TZE200_radon.cpp
    definitions/tuya/TZE200_mailbox.cpp
    definitions/tuya/TZE200_wind.cpp
    definitions/tuya/TZE200_snow.cpp
    definitions/tuya/TZE200_light_curtain.cpp
    definitions/tuya/TZE200_1g_usb_cd.cpp
    definitions/tuya/TZE200_4g_bl.cpp
    definitions/tuya/TZE200_smart_breaker.cpp
    definitions/tuya/TZE200_din_relay.cpp
    definitions/tuya/TZE200_neutral_relay.cpp
    definitions/tuya/TZE200_heated_mat.cpp
    definitions/tuya/TZE200_swamp.cpp
    definitions/tuya/TZE200_ceiling_fan.cpp
    definitions/tuya/TZE200_towel_warmer.cpp
    definitions/tuya/TZE200_garden_timer.cpp
    definitions/tuya/TZE200_soil_ec.cpp
    definitions/tuya/TZE200_solar.cpp
    definitions/tuya/TZE200_ph.cpp
    definitions/tuya/TS0045.cpp
    definitions/tuya/TS110F.cpp
    definitions/tuya/TZE200_ev_charger.cpp
    definitions/tuya/TS0204.cpp
    definitions/tuya/TS0206.cpp
    definitions/tuya/TZE200_rgb_strip.cpp
    definitions/tuya/TZE200_night_light.cpp
    definitions/tuya/TZE200_mood_light.cpp
    definitions/tuya/TZE200_panel_light.cpp
    definitions/tuya/TZE200_star_light.cpp
    definitions/tuya/TZE200_scale.cpp
    definitions/tuya/TZE200_glass.cpp
    definitions/tuya/TZE200_air_filter.cpp
    definitions/tuya/TZE200_uv_lamp.cpp
    definitions/tuya/TZE200_sleep_v2.cpp
    definitions/tuya/TZE200_scada.cpp
    definitions/tuya/TZE200_tank.cpp
    definitions/tuya/TZE200_conveyor.cpp
    definitions/tuya/TZE200_pressure_vessel.cpp
    definitions/tuya/TZE200_motor_vib.cpp
    definitions/tuya/TZE200_washer.cpp
    definitions/tuya/TZE200_kettle.cpp
    definitions/tuya/TZE200_rice_cooker.cpp
    definitions/tuya/TZE200_robot_vac.cpp
    definitions/tuya/TZE200_air_fryer.cpp
    definitions/tuya/TS0014.cpp
    definitions/tuya/TS0505A.cpp
    definitions/tuya/TS0505B.cpp
    definitions/tuya/TS0503A.cpp
    definitions/tuya/TS0502A.cpp
    definitions/tuya/TZE200_alcohol.cpp
    definitions/tuya/TZE200_orp.cpp
    definitions/tuya/TZE200_tds.cpp
    definitions/tuya/TZE200_leaf.cpp
    definitions/tuya/TZE200_bme680.cpp
    definitions/tuya/TZE200_anemometer.cpp
    definitions/tuya/TZE200_light_uv.cpp
    definitions/tuya/TZE200_fp.cpp
    definitions/tuya/TZE200_5g_scene.cpp
    definitions/tuya/TZE200_load_relay.cpp
    definitions/tuya/TZE200_socket_v3.cpp
    definitions/tuya/TZE200_panel_6g.cpp
    definitions/tuya/TZE200_wall_heater.cpp
    definitions/tuya/TZE200_humidifier.cpp
    definitions/tuya/TZE200_dishwasher.cpp
    definitions/tuya/TZE200_fridge.cpp
    definitions/tuya/TZE200_coffee.cpp
    definitions/tuya/TZE200_outdoor_siren.cpp
    definitions/tuya/TZE200_gate.cpp
    definitions/tuya/TZE200_street_light.cpp
    definitions/tuya/TS000F.cpp
    definitions/tuya/TS0101.cpp
    definitions/tuya/TS0501A.cpp
    definitions/tuya/TS0504A.cpp
    definitions/tuya/TS030F.cpp
    definitions/tuya/TS0218.cpp
    definitions/tuya/TS0219.cpp
    definitions/tuya/TS0111.cpp
    definitions/tuya/TZE200_dust.cpp
    definitions/tuya/TZE200_purifier.cpp
    definitions/tuya/TZE200_emeter.cpp
    definitions/tuya/TZE200_gas_det.cpp
    definitions/tuya/TZE200_fan_dim.cpp
    definitions/tuya/TZE200_water_heater.cpp
    definitions/tuya/TZE200_shutter_ctrl.cpp
    definitions/tuya/TZE200_zone_heat.cpp
    definitions/tuya/TZE200_2g_relay.cpp
    definitions/tuya/TZE200_6g_relay.cpp
    definitions/tuya/TZE200_oled_thermo.cpp
    definitions/tuya/TZE200_digital_scale.cpp
    definitions/tuya/TZE200_water_timer.cpp
    definitions/tuya/TZE200_4key_pad.cpp
    definitions/tuya/TZE200_solar_inv.cpp
    definitions/tuya/TZE200_batt_tester.cpp
    definitions/tuya/TZE200_solenoid.cpp
    definitions/tuya/TZE204_thermostat_v4.cpp
    definitions/tuya/TZE200_hvac_v2.cpp
    definitions/tuya/TS0503B.cpp
    definitions/tuya/TS0502B.cpp
    definitions/tuya/TS0504B.cpp
    definitions/tuya/TZE200_aao6qtcs.cpp
    definitions/tuya/TZE200_bqcqqjpb.cpp
    definitions/tuya/TZE200_c88teujp.cpp
    definitions/tuya/TZE200_cpmgn2cf.cpp
    definitions/tuya/TZE200_dsjszp0x.cpp
    definitions/tuya/TZE200_bq5c8xfe.cpp
    definitions/tuya/TZE200_e3oitdyu.cpp
    definitions/tuya/TZE200_g1o5lwkd.cpp
    definitions/tuya/TZE200_tz32mtia.cpp
    definitions/tuya/TZE200_bxeedvvy.cpp
    definitions/tuya/TZE200_cwnjrr72.cpp
    definitions/tuya/TZE200_husqqvux.cpp
    definitions/tuya/TZE200_4hbc6bwa.cpp
    definitions/tuya/TZE200_gz8j3tva.cpp
    definitions/tuya/TZE200_e4cfv1l0.cpp
    definitions/tuya/TZE200_5d3vhjro.cpp
    definitions/tuya/TZE200_1hwjutgo.cpp
    definitions/tuya/TZE200_mraovvmm.cpp
    definitions/tuya/TZE200_8nyaanzb.cpp
    definitions/tuya/TZE200_rmjr4ufz.cpp
    definitions/tuya/TZE200_ar0slwnd.cpp
    definitions/tuya/TZE200_cf1sl3tj.cpp
    definitions/tuya/TZE200_nueqqe6k.cpp
    definitions/tuya/TZE200_zpzndjez.cpp
    definitions/tuya/TZE200_68nvbio9.cpp
    definitions/lumi/WXKG01LM.cpp
    definitions/lumi/WXKG11LM.cpp
    definitions/lumi/RTCGQ11LM.cpp
    definitions/lumi/ZNCZ02LM.cpp
    definitions/lumi/MCCGQ01LM.cpp
    definitions/lumi/LGZCDD01LM.cpp
    definitions/lumi/QBKG11LM.cpp
    definitions/lumi/WXKG03LM.cpp
    definitions/lumi/PS_S04D.cpp
    definitions/lumi/UT_A01E.cpp
    definitions/lumi/WT_A03E.cpp
    definitions/lumi/WS_K03E.cpp
    definitions/lumi/TH_S04D.cpp
    definitions/lumi/WS_K02E.cpp
    definitions/lumi/WS_K04E.cpp
    definitions/lumi/WS_K05E.cpp
    definitions/lumi/KD_R01D.cpp
    definitions/lumi/DS_K02D_DS_K02E.cpp
    definitions/lumi/WS_K08D.cpp
    definitions/lumi/T2_E27_CCT.cpp
    definitions/lumi/T2_E27.cpp
    definitions/lumi/WS_K07E.cpp
    definitions/lumi/VC_X01D.cpp
    definitions/lumi/CL_L02D.cpp
    definitions/lumi/WS_K01D.cpp
    definitions/lumi/ZNQBKG45LM.cpp
    definitions/lumi/ZNQBKG44LM.cpp
    definitions/lumi/ZNQBKG43LM.cpp
    definitions/lumi/ZNQBKG42LM.cpp
    definitions/lumi/ZNQBKG41LM.cpp
    definitions/lumi/ZNQBKG40LM.cpp
    definitions/lumi/ZNQBKG39LM.cpp
    definitions/lumi/ZNQBKG38LM.cpp
    definitions/lumi/ZNCWWSQ01LM.cpp
    definitions/lumi/SRTS_A01.cpp
    definitions/lumi/WXKG17LM.cpp
    definitions/lumi/QBCZ14LM.cpp
    definitions/lumi/QBCZ15LM.cpp
    definitions/lumi/VOCKQJK11LM.cpp
    definitions/lumi/QBKG39LM.cpp
    definitions/lumi/QBKG38LM.cpp
    definitions/lumi/ZNCJMB14LM.cpp
    definitions/lumi/SSWQD03LM.cpp
    definitions/lumi/ZNDDQDQ13LM.cpp
    definitions/lumi/ZNDDQDQ12LM.cpp
    definitions/lumi/ZNDDQDQ11LM.cpp
    definitions/lumi/HLQDQ01LM.cpp
    definitions/lumi/ZNTGMK12LM.cpp
    definitions/lumi/TDL01LM.cpp
    definitions/lumi/LLKZMK12LM.cpp
    definitions/lumi/LLKZMK11LM.cpp
    definitions/lumi/ZNCLDJ01LM.cpp
    definitions/lumi/ZNGZDJ16LM.cpp
    definitions/lumi/ZNGZDJ11LM.cpp
    definitions/lumi/JTQJ_BF_01LM_BW.cpp
    definitions/lumi/ZNQBCZ11LM.cpp
    definitions/lumi/FP1E.cpp
    definitions/lumi/QBKG20LM.cpp
    definitions/lumi/QBKG19LM.cpp
    definitions/lumi/QBKG18LM.cpp
    definitions/lumi/QBKG17LM.cpp
    definitions/lumi/QBKG26LM.cpp
    definitions/lumi/QBKG28LM.cpp
    definitions/lumi/QBKG27LM.cpp
    definitions/lumi/WS_USC02.cpp
    definitions/lumi/WS_USC01.cpp
    definitions/lumi/LGYCDD01LM.cpp
    definitions/lumi/JWDL001A.cpp
    definitions/lumi/JWSP001A.cpp
    definitions/lumi/KQJCMB11LM.cpp
    definitions/lumi/WXKG04LM.cpp
    definitions/lumi/ZNCZ15LM.cpp
    definitions/lumi/RTCGQ12LM.cpp
    definitions/lumi/RTCGQ13LM.cpp
    definitions/lumi/RTCGQ14LM.cpp
    definitions/lumi/MCCGQ11LM.cpp
    definitions/lumi/WXKG02LM.cpp
    definitions/lumi/WXKG12LM.cpp
    definitions/lumi/WSDCGQ11LM.cpp
    definitions/lumi/DJT11LM.cpp
    definitions/lumi/RTCGQ01LM.cpp
    definitions/lumi/MCCGQ14LM.cpp
    definitions/lumi/WXKG06LM.cpp
    definitions/lumi/WXKG07LM.cpp
    definitions/lumi/WXKG13LM.cpp
    definitions/lumi/WSDCGQ01LM.cpp
    definitions/lumi/SJCGQ11LM.cpp
    definitions/lumi/JTYJGD01LM.cpp
    definitions/lumi/WXKG14LM.cpp
    definitions/lumi/WXKG16LM.cpp
    definitions/lumi/WXKG20LM.cpp
    definitions/lumi/RTCGQ15LM.cpp
    definitions/lumi/MCCGQ13LM.cpp
    definitions/lumi/WSDCGQ12LM.cpp
    definitions/lumi/SJCGQ12LM.cpp
    definitions/lumi/JYGZ01AQ.cpp
    definitions/lumi/WXKG15LM.cpp
    definitions/lumi/WXKG18LM.cpp

    definitions/lumi/WXKG21LM.cpp
    definitions/lumi/DJT12LM.cpp
    definitions/lumi/RTCGQ16LM.cpp
    definitions/lumi/MCCGQ12LM.cpp
    definitions/lumi/GZCGQ01LM.cpp
    definitions/lumi/RTCGQ21LM.cpp
    definitions/lumi/RTCGQ22LM.cpp
    definitions/lumi/MCCGQ16LM.cpp
    definitions/lumi/MCCGQ17LM.cpp
    definitions/lumi/WXKG22LM.cpp
    definitions/lumi/WXKG23LM.cpp
    definitions/lumi/WSDCGQ21LM.cpp
    definitions/lumi/DJT21LM.cpp

    definitions/lumi/MCCGQ18LM.cpp
    definitions/lumi/MCCGQ19LM.cpp
    definitions/lumi/WXKG24LM.cpp
    definitions/lumi/WXKG25LM.cpp
    definitions/lumi/WXKG26LM.cpp
    definitions/lumi/WXCJKG11LM.cpp
    definitions/lumi/WXCJKG12LM.cpp
    definitions/lumi/WXCJKG13LM.cpp
    definitions/lumi/RTCGQ24LM.cpp
    definitions/lumi/RTCGQ25LM.cpp
    definitions/lumi/MCCGQ20LM.cpp
    definitions/lumi/SJCGQ13LM.cpp
    definitions/lumi/DJT31LM.cpp
    definitions/lumi/RTCGQ26LM.cpp
    definitions/lumi/RTCGQ27LM.cpp
    definitions/lumi/RTCGQ28LM.cpp
    definitions/lumi/RTCGQ31LM.cpp
    definitions/lumi/RTCGQ32LM.cpp
    definitions/lumi/MCCGQ21LM.cpp
    definitions/lumi/MCCGQ22LM.cpp
    definitions/lumi/MCCGQ23LM.cpp
    definitions/lumi/MCCGQ24LM.cpp
    definitions/lumi/WXKG27LM.cpp
    definitions/lumi/WXKG28LM.cpp
    definitions/lumi/WXKG29LM.cpp
    definitions/lumi/WXKG30LM.cpp
    definitions/lumi/WXKG31LM.cpp
    definitions/lumi/WSDCGQ22LM.cpp
    definitions/lumi/WSDCGQ23LM.cpp
    definitions/lumi/WSDCGQ24LM.cpp
    definitions/lumi/GZCGQ11LM.cpp
    definitions/lumi/SJCGQ14LM.cpp
    definitions/lumi/SJCGQ15LM.cpp
    definitions/lumi/JYGZ03AQ.cpp
    definitions/lumi/JTBZ01AQ.cpp
    definitions/lumi/DJT41LM.cpp
    definitions/lumi/DJT51LM.cpp
    definitions/lumi/DWZTCGQ11LM.cpp
    definitions/lumi/QBKG04LM.cpp
    definitions/lumi/QBKG21LM.cpp
    definitions/lumi/QBKG23LM.cpp
    definitions/lumi/SSM_U01.cpp
    definitions/lumi/WS_EUK01.cpp
    definitions/lumi/QBKG32LM.cpp
    definitions/lumi/QBKG33LM.cpp
    definitions/lumi/ZNQBKG11LM.cpp
    definitions/lumi/ZNQBKG12LM.cpp
    definitions/lumi/QBKG03LM.cpp
    definitions/lumi/QBKG12LM.cpp
    definitions/lumi/QBKG22LM.cpp
    definitions/lumi/QBKG24LM.cpp
    definitions/lumi/QBKG25LM.cpp
    definitions/lumi/SSM_U02.cpp
    definitions/lumi/WS_EUK03.cpp
    definitions/lumi/QBKG34LM.cpp
    definitions/lumi/ZNQBKG13LM.cpp
    definitions/lumi/ZNQBKG14LM.cpp
    definitions/lumi/QBKG29LM.cpp
    definitions/lumi/QBKG30LM.cpp
    definitions/lumi/QBKG31LM.cpp
    definitions/lumi/WS_EUK04.cpp
    definitions/lumi/QBKG35LM.cpp
    definitions/lumi/QBKG40LM.cpp
    definitions/lumi/QBKG41LM.cpp
    definitions/lumi/ZNQBKG15LM.cpp
    definitions/lumi/ZNQBKG16LM.cpp
    definitions/lumi/ZNQBKG17LM.cpp
    definitions/lumi/ZNLDP12LM.cpp
    definitions/lumi/ZNLDP13LM.cpp
    definitions/lumi/ZNLDP14LM.cpp
    definitions/lumi/ZNLDP16LM.cpp
    definitions/lumi/ZNLDP17LM.cpp
    definitions/lumi/ZNLDP18LM.cpp
    definitions/lumi/ZNLDP19LM.cpp
    definitions/lumi/ZNLDP20LM.cpp
    definitions/lumi/ZNLDP21LM.cpp
    definitions/lumi/ZNLDP22LM.cpp
    definitions/lumi/ZNLDP23LM.cpp
    definitions/lumi/ZNLDP24LM.cpp
    definitions/lumi/ZNLDP25LM.cpp
    definitions/lumi/ZNLDP26LM.cpp
    definitions/lumi/ZNLDP27LM.cpp
    definitions/lumi/ZNLDP28LM.cpp
    definitions/lumi/ZNLDP30LM.cpp
    definitions/lumi/ZNLDP31LM.cpp
    definitions/lumi/ZNXNKG01LM.cpp
    definitions/lumi/ZNXNKG02LM.cpp
    definitions/lumi/ZNXNKG03LM.cpp
    definitions/lumi/ZNXNKG04LM.cpp
    definitions/lumi/ZNXNKG05LM.cpp
    definitions/lumi/ZNCZ03LM.cpp
    definitions/lumi/ZNCZ04LM.cpp
    definitions/lumi/ZNCZ11LM.cpp
    definitions/lumi/ZNCZ12LM.cpp
    definitions/lumi/ZNCZ14LM.cpp
    definitions/lumi/SP_EUC01.cpp
    definitions/lumi/WP_P01D.cpp
    definitions/lumi/QBCZ11LM.cpp
    definitions/lumi/QBCZ13LM.cpp
    definitions/lumi/ZNMCPZ01LM.cpp
    definitions/lumi/ZNMCPZ02LM.cpp
    definitions/lumi/SP_EUC02.cpp
    definitions/lumi/WP_P02D.cpp
    definitions/lumi/WP_P03D.cpp
    definitions/lumi/MFKZQ01LM.cpp
    definitions/lumi/CTPR01.cpp
    definitions/lumi/ZNCLDJ11LM.cpp
    definitions/lumi/ZNCLDJ12LM.cpp
    definitions/lumi/ZNCLDJ14LM.cpp
    definitions/lumi/ZNCLBL01LM.cpp
    definitions/lumi/ZNMS11LM.cpp
    definitions/lumi/ZNMS12LM.cpp
    definitions/lumi/ZNMS13LM.cpp
    definitions/lumi/RTCZCGQ11LM.cpp
    definitions/lumi/RTCZCGQ12LM.cpp
    definitions/lumi/ZNQPKG01LM.cpp
    definitions/lumi/SP_PDC01.cpp
    definitions/lumi/ZNCLDJ15LM.cpp
    definitions/lumi/SRSCM01.cpp
    definitions/lumi/ZNXJKG01LM.cpp
    definitions/lumi/ZNWXKG01LM.cpp
    definitions/lumi/SP_WP01.cpp
    definitions/lumi/WGLZ00LM.cpp
)
target_include_directories(zhc_definitions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(zhc_definitions PUBLIC zhc_parser zhc_runtime)
# Device cpps rely on zero-initialisation of unused PreparedDefinition
# fields (Tuya-side discriminators, expose arrays, etc). Silence the
# missing-field warning here only — core/runtime code still sees it.
target_compile_options(zhc_definitions PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-missing-field-initializers>
)

# Auto-generated Tuya device cpps (produced by tools/generate_tuya_ports.py
# against z2m's devices/tuya.ts). These are placeholder stubs — the DP
# ones are inert until a human fills in the map. CONFIGURE_DEPENDS means
# CMake picks up new generated files on re-configure without touching
# the hand-maintained list.
file(GLOB TUYA_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/tuya/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${TUYA_GENERATED_SRCS})

# Moes registry + generated cpps (Tuya OEM rebrand — thin wrappers).
file(GLOB MOES_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/moes/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/moes/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${MOES_GENERATED_SRCS})

# Zosung IR runtime — shared state machine for chunked IR file
# transfer (clusters 0xED00 / 0xE004). Used by Moes UFO-R11 family
# and Tuya/Zosung IR clones. Header-only API, single shared cpp.
file(GLOB ZOSUNG_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/zosung/*.cpp)
target_sources(zhc_definitions PRIVATE ${ZOSUNG_SRCS})

# Philips Hue registry + generated cpps. Generator lives in
# tools/generate_philips_ports.py; every entry pulls in the shared
# generic on-off + brightness + color-temp converters.
file(GLOB PHILIPS_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/philips/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/philips/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${PHILIPS_GENERATED_SRCS})

# Sonoff / Itead / eWeLink registry + generated cpps. Generator in
# tools/generate_sonoff_ports.py lowers `m.onOff/battery/light/
# commandsOnOff` into stock generic converters.
file(GLOB SONOFF_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/sonoff/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/sonoff/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${SONOFF_GENERATED_SRCS})

# Heiman + Saswell registries.
file(GLOB HEIMAN_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/heiman/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/heiman/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${HEIMAN_GENERATED_SRCS})

file(GLOB SASWELL_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/saswell/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/saswell/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${SASWELL_GENERATED_SRCS})

# Efekta — air-quality / CO2 / PM / soil sensors (modernExtend only).
file(GLOB EFEKTA_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/efekta/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/efekta/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${EFEKTA_GENERATED_SRCS})

# Schneider Electric — Wiser / Clipsal / Merten devices.
file(GLOB SCHNEIDER_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/schneider/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/schneider/generated/*.cpp)
target_sources(zhc_definitions PRIVATE ${SCHNEIDER_GENERATED_SRCS})

# Tier A — Ikea / Innr / Gledopto (lighting-dominated).
file(GLOB IKEA_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/ikea/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/ikea/generated/*.cpp)
file(GLOB INNR_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/innr/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/innr/generated/*.cpp)
file(GLOB GLEDOPTO_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/gledopto/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/gledopto/generated/*.cpp)
target_sources(zhc_definitions PRIVATE
     ${IKEA_GENERATED_SRCS}
     ${INNR_GENERATED_SRCS}
     ${GLEDOPTO_GENERATED_SRCS})

# Tier B — Sunricher / Namron / Ledvance / Osram / Adeo.
file(GLOB SUNRICHER_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/sunricher/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/sunricher/generated/*.cpp)
file(GLOB NAMRON_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/namron/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/namron/generated/*.cpp)
file(GLOB LEDVANCE_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/ledvance/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/ledvance/generated/*.cpp)
file(GLOB OSRAM_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/osram/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/osram/generated/*.cpp)
file(GLOB ADEO_GENERATED_SRCS CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/adeo/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/adeo/generated/*.cpp)
target_sources(zhc_definitions PRIVATE
     ${SUNRICHER_GENERATED_SRCS}
     ${NAMRON_GENERATED_SRCS}
     ${LEDVANCE_GENERATED_SRCS}
     ${OSRAM_GENERATED_SRCS}
     ${ADEO_GENERATED_SRCS})

# Tier C — 13 mid-size vendors (30–45 devices each).
foreach(TC_V IN ITEMS legrand shinasystem third_reality robb paulmann
                       orvibo slacky_diy nue_3a muller_licht develco
                       iluminize hive feibit)
    file(GLOB TC_SRCS CONFIGURE_DEPENDS
         ${CMAKE_CURRENT_SOURCE_DIR}/definitions/${TC_V}/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/definitions/${TC_V}/generated/*.cpp)
    target_sources(zhc_definitions PRIVATE ${TC_SRCS})
endforeach()

# Tier D — 18 mid-small vendors (~18–30 devices each).
foreach(TD_V IN ITEMS yale sengled smartthings yokis zemismart ewelink
                       candeo sylvania sinope qa lincukoo bitron
                       aurora_lighting immax bosch shelly owon adurosmart)
    file(GLOB TD_SRCS CONFIGURE_DEPENDS
         ${CMAKE_CURRENT_SOURCE_DIR}/definitions/${TD_V}/*.cpp
         ${CMAKE_CURRENT_SOURCE_DIR}/definitions/${TD_V}/generated/*.cpp)
    target_sources(zhc_definitions PRIVATE ${TD_SRCS})
endforeach()

# Tier E — 323 long-tail vendors (auto-generated, aggregator-driven).
file(GLOB TIER_E_AGG
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/_tier_e/*.cpp)
target_sources(zhc_definitions PRIVATE ${TIER_E_AGG})
file(GLOB TIER_E_VENDOR_DIRS
     LIST_DIRECTORIES true
     ${CMAKE_CURRENT_SOURCE_DIR}/definitions/*)
foreach(TE_DIR IN LISTS TIER_E_VENDOR_DIRS)
    if(IS_DIRECTORY ${TE_DIR})
        get_filename_component(TE_NAME ${TE_DIR} NAME)
        # Skip already-wired dirs from Tiers 0-D + internal scaffolding.
        set(SKIP_NAMES _generic _tier_e
            lumi tuya moes philips sonoff heiman saswell efekta schneider
            ikea innr gledopto sunricher namron ledvance osram adeo
            legrand shinasystem third_reality robb paulmann orvibo slacky_diy
            nue_3a muller_licht develco iluminize hive feibit
            yale sengled smartthings yokis zemismart ewelink candeo sylvania
            sinope qa lincukoo bitron aurora_lighting immax bosch shelly
            owon adurosmart)
        list(FIND SKIP_NAMES ${TE_NAME} _found)
        if(_found EQUAL -1)
            file(GLOB _TE_SRCS CONFIGURE_DEPENDS
                 ${TE_DIR}/*.cpp ${TE_DIR}/generated/*.cpp)
            target_sources(zhc_definitions PRIVATE ${_TE_SRCS})
        endif()
    endif()
endforeach()

add_executable(zhc_tests tests/test_main.cpp)
target_link_libraries(zhc_tests PRIVATE zhc_core)

add_executable(zhc_parser_tests tests/test_foundation.cpp)
target_link_libraries(zhc_parser_tests PRIVATE zhc_parser)

add_executable(zhc_decoder_tests tests/test_decoder.cpp)
target_link_libraries(zhc_decoder_tests PRIVATE zhc_parser)

add_executable(zhc_fixture_runner tools/fixture_runner.cpp)
target_include_directories(zhc_fixture_runner PRIVATE tools)
target_link_libraries(zhc_fixture_runner PRIVATE zhc_definitions)

add_executable(zhc_dispatch_tests tests/test_dispatch.cpp)
target_link_libraries(zhc_dispatch_tests PRIVATE zhc_runtime)

add_executable(zhc_config_steps_tests tests/test_config_steps.cpp)
target_link_libraries(zhc_config_steps_tests PRIVATE zhc_runtime)

add_executable(zhc_configure_write_defs_tests tests/test_configure_write_defs.cpp)
target_link_libraries(zhc_configure_write_defs_tests PRIVATE zhc_definitions)

add_executable(zhc_matcher_tests tests/test_definition_runtime.cpp)
target_link_libraries(zhc_matcher_tests PRIVATE zhc_runtime)

add_executable(zhc_timer_tests tests/test_timer.cpp)
target_link_libraries(zhc_timer_tests PRIVATE zhc_core)

add_executable(zhc_expose_range_tests tests/test_expose_range.cpp)
target_link_libraries(zhc_expose_range_tests PRIVATE zhc_core)

add_executable(zhc_wxkg01lm_tests tests/test_wxkg01lm.cpp)
target_link_libraries(zhc_wxkg01lm_tests PRIVATE zhc_definitions)

add_executable(zhc_rtcgq11lm_tests tests/test_rtcgq11lm.cpp)
target_link_libraries(zhc_rtcgq11lm_tests PRIVATE zhc_definitions)

add_executable(zhc_philips_motion_tests tests/test_philips_motion.cpp)
target_link_libraries(zhc_philips_motion_tests PRIVATE zhc_definitions)

add_executable(zhc_generic_tests tests/test_generic.cpp)
target_link_libraries(zhc_generic_tests PRIVATE zhc_definitions)

add_executable(zhc_modern_extend_tests tests/test_modern_extend.cpp)
target_link_libraries(zhc_modern_extend_tests PRIVATE zhc_runtime zhc_parser)

add_executable(zhc_lumi_specific_tests tests/test_lumi_specific.cpp)
target_link_libraries(zhc_lumi_specific_tests PRIVATE zhc_definitions)

add_executable(zhc_wxkg11lm_tests tests/test_wxkg11lm.cpp)
target_link_libraries(zhc_wxkg11lm_tests PRIVATE zhc_definitions)

add_executable(zhc_zncz02lm_tests tests/test_zncz02lm.cpp)
target_link_libraries(zhc_zncz02lm_tests PRIVATE zhc_definitions)

add_executable(zhc_lumi_shared_extra_tests tests/test_lumi_shared_extra.cpp)
target_link_libraries(zhc_lumi_shared_extra_tests PRIVATE zhc_definitions)

add_executable(zhc_lgzcdd01lm_tests tests/test_lgzcdd01lm.cpp)
target_link_libraries(zhc_lgzcdd01lm_tests PRIVATE zhc_definitions)

add_executable(zhc_lumi_factories_tests tests/test_lumi_factories.cpp)
target_link_libraries(zhc_lumi_factories_tests PRIVATE zhc_definitions)

add_executable(zhc_lumi_extend_tests tests/test_lumi_extend.cpp)
target_link_libraries(zhc_lumi_extend_tests PRIVATE zhc_definitions)

add_executable(zhc_wave_a_tests tests/test_wave_a.cpp)
target_link_libraries(zhc_wave_a_tests PRIVATE zhc_definitions)

add_executable(zhc_wave_b_tests tests/test_wave_b.cpp)
target_link_libraries(zhc_wave_b_tests PRIVATE zhc_definitions)

add_executable(zhc_wave_c_tests tests/test_wave_c.cpp)
target_link_libraries(zhc_wave_c_tests PRIVATE zhc_definitions)

add_executable(zhc_wave_d_tests tests/test_wave_d.cpp)
target_link_libraries(zhc_wave_d_tests PRIVATE zhc_definitions)

add_executable(zhc_wave_e_tests tests/test_wave_e.cpp)
target_link_libraries(zhc_wave_e_tests PRIVATE zhc_definitions)

add_executable(zhc_lumi_registry_tests tests/test_lumi_registry.cpp)
target_link_libraries(zhc_lumi_registry_tests PRIVATE zhc_definitions)

add_executable(zhc_lumi_switch_reports_tests tests/test_lumi_switch_reports.cpp)
target_link_libraries(zhc_lumi_switch_reports_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_matcher_tests tests/test_tuya_matcher.cpp)
target_link_libraries(zhc_tuya_matcher_tests PRIVATE zhc_runtime)

add_executable(zhc_tuya_dp_tests tests/test_tuya_dp.cpp)
target_link_libraries(zhc_tuya_dp_tests PRIVATE zhc_parser)

add_executable(zhc_tuya_mcu_sync_time_tests tests/test_tuya_mcu_sync_time.cpp)
target_link_libraries(zhc_tuya_mcu_sync_time_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_datapoints_tests tests/test_tuya_datapoints.cpp)
target_link_libraries(zhc_tuya_datapoints_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_base_device_tests tests/test_tuya_base_device.cpp)
target_link_libraries(zhc_tuya_base_device_tests PRIVATE zhc_definitions)

add_executable(zhc_tze200_soil_tests tests/test_tze200_soil.cpp)
target_link_libraries(zhc_tze200_soil_tests PRIVATE zhc_definitions)

add_executable(zhc_dwcarsat_tests tests/test_dwcarsat.cpp)
target_link_libraries(zhc_dwcarsat_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_alarms_tests tests/test_tuya_alarms.cpp)
target_link_libraries(zhc_tuya_alarms_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_extend_tests tests/test_tuya_extend.cpp)
target_link_libraries(zhc_tuya_extend_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_dp_helpers_tests tests/test_tuya_dp_helpers.cpp)
target_link_libraries(zhc_tuya_dp_helpers_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_factories_tests tests/test_tuya_factories.cpp)
target_link_libraries(zhc_tuya_factories_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_devices_tests tests/test_tuya_devices.cpp)
target_link_libraries(zhc_tuya_devices_tests PRIVATE zhc_definitions)

add_executable(zhc_tz_generic_tests tests/test_tz_generic.cpp)
target_link_libraries(zhc_tz_generic_tests PRIVATE zhc_definitions)

add_executable(zhc_color_tests tests/test_color.cpp)
target_link_libraries(zhc_color_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_tz_tests tests/test_tuya_tz.cpp)
target_link_libraries(zhc_tuya_tz_tests PRIVATE zhc_definitions)

add_executable(zhc_tuya_switch_reports_tests tests/test_tuya_switch_reports.cpp)
target_link_libraries(zhc_tuya_switch_reports_tests PRIVATE zhc_definitions)
add_executable(zhc_tuya_plug_reports_tests tests/test_tuya_plug_reports.cpp)
target_link_libraries(zhc_tuya_plug_reports_tests PRIVATE zhc_definitions)
add_executable(zhc_tuya_light_reports_tests tests/test_tuya_light_reports.cpp)
target_link_libraries(zhc_tuya_light_reports_tests PRIVATE zhc_definitions)

add_executable(zhc_ts0502b_config_tests tests/test_ts0502b_config.cpp)
target_link_libraries(zhc_ts0502b_config_tests PRIVATE zhc_definitions)

add_executable(zhc_hoch_din_tz_tests tests/test_hoch_din_tz.cpp)
target_link_libraries(zhc_hoch_din_tz_tests PRIVATE zhc_definitions)

add_executable(zhc_diy_range_extender_tests tests/test_diy_range_extender.cpp)
target_link_libraries(zhc_diy_range_extender_tests PRIVATE zhc_definitions)

add_executable(zhc_sonoff_snzb02_tests tests/test_sonoff_snzb02.cpp)
target_link_libraries(zhc_sonoff_snzb02_tests PRIVATE zhc_definitions)

# Parity fixtures for three Schneider/Wiser devices whose auto-generated defs
# dropped a core decoder (EER53000 TRV ported as a switch; 550B1024 sensor
# battery-only; CCTFR6400 missing its humidity measurement). All graduated to
# parent overrides. See tests/README.md for the pattern.
add_executable(zhc_schneider_parity_tests tests/test_schneider_parity.cpp)
target_link_libraries(zhc_schneider_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Heiman IAS-Zone sensor family: ~27 generated defs
# decoded the bare "alarm" key (generic kFzIasZone) while exposing the
# semantic key (smoke/gas/water_leak/occupancy/contact/vibration/co). All
# graduated to parent overrides using the typed kFzIas<Type>Alarm converters;
# HS1MIS-3.0 re-pointed at msOccupancySensing. See tests/README.md.
add_executable(zhc_heiman_parity_tests tests/test_heiman_parity.cpp)
target_link_libraries(zhc_heiman_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Lidl Silvercrest / Livarno / Parkside family
# (Tuya-OEM). The two IAS sensors (HG06335 motion, HG06336 contact) lowered
# the generic kFzIasZone (bare "alarm") instead of the typed
# kFzIas<Type>Alarm their zoneType implies; the three command remotes
# (FB20-002, FB21-001, HG08164) emitted a controllable on/off `state`
# instead of an `action` stream; the HG08673 power plug dropped its entire
# metering surface; and PSBZS-A1 carried a duplicate battery+onoff stub
# shadowing the full Tuya-DP def. See tests/README.md.
add_executable(zhc_lidl_parity_tests tests/test_lidl_parity.cpp)
target_link_libraries(zhc_lidl_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Orvibo scene remotes CR11S8UZ + T40S6Z. Both
# were generated as state-only kFzOnOff stubs, but they are stateless
# scene remotes: z2m decodes a vendor-private raw frame on cluster 0x0017
# (fz.orvibo_raw_1 / _raw_2) into a "button_<n>_<click|hold|release>"
# action. Graduated to Tier-2 parents wiring the shared
# kFzOrviboRaw1Action / kFzOrviboRaw2Action converters. See tests/README.md.
add_executable(zhc_orvibo_parity_tests tests/test_orvibo_parity.cpp)
target_link_libraries(zhc_orvibo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Develco / Frient family. The IAS sensors
# (FLSZB/HESZB/SMSZB/MOSZB/WISZB) lowered the generic kFzIasZone (bare
# "alarm") instead of the typed kFzIas<Type>Alarm their expose declared,
# and dropped the msTemperatureMeasurement channel several models report.
# AQSZB-110 (voc/air_quality/temp/humidity) and HMSZB-120 (temp/humidity)
# wired only battery. REXZB-111 (repeater) carried a phantom IAS
# alarm/tamper bundle; SBTZB-110 (button) was wired as an on/off switch
# instead of fz.ewelink_action. All graduated to parent overrides.
add_executable(zhc_develco_parity_tests tests/test_develco_parity.cpp)
target_link_libraries(zhc_develco_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Samsung SmartThings sensor family. Motion
# sensors 3305-S / 3325-S read occupancy from fz.ias_occupancy_alarm_2
# (zoneStatus bit 1) but the generated ports wired kFzIasMotionAlarm
# (bit 0) — occupancy never fired; switched to kFzIasMotionAlarm2. The
# IM6001-BTP01 button advertised an action enum that was never populated
# (the generated def wired kFzIasZoneStatusChange, which emits alarm-bit
# booleans); z2m maps the whole ssIasZone zoneStatus value via
# fz.command_status_change_notification_action -> action. Added a vendor
# kFzStButtonAction converter (definitions/smartthings/_shared.cpp) and
# graduated all three defs to Tier-2 parents.
add_executable(zhc_smartthings_parity_tests tests/test_smartthings_parity.cpp)
target_link_libraries(zhc_smartthings_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Halo Smart Labs HALO / HALO+ smoke + CO
# detectors. Both defs were battery+on/off stubs that dropped the whole
# safety-sensor set; the Halo IAS demux converter (kFzHaloIasZone) already
# existed in _shared.cpp but was never wired, and temperature/humidity/
# pressure (standard clusters) plus the EP2 colour light were missing.
add_executable(zhc_halo_parity_tests tests/test_halo_parity.cpp)
target_link_libraries(zhc_halo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Legrand / BTicino / Netatmo battery action
# remotes. 067694/067773/067774/WNAL63/067646/067755/067767 are wireless
# pushbuttons & scene remotes with no local relay, but their generated
# defs wired kFzOnOff + a controllable on/off TZ — dead, since these
# devices only *send* genOnOff/genLevelCtrl/genScenes/cover commands. All
# graduated to Tier-2 parents wiring the generic command-action decoders
# and exposing "action" (+ per-gang suffix on the dual 067774) instead of
# a phantom "state".
add_executable(zhc_legrand_parity_tests tests/test_legrand_parity.cpp)
target_link_libraries(zhc_legrand_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the IKEA non-light family: several generated defs
# wired the full kFzIkeaLight bundle (state/brightness/color_temp) onto
# devices that are not lights — TRADFRI motion (E1525/E1745), VINDSTYRKA
# air quality (E2112), signal repeater (E1746), STARKVIND air purifier
# (E2007) — leaving their real channels dead. VALLHORN (E2134) lost its
# occupancy/illuminance decoders, and the FYRTUR/KADRILJ/PRAKTLYSING/
# TREDANSEN blinds dropped ikeaBattery(). All graduated to Tier-2 parent
# overrides. See tests/README.md.
add_executable(zhc_ikea_parity_tests tests/test_ikea_parity.cpp)
target_link_libraries(zhc_ikea_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Easyiot ZB-WB01/02/03/08 multi-button scene
# remotes. Their auto-generated defs lowered the generic
# kFzCommandOn/Off/Toggle (emit bare "on"/"off"/"toggle", no button
# prefix), but z2m's easyiot_action remaps the verbs (toggle->single,
# on->double, off->long) AND prefixes the button from src_endpoint. All
# four graduated to Tier-2 parents wiring the vendor kFzEasyiotAction.
add_executable(zhc_easyiot_parity_tests tests/test_easyiot_parity.cpp)
target_link_libraries(zhc_easyiot_parity_tests PRIVATE zhc_definitions)

# Custom devices (DiY) flower/soil sensors. ZeeFlora + b-parasite exposed
# soil_moisture but no generic kFzSoilMoisture existed (0x0408 also missing
# from cluster_names); soil reports dropped. Added the decoder + graduated
# both defs to Tier-2 (ZeeFlora also regains m.illuminance()).
add_executable(zhc_custom_devices_diy_parity_tests tests/test_custom_devices_diy_parity.cpp)
target_link_libraries(zhc_custom_devices_diy_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Yale smart-door-lock family. Every Yale lock in
# z2m decodes battery (via the shared lockExtend() helper that bundles
# fz.battery + e.battery(), or via m.battery()), but 23 of the 28 generated
# defs wired only kFzLock (lock_state) and dropped the genPowerCfg decoder,
# the "battery" expose and the 0x0001 bind — a battery-powered lock that
# never reported its charge. All 23 graduated to Tier-2 parents wiring the
# generic kFzBattery + a "battery" expose + the genPowerCfg binding. The
# lock-state decode (kFzLock) and lock/unlock encoder (kTzLock) were already
# correct and are pinned as a regression guard. See tests/README.md.
add_executable(zhc_yale_parity_tests tests/test_yale_parity.cpp)
target_link_libraries(zhc_yale_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for three Sber/SDevices IAS sensors (SBDV-00029 motion,
# SBDV-00030 contact, SBDV-00154 water leak) whose auto-generated defs wired the
# generic kFzIasZone (emits raw "alarm_1"/...) against a bare "alarm" expose, so
# the sensor state never reached the shadow. All graduated to Tier-2 parent
# overrides wiring the typed kFzIas<Type>Alarm converters. See tests/README.md.
add_executable(zhc_sber_parity_tests tests/test_sber_parity.cpp)
target_link_libraries(zhc_sber_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Lincukoo SZT06 temperature/humidity sensor, whose
# generated stub lowered only kFzBattery (battery/voltage exposes) and so
# surfaced no temperature/humidity despite reporting both on the standard
# msTemperatureMeasurement (0x0402) / msRelativeHumidity (0x0405) clusters
# (z2m m.temperature()/m.humidity()). Graduated to a Tier-2 parent override
# adding kFzTemperature + kFzHumidity and the 0x0402/0x0405 bindings. The
# remaining Lincukoo devices are Tuya-DP (tuyaDatapoints already wired); the
# named model stubs are dead duplicates of their manufacturer-discriminated
# kDefLin__TZE* twins (which win in find_definition Pass 1). See
# tests/README.md for the pattern.
add_executable(zhc_lincukoo_parity_tests tests/test_lincukoo_parity.cpp)
target_link_libraries(zhc_lincukoo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the QA QAT44Z4H (4-gang) / QAT44Z6H (6-gang) wall
# switches: both generated stubs wired generic genOnOff (0x0006) + a single
# bare `state`, but these TS0601 _TZE204_* devices are Tuya-MCU switches that
# z2m decodes via legacy.fz.tuya_switch (datapoints 1..N → gangs l1..lN on
# manuSpecificTuya 0xEF00). Graduated to Tier-2 parent overrides wiring the
# Tuya-DP infra with a {1..N → state_lN} map + per-gang exposes.
# See tests/README.md for the pattern.
add_executable(zhc_qa_parity_tests tests/test_qa_parity.cpp)
target_link_libraries(zhc_qa_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Sinopé family: WL4200/S/4210 + RM3500ZB leak sensors
# (graduated from generic kFzIasZone "alarm" to typed kFzIasWaterLeakAlarm +
# kFzTemperature), VA4200WZ/4201WZ/4220ZB valves (graduated from the wrong-
# cluster generic cover converters to genLevelCtrl/genOnOff Sinopé valve
# converters), and LM4110ZB tank monitor (genAnalogInput tank_level + temp).
# See tests/README.md for the pattern.
add_executable(zhc_sinope_parity_tests tests/test_sinope_parity.cpp)
target_link_libraries(zhc_sinope_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Sunricher family: six IAS alarm sensors (smoke/gas/
# carbon_monoxide/water_leak x2/contact) graduated from generic kFzIasZone
# "alarm" to the typed kFzIas*Alarm converters with semantic-key exposes; the
# SR-ZG9030F-PS presence sensor (mis-ported as an on/off relay) → occupancy +
# illuminance; and the HK-SENSOR-4IN1-A / SR-ZG9033TH / SR-ZG9032A-PIR sensors
# that had lost their measurement decoders (occupancy/temp/humidity/illuminance).
# See tests/README.md for the pattern.
add_executable(zhc_sunricher_parity_tests tests/test_sunricher_parity.cpp)
target_link_libraries(zhc_sunricher_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Third Reality IAS-Zone sensor family: nine generated
# defs decoded the bare "alarm" key (generic kFzIasZone) while exposing the
# semantic key (occupancy x4 [3RMS16BZ/3RSMR01067Z/3RPS01083Z/night-light
# 3RSNL02043Z], contact x2 [3RDS17BZ/3RDTS01056Z], water_leak x2
# [3RWS18BZ/3RWS0218Z], vibration [3RVS01031Z]). All graduated to Tier-2
# parent overrides using the typed kFzIas<Type>Alarm converters; the light /
# buzzer genOnOff paths on the night light + water sensor are kept intact.
# See tests/README.md for the pattern.
add_executable(zhc_third_reality_parity_tests tests/test_third_reality_parity.cpp)
target_link_libraries(zhc_third_reality_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Namron 4512752/4512753 Tuya-DP touch thermostat, whose
# full-DP def collided with a metering-only generated stub on the same TS0601 +
# _TZE204_p3lqqy2r fingerprint; the stub was removed and the full-DP def
# graduated to a Tier-2 parent override. See tests/README.md for the pattern.
add_executable(zhc_namron_parity_tests tests/test_namron_parity.cpp)
target_link_libraries(zhc_namron_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Danfoss Ally TRV 014G2461, which declared a
# pi_heating_demand expose (z2m danfossThermostat piHeatingDemand → standard
# fz.thermostat, hvacThermostat attr 0x0008) but only wired the generic
# kFzThermostat (0x0000/0x0012/0x001C), leaving the valve-demand readout
# dead. kFzDanfossThermostat now also decodes 0x0008 (raw 0-100,
# dontMapPIHeatingDemand). See tests/README.md for the pattern.
add_executable(zhc_danfoss_parity_tests tests/test_danfoss_parity.cpp)
target_link_libraries(zhc_danfoss_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Yandex (Alice) family: the YNDX-00526 contact and
# YNDX-00527 leak sensors lowered the generic kFzIasZone (bare "alarm") against
# a bare expose, the YNDX-00528 motion sensor exposed `occupancy` with no
# decoder (claimed the generic helper was missing), and the YNDX-00518 TRV
# decoded only genOnOff + battery and bound no hvacThermostat — the whole
# thermostat surface was dead. All four graduated to Tier-2 parent overrides
# wiring the typed kFzIasContactAlarm / kFzIasWaterLeakAlarm, kFzOccupancy, and
# kFzThermostat + kTzThermostat (flat surface). See tests/README.md.
add_executable(zhc_yandex_parity_tests tests/test_yandex_parity.cpp)
target_link_libraries(zhc_yandex_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the CTM Lyng family: stove-guard IAS zone bits
# (high_temperature/power_cut_off, was a dead `alarm`), water-leak bit
# mapping (leak on bit 1, was bit 0), and the MBD motion detectors'
# dead occupancy expose (now decoded via kFzOccupancy). Several defs
# were graduated to Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_ctm_parity_tests tests/test_ctm_parity.cpp)
target_link_libraries(zhc_ctm_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Bosch Smart Home family: a row of non-light
# devices (contacts/water/smoke/siren/plug/thermostats) were mis-classed
# onto the Bosch "Light" bundle, and the IAS sensors emitted a bare
# `alarm` instead of the semantic contact/occupancy/smoke/water_leak key.
# All graduated to Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_bosch_parity_tests tests/test_bosch_parity.cpp)
target_link_libraries(zhc_bosch_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Shelly S4SN-0071A "Flood Gen 4" leak sensor,
# whose auto-generated def wired the generic kFzIasZone (attr-report
# decoder emitting the bare `alarm` key) instead of the typed water-leak
# StatusChangeNotification decoder z2m uses. Graduated to a Tier-2 parent
# override (kFzIasWaterLeakAlarm + `water_leak` expose). See tests/README.md.
add_executable(zhc_shelly_parity_tests tests/test_shelly_parity.cpp)
target_link_libraries(zhc_shelly_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Bacchus DIY flower / soil sensors. Flower_Sensor_v2
# and Flower_Sensor_v4 both expose `soil_moisture` (z2m m.soilMoisture() on
# msSoilMoisture 0x0408 — the headline measurement) but the generated Tier-1
# defs never wired generic::kFzSoilMoisture, so every soil report silently
# dropped. Wired it (converter already existed for custom_devices_diy/diyruz/
# efekta flower sensors) and graduated both defs to Tier-2. See tests/README.md.
add_executable(zhc_bacchus_parity_tests tests/test_bacchus_parity.cpp)
target_link_libraries(zhc_bacchus_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the PushOk Hardware DIY family. POK003/POK010 (water
# level) exposed `contact` (genBinaryInput presentValue) with no decoder —
# wired vendor kFzPushokContact. POK008 (thermostat relay) dropped its
# temperature channel — wired generic kFzTemperature. POK016/POK017
# (window/vent) are m.windowCovering covers that were misclassified as an
# on/off switch (POK016) or stripped to battery-only (POK017) — restored the
# cover position. See tests/README.md.
add_executable(zhc_pushok_parity_tests tests/test_pushok_parity.cpp)
target_link_libraries(zhc_pushok_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Ubisys family. H1/H10 thermostats declared
# pi_heating_demand + running_mode exposes that the generic kFzThermostat
# never decoded (attrs 0x0008/0x001E) — wired kFzUbisysThermostatExtras
# (0-255 -> 0-100 % demand remap, enum->string running_mode). J1 shutter
# gained a tilt axis (attr 0x0009) via kFzUbisysCoverPositionTilt. R0
# router shed a dead on/off bundle (z2m exposes: []). All graduated to
# Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_ubisys_parity_tests tests/test_ubisys_parity.cpp)
target_link_libraries(zhc_ubisys_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Owon HVAC family. AC201/AC221/PCT504 declared a
# cooling setpoint and PCT504/PCT512 declared running_state + occupancy that
# the generic kFzThermostat (0x0000/0x0012/0x001C) never decoded and no
# occupancy decoder was wired — all dead exposes. Wired a vendor-local
# kFzOwonThermostatExtras (attr 0x0011 cooling setpoint + 0x0029 running_state
# enum->string) alongside the generic decoder, plus the generic kFzOccupancy.
# All four thermostat defs graduated to Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_owon_parity_tests tests/test_owon_parity.cpp)
target_link_libraries(zhc_owon_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the VSmart (HumanSmart) family. The HS-SEDR00ZB door
# sensor (z2m m.iasZoneAlarm zoneType:"contact", invertAlarm:true) was wired
# to the generic kFzIasZone, which emits a generic "alarm" boolean and never
# the "contact" key z2m advertises — a dead expose. Re-wired to the typed
# kFzIasContactAlarm (contact/tamper/battery_low) and graduated to a Tier-2
# parent override. Regression-covers the SEOC occupancy sensor, the SW100/
# SW200 switches (endpoint_map presence), and the custom LED/vibration TZ
# command encoders. See tests/README.md.
add_executable(zhc_vsmart_parity_tests tests/test_vsmart_parity.cpp)
target_link_libraries(zhc_vsmart_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Innr smart-plug family. Innr is otherwise a pure
# lighting/remote vendor (CLEAN), but the metering plugs SP 120/234/240 and
# OSP 240 use z2m m.electricityMeter() — which defaults to cluster:"both"
# (seMetering 0x0702 + haElectricalMeasurement 0x0B04) — while the generated
# Tier-1 ports wired only kFzMetering, dropping the current + voltage
# channels. Wired the generic kFzElectricalMeasurement + current/voltage
# exposes + 0x0B04 bind; all four graduated to Tier-2 overrides. See
# tests/README.md.
add_executable(zhc_innr_parity_tests tests/test_innr_parity.cpp)
target_link_libraries(zhc_innr_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the BITUO TECHNIK DIN-rail energy-meter family (11 SDM/SPM
# defs). All run z2m m.electricityMeter (producedEnergy + acFrequency +
# powerFactor, threePhase on SDM01W/SPM02) plus the vendor bituo_fz.total_power /
# phase_energy converters, which read STANDARD seMetering 0x0702 +
# haElectricalMeasurement 0x0B04 attrs (no manufacturer-specific cluster). The
# generated ports catalogued the full expose set but the generic kFzMetering
# (energy/power) + kFzElectricalMeasurement (power/voltage/current) decode none
# of the extras, so produced_energy / ac_frequency / power_factor /
# power_apparent / power_reactive / total_power* / per-phase b/c voltage/current/
# power / per-phase tier energy were dead. Added kFzBituoMeteringExtras +
# kFzBituoElectricalMeasurementExtras (definitions/bituo_technik/_shared.cpp),
# wired alongside the generics on all 11 graduated Tier-2 defs. See
# tests/README.md.
add_executable(zhc_bituo_technik_parity_tests tests/test_bituo_technik_parity.cpp)
target_link_libraries(zhc_bituo_technik_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the xyzroe DIY USB/DC power-monitor family
# (ZigUSB / ZigUSB_C6 / ZigDC). ZigUSB + ZigDC route their voltage/current/
# power over genAnalogInput (0x000C) with vendor description-string parsers
# (no generic 0x000C converter) -> INFRA, deferred; only their standard
# fz.temperature/fz.humidity (0x0402/0x0405) channels are portable and are
# pinned here. ZigUSB_C6 is pure modernExtend: m.electricityMeter(cluster:
# "electrical") maps to haElectricalMeasurement (0x0B04) ONLY -- the earlier
# port's kFzMetering + `energy` + 0x0702 bind were phantom and were removed,
# and the dropped m.iasZoneAlarm over-current channel (ssIasZone 0x0500 ->
# alarm_1) was restored via the generic kFzIasZoneStatusChange.
add_executable(zhc_xyzroe_parity_tests tests/test_xyzroe_parity.cpp)
target_link_libraries(zhc_xyzroe_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Hive (British Gas) family. Two gaps: (1) the contact
# (DWS003), motion (MOT003) and keypad (KEYPAD001) IAS devices lowered the
# generic kFzIasZone (bare "alarm_1") while exposing contact/occupancy — dead
# keys — fixed by swapping in the typed kFzIasContactAlarm / kFzIasMotionAlarm;
# (2) every heating receiver (SLR1/1b/1c/1d, SLR2/2b/2c, OTR1) dropped
# running_state because the generic kFzThermostat decodes only 0x0000/0x0012/
# 0x001C — fixed with a vendor-local kFzHiveThermostatExtras (attr 0x0029
# enum->string) alongside the generic decoder + a running_state expose. All
# eleven defs graduated to Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_hive_parity_tests tests/test_hive_parity.cpp)
target_link_libraries(zhc_hive_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Lytko L101Ze thermostat family: six of the seven
# L101Ze-* ports were mis-lowered as battery on/off stubs (z2m defines them
# via modernExtend thermostat({...}) dynamic exposes the generator can't read)
# and were graduated to Tier-2 parent thermostat overrides. See tests/README.md.
add_executable(zhc_lytko_parity_tests tests/test_lytko_parity.cpp)
target_link_libraries(zhc_lytko_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the SlackyDiy THERM_SLACKY_DIY_R08 thermostat, whose
# auto-generated def was mis-lowered as a battery on/off switch and was
# graduated to a Tier-2 parent override. See tests/README.md for the pattern.
add_executable(zhc_slacky_therm_r08_tests tests/test_slacky_therm_r08.cpp)
target_link_libraries(zhc_slacky_therm_r08_tests PRIVATE zhc_definitions)

# Parity fixture for the Engo (ENGO Controls) TS0601 thermostat family. The
# engo registry held SEVEN stale battery+on/off stub defs (kDef_E25_230,
# kDef_E25_BATB, kDef_E40, kDef_ECB62_ZB, kDef_EONE, kDef_EONE_230W,
# kDef_EONE_BATB) from an earlier generation pass, each duplicating the
# {TS0601 + _TZE20x_*} fingerprint of a kDefEng__TZE* DP def that carries the
# full thermostat datapoint map. Two defs per fingerprint made the resolved
# behaviour hinge on registry order (find_definition is first-match), risking
# a battery+state-only readout instead of setpoint/local_temperature/mode.
# The stubs were removed; the DP defs cover all ten manufacturer names. See
# tests/README.md for the pattern.
add_executable(zhc_engo_parity_tests tests/test_engo_parity.cpp)
target_link_libraries(zhc_engo_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Zemismart curtain/blind-motor + switch family. Six
# generic-cover stubs (kFzCoverPosition on the 0x0102 cluster) shadowed TS0601
# Tuya-DP devices that only speak 0xEF00; five duplicated a kDefZem__TZE* DP
# def's fingerprint and were removed, and ZM25TQ was graduated to a real DP
# def. Pins DP resolution + position/state decode. See tests/README.md.
add_executable(zhc_zemismart_parity_tests tests/test_zemismart_parity.cpp)
target_link_libraries(zhc_zemismart_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Avatto family (Tuya-OEM TS0601 DP devices). Three
# auto-generated DP-table decode bugs fixed: ME168_AVATTO DP47 calibration
# divisor 100->1 (z2m localTempCalibration2 is identity), and ZDMS16-1 +
# ZDMS16-2 (both channels) switch_type enums re-ordered from the generic
# switchType order to z2m switchType2 {toggle:0,state:1,momentary:2}. Pins the
# corrected calibration scale + switch_type labels on the 0xEF00 wire. See
# tests/README.md.
add_executable(zhc_avatto_parity_tests tests/test_avatto_parity.cpp)
target_link_libraries(zhc_avatto_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Neo (NEO Coolcam, Tuya-OEM) family. Four named
# devices were misrouted by the generator to IAS Zone / battery-only: the
# NAS-AB02B0 / NAS-AB02B2 sirens and NAS-PD07 PIR are Tuya-DP (neo_t_h_alarm /
# neo_alarm / neo_nas_pd07), and NAS-TH07B2 (SNZB-02) is standard-cluster T/H.
# These pin the restored Tuya-DP + standard temperature/humidity decode paths.
add_executable(zhc_neo_parity_tests tests/test_neo_parity.cpp)
target_link_libraries(zhc_neo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the ShinaSystem (SiHAS) family. Generator misses left
# several primary sensor/control channels dead: USM/OSM/DMS exposed
# occupancy with no decoder (and USM dropped illuminance), and GCM-300Z's
# gas valve had a null to_zigbee plus a state/gas_valve_state key mismatch.
# These pin the restored decode/encode paths. See tests/README.md.
add_executable(zhc_shinasystem_parity_tests tests/test_shinasystem_parity.cpp)
target_link_libraries(zhc_shinasystem_parity_tests PRIVATE zhc_definitions)

# Wirenboard WB-MSW-ZIGBEE v.3/v.4 — occupancy + co2 channels were
# declared but undecoded; wired kFzOccupancy + new generic kFzCO2.
add_executable(zhc_wirenboard_parity_tests tests/test_wirenboard_parity.cpp)
target_link_libraries(zhc_wirenboard_parity_tests PRIVATE zhc_definitions)

# Niko (Belgian wired Connected) family. The 552-80401 wireless motion
# sensor lowered the generic kFzIasZone converter (bare key "alarm")
# instead of the typed kFzIasMotionAlarm, leaving its "occupancy" expose
# dead; graduated to a Tier-2 parent and re-wired. Regression guards also
# pin the verified-correct cover (552-72301 lift-only position), scene
# controller (552-720X2 action) and double switch (552-721X2 per-endpoint
# on/off) decode shapes. See tests/README.md.
add_executable(zhc_niko_parity_tests tests/test_niko_parity.cpp)
target_link_libraries(zhc_niko_parity_tests PRIVATE zhc_definitions)

# Behavioural-parity fixture tests for devices wired in the 2026-04-25
# MOES parity sweep. See tests/README.md for the pattern.
add_executable(zhc_moes_bht002_tests tests/test_moes_bht002.cpp)
target_link_libraries(zhc_moes_bht002_tests PRIVATE zhc_definitions)

add_executable(zhc_moes_zht_sr_tests tests/test_moes_zht_sr.cpp)
target_link_libraries(zhc_moes_zht_sr_tests PRIVATE zhc_definitions)

add_executable(zhc_zosung_ir_tests tests/test_zosung_ir.cpp)
target_link_libraries(zhc_zosung_ir_tests PRIVATE zhc_definitions)

# Behavioural-parity fixtures locking wire frames captured on real
# hardware after a generator miss was hand-corrected. Regressions in
# either the definition or shared converter (kFzTuyaButtonAction,
# kFzIasMotionAlarm) will surface here before they reach the device.
add_executable(zhc_zg_204z_tests tests/test_zg_204z.cpp)
target_link_libraries(zhc_zg_204z_tests PRIVATE zhc_definitions)

add_executable(zhc_ts0044_tests tests/test_ts0044.cpp)
target_link_libraries(zhc_ts0044_tests PRIVATE zhc_definitions)

add_executable(zhc_ts004f_knob_tests tests/test_ts004f_knob.cpp)
target_link_libraries(zhc_ts004f_knob_tests PRIVATE zhc_definitions)

enable_testing()
add_test(NAME zhc_tests           COMMAND zhc_tests)
add_test(NAME zhc_parser_tests    COMMAND zhc_parser_tests)
add_test(NAME zhc_decoder_tests   COMMAND zhc_decoder_tests)
add_test(NAME zhc_dispatch_tests  COMMAND zhc_dispatch_tests)
add_test(NAME zhc_config_steps_tests COMMAND zhc_config_steps_tests)
add_test(NAME zhc_configure_write_defs_tests COMMAND zhc_configure_write_defs_tests)
add_test(NAME zhc_matcher_tests   COMMAND zhc_matcher_tests)
add_test(NAME zhc_timer_tests     COMMAND zhc_timer_tests)
add_test(NAME zhc_expose_range_tests COMMAND zhc_expose_range_tests)
add_test(NAME zhc_wxkg01lm_tests  COMMAND zhc_wxkg01lm_tests)
add_test(NAME zhc_rtcgq11lm_tests COMMAND zhc_rtcgq11lm_tests)
add_test(NAME zhc_philips_motion_tests COMMAND zhc_philips_motion_tests)
add_test(NAME zhc_generic_tests   COMMAND zhc_generic_tests)
add_test(NAME zhc_modern_extend_tests COMMAND zhc_modern_extend_tests)
add_test(NAME zhc_lumi_specific_tests COMMAND zhc_lumi_specific_tests)
add_test(NAME zhc_wxkg11lm_tests      COMMAND zhc_wxkg11lm_tests)
add_test(NAME zhc_zncz02lm_tests      COMMAND zhc_zncz02lm_tests)
add_test(NAME zhc_lumi_shared_extra_tests COMMAND zhc_lumi_shared_extra_tests)
add_test(NAME zhc_lgzcdd01lm_tests    COMMAND zhc_lgzcdd01lm_tests)
add_test(NAME zhc_lumi_factories_tests COMMAND zhc_lumi_factories_tests)
add_test(NAME zhc_lumi_extend_tests   COMMAND zhc_lumi_extend_tests)
add_test(NAME zhc_wave_a_tests        COMMAND zhc_wave_a_tests)
add_test(NAME zhc_wave_b_tests        COMMAND zhc_wave_b_tests)
add_test(NAME zhc_wave_c_tests        COMMAND zhc_wave_c_tests)
add_test(NAME zhc_wave_d_tests        COMMAND zhc_wave_d_tests)
add_test(NAME zhc_wave_e_tests        COMMAND zhc_wave_e_tests)
add_test(NAME zhc_lumi_registry_tests COMMAND zhc_lumi_registry_tests)
add_test(NAME zhc_tuya_matcher_tests  COMMAND zhc_tuya_matcher_tests)
add_test(NAME zhc_tuya_dp_tests       COMMAND zhc_tuya_dp_tests)
add_test(NAME zhc_tuya_mcu_sync_time_tests COMMAND zhc_tuya_mcu_sync_time_tests)
add_test(NAME zhc_tuya_datapoints_tests    COMMAND zhc_tuya_datapoints_tests)
add_test(NAME zhc_tuya_base_device_tests   COMMAND zhc_tuya_base_device_tests)
add_test(NAME zhc_tze200_soil_tests        COMMAND zhc_tze200_soil_tests)
add_test(NAME zhc_dwcarsat_tests           COMMAND zhc_dwcarsat_tests)
add_test(NAME zhc_tuya_alarms_tests        COMMAND zhc_tuya_alarms_tests)
add_test(NAME zhc_tuya_extend_tests        COMMAND zhc_tuya_extend_tests)
add_test(NAME zhc_tuya_dp_helpers_tests    COMMAND zhc_tuya_dp_helpers_tests)
add_test(NAME zhc_tuya_factories_tests     COMMAND zhc_tuya_factories_tests)
add_test(NAME zhc_tuya_devices_tests       COMMAND zhc_tuya_devices_tests)
add_test(NAME zhc_tz_generic_tests          COMMAND zhc_tz_generic_tests)
add_test(NAME zhc_color_tests               COMMAND zhc_color_tests)
add_test(NAME zhc_tuya_tz_tests             COMMAND zhc_tuya_tz_tests)
add_test(NAME zhc_tuya_switch_reports_tests COMMAND zhc_tuya_switch_reports_tests)
add_test(NAME zhc_tuya_plug_reports_tests   COMMAND zhc_tuya_plug_reports_tests)
add_test(NAME zhc_lumi_switch_reports_tests COMMAND zhc_lumi_switch_reports_tests)
add_test(NAME zhc_tuya_light_reports_tests  COMMAND zhc_tuya_light_reports_tests)
add_test(NAME zhc_ts0502b_config_tests      COMMAND zhc_ts0502b_config_tests)
add_test(NAME zhc_hoch_din_tz_tests         COMMAND zhc_hoch_din_tz_tests)
add_test(NAME zhc_diy_range_extender_tests  COMMAND zhc_diy_range_extender_tests)
add_test(NAME zhc_sonoff_snzb02_tests       COMMAND zhc_sonoff_snzb02_tests)
add_test(NAME zhc_schneider_parity_tests    COMMAND zhc_schneider_parity_tests)
add_test(NAME zhc_heiman_parity_tests       COMMAND zhc_heiman_parity_tests)
add_test(NAME zhc_lidl_parity_tests         COMMAND zhc_lidl_parity_tests)
add_test(NAME zhc_orvibo_parity_tests       COMMAND zhc_orvibo_parity_tests)
add_test(NAME zhc_develco_parity_tests      COMMAND zhc_develco_parity_tests)
add_test(NAME zhc_smartthings_parity_tests  COMMAND zhc_smartthings_parity_tests)
add_test(NAME zhc_halo_parity_tests         COMMAND zhc_halo_parity_tests)
add_test(NAME zhc_legrand_parity_tests      COMMAND zhc_legrand_parity_tests)
add_test(NAME zhc_ikea_parity_tests         COMMAND zhc_ikea_parity_tests)
add_test(NAME zhc_easyiot_parity_tests      COMMAND zhc_easyiot_parity_tests)
add_test(NAME zhc_custom_devices_diy_parity_tests COMMAND zhc_custom_devices_diy_parity_tests)
add_test(NAME zhc_yale_parity_tests         COMMAND zhc_yale_parity_tests)
add_test(NAME zhc_shelly_parity_tests       COMMAND zhc_shelly_parity_tests)
add_test(NAME zhc_bacchus_parity_tests      COMMAND zhc_bacchus_parity_tests)
add_test(NAME zhc_pushok_parity_tests       COMMAND zhc_pushok_parity_tests)
add_test(NAME zhc_sber_parity_tests         COMMAND zhc_sber_parity_tests)
add_test(NAME zhc_lincukoo_parity_tests     COMMAND zhc_lincukoo_parity_tests)
add_test(NAME zhc_qa_parity_tests           COMMAND zhc_qa_parity_tests)
add_test(NAME zhc_sinope_parity_tests       COMMAND zhc_sinope_parity_tests)
add_test(NAME zhc_sunricher_parity_tests    COMMAND zhc_sunricher_parity_tests)
add_test(NAME zhc_third_reality_parity_tests COMMAND zhc_third_reality_parity_tests)
add_test(NAME zhc_namron_parity_tests       COMMAND zhc_namron_parity_tests)
add_test(NAME zhc_danfoss_parity_tests      COMMAND zhc_danfoss_parity_tests)
add_test(NAME zhc_yandex_parity_tests       COMMAND zhc_yandex_parity_tests)
add_test(NAME zhc_ctm_parity_tests          COMMAND zhc_ctm_parity_tests)
add_test(NAME zhc_bosch_parity_tests        COMMAND zhc_bosch_parity_tests)
add_test(NAME zhc_ubisys_parity_tests       COMMAND zhc_ubisys_parity_tests)
add_test(NAME zhc_owon_parity_tests         COMMAND zhc_owon_parity_tests)
add_test(NAME zhc_vsmart_parity_tests       COMMAND zhc_vsmart_parity_tests)
add_test(NAME zhc_innr_parity_tests         COMMAND zhc_innr_parity_tests)
add_test(NAME zhc_bituo_technik_parity_tests COMMAND zhc_bituo_technik_parity_tests)
add_test(NAME zhc_xyzroe_parity_tests       COMMAND zhc_xyzroe_parity_tests)
add_test(NAME zhc_hive_parity_tests         COMMAND zhc_hive_parity_tests)
add_test(NAME zhc_lytko_parity_tests        COMMAND zhc_lytko_parity_tests)
add_test(NAME zhc_slacky_therm_r08_tests    COMMAND zhc_slacky_therm_r08_tests)
add_test(NAME zhc_engo_parity_tests         COMMAND zhc_engo_parity_tests)
add_test(NAME zhc_zemismart_parity_tests    COMMAND zhc_zemismart_parity_tests)
add_test(NAME zhc_avatto_parity_tests       COMMAND zhc_avatto_parity_tests)
add_test(NAME zhc_neo_parity_tests          COMMAND zhc_neo_parity_tests)
add_test(NAME zhc_shinasystem_parity_tests  COMMAND zhc_shinasystem_parity_tests)
add_test(NAME zhc_wirenboard_parity_tests   COMMAND zhc_wirenboard_parity_tests)
add_test(NAME zhc_niko_parity_tests         COMMAND zhc_niko_parity_tests)
add_test(NAME zhc_moes_bht002_tests         COMMAND zhc_moes_bht002_tests)
add_test(NAME zhc_moes_zht_sr_tests         COMMAND zhc_moes_zht_sr_tests)
add_test(NAME zhc_zosung_ir_tests           COMMAND zhc_zosung_ir_tests)
add_test(NAME zhc_zg_204z_tests             COMMAND zhc_zg_204z_tests)
add_test(NAME zhc_ts0044_tests              COMMAND zhc_ts0044_tests)
add_test(NAME zhc_ts004f_knob_tests         COMMAND zhc_ts004f_knob_tests)
add_test(NAME zhc_fixtures
         COMMAND zhc_fixture_runner
                 ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures)

# Fingerprint-collision audit: fails when a Tuya (model+manu) or
# plain (model) bucket has two cpps claiming the same key AND the
# registry's first pick has fewer wired axes than another candidate
# of the SAME vendor. Cross-vendor collisions (legitimate whitelabel
# overlaps) are informational.
add_test(NAME zhc_collision_audit
         COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tools/audit_collisions.py)

# Legacy Tuya action-name guard: pins kFzTuya{OnOff,Multi,Button}Action
# usage to its 2026-05-15 baseline. New uses (or growth in an existing
# file) fail; reductions are always allowed. Drains as the generator is
# updated to emit canonical kFzTuyaAction{Flat,PerEp} names.
add_test(NAME zhc_legacy_tuya_action_guard
         COMMAND python3
            ${CMAKE_CURRENT_SOURCE_DIR}/tools/lint_no_new_legacy_tuya_action.py)

# Tier-comment guard: every device cpp under definitions/<vendor>/
# (excluding shared + registry scaffolding) must carry a
# `// Tier [123]` marker so reviewers see coverage shape at a glance.
add_test(NAME zhc_tier_check
         COMMAND bash -c
            "set -euo pipefail; missing=0; \
             for f in $(find ${CMAKE_CURRENT_SOURCE_DIR}/definitions \
                            -maxdepth 2 -name '*.cpp' \
                            -not -name 'registry.cpp' \
                            -not -name '_shared.cpp'); do \
               if ! grep -qE 'Tier [123]' \"$f\"; then \
                 echo \"tier-comment missing: $f\" >&2; \
                 missing=$((missing + 1)); \
               fi; \
             done; \
             exit $missing")

# ── Fuzzing (FINDINGS.md F31) ──────────────────────────────────────────
# OFF by default. Enable with clang:
#   cmake -B build-fuzz -DZHC_FUZZ=ON \
#         -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .
#   cmake --build build-fuzz --target zhc_fuzz_parsers
# Fuzzes the ZCL/Tuya wire-frame walkers with libFuzzer + ASan + UBSan.
option(ZHC_FUZZ "Build the libFuzzer parser harness (clang only)" OFF)
if(ZHC_FUZZ)
    add_executable(zhc_fuzz_parsers tests/fuzz/fuzz_parsers.cpp)
    target_link_libraries(zhc_fuzz_parsers PRIVATE zhc_parser)
    target_compile_options(zhc_fuzz_parsers PRIVATE
        -g -O1 -fsanitize=fuzzer,address,undefined)
    target_link_options(zhc_fuzz_parsers PRIVATE
        -fsanitize=fuzzer,address,undefined)
endif()
