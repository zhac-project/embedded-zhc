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

# Parity fixtures for the M-ELEC "Stitchy" family (3 dimmers + 1 relay).
# The auto-ports dropped the power_on_behavior axis that z2m's m.light() /
# m.onOff() modernExtend add by default (powerOnBehavior=true →
# fz.power_on_behavior on genOnOff 0x4003 startUpOnOff + the enum expose).
# Restored via kFzPowerOnBehavior / kTzPowerOnBehavior1. Also pins the
# FALSE colour suspect: m.light() with no color/colorTemp args has NO
# 0x0300 axis. All graduated to parent overrides.
add_executable(zhc_m_elec_parity_tests tests/test_m_elec_parity.cpp)
target_link_libraries(zhc_m_elec_parity_tests PRIVATE zhc_definitions)
# Hoftronic HD300W-ZB rotary LED dimmer is z2m m.light({powerOnBehavior:true,
# ...}); the auto-port dropped the power_on_behavior axis (genOnOff 0x4003).
# Restored via kFzPowerOnBehavior / kTzPowerOnBehavior1 + the enum expose.
# FALSE colour suspect: m.light() called with no color/colorTemp args has NO
# 0x0300 axis — asserted absent.
add_executable(zhc_hoftronic_parity_tests tests/test_hoftronic_parity.cpp)
target_link_libraries(zhc_hoftronic_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the Scan Products family (12501 push dimmer = m.light(),
# 12502 switch = m.onOff()). Both modernExtend calls default powerOnBehavior=true,
# wiring fz/tz.power_on_behavior (genOnOff 0x4003 startUpOnOff) + the enum expose;
# the Tier-1 auto-ports dropped that axis. Restored via kFzPowerOnBehavior /
# kTzPowerOnBehavior1. FALSE colour suspect: bare m.light({configureReporting})
# passes no color/colorTemp args, so there is NO 0x0300 axis.
add_executable(zhc_scanproducts_parity_tests tests/test_scanproducts_parity.cpp)
target_link_libraries(zhc_scanproducts_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the Matcall BV LED dimmer drivers (ZG401224, ZG430700).
# Both are z2m m.light() with no args: on/off + brightness + power_on_behavior
# (m.light() defaults powerOnBehavior=true → fz.power_on_behavior on genOnOff
# 0x4003 startUpOnOff + the enum expose). The Tier-1 auto-ports dropped the
# power_on_behavior axis; restored via kFzPowerOnBehavior / kTzPowerOnBehavior1.
# Also pins the FALSE colour suspect: m.light() with no color/colorTemp args
# has NO 0x0300 axis. Both graduated to parent overrides.
add_executable(zhc_matcall_bv_parity_tests tests/test_matcall_bv_parity.cpp)
target_link_libraries(zhc_matcall_bv_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the LDS (Hive / Ynoa OEM) family. ZBT-RGBWLight-A0000 is
# z2m m.light({colorTemp:{range:[153,555]}, color:true}) but the auto-port
# collapsed it to on/off + brightness, dropping the colorTemp + XY/HS colour
# axis (restored via kFzColorTemperature + kFzColor, bind 0x0300). PFMOT001
# (Hive Pet Friendly Motion sensor, m.battery()+m.iasZoneAlarm zoneType
# "occupancy") was dropped entirely by the auto-port; added as battery +
# kFzIasMotionAlarm (occupancy/tamper/battery_low). FWBulb03UK (bare m.light())
# is dimmable-only and verified correct.
add_executable(zhc_lds_parity_tests tests/test_lds_parity.cpp)
target_link_libraries(zhc_lds_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Securifi Almond Click button. The auto-port mis-ported
# it as a controllable on/off switch (kFzOnOff + kTzOnOff + bare `state`); z2m
# wires fz.almond_click (ssIasAce commandArm, device-specific armmode lookup
# 3->single/0->double/2->long) + e.action([...]) + no toZigbee. Graduated to a
# parent override with the vendor kFzAlmondClick decoder. The Peanut plug
# (PP-WHT-US) is verified clean (electrical_measurement 0x0B04, no 0x0702).
add_executable(zhc_securifi_parity_tests tests/test_securifi_parity.cpp)
target_link_libraries(zhc_securifi_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_smart9_parity_tests tests/test_smart9_parity.cpp)
target_link_libraries(zhc_smart9_parity_tests PRIVATE zhc_definitions)

# Parity for the Smart Home Pty HGZB-07A RGBW downlight. z2m extend is
# m.light({colorTemp: {range: undefined}, color: true}) but the auto-generator
# collapsed m.light() to on/off + brightness only, dropping the color-temperature
# and colour (xy) axes (lightingColorCtrl 0x0300). Graduated to a parent override
# restoring kFzColorTemperature/kFzColor + tz + exposes + the 0x0300 binding. The
# HGZB-20-DE plug (m.onOff()) is verified clean (no phantom colour/metering).
add_executable(zhc_smart_home_pty_parity_tests tests/test_smart_home_pty_parity.cpp)
target_link_libraries(zhc_smart_home_pty_parity_tests PRIVATE zhc_definitions)
# Parity fixture for the Oz Smart Things DL41-03-10-R-ZB RGBW downlight. z2m is
# tuya.modernExtend.tuyaLight({colorTemp:{range:[153,500]}, color:true}) →
# on/off + brightness + colorTemp + color (xy + hs). The auto-port collapsed it
# to a battery + on/off sensor stub (phantom battery/voltage exposes). Graduated
# the def + restored the full light bundle (kFzOnOff/kFzBrightness/
# kFzColorTemperature/kFzColor + kTz*, bind 0x0006/0x0008/0x0300).
add_executable(zhc_ozsmartthings_parity_tests tests/test_ozsmartthings_parity.cpp)
target_link_libraries(zhc_ozsmartthings_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_senoro_parity_tests tests/test_senoro_parity.cpp)
target_link_libraries(zhc_senoro_parity_tests PRIVATE zhc_definitions)

# Parity for the Shyugj S901D-ZG door sensor. z2m wires m.iasZoneAlarm with
# zoneType:"generic" + zoneAttributes:["alarm_1","alarm_2","tamper",
# "battery_low"], exposing BOTH alarm bits (bit0/bit1). The auto-port wired
# generic kFzIasZone (single `alarm` key = bit0 + tamper + battery_low),
# emitting the WRONG key (`alarm` vs `alarm_1`) and dropping alarm_2 (bit1).
# Graduated to a parent override wiring kFzIasZoneStatusChange.
add_executable(zhc_shyugj_parity_tests tests/test_shyugj_parity.cpp)
target_link_libraries(zhc_shyugj_parity_tests PRIVATE zhc_definitions)
# Parity fixture for Commercial Electric. The 53170161 CCT downlight is a
# colour-temperature tunable m.light({colorTemp}), but the auto-port dropped
# the CCT axis (on/off + brightness only). Restored kFzColorTemperature /
# kTzColorTemp, the color_temp expose, and the lightingColorCtrl (0x0300) bind.
add_executable(zhc_commercial_electric_parity_tests tests/test_commercial_electric_parity.cpp)
target_link_libraries(zhc_commercial_electric_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Swann security family. The auto-port emitted dead
# channels: SWO-MOS1PA/SWO-WDS1PA used generic kFzIasZone (bare `alarm` key)
# instead of typed occupancy/contact, and SWO-KEF1PA was mis-ported as a
# controllable genOnOff switch instead of an ssIasAce arm/panic key fob.
add_executable(zhc_swann_parity_tests tests/test_swann_parity.cpp)
target_link_libraries(zhc_swann_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for three smarli. devices whose auto-generated defs dropped a
# real channel. PDM1 dimmer + 1RE1 relay + COV1 curtain all use
# m.electricityMeter() (cluster:"both" → 0x0702 energy + 0x0B04 power/voltage/
# current) but the auto-port wired only kFzMetering (0x0702), dropping the
# 0x0B04 half. COV1 also declared controls:["lift","tilt"] but wired only
# kFzCoverPosition, dropping the tilt channel. All graduated to parent overrides.
add_executable(zhc_smarli_parity_tests tests/test_smarli_parity.cpp)
target_link_libraries(zhc_smarli_parity_tests PRIVATE zhc_definitions)

# Parity for the Soanalarm SNT858Z TS0601 Tuya-MCU soil moisture sensor. The
# auto-port mis-classified it as a phantom on/off + battery device (kFzOnOff +
# kFzBattery, genOnOff/genPowerCfg bindings) and decoded nothing real. It is a
# 0xEF00 DP device: dp3 soil_moisture, dp5 temperature (raw /1), dp9
# temperature_unit enum, dp15 battery — graduated to a parent override wiring
# the Tuya DP map via factory::TuyaRw.
add_executable(zhc_soanalarm_parity_tests tests/test_soanalarm_parity.cpp)
target_link_libraries(zhc_soanalarm_parity_tests PRIVATE zhc_definitions)

# Parity for Current Products Corp CP180335E-01 ("E-Wand") tilt blind. The
# auto-port wired the LIFT cover channel (kFzCoverPosition/kTzCoverPosition ->
# "position") but z2m decodes via TILT (fz.cover_position_tilt 0x0009,
# coverStateFromTilt) and exposes e.cover_tilt() = state + tilt. Graduated to a
# parent override that swaps to kFzCoverTilt + kTzCoverState/kTzCoverPositionTilt.
add_executable(zhc_current_products_corp_parity_tests tests/test_current_products_corp_parity.cpp)
target_link_libraries(zhc_current_products_corp_parity_tests PRIVATE zhc_definitions)
# Parity for the Smartwings WM25L-Z roller shade. z2m declares meta
# {battery:{dontDividePercentage:true}} so batteryPercentageRemaining is a
# whole 0..100 % value, but the auto-port wired the default kFzBattery which
# always halves it (read 2x low). Override uses the new kFzBatteryNoDivide.
add_executable(zhc_smartwings_parity_tests tests/test_smartwings_parity.cpp)
target_link_libraries(zhc_smartwings_parity_tests PRIVATE zhc_definitions)

# Parity for the Schwaiger HAL3xx/5xx/8xx LED bulbs. The auto-ports
# flattened every m.light({colorTemp, color}) bulb to bare on/off +
# brightness, dropping the lightingColorCtrl 0x0300 colour-temp + XY/HS
# colour channels. HAL500 = CCT only; HAL300/HAL550/HAL800 = full colour.
# Also pins the ZHS-15 plug shape (0x0B04 power/voltage/current, NO 0x0702).
add_executable(zhc_schwaiger_parity_tests tests/test_schwaiger_parity.cpp)
target_link_libraries(zhc_schwaiger_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the TCI LED-driver family. "TCI - Mini ZLL I" is a
# Dali driver that is both a settable light AND a controller; the auto-
# port dropped its m.commandsOnOff/commandsLevelCtrl action stream. The
# three plain drivers are bare m.light() (dimmable only) — regression
# guard against a phantom color/color_temp axis.
add_executable(zhc_tci_parity_tests tests/test_tci_parity.cpp)
target_link_libraries(zhc_tci_parity_tests PRIVATE zhc_definitions)
add_executable(zhc_tapestry_parity_tests tests/test_tapestry_parity.cpp)
target_link_libraries(zhc_tapestry_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the YSRSAI LED-controller family. z2m drives all three
# with tuya.modernExtend.tuyaLight() (a mains-powered ZCL light), but the
# auto-port mis-classified the dimmer (ZB-DL01) and the WW/CW (ZB-CT01) as
# battery sensors — phantom battery/voltage exposes plus a dropped brightness
# (and color_temp on the CCT) channel. Restored kFzBrightness/kFzColorTemp +
# write-back and removed the phantom battery bundle. rgbcct is a regression
# guard for the full RGB+CCT axis.
add_executable(zhc_ysrsai_parity_tests tests/test_ysrsai_parity.cpp)
target_link_libraries(zhc_ysrsai_parity_tests PRIVATE zhc_definitions)
# Parity for the WMUN ZS05 universal IR remote (TS1201). The auto-port
# reduced this IR remote to a battery-only device, dropping the entire
# Zosung IR channel (6 fzZosung handlers + 2 tzZosung + the
# learn/learned/to_send exposes + the 0xED00/0xE004 cluster binds) that
# z2m wires via zosungExtend. Graduated to a parent override wired
# against the shared Zosung IR runtime (sibling of Moes UFO-R11).
add_executable(zhc_wmun_parity_tests tests/test_wmun_parity.cpp)
target_link_libraries(zhc_wmun_parity_tests PRIVATE zhc_definitions)

# Parity for the Wyze WLCKG1 ("Ford") door lock. z2m wires
# [fz.lock, fz.lock_operation_event, fz.battery]; the auto-port dropped
# fz.lock_operation_event (closuresDoorLock cmd 0x21 ServerToClient ->
# action / action_user / action_source[_name]). Graduated to a parent
# override wiring the generic kFzLockOperationEvent.
add_executable(zhc_wyze_parity_tests tests/test_wyze_parity.cpp)
target_link_libraries(zhc_wyze_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Ajax Online LED-bulb family. Pins the dropped
# colour / colour-temperature axes the auto-ports carried: full-colour bulbs
# (Aj_Zigbee_Led_Strip / AJ_ZB_GU10 / AJ_ZIGPROA60 / ZB_A60_RGBCW / the Tuya
# AJ_RGBCCT_CTRL), CCT-only ZB-CCT_Filament, plus the AJ_RGBCCT_CTRL phantom
# battery channel that was removed.
add_executable(zhc_ajax_online_parity_tests tests/test_ajax_online_parity.cpp)
target_link_libraries(zhc_ajax_online_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the FrankEver FK_V02 Tuya-DP water valve. Pins the
# rewire from misrouted genOnOff to the 0xEF00 DP map (state/threshold/timer)
# and the timer seconds->minutes (/60) scale dropped by the auto-port.
add_executable(zhc_frankever_parity_tests tests/test_frankever_parity.cpp)
target_link_libraries(zhc_frankever_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Seastar Intelligence 020B0B (Fischer & Honsel LED
# Tischleuchte Beta Zig). z2m m.light({colorTemp:{range:[153,370]}, color:true})
# but the auto-port collapsed it to on/off + brightness, dropping the
# colorTemp + XY/HS colour axis (restored via kFzColorTemperature + kFzColor,
# bind 0x0300). Graduated to a parent override.
add_executable(zhc_seastar_intelligence_parity_tests tests/test_seastar_intelligence_parity.cpp)
target_link_libraries(zhc_seastar_intelligence_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Atsmart Z6 3-gang wall switch. z2m m.deviceEndpoints
# ({left:1,center:2,right:3}) + m.onOff({endpointNames:["left","center","right"]}).
# The auto-port bound genOnOff (0x0006) on endpoint 1 ONLY (center/right gangs
# unconfigured) and collapsed the per-gang exposes into a single bare "state"
# (dispatch suffixes onOff keys to state_<label> via the endpoint_map, so the
# bare key matched nothing). Graduated to a parent override that binds 0x0006 on
# endpoints 1/2/3 and declares state_left/state_center/state_right.
add_executable(zhc_atsmart_parity_tests tests/test_atsmart_parity.cpp)
target_link_libraries(zhc_atsmart_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Cleverio SA100 siren + SS300 temp/humidity sensor.
# SA100 swapped kFzIasZone for kFzIasZoneAlarmOnly (z2m fz.ias_alarm_only_alarm_1
# emits bare `alarm` only) and dropped phantom tamper/battery_low exposes.
# SS300 had temperature + humidity channels dropped (battery-only); restored
# kFzTemperature + kFzHumidity converters and exposes.
add_executable(zhc_cleverio_parity_tests tests/test_cleverio_parity.cpp)
target_link_libraries(zhc_cleverio_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Atlantic Group GW003-AS-IN-TE-FC AC interface. The
# generated def wired generic kFzThermostat + kFzFanMode but exposed
# occupied_cooling_setpoint (hvacThermostat attr 0x0011) and
# programming_operation_mode (attr 0x0025), neither of which kFzThermostat
# decodes — both were dead on read. Graduated to a Tier-2 override adding
# kFzAtlanticThermostatExtras. See tests/README.md for the pattern.
add_executable(zhc_atlantic_parity_tests tests/test_atlantic_parity.cpp)
target_link_libraries(zhc_atlantic_parity_tests PRIVATE zhc_definitions)

# Aubess ZXZIR-02 IR remote: the auto-generated def mis-classified this pure
# Zosung IR blaster/learner as a bare on/off + battery device (phantom
# kFzOnOff/kFzBattery, bindings to genOnOff/genPowerCfg). Graduated to a
# Tier-2 override wiring the existing Zosung IR runtime (clusters 0xED00 +
# 0xE004), matching the Moes UFO-R11 sibling but without battery.
add_executable(zhc_aubess_parity_tests tests/test_aubess_parity.cpp)
target_link_libraries(zhc_aubess_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for two Light Solutions bulbs whose auto-generated defs
# dropped a lighting axis. 42-032 (CCT driver) lost the colorTemp axis and
# 91-943 (RGBW+CCT controller) lost BOTH color (hue/sat/xy) and colorTemp.
# Both graduated to parent overrides wiring the generic color/colorTemp
# converters + 0x0300 binding. The plain dimmers/switch are guarded
# FALSE-by-category.
add_executable(zhc_lightsolutions_parity_tests tests/test_lightsolutions_parity.cpp)
target_link_libraries(zhc_lightsolutions_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Casaia CTHS-317-ET remote temperature probe. The
# generated def reduced it to battery-only (kFzBattery + battery/voltage),
# dropping z2m's fz.temperature decoder + e.temperature() expose (the whole
# device purpose) and the e.battery_low() expose. Graduated to a Tier-2
# override adding kFzTemperature + the temperature & battery_low exposes +
# the 0x0402 binding (ep3). See tests/README.md for the pattern.
add_executable(zhc_casaia_parity_tests tests/test_casaia_parity.cpp)
target_link_libraries(zhc_casaia_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Akuvox M423-9E temperature & humidity sensor (TS0201).
# The generated def reduced it to battery-only (kFzBattery + battery/voltage),
# dropping z2m's fz.temperature + fz.humidity decoders and the e.temperature()
# + e.humidity() exposes (the whole sensor purpose). Graduated to a Tier-2
# override adding kFzTemperature + kFzHumidity + the temperature & humidity
# exposes + the 0x0402 (ep1) / 0x0405 (ep2) bindings. See tests/README.md.
add_executable(zhc_akuvox_parity_tests tests/test_akuvox_parity.cpp)
target_link_libraries(zhc_akuvox_parity_tests PRIVATE zhc_definitions)
add_executable(zhc_spotmau_parity_tests tests/test_spotmau_parity.cpp)
target_link_libraries(zhc_spotmau_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Alchemy LED-luminaire family: both m.light()
# defs (AL8TC13W-AP tuneable-white, AL8RGB13W-AP RGBW) were auto-ported
# to on/off + brightness only, dropping the lightingColorCtrl (0x0300)
# colorTemp + colour axis. Graduated to Tier-2 overrides restoring
# kFzColorTemperature / kFzColor (+ matching Tz) and the color_temp /
# color_xy exposes.
add_executable(zhc_alchemy_parity_tests tests/test_alchemy_parity.cpp)
target_link_libraries(zhc_alchemy_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Heiman IAS-Zone sensor family: ~27 generated defs
# decoded the bare "alarm" key (generic kFzIasZone) while exposing the
# semantic key (smoke/gas/water_leak/occupancy/contact/vibration/co). All
# graduated to parent overrides using the typed kFzIas<Type>Alarm converters;
# HS1MIS-3.0 re-pointed at msOccupancySensing. See tests/README.md.
add_executable(zhc_heiman_parity_tests tests/test_heiman_parity.cpp)
target_link_libraries(zhc_heiman_parity_tests PRIVATE zhc_definitions)

# Oujiabao CR701-YZ gas + CO alarm — the auto-port lowered the generic
# kFzIasZone (bare "alarm") and a phantom "alarm" expose against z2m's
# fz.ias_carbon_monoxide_alarm_1_gas_alarm_2 (carbon_monoxide=bit0,
# gas=bit1). Graduated to a Tier-2 override wiring kFzIasCoAlarm +
# kFzIasGasAlarm2 and aligning exposes to carbon_monoxide/gas/tamper/
# battery_low.
add_executable(zhc_oujiabao_parity_tests tests/test_oujiabao_parity.cpp)
target_link_libraries(zhc_oujiabao_parity_tests PRIVATE zhc_definitions)

# HomeSeer DS150ZB door sensor — the auto-port wired kFzIasZone (bare "alarm")
# against z2m's iasZoneAlarm({zoneType:"contact"}) semantic+inverted "contact"
# key, and declared phantom alarm/tamper/voltage exposes. Repointed to typed
# kFzIasContactAlarm and aligned exposes to battery/contact/battery_low.
add_executable(zhc_homeseer_parity_tests tests/test_homeseer_parity.cpp)
target_link_libraries(zhc_homeseer_parity_tests PRIVATE zhc_definitions)
# Spacetronik ZB-DG02 gas leakage sensor — auto-ported as a generic IAS-zone
# (ssIasZone 0x0500) emitting bare alarm/tamper/battery_low; z2m decodes it via
# a Tuya 0xEF00 DP stream ([1,"gas",trueFalse0]). Graduated to a Tier-2 override
# wiring fz_tuya_datapoints. See tests/README.md.
add_executable(zhc_spacetronik_parity_tests tests/test_spacetronik_parity.cpp)
target_link_libraries(zhc_spacetronik_parity_tests PRIVATE zhc_definitions)

# Hommyn MS-20-Z / WS-20-Z — both auto-ports wired kFzIasZone (bare "alarm")
# against a semantic expose key. Repointed to typed kFzIasMotionAlarm
# (occupancy) / kFzIasWaterLeakAlarm (water_leak). See tests/README.md.
add_executable(zhc_hommyn_parity_tests tests/test_hommyn_parity.cpp)
target_link_libraries(zhc_hommyn_parity_tests PRIVATE zhc_definitions)

# EDP re:dy plug — z2m configures genOnOff + seMetering on getEndpoint(85)
# ("re:dy plug uses ep relay") but the auto-port bound those clusters on
# endpoint 1, so state/power/energy never reported. Repointed both bindings
# to endpoint 85. Metering stays 0x0702-only (z2m uses fz.metering, not
# fz.electrical_measurement). See tests/README.md.
add_executable(zhc_edp_parity_tests tests/test_edp_parity.cpp)
target_link_libraries(zhc_edp_parity_tests PRIVATE zhc_definitions)

# Prolight — white-and-colour bulbs (E27 5412748727371 / GU10 5412748727401)
# had the 0x0300 color/colorTemp axis dropped (color_temp + color_xy + bind);
# the remote (5412748727388) was mis-ported as a controllable on/off switch
# instead of an action-only command transmitter. See tests/README.md.
add_executable(zhc_prolight_parity_tests tests/test_prolight_parity.cpp)
target_link_libraries(zhc_prolight_parity_tests PRIVATE zhc_definitions)

# Plaid Systems Spruce PS-SPRZMS-SLP3 temp+moisture sensor — the auto-port
# dropped the temperature (0x0402) + humidity (0x0405) channels and added a
# phantom on/off bundle, reducing the moisture sensor to battery-only.
add_executable(zhc_plaid_parity_tests tests/test_plaid_parity.cpp)
target_link_libraries(zhc_plaid_parity_tests PRIVATE zhc_definitions)

# Bega LED luminaires — 13557 (E27 RGBW) + 85000 (Garden Spotlight RGBW)
# were z2m m.light({colorTemp, color}) but the auto-port dropped the
# 0x0300 lightingColorCtrl axis (color_temp/color_xy exposes + bind).
add_executable(zhc_bega_parity_tests tests/test_bega_parity.cpp)
target_link_libraries(zhc_bega_parity_tests PRIVATE zhc_definitions)

# MSH / MySmartHouse DIY ESP sensors — multi-channel restored. bme280psm
# (temp/humidity/pressure) and ds18b20psm (temp) were reduced to
# battery-only; AirQMon was mis-ported as a bare on/off switch instead of a
# co2 + temperature@l3 air-quality sensor. See tests/README.md.
add_executable(zhc_msh_parity_tests tests/test_msh_parity.cpp)
target_link_libraries(zhc_msh_parity_tests PRIVATE zhc_definitions)

# IMOU (Dahua) IAS battery-sensor family — dead-key / missing-channel
# fixes (ZP1 occupancy, ZR1/ZD1 alarm, ZGA1 gas split-bits + test, ZTM1
# temp/humidity, ZE1 button action). See tests/README.md.
add_executable(zhc_imou_parity_tests tests/test_imou_parity.cpp)
target_link_libraries(zhc_imou_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Hej (Goqual) family. The two KKZ IAS sensors
# lowered the generic kFzIasZone (bare key "alarm") while exposing the
# semantic key (contact/occupancy) — graduated to parent overrides using
# the typed kFzIasContactAlarm / kFzIasMotionAlarm converters. Also pins
# the GLSK multi-gang endpoint_map state-suffix path.
add_executable(zhc_hej_parity_tests tests/test_hej_parity.cpp)
target_link_libraries(zhc_hej_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Visonic (PowerMax/Tyco) IAS-Zone sensor family.
# All 8 generated defs lowered the generic kFzIasZone (bare key "alarm") while
# exposing the semantic key (occupancy/contact/vibration) — the primary sensor
# state never reached the shadow. Graduated to Tier-2 overrides using the typed
# kFzIas<Type>Alarm converters. The five temperature-capable models (MP-840,
# MCT-350/340 E/340 SMA/302 SMA) also dropped z2m's fz.temperature decoder +
# e.temperature() expose; restored kFzTemperature + the expose + 0x0402 binding.
add_executable(zhc_visonic_parity_tests tests/test_visonic_parity.cpp)
target_link_libraries(zhc_visonic_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the PEQ 3300-P door & window contact sensor. The auto-port
# lowered the generic kFzIasZone (emits the bare "alarm" key) so the contact
# state never matched the expose, and it dropped the z2m fz.temperature channel
# entirely (decoder + `temperature` expose + 0x0402 binding). Graduated to a
# parent override wiring the typed kFzIasContactAlarm (z2m-inverted contact)
# plus the restored kFzTemperature channel.
add_executable(zhc_peq_parity_tests tests/test_peq_parity.cpp)
target_link_libraries(zhc_peq_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the iHORN sensor family. Every IAS def lowered the generic
# kFzIasZone (bare key "alarm") while exposing the semantic key — graduated to
# Tier-2 overrides using typed kFzIasMotionAlarm (LH-992ZB/LH-990ZB/LH-990F →
# occupancy), kFzIasContactAlarm (HO-09ZB/LH03121 → contact), kFzIasSmokeAlarm
# (LH-09521 siren → smoke). The LH-32ZB T+H sensor was reduced to battery-only;
# restored kFzTemperature + kFzHumidity + exposes + 0x0402/0x0405 bindings.
add_executable(zhc_ihorn_parity_tests tests/test_ihorn_parity.cpp)
target_link_libraries(zhc_ihorn_parity_tests PRIVATE zhc_definitions)

# Parity for the Somgoms TS0601 / legacy-DP family (1-gang switch, dimmer, two
# curtain motors). All 4 defs were auto-ported onto generic standard-cluster
# converters (genOnOff / genLevelCtrl / windowCovering) but z2m wires them as
# Tuya-MCU devices on the 0xEF00 DP stream — every port was DEAD. Re-wired onto
# the Tuya-DP read+write infra; tests decode DP1 state / DP2+DP3 position /
# DP3 brightness frames through the dispatcher.
add_executable(zhc_somgoms_parity_tests tests/test_somgoms_parity.cpp)
target_link_libraries(zhc_somgoms_parity_tests PRIVATE zhc_definitions)
# Parity for the Ecolink sensors. The 4655BC0-R contact sensor lowered the
# generic kFzIasZone (bare "alarm") while the expose declared "contact";
# swapped in kFzIasContactAlarm (inverted contact). Both 4655BC0-R and
# FFZB1-SM-ECO dropped the temperature channel; restored kFzTemperature +
# expose + 0x0402 binding. FFZB1-SM-ECO keeps its correct bare "alarm" key.
add_executable(zhc_ecolink_parity_tests tests/test_ecolink_parity.cpp)
target_link_libraries(zhc_ecolink_parity_tests PRIVATE zhc_definitions)

# Roome contact sensor (HSC1-WD-0): pins the (c) IAS dead-key fix — kFzIasZone
# emitted the bare "alarm" key while z2m's fz.ias_contact_alarm_1 publishes the
# inverted semantic "contact" key. Now uses kFzIasContactAlarm + contact expose.
add_executable(zhc_roome_parity_tests tests/test_roome_parity.cpp)
target_link_libraries(zhc_roome_parity_tests PRIVATE zhc_definitions)

# Parity for the Fantem ZB003-X 4-in-1 multi sensor. Its IAS path lowered the
# generic kFzIasZoneStatusChange (bare "alarm_1") while the expose declared
# `occupancy`; z2m fz.ZB003X_occupancy maps zoneStatus bit0 -> occupancy, so
# the def was re-pointed at the typed kFzIasMotionAlarm. Tests pin the
# occupancy/tamper/battery_low IAS decode plus the Tuya-DP temp/humidity path.
add_executable(zhc_fantem_parity_tests tests/test_fantem_parity.cpp)
target_link_libraries(zhc_fantem_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the HZC Electric sensor family. The S093TH-ZG
# temp/humidity sensor was mis-ported as a phantom on/off switch (kFzOnOff +
# a `state` expose + kTzOnOff) — restored kFzTemperature + kFzHumidity, the
# temperature/humidity exposes and 0x0402 / 0x0405 bindings. The S900W-ZG
# water-leak and S902M-ZG motion sensors lowered the generic kFzIasZone (bare
# key "alarm") while exposing the semantic key — graduated to the typed
# kFzIasWaterLeakAlarm / kFzIasMotionAlarm (water_leak / occupancy). S902M
# also dropped z2m's m.illuminance() — restored kFzIlluminance + the
# illuminance expose + 0x0400 binding.
add_executable(zhc_hzc_electric_parity_tests tests/test_hzc_electric_parity.cpp)
target_link_libraries(zhc_hzc_electric_parity_tests PRIVATE zhc_definitions)

# Woolley BSD29/BSD59 smart plug. The generated port wired the generic
# kFzElectricalMeasurement (0x0B04) for power/voltage/current, but z2m's
# fzLocal.BSD29 decodes a manuSpecific cluster 0xFC11 (attrs 0x7006/0x7005/
# 0x7004, each /1000). Graduated + re-pointed at kFzWoolleyElectricity
# (cluster "manuSpecificWoolley"); dropped the phantom 0x0B04 binding.
add_executable(zhc_woolley_parity_tests tests/test_woolley_parity.cpp)
target_link_libraries(zhc_woolley_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the GS (Linkind-OEM) family. The 6 IAS sensors lowered
# the generic kFzIasZone (bare key "alarm") while exposing a semantic key —
# graduated to the typed kFzIas<Type>Alarm converters (occupancy/contact/
# smoke/water_leak/gas; the two gas sensors read alarm_2 bit 1 via
# kFzIasGasAlarm2). The SKHMP30-I1 socket dropped z2m electricityMeter()'s
# 0x0B04 half (power/voltage/current) — restored kFzElectricalMeasurement +
# voltage/current exposes + 0x0B04 binding. The two B*HM bulbs were collapsed
# to on/off+brightness — restored colorTemp (+ HS colour on BRHM) + 0x0300.
add_executable(zhc_gs_parity_tests tests/test_gs_parity.cpp)
target_link_libraries(zhc_gs_parity_tests PRIVATE zhc_definitions)

# Parity for the Hornbach (FLAIR Viyu) LED bulb family. All 14 generated defs
# wired only kFzOnOff + kFzBrightness (0x0006/0x0008), dropping the
# lightingColorCtrl (0x0300) axis that z2m's m.light({colorTemp, color?})
# exposes — every bulb lost color_temp, the six RGBW variants lost colour too.
# Graduated to two shared bundles (CTLight / ColorCTLight) in
# definitions/hornbach/_shared.cpp. See tests/README.md for the pattern.
add_executable(zhc_hornbach_parity_tests tests/test_hornbach_parity.cpp)
target_link_libraries(zhc_hornbach_parity_tests PRIVATE zhc_definitions)

# Parity for the Somfy roller-shutter / blind family. Ysia 5 (1871154) is a
# 5-channel remote whose port carried an endpoint_map but no
# endpoint_action_suffix, so all five buttons collided on a bare "action";
# added the flag so dispatch emits action_<n>. 1871215B plug uses z2m
# m.electricityMeter() (cluster:"both") but the port wired only kFzMetering,
# dropping the 0x0B04 electrical half (voltage/current) — added
# kFzElectricalMeasurement + exposes + bind.
add_executable(zhc_somfy_parity_tests tests/test_somfy_parity.cpp)
target_link_libraries(zhc_somfy_parity_tests PRIVATE zhc_definitions)

# Parity for the Chacon roller-shutter / on-off family. ZB-ERSM-01 wires
# m.windowCovering + m.commandsWindowCovering; the auto-port kept only the
# position half and dropped the action commands (closuresWindowCovering
# 0x00/0x01/0x02) — restored kFzCommandCoverOpen/Close/Stop + action expose.
add_executable(zhc_chacon_parity_tests tests/test_chacon_parity.cpp)
target_link_libraries(zhc_chacon_parity_tests PRIVATE zhc_definitions)

# Parity for the Samotech relay/dimmer + metering family. SM309-S, SM323_v2
# (SM323 / HK_DIM_A) and SM308-2CH all use z2m m.electricityMeter() with no
# args -> cluster:"both" + ac, decoding seMetering (0x0702: energy) AND
# haElectricalMeasurement (0x0B04: power/voltage/current). The auto-ports
# wired only kFzMetering, dropping voltage/current; added
# kFzElectricalMeasurement + the voltage/current exposes + 0x0B04 bind.
# SM308-2CH keeps its (correct) per-channel endpoint_map {l1,l2} and gains a
# 2nd-channel genOnOff bind. See tests/README.md for the pattern.
add_executable(zhc_samotech_parity_tests tests/test_samotech_parity.cpp)
target_link_libraries(zhc_samotech_parity_tests PRIVATE zhc_definitions)

# Parity for the Climax family. SCM-5ZBS roller shutter was wired to the
# closuresWindowCovering cluster (0x0102) it never speaks; z2m proxies
# position through genLevelCtrl + on/off (fz/tz cover_via_brightness) — both
# decode and control were dead, rewired to the new generic via-brightness
# converters. SRAC-23B-ZBSR siren wired kFzIasZone (attribute-only) against
# z2m fz.ias_siren (commandStatusChangeNotification) so `alarm` was dead;
# swapped to kFzIasGenericAlarm and regained the dropped ias_wd max_duration
# channel + warning/squawk/max_duration exposes. See tests/README.md.
add_executable(zhc_climax_parity_tests tests/test_climax_parity.cpp)
target_link_libraries(zhc_climax_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_blaupunkt_parity_tests tests/test_blaupunkt_parity.cpp)
target_link_libraries(zhc_blaupunkt_parity_tests PRIVATE zhc_definitions)

# Parity for the Lupus (LUPUSEC) family. The 12031 / LS12128 SCM-family roller
# shutters were ported against closuresWindowCovering (0x0102) — a cluster they
# never speak — leaving position decode and control dead; rewired to the
# via-brightness converters (genLevelCtrl 0x0008 + genOnOff 0x0006) and the
# z2m cover_position() `state` expose restored. Also pins the 12127 two-channel
# relay's l1/l2 endpoint_map key suffixing. See tests/README.md.
add_executable(zhc_lupus_parity_tests tests/test_lupus_parity.cpp)
target_link_libraries(zhc_lupus_parity_tests PRIVATE zhc_definitions)

# Parity for the Enbrighten (GE/Jasco) switch/outlet/dimmer family. Every
# device pairs its controllable surface with z2m commandsOnOff (+ dimmers:
# commandsLevelCtrl), which decode the device's physical button presses
# (genOnOff commandOn/commandOff, genLevelCtrl Move/Stop) into an `action`
# enum. The auto-generated ports kept only the controllable half and dropped
# that action channel; this graduates all four to Tier-2 overrides wiring
# kFzCommandOn/Off (+ kFzCommandMove/Stop for dimmers) and an `action`
# expose. electricityMeter(cluster:"metering") stays 0x0702-only (no
# voltage/current). See tests/README.md.
add_executable(zhc_enbrighten_parity_tests tests/test_enbrighten_parity.cpp)
target_link_libraries(zhc_enbrighten_parity_tests PRIVATE zhc_definitions)

# Parity for the Lonsonho (Tuya-OEM) family. X701A (1-gang switch) dropped
# the "_TZ3000_ehgouyvu" manufacturer name from its TS0001 fingerprint, so
# that variant never matched; graduated to a parent override restoring both
# names. The rest are structural guards (multi-gang endpoint_maps,
# attribute-based TS130F covers on 0x0102, RGBCW colour surface; Tuya-DP
# switches/dimmer surfaces are INFRA-deferred). See tests/README.md.
add_executable(zhc_lonsonho_parity_tests tests/test_lonsonho_parity.cpp)
target_link_libraries(zhc_lonsonho_parity_tests PRIVATE zhc_definitions)

# Parity for the Quotra LED family — the QV-RGBCCT strip (B07JHL6DRV) had
# its colour axes dropped by the auto-port (m.light({colorTemp,color}) ->
# on/off+brightness only); graduated to restore color_temp/color_xy +
# the lightingColorCtrl bind. The B07CVL9SZF dimmer is pinned colourless.
add_executable(zhc_quotra_parity_tests tests/test_quotra_parity.cpp)
target_link_libraries(zhc_quotra_parity_tests PRIVATE zhc_definitions)

# Parity for the SLV (VALETO) family — the four LED luminaires had their
# colour axes dropped by the auto-port (m.light({colorTemp[,color]}) ->
# on/off+brightness only); the VALETO remote carried a phantom
# state/battery/voltage bundle while z2m declares exposes:[].
add_executable(zhc_slv_parity_tests tests/test_slv_parity.cpp)
target_link_libraries(zhc_slv_parity_tests PRIVATE zhc_definitions)

# Parity for the Honyar (Tuya-OEM) family. The three multi-gang switches
# (HY0096 2-gang, HY0097 + 00500c35 3-gang) declared multiple z2m switch
# endpoints but the generated defs carried a single bare `state` expose and
# bound genOnOff only on EP1, so the gangs collided on the bare key and the
# upper endpoints were never bound. Graduated to Tier-2 overrides with
# per-gang suffixed exposes + per-endpoint bindings + endpoint_map. The
# HY0157 dual-socket metering plug is INFRA-deferred (multiEndpointSkip on
# the metering keys has no per-def runtime hook). See tests/README.md.
add_executable(zhc_honyar_parity_tests tests/test_honyar_parity.cpp)
target_link_libraries(zhc_honyar_parity_tests PRIVATE zhc_definitions)

# Parity for the MakeGood (Tuya-OEM) family. MG-AUZG01 (TS011F) is a metering
# double power point whose z2m tuyaOnOff({electricalMeasurements:true}) was not
# expanded by the auto-port, dropping the whole power/energy/voltage/current
# surface (0x0702 + 0x0B04); graduated to a parent override wiring the generic
# seMetering + haElectricalMeasurement decoders + exposes + bindings. The
# dual-endpoint (l1/l2) split is INFRA-deferred (multiEndpointSkip metering has
# no per-def runtime hook), same as Honyar HY0157. MG-GPO01 was already correct.
add_executable(zhc_makegood_parity_tests tests/test_makegood_parity.cpp)
target_link_libraries(zhc_makegood_parity_tests PRIVATE zhc_definitions)

# Parity for the GE (Jasco) light + switch + metering family. The one real
# gap: PTAPT-WH02 declares z2m endpoint default:2 (genOnOff lives on EP2);
# the auto-port bound onOff on EP1 with default_endpoint=0, so on/off frames
# routed to the wrong endpoint (dead toggle). Fixed by re-endpointing the
# binding ep1->ep2 + setting default_endpoint=2. Regression guards: the GE
# metering devices are seMetering 0x0702 only (fz.metering /
# electricityMeter cluster:"metering") with NO 0x0B04 voltage/current half;
# bulbs/dimmers are plain m.light() = on/off + brightness, no color_temp.
add_executable(zhc_ge_parity_tests tests/test_ge_parity.cpp)
target_link_libraries(zhc_ge_parity_tests PRIVATE zhc_definitions)

# Parity for the Sylvania (OSRAM Lightify US) family. The 74388 contact +
# temperature sensor lowered the generic kFzIasZone converter (bare "alarm"
# key) while z2m's fz.ias_contact_alarm_1 publishes the semantic key
# "contact"; graduated to a parent override swapping in kFzIasContactAlarm
# and renaming the expose. The temperature axis (msTemperatureMeasurement
# 0x0402) was already wired. Everything else (ledvanceLight/ledvanceOnOff
# bulbs/strips/plug, the 73743 action remote) is structurally correct.
# See tests/README.md.
add_executable(zhc_sylvania_parity_tests tests/test_sylvania_parity.cpp)
target_link_libraries(zhc_sylvania_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the AwoX / EGLO non-light family. The EPIR_Zm PIR
# motion sensor dropped its `occupancy` channel (shipped state/battery
# only); the ERCU_WS_Zm wall remote (multi-EP) and 33952 (ERCU_Zm) remote
# were both mis-ported as dead on/off switches. Graduated to parent
# overrides wiring kFzOccupancy and the generic command-action decoders.
add_executable(zhc_awox_parity_tests tests/test_awox_parity.cpp)
target_link_libraries(zhc_awox_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the EGLO connect family (an AwoX rebadge). Five bulbs
# were lowered onto on/off + brightness only, dropping the colorTemp /
# colour channels; the 99099 + 99106 remotes were mis-ported as on/off
# switches with a dead `state`. Graduated to Tier-2 parents: eglo/_shared CT
# + full-RGBW light bundles, and the generic command-action decoders for the
# remotes.
add_executable(zhc_eglo_parity_tests tests/test_eglo_parity.cpp)
target_link_libraries(zhc_eglo_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the eCozy 1TST-EU thermostat. z2m's fz.thermostat
# decodes pi_heating_demand (0x0008), local_temperature_calibration (0x0010)
# and running_state (0x0029), but the generic kFzThermostat only handles
# 0x0000/0x0012/0x001C — those three keys were exposed-but-dead. Graduated
# to a Tier-2 parent with kFzEcozyThermostatExtras + the real climate-flat
# exposes.
add_executable(zhc_ecozy_parity_tests tests/test_ecozy_parity.cpp)
target_link_libraries(zhc_ecozy_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Adeo (ENKI LEXMAN) sensor + key-fob family. Six IAS
# sensors lowered the generic kFzIasZone behind a semantic expose (dead key);
# the 4-in-1 (83633205) additionally dropped its illuminance/temp/humidity
# channels; the LDSENK09 key fob was mis-ported as an on/off relay. Graduated
# to parent overrides with typed kFzIas<Type>Alarm / kFzIasGenericAlarm,
# kFzIasAceArm + kFzIasAcePanic, and the standard measurement decoders.
add_executable(zhc_adeo_parity_tests tests/test_adeo_parity.cpp)
target_link_libraries(zhc_adeo_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_evology_parity_tests tests/test_evology_parity.cpp)
target_link_libraries(zhc_evology_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_philio_parity_tests tests/test_philio_parity.cpp)
target_link_libraries(zhc_philio_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_airam_parity_tests tests/test_airam_parity.cpp)
target_link_libraries(zhc_airam_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_net2grid_parity_tests tests/test_net2grid_parity.cpp)
target_link_libraries(zhc_net2grid_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the EnOcean PTM 215Z / 215ZE / 216Z Green Power
# pushbutton transmitters. All three were mis-ported as controllable on/off
# relays (kFzOnOff/kTzOnOff + dead `state` + genOnOff bind); each now exposes
# the semantic `action` enum, is non-controllable, and binds nothing. The
# action decoder stays a documented INFRA defer (no Green Power frame family).
add_executable(zhc_enocean_parity_tests tests/test_enocean_parity.cpp)
target_link_libraries(zhc_enocean_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the LED-Trading family. The 9133 "Pushbutton
# transmitter module" is a self-powered Green Power transmitter that was
# mis-ported as a controllable on/off relay (kFzOnOff/kTzOnOff + dead `state`
# + genOnOff bind); it now exposes the semantic `action` enum, is
# non-controllable, and binds nothing (action decoder stays a documented
# Green Power INFRA defer, as for enocean). The 9134 5-gang powerstrip,
# 9135 cover and HK-LN-DIM-A dimmer are pinned as parity-correct regressions.
add_executable(zhc_led_trading_parity_tests tests/test_led_trading_parity.cpp)
target_link_libraries(zhc_led_trading_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Woox (Tuya-OEM) family. The R7051 TS0219 siren
# decodes its alarm via z2m fz.ias_alarm_only_alarm_1 (ssIasZone attr 0x0002
# bit 0 -> `alarm` ONLY), but the generated def lowered the broader kFzIasZone
# (which also emits tamper + battery_low) and declared phantom tamper /
# battery_low exposes z2m never surfaces. Graduated to a parent override wiring
# the new generic kFzIasZoneAlarmOnly with the two phantom exposes removed.
# The Tuya-DP devices (R7049 smoke, R7060 irrigation, R7067 TRV) are pinned
# against the z2m legacy semantics. See tests/README.md.
add_executable(zhc_woox_parity_tests tests/test_woox_parity.cpp)
target_link_libraries(zhc_woox_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the Linkind family. The ZS230002 5-key smart-bulb
# dimmer remote wired the wrong color converters: kFzCommandStepColorTemp
# (cmd 0x4C, a z2m converter not in this bundle) and
# kFzCommandMoveToHueAndSaturation (cmd 0x06, a phantom action). z2m uses
# fz.command_move_color_temperature (cmd 0x4B -> color_temperature_move_*)
# and fz.command_move_to_color (cmd 0x07 -> color_move); both generic
# converters already exist and are now wired. Guards the IAS sensors
# (typed bit-0 keys) and the senders (action, no settable state) too.
add_executable(zhc_linkind_parity_tests tests/test_linkind_parity.cpp)
target_link_libraries(zhc_linkind_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for Universal Electronics Inc (One For All / Xfinity). The
# XHK1-UE + UEHK2AZ0 security keypads decode ssIasAce commandArm via the new
# kFzIasAceArmWithTransaction converter (mirrors z2m
# fz.command_arm_with_transaction): the ZCL transaction sequence number now
# reaches the declared `action_transaction` expose. Also pins the XHS1/XHS2
# typed IAS bit-0 sensor keys.
add_executable(zhc_universal_electronics_inc_parity_tests tests/test_universal_electronics_inc_parity.cpp)
target_link_libraries(zhc_universal_electronics_inc_parity_tests PRIVATE zhc_definitions)

# Technicolor XHK1-TC — Xfinity security keypad, sibling of XHK1-UE. The
# auto-port wired phantom kFzOnOff + the generic dead-key kFzIasZone and dropped
# temperature; rewired to typed IAS occupancy/contact + kFzIasAceArmWithTransaction
# so the declared `action_transaction` (ZCL TSN) reaches the shadow.
add_executable(zhc_technicolor_parity_tests tests/test_technicolor_parity.cpp)
target_link_libraries(zhc_technicolor_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the eWeLink (Coolkit / Sonoff-OEM) vendor. The WS01
# rainfall sensor reports rain over ssIasZone commandStatusChangeNotification
# (zoneStatus bit 0); z2m maps it via fzLocal.WS01_rain -> {rain}. Its
# generated port declared the `rain` expose but wired the generic
# kFzIasZoneStatusChange (which only emits alarm_1/tamper/battery_low), so the
# expose stayed empty. Graduated to a parent override wiring the new typed
# kFzIasRainAlarm. Guards SNZB-05 (typed water_leak) and CK-BL702-ROUTER
# (z2m exposes:[] -> no phantom on/off) too. See tests/README.md.
add_executable(zhc_ewelink_parity_tests tests/test_ewelink_parity.cpp)
target_link_libraries(zhc_ewelink_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Immax (Neo) 07046L 4-Touch key fob. It was generated
# with kFzCommandRecall (genScenes 0x0005), but the device sends IAS ACE
# (ssIasAce 0x0501) commandArm/commandPanic — so no action ever decoded.
# Graduated to a Tier-2 parent wiring kFzIasAceArm + kFzIasAcePanic and
# binding 0x0501. See tests/README.md.
add_executable(zhc_immax_parity_tests tests/test_immax_parity.cpp)
target_link_libraries(zhc_immax_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Iluminize (German lighting) non-trivial ports. The
# 5128.10 roller-shutter relay is a brightness cover in z2m
# (cover_position_via_brightness + cover_state_via_onoff), but the auto-gen
# port bound only the generic kFzCoverPosition (closuresWindowCovering) and
# exposed `position` alone, so a real device reporting on genLevelCtrl/genOnOff
# decoded to nothing and never surfaced state. The 5715/5717 metering dimmers
# use m.electricityMeter() (cluster:"both") but dropped the entire
# haElectricalMeasurement (0x0B04) half — current/voltage never decoded.
# Graduated with vendor cover-via-brightness/onoff converters + a state expose,
# and kFzElectricalMeasurement + current/voltage exposes. See tests/README.md.
add_executable(zhc_iluminize_parity_tests tests/test_iluminize_parity.cpp)
target_link_libraries(zhc_iluminize_parity_tests PRIVATE zhc_definitions)

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

# Parity fixture for the LivingWise LVS series. The IAS contact + occupancy
# sensors were generated with the generic kFzIasZone (bare `alarm` key)
# while exposing the semantic contact/occupancy keys (dead IAS key); the
# LVS-SC7 scene controller was generated as a controllable on/off switch
# but is a stateless orvibo_raw_2 button remote. Graduated to Tier-2 parents
# wiring kFzIasContactAlarm / kFzIasMotionAlarm / kFzOrviboRaw2Action.
add_executable(zhc_livingwise_parity_tests tests/test_livingwise_parity.cpp)
target_link_libraries(zhc_livingwise_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_linptech_parity_tests tests/test_linptech_parity.cpp)
target_link_libraries(zhc_linptech_parity_tests PRIVATE zhc_definitions)

# Parity fixture for Profalux cover motors + remotes. NB102 is an empty
# remote (was hallucinated with a phantom on/off + battery bundle);
# MOT-C1ZxxC/F is a lift+tilt cover on EP2 (was on/off + dropped tilt);
# NSAV061 is a genLevelCtrl-driven cover (was raw `brightness`, position
# decoded to nothing). See tests/README.md.
add_executable(zhc_profalux_parity_tests tests/test_profalux_parity.cpp)
target_link_libraries(zhc_profalux_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Mercator Ikuü SSWF01G AC fan controller. z2m's
# fz.fan decodes hvacFanCtrl attr 0x0000 (fanMode) into BOTH `fan_mode` and
# `fan_state`; the generated port wired the generic kFzFanMode (fan_mode only)
# while declaring a `fan_state` expose, leaving that key dead. Graduated to a
# Tier-2 parent with an inlined vendor converter (kFzMercatorFan) emitting
# fan_mode (u8) + fan_state (Bool). See tests/README.md.
add_executable(zhc_mercator_parity_tests tests/test_mercator_parity.cpp)
target_link_libraries(zhc_mercator_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Mill (Norwegian electric heating) family. Both the
# Mill-gen-4 WiFi heating panel and the MFTWIFI smart floor thermostat are z2m
# m.thermostat() devices (hvacThermostat 0x0201) but were auto-ported as bare
# genOnOff switch + battery. Re-wired to kFzThermostat/kTzThermostat; the test
# pins the flat thermostat shape + 0x0000/0x0012/0x001C decoders.
add_executable(zhc_mill_parity_tests tests/test_mill_parity.cpp)
target_link_libraries(zhc_mill_parity_tests PRIVATE zhc_definitions)
# Hampton Bay ceiling-fan + tunable-white light parity. #99432 derives
# fan_state from hvacFanCtrl (inlined kFzHamptonFan); #54668161 restores the
# dropped color_temp (lightingColorCtrl) axis. See tests/README.md.
add_executable(zhc_hampton_bay_parity_tests tests/test_hampton_bay_parity.cpp)
target_link_libraries(zhc_hampton_bay_parity_tests PRIVATE zhc_definitions)

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

# Parity fixtures for the D-Link DCH-B112 door/window + vibration sensor.
# The auto port wired the generic kFzIasZone (bare `alarm`, bit0 not
# inverted) and dropped the contact + vibration channels. z2m's
# fzLocal.DCH_B112 decodes the ZoneStatusChangeNotification into
# contact=!(bit0) / vibration=bit1 / tamper=bit2 / battery_low=bit3.
add_executable(zhc_dlink_parity_tests tests/test_dlink_parity.cpp)
target_link_libraries(zhc_dlink_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Databyte (databyte.ch) family. Touch4
# (DTB-ED2011-014) dropped the fzLocal.DTB2011014 decoder (battery-only)
# so key_1..key_4 (genOnOff manufacturer attrs 0xA191..0xA194) never
# surfaced. DTB190502A1 (CC2530 IO board) fabricated a controllable
# on/off `state` + phantom battery bundle and dropped fz.DTB190502A1
# (led_state/key_state/cpu_temperature). Both graduated to parent
# overrides wiring definitions/databyte/_shared.cpp converters.
add_executable(zhc_databyte_parity_tests tests/test_databyte_parity.cpp)
target_link_libraries(zhc_databyte_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Frient family (Develco rebrand, same silicon).
# WISZB-131 lowered the generic kFzIasZone (bare "alarm") + dropped the
# msTemperatureMeasurement channel; SMRZB-153 + EMIZB-151 wired only
# kFzMetering (0x0702) and dropped the 0x0B04 electrical half
# (voltage/current). All graduated to parent overrides mirroring develco.
add_executable(zhc_frient_parity_tests tests/test_frient_parity.cpp)
target_link_libraries(zhc_frient_parity_tests PRIVATE zhc_definitions)

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

# Parity fixtures for the KAMI (Yi) N20 contact/motion sensor: graduated
# from a phantom on/off + generic kFzIasZone port to vendor raw byte-7
# decoders (kami::kFzKamiContact -> contact, kami::kFzKamiOccupancy ->
# action "motion"), matching z2m fz.KAMI_contact / fz.KAMI_occupancy.
add_executable(zhc_kami_parity_tests tests/test_kami_parity.cpp)
target_link_libraries(zhc_kami_parity_tests PRIVATE zhc_definitions)

# jxuan — IAS dead-key (PRZ01 occupancy / DSZ01 contact) + WSZ01 phantom
# on/off -> button action (manuSpecific 0xFE05 attr 1) fixes.
add_executable(zhc_jxuan_parity_tests tests/test_jxuan_parity.cpp)
target_link_libraries(zhc_jxuan_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_sengled_parity_tests tests/test_sengled_parity.cpp)
target_link_libraries(zhc_sengled_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Halo Smart Labs HALO / HALO+ smoke + CO
# detectors. Both defs were battery+on/off stubs that dropped the whole
# safety-sensor set; the Halo IAS demux converter (kFzHaloIasZone) already
# existed in _shared.cpp but was never wired, and temperature/humidity/
# pressure (standard clusters) plus the EP2 colour light were missing.
add_executable(zhc_halo_parity_tests tests/test_halo_parity.cpp)
target_link_libraries(zhc_halo_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_halemeier_parity_tests tests/test_halemeier_parity.cpp)
target_link_libraries(zhc_halemeier_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Datek Eva AMS HAN power-meter (HSE2905E "Meter
# Reader"). z2m stacks two m.electricityMeter bundles (metering +
# producedEnergy, electrical + threePhase) but the generated port wired
# only the generic kFzMetering/kFzElectricalMeasurement (phase-A core), so
# produced_energy, phase B/C voltage/current, ac_frequency and
# power_factor were dead exposes. Added Datek kFzMeteringExtras +
# kFzElectricalMeasurementExtras (definitions/datek/_shared.cpp) and
# graduated the def to a Tier-2 parent.
add_executable(zhc_datek_parity_tests tests/test_datek_parity.cpp)
target_link_libraries(zhc_datek_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Danalock V3-BTZB/V3-BTZBE smartlock: wires the
# generic kFzLockOperationEvent / kFzLockProgrammingEvent action stream +
# kTzLockPinCode write that the generated port dropped.
add_executable(zhc_danalock_parity_tests tests/test_danalock_parity.cpp)
target_link_libraries(zhc_danalock_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the Schlage BE468 Connect smart deadbolt. The auto-port
# wired only kFzLock + kFzBattery and dropped the z2m lock-action stream
# (fz.lock_operation_event / fz.lock_programming_event -> action / action_user
# / action_source / action_source_name) plus tz.pincode_lock -> pin_code.
# Restored the generic converters + exposes (kwikset precedent) and corrected
# lock_state Binary->Enum; graduated the def to a Tier-2 parent.
add_executable(zhc_schlage_parity_tests tests/test_schlage_parity.cpp)
target_link_libraries(zhc_schlage_parity_tests PRIVATE zhc_definitions)

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

# Parity fixture for the Futurehome TS0601 thermostat (Co020 / FH-clones).
# The hand-ported Tier-2 DP map aggregated z2m's datapoints but dropped DP 104
# running_state (z2m lookup{idle:false, heat:true}, exposed via
# withRunningState) — a declared-but-dead channel. Restored as a Bool→string
# lookup DP (kTuyaDpFlagBoolEnum) + running_state expose.
add_executable(zhc_futurehome_parity_tests tests/test_futurehome_parity.cpp)
target_link_libraries(zhc_futurehome_parity_tests PRIVATE zhc_definitions)

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

# Parity fixtures for the Siglis zigfred plus (ZFP-1A-CH) / uno
# (ZFU-1D-CH) wall controllers. The hand-rewrite left EP5 (`l1`,
# zigfredEndpoint) out of the endpoint_map, so the front-surface RGB
# LED's reports never picked up the `_l1` suffix to match the
# state_l1/brightness_l1/color_l1 exposes; and button presses on the
# manuSpecificSiglisZigfred cluster (0xFC42 cmd 0x02) were undecoded.
# Both fixed: l1 mapped + siglis::kFzZigfredButtonEvent decodes `action`.
add_executable(zhc_siglis_parity_tests tests/test_siglis_parity.cpp)
target_link_libraries(zhc_siglis_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Inovelli Blue-series switches/dimmers/fan
# (VZM30/31/32/35-SN). z2m marks them supportsButtonTaps:true, emitting a
# manuSpecificInovelli (0xFC31 cmd 0x00) raw frame on EP2 → action
# <button>_<click>. The generated ports dropped both the action expose
# and the decoder; inovelli::kFzInovelliSceneAction restores them.
add_executable(zhc_inovelli_parity_tests tests/test_inovelli_parity.cpp)
target_link_libraries(zhc_inovelli_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Jasco / GE 43095 command_on_state / command_off_state
# fix (genOnOff commands → state) + 43132 metering-only (0x0702) guard.
add_executable(zhc_jasco_parity_tests tests/test_jasco_parity.cpp)
target_link_libraries(zhc_jasco_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Easyiot ZB-WB01/02/03/08 multi-button scene
# remotes. Their auto-generated defs lowered the generic
# kFzCommandOn/Off/Toggle (emit bare "on"/"off"/"toggle", no button
# prefix), but z2m's easyiot_action remaps the verbs (toggle->single,
# on->double, off->long) AND prefixes the button from src_endpoint. All
# four graduated to Tier-2 parents wiring the vendor kFzEasyiotAction.
add_executable(zhc_easyiot_parity_tests tests/test_easyiot_parity.cpp)
target_link_libraries(zhc_easyiot_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for Echostar "SAGE by Hughes" remotes. Both SAGE devices
# are battery controllers that EMIT genOnOff On/Off commands as an `action`
# (z2m toZigbee empty), but the auto-port mis-modelled both as a settable
# on/off `state` (kFzOnOff + kTzOnOff) — a dead control — losing the action
# stream. SAGE206611 -> generic kFzCommandOn/Off ("on"/"off"); SAGE206612
# (doorbell) -> vendor kFzSageBellAction (commandOn->"bell1"/off->"bell2").
add_executable(zhc_echostar_parity_tests tests/test_echostar_parity.cpp)
target_link_libraries(zhc_echostar_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for ROBB (ROBB Smarrt / rebranded Sunricher). The
# auto-generated port misclassified several devices: the 4-button wireless
# switches (081/084) became controllable on/off relays (dead `state` +
# kTzOnOff) instead of `action`-emitting command clients; the PIR sensor
# (070) dropped occupancy/temperature/humidity/illuminance; the metering
# plugs (017-0/017-1) dropped their internal temperature; and the color
# remotes (016 RGB-CCT, 018 knob) were missing or mis-wired lightingColorCtrl
# command decoders. All graduated to Tier-2 parents wiring the correct
# generic converters.
add_executable(zhc_robb_parity_tests tests/test_robb_parity.cpp)
target_link_libraries(zhc_robb_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_repenic_ltd_parity_tests tests/test_repenic_ltd_parity.cpp)
target_link_libraries(zhc_repenic_ltd_parity_tests PRIVATE zhc_definitions)

# Paul Neuhaus (Q-Home). The two remotes (100.462.31 Q-REMOTE, E0040006
# JZ-RC-J4R) skipped / mis-substituted the lightingColorCtrl colour-wheel
# actions (color_loop_set, enhanced_move_to_hue_and_saturation). Those
# generic decoders exist now; wired them so the enum actions surface.
add_executable(zhc_paul_neuhaus_parity_tests tests/test_paul_neuhaus_parity.cpp)
target_link_libraries(zhc_paul_neuhaus_parity_tests PRIVATE zhc_definitions)

# EcoDim (Dutch dimmers + wall remotes). The multi-button wall remotes
# (ED-10011/12/13/14/15) emitted a bare `action` (a kAlwaysGlobalKey),
# collapsing every button-pair onto one key. Set endpoint_map +
# endpoint_action_suffix so each keeps identity (action_<n>) — same fix as
# robb / vesternet. ED-10010 stays unsuffixed (z2m has no _n suffix there).
add_executable(zhc_ecodim_parity_tests tests/test_ecodim_parity.cpp)
target_link_libraries(zhc_ecodim_parity_tests PRIVATE zhc_definitions)

# Vesternet (rebranded Sunricher). The multi-endpoint scene remotes (REM-013
# 12-button, WAL-011 4-button, WAL-012 8-button) emitted a bare `action`
# (a kAlwaysGlobalKey), collapsing every button onto one key. Set
# endpoint_action_suffix + endpoint_map so each button keeps identity
# (action_<n>) — same fix as the robb rebrand of the same hardware.
add_executable(zhc_vesternet_parity_tests tests/test_vesternet_parity.cpp)
target_link_libraries(zhc_vesternet_parity_tests PRIVATE zhc_definitions)

# Dresden Elektronik scene-remote parity. BN-600085 ("Scene Switch") is a
# 3-part battery scene remote: z2m wires m.commandsScenes(), so genScenes
# (0x0005) recall/store presses surface as action "recall_<scene>" /
# "store_<scene>". The port dropped both scene decoders (stale "no generic
# decoder" comment — kFzCommandRecall/kFzCommandStore already exist), so every
# scene button was silent. Wired both; remote is single-EP so actions stay bare.
add_executable(zhc_dresden_elektronik_parity_tests tests/test_dresden_elektronik_parity.cpp)
target_link_libraries(zhc_dresden_elektronik_parity_tests PRIVATE zhc_definitions)

# Gewiss family parity fixtures. GWA1201 used m.electricityMeter() (default
# cluster:"both") but the port wired only kFzMetering (0x0702), dropping the
# 0x0B04 electrical half (voltage/current) — added kFzElectricalMeasurement +
# exposes + bind. GWA1502 (mains 230V contact) was mis-ported as a phantom
# battery + phantom on/off; it is actually two genBinaryInput.presentValue
# channels — rewired to the new generic kFzBinaryInput on ep1/ep2 (no battery,
# no on/off). GWA1501 kept its battery but dropped the same two input channels
# — added kFzBinaryInput on both endpoints. See tests/README.md.
add_executable(zhc_gewiss_parity_tests tests/test_gewiss_parity.cpp)
target_link_libraries(zhc_gewiss_parity_tests PRIVATE zhc_definitions)

# Aeotec family parity fixtures. WG001 ("Range extender Zi") is a router
# (z2m exposes:[]) but was mis-ported as a controllable on/off — stripped to
# exposeless. ZGA002/ZGA003 used m.electricityMeter() (default cluster:"both")
# but the port wired only kFzMetering (0x0702), dropping the 0x0B04 electrical
# half (voltage/current) — added kFzElectricalMeasurement + exposes + bind.
# ZGA004 ("Pico shutter") used m.windowCovering({controls:["lift","tilt"]}) but
# the port wired only kFzCoverPosition (lift), dropping tilt (0x0009) — added
# kFzCoverTilt + kTzCoverPositionTilt + a tilt expose. All three ZGA* also
# dropped m.deviceTemperature() (0x0002) and the m.commands* scene-button
# endpoints — added kFzDeviceTemperature + kFzCommand* + action_<n> suffixing.
add_executable(zhc_aeotec_parity_tests tests/test_aeotec_parity.cpp)
target_link_libraries(zhc_aeotec_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_netvox_parity_tests tests/test_netvox_parity.cpp)
target_link_libraries(zhc_netvox_parity_tests PRIVATE zhc_definitions)

# Konke family parity fixtures. The 2AJZ4KPFT and KK-ES-J01W sensors were
# auto-ported as battery-only — the generator dropped every msTemperature /
# msRelativeHumidity / msIlluminance channel z2m surfaces. Restored the
# generic temperature/humidity/illuminance decoders + exposes. The button
# (kFzKonkeAction) and 4-key scene switch (kFzKonkeSceneAction) action fixtures
# pin the pre-existing custom converters.
add_executable(zhc_konke_parity_tests tests/test_konke_parity.cpp)
target_link_libraries(zhc_konke_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_multir_parity_tests tests/test_multir_parity.cpp)
target_link_libraries(zhc_multir_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_kwikset_parity_tests tests/test_kwikset_parity.cpp)
target_link_libraries(zhc_kwikset_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_weiser_parity_tests tests/test_weiser_parity.cpp)
target_link_libraries(zhc_weiser_parity_tests PRIVATE zhc_definitions)

# Paulmann remote family (501.37/501.34/500.67/501.40/501.41): all five are
# battery scene/RGB remotes (toZigbee: []) that the generator wrong-bundled
# as settable on/off lights, killing every button press. Graduated each def,
# wired the genOnOff/genLevelCtrl/lightingColorCtrl/genScenes command
# decoders + an `action` expose, dropped the phantom state, and added
# endpoint_map + endpoint_action_suffix for the multi-button models.
add_executable(zhc_paulmann_parity_tests tests/test_paulmann_parity.cpp)
target_link_libraries(zhc_paulmann_parity_tests PRIVATE zhc_definitions)

# EVN: ZB24100VS multicolor controller dropped the entire lightingColorCtrl
# (0x0300) colour + colour-temperature axis (ported as on/off+brightness
# only); the ZBHS4RGBW 4-channel RGBW remote was wrong-bundled as a settable
# on/off switch (dead action). Color/CT restored on the light; the remote
# rewired to genOnOff/genLevelCtrl/lightingColorCtrl command decoders + an
# `action` expose with endpoint_map {ep1..ep4} + endpoint_action_suffix.
add_executable(zhc_evn_parity_tests tests/test_evn_parity.cpp)
target_link_libraries(zhc_evn_parity_tests PRIVATE zhc_definitions)

# Lutron: both ZHA entries (LZL4BWHL01 Connected Bulb Remote, Z3-1BRL Aurora
# rotary dimmer) are battery remotes that drive a paired bulb via genLevelCtrl
# commands (Step/MoveToLevel/Stop) with toZigbee: []. The generator
# wrong-bundled both as settable on/off lights, killing every action. Graduated
# each def, wired the genLevelCtrl command decoders + an `action` expose,
# dropped the phantom state + toolbox; kept Z3-1BRL battery.
add_executable(zhc_lutron_parity_tests tests/test_lutron_parity.cpp)
target_link_libraries(zhc_lutron_parity_tests PRIVATE zhc_definitions)

# The Light Group (SLC SmartOne): lighting vendor whose two remotes (S57003
# 4-channel wall remote, S57007 3-button remote) were wrong-bundled as
# settable on/off lights. Graduated to genOnOff/genLevelCtrl/genScenes command
# decoders + an `action` expose, dropped the phantom state, added endpoint_map
# + endpoint_action_suffix so each button keeps identity (action_<n>).
add_executable(zhc_the_light_group_parity_tests tests/test_the_light_group_parity.cpp)
target_link_libraries(zhc_the_light_group_parity_tests PRIVATE zhc_definitions)

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

# Parity fixture for the SMaBiT (Bitron Video) AV2010/33 vibration sensor,
# whose generated def read zoneStatus bit 0 (kFzIasVibrationAlarm) while z2m
# wires fz.ias_occupancy_alarm_2 (bit 1) — so vibration never fired. Graduated
# to a Tier-2 parent override using the new bit-1 variant kFzIasVibrationAlarm2;
# the four bit-0 vibration sensors are unaffected. See tests/README.md.
add_executable(zhc_bitron_parity_tests tests/test_bitron_parity.cpp)
target_link_libraries(zhc_bitron_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_brimate_parity_tests tests/test_brimate_parity.cpp)
target_link_libraries(zhc_brimate_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the TP-Link IAS-Zone sensors (MS100 motion, CS100 contact).
# Both generated defs were lowered with the generic kFzIasZone (bare `alarm`),
# leaving the semantic occupancy/contact exposes dead, and MS100 dropped its
# m.illuminance() channel. Graduated to Tier-2 overrides using the typed
# kFzIasMotionAlarm (occupancy bit0) / kFzIasContactAlarm (inverted contact) +
# kFzIlluminance. See tests/README.md.
add_executable(zhc_tplink_parity_tests tests/test_tplink_parity.cpp)
target_link_libraries(zhc_tplink_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Nyce NCZ-30xx IAS / multi-sensor family. The five
# generated defs were lowered as battery + generic kFzIasZone (bare `alarm`,
# wrong zoneStatus bit) and dropped the motion sensors' occupancy/temperature/
# humidity channels. Contact defs re-wired to kFzIasContactAlarm (bit 0); motion
# defs re-wired to kFzOccupancy + kFzTemperature + kFzHumidity +
# kFzIasMotionAlarm2 (alarm_2 / bit 1). See tests/README.md.
add_executable(zhc_nyce_parity_tests tests/test_nyce_parity.cpp)
target_link_libraries(zhc_nyce_parity_tests PRIVATE zhc_definitions)

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

# Parity fixture for the Leviton RC-2000WH (Omnistat2) thermostat: z2m wires
# the full fz.thermostat but the auto-port only wired the generic
# kFzThermostat (0x0000/0x0012/0x001C), leaving pi_heating_demand (0x0008),
# local_temperature_calibration (0x0010) and occupied_cooling_setpoint
# (0x0011) dead. kFzLevitonThermostat (definitions/leviton/_shared.cpp) now
# decodes those alongside the generic decoder + kFzFanMode. Unlike Danfoss,
# Leviton does not set dontMapPIHeatingDemand → pi_heating_demand is mapped
# 0-255→0-100 %. See tests/README.md for the pattern.
add_executable(zhc_leviton_parity_tests tests/test_leviton_parity.cpp)
target_link_libraries(zhc_leviton_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Zen Zen-01-W thermostat: z2m wires the full
# fz.thermostat + fz.fan + fz.battery but the auto-port only wired the generic
# kFzThermostat (0x0000/0x0012/0x001C), leaving occupied_cooling_setpoint
# (0x0011), running_state (0x0029) and local_temperature_calibration (0x0010)
# dead — and omitted those exposes entirely while mistyping system_mode/fan_mode
# as Binary. kFzZenThermostat (definitions/zen/_shared.cpp) now decodes those
# alongside the generic decoder + kFzFanMode. See tests/README.md for the pattern.
add_executable(zhc_zen_parity_tests tests/test_zen_parity.cpp)
target_link_libraries(zhc_zen_parity_tests PRIVATE zhc_definitions)

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

# Parity fixture for the JetHome WS7 3-channel battery discrete-input module.
# The generated def wired a phantom genOnOff in/out (settable `state`) and
# dropped the entire genMultistateInput discrete-input channel. Graduated to
# a Tier-2 override wiring kFzBattery + kFzJetHomeMultiStateAction, an
# `action` enum, and per-input action_in1..in3 suffixing. See tests/README.md.
add_executable(zhc_jethome_parity_tests tests/test_jethome_parity.cpp)
target_link_libraries(zhc_jethome_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the LeTV (Leeco) 8-key scene remote (qlwz.letv8key.10).
# The auto-port mis-cast the remote as a controllable on/off switch (phantom
# genOnOff + settable `state` + 0x0006 binding) and dropped the whole action
# channel. Restored: kFzLetv8KeyAction decodes genMultistateInput.presentValue
# keyed by source endpoint into "<gesture>_<button>" (single_up / hold_center).
add_executable(zhc_letv_parity_tests tests/test_letv_parity.cpp)
target_link_libraries(zhc_letv_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the L&S Lighting (ls) family. The SEMOTE remote
# (756200643) declared a `stop_move_step` action in its enum but emitted
# it from NO converter (lightingColorCtrl cmd 0x47 had no decoder); fixed
# with the bespoke kFzLsCommandStopMoveStep. The three L&S lights are
# spot-checked for correct sink shape. See tests/README.md.
add_executable(zhc_ls_parity_tests tests/test_ls_parity.cpp)
target_link_libraries(zhc_ls_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Livolo TI0001-* family. Every device speaks a
# bespoke raw protocol over genPowerCfg [0x7A 0xD1]; those frames decode
# to MessageType::Raw, but the custom decoders gated only on Command so
# none ever fired. Plus a phantom IAS bundle on the PIR, a bare-"state"
# expose on the 4-gang switch, and dead generic on/off/cover converters
# on the curtain/dimmer/cover ports.
add_executable(zhc_livolo_parity_tests tests/test_livolo_parity.cpp)
target_link_libraries(zhc_livolo_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the EasyAccess EasyCode903G2.1 door lock: the port
# carried a phantom genOnOff bundle, dropped fz.easycode_action (the
# keypad/RFID/zigbee unlock event stream), and dropped sound_volume /
# auto_relock. Fixed to the closuresDoorLock wire shape.
add_executable(zhc_easyaccess_parity_tests tests/test_easyaccess_parity.cpp)
target_link_libraries(zhc_easyaccess_parity_tests PRIVATE zhc_definitions)

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

# Parity fixtures for the DIYRuZ family. DIYRuZ_Flower exposed
# `soil_moisture` (msSoilMoisture) and DIYRuZ_AirSense exposed `co2` (msCO2)
# with no decoder — the generic kFzSoilMoisture/kFzCO2 did not exist when
# the ports were generated; wired both now. DIYRuZ_magnet exposed `contact`
# (genOnOff onOff, NOT IAS) with no decoder — added vendor kFzDiyruzContact.
# See tests/README.md.
add_executable(zhc_diyruz_parity_tests tests/test_diyruz_parity.cpp)
target_link_libraries(zhc_diyruz_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the MindY Leleka air-quality monitor. The def declared
# a `co2` expose and bound msCO2 (0x040D) but omitted the decoder, so every
# CO2 report silently dropped (the generic kFzCO2 already existed; the def's
# "Tier-1 has no kFzCO2" comment was stale). Wired generic::kFzCO2 and
# graduated the def generated/ -> parent. See tests/README.md.
add_executable(zhc_mindy_parity_tests tests/test_mindy_parity.cpp)
target_link_libraries(zhc_mindy_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the PushOk Hardware DIY family. POK003/POK010 (water
# level) exposed `contact` (genBinaryInput presentValue) with no decoder —
# wired vendor kFzPushokContact. POK008 (thermostat relay) dropped its
# temperature channel — wired generic kFzTemperature. POK016/POK017
# (window/vent) are m.windowCovering covers that were misclassified as an
# on/off switch (POK016) or stripped to battery-only (POK017) — restored the
# cover position. See tests/README.md.
add_executable(zhc_pushok_parity_tests tests/test_pushok_parity.cpp)
target_link_libraries(zhc_pushok_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Osram (LEDVANCE/Lightify) family. Otherwise a pure
# lighting vendor (ledvanceLight/ledvanceOnOff over stock ZCL — CLEAN, plugs
# are on/off-only with no metering in z2m). Three non-light ports were dead:
# the SMART+ Switch Mini (AC0251100NJ/...) and Switch (4058075816459) remotes
# lowered kFzOnOff + a phantom `state` instead of the genOnOff/genLevelCtrl/
# lightingColorCtrl command action converters — every button was dead; and the
# SMART+ Motion Sensor (AC01353010G) had no temperature decoder/expose/0x0402
# binding and lowered the generic kFzIasZone (bare `alarm`) instead of the
# `occupancy` key z2m advertises. All three graduated to Tier-2 parent
# overrides (action enum + command converters; kFzTemperature +
# kFzIasMotionAlarm2). See tests/README.md.
add_executable(zhc_osram_parity_tests tests/test_osram_parity.cpp)
target_link_libraries(zhc_osram_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Müller-Licht (tint) battery remotes. The three
# remotes (MLI-404011/404049, 404022/404049C, 404002) advertised action
# enums for colour-wheel / store events the auto-generated ports never
# wired. Graduated to Tier-2 parents wiring kFzCommandMoveToColor (+
# MoveToColorTemp on 404011) and kFzCommandStore. scene_<n> stays a
# pinned INFRA gap (fz.tint_scene needs Write-attr payload parsing).
add_executable(zhc_muller_licht_parity_tests tests/test_muller_licht_parity.cpp)
target_link_libraries(zhc_muller_licht_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Ubisys family. H1/H10 thermostats declared
# pi_heating_demand + running_mode exposes that the generic kFzThermostat
# never decoded (attrs 0x0008/0x001E) — wired kFzUbisysThermostatExtras
# (0-255 -> 0-100 % demand remap, enum->string running_mode). J1 shutter
# gained a tilt axis (attr 0x0009) via kFzUbisysCoverPositionTilt. R0
# router shed a dead on/off bundle (z2m exposes: []). All graduated to
# Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_ubisys_parity_tests tests/test_ubisys_parity.cpp)
target_link_libraries(zhc_ubisys_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the LUX KONOz KN-Z-WH1-B04 HVAC thermostat. The
# auto-port wired only the generic kFzThermostat (0x0000/0x0012/0x001C),
# leaving occupied_cooling_setpoint (0x0011) + running_state (0x0029) as
# dead exposes; running_mode (0x001E) is z2m-decoded too. Fixed by wiring
# kFzLuxThermostatExtras. fan_mode (hvacFanCtrl) regression-checked.
add_executable(zhc_lux_parity_tests tests/test_lux_parity.cpp)
target_link_libraries(zhc_lux_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the NodOn family. TRV-4-1-00 declared trv_mode
# (hvacThermostat attr 0x4000) + valve_position (0x4001) with matching Tz
# writers but only wired the generic kFzThermostat (0x0000/0x0012/0x001C),
# leaving both write-only — fixed with kFzNodonTrvExtras. SIN-4-RS-20[_PRO]
# roller shutters gained a tilt axis (attr 0x0009) via
# kFzNodonCoverPositionTilt + a `tilt` expose + the generic
# kTzCoverPositionTilt writer (z2m controls:['tilt','lift']). Both
# graduated to Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_nodon_parity_tests tests/test_nodon_parity.cpp)
target_link_libraries(zhc_nodon_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_nordtronic_parity_tests tests/test_nordtronic_parity.cpp)
target_link_libraries(zhc_nordtronic_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_novo_parity_tests tests/test_novo_parity.cpp)
target_link_libraries(zhc_novo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Owon HVAC family. AC201/AC221/PCT504 declared a
# cooling setpoint and PCT504/PCT512 declared running_state + occupancy that
# the generic kFzThermostat (0x0000/0x0012/0x001C) never decoded and no
# occupancy decoder was wired — all dead exposes. Wired a vendor-local
# kFzOwonThermostatExtras (attr 0x0011 cooling setpoint + 0x0029 running_state
# enum->string) alongside the generic decoder, plus the generic kFzOccupancy.
# All four thermostat defs graduated to Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_owon_parity_tests tests/test_owon_parity.cpp)
target_link_libraries(zhc_owon_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Airzone Aidoo AZAI6ZBEMHI AC controller. The def
# exposed a cooling setpoint (z2m .withSetpoint("occupied_cooling_setpoint"))
# decoded by fz.thermostat from hvacThermostat attr 0x0011, which the generic
# kFzThermostat (0x0000/0x0012/0x001C only) never filled. Fixed by wiring the
# Tier-2 kFzAirzoneThermostatExtras converter alongside the generic decoder.
add_executable(zhc_airzone_aidoo_parity_tests tests/test_airzone_aidoo_parity.cpp)
target_link_libraries(zhc_airzone_aidoo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Imhotep Creation heater thermostats + BRI4P bridge.
# Both defs exposed standard hvacThermostat setpoint-limit / cooling-setpoint
# keys the generic kFzThermostat (0x0000/0x0012/0x001C only) never filled —
# dead exposes against z2m's fz.thermostat. E-Ctrl: min/max heat setpoint
# limits (0x0015/0x0016). BRI4P: cooling setpoint (0x0011) + min/max heat AND
# cool setpoint limits (0x0015-0x0018), none of which were even exposed.
# Wired a vendor-local kFzImhotepThermostatExtras alongside the generic
# decoder; both defs graduated to Tier-2 parent overrides. See tests/README.md.
add_executable(zhc_imhotepcreation_parity_tests tests/test_imhotepcreation_parity.cpp)
target_link_libraries(zhc_imhotepcreation_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the iCasa family. ICZB-IW21D (dimmer + electricity
# meter) was auto-ported with only kFzMetering (seMetering 0x0702) so the
# z2m m.electricityMeter() cluster:"both" voltage/current channels
# (haElectricalMeasurement 0x0B04) were dropped; the fix wires
# kFzElectricalMeasurement + binding + voltage/current exposes. The keypad /
# remote command-action ports (KPD12/KPD14S/RM11S) are regression-guarded.
add_executable(zhc_icasa_parity_tests tests/test_icasa_parity.cpp)
target_link_libraries(zhc_icasa_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Plugwise heating family + plug. The Tom (106-03)
# and Emma (170-01) thermostat ports declared standard hvacThermostat
# exposes that the generic kFzThermostat (0x0000/0x0012/0x001C only) never
# filled: pi_heating_demand (0x0008), occupied_cooling_setpoint (0x0011),
# running_state (0x0029), local_temperature_calibration (0x0010) and
# outdoor_temperature (0x0001). Fixed by extending kFzPlugwiseThermostat to
# decode them; Emma's pIHeatingDemand-as-boiler_setpoint overload wired via
# a def-local converter + expose rename. Both defs graduated to Tier-2.
add_executable(zhc_plugwise_parity_tests tests/test_plugwise_parity.cpp)
target_link_libraries(zhc_plugwise_parity_tests PRIVATE zhc_definitions)

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

# Parity fixtures for the MEAZON metering plugs (BIZY_PLUG, DINRAIL). Both run
# z2m fz.meazon_meter, which decodes MANUFACTURER-SPECIFIC (MEAZON_S_A)
# attributes carried inside seMetering 0x0702 (power 0x2001, voltage 0x2004/
# 0x2015, current 0x2007/0x2018, energy 0x3000) — proprietary IDs that the
# generic kFzMetering (0x0000/0x0400) + kFzElectricalMeasurement (0x0B04) never
# read, so every channel was a dead expose. Added kFzMeazonMeter
# (definitions/meazon/_shared.cpp) replacing the generic pair on both graduated
# Tier-2 defs; bindings realigned to endpoint 10 (genOnOff + seMetering). See
# tests/README.md.
add_executable(zhc_meazon_parity_tests tests/test_meazon_parity.cpp)
target_link_libraries(zhc_meazon_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the LifeControl (MCLH series). The sweep restored
# three dropped channels: MCLH-02 colour bulb (lightingColorCtrl half),
# MCLH-03 socket (haElectricalMeasurement voltage/current), and MCLH-08
# air-quality sensor (the custom msTemperatureMeasurement-packed temp/
# humidity/eco2/voc report). See tests/README.md.
add_executable(zhc_lifecontrol_parity_tests tests/test_lifecontrol_parity.cpp)
target_link_libraries(zhc_lifecontrol_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Dawon DNS (Korean smart-plug / in-wall-switch)
# family. The vendor is mostly clean (m.onOff switches, fz.metering plugs,
# TH-110 temp/humidity, KB-HD100 ssIasZone card-holder). The one real gap
# was PM-B540-ZB, which alone reports its die temperature on genDeviceTempCfg
# (0x0002): z2m wires fz.device_temperature but the auto-port dropped it.
# Added the generic kFzDeviceTemperature converter + 0x0002 cluster label
# and graduated Daw_PM_B540_ZB.cpp to a Tier-2 override.
add_executable(zhc_dawon_dns_parity_tests tests/test_dawon_dns_parity.cpp)
target_link_libraries(zhc_dawon_dns_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_dqsmart_parity_tests tests/test_dqsmart_parity.cpp)
target_link_libraries(zhc_dqsmart_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Amina amina S EV Charger. Its AC metering rides
# standard haElectricalMeasurement 0x0B04 via z2m's m.electricityMeter
# (cluster:"electrical", acFrequency, threePhase) + a standalone
# total_active_power numeric. The hand-rewrite leaned solely on the
# generic kFzElectricalMeasurement (activePower/voltage/current) and only
# exposed power/energy, so ac_frequency / total_active_power / the per-
# phase b/c voltage/current/power were dropped and voltage/current were
# decoded-but-unexposed. Added kFzAminaElectricalMeasurementExtras
# (definitions/amina/_shared.cpp) wired alongside the generic + the
# missing exposes. The EV-state/alarms/energy channels on the 0xFEE7
# manuSpecific cluster were already decoded (regression-tested here).
add_executable(zhc_amina_parity_tests tests/test_amina_parity.cpp)
target_link_libraries(zhc_amina_parity_tests PRIVATE zhc_definitions)

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

add_executable(zhc_evanell_parity_tests tests/test_evanell_parity.cpp)
target_link_libraries(zhc_evanell_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_envilar_parity_tests tests/test_envilar_parity.cpp)
target_link_libraries(zhc_envilar_parity_tests PRIVATE zhc_definitions)

# Parity for the Siterwell GS361A-H04 TS0601 radiator-valve thermostat. The
# auto-port MISROUTED the decoder: it wired the generic genThermostat pair
# (kFzThermostat / kTzThermostat) + bound ZCL 0x0201, but z2m decodes this
# Tuya-MCU TRV entirely over the 0xEF00 DP stream (legacy.fz.tuya_thermostat),
# so setpoint / local_temperature / system_mode / child_lock / window+valve
# detection / battery / valve-position were all dead. Graduated to a Tier-2
# DP map with z2m-matching scale (setpoint+local_temp /10) and enum order
# (system_mode {0:off,1:auto,2:heat}). See tests/README.md.
add_executable(zhc_siterwell_parity_tests tests/test_siterwell_parity.cpp)
target_link_libraries(zhc_siterwell_parity_tests PRIVATE zhc_definitions)

# Parity for the TECH Controllers VNTH-T2 smart radiator valves (TS0601 Tuya
# DP TRVs). z2m fans DP 101 to BOTH `state` (on/off) AND `system_mode`
# (lookup {heat:true, off:false}); the auto extract kept only `state`, so the
# heat/off system_mode channel was undecoded + unexposed. A second DP-101 row
# now emits system_mode via the new kTuyaDpFlagBoolEnum (bool DP fanned to a
# string label). See tests/README.md.
add_executable(zhc_tech_parity_tests tests/test_tech_parity.cpp)
target_link_libraries(zhc_tech_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Essentials "Thermostat Zigbee smart home"
# (TS0601 / _TZE200_i48qyn9s) Tuya-DP thermostat. The auto extract dropped 11
# of z2m's 18 active datapoints, including the PRIMARY setpoint (DP 16,
# divideBy 2) and the open-window detection channels (DP 107 window_open,
# DP 116 open_window_temperature, DP 117 detect_window_time_minute). These are
# now mapped with z2m-matching scale; window_open fans the boolean DP to a
# "YES"/"NO" string via kTuyaDpFlagBoolEnum. See tests/README.md.
add_executable(zhc_essentials_parity_tests tests/test_essentials_parity.cpp)
target_link_libraries(zhc_essentials_parity_tests PRIVATE zhc_definitions)

# Parity for the Onenuo 288WZ smoke detector (TS0601 / _TZE204_kgaxpvxr +
# _TZE284_n4ttsck2). The auto-extracted DP defs dropped DP1 (smoke +
# smoke_state) and DP101 (self_test_result) and carried phantom state/action
# exposes; a sibling stub mis-wired the detector to ssIasZone. DP1 enum fans
# to a boolean smoke alarm via the new kTuyaDpFlagEnumBool flag.
add_executable(zhc_onenuo_parity_tests tests/test_onenuo_parity.cpp)
target_link_libraries(zhc_onenuo_parity_tests PRIVATE zhc_definitions)

# Parity for the GiEX water-irrigation valves (QT06_1 / QT06_2). Both are
# Tuya-MCU (0xEF00) DP-stream devices (z2m legacy.fromZigbee.giexWaterValve)
# auto-ported as bare kFzBattery + kFzOnOff stubs binding ZCL 0x0001/0x0006
# they never speak and dropping 8 irrigation datapoints. Graduated to Tier-2
# DP-map overrides: mode/state (bool->string via kTuyaDpFlagBoolEnum),
# raw numeric water_consumed/battery/irrigation_target/cycle counts (z2m
# applies NO scale — the float-divisor suspect was FALSE), and String DPs
# (start/end time, last duration). DP 106 currentTemperature stays unmapped
# (z2m ignores it). See tests/README.md.
add_executable(zhc_giex_parity_tests tests/test_giex_parity.cpp)
target_link_libraries(zhc_giex_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the IOTPerfect PF-PM02D-TYZ smart water/gas valve (TS0601 /
# 6× _TZE2xx). The auto-port mis-routed this Tuya-MCU (0xEF00) valve as a bare
# kFzOnOff / kTzOnOff stub bound to genOnOff 0x0006, leaving the `state` expose
# dead. z2m decodes via legacy.fz.tuya_switch (DP 1 state, bool -> ON/OFF) and
# writes via legacy.tz.tuya_switch_state. Graduated to a Tier-2 override wiring
# fz_tuya_datapoints + tz_tuya_datapoints over the 0xEF00 DP map.
add_executable(zhc_iotperfect_parity_tests tests/test_iotperfect_parity.cpp)
target_link_libraries(zhc_iotperfect_parity_tests PRIVATE zhc_definitions)

# Parity for the Qoto QT-05M solar garden watering timer. A Tuya-MCU (0xEF00)
# DP-stream device (z2m legacy.fromZigbee.watering_timer) auto-ported as a bare
# kFzBattery + kFzOnOff stub binding ZCL 0x0001/0x0006 it never speaks, with
# phantom state/battery/voltage, dropping all 7 watering datapoints. Graduated
# to a Tier-2 DP-map override: raw numeric water_flow / last+remaining
# duration / valve_state / valve_state_auto_shutdown / shutdown_timer / battery
# (z2m applies NO scale — the float-divisor suspect was FALSE). See tests/README.md.
add_executable(zhc_qoto_parity_tests tests/test_qoto_parity.cpp)
target_link_libraries(zhc_qoto_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Alecto SMART-HEAT10 (Tuya-MCU TRV thermostat) and
# SMART-SMOKE10 (Tuya-MCU smoke detector). The auto extract misrouted HEAT10
# to the generic hvacThermostat 0x0201 cluster and SMOKE10 to a generic IAS
# zone (0x0500) — both speak only the 0xEF00 DP stream. Graduated to Tier-2
# overrides wiring fz_tuya_datapoints + tz_tuya_datapoints with the full DP
# maps (HEAT10 setpoint/local_temperature /10; SMOKE10 numeric raw).
add_executable(zhc_alecto_parity_tests tests/test_alecto_parity.cpp)
target_link_libraries(zhc_alecto_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the iLightsIn HSSA18-Z-MID (Zhaga light + multi-sensor).
# The auto-port dropped m.illuminance() + m.occupancy() entirely and wired the
# IAS alarm to the generic kFzIasZone (dead bare `alarm`, bit 1 lost). Tier-2
# override restores illuminance/occupancy decoders+bindings and the split
# occupancy_alarm_1/occupancy_alarm_2 typed IAS converters.
add_executable(zhc_ilightsin_parity_tests tests/test_ilightsin_parity.cpp)
target_link_libraries(zhc_ilightsin_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Simpla Home Soil Pro soil-moisture sensor. The
# auto-generated def was a battery-only stub that dropped temperature,
# illuminance and BOTH soil_moisture channels and placed the two soil
# endpoints at ep 1. The graduated def wires the dropped decoders and
# corrects the endpoint_map to {z1_top: 2, z2_bottom: 3}.
add_executable(zhc_simpla_home_parity_tests tests/test_simpla_home_parity.cpp)
target_link_libraries(zhc_simpla_home_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Zemismart curtain/blind-motor + switch family. Six
# generic-cover stubs (kFzCoverPosition on the 0x0102 cluster) shadowed TS0601
# Tuya-DP devices that only speak 0xEF00; five duplicated a kDefZem__TZE* DP
# def's fingerprint and were removed, and ZM25TQ was graduated to a real DP
# def. Pins DP resolution + position/state decode. See tests/README.md.
add_executable(zhc_zemismart_parity_tests tests/test_zemismart_parity.cpp)
target_link_libraries(zhc_zemismart_parity_tests PRIVATE zhc_definitions)

# Ynoa (Lytimages): two remotes (LA-5KEY-RGBW, 8718801528334) were
# wrong-bundled as settable on/off lights — every key press dead — and the
# four CCT/RGBW bulbs dropped their colorTemp (+ color on the RGBW) axes.
# Graduated each def: remotes → genOnOff/genLevelCtrl/lightingColorCtrl
# command decoders + `action`, no toolbox; bulbs → kFzColorTemperature
# (+ kFzColor) + color_temp/color_xy exposes + 0x0300 binding. Also extended
# generic kFzBattery to decode genPowerCfg batteryAlarmState (0x0035) so the
# 5KEY's battery_low is no longer dead. Plug pinned as a regression guard
# (0x0702 + 0x0B04 both alive).
add_executable(zhc_ynoa_parity_tests tests/test_ynoa_parity.cpp)
target_link_libraries(zhc_ynoa_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Avatto family (Tuya-OEM TS0601 DP devices). Three
# auto-generated DP-table decode bugs fixed: ME168_AVATTO DP47 calibration
# divisor 100->1 (z2m localTempCalibration2 is identity), and ZDMS16-1 +
# ZDMS16-2 (both channels) switch_type enums re-ordered from the generic
# switchType order to z2m switchType2 {toggle:0,state:1,momentary:2}. Pins the
# corrected calibration scale + switch_type labels on the 0xEF00 wire. See
# tests/README.md.
add_executable(zhc_avatto_parity_tests tests/test_avatto_parity.cpp)
target_link_libraries(zhc_avatto_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_girier_parity_tests tests/test_girier_parity.cpp)
target_link_libraries(zhc_girier_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for Xenon. The single z2m X7726 curtain motor (Tuya TS0601 /
# _TZE284_hbjwgkdh, a 0xEF00 DP device) had been emitted as TWO colliding defs
# for the same fingerprint: the correct full DP port plus a stub that misrouted
# to genWindowCovering (0x0102) and dropped state/calibration/temperature. The
# dead stub was removed; the DP def graduated to definitions/xenon/X7726.cpp with
# the z2m identity and the phantom `action` expose dropped. See
# tests/test_xenon_parity.cpp.
add_executable(zhc_xenon_parity_tests tests/test_xenon_parity.cpp)
target_link_libraries(zhc_xenon_parity_tests PRIVATE zhc_definitions)
add_executable(zhc_xal_parity_tests tests/test_xal_parity.cpp)
target_link_libraries(zhc_xal_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for Javis. The JS-MC-SENSOR-ZB microwave/presence sensor had
# been mis-wired to the IAS-zone converter (phantom alarm/tamper exposes); z2m
# decodes it via the 0xEF00 Tuya DP stream. Ported the real DP->key map and
# split the _TZE200_kagkgk0i DP-remap variant into its own def. The JS-SLK2-ZB
# lock had been mis-ported as a controllable on/off switch (z2m: action+battery,
# empty toZigbee). See tests/test_javis_parity.cpp.
add_executable(zhc_javis_parity_tests tests/test_javis_parity.cpp)
target_link_libraries(zhc_javis_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for Weten. The single z2m "PCI E" Tuya TS0601 DP device had
# been emitted as three colliding artifacts (a misrouted genOnOff dup + two
# model-only DP defs split per manufacturer name); consolidated into one def
# carrying both _TZE* names + white-label. Real fix: DP101 restart_mode kept
# only 1 of 3 enum values ("force restart"/"–" were silently dropped on decode).
add_executable(zhc_weten_parity_tests tests/test_weten_parity.cpp)
target_link_libraries(zhc_weten_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for Nedis (single z2m entry: ZBHTR20WT Tuya TS0601 DP TRV,
# 3 manufacturer names). One real decode bug fixed: DP27 calibration baked
# divisor=100 but z2m wires localTempCalibration2 (identity) -> divisor=1.
add_executable(zhc_nedis_parity_tests tests/test_nedis_parity.cpp)
target_link_libraries(zhc_nedis_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for Saswell (single z2m entry: SEA801/SEA802 Tuya-MCU TRV,
# 0xEF00 DP stream). The legacy _TYST11_* fingerprints were mis-routed to the
# generic genThermostat (0x0201) decoder via a duplicate def and decoded
# nothing; folded into the Tuya-DP def and the dead duplicate removed.
add_executable(zhc_saswell_parity_tests tests/test_saswell_parity.cpp)
target_link_libraries(zhc_saswell_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for MAZDA (single device: TR-M2Z TRV, Tuya-OEM TS0601 0xEF00
# DP valve). The generated DP map truncated DP47 local_temperature_calibration
# (count=23 vs 24 entries) and gave it divisor=1; z2m wires localTempCalibration3
# = v/10. Fixed on both fingerprints. See tests/README.md.
add_executable(zhc_mazda_parity_tests tests/test_mazda_parity.cpp)
target_link_libraries(zhc_mazda_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Neo (NEO Coolcam, Tuya-OEM) family. Four named
# devices were misrouted by the generator to IAS Zone / battery-only: the
# NAS-AB02B0 / NAS-AB02B2 sirens and NAS-PD07 PIR are Tuya-DP (neo_t_h_alarm /
# neo_alarm / neo_nas_pd07), and NAS-TH07B2 (SNZB-02) is standard-cluster T/H.
# These pin the restored Tuya-DP + standard temperature/humidity decode paths.
add_executable(zhc_neo_parity_tests tests/test_neo_parity.cpp)
target_link_libraries(zhc_neo_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_bseed_parity_tests tests/test_bseed_parity.cpp)
target_link_libraries(zhc_bseed_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Nous (Tuya-OEM) family. The E6 and SZ-T04 LCD
# temperature/humidity sensors are TS0601 Tuya-DP devices but the generated
# defs wired a genPowerCfg-only battery converter, so temperature, humidity
# and battery never decoded. These pin the restored Tuya-DP decode path
# (temperature DP1 /10, humidity DP2, battery DP4, sensitivity DP19 /10).
add_executable(zhc_nous_parity_tests tests/test_nous_parity.cpp)
target_link_libraries(zhc_nous_parity_tests PRIVATE zhc_definitions)

# Parity fixture for Automaton AUT000069 (TS011F 5-zone valve controller).
# The auto-port collapsed z2m's 5-endpoint tuyaOnOff into a single bare
# "state" with one EP1 binding and no endpoint_map, so zones 2..5 collided
# and never bound. This pins the per-endpoint suffixed keys (state_l1..l5).
add_executable(zhc_automaton_parity_tests tests/test_automaton_parity.cpp)
target_link_libraries(zhc_automaton_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the ShinaSystem (SiHAS) family. Generator misses left
# several primary sensor/control channels dead: USM/OSM/DMS exposed
# occupancy with no decoder (and USM dropped illuminance), and GCM-300Z's
# gas valve had a null to_zigbee plus a state/gas_valve_state key mismatch.
# These pin the restored decode/encode paths. See tests/README.md.
add_executable(zhc_shinasystem_parity_tests tests/test_shinasystem_parity.cpp)
target_link_libraries(zhc_shinasystem_parity_tests PRIVATE zhc_definitions)

# FireAngel CO family. The generated def dropped the W2-Module CO channel
# (battery-only; CO rides ssIasZone zoneStatus bit 8, not the standard bit 0)
# and omitted the ZBCO-AE-10X-EUR alarm entirely. These pin the restored /
# authored decoders + the manufacturer-gated find_definition routing.
add_executable(zhc_fireangel_parity_tests tests/test_fireangel_parity.cpp)
target_link_libraries(zhc_fireangel_parity_tests PRIVATE zhc_definitions)

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

# Insta — 57008000 blinds actuator declared a `tilt` expose and z2m's
# cover_position_tilt reports both currentPositionLiftPercentage (0x0008)
# and currentPositionTiltPercentage (0x0009) on EP6, but the def wired only
# the generic kFzCoverPosition (lift). Tilt reports decoded to nothing.
# Added kFzCoverTilt + default_endpoint=6; test asserts both halves decode.
add_executable(zhc_insta_parity_tests tests/test_insta_parity.cpp)
target_link_libraries(zhc_insta_parity_tests PRIVATE zhc_definitions)

# Perenio — PEHWE20 two-channel relay was missing `endpoint_map`, so its
# `_l1`/`_l2` exposes never populated (both channels collided on the bare
# `state`/`power_on_behavior`/`switch_type` keys). Graduated to Tier-2 +
# added the map. Tests pin per-endpoint key suffixing, the perenioSpecific
# smart-plug decode (PEHPL0X), and the IAS sensor semantic keys.
add_executable(zhc_perenio_parity_tests tests/test_perenio_parity.cpp)
target_link_libraries(zhc_perenio_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Stelpro (Quebec) line-voltage thermostat family.
# All six defs (HT402/SMT402/SMT402AD/SORB/ST218/STZB402) declared the
# keypad_lockout expose StateSet but wired neither decoder nor encoder
# (z2m fz.hvac_user_interface + tz.thermostat_keypad_lockout on every
# device) — a dead attribute. Graduated to parent overrides wiring the
# new kFzStelproKeypadLockout / kTzStelproKeypadLockout. Guards the
# pre-existing thermostat decode (Eco→auto, running_state from
# pIHeatingDemand) and the outdoor-temp manuSpec write too.
add_executable(zhc_stelpro_parity_tests tests/test_stelpro_parity.cpp)
target_link_libraries(zhc_stelpro_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Stello (Hilo) family. Pins the STLO-34
# keypad_lockout dead-attribute fix (decode + write via the shared
# stelpro converters, graduated to parent), the Hilo power/energy
# decoders (hvacThermostat 0x4008/0x4009) + thermostat surface, and the
# STLO-23 onOff/metering/peak-demand-icon surface.
add_executable(zhc_stello_parity_tests tests/test_stello_parity.cpp)
target_link_libraries(zhc_stello_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Netica FRZ1 ("FreezBee") Frisquet-boiler
# thermostat. Pins the graduated-def fixes: phantom battery removed,
# running_state (0x0029) + control_sequence_of_operation (0x001B) +
# the two manuSpec read-backs (remote_temperature 0x4000,
# target_water_temperature 0x4002) now decode.
add_executable(zhc_netica_parity_tests tests/test_netica_parity.cpp)
target_link_libraries(zhc_netica_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Purmo/Radson Yali Parada Plus (PUMM01102)
# electric oil-filled radiator. The Tier-1 auto-port wired only the
# generic kFzThermostat (0x0000/0x0012/0x001C), leaving the exposed
# unoccupied_heating_setpoint (0x0014), local_temperature_calibration
# (0x0010), running_mode (0x001E), running_state (0x0029),
# max_heat_setpoint_limit (0x0016) and occupancy (0x0002) dead — z2m's
# fz.thermostat decodes all of them. Now decoded via
# kFzPurmoThermostatExtras; system_mode re-typed Binary -> Enum.
add_executable(zhc_purmo_parity_tests tests/test_purmo_parity.cpp)
target_link_libraries(zhc_purmo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Acova electric-radiator thermostats
# (ALCANTARA2, TAFFETAS2/PERCALE2, IHC-Enki). The generic kFzThermostat
# emits attr 0x0012 as "current_heating_setpoint" and decodes none of
# 0x0014 / 0x0010 / 0x0029, so occupied/unoccupied setpoint, calibration
# and running_state were dead keys. kFzAcovaThermostatExtras fixes them.
add_executable(zhc_acova_parity_tests tests/test_acova_parity.cpp)
target_link_libraries(zhc_acova_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the VAV (NodOn-derived) modules (MOD1 pilot-wire
# heater, MOD2 metering relay). The Tier-1 auto-port dropped
# power_on_behavior (genOnOff 0x4003) on both, and the entire NodOn
# pilot-wire cluster (0xFC00 attr 0x0000 → pilot_wire_mode) on MOD1.
add_executable(zhc_vav_parity_tests tests/test_vav_parity.cpp)
target_link_libraries(zhc_vav_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Espressif ESP32-C6/H2 range extender. z2m defines
# it as a pure router (exposes:[], fromZigbee:[], toZigbee:[]); the Tier-1
# auto-port carried a phantom on/off + battery bundle. Pinned stripped.
add_executable(zhc_espressif_parity_tests tests/test_espressif_parity.cpp)
target_link_libraries(zhc_espressif_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the TubesZB "tubeszb.router" (CC2652 Router). z2m defines
# it as a pure router (exposes:[], toZigbee:[], fromZigbee only linkquality);
# the Tier-1 auto-port carried a phantom on/off bundle. Pinned stripped.
add_executable(zhc_tubeszb_parity_tests tests/test_tubeszb_parity.cpp)
target_link_libraries(zhc_tubeszb_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Intuis (Muller Intuitiv) intuisradiator electric
# heater. The generic kFzThermostat only decodes 0x0000 / 0x0012 / 0x001C
# (emitting 0x0012 as current_heating_setpoint), so occupied/unoccupied
# setpoint + running_state were dead; occupancy (0x0406) and the
# hvacUserInterfaceCfg (0x0204) keypad_lockout / temperature_display_mode
# had no decoder. kFzIntuisThermostat + kFzIntuisHvacUi + kFzOccupancy fix them.
add_executable(zhc_intuis_parity_tests tests/test_intuis_parity.cpp)
target_link_libraries(zhc_intuis_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for IT Commander. The ITCMDR_Contact sensor wired the
# generic kFzIasZone (bare `alarm`, dead `contact` expose); swapped to
# kFzIasContactAlarm (inverted bit-0 → `contact`). The ITCMDR_Click
# button was mis-ported as a controllable genOnOff switch; re-ported to
# the vendor kFzItcmdrClicks (genMultistateInput PresentValue → action).
add_executable(zhc_ITCommander_parity_tests tests/test_ITCommander_parity.cpp)
target_link_libraries(zhc_ITCommander_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Nobø (Glen Dimplex) SWT-IZ / SPC-IZ electric-heater
# thermostats. Both use z2m m.thermostat() but were mis-ported as a bare
# on/off + battery class (kFzBattery + kFzOnOff) — the whole thermostat
# surface was dead. kFzNoboThermostat decodes local_temperature,
# occupied/unoccupied_heating_setpoint, system_mode and running_mode.
add_executable(zhc_nobo_parity_tests tests/test_nobo_parity.cpp)
target_link_libraries(zhc_nobo_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Aurora "AOne" non-light family. The PIR
# (AU-A1ZBPIRS) and contact (AU-A1ZBDWS) sensors decoded the bare "alarm"
# key (generic kFzIasZone) instead of the typed occupancy/contact, and the
# PIR dropped its illuminance channel; the smart remote (AU-A1ZBRC) and both
# wireless battery rotary dimmers (AU-A1ZBR1GW single-EP, AU-A1ZBR2GW
# multi-EP) were mis-ported as dead on/off switches with the action stream
# dropped. Graduated to parent overrides wiring kFzIas<Type>Alarm +
# kFzIlluminance and the generic command-action decoders.
add_executable(zhc_aurora_lighting_parity_tests tests/test_aurora_lighting_parity.cpp)
target_link_libraries(zhc_aurora_lighting_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_wally_parity_tests tests/test_wally_parity.cpp)
target_link_libraries(zhc_wally_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Onesti (EasyAccess / Nimly / Relax) vendor. The two
# door locks are clean on their generic core surface (lock state / battery /
# action / pincode); the vendor-specific last_action / last_used_pin_code
# attribute + raw-cluster decoders are INFRA-deferred. The S4RX-110 "Relax"
# metering plug dropped its device_temperature channel (z2m wires
# fz.device_temperature + binds genDeviceTempCfg 0x0002) — same gap class as
# dawon_dns PM-B540. Graduated to a Tier-2 parent wiring kFzDeviceTemperature
# + the expose + the 0x0002 bind, with all clusters bound on endpoint 2
# (z2m default:2). See tests/README.md.
add_executable(zhc_onesti_parity_tests tests/test_onesti_parity.cpp)
target_link_libraries(zhc_onesti_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the Feibit (Heiman-OEM) family. Six IAS sensors
# (motion/smoke/CO/gas/water/contact) + the SEB01ZB SOS button wired the
# generic kFzIasZone against semantic exposes that the converter never
# produced; graduated to typed kFzIas<Type>Alarm (+ a new generic
# kFzIasSosAlarm2 for the SOS bit-1 key). STH01ZB dropped temperature +
# humidity (only kFzBattery lowered); TSKT222W-H4 dropped the 0x0B04
# electrical half of m.electricityMeter() cluster:"both". Two lift+tilt
# covers (SZT211_AW-P1, TCUR218W-V1) dropped the tilt channel — added a new
# generic kFzCoverTilt (attr 0x0009); TCUR218W-V1 also had wrong endpoint IDs.
# Five remotes/scene-switches (SZSN325W-Q, NZRC106W-M2, TZSN408W-V1,
# NZSN421W-Q, FMRC209W) were mis-modelled as dead settable on/off switches;
# graduated to command-action / IAS-ACE decoders. SSS401ZB regained its scene
# recall action while keeping its controllable switch. See tests/README.md.
add_executable(zhc_feibit_parity_tests tests/test_feibit_parity.cpp)
target_link_libraries(zhc_feibit_parity_tests PRIVATE zhc_definitions)

# Letsled HLC929-Z-RGBW-4C-IA-OTA-3.0: restored the colorTemp + color (xy/hs)
# axes the auto-generator dropped when collapsing m.light() to on/off+level.
add_executable(zhc_letsled_parity_tests tests/test_letsled_parity.cpp)
target_link_libraries(zhc_letsled_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_aldi_parity_tests tests/test_aldi_parity.cpp)
target_link_libraries(zhc_aldi_parity_tests PRIVATE zhc_definitions)
# TNCE TNCE_CLIMATE temp/humidity sensor: restored temperature (0x0402) +
# humidity (0x0405) channels the auto-port dropped to battery-only.
add_executable(zhc_tnce_parity_tests tests/test_tnce_parity.cpp)
target_link_libraries(zhc_tnce_parity_tests PRIVATE zhc_definitions)
# Hilux LED bulbs: DZ8 ("Spot 7W") is z2m m.light({colorTemp:{range:[153,370]}})
# but the auto-port collapsed it to on/off + brightness, dropping the CCT axis.
# Restored color_temp expose + lightingColorCtrl (0x0300) binding +
# kFzColorTemperature/kTzColorTemp. D160-ZG (plain m.light) is a negative
# control. See tests/README.md.
add_executable(zhc_hilux_parity_tests tests/test_hilux_parity.cpp)
target_link_libraries(zhc_hilux_parity_tests PRIVATE zhc_definitions)

# Jiawen RGBW bulb K2RGBW01 (m.light({colorTemp, color:true})): restored the
# dropped colorTemp + xy-color axes (exposes color_temp/color_xy, 0x0300 bind,
# kFzColorTemperature + kFzColor decoders, kTzColorTemp + kTzColor writes).
# JW-A04-CT (plain m.light) is a negative control. See tests/README.md.
add_executable(zhc_jiawen_parity_tests tests/test_jiawen_parity.cpp)
target_link_libraries(zhc_jiawen_parity_tests PRIVATE zhc_definitions)

# Brun Holding "Fire Fence" stove guard: restored short_override on EP2 (was
# colliding on EP1), regained the dropped m.temperature() decoder/expose/bind,
# and removed the phantom `voltage` expose (z2m disables voltage + current).
add_executable(zhc_brun_holding_parity_tests tests/test_brun_holding_parity.cpp)
target_link_libraries(zhc_brun_holding_parity_tests PRIVATE zhc_definitions)
# Parity for the Rademacher addZ colour-LED bulbs (35104001 / 35144001 /
# RDM-35274001). All three are full RGB+CCT m.light() devices that the
# auto-generator collapsed to on/off + brightness only — the colour-temperature
# and colour axes were dropped. The fix re-wires kFzColorTemperature + kFzColor
# and restores the matching exposes + lightingColorCtrl (0x0300) binding.
add_executable(zhc_rademacher_parity_tests tests/test_rademacher_parity.cpp)
target_link_libraries(zhc_rademacher_parity_tests PRIVATE zhc_definitions)

# Parity for the Domraem DOM-Z-105P "5 in 1" LED controller family. The
# auto-generator collapsed every m.light() to on/off + brightness, dropping the
# 0x0300 axes: RGB regained hs color (kFzColor); RGBW/RGBCCT regained colorTemp
# + xy + hs; CCT + WW/CW regained colorTemp. DIMMER stays clean.
add_executable(zhc_domraem_parity_tests tests/test_domraem_parity.cpp)
target_link_libraries(zhc_domraem_parity_tests PRIVATE zhc_definitions)

# Parity for the iStar SCCV24xx LED-controller family. The auto-generator
# collapsed every m.light() to on/off + brightness, dropping the 0x0300
# axes: SCCV2403-2 (CCT) regained colorTemp; SCCV2401-4 (RGBW) regained
# colorTemp + xy + hs. SCCV2401-1 (DIM) stays clean.
add_executable(zhc_istar_parity_tests tests/test_istar_parity.cpp)
target_link_libraries(zhc_istar_parity_tests PRIVATE zhc_definitions)

# Parity for the Shenzhen Homa LED-driver family. The auto-generator collapsed
# every m.light() to on/off + brightness, dropping the 0x0300 color axis: RGB +
# RGBW (m.light({color:true})) regained xy color (kFzColor); RGBW suffixes its
# color keys via the white/rgb endpoint_map; HLD503-Z-CT regained color_temp
# (kFzColorTemperature). Plain m.light() drivers stay on/off+brightness.
add_executable(zhc_shenzhen_homa_parity_tests tests/test_shenzhen_homa_parity.cpp)
target_link_libraries(zhc_shenzhen_homa_parity_tests PRIVATE zhc_definitions)

# Parity for the Viessmann (ViCare) family. The ZK03840 radiator TRV decoded
# manuSpec attr 0x4000 viessmannWindowOpenInternal as a flat `!= 0` boolean,
# but z2m only treats raw 3/4 as window-open (0-2/5 are "unknown"). Fixed in
# kFzViessmannThermostat. See tests/README.md for the pattern.
add_executable(zhc_viessmann_parity_tests tests/test_viessmann_parity.cpp)
target_link_libraries(zhc_viessmann_parity_tests PRIVATE zhc_definitions)
# Parity fixture for the Salus Controls vendor. The RE600 Zigbee router was
# auto-ported with a phantom on/off (state) + battery/voltage bundle and bound
# genOnOff/genPowerCfg; z2m declares it fromZigbee:[]/toZigbee:[]/exposes:[]
# (pure repeater). Graduated to a Tier-2 parent stripping it all. Also guards
# the already-correct SW600/OS600/WLS600 typed-IAS decode, the FC600 flat
# thermostat surface, and the SP600/SPE600/SX885ZB metering plugs (seMetering
# 0x0702 only, no phantom 0x0B04). See tests/README.md.
add_executable(zhc_salus_controls_parity_tests tests/test_salus_controls_parity.cpp)
target_link_libraries(zhc_salus_controls_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the KMPCIL multi-sensor family. The KMPCIL-tag-001
# arrival sensor declared e.battery() and z2m's presence_power converter
# applies meta.battery.voltageToPercentage "3V_1500_2800" to publish
# `battery`, but the ported kFzKmpcilPresencePower emitted only `voltage`
# — the battery expose was dead. The shared converter now also derives
# battery % from the non-linear 3V_1500_2800 formula. The def was
# graduated to a Tier-2 parent. The presentValue bitfield decoders
# (tag-001 presence/power_state/vibration/occupancy) and the RES005
# genBinaryInput/Output occupancy/state bespoke decoders are pinned as
# regression guards. See tests/README.md.
add_executable(zhc_kmpcil_parity_tests tests/test_kmpcil_parity.cpp)
target_link_libraries(zhc_kmpcil_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the RGB Genie scene/rotary remote family. ZB-1026 is the
# lone real m.light() dimmer (sink); the other seven (ZB-5001/5004/5121/5122,
# ZB-3008/3009, ZB-5028) are action senders the auto-gen wrongly bundled as
# settable on/off lights — already corrected to action+battery/colour bundles
# in definitions/rgb_genie/_shared.{hpp,cpp}. This suite pins remote shape
# (action expose, no settable state, empty toolbox), single- vs multi-EP
# action suffixing, battery decode, and LOCKS the ColorCtrl move decoders
# (command_move_hue / move_to_color / move_color_temperature) that stale port
# comments wrongly described as "not yet wired in _generic" — they are wired
# (kFzCommandMoveHue/MoveToColor/MoveColorTemperature). See tests/README.md.
add_executable(zhc_rgb_genie_parity_tests tests/test_rgb_genie_parity.cpp)
target_link_libraries(zhc_rgb_genie_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the AduroSmart ERIA power-measuring family. All three
# (81848 plug, 81998 relay, 81949 dimmer) use z2m electricityMeter
# ({cluster:"electrical"}) / fz.electrical_measurement — haElectricalMeasurement
# (0x0B04) ONLY: power/voltage/current, NO energy. The shared "EM" bundle
# wrongly also wired the generic seMetering (0x0702) decoder, declared a
# phantom `energy` expose and bound 0x0702. Fixed by making the kFzAdu*EM
# bundles electrical-only. See tests/README.md.
add_executable(zhc_adurosmart_parity_tests tests/test_adurosmart_parity.cpp)
target_link_libraries(zhc_adurosmart_parity_tests PRIVATE zhc_definitions)

add_executable(zhc_acuity_brands_lighting_parity_tests tests/test_acuity_brands_lighting_parity.cpp)
target_link_libraries(zhc_acuity_brands_lighting_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the EssentielB (Boulanger) family. All nine m.light()
# bulbs dropped their colour-temp axis (and the RGBW SKUs their colour axis)
# — kFzColorTemperature/kFzColor + the 0x0300 binding were missing. The
# EB-SB-1B smart button was mis-ported as a controllable on/off switch with
# a dead `state` + on/off TZ instead of a command-action remote.
add_executable(zhc_essentialb_parity_tests tests/test_essentialb_parity.cpp)
target_link_libraries(zhc_essentialb_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the Sercomm (Telstra/SmartThings OEM) family. The
# SZ-PIR04N PIR sensor dropped its illuminance channel (z2m adds
# m.illuminance() — msIlluminanceMeasurement 0x0400 — on top of motion/temp/
# battery); graduated to a parent override wiring the generic kFzIlluminance
# converter, the `illuminance` expose, and a 0x0400 binding. The rest of the
# family (contact/motion/water IAS sensors + metering plugs) are structural
# guards. See tests/README.md.
add_executable(zhc_sercomm_parity_tests tests/test_sercomm_parity.cpp)
target_link_libraries(zhc_sercomm_parity_tests PRIVATE zhc_definitions)
# Parity fixtures for the TERNCY AduroSmart-cluster (0xFCCC) raw-frame
# devices (PP01 awareness switch, SD01 knob, LS01 light socket). The
# shared terncy_raw decoder indexed the body by z2m's FULL-frame offsets
# (raw_body[4]/[6]/[7]) — but the mfg-specific header is 5 bytes, so
# raw_body starts at frame index 5. The discriminator is the COMMAND id
# (msg.command_id), and the action/side values live at raw_body[1]/[2];
# the old code read past the body and matched nothing on real frames.
add_executable(zhc_terncy_parity_tests tests/test_terncy_parity.cpp)
target_link_libraries(zhc_terncy_parity_tests PRIVATE zhc_definitions)

# BTicino (Legrand-family): F20T60A DIN power module was ported with the
# wrong cluster (seMetering 0x0702 + phantom energy) when z2m's
# electricityMeter(cluster:"electrical") uses haElectricalMeasurement
# (0x0B04) → power/voltage/current. K4003C light switch dropped the
# button action (genBinaryInput 0x000F presentValue → action on/off).
add_executable(zhc_bticino_parity_tests tests/test_bticino_parity.cpp)
target_link_libraries(zhc_bticino_parity_tests PRIVATE zhc_definitions)
add_executable(zhc_byun_parity_tests tests/test_byun_parity.cpp)
target_link_libraries(zhc_byun_parity_tests PRIVATE zhc_definitions)
add_executable(zhc_calex_parity_tests tests/test_calex_parity.cpp)
target_link_libraries(zhc_calex_parity_tests PRIVATE zhc_definitions)

# Villeroy & Boch — two CCT (tunable-white) lights, m.light({colorTemp:...}).
# Auto-port dropped the lightingColorCtrl (0x0300) colour-temp axis; this
# pins the restored color_temp expose + decode (no colour xy/hs axis).
add_executable(zhc_villeroy_boch_parity_tests tests/test_villeroy_boch_parity.cpp)
target_link_libraries(zhc_villeroy_boch_parity_tests PRIVATE zhc_definitions)

# Parity fixtures for the Lellki (Tuya-OEM) family. Two single-gang metering
# sockets (XF-EU-S100-1-M, WK34-EU) were ported as bare on/off, dropping the
# seMetering (0x0702) + haElectricalMeasurement (0x0B04) channels that z2m's
# tuyaOnOff({electricalMeasurements:true}) surfaces. These pin the restored
# power/current/voltage/energy decode + the unaffected siblings.
add_executable(zhc_lellki_parity_tests tests/test_lellki_parity.cpp)
target_link_libraries(zhc_lellki_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Vimar (wired Italian) family: the 02973.B IoT
# thermostat wired only the generic kFzThermostat (0x0000/0x0012/0x001C),
# leaving the occupied_cooling_setpoint (attr 0x0011) expose dead — z2m
# wires the full fz.thermostat. kFzVimarThermostatExtras
# (definitions/vimar/_shared.cpp) decodes it alongside the generic one,
# and the RemoteControl_v1.0 transmitter was mis-ported as an on/off
# switch instead of an action emitter (m.commandsOnOff/WindowCovering/
# LevelCtrl). See tests/README.md for the pattern.
add_executable(zhc_vimar_parity_tests tests/test_vimar_parity.cpp)
target_link_libraries(zhc_vimar_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Multiterm ZC0101 ZeeFan fan coil unit controller.
# The auto-port modelled it as a bare single `fan_mode` binary and dropped
# the entire multi-endpoint binary-output half — three genBinaryOutput
# (0x0010) channels on endpoints 8/9/10 (silent_mode/heating_cooling/
# electric_valve) had no decoder and would have collided on a bare `state`
# key. Wired the new generic kFzBinaryOutput (presentValue -> boolean state)
# and added an endpoint_map for ep9/ep10 (ep8 left unmapped so the fan
# control's hvacFanCtrl key stays unsuffixed). See tests/README.md.
add_executable(zhc_multiterm_parity_tests tests/test_multiterm_parity.cpp)
target_link_libraries(zhc_multiterm_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Zunzunbee SSWZ8T 8-button battery scene switch.
# The generated def collapsed it into a settable on/off `state`; z2m packs
# button + press type into the ssIasZone zoneStatus word and surfaces a
# composite `action`. Graduated to a Tier-2 override + vendor zoneStatus
# decoder (kFzZunzunbeeSlateSwitch). See tests/README.md.
add_executable(zhc_zunzunbee_parity_tests tests/test_zunzunbee_parity.cpp)
target_link_libraries(zhc_zunzunbee_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the two Smartenit defs. ZBHT-1 (temp/humidity sensor)
# was auto-ported to battery-only — restored fz.temperature (0x0402) +
# fz.humidity (0x0405). 4040B (dual-load metering switch) lost the z2m
# endpoint map {l1:1, l2:2}, collapsing both relays onto a bare "state";
# added endpoint_map (state_l1/state_l2) and kept power/energy device-global.
add_executable(zhc_smartenit_parity_tests tests/test_smartenit_parity.cpp)
target_link_libraries(zhc_smartenit_parity_tests PRIVATE zhc_definitions)

# Parity fixture for two BlitzWolf sensors the auto-port misrouted. BW-IS3
# (PIR) was wired to generic kFzIasZone (0x0500) with phantom
# alarm/tamper/battery_low, but z2m decodes it via
# fz.blitzwolf_occupancy_with_timeout — a Tuya 0xEF00 DP stream (DP3 ->
# occupancy); graduated to a Tier-2 Tuya-DP override. BW-IS9 (water leak) used
# generic kFzIasZone (bare alarm) while z2m's iasZoneAlarm bothAlarms config
# exposes water_leak_alarm_1 (bit0) + water_leak_alarm_2 (bit1); repointed to
# the new typed kFzIasWaterLeakAlarm1/2 converters.
add_executable(zhc_blitzwolf_parity_tests tests/test_blitzwolf_parity.cpp)
target_link_libraries(zhc_blitzwolf_parity_tests PRIVATE zhc_definitions)
# Parity fixture for the two Qmotion blind/shade defs. QZR-ZIG2400
# ("5 channel remote") was mis-ported as a controllable on/off cover —
# kFzOnOff, a writable `state` Binary expose, and phantom kTzOnOff +
# kTzCoverPosition write paths — whereas z2m's remote has an EMPTY
# toZigbee. Graduated to a Tier-2 override matching z2m: fz.identify
# (genIdentify → action) + read-only fz.cover_position_tilt position, no
# toZigbee, genOnOff binding dropped. HDM40PV620 verified already correct
# (single kTzCoverPosition claims both position + state).
add_executable(zhc_qmotion_parity_tests tests/test_qmotion_parity.cpp)
target_link_libraries(zhc_qmotion_parity_tests PRIVATE zhc_definitions)

# Parity fixture for the Titan Products TPZRCO2HT-Z3 room CO2/humidity/temperature
# sensor. The auto-generated def was reduced to battery-only — it dropped the
# temperature (0x0402), humidity (0x0405, ep2) and CO2 (0x040D) channels that
# z2m wires via fz.temperature/fz.humidity/fz.co2. The graduated def restores
# the three generic decoders + exposes + bindings.
add_executable(zhc_titan_products_parity_tests tests/test_titan_products_parity.cpp)
target_link_libraries(zhc_titan_products_parity_tests PRIVATE zhc_definitions)

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
add_test(NAME zhc_m_elec_parity_tests       COMMAND zhc_m_elec_parity_tests)
add_test(NAME zhc_hoftronic_parity_tests    COMMAND zhc_hoftronic_parity_tests)
add_test(NAME zhc_scanproducts_parity_tests COMMAND zhc_scanproducts_parity_tests)
add_test(NAME zhc_matcall_bv_parity_tests   COMMAND zhc_matcall_bv_parity_tests)
add_test(NAME zhc_lds_parity_tests          COMMAND zhc_lds_parity_tests)
add_test(NAME zhc_securifi_parity_tests     COMMAND zhc_securifi_parity_tests)
add_test(NAME zhc_smart9_parity_tests       COMMAND zhc_smart9_parity_tests)
add_test(NAME zhc_smart_home_pty_parity_tests COMMAND zhc_smart_home_pty_parity_tests)
add_test(NAME zhc_ozsmartthings_parity_tests COMMAND zhc_ozsmartthings_parity_tests)
add_test(NAME zhc_senoro_parity_tests       COMMAND zhc_senoro_parity_tests)
add_test(NAME zhc_commercial_electric_parity_tests COMMAND zhc_commercial_electric_parity_tests)
add_test(NAME zhc_swann_parity_tests        COMMAND zhc_swann_parity_tests)
add_test(NAME zhc_smarli_parity_tests       COMMAND zhc_smarli_parity_tests)
add_test(NAME zhc_soanalarm_parity_tests    COMMAND zhc_soanalarm_parity_tests)
add_test(NAME zhc_current_products_corp_parity_tests COMMAND zhc_current_products_corp_parity_tests)
add_test(NAME zhc_smartwings_parity_tests   COMMAND zhc_smartwings_parity_tests)
add_test(NAME zhc_schwaiger_parity_tests    COMMAND zhc_schwaiger_parity_tests)
add_test(NAME zhc_tci_parity_tests          COMMAND zhc_tci_parity_tests)
add_test(NAME zhc_tapestry_parity_tests     COMMAND zhc_tapestry_parity_tests)
add_test(NAME zhc_ysrsai_parity_tests       COMMAND zhc_ysrsai_parity_tests)
add_test(NAME zhc_wmun_parity_tests         COMMAND zhc_wmun_parity_tests)
add_test(NAME zhc_wyze_parity_tests         COMMAND zhc_wyze_parity_tests)
add_test(NAME zhc_ajax_online_parity_tests  COMMAND zhc_ajax_online_parity_tests)
add_test(NAME zhc_cleverio_parity_tests     COMMAND zhc_cleverio_parity_tests)
add_test(NAME zhc_atlantic_parity_tests     COMMAND zhc_atlantic_parity_tests)
add_test(NAME zhc_aubess_parity_tests       COMMAND zhc_aubess_parity_tests)
add_test(NAME zhc_lightsolutions_parity_tests COMMAND zhc_lightsolutions_parity_tests)
add_test(NAME zhc_casaia_parity_tests       COMMAND zhc_casaia_parity_tests)
add_test(NAME zhc_akuvox_parity_tests       COMMAND zhc_akuvox_parity_tests)
add_test(NAME zhc_alchemy_parity_tests      COMMAND zhc_alchemy_parity_tests)
add_test(NAME zhc_spotmau_parity_tests      COMMAND zhc_spotmau_parity_tests)
add_test(NAME zhc_heiman_parity_tests       COMMAND zhc_heiman_parity_tests)
add_test(NAME zhc_oujiabao_parity_tests     COMMAND zhc_oujiabao_parity_tests)
add_test(NAME zhc_homeseer_parity_tests     COMMAND zhc_homeseer_parity_tests)
add_test(NAME zhc_spacetronik_parity_tests  COMMAND zhc_spacetronik_parity_tests)
add_test(NAME zhc_hommyn_parity_tests       COMMAND zhc_hommyn_parity_tests)
add_test(NAME zhc_edp_parity_tests          COMMAND zhc_edp_parity_tests)
add_test(NAME zhc_prolight_parity_tests     COMMAND zhc_prolight_parity_tests)
add_test(NAME zhc_imou_parity_tests         COMMAND zhc_imou_parity_tests)
add_test(NAME zhc_hej_parity_tests          COMMAND zhc_hej_parity_tests)
add_test(NAME zhc_fantem_parity_tests       COMMAND zhc_fantem_parity_tests)
add_test(NAME zhc_hzc_electric_parity_tests COMMAND zhc_hzc_electric_parity_tests)
add_test(NAME zhc_woolley_parity_tests      COMMAND zhc_woolley_parity_tests)
add_test(NAME zhc_somfy_parity_tests        COMMAND zhc_somfy_parity_tests)
add_test(NAME zhc_chacon_parity_tests       COMMAND zhc_chacon_parity_tests)
add_test(NAME zhc_multiterm_parity_tests    COMMAND zhc_multiterm_parity_tests)
add_test(NAME zhc_climax_parity_tests       COMMAND zhc_climax_parity_tests)
add_test(NAME zhc_blaupunkt_parity_tests    COMMAND zhc_blaupunkt_parity_tests)
add_test(NAME zhc_lupus_parity_tests        COMMAND zhc_lupus_parity_tests)
add_test(NAME zhc_enbrighten_parity_tests   COMMAND zhc_enbrighten_parity_tests)
add_test(NAME zhc_lonsonho_parity_tests     COMMAND zhc_lonsonho_parity_tests)
add_test(NAME zhc_quotra_parity_tests       COMMAND zhc_quotra_parity_tests)
add_test(NAME zhc_slv_parity_tests          COMMAND zhc_slv_parity_tests)
add_test(NAME zhc_honyar_parity_tests       COMMAND zhc_honyar_parity_tests)
add_test(NAME zhc_makegood_parity_tests     COMMAND zhc_makegood_parity_tests)
add_test(NAME zhc_ge_parity_tests           COMMAND zhc_ge_parity_tests)
add_test(NAME zhc_sylvania_parity_tests     COMMAND zhc_sylvania_parity_tests)
add_test(NAME zhc_awox_parity_tests         COMMAND zhc_awox_parity_tests)
add_test(NAME zhc_eglo_parity_tests         COMMAND zhc_eglo_parity_tests)
add_test(NAME zhc_ecozy_parity_tests        COMMAND zhc_ecozy_parity_tests)
add_test(NAME zhc_adeo_parity_tests         COMMAND zhc_adeo_parity_tests)
add_test(NAME zhc_evology_parity_tests      COMMAND zhc_evology_parity_tests)
add_test(NAME zhc_philio_parity_tests       COMMAND zhc_philio_parity_tests)
add_test(NAME zhc_airam_parity_tests        COMMAND zhc_airam_parity_tests)
add_test(NAME zhc_net2grid_parity_tests     COMMAND zhc_net2grid_parity_tests)
add_test(NAME zhc_enocean_parity_tests      COMMAND zhc_enocean_parity_tests)
add_test(NAME zhc_led_trading_parity_tests  COMMAND zhc_led_trading_parity_tests)
add_test(NAME zhc_woox_parity_tests         COMMAND zhc_woox_parity_tests)
add_test(NAME zhc_linkind_parity_tests      COMMAND zhc_linkind_parity_tests)
add_test(NAME zhc_universal_electronics_inc_parity_tests COMMAND zhc_universal_electronics_inc_parity_tests)
add_test(NAME zhc_technicolor_parity_tests  COMMAND zhc_technicolor_parity_tests)
add_test(NAME zhc_ewelink_parity_tests      COMMAND zhc_ewelink_parity_tests)
add_test(NAME zhc_immax_parity_tests        COMMAND zhc_immax_parity_tests)
add_test(NAME zhc_iluminize_parity_tests    COMMAND zhc_iluminize_parity_tests)
add_test(NAME zhc_lidl_parity_tests         COMMAND zhc_lidl_parity_tests)
add_test(NAME zhc_orvibo_parity_tests       COMMAND zhc_orvibo_parity_tests)
add_test(NAME zhc_livingwise_parity_tests   COMMAND zhc_livingwise_parity_tests)
add_test(NAME zhc_linptech_parity_tests     COMMAND zhc_linptech_parity_tests)
add_test(NAME zhc_profalux_parity_tests     COMMAND zhc_profalux_parity_tests)
add_test(NAME zhc_develco_parity_tests      COMMAND zhc_develco_parity_tests)
add_test(NAME zhc_dlink_parity_tests        COMMAND zhc_dlink_parity_tests)
add_test(NAME zhc_databyte_parity_tests     COMMAND zhc_databyte_parity_tests)
add_test(NAME zhc_frient_parity_tests       COMMAND zhc_frient_parity_tests)
add_test(NAME zhc_smartthings_parity_tests  COMMAND zhc_smartthings_parity_tests)
add_test(NAME zhc_kami_parity_tests         COMMAND zhc_kami_parity_tests)
add_test(NAME zhc_jxuan_parity_tests        COMMAND zhc_jxuan_parity_tests)
add_test(NAME zhc_sengled_parity_tests      COMMAND zhc_sengled_parity_tests)
add_test(NAME zhc_halo_parity_tests         COMMAND zhc_halo_parity_tests)
add_test(NAME zhc_halemeier_parity_tests    COMMAND zhc_halemeier_parity_tests)
add_test(NAME zhc_legrand_parity_tests      COMMAND zhc_legrand_parity_tests)
add_test(NAME zhc_ikea_parity_tests         COMMAND zhc_ikea_parity_tests)
add_test(NAME zhc_siglis_parity_tests       COMMAND zhc_siglis_parity_tests)
add_test(NAME zhc_inovelli_parity_tests     COMMAND zhc_inovelli_parity_tests)
add_test(NAME zhc_jasco_parity_tests        COMMAND zhc_jasco_parity_tests)
add_test(NAME zhc_easyiot_parity_tests      COMMAND zhc_easyiot_parity_tests)
add_test(NAME zhc_echostar_parity_tests     COMMAND zhc_echostar_parity_tests)
add_test(NAME zhc_robb_parity_tests         COMMAND zhc_robb_parity_tests)
add_test(NAME zhc_repenic_ltd_parity_tests  COMMAND zhc_repenic_ltd_parity_tests)
add_test(NAME zhc_paul_neuhaus_parity_tests COMMAND zhc_paul_neuhaus_parity_tests)
add_test(NAME zhc_ecodim_parity_tests       COMMAND zhc_ecodim_parity_tests)
add_test(NAME zhc_vesternet_parity_tests    COMMAND zhc_vesternet_parity_tests)
add_test(NAME zhc_konke_parity_tests        COMMAND zhc_konke_parity_tests)
add_test(NAME zhc_multir_parity_tests       COMMAND zhc_multir_parity_tests)
add_test(NAME zhc_kwikset_parity_tests      COMMAND zhc_kwikset_parity_tests)
add_test(NAME zhc_weiser_parity_tests       COMMAND zhc_weiser_parity_tests)
add_test(NAME zhc_paulmann_parity_tests     COMMAND zhc_paulmann_parity_tests)
add_test(NAME zhc_evn_parity_tests          COMMAND zhc_evn_parity_tests)
add_test(NAME zhc_lutron_parity_tests       COMMAND zhc_lutron_parity_tests)
add_test(NAME zhc_hornbach_parity_tests     COMMAND zhc_hornbach_parity_tests)
add_test(NAME zhc_the_light_group_parity_tests COMMAND zhc_the_light_group_parity_tests)
add_test(NAME zhc_muller_licht_parity_tests COMMAND zhc_muller_licht_parity_tests)
add_test(NAME zhc_custom_devices_diy_parity_tests COMMAND zhc_custom_devices_diy_parity_tests)
add_test(NAME zhc_yale_parity_tests         COMMAND zhc_yale_parity_tests)
add_test(NAME zhc_shelly_parity_tests       COMMAND zhc_shelly_parity_tests)
add_test(NAME zhc_bacchus_parity_tests      COMMAND zhc_bacchus_parity_tests)
add_test(NAME zhc_diyruz_parity_tests       COMMAND zhc_diyruz_parity_tests)
add_test(NAME zhc_mindy_parity_tests        COMMAND zhc_mindy_parity_tests)
add_test(NAME zhc_pushok_parity_tests       COMMAND zhc_pushok_parity_tests)
add_test(NAME zhc_sber_parity_tests         COMMAND zhc_sber_parity_tests)
add_test(NAME zhc_lincukoo_parity_tests     COMMAND zhc_lincukoo_parity_tests)
add_test(NAME zhc_qa_parity_tests           COMMAND zhc_qa_parity_tests)
add_test(NAME zhc_sinope_parity_tests       COMMAND zhc_sinope_parity_tests)
add_test(NAME zhc_sunricher_parity_tests    COMMAND zhc_sunricher_parity_tests)
add_test(NAME zhc_third_reality_parity_tests COMMAND zhc_third_reality_parity_tests)
add_test(NAME zhc_bitron_parity_tests       COMMAND zhc_bitron_parity_tests)
add_test(NAME zhc_brimate_parity_tests      COMMAND zhc_brimate_parity_tests)
add_test(NAME zhc_tplink_parity_tests       COMMAND zhc_tplink_parity_tests)
add_test(NAME zhc_nyce_parity_tests         COMMAND zhc_nyce_parity_tests)
add_test(NAME zhc_namron_parity_tests       COMMAND zhc_namron_parity_tests)
add_test(NAME zhc_danfoss_parity_tests      COMMAND zhc_danfoss_parity_tests)
add_test(NAME zhc_leviton_parity_tests      COMMAND zhc_leviton_parity_tests)
add_test(NAME zhc_zen_parity_tests          COMMAND zhc_zen_parity_tests)
add_test(NAME zhc_yandex_parity_tests       COMMAND zhc_yandex_parity_tests)
add_test(NAME zhc_ctm_parity_tests          COMMAND zhc_ctm_parity_tests)
add_test(NAME zhc_jethome_parity_tests      COMMAND zhc_jethome_parity_tests)
add_test(NAME zhc_letv_parity_tests         COMMAND zhc_letv_parity_tests)
add_test(NAME zhc_ls_parity_tests           COMMAND zhc_ls_parity_tests)
add_test(NAME zhc_livolo_parity_tests       COMMAND zhc_livolo_parity_tests)
add_test(NAME zhc_easyaccess_parity_tests   COMMAND zhc_easyaccess_parity_tests)
add_test(NAME zhc_bosch_parity_tests        COMMAND zhc_bosch_parity_tests)
add_test(NAME zhc_ubisys_parity_tests       COMMAND zhc_ubisys_parity_tests)
add_test(NAME zhc_lux_parity_tests          COMMAND zhc_lux_parity_tests)
add_test(NAME zhc_osram_parity_tests        COMMAND zhc_osram_parity_tests)
add_test(NAME zhc_nodon_parity_tests        COMMAND zhc_nodon_parity_tests)
add_test(NAME zhc_nordtronic_parity_tests   COMMAND zhc_nordtronic_parity_tests)
add_test(NAME zhc_novo_parity_tests         COMMAND zhc_novo_parity_tests)
add_test(NAME zhc_owon_parity_tests         COMMAND zhc_owon_parity_tests)
add_test(NAME zhc_airzone_aidoo_parity_tests COMMAND zhc_airzone_aidoo_parity_tests)
add_test(NAME zhc_imhotepcreation_parity_tests COMMAND zhc_imhotepcreation_parity_tests)
add_test(NAME zhc_icasa_parity_tests        COMMAND zhc_icasa_parity_tests)
add_test(NAME zhc_plugwise_parity_tests     COMMAND zhc_plugwise_parity_tests)
add_test(NAME zhc_plaid_parity_tests        COMMAND zhc_plaid_parity_tests)
add_test(NAME zhc_vsmart_parity_tests       COMMAND zhc_vsmart_parity_tests)
add_test(NAME zhc_innr_parity_tests         COMMAND zhc_innr_parity_tests)
add_test(NAME zhc_bituo_technik_parity_tests COMMAND zhc_bituo_technik_parity_tests)
add_test(NAME zhc_meazon_parity_tests       COMMAND zhc_meazon_parity_tests)
add_test(NAME zhc_lifecontrol_parity_tests  COMMAND zhc_lifecontrol_parity_tests)
add_test(NAME zhc_amina_parity_tests        COMMAND zhc_amina_parity_tests)
add_test(NAME zhc_xyzroe_parity_tests       COMMAND zhc_xyzroe_parity_tests)
add_test(NAME zhc_hive_parity_tests         COMMAND zhc_hive_parity_tests)
add_test(NAME zhc_lytko_parity_tests        COMMAND zhc_lytko_parity_tests)
add_test(NAME zhc_slacky_therm_r08_tests    COMMAND zhc_slacky_therm_r08_tests)
add_test(NAME zhc_engo_parity_tests         COMMAND zhc_engo_parity_tests)
add_test(NAME zhc_evanell_parity_tests      COMMAND zhc_evanell_parity_tests)
add_test(NAME zhc_envilar_parity_tests      COMMAND zhc_envilar_parity_tests)
add_test(NAME zhc_siterwell_parity_tests    COMMAND zhc_siterwell_parity_tests)
add_test(NAME zhc_tech_parity_tests         COMMAND zhc_tech_parity_tests)
add_test(NAME zhc_essentials_parity_tests   COMMAND zhc_essentials_parity_tests)
add_test(NAME zhc_onenuo_parity_tests       COMMAND zhc_onenuo_parity_tests)
add_test(NAME zhc_giex_parity_tests         COMMAND zhc_giex_parity_tests)
add_test(NAME zhc_iotperfect_parity_tests   COMMAND zhc_iotperfect_parity_tests)
add_test(NAME zhc_qoto_parity_tests         COMMAND zhc_qoto_parity_tests)
add_test(NAME zhc_alecto_parity_tests       COMMAND zhc_alecto_parity_tests)
add_test(NAME zhc_ilightsin_parity_tests    COMMAND zhc_ilightsin_parity_tests)
add_test(NAME zhc_simpla_home_parity_tests  COMMAND zhc_simpla_home_parity_tests)
add_test(NAME zhc_zemismart_parity_tests    COMMAND zhc_zemismart_parity_tests)
add_test(NAME zhc_ynoa_parity_tests         COMMAND zhc_ynoa_parity_tests)
add_test(NAME zhc_avatto_parity_tests       COMMAND zhc_avatto_parity_tests)
add_test(NAME zhc_girier_parity_tests       COMMAND zhc_girier_parity_tests)
add_test(NAME zhc_xenon_parity_tests        COMMAND zhc_xenon_parity_tests)
add_test(NAME zhc_xal_parity_tests          COMMAND zhc_xal_parity_tests)
add_test(NAME zhc_javis_parity_tests        COMMAND zhc_javis_parity_tests)
add_test(NAME zhc_weten_parity_tests        COMMAND zhc_weten_parity_tests)
add_test(NAME zhc_nedis_parity_tests        COMMAND zhc_nedis_parity_tests)
add_test(NAME zhc_saswell_parity_tests      COMMAND zhc_saswell_parity_tests)
add_test(NAME zhc_mazda_parity_tests        COMMAND zhc_mazda_parity_tests)
add_test(NAME zhc_neo_parity_tests          COMMAND zhc_neo_parity_tests)
add_test(NAME zhc_bseed_parity_tests        COMMAND zhc_bseed_parity_tests)
add_test(NAME zhc_nous_parity_tests         COMMAND zhc_nous_parity_tests)
add_test(NAME zhc_automaton_parity_tests    COMMAND zhc_automaton_parity_tests)
add_test(NAME zhc_shinasystem_parity_tests  COMMAND zhc_shinasystem_parity_tests)
add_test(NAME zhc_fireangel_parity_tests    COMMAND zhc_fireangel_parity_tests)
add_test(NAME zhc_dawon_dns_parity_tests    COMMAND zhc_dawon_dns_parity_tests)
add_test(NAME zhc_dqsmart_parity_tests      COMMAND zhc_dqsmart_parity_tests)
add_test(NAME zhc_wirenboard_parity_tests   COMMAND zhc_wirenboard_parity_tests)
add_test(NAME zhc_niko_parity_tests         COMMAND zhc_niko_parity_tests)
add_test(NAME zhc_perenio_parity_tests      COMMAND zhc_perenio_parity_tests)
add_test(NAME zhc_lellki_parity_tests       COMMAND zhc_lellki_parity_tests)
add_test(NAME zhc_stelpro_parity_tests      COMMAND zhc_stelpro_parity_tests)
add_test(NAME zhc_stello_parity_tests       COMMAND zhc_stello_parity_tests)
add_test(NAME zhc_netica_parity_tests       COMMAND zhc_netica_parity_tests)
add_test(NAME zhc_purmo_parity_tests        COMMAND zhc_purmo_parity_tests)
add_test(NAME zhc_acova_parity_tests        COMMAND zhc_acova_parity_tests)
add_test(NAME zhc_vav_parity_tests          COMMAND zhc_vav_parity_tests)
add_test(NAME zhc_espressif_parity_tests    COMMAND zhc_espressif_parity_tests)
add_test(NAME zhc_tubeszb_parity_tests      COMMAND zhc_tubeszb_parity_tests)
add_test(NAME zhc_intuis_parity_tests       COMMAND zhc_intuis_parity_tests)
add_test(NAME zhc_nobo_parity_tests         COMMAND zhc_nobo_parity_tests)
add_test(NAME zhc_aurora_lighting_parity_tests COMMAND zhc_aurora_lighting_parity_tests)
add_test(NAME zhc_wally_parity_tests        COMMAND zhc_wally_parity_tests)
add_test(NAME zhc_onesti_parity_tests       COMMAND zhc_onesti_parity_tests)
add_test(NAME zhc_feibit_parity_tests       COMMAND zhc_feibit_parity_tests)
add_test(NAME zhc_letsled_parity_tests      COMMAND zhc_letsled_parity_tests)
add_test(NAME zhc_aldi_parity_tests         COMMAND zhc_aldi_parity_tests)
add_test(NAME zhc_tnce_parity_tests         COMMAND zhc_tnce_parity_tests)
add_test(NAME zhc_hilux_parity_tests        COMMAND zhc_hilux_parity_tests)
add_test(NAME zhc_jiawen_parity_tests       COMMAND zhc_jiawen_parity_tests)
add_test(NAME zhc_brun_holding_parity_tests COMMAND zhc_brun_holding_parity_tests)
add_test(NAME zhc_rademacher_parity_tests   COMMAND zhc_rademacher_parity_tests)
add_test(NAME zhc_domraem_parity_tests      COMMAND zhc_domraem_parity_tests)
add_test(NAME zhc_istar_parity_tests        COMMAND zhc_istar_parity_tests)
add_test(NAME zhc_shenzhen_homa_parity_tests COMMAND zhc_shenzhen_homa_parity_tests)
add_test(NAME zhc_moes_bht002_tests         COMMAND zhc_moes_bht002_tests)
add_test(NAME zhc_moes_zht_sr_tests         COMMAND zhc_moes_zht_sr_tests)
add_test(NAME zhc_zosung_ir_tests           COMMAND zhc_zosung_ir_tests)
add_test(NAME zhc_zg_204z_tests             COMMAND zhc_zg_204z_tests)
add_test(NAME zhc_ts0044_tests              COMMAND zhc_ts0044_tests)
add_test(NAME zhc_ts004f_knob_tests         COMMAND zhc_ts004f_knob_tests)
add_test(NAME zhc_datek_parity_tests        COMMAND zhc_datek_parity_tests)
add_test(NAME zhc_danalock_parity_tests     COMMAND zhc_danalock_parity_tests)
add_test(NAME zhc_schlage_parity_tests      COMMAND zhc_schlage_parity_tests)
add_test(NAME zhc_mercator_parity_tests     COMMAND zhc_mercator_parity_tests)
add_test(NAME zhc_mill_parity_tests         COMMAND zhc_mill_parity_tests)
add_test(NAME zhc_hampton_bay_parity_tests  COMMAND zhc_hampton_bay_parity_tests)
add_test(NAME zhc_viessmann_parity_tests    COMMAND zhc_viessmann_parity_tests)
add_test(NAME zhc_salus_controls_parity_tests COMMAND zhc_salus_controls_parity_tests)
add_test(NAME zhc_kmpcil_parity_tests       COMMAND zhc_kmpcil_parity_tests)
add_test(NAME zhc_rgb_genie_parity_tests    COMMAND zhc_rgb_genie_parity_tests)
add_test(NAME zhc_adurosmart_parity_tests   COMMAND zhc_adurosmart_parity_tests)
add_test(NAME zhc_acuity_brands_lighting_parity_tests COMMAND zhc_acuity_brands_lighting_parity_tests)
add_test(NAME zhc_essentialb_parity_tests   COMMAND zhc_essentialb_parity_tests)
add_test(NAME zhc_sercomm_parity_tests      COMMAND zhc_sercomm_parity_tests)
add_test(NAME zhc_terncy_parity_tests       COMMAND zhc_terncy_parity_tests)
add_test(NAME zhc_bticino_parity_tests      COMMAND zhc_bticino_parity_tests)
add_test(NAME zhc_byun_parity_tests         COMMAND zhc_byun_parity_tests)
add_test(NAME zhc_calex_parity_tests        COMMAND zhc_calex_parity_tests)
add_test(NAME zhc_villeroy_boch_parity_tests COMMAND zhc_villeroy_boch_parity_tests)
add_test(NAME zhc_insta_parity_tests        COMMAND zhc_insta_parity_tests)
add_test(NAME zhc_futurehome_parity_tests   COMMAND zhc_futurehome_parity_tests)
add_test(NAME zhc_dresden_elektronik_parity_tests COMMAND zhc_dresden_elektronik_parity_tests)
add_test(NAME zhc_gewiss_parity_tests       COMMAND zhc_gewiss_parity_tests)
add_test(NAME zhc_aeotec_parity_tests       COMMAND zhc_aeotec_parity_tests)
add_test(NAME zhc_netvox_parity_tests       COMMAND zhc_netvox_parity_tests)
add_test(NAME zhc_visonic_parity_tests      COMMAND zhc_visonic_parity_tests)
add_test(NAME zhc_peq_parity_tests          COMMAND zhc_peq_parity_tests)
add_test(NAME zhc_ihorn_parity_tests        COMMAND zhc_ihorn_parity_tests)
add_test(NAME zhc_somgoms_parity_tests      COMMAND zhc_somgoms_parity_tests)
add_test(NAME zhc_ecolink_parity_tests      COMMAND zhc_ecolink_parity_tests)
add_test(NAME zhc_roome_parity_tests        COMMAND zhc_roome_parity_tests)
add_test(NAME zhc_titan_products_parity_tests COMMAND zhc_titan_products_parity_tests)
add_test(NAME zhc_vimar_parity_tests        COMMAND zhc_vimar_parity_tests)
add_test(NAME zhc_samotech_parity_tests     COMMAND zhc_samotech_parity_tests)
add_test(NAME zhc_gs_parity_tests           COMMAND zhc_gs_parity_tests)
add_test(NAME zhc_msh_parity_tests          COMMAND zhc_msh_parity_tests)
add_test(NAME zhc_zunzunbee_parity_tests    COMMAND zhc_zunzunbee_parity_tests)
add_test(NAME zhc_smartenit_parity_tests    COMMAND zhc_smartenit_parity_tests)
add_test(NAME zhc_blitzwolf_parity_tests    COMMAND zhc_blitzwolf_parity_tests)
add_test(NAME zhc_ITCommander_parity_tests  COMMAND zhc_ITCommander_parity_tests)
add_test(NAME zhc_qmotion_parity_tests      COMMAND zhc_qmotion_parity_tests)
add_test(NAME zhc_bega_parity_tests         COMMAND zhc_bega_parity_tests)
add_test(NAME zhc_frankever_parity_tests    COMMAND zhc_frankever_parity_tests)
add_test(NAME zhc_seastar_intelligence_parity_tests COMMAND zhc_seastar_intelligence_parity_tests)
add_test(NAME zhc_shyugj_parity_tests       COMMAND zhc_shyugj_parity_tests)
add_test(NAME zhc_atsmart_parity_tests      COMMAND zhc_atsmart_parity_tests)
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
