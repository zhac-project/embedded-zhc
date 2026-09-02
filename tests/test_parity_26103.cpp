// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// z2m v26.101.0 → v26.103.0 (zigbee2mqtt 2.14.0) parity window. Pins the
// codecs that were hand-written this window and the fingerprints that were
// folded into hand definitions:
//
//   * Schneider CCTFR6400 boost command 0x80 — body layout and the
//     set / cancel split (cancel carries temperature 0xFFF and duration 0).
//   * Heiwa HPZERAD-V1 — vendor attribute decode (scaled temperatures, the
//     battery step table, profile → remote_power, setpoint step), the
//     two-record setpoint write and a signed scaled offset write.
//   * PushOk POK001 — the widened valve status lookup and the `inverted`
//     read/write pair on genBinaryOutput.
//   * Bosch BTH-RM230Z — humidity_alarm_led decode + the 0x07/0x06 write.
//   * Fingerprints: every manufacturer name this window added, and the
//     SJCGQ12LM-ES modelID with its three trailing control bytes.
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <vector>
#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::schneider { extern const PreparedDefinition kDef_CCTFR6400; }
namespace zhc::devices::heiwa     { extern const PreparedDefinition kDef_HPZERAD_V1; }
namespace zhc::devices::pushok    { extern const PreparedDefinition kDef_POK001; }
namespace zhc::devices::bosch     { extern const PreparedDefinition kDef_BTH_RM230Z; }
namespace zhc::devices::tuya      { extern const PreparedDefinition kDefTS0601_cover;
                                    extern const PreparedDefinition kDef_TS0001_bzzgvet0;
                                    extern const PreparedDefinition kDef_MG_DIM02Z;
                                    extern const PreparedDefinition kDef_ZTH05Z_onenuo; }
namespace zhc::devices::zemismart { extern const PreparedDefinition kDef_ZMS_206US_4;
                                    extern const PreparedDefinition kDef_ZMS_206EU_2; }
namespace zhc::devices::nous      { extern const PreparedDefinition kDef_D4Z; }
namespace zhc::devices::lonsonho  { extern const PreparedDefinition kDef_TS130F_dual; }
namespace zhc::devices::lumi      { extern const PreparedDefinition kDefSJCGQ12LM_ES;
                                    extern const PreparedDefinition kDefSJCGQ12LM; }
namespace zhc::devices::moes      { extern const PreparedDefinition kDef_SFD02_Z; }
namespace zhc::devices::mowe      { extern const PreparedDefinition kDef_MW833P; }
namespace zhc::devices::rti_tek   { extern const PreparedDefinition kDef_STHZB; }

using namespace zhc;

namespace {

int g_failures = 0;
void check(bool cond, const char* what) {
    if (!cond) { std::printf("  FAIL: %s\n", what); ++g_failures; }
}

DispatchResult dispatch(const PreparedDefinition& def, std::uint16_t cluster_id,
                        std::uint8_t src_ep, std::span<const std::uint8_t> bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = src_ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(cluster_id);
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Global Report Attributes: [fc 0x18][tsn][0x0A] + records.
struct Report {
    std::vector<std::uint8_t> v{0x18, 0x01, 0x0A};
    Report& u8(std::uint16_t attr, std::uint8_t type, std::uint8_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type, val}); return *this; }
    Report& u16(std::uint16_t attr, std::uint8_t type, std::uint16_t val) {
        v.insert(v.end(), {std::uint8_t(attr & 0xFF), std::uint8_t(attr >> 8), type,
                           std::uint8_t(val & 0xFF), std::uint8_t(val >> 8)}); return *this; }
};

bool str_is(const DispatchResult& r, const char* key, const char* expected) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, expected) == 0;
}
bool float_is(const DispatchResult& r, const char* key, float expected) {
    const Value* v = r.merged.find(key);
    if (!v) return false;
    const float f = v->type == ValueType::Float ? v->f
                  : v->type == ValueType::Int   ? static_cast<float>(v->i)
                  : v->type == ValueType::Uint  ? static_cast<float>(v->u) : -1e9f;
    return std::fabs(f - expected) < 0.001f;
}
bool bool_is(const DispatchResult& r, const char* key, bool expected) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Bool && v->b == expected;
}
bool has_manu(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.manufacturer_names_count; ++i)
        if (d.manufacturer_names[i] && std::strcmp(d.manufacturer_names[i], name) == 0) return true;
    return false;
}
bool has_expose(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (std::strcmp(d.exposes[i].name, name) == 0) return true;
    return false;
}
bool frame_is(const std::uint8_t* got, std::size_t n, std::initializer_list<std::uint8_t> want) {
    if (n != want.size()) return false;
    std::size_t i = 0;
    for (auto b : want) if (got[i++] != b) return false;
    return true;
}

