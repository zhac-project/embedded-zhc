// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// The 2026-09-29 Aqara batch, against z2m v26.105.0 (zigbee-herdsman-converters
// src/lib/lumi.ts, src/devices/lumi.ts):
//   1  button_lock and flip_indicator_light encoding; which models offer button_lock
//   2  the answers to Aqara's "may I reset?" (genBasic 0xFFF0) and "leave" (0xFCC0 0x00FC)
//   3  the modern heartbeat, 0xFCC0 attribute 0x00F7
//   4  the legacy heartbeat (genBasic 0xFF01): a trailing byte, WSDCGQ11LM's outage count
//   5  no 0xFCC0 binding
//   6  wall-switch event mode and operation_mode
// Frames go through decode_frame as zhc_adapter feeds them, so Aqara's
// manufacturer-specific frames reach the converters with no cluster name, as on
// the hub. Payloads marked "real" were published in z2m issues or z2m's own tests;
// the others are built from the z2m converter they exercise.
// `zhc_lumi_aqara_fixes_tests 3` runs item 3 alone.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <span>
#include <string>
#include <vector>

#include "definitions/lumi/_shared.hpp"
#include "zhc/devices/lumi_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/zcl/foundation.hpp"

using namespace zhc;

namespace {

// ── registry / sets ──────────────────────────────────────────────────

using Set = std::span<const char* const>;

bool in(Set s, const char* model) {
    for (const char* m : s) if (std::strcmp(m, model) == 0) return true;
    return false;
}

const PreparedDefinition& def_of(const char* model) {
    for (std::size_t i = 0; i < devices::lumi::kLumiRegistryCount; ++i) {
        const PreparedDefinition* d = devices::lumi::kLumiRegistry[i];
        if (std::strcmp(d->model, model) == 0) return *d;
    }
    std::fprintf(stderr, "no Lumi definition %s\n", model);
    std::abort();
}

template <typename F>
void each_lumi(F f) {
    for (std::size_t i = 0; i < devices::lumi::kLumiRegistryCount; ++i)
        f(*devices::lumi::kLumiRegistry[i]);
}

// ── frames in ────────────────────────────────────────────────────────

struct Write {
    std::uint8_t ep;
    std::uint16_t cluster, attr;
    std::uint8_t type;
    std::vector<std::uint8_t> value;
    std::uint16_t manu;
};
std::vector<Write> g_writes;

bool capture_write(std::uint16_t, std::uint8_t ep, std::uint16_t cluster, std::uint16_t attr,
                   std::uint8_t type, const std::uint8_t* v, std::size_t len, std::uint16_t manu) {
    g_writes.push_back({ep, cluster, attr, type, std::vector<std::uint8_t>(v, v + len), manu});
    return true;
}

// A manufacturer-specific attribute report, server to client, as Aqara sends it:
// FC 0x1C, code 0x115F, then attr / type / value.
std::vector<std::uint8_t> lumi_report(std::uint16_t attr, std::uint8_t type,
                                      std::initializer_list<std::uint8_t> value) {
    std::vector<std::uint8_t> f{0x1C, 0x5F, 0x11, 0x30, 0x0A,
                                static_cast<std::uint8_t>(attr), static_cast<std::uint8_t>(attr >> 8),
                                type};
    f.insert(f.end(), value.begin(), value.end());
    return f;
}

// 0xFCC0 attribute 0x00F7, octet string.
std::vector<std::uint8_t> f7(std::initializer_list<std::uint8_t> payload) {
    std::vector<std::uint8_t> f{0x1C, 0x5F, 0x11, 0x31, 0x0A, 0xF7, 0x00, 0x41,
                                static_cast<std::uint8_t>(payload.size())};
    f.insert(f.end(), payload.begin(), payload.end());
    return f;
}

// Decode + dispatch like zhc_adapter's try_decode; `check` runs while the frame,
// the decoded message and the runtime context are still alive.
void feed(const PreparedDefinition& def, std::uint16_t cluster, std::uint8_t ep,
          const std::vector<std::uint8_t>& zcl,
          const std::function<void(const DispatchResult&)>& check = {}) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster;
    raw.src_endpoint = ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = std::span<const std::uint8_t>(zcl.data(), zcl.size());
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    RuntimeContext ctx{};
    ctx.configure_write = &capture_write;
    const DispatchResult r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    if (check) check(r);
}

bool is_uint(const Value* v, std::uint64_t want) { return v && v->type == ValueType::Uint && v->u == want; }
bool is_int(const Value* v, std::int64_t want) { return v && v->type == ValueType::Int && v->i == want; }
bool is_num(const Value* v, double want, double eps) {
    if (!v) return false;
    double d;
    switch (v->type) {
        case ValueType::Uint:  d = static_cast<double>(v->u); break;
        case ValueType::Int:   d = static_cast<double>(v->i); break;
        case ValueType::Float: d = v->f; break;
        default: return false;
    }
    return std::fabs(d - want) <= eps;
}
bool is_str(const Value* v, const char* want) {
    return v && v->type == ValueType::StringRef && std::strcmp(v->str, want) == 0;
}

// ── frames out ───────────────────────────────────────────────────────

struct Out {
    bool ok;
    std::uint16_t cluster;
    std::uint8_t endpoint;
    std::vector<std::uint8_t> frame;
};

Value str_v(const char* s) { Value v{}; v.type = ValueType::StringRef; v.str = s; return v; }
Value bool_v(bool b) { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }

Out send(const PreparedDefinition& def, const char* key, const Value& v) {
    RuntimeContext ctx{};
    std::uint8_t buf[64]{};
    const TzDispatchResult r = dispatch_to_zigbee(def, key, v, ctx, buf);
    return {r.ok, r.cluster_id, r.endpoint, std::vector<std::uint8_t>(buf, buf + r.frame_size)};
}

bool bytes_are(const std::vector<std::uint8_t>& got, std::initializer_list<std::uint8_t> want) {
    return got.size() == want.size() && std::equal(got.begin(), got.end(), want.begin());
}

const Expose* expose_of(const PreparedDefinition& def, const char* name) {
    for (std::uint8_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, name) == 0) return &def.exposes[i];
    return nullptr;
}

bool enum_is(const Expose* e, std::initializer_list<const char*> values) {
    if (!e || e->type != ExposeType::Enum || e->enum_count != values.size()) return false;
    std::uint8_t i = 0;
    for (const char* v : values)
        if (std::strcmp(e->enum_values[i++], v) != 0) return false;
    return true;
}

