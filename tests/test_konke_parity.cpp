// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Konke family. Found by a z2m<->embedded-zhc parity
// pass over the 16 konke defs (konke.ts).
//
// Bugs fixed:
//   * 2AJZ4KPFT (temperature + humidity sensor): the Tier-1 auto-port wired
//     only kFzBattery and exposed only battery/voltage, silently dropping
//     both sensor channels. z2m fromZigbee is [fz.temperature, fz.humidity,
//     fz.battery] with exposes [temperature, humidity, battery]. Restored the
//     msTemperatureMeasurement (0x0402) + msRelativeHumidity (0x0405)
//     decoders + exposes.
//   * KK-ES-J01W (temperature/humidity/illuminance sensor): same regression —
//     only battery wired. z2m fromZigbee is [fz.battery, fz.humidity,
//     fz.temperature] + extend [m.illuminance()]. Restored temperature,
//     humidity and illuminance (msIlluminanceMeasurement 0x0400) decoders +
//     exposes.
//
// The button / scene-switch fixtures pin the pre-existing custom converters
// (kFzKonkeAction onOff 128/129/130 -> single/double/hold; kFzKonkeSceneAction
// genScenes commandRecall 241..244 -> hexagon/square/triangle/circle) so the
// already-correct ports stay correct.
//
// z2m-source: zigbee-herdsman-converters/src/devices/konke.ts.

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::konke {
extern const PreparedDefinition kDef_D2AJZ4KPFT;  // temp + humidity
extern const PreparedDefinition kDef_KK_ES_J01W;  // temp + humidity + illum
extern const PreparedDefinition kDef_D2AJZ4KPKEY; // button -> action
extern const PreparedDefinition kDef_KK_TQ_J01W;  // scene switch -> action
}  // namespace zhc::devices::konke

using namespace zhc;

namespace {

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

double as_num(const Value* v) {
    assert(v);
    return (v->type == ValueType::Int) ? double(v->i)
         : (v->type == ValueType::Uint) ? double(v->u)
         : v->f;
}

InboundApsFrame make_zcl_frame(std::uint16_t cluster,
                               std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};   // fc, tsn, cmd=ReportAttributes
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

DispatchResult dispatch_zcl(const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_zcl_frame(cluster_id, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Report a u16 attribute (LE) of the given attr id / ZCL type.
DispatchResult dispatch_u16(const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::uint16_t attr_id, std::uint8_t zcl_type,
                            std::uint16_t value) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>(value & 0xFF),
        static_cast<std::uint8_t>(value >> 8),
    };
    return dispatch_zcl(def, cluster_id, cluster_name,
                        attr_report(attr_id, zcl_type, b));
}

// ── 2AJZ4KPFT — temperature + humidity (not battery-only) ─────────────
void test_2ajz4kpft_sensor() {
    const auto& d = devices::konke::kDef_D2AJZ4KPFT;
    assert(def_exposes(d, "temperature"));
    assert(def_exposes(d, "humidity"));
    assert(def_exposes(d, "battery"));

    // msTemperatureMeasurement 0x0402 attr 0x0000 INT16 (0x29): 2150 -> 21.5 C.
    auto t = dispatch_u16(d, 0x0402, "msTemperatureMeasurement", 0x0000, 0x29, 2150);
    assert(t.any_matched);
    const Value* tv = t.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(as_num(tv) - 21.5) < 0.01);

    // msRelativeHumidity 0x0405 attr 0x0000 UINT16 (0x21): 5530 -> 55.3 %.
    auto h = dispatch_u16(d, 0x0405, "msRelativeHumidity", 0x0000, 0x21, 5530);
    assert(h.any_matched);
    const Value* hv = h.merged.find("humidity");
    assert(hv && hv->type == ValueType::Float);
    assert(std::fabs(as_num(hv) - 55.3) < 0.01);
}