void test_cctfr6400_boost() {
    std::printf("schneider CCTFR6400 boost command\n");
    const auto& def = devices::schneider::kDef_CCTFR6400;
    // [fc 0x05 = cluster-specific + mfg-specific, client->server][mfg 0x105E][tsn][cmd 0x80]
    // body: command 0, enable 1, temperature 2500 (25.00 C), duration 30 min
    const std::uint8_t set[] = {0x05, 0x5E, 0x10, 0x42, 0x80, 0x00, 0x01, 0xC4, 0x09, 0x1E, 0x00};
    auto r = dispatch(def, 0x0201, 1, set);
    check(r.any_matched, "boost_set matched");
    check(str_is(r, "action", "boost_set"), "action == boost_set");
    check(float_is(r, "boost_duration", 30.f), "boost_duration == 30");
    check(float_is(r, "boost_temperature", 25.f), "boost_temperature == 25.0");
    // cancel: enable 0, temperature 0xFFF, duration 0
    const std::uint8_t cancel[] = {0x05, 0x5E, 0x10, 0x43, 0x80, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x00};
    r = dispatch(def, 0x0201, 1, cancel);
    check(str_is(r, "action", "boost_cancel"), "action == boost_cancel");
    check(float_is(r, "boost_duration", 0.f), "cancel duration == 0");
    check(r.merged.find("boost_temperature") == nullptr, "cancel carries no boost_temperature");
    check(has_expose(def, "boost_temperature"), "boost_temperature exposed");
}

void test_heiwa_decode() {
    std::printf("heiwa HPZERAD-V1 attribute decode (endpoint 25)\n");
    const auto& def = devices::heiwa::kDef_HPZERAD_V1;
    Report rep;
    rep.u16(0x0420, 0x29, 215)    // displayTemperature 21.5 C
       .u16(0x040F, 0x21, 595)    // battery 5.95 V -> 75 %
       .u8 (0x0474, 0x30, 3)      // activeProfile comfort
       .u8 (0x0466, 0x30, 1);     // setpointStep 0.5
    auto r = dispatch(def, 0x0201, 25, rep.v);
    check(r.any_matched, "report matched");
    check(float_is(r, "local_temperature", 21.5f), "local_temperature 21.5");
    check(float_is(r, "display_temperature", 21.5f), "display_temperature 21.5");
    check(float_is(r, "battery", 75.f), "battery 75 %");
    check(float_is(r, "battery_voltage", 5.95f), "battery_voltage 5.95");
    check(str_is(r, "active_profile", "comfort"), "active_profile comfort");
    check(str_is(r, "profile", "comfort"), "profile comfort");
    check(str_is(r, "remote_power", "ON"), "remote_power ON");
    check(float_is(r, "setpoint_step", 0.5f), "setpoint_step 0.5");

    Report rep2;
    rep2.u8(0x001C, 0x30, 0)      // systemMode off
        .u8(0x0475, 0x30, 1)      // heating icon
        .u8(0x0476, 0x30, 0)
        .u16(0x0421, 0x29, static_cast<std::uint16_t>(-25));   // temperature_offset -2.5
    r = dispatch(def, 0x0201, 25, rep2.v);
    check(str_is(r, "system_mode", "off"), "system_mode off");
    check(str_is(r, "remote_power", "OFF"), "remote_power OFF when off");
    check(str_is(r, "zone_demand_icon", "heating"), "zone_demand_icon heating");
    check(float_is(r, "temperature_offset", -2.5f), "temperature_offset -2.5 (signed)");
    check(def.default_endpoint == 25, "default_endpoint 25");
}