// ── 1  button_lock / flip_indicator_light ────────────────────────────
//
// z2m lumi_socket_button_lock and lumiButtonLock: manuSpecificLumi 0x0200, u8 (0x20),
// ON = 0, OFF = 1, code 0x115F — on six plugs only. On a wall switch 0x0200 is the
// relay's operation mode, and 0 there means "decoupled".
constexpr const char* kButtonLockModels[] = {"QBCZ14LM", "QBCZ15LM", "SP-EUC01", "WP-P01D",
                                             "ZNCZ15LM", "ZNQBCZ11LM"};

void item1_button_lock() {
    for (const char* m : kButtonLockModels) {
        const auto& def = def_of(m);
        for (auto [v, byte] : {std::pair{str_v("ON"), 0x00}, {str_v("OFF"), 0x01},
                               {bool_v(true), 0x00}, {bool_v(false), 0x01}}) {
            const Out o = send(def, "button_lock", v);
            assert(o.ok && o.cluster == 0xFCC0);
            assert(bytes_are(o.frame, {0x14, 0x5F, 0x11, 0x00, 0x02, 0x00, 0x02, 0x20,
                                       static_cast<std::uint8_t>(byte)}));
        }
        const Expose* e = expose_of(def, "button_lock");
        assert(e && e->type == ExposeType::Binary && e->access == Access::StateSet);
    }
    // No other Lumi definition takes button_lock — above all no wall switch.
    each_lumi([](const PreparedDefinition& def) {
        if (in(kButtonLockModels, def.model)) return;
        for (const Value& v : {str_v("ON"), str_v("OFF"), bool_v(true), bool_v(false)})
            assert(!send(def, "button_lock", v).ok);
        assert(!expose_of(def, "button_lock"));
    });
    assert(!send(def_of("QBKG25LM"), "button_lock", bool_v(false)).ok);   // was 0x0200 = 0

    // z2m lumiFlipIndicatorLight: 0x00F0, u8, ON = 1, OFF = 0, code 0x115F (was 0x00F5 bool).
    const TzConverter& flip = lumi::kTzLumiFlipIndicatorLight;
    for (auto [v, byte] : {std::pair{str_v("ON"), 0x01}, {str_v("OFF"), 0x00},
                           {bool_v(true), 0x01}, {bool_v(false), 0x00}}) {
        RuntimeContext ctx{};
        std::uint8_t buf[32]{};
        std::size_t n = 0;
        assert(flip.fn("flip_indicator_light", v, flip, def_of("QBKG25LM"), ctx,
                       std::span<std::uint8_t>(buf, sizeof(buf)), n));
        assert(bytes_are(std::vector<std::uint8_t>(buf, buf + n),
                         {0x14, 0x5F, 0x11, 0x00, 0x02, 0xF0, 0x00, 0x20,
                          static_cast<std::uint8_t>(byte)}));
    }
    assert(flip.cluster_id == 0xFCC0);
}

// ── 2  "may I reset?" / "leave" ──────────────────────────────────────
//
// z2m lumiPreventReset (lib/lumi.ts): a genBasic attributeReport of 0xFFF0 starting
// aa 10 05 41 87 is answered with a genBasic write of 0xFFF0, octet string (0x41)
// aa 10 05 41 47 01 01 10 01, code 0x115F, on endpoint 1. z2m lumiPreventLeave:
// 0xFCC0 attributeReport 0x00FC = false is answered with 0x00FC = true (bool).
// The model lists are z2m's, mapped onto ZHAC's definitions (a ZHAC definition
// covering several z2m models follows the one it is named after).
constexpr const char* kPreventResetModels[] = {
    "DWZTCGQ11LM", "QBKG03LM", "QBKG04LM", "QBKG11LM", "QBKG12LM", "QBKG17LM", "QBKG18LM",
    "QBKG19LM", "QBKG20LM", "QBKG21LM", "QBKG22LM", "QBKG23LM", "QBKG24LM", "QBKG25LM",
    "QBKG26LM", "QBKG27LM", "QBKG28LM", "QBKG29LM", "QBKG30LM", "QBKG31LM", "QBKG32LM",
    "QBKG33LM", "QBKG34LM", "QBKG38LM", "QBKG39LM", "QBKG40LM", "QBKG41LM", "WS-EUK01",
    "WS-EUK03", "WS-EUK04", "WS-K01D", "WS-K02E", "WS-K03E", "WS-K04E", "WS-K05E",
    "WS-USC01", "WS-USC02", "WXKG02LM", "WXKG03LM", "WXKG06LM", "WXKG07LM", "WXKG17LM",
    "ZNQBKG16LM", "ZNQBKG38LM", "ZNQBKG39LM", "ZNQBKG40LM", "ZNQBKG41LM", "ZNQBKG42LM",
    "ZNQBKG43LM", "ZNQBKG44LM", "ZNQBKG45LM", "ZNWXKG01LM", "ZNXNKG01LM", "ZNXNKG02LM",
};
constexpr const char* kPreventLeaveModels[] = {"FP310", "KD-R01D", "PS-S04D", "WS-K05E",
                                               "WS-K07E", "WS-K08D"};

// The request, with the example bytes z2m's converter carries.
const std::vector<std::uint8_t> kResetAsk =
    lumi_report(0xFFF0, 0x41, {0x09, 0xAA, 0x10, 0x05, 0x41, 0x87, 0x01, 0x01, 0x10, 0x00});

void expect_keep_reply() {
    assert(g_writes.size() == 1);
    const Write& w = g_writes[0];
    assert(w.ep == 1 && w.cluster == 0x0000 && w.attr == 0xFFF0 && w.type == 0x41 && w.manu == 0x115F);
    assert(bytes_are(w.value, {0x09, 0xAA, 0x10, 0x05, 0x41, 0x47, 0x01, 0x01, 0x10, 0x01}));
}

