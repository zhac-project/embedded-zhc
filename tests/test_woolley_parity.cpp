// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Woolley BSD29/BSD59 smart plug.
// z2m-source: woolley.ts #BSD29/BSD59.
//
// Graduated Tier-2 fix pinned here (MISSING decoder/channel, gap class b):
//
//   The generated port wired the generic kFzElectricalMeasurement
//   (haElectricalMeasurement 0x0B04) for the power/voltage/current
//   exposes. But this device reports metering on a manufacturer-specific
//   cluster 0xFC11 (decimal 64529) — z2m's fzLocal.BSD29 reads attrs
//   0x7006/0x7005/0x7004 (power/voltage/current), each /1000. Against
//   0x0B04 every metering report was dropped. Re-pointed at a vendor Fz
//   (kFzWoolleyElectricity) selecting cluster "manuSpecificWoolley" and
//   emitting power/voltage/current as Float = raw/1000. The phantom
//   0x0B04 binding was dropped (z2m binds only genOnOff).

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::woolley {
extern const PreparedDefinition kDef_BSD29_BSD59;
}  // namespace zhc::devices::woolley

using namespace zhc;

namespace {

InboundApsFrame make_frame(std::uint16_t cluster, std::uint8_t src_ep,
                           std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_ep;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// Append a uint24 (0x22) attribute record: attr_id(2 LE) type(1) value(3 LE).
void push_u24_attr(std::vector<std::uint8_t>& v, std::uint16_t attr,
                   std::uint32_t value) {
    v.push_back(static_cast<std::uint8_t>(attr & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr >> 8));
    v.push_back(0x22);  // uint24
    v.push_back(static_cast<std::uint8_t>(value & 0xFF));
    v.push_back(static_cast<std::uint8_t>((value >> 8) & 0xFF));
    v.push_back(static_cast<std::uint8_t>((value >> 16) & 0xFF));
}

bool near(float got, float want) {
    const float d = got - want;
    return d > -0.01f && d < 0.01f;
}

}  // namespace

// ── exposes / bindings shape ─────────────────────────────────────────
static void test_shape() {
    const auto& def = devices::woolley::kDef_BSD29_BSD59;
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "power"));
    assert(def_exposes(def, "voltage"));
    assert(def_exposes(def, "current"));
    // z2m configure() binds only genOnOff; the phantom 0x0B04 bind is gone.
    assert(def_binds(def, 0x0006));
    assert(!def_binds(def, 0x0B04));
}

// ── metering decode on the manuSpecific cluster 0xFC11 ───────────────
static void test_metering_decode() {
    const auto& def = devices::woolley::kDef_BSD29_BSD59;

    // Manufacturer-specific Report Attributes frame on 0xFC11:
    //   fc=0x1C (manuSpec|server->client|disable-default-resp)
    //   mfg_code(2 LE) tsn cmd=0x0A  then three uint24 attr records.
    // z2m fzLocal.BSD29: attr 0x7006=power, 0x7005=voltage, 0x7004=current,
    // each value/1000. Raw: power 60000 -> 60.0 W, voltage 230000 ->
    // 230.0 V, current 261 -> 0.261 A.
    std::vector<std::uint8_t> body = {
        0x1C, 0x41, 0x11, 0x42, 0x0A,  // fc, mfg_lo, mfg_hi, tsn, cmd=Report
    };
    push_u24_attr(body, 0x7006, 60000);   // power   -> 60.000 W
    push_u24_attr(body, 0x7005, 230000);  // voltage -> 230.000 V
    push_u24_attr(body, 0x7004, 261);     // current -> 0.261 A

    auto raw = make_frame(0xFC11, 1,
                          std::span<const std::uint8_t>(body.data(), body.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    // 0xFC11 now maps to "manuSpecificWoolley" in cluster_id_to_name; stamp
    // it the way the dispatch layer does.
    msg.cluster = "manuSpecificWoolley";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);

    const Value* p = r.merged.find("power");
    assert(p && p->type == ValueType::Float && near(p->f, 60.0f));

    const Value* v = r.merged.find("voltage");
    assert(v && v->type == ValueType::Float && near(v->f, 230.0f));

    const Value* c = r.merged.find("current");
    assert(c && c->type == ValueType::Float && near(c->f, 0.261f));
}

// ── a frame on the OLD 0x0B04 cluster must now decode NOTHING ─────────
// Proves the metering channel is no longer wrongly bound to
// haElectricalMeasurement (regression guard for the original bug).
static void test_old_0b04_is_dead() {
    const auto& def = devices::woolley::kDef_BSD29_BSD59;
    // haElectricalMeasurement RMSVoltage (attr 0x0505, uint16) report.
    const std::uint8_t k0b04[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x05, 0x05,              // attr 0x0505 (RMSVoltage)
        0x21,                    // uint16
        0xE6, 0x00,              // 0x00E6 = 230
    };
    auto raw = make_frame(0x0B04, 1, k0b04);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "haElectricalMeasurement";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    // No converter on this def claims 0x0B04 anymore.
    assert(!r.any_matched);
    assert(r.merged.find("voltage") == nullptr);
}

int main() {
    test_shape();
    test_metering_decode();
    test_old_0b04_is_dead();
    return 0;
}