void test_heiwa_writes() {
    std::printf("heiwa HPZERAD-V1 writes\n");
    const auto& def = devices::heiwa::kDef_HPZERAD_V1;
    RuntimeContext ctx{};
    std::uint8_t frame[48]{};
    Value v{}; v.type = ValueType::Float; v.f = 20.0f;
    auto r = dispatch_to_zigbee(def, "current_heating_setpoint", v, ctx, frame);
    check(r.ok && r.cluster_id == 0x0201 && r.command_id == 0x02, "setpoint routed to thermostat write");
    // cooling 18.5 -> 1850 = 0x073A, heating 21.5 -> 2150 = 0x0866
    check(frame_is(frame, r.frame_size, {0x10, 0x00, 0x02, 0x11, 0x00, 0x29, 0x3A, 0x07,
                                          0x12, 0x00, 0x29, 0x66, 0x08}), "setpoint = cooling/heating pair");
    v.f = 20.3f;
    r = dispatch_to_zigbee(def, "current_heating_setpoint", v, ctx, frame);
    check(!r.ok, "setpoint off the 0.5 grid refused");
    v.f = 30.0f;
    r = dispatch_to_zigbee(def, "current_heating_setpoint", v, ctx, frame);
    check(!r.ok, "setpoint above 27 refused");

    v.f = -2.5f;
    r = dispatch_to_zigbee(def, "temperature_offset", v, ctx, frame);
    check(r.ok, "temperature_offset write ok");
    check(frame_is(frame, r.frame_size, {0x10, 0x00, 0x02, 0x21, 0x04, 0x29, 0xE7, 0xFF}),
          "temperature_offset -2.5 -> s16 -25");

    Value s{}; s.type = ValueType::StringRef; s.str = "heating";
    r = dispatch_to_zigbee(def, "zone_demand_icon", s, ctx, frame);
    check(r.ok, "zone_demand_icon write ok");
    check(frame_is(frame, r.frame_size, {0x10, 0x00, 0x02, 0x76, 0x04, 0x30, 0x00,
                                          0x75, 0x04, 0x30, 0x01}), "heating: cooling icon cleared first");

    s.str = "eco";
    r = dispatch_to_zigbee(def, "profile", s, ctx, frame);
    check(r.ok && frame_is(frame, r.frame_size, {0x10, 0x00, 0x02, 0x73, 0x04, 0x30, 0x01}),
          "profile eco -> 0x0473 = 1");
}

void test_pok001() {
    std::printf("pushok POK001 status + inverted\n");
    const auto& def = devices::pushok::kDef_POK001;
    Report st; st.u16(0x0055, 0x21, 6);
    auto r = dispatch(def, 0x0012, 1, st.v);
    check(str_is(r, "status", "BLOCKED"), "status 6 == BLOCKED");
    Report st2; st2.u16(0x0055, 0x21, 2);
    r = dispatch(def, 0x0012, 1, st2.v);
    check(str_is(r, "status", "MOVING"), "status 2 == MOVING");
    Report inv; inv.u8(0x0055, 0x10, 1);
    r = dispatch(def, 0x0010, 1, inv.v);
    check(bool_is(r, "inverted", true), "inverted true");
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = true;
    auto w = dispatch_to_zigbee(def, "inverted", v, ctx, frame);
    check(w.ok && w.cluster_id == 0x0010, "inverted write routed to genBinaryOutput");
    check(frame_is(frame, w.frame_size, {0x10, 0x00, 0x02, 0x55, 0x00, 0x10, 0x01}), "inverted write frame");
    check(!has_expose(def, "offline_action"), "offline_action not exposed (no float32 path)");
}

void test_bosch_rm230z() {
    std::printf("bosch BTH-RM230Z humidity_alarm_led\n");
    const auto& def = devices::bosch::kDef_BTH_RM230Z;
    Report led; led.u8(0x4023, 0x30, 0x07);
    auto r = dispatch(def, 0x0201, 1, led.v);
    check(bool_is(r, "humidity_alarm_led", true), "0x07 -> on");
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::Bool; v.b = false;
    auto w = dispatch_to_zigbee(def, "humidity_alarm_led", v, ctx, frame);
    check(w.ok && frame_is(frame, w.frame_size, {0x14, 0x09, 0x12, 0x00, 0x02, 0x23, 0x40, 0x30, 0x06}),
          "off -> mfg-specific write of 0x06");
    check(has_expose(def, "humidity") && !has_expose(def, "battery"), "humidity in, battery out");
}