void item2_prevent_reset_and_leave() {
    // The owner's QBKG11LM.
    g_writes.clear();
    feed(def_of("QBKG11LM"), 0x0000, 1, kResetAsk,
         [](const DispatchResult& r) { assert(r.any_matched); });
    expect_keep_reply();
    // The same request without a manufacturer code; asked from another endpoint.
    g_writes.clear();
    feed(def_of("QBKG11LM"), 0x0000, 4,
         {0x18, 0x31, 0x0A, 0xF0, 0xFF, 0x41, 0x09, 0xAA, 0x10, 0x05, 0x41, 0x87, 0x01, 0x01, 0x10, 0x00});
    expect_keep_reply();
    // Anything else on 0xFFF0 is no request (here: the answer echoed back).
    g_writes.clear();
    feed(def_of("QBKG11LM"), 0x0000, 1,
         lumi_report(0xFFF0, 0x41, {0x09, 0xAA, 0x10, 0x05, 0x41, 0x47, 0x01, 0x01, 0x10, 0x01}));
    feed(def_of("QBKG11LM"), 0x0000, 1, lumi_report(0xFFF0, 0x41, {0x03, 0xAA, 0x10, 0x05}));
    assert(g_writes.empty());
    // Exactly z2m's models answer.
    each_lumi([](const PreparedDefinition& def) {
        g_writes.clear();
        feed(def, 0x0000, 1, kResetAsk);
        if (in(kPreventResetModels, def.model)) {
            expect_keep_reply();
        } else if (!g_writes.empty()) {
            std::fprintf(stderr, "%s answers the reset request\n", def.model);
            assert(false);
        }
    });

    // "Leave": 0x00FC false → write true.
    each_lumi([](const PreparedDefinition& def) {
        g_writes.clear();
        feed(def, 0xFCC0, 1, lumi_report(0x00FC, 0x10, {0x00}));
        if (in(kPreventLeaveModels, def.model)) {
            assert(g_writes.size() == 1);
            const Write& w = g_writes[0];
            assert(w.ep == 1 && w.cluster == 0xFCC0 && w.attr == 0x00FC && w.type == 0x10 &&
                   w.manu == 0x115F && bytes_are(w.value, {0x01}));
            g_writes.clear();
            feed(def, 0xFCC0, 1, lumi_report(0x00FC, 0x10, {0x01}));   // true: nothing to do
            assert(g_writes.empty());
        } else if (!g_writes.empty()) {
            std::fprintf(stderr, "%s answers the leave flag\n", def.model);
            assert(false);
        }
    });
}

// ── 3  modern heartbeat, 0xFCC0 0x00F7 ───────────────────────────────
//
// z2m lumi_specific → numericAttributes2Payload, case "247": buffer2DataObject, then
// the same per-model cases: 1 voltage (+ battery on the model's voltageToPercentage
// curve), 2 outage count on JT-BZ-01AQ/A, 3 device_temperature unless the model is on
// the constant-25 °C list, 5 outage count - 1, 101/102 battery on the models that put
// it there, 149 energy, 150 voltage x0.1 (x0.01 on KD-R01D/WS-K05E), 151 current x0.001,
// 152 power. lumiBattery (a separate extend) reads its own voltage/percentage tags.
constexpr const char* kHeartbeatModels[] = {
    "CL-L02D", "CTP-R01", "DJT12LM", "DWZTCGQ11LM", "FP1E", "FP310", "GZCGQ01LM", "GZCGQ11LM",
    "JTBZ01AQ", "JWDL001A", "JWSP001A", "JYGZ01AQ", "KD-R01D", "LGYCDD01LM", "LLKZMK12LM",
    "MCCGQ12LM", "MCCGQ13LM", "MCCGQ14LM", "PS-S04D", "QBCZ14LM", "QBCZ15LM", "QBKG17LM",
    "QBKG18LM", "QBKG19LM", "QBKG20LM", "QBKG24LM", "QBKG25LM", "QBKG26LM", "QBKG27LM",
    "QBKG28LM", "QBKG29LM", "QBKG30LM", "QBKG31LM", "QBKG32LM", "QBKG33LM", "QBKG34LM",
    "QBKG38LM", "QBKG39LM", "QBKG40LM", "QBKG41LM", "RTCGQ12LM", "RTCGQ13LM", "RTCGQ14LM",
    "RTCGQ15LM", "RTCZCGQ11LM", "SJCGQ12LM", "SJCGQ13LM", "SP-EUC01", "SSM-U01", "SSM-U02",
    "SSWQD03LM", "SSWQD22LM", "SSWQDYH02", "T2_E27", "T2_E27_CCT", "TDL01LM", "TH-S04D",
    "VOCKQJK11LM", "WP-P09D", "WP-P01D", "WSDCGQ12LM", "WS-EUK01", "WS-EUK03", "WS-EUK04",
    "WS-K01D", "WS-K02E", "WS-K03E", "WS-K04E", "WS-K05E", "WS-K07E", "WS-K08D", "WS-USC01",
    "WS-USC02", "WXCJKG11LM", "WXCJKG12LM", "WXCJKG13LM", "WXKG04LM", "WXKG13LM", "WXKG14LM",
    "WXKG15LM", "WXKG16LM", "WXKG17LM", "WXKG20LM", "WXKG21LM", "WXKG22LM", "ZNCLBL01LM",
    "ZNCZ04LM", "ZNCZ12LM", "ZNCZ15LM", "ZNDDQDQ11LM", "ZNDDQDQ12LM", "ZNDDQDQ13LM",
    "ZNLDP12LM", "ZNLDP13LM", "ZNLDP14LM", "ZNLDP18LM", "ZNLDP19LM", "ZNQBCZ11LM",
    "ZNQBKG16LM", "ZNQBKG38LM", "ZNQBKG39LM", "ZNQBKG40LM", "ZNQBKG41LM", "ZNQBKG42LM",
    "ZNQBKG43LM", "ZNQBKG44LM", "ZNQBKG45LM", "ZNTGMK12LM", "ZNWXKG01LM", "ZNXNKG01LM",
    "ZNXNKG02LM",
};
// meta.battery.voltageToPercentage (lumi_specific tag 1) or lumiBattery on tag 1.
constexpr const char* kTag1BatteryModels[] = {
    "CTP-R01", "GZCGQ01LM", "GZCGQ11LM", "JYGZ01AQ", "MCCGQ12LM", "MCCGQ13LM", "MCCGQ14LM",
    "RTCGQ12LM", "RTCGQ13LM", "RTCGQ14LM", "RTCGQ15LM", "SJCGQ12LM", "SJCGQ13LM",
    "VOCKQJK11LM", "WSDCGQ12LM", "WXCJKG11LM", "WXCJKG12LM", "WXCJKG13LM", "WXKG04LM",
    "WXKG13LM", "WXKG14LM", "WXKG15LM", "WXKG16LM", "WXKG17LM", "WXKG20LM", "WXKG21LM",
    "WXKG22LM", "DJT12LM", "ZNXNKG02LM",
};
// lumiBattery without lumi_specific: no tag but the battery ones.
constexpr const char* kBatteryOnlyModels[] = {"DJT12LM", "ZNXNKG02LM", "DWZTCGQ11LM"};

