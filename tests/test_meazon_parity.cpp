// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the MEAZON metering plugs (BIZY_PLUG, DINRAIL).
//
// Both run z2m's vendor `fz.meazon_meter`, which decodes a set of
// MANUFACTURER-SPECIFIC (MEAZON_S_A) attributes carried INSIDE the
// `seMetering` (0x0702) cluster — proprietary attribute IDs that do not
// overlap the standard seMetering (0x0000/0x0400) or
// haElectricalMeasurement (0x0B04) slots. The auto-generated ports wired
// the generic kFzMetering + kFzElectricalMeasurement instead, so every
// channel (power/voltage/current/energy) was a dead expose: the device
// reports power on 0x2001, voltage on 0x2004/0x2015, current on
// 0x2007/0x2018 and energy on 0x3000 — the generics never look there
// (and the device reports on NO 0x0B04 cluster at all).
//
// The fix wires the shared `kFzMeazonMeter` (seMetering) in place of the
// generic pair on both defs. All channels are raw pass-through (z2m only
// `precisionRound(value, 2)`; the runtime scales downstream).
//
// Verified here:
//   * BIZY_PLUG: power (0x2001 s), voltage (0x2004 + alternate 0x2015),
//     current (0x2007 + alternate 0x2018), energy (0x3000) all decode to
//     the exposed keys; the report carried manufacturer-specific
//     (fc bit 0x04 + MEAZON_S_A code) decodes transparently; genOnOff
//     still decodes "state".
//   * DINRAIL: power/voltage/current decode (no energy expose).
//   * Regression: the generic kFzMetering standard energy attr (0x0000)
//     no longer mis-fires on this device (that converter is gone).
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts
//             (meazon_meter) + src/devices/meazon.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::meazon {
extern const PreparedDefinition kDef_MEAZON_BIZY_PLUG;
extern const PreparedDefinition kDef_MEAZON_DINRAIL;
}  // namespace zhc::devices::meazon

using namespace zhc;

namespace {

constexpr std::uint16_t SE_METERING = 0x0702;
constexpr std::uint16_t MEAZON_CODE = 0x1072;  // MEAZON_S_A (arbitrary here)

// ── ZCL attribute report builder ───────────────────────────────────
// Plain (non-manufacturer) report: fc=0x18, tsn, cmd=0x0A, records.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

// Manufacturer-specific report: fc=0x1C (server-to-client + mfg bit),
// mfg-code (LE), tsn, cmd=0x0A, records.
std::vector<std::uint8_t> mfg_attr_report(std::uint16_t mfg, std::uint16_t attr_id,
                                          std::uint8_t type,
                                          std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x1C};
    v.push_back(static_cast<std::uint8_t>(mfg & 0xFF));
    v.push_back(static_cast<std::uint8_t>(mfg >> 8));
    v.push_back(0x42);  // tsn
    v.push_back(0x0A);  // report-attributes
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
                            const char* cluster_name, std::uint8_t src_ep,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return true;
    return false;
}

// LE payload helpers.
std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}
std::vector<std::uint8_t> u32(std::uint32_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8),
            static_cast<std::uint8_t>(x >> 16), static_cast<std::uint8_t>(x >> 24)};
}

void expect_uint_mfg(const PreparedDefinition& def, std::uint16_t attr,
                     std::uint8_t type, const std::vector<std::uint8_t>& val,
                     const char* key, std::uint64_t want) {
    assert(def_exposes(def, key));
    auto r = dispatch_zcl(def, SE_METERING, "seMetering", 10,
                          mfg_attr_report(MEAZON_CODE, attr, type, val));
    assert(r.any_matched);
    const Value* v = r.merged.find(key);
    assert(v && v->type == ValueType::Uint && v->u == want);
}

void expect_int_mfg(const PreparedDefinition& def, std::uint16_t attr,
                    std::uint8_t type, const std::vector<std::uint8_t>& val,
                    const char* key, std::int64_t want) {
    assert(def_exposes(def, key));
    auto r = dispatch_zcl(def, SE_METERING, "seMetering", 10,
                          mfg_attr_report(MEAZON_CODE, attr, type, val));
    assert(r.any_matched);
    const Value* v = r.merged.find(key);
    assert(v && v->type == ValueType::Int && v->i == want);
}

// ── BIZY_PLUG: full channel set incl. energy ────────────────────────
void check_bizy_plug() {
    const auto& def = devices::meazon::kDef_MEAZON_BIZY_PLUG;

    // power — attr 0x2001 (8193), signed (s32). 1500 W.
    expect_int_mfg(def, 0x2001, 0x2B, u32(1500), "power", 1500);
    // negative power (production) round-trips.
    expect_int_mfg(def, 0x2001, 0x2B, u32(0xFFFFFB00), "power", -1280);

    // voltage — primary 0x2004 (8196) u16, and alternate 0x2015 (8213).
    expect_uint_mfg(def, 0x2004, 0x21, u16(2300), "voltage", 2300);
    expect_uint_mfg(def, 0x2015, 0x21, u16(2305), "voltage", 2305);

    // current — primary 0x2007 (8199) u16, and alternate 0x2018 (8216).
    expect_uint_mfg(def, 0x2007, 0x21, u16(650), "current", 650);
    expect_uint_mfg(def, 0x2018, 0x21, u16(655), "current", 655);

    // energy — attr 0x3000 (12288) u32. BIZY_PLUG only.
    expect_uint_mfg(def, 0x3000, 0x23, u32(123456), "energy", 123456);

    // genOnOff still decodes "state" (regression).
    assert(def_exposes(def, "state"));
    const std::uint8_t on[] = {0x01};
    auto rs = dispatch_zcl(def, 0x0006, "genOnOff", 10, attr_report(0x0000, 0x10, on));
    const Value* sv = rs.merged.find("state");
    assert(sv && sv->type == ValueType::Bool && sv->b == true);

    // Regression: standard seMetering energy attr 0x0000 must NOT decode
    // to "energy" anymore (generic kFzMetering removed). meazon_meter
    // ignores it -> no match.
    auto rstd = dispatch_zcl(def, SE_METERING, "seMetering", 10,
                             attr_report(0x0000, 0x25,
                                         std::array<std::uint8_t, 6>{1, 0, 0, 0, 0, 0}));
    assert(rstd.merged.find("energy") == nullptr);
}

// ── DINRAIL: power/voltage/current, no energy expose ────────────────
void check_dinrail() {
    const auto& def = devices::meazon::kDef_MEAZON_DINRAIL;

    expect_int_mfg(def, 0x2001, 0x2B, u32(900), "power", 900);
    expect_uint_mfg(def, 0x2004, 0x21, u16(2310), "voltage", 2310);
    expect_uint_mfg(def, 0x2007, 0x21, u16(400), "current", 400);

    // DINRAIL exposes no energy.
    assert(!def_exposes(def, "energy"));
}

}  // namespace

int main() {
    check_bizy_plug();
    check_dinrail();
    return 0;
}