void test_fingerprints() {
    std::printf("fingerprints folded this window\n");
    check(has_manu(devices::tuya::kDefTS0601_cover, "_TZE28C1000000_1fuxihti") &&
          has_manu(devices::tuya::kDefTS0601_cover, "_TZE204_1fuxihti"), "TS0601_cover carries 1fuxihti");
    check(devices::tuya::kDefTS0601_cover.manufacturer_names_count == 12, "TS0601_cover count is sizeof-derived (12)");
    check(has_manu(devices::tuya::kDef_ZTH05Z_onenuo, "_TZE284_qf5mzewi") &&
          has_manu(devices::tuya::kDef_ZTH05Z_onenuo, "_TZE2841000000_qf5mzewi"), "ONENUO TH05Z both names");
    check(has_manu(devices::zemismart::kDef_ZMS_206US_4, "_TZE28C1000000_xibaabmu") &&
          has_manu(devices::zemismart::kDef_ZMS_206US_4, "_TZE28C1000000_y4jqpry8") &&
          devices::zemismart::kDef_ZMS_206US_4.manufacturer_names_count == 9, "ZMS-206US-4 9 names");
    check(has_manu(devices::zemismart::kDef_ZMS_206EU_2, "_TZE28C1000000_dmckrsxg") &&
          devices::zemismart::kDef_ZMS_206EU_2.manufacturer_names_count == 4, "ZMS-206EU-2 4 names");
    check(has_expose(devices::zemismart::kDef_ZMS_206US_4, "relay_status_l4"), "ZMS-206US-4 relay_status_l4 wired");
    check(has_manu(devices::nous::kDef_D4Z, "_TZE200_loejka0i") &&
          devices::nous::kDef_D4Z.manufacturer_names_count == 3, "Nous D4Z 3 names");
    check(has_expose(devices::nous::kDef_D4Z, "faults"), "Nous D4Z faults wired");
    check(has_manu(devices::lonsonho::kDef_TS130F_dual, "_TZ3000_wvedmwyp"), "TS130F_dual wvedmwyp");
    check(has_manu(devices::tuya::kDef_TS0001_bzzgvet0, "_TZ3000_bzzgvet0") &&
          has_expose(devices::tuya::kDef_TS0001_bzzgvet0, "switch_type"), "TS0001 bzzgvet0 variant");
    check(has_manu(devices::moes::kDef_SFD02_Z, "_TZE284_t88bjhfu"), "SFD02-Z");
    check(has_manu(devices::mowe::kDef_MW833P, "_TZE200_ops9sidw"), "MW833P");
    check(has_manu(devices::tuya::kDef_MG_DIM02Z, "_TZE284_da26abzz"), "MG-DIM02Z");
    check(std::strcmp(devices::rti_tek::kDef_STHZB.model, "STHZB") == 0, "STH1Z renamed STHZB");
    const auto& es = devices::lumi::kDefSJCGQ12LM_ES;
    check(std::strcmp(es.zigbee_models[0], "lumi.flood.agl02\tF\x01") == 0 &&
          std::strlen(es.zigbee_models[0]) == 19, "SJCGQ12LM-ES modelID keeps \\t F \\x01");
    check(has_expose(devices::lumi::kDefSJCGQ12LM, "water_leak"), "SJCGQ12LM water_leak wired");
}

void test_sjcgq12lm_leak() {
    std::printf("lumi SJCGQ12LM IAS water leak\n");
    // IAS Zone Status Change Notification: fc 0x09, tsn, cmd 0x00,
    // zoneStatus LE (alarm_1 set), extStatus, zoneId, delay LE.
    const std::uint8_t z[] = {0x09, 0x42, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto r = dispatch(devices::lumi::kDefSJCGQ12LM, 0x0500, 1, z);
    check(bool_is(r, "water_leak", true), "alarm_1 -> water_leak true");
    r = dispatch(devices::lumi::kDefSJCGQ12LM_ES, 0x0500, 1, z);
    check(bool_is(r, "water_leak", true), "ES variant decodes the same");
}

}  // namespace

int main() {
    test_cctfr6400_boost();
    test_heiwa_decode();
    test_heiwa_writes();
    test_pok001();
    test_bosch_rm230z();
    test_fingerprints();
    test_sjcgq12lm_leak();
    if (g_failures) { std::printf("%d failure(s)\n", g_failures); return 1; }
    std::printf("all parity-26103 tests passed\n");
    return 0;
}