void item3_heartbeat() {
    // Real, SJCGQ12LM (z2m #20764): 3013 mV, 25 °C, tag 5 = 17.
    const auto t1 = f7({0x01, 0x21, 0xC5, 0x0B, 0x03, 0x28, 0x19, 0x04, 0x21, 0xA8, 0x13, 0x05,
                        0x21, 0x11, 0x00, 0x06, 0x24, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08, 0x21,
                        0x1C, 0x01, 0x0A, 0x21, 0x00, 0x00, 0x0C, 0x20, 0x01, 0x64, 0x10, 0x00});
    feed(def_of("SJCGQ12LM"), 0xFCC0, 1, t1, [](const DispatchResult& r) {
        assert(is_uint(r.merged.find("voltage"), 3013));
        assert(is_uint(r.merged.find("battery"), 100));
        assert(is_int(r.merged.find("device_temperature"), 25));
        assert(is_int(r.merged.find("power_outage_count"), 16));
    });
    // Same family, on a model whose tag 3 z2m ignores (a constant 25 °C).
    feed(def_of("MCCGQ14LM"), 0xFCC0, 1, t1, [](const DispatchResult& r) {
        assert(is_uint(r.merged.find("battery"), 100));
        assert(!r.merged.find("device_temperature"));
        assert(is_int(r.merged.find("power_outage_count"), 16));
    });
    // Real, WSDCGQ12LM at 2625 mV (z2m #31671, "0% battery"), 18 °C, tag 5 = 16.
    feed(def_of("WSDCGQ12LM"), 0xFCC0, 1,
         f7({0x01, 0x21, 0x41, 0x0A, 0x03, 0x28, 0x12, 0x04, 0x21, 0xA8, 0x43, 0x05, 0x21, 0x10,
             0x00, 0x06, 0x24, 0x06, 0x00, 0x00, 0x00, 0x00, 0x08, 0x21, 0x1D, 0x01, 0x0A, 0x21,
             0x76, 0x9E, 0x0C, 0x20, 0x01, 0x64, 0x29, 0x66, 0x07, 0x65, 0x29, 0x7A, 0x0F, 0x66,
             0x29, 0xEE, 0x03}),
         [](const DispatchResult& r) {
             assert(is_uint(r.merged.find("voltage"), 2625));
             assert(is_uint(r.merged.find("battery"), 0));
             assert(is_int(r.merged.find("device_temperature"), 18));
             assert(is_int(r.merged.find("power_outage_count"), 15));
         });
    // Real, DJT12LM (z2m #33186): lumiBattery only, so voltage and battery and nothing else.
    const std::initializer_list<std::uint8_t> djt = {
        0x01, 0x21, 0xCA, 0x0B, 0x03, 0x28, 0x19, 0x04, 0x21, 0xA8, 0x13, 0x05, 0x21, 0x25,
        0x00, 0x06, 0x24, 0x06, 0x00, 0x00, 0x00, 0x00, 0x08, 0x21, 0x1C, 0x01, 0x0A, 0x21,
        0x47, 0xB7, 0x0C, 0x20, 0x01, 0x66, 0x20, 0x03, 0x67, 0x20, 0x01, 0x68, 0x21, 0xA8, 0x00};
    feed(def_of("DJT12LM"), 0xFCC0, 1, f7(djt), [](const DispatchResult& r) {
        assert(is_uint(r.merged.find("voltage"), 3018));
        assert(is_uint(r.merged.find("battery"), 100));
        assert(!r.merged.find("device_temperature"));
        assert(!r.merged.find("power_outage_count"));
    });
    // The same issue's log shows z2m publishing voltage 2956 as battery 71: z2m rounds
    // (the legacy 0xFF01 path truncates to 70).
    std::vector<std::uint8_t> djt_2956 = f7(djt);
    djt_2956[9 + 2] = 0x8C;   // tag 1 = 0x0B8C = 2956
    feed(def_of("DJT12LM"), 0xFCC0, 1, djt_2956, [](const DispatchResult& r) {
        assert(is_uint(r.merged.find("voltage"), 2956));
        assert(is_uint(r.merged.find("battery"), 71));
    });
    // Real, ZNXNKG02LM (z2m #29505).
    feed(def_of("ZNXNKG02LM"), 0xFCC0, 1,
         f7({0x01, 0x21, 0xF4, 0x0B, 0x03, 0x28, 0x19, 0x04, 0x21, 0xA8, 0x01, 0x05, 0x21, 0x1C,
             0x00, 0x06, 0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x21, 0x14, 0x01, 0x0A, 0x21,
             0x9D, 0x32, 0x0C, 0x20, 0x01, 0x64, 0x10, 0x00}),
         [](const DispatchResult& r) {
             assert(is_uint(r.merged.find("voltage"), 3060));
             assert(is_uint(r.merged.find("battery"), 100));
             assert(!r.merged.find("power_outage_count"));
         });
    // Real, P100 DWZTCGQ11LM (z2m #31503): lumiBattery voltage tag 23, percentage tag 24.
    feed(def_of("DWZTCGQ11LM"), 0xFCC0, 1,
         f7({0x05, 0x21, 0x09, 0x00, 0x0A, 0x21, 0xF8, 0xF9, 0x0C, 0x20, 0x0A, 0x0D, 0x23, 0x15,
             0x00, 0x00, 0x00, 0x13, 0x20, 0x00, 0x17, 0x21, 0x06, 0x0B, 0x18, 0x20, 0x64, 0x1A,
             0x21, 0x30, 0x0B, 0x1C, 0x10, 0x00, 0x65, 0x20, 0x05}),
         [](const DispatchResult& r) {
             assert(is_uint(r.merged.find("voltage"), 2822));
             assert(is_uint(r.merged.find("battery"), 100));
             assert(!r.merged.find("power_outage_count"));
         });
    // PS-S04D, the vector of z2m's own test: tag 23 2916 mV, tag 24 97 %.
    feed(def_of("PS-S04D"), 0xFCC0, 1, f7({0x17, 0x21, 0x64, 0x0B, 0x18, 0x20, 0x61}),
         [](const DispatchResult& r) {
             assert(is_uint(r.merged.find("voltage"), 2916));
             assert(is_uint(r.merged.find("battery"), 97));
         });
    // FP310: lumi_specific (tag 5) + lumiBattery on tag 23 with the 2850-3000 curve.
    feed(def_of("FP310"), 0xFCC0, 1, f7({0x05, 0x21, 0x03, 0x00, 0x17, 0x21, 0x64, 0x0B}),
         [](const DispatchResult& r) {
             assert(is_int(r.merged.find("power_outage_count"), 2));
             assert(is_uint(r.merged.find("voltage"), 2916));
             assert(is_uint(r.merged.find("battery"), 44));
         });
    // Real, WS-EUK04 wall switch (z2m #31318): floats for power 152, energy 149,
    // voltage 150 (x0.1), current 151 (x0.001); no battery on a mains device.
    feed(def_of("WS-EUK04"), 0xFCC0, 1,
         f7({0x64, 0x10, 0x00, 0x65, 0x10, 0x00, 0x03, 0x28, 0x1A, 0x98, 0x39, 0x00, 0x00, 0x00,
             0x00, 0x95, 0x39, 0xFC, 0x83, 0xC6, 0x40, 0x96, 0x39, 0x00, 0xC0, 0x14, 0x45, 0x97,
             0x39, 0x00, 0x00, 0x00, 0x00, 0x05, 0x21, 0x1E, 0x00, 0x9A, 0x20, 0x10, 0x09, 0x21,
             0x01, 0x15, 0x0D, 0x23, 0x17, 0x0B, 0x00, 0x00, 0x0E, 0x23, 0x00, 0x00, 0x00, 0x00,
             0x0A, 0x21, 0x78, 0x52, 0x0C, 0x20, 0x01, 0x11, 0x23, 0x01, 0x00, 0x00, 0x00}),
         [](const DispatchResult& r) {
             assert(is_int(r.merged.find("device_temperature"), 26));
             assert(is_num(r.merged.find("power"), 0.0, 1e-6));
             assert(is_num(r.merged.find("energy"), 6.2036, 1e-3));
             assert(is_num(r.merged.find("voltage"), 238.0, 1e-3));
             assert(is_num(r.merged.find("current"), 0.0, 1e-6));
             assert(is_int(r.merged.find("power_outage_count"), 29));
             assert(!r.merged.find("battery"));
         });
    // Real, SP-EUC01 plug (z2m #31071): 52 W, 239 V, 0.218 A, 0.1746 kWh, 26 °C.
    feed(def_of("SP-EUC01"), 0xFCC0, 1,
         f7({0x64, 0x10, 0x01, 0x03, 0x28, 0x1A, 0x98, 0x39, 0x00, 0x00, 0x50, 0x42, 0x95, 0x39,
             0x91, 0xBE, 0x32, 0x3E, 0x96, 0x39, 0x00, 0x60, 0x15, 0x45, 0x97, 0x39, 0xBF, 0x92,
             0x59, 0x43, 0x05, 0x21, 0x03, 0x00, 0x9A, 0x20, 0x00, 0x08, 0x21, 0x2D, 0x01, 0x09,
             0x21, 0x00, 0x08, 0x0B, 0x20, 0x00, 0x9B, 0x10, 0x01, 0x0A, 0x21, 0x00, 0x00, 0x0C,
             0x20, 0x01}),
         [](const DispatchResult& r) {
             assert(is_int(r.merged.find("device_temperature"), 26));
             assert(is_num(r.merged.find("power"), 52.0, 1e-4));
             assert(is_num(r.merged.find("energy"), 0.174555, 1e-4));
             assert(is_num(r.merged.find("voltage"), 239.0, 1e-3));
             assert(is_num(r.merged.find("current"), 0.217574, 1e-4));
             assert(is_int(r.merged.find("power_outage_count"), 2));
         });
    // Per-model scales: KD-R01D voltage x0.01 (float 23508), LLKZMK12LM energy / 1000
    // (float 1234), TH-S04D battery from tag 102, ZNCLBL01LM battery = tag 101 / 2,
    // JT-BZ-01AQ/A outage count from tag 2.
    feed(def_of("KD-R01D"), 0xFCC0, 1, f7({0x96, 0x39, 0x00, 0xA8, 0xB7, 0x46}),
         [](const DispatchResult& r) { assert(is_num(r.merged.find("voltage"), 235.08, 1e-3)); });
    feed(def_of("WS-EUK04"), 0xFCC0, 1, f7({0x96, 0x39, 0x00, 0xA8, 0xB7, 0x46}),
         [](const DispatchResult& r) { assert(is_num(r.merged.find("voltage"), 2350.8, 1e-2)); });
    feed(def_of("LLKZMK12LM"), 0xFCC0, 1, f7({0x95, 0x39, 0x00, 0x40, 0x9A, 0x44}),
         [](const DispatchResult& r) { assert(is_num(r.merged.find("energy"), 1.234, 1e-5)); });
    feed(def_of("TH-S04D"), 0xFCC0, 1, f7({0x01, 0x21, 0xE8, 0x0B, 0x66, 0x20, 0x5A}),
         [](const DispatchResult& r) {
             assert(is_uint(r.merged.find("voltage"), 3048));
             assert(is_uint(r.merged.find("battery"), 90));
         });
    feed(def_of("ZNCLBL01LM"), 0xFCC0, 1, f7({0x65, 0x20, 0xC7}),
         [](const DispatchResult& r) { assert(is_num(r.merged.find("battery"), 99.5, 1e-4)); });
    feed(def_of("JTBZ01AQ"), 0xFCC0, 1, f7({0x02, 0x21, 0x05, 0x00}),
         [](const DispatchResult& r) { assert(is_int(r.merged.find("power_outage_count"), 4)); });
    // buffer2DataObject: a trailing byte is never a tag; 0x42 carries one byte, 0x5F four.
    feed(def_of("SJCGQ12LM"), 0xFCC0, 1,
         f7({0x03, 0x28, 0x19, 0x07, 0x42, 0xAA, 0x08, 0x5F, 0x01, 0x02, 0x03, 0x04, 0x05, 0x21,
             0x11, 0x00, 0x05}),
         [](const DispatchResult& r) {
             assert(is_int(r.merged.find("device_temperature"), 25));
             assert(is_int(r.merged.find("power_outage_count"), 16));
         });

    // Exactly z2m's models decode it. (WP-P09D's endpoint map suffixes the per-endpoint
    // keys of a report from endpoint 1 with its label "1".)
    const auto probe = f7({0x01, 0x21, 0xC5, 0x0B, 0x05, 0x21, 0x03, 0x00});
    each_lumi([&probe](const PreparedDefinition& def) {
        feed(def, 0xFCC0, 1, probe, [&def](const DispatchResult& r) {
            const bool hb = in(kHeartbeatModels, def.model);
            const bool v = r.merged.find("voltage") != nullptr;
            const bool b = r.merged.find("battery") != nullptr;
            const bool o = r.merged.find(def.endpoint_map ? "power_outage_count_1"
                                                          : "power_outage_count") != nullptr;
            const bool want_v = hb && std::strcmp(def.model, "DWZTCGQ11LM") != 0;
            const bool want_b = in(kTag1BatteryModels, def.model);
            const bool want_o = hb && !in(kBatteryOnlyModels, def.model);
            if (v != want_v || b != want_b || o != want_o) {
                std::fprintf(stderr, "%s: voltage %d/%d battery %d/%d outage %d/%d\n", def.model,
                             v, want_v, b, want_b, o, want_o);
                assert(false);
            }
        });
    });
}