// ── KK-ES-J01W — temperature + humidity + illuminance ─────────────────
void test_kk_es_j01w_sensor() {
    const auto& d = devices::konke::kDef_KK_ES_J01W;
    assert(def_exposes(d, "temperature"));
    assert(def_exposes(d, "humidity"));
    assert(def_exposes(d, "illuminance"));
    assert(def_exposes(d, "battery"));

    auto t = dispatch_u16(d, 0x0402, "msTemperatureMeasurement", 0x0000, 0x29, 1980);
    const Value* tv = t.merged.find("temperature");
    assert(tv && tv->type == ValueType::Float);
    assert(std::fabs(as_num(tv) - 19.8) < 0.01);

    auto h = dispatch_u16(d, 0x0405, "msRelativeHumidity", 0x0000, 0x21, 4200);
    const Value* hv = h.merged.find("humidity");
    assert(hv && hv->type == ValueType::Float);
    assert(std::fabs(as_num(hv) - 42.0) < 0.01);

    // msIlluminanceMeasurement 0x0400 attr 0x0000 UINT16 (0x21): raw passthrough.
    auto l = dispatch_u16(d, 0x0400, "msIlluminanceMeasurement", 0x0000, 0x21, 12345);
    assert(l.any_matched);
    const Value* lv = l.merged.find("illuminance");
    assert(lv && lv->type == ValueType::Uint);
    assert(std::fabs(as_num(lv) - 12345.0) < 0.5);
}

// ── 2AJZ4KPKEY — multi-function button action (regression pin) ────────
void test_2ajz4kpkey_action() {
    const auto& d = devices::konke::kDef_D2AJZ4KPKEY;
    assert(def_exposes(d, "action"));

    // genOnOff 0x0006 attr 0x0000 (onOff) UINT8 (0x20): 128/129/130 ->
    // single/double/hold.
    struct { std::uint8_t wire; const char* action; } cases[] = {
        {128, "single"}, {129, "double"}, {130, "hold"},
    };
    for (auto& c : cases) {
        const std::uint8_t b[] = { c.wire };
        auto r = dispatch_zcl(d, 0x0006, "genOnOff", attr_report(0x0000, 0x20, b));
        assert(r.any_matched);
        const Value* av = r.merged.find("action");
        assert(av && av->type == ValueType::StringRef);
        assert(std::strcmp(av->str, c.action) == 0);
    }

    // An unmapped value is dropped (no action).
    const std::uint8_t bz[] = { 0x00 };
    auto z = dispatch_zcl(d, 0x0006, "genOnOff", attr_report(0x0000, 0x20, bz));
    assert(z.merged.find("action") == nullptr);
}

// ── KK-TQ-J01W — 4-key scene switch action (regression pin) ───────────
void test_kk_tq_j01w_action() {
    const auto& d = devices::konke::kDef_KK_TQ_J01W;
    assert(def_exposes(d, "action"));

    // genScenes commandRecall (cmd 0x05). Cluster-specific frame header:
    //   [fc=0x01] [tsn] [cmd=0x05] then body u16 group (LE) + u8 sceneId.
    struct { std::uint8_t scene; const char* action; } cases[] = {
        {241, "hexagon"}, {242, "square"}, {243, "triangle"}, {244, "circle"},
    };
    for (auto& c : cases) {
        std::vector<std::uint8_t> frame{0x01, 0x42, 0x05, 0x00, 0x00, c.scene};
        auto raw = make_zcl_frame(0x0005, frame);
        DecodedMessage msg{};
        assert(decode_frame(raw, {}, msg));
        msg.cluster = "genScenes";
        RuntimeContext ctx{};
        auto r = dispatch_from_zigbee(msg, {}, d, raw, ctx);
        assert(r.any_matched);
        const Value* av = r.merged.find("action");
        assert(av && av->type == ValueType::StringRef);
        assert(std::strcmp(av->str, c.action) == 0);
    }
}

}  // namespace

int main() {
    test_2ajz4kpft_sensor();
    test_kk_es_j01w_sensor();
    test_2ajz4kpkey_action();
    test_kk_tq_j01w_action();
    return 0;
}