// ── 4  legacy heartbeat (genBasic 0xFF01) ────────────────────────────

void item4_legacy_heartbeat() {
    // herdsman readMiStruct skips "a trailing byte" some Xiaomi structs carry;
    // parse what is there instead of dropping the whole struct.
    {
        const std::uint8_t s[] = {0x01, 0x21, 0xA8, 0x0B, 0x03, 0x28, 0x1D, 0x05, 0x21, 0x12, 0x00,
                                  0x00};   // trailing byte
        char scratch[64];
        FixedPayload<ZHC_MI_STRUCT_CAP> arena{};
        assert(parse_mi_struct(s, scratch, sizeof(scratch), arena));
        assert(arena.count == 3);
        assert(is_uint(arena.find("5"), 18));
    }
    // A record cut short keeps the records before it.
    {
        const std::uint8_t s[] = {0x01, 0x21, 0xA8, 0x0B, 0x05, 0x21, 0x12};
        char scratch[64];
        FixedPayload<ZHC_MI_STRUCT_CAP> arena{};
        assert(parse_mi_struct(s, scratch, sizeof(scratch), arena));
        assert(arena.count == 1 && is_uint(arena.find("1"), 2984));
    }
    // The owner's WXKG01LM heartbeat with a trailing byte inside the 0xFF01 string:
    // battery, voltage, temperature and outage count still arrive.
    feed(def_of("WXKG01LM"), 0x0000, 1,
         lumi_report(0xFF01, 0x42, {0x0C, 0x01, 0x21, 0xA8, 0x0B, 0x03, 0x28, 0x1D, 0x05, 0x21,
                                    0x12, 0x00, 0x00}),
         [](const DispatchResult& r) {
             assert(is_uint(r.merged.find("voltage"), 2984));
             assert(r.merged.find("battery"));
             assert(is_int(r.merged.find("device_temperature"), 29));
             assert(is_uint(r.merged.find("power_outage_count"), 17));
         });
    // WSDCGQ11LM: the outage count is tag 5 - 1 (z2m case "5"). Tag 4 is mode_switch on
    // wall switches and nothing on this sensor; it carries 5032 here, as on most Lumi gear.
    feed(def_of("WSDCGQ11LM"), 0x0000, 1,
         lumi_report(0xFF01, 0x42, {0x28, 0x01, 0x21, 0xD1, 0x0B, 0x03, 0x28, 0x16, 0x04, 0x21,
                                    0xA8, 0x13, 0x05, 0x21, 0x25, 0x00, 0x06, 0x24, 0x01, 0x00,
                                    0x00, 0x00, 0x00, 0x0A, 0x21, 0x00, 0x00, 0x64, 0x29, 0xC7,
                                    0x07, 0x65, 0x21, 0x0B, 0x17, 0x66, 0x2B, 0x1A, 0x8A, 0x01,
                                    0x00}),
         [](const DispatchResult& r) {
             assert(is_num(r.merged.find("power_outage_count"), 36, 0));
             assert(is_num(r.merged.find("temperature"), 19.91, 1e-3));
         });
}

// ── 5  bindings ──────────────────────────────────────────────────────
//
// z2m binds manuSpecificLumi only on the H1/H2 EU switches' button endpoints and the
// FP300; none of these eleven. Their other bindings stay as they were.
void item5_bindings() {
    struct Want { const char* model; std::vector<BindingSpec> binds; };
    const Want wants[] = {
        {"MCCGQ13LM", {{1, 0x0000}}},  {"MCCGQ14LM", {{1, 0x0000}}},
        {"MCCGQ16LM", {{1, 0x0000}}},  {"MCCGQ17LM", {{1, 0x0000}}},
        {"MCCGQ18LM", {{1, 0x0000}}},  {"MCCGQ19LM", {{1, 0x0000}}},
        {"RTCGQ13LM", {{1, 0x0000}, {1, 0x0406}}},
        {"RTCGQ14LM", {{1, 0x0406}, {1, 0x0000}}},
        {"RTCGQ15LM", {{1, 0x0000}, {1, 0x0406}}},
        {"RTCGQ16LM", {{1, 0x0000}, {1, 0x0406}}},
        {"ZNCZ15LM", {{1, 0x0006}, {1, 0x0702}, {1, 0x0B04}}},
    };
    for (const Want& w : wants) {
        const auto& def = def_of(w.model);
        assert(def.bindings_count == w.binds.size());
        std::size_t i = 0;
        for (const BindingSpec& b : w.binds) {
            assert(def.bindings[i].endpoint == b.endpoint && def.bindings[i].cluster_id == b.cluster_id);
            ++i;
        }
    }
    each_lumi([](const PreparedDefinition& def) {
        for (std::uint8_t i = 0; i < def.bindings_count; ++i) assert(def.bindings[i].cluster_id != 0xFCC0);
    });
}

// ── 6  wall-switch event mode and operation_mode ─────────────────────
//
// Event mode: z2m lumiSetEventMode / lumiCommandMode / the per-model configure write
// manuSpecificLumi `mode` (0x0009, u8) = 1, code 0x115F, endpoint 1. The six non-switch
// models ZHAC already wrote it for stay as they are.
constexpr const char* kEventModeModels[] = {
    "QBKG25LM", "QBKG26LM", "QBKG27LM", "QBKG28LM", "QBKG29LM", "QBKG38LM", "QBKG39LM",
    "QBKG40LM", "QBKG41LM", "SP-EUC01", "WS-EUK01", "WS-EUK03", "WS-EUK04", "WS-K01D",
    "WS-K05E", "WS-USC01", "WS-USC02", "ZNQBKG16LM", "ZNQBKG38LM", "ZNQBKG39LM",
    "ZNQBKG40LM", "ZNQBKG41LM", "ZNWXKG01LM", "ZNXNKG01LM", "ZNXNKG02LM",
    "WXCJKG11LM", "WXCJKG12LM", "WXCJKG13LM", "WXKG15LM", "WXKG21LM", "WXKG22LM",
};

bool writes_event_mode(const PreparedDefinition& def) {
    for (std::uint8_t i = 0; i < def.config_steps_count; ++i) {
        const ConfigStep& s = def.config_steps[i];
        if (s.op == ConfigStepOp::Write && s.cluster_id == 0xFCC0 && s.attr_id == 0x0009) {
            assert(s.endpoint == 1 && s.attr_type == 0x20 && s.manu_code == 0x115F);
            assert(s.payload_len == 1 && s.payload[0] == 0x01);
            return true;
        }
    }
    return false;
}

// operation_mode writers, as z2m has them:
//   0xFCC0 0x0200 u8 {control_relay 1, decoupled 0} on the button's endpoint
//     (lumi_switch_operation_mode_opple, lumiOnOff({operationMode}), lumiOperationMode);
//   genBasic 0xFF22 / 0xFF23 u8 on endpoint 1 (lumi_switch_operation_mode_basic):
//     single {control_relay 0x12, decoupled 0xFE}, per button
//     {control_left_relay 0x12, control_right_relay 0x22, decoupled 0xFE};
//   0xFCC0 0x0009 u8 {command 0, event 1} (lumiCommandMode).
struct Op { const char* model; const char* key; std::uint16_t cluster, attr; std::uint8_t ep; };
constexpr Op kOps[] = {
    // lumi_switch_operation_mode_opple
    {"QBKG25LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG25LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"QBKG25LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    {"QBKG26LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG26LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"QBKG26LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    {"QBKG27LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"QBKG28LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG28LM", "operation_mode_right", 0xFCC0, 0x0200, 2},
    {"QBKG29LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG29LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"QBKG29LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    {"QBKG30LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"QBKG31LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG31LM", "operation_mode_right", 0xFCC0, 0x0200, 2},
    {"QBKG32LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG32LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"QBKG32LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    {"QBKG38LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"QBKG39LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG39LM", "operation_mode_right", 0xFCC0, 0x0200, 2},
    {"QBKG40LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"QBKG41LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG41LM", "operation_mode_right", 0xFCC0, 0x0200, 2},
    {"WS-EUK01", "operation_mode", 0xFCC0, 0x0200, 1},
    {"WS-EUK03", "operation_mode", 0xFCC0, 0x0200, 1},
    {"WS-EUK04", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"WS-EUK04", "operation_mode_right", 0xFCC0, 0x0200, 2},
    {"WS-USC01", "operation_mode", 0xFCC0, 0x0200, 1},
    {"WS-USC02", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"WS-USC02", "operation_mode_bottom", 0xFCC0, 0x0200, 2},
    {"ZNQBKG16LM", "operation_mode_left", 0xFCC0, 0x0200, 1},    // z2m ZNQBKG26LM
    {"ZNQBKG16LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"ZNQBKG16LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    {"ZNWXKG01LM", "operation_mode_left", 0xFCC0, 0x0200, 1},    // z2m ZNQBKG31LM
    {"ZNWXKG01LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"ZNWXKG01LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    // lumiOnOff({operationMode: true})
    {"QBKG17LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"QBKG18LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG18LM", "operation_mode_right", 0xFCC0, 0x0200, 2},
    {"QBKG19LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"QBKG20LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG20LM", "operation_mode_right", 0xFCC0, 0x0200, 2},
    {"QBKG33LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG33LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"QBKG33LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    {"QBKG34LM", "operation_mode_left", 0xFCC0, 0x0200, 1},
    {"QBKG34LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"QBKG34LM", "operation_mode_right", 0xFCC0, 0x0200, 3},
    {"WS-K01D", "operation_mode", 0xFCC0, 0x0200, 1},
    {"WS-K02E", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"WS-K03E", "operation_mode_up", 0xFCC0, 0x0200, 1},
    {"WS-K03E", "operation_mode_down", 0xFCC0, 0x0200, 2},
    {"WS-K04E", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"WS-K04E", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"WS-K04E", "operation_mode_bottom", 0xFCC0, 0x0200, 3},
    {"ZNQBKG38LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"ZNQBKG39LM", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"ZNQBKG39LM", "operation_mode_bottom", 0xFCC0, 0x0200, 2},
    {"ZNQBKG40LM", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"ZNQBKG40LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"ZNQBKG40LM", "operation_mode_bottom", 0xFCC0, 0x0200, 3},
    {"ZNQBKG41LM", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"ZNQBKG41LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"ZNQBKG41LM", "operation_mode_bottom", 0xFCC0, 0x0200, 3},
    {"ZNQBKG42LM", "operation_mode", 0xFCC0, 0x0200, 1},
    {"ZNQBKG43LM", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"ZNQBKG43LM", "operation_mode_bottom", 0xFCC0, 0x0200, 2},
    {"ZNQBKG44LM", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"ZNQBKG44LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"ZNQBKG44LM", "operation_mode_bottom", 0xFCC0, 0x0200, 3},
    {"ZNQBKG45LM", "operation_mode_top", 0xFCC0, 0x0200, 1},
    {"ZNQBKG45LM", "operation_mode_center", 0xFCC0, 0x0200, 2},
    {"ZNQBKG45LM", "operation_mode_bottom", 0xFCC0, 0x0200, 3},
    // lumiOperationMode(...)
    {"KD-R01D", "operation_mode", 0xFCC0, 0x0200, 1},
    {"LLKZMK12LM", "operation_mode_l1", 0xFCC0, 0x0200, 1},
    {"LLKZMK12LM", "operation_mode_l2", 0xFCC0, 0x0200, 2},
    {"WS-K05E", "operation_mode_power", 0xFCC0, 0x0200, 1},
    {"WS-K05E", "operation_mode_bright", 0xFCC0, 0x0200, 2},
    {"WS-K05E", "operation_mode_dim", 0xFCC0, 0x0200, 3},
    {"WS-K07E", "operation_mode_up", 0xFCC0, 0x0200, 1},
    {"WS-K08D", "operation_mode_left", 0xFCC0, 0x0200, 1},        // z2m WS-K08E
    {"WS-K08D", "operation_mode_right", 0xFCC0, 0x0200, 2},
    // lumi_switch_operation_mode_basic (genBasic, endpoint 1)
    {"QBKG03LM", "operation_mode_left", 0x0000, 0xFF22, 1},
    {"QBKG03LM", "operation_mode_right", 0x0000, 0xFF23, 1},
    {"QBKG04LM", "operation_mode", 0x0000, 0xFF22, 1},
    {"QBKG11LM", "operation_mode", 0x0000, 0xFF22, 1},
    {"QBKG12LM", "operation_mode_left", 0x0000, 0xFF22, 1},
    {"QBKG12LM", "operation_mode_right", 0x0000, 0xFF23, 1},
    {"QBKG21LM", "operation_mode", 0x0000, 0xFF22, 1},
    {"QBKG22LM", "operation_mode_left", 0x0000, 0xFF22, 1},
    {"QBKG22LM", "operation_mode_right", 0x0000, 0xFF23, 1},
    {"QBKG23LM", "operation_mode", 0x0000, 0xFF22, 1},
    {"QBKG24LM", "operation_mode_left", 0x0000, 0xFF22, 1},
    {"QBKG24LM", "operation_mode_right", 0x0000, 0xFF23, 1},
    // lumiCommandMode
    {"ZNXNKG01LM", "operation_mode", 0xFCC0, 0x0009, 1},
    {"ZNXNKG02LM", "operation_mode", 0xFCC0, 0x0009, 1},
};

constexpr const char* kOpKeys[] = {
    "operation_mode", "operation_mode_left", "operation_mode_center", "operation_mode_right",
    "operation_mode_top", "operation_mode_bottom", "operation_mode_up", "operation_mode_down",
    "operation_mode_l1", "operation_mode_l2", "operation_mode_power", "operation_mode_bright",
    "operation_mode_dim",
};

bool has_op(const char* model, const char* key) {
    for (const Op& o : kOps)
        if (std::strcmp(o.model, model) == 0 && std::strcmp(o.key, key) == 0) return true;
    return false;
}

void item6_switch_settings() {
    each_lumi([](const PreparedDefinition& def) {
        if (writes_event_mode(def) != in(kEventModeModels, def.model)) {
            std::fprintf(stderr, "%s: event mode %d\n", def.model, writes_event_mode(def));
            assert(false);
        }
    });

    for (const Op& o : kOps) {
        const auto& def = def_of(o.model);
        const bool command = o.attr == 0x0009;
        const std::uint8_t lo = static_cast<std::uint8_t>(o.attr), hi = static_cast<std::uint8_t>(o.attr >> 8);
        const Out a = send(def, o.key, str_v(command ? "command" : "decoupled"));
        if (!a.ok) { std::fprintf(stderr, "%s: %s not taken\n", o.model, o.key); assert(false); }
        assert(a.cluster == o.cluster && a.endpoint == o.ep);
        const std::uint8_t off = command ? 0x00 : (o.cluster == 0x0000 ? 0xFE : 0x00);
        assert(bytes_are(a.frame, {0x14, 0x5F, 0x11, 0x00, 0x02, lo, hi, 0x20, off}));
        const Out b = send(def, o.key, str_v(command ? "event" : "control_relay"));
        const std::uint8_t on = command ? 0x01
                              : o.attr == 0x0200 ? 0x01
                              : o.attr == 0xFF22 ? 0x12 : 0x22;   // control_relay = the key's own relay
        assert(b.ok && bytes_are(b.frame, {0x14, 0x5F, 0x11, 0x00, 0x02, lo, hi, 0x20, on}));
        const Expose* e = expose_of(def, o.key);
        assert(e && e->type == ExposeType::Enum && e->access == Access::StateSet);
    }
    // The legacy two-rocker switches take either relay on either key.
    {
        const auto& def = def_of("QBKG12LM");
        const Out r = send(def, "operation_mode_right", str_v("control_left_relay"));
        assert(r.ok && r.frame.back() == 0x12);
        assert(enum_is(expose_of(def, "operation_mode_left"),
                       {"control_left_relay", "control_right_relay", "decoupled"}));
        assert(enum_is(expose_of(def_of("QBKG24LM"), "operation_mode_right"),
                       {"control_right_relay", "decoupled"}));
        assert(enum_is(expose_of(def_of("QBKG28LM"), "operation_mode_left"), {"control_relay", "decoupled"}));
        assert(enum_is(expose_of(def_of("ZNXNKG02LM"), "operation_mode"), {"event", "command"}));
    }
    // Nothing else takes an operation_mode key.
    each_lumi([](const PreparedDefinition& def) {
        for (const char* k : kOpKeys) {
            const bool taken = send(def, k, str_v("decoupled")).ok || send(def, k, str_v("command")).ok;
            if (taken != has_op(def.model, k)) {
                std::fprintf(stderr, "%s: %s taken %d\n", def.model, k, taken);
                assert(false);
            }
        }
    });

    // Read back under the key the write uses. Real, WS-EUK04 (z2m #31318): 0x0200 = 0
    // on endpoint 1, 1 on endpoint 2.
    feed(def_of("WS-EUK04"), 0xFCC0, 1, lumi_report(0x0200, 0x20, {0x00}),
         [](const DispatchResult& r) { assert(is_str(r.merged.find("operation_mode_left"), "decoupled")); });
    feed(def_of("WS-EUK04"), 0xFCC0, 2, lumi_report(0x0200, 0x20, {0x01}),
         [](const DispatchResult& r) {
             assert(is_str(r.merged.find("operation_mode_right"), "control_relay"));
             assert(!r.merged.find("operation_mode_left"));
         });
    feed(def_of("QBKG40LM"), 0xFCC0, 1, lumi_report(0x0200, 0x20, {0x00}),
         [](const DispatchResult& r) { assert(is_str(r.merged.find("operation_mode"), "decoupled")); });
    feed(def_of("QBKG12LM"), 0x0000, 1, lumi_report(0xFF23, 0x20, {0x22}),
         [](const DispatchResult& r) { assert(is_str(r.merged.find("operation_mode_right"), "control_right_relay")); });
    feed(def_of("QBKG11LM"), 0x0000, 1, lumi_report(0xFF22, 0x20, {0xFE}),
         [](const DispatchResult& r) { assert(is_str(r.merged.find("operation_mode"), "decoupled")); });
    feed(def_of("ZNXNKG01LM"), 0xFCC0, 1, lumi_report(0x0009, 0x20, {0x01}),
         [](const DispatchResult& r) { assert(is_str(r.merged.find("operation_mode"), "event")); });
    // z2m decodes neither QBKG23LM's 0xFF22 nor ZNXNKG02LM's `mode`.
    feed(def_of("QBKG23LM"), 0x0000, 1, lumi_report(0xFF22, 0x20, {0xFE}),
         [](const DispatchResult& r) { assert(!r.merged.find("operation_mode")); });
    feed(def_of("ZNXNKG02LM"), 0xFCC0, 1, lumi_report(0x0009, 0x20, {0x01}),
         [](const DispatchResult& r) { assert(!r.merged.find("operation_mode")); });
}

}  // namespace

int main(int argc, char** argv) {
    const int only = argc > 1 ? std::atoi(argv[1]) : 0;
    void (*const items[])() = {item1_button_lock, item2_prevent_reset_and_leave, item3_heartbeat,
                               item4_legacy_heartbeat, item5_bindings, item6_switch_settings};
    for (int i = 0; i < 6; ++i) {
        if (only && only != i + 1) continue;
        items[i]();
        std::printf("item %d ok\n", i + 1);
    }
    return 0;
}
