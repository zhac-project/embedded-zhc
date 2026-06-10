// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Repenic Ltd. RD-250ZG "Dimmer".
//
// z2m wires `m.light({configureReporting:true})` + `m.electricityMeter()`.
// electricityMeter() defaults to cluster:"both": seMetering 0x0702 (energy)
// + haElectricalMeasurement 0x0B04 (power/voltage/current), ac type with
// acFrequency/powerFactor OFF. So z2m exposes power, voltage, current,
// energy. In "both" mode z2m deletes seMetering.power and sources power from
// haElectricalMeasurement (ActivePower 0x050B).
//
// The auto-port wired only the generic kFzMetering (0x0702) and exposed
// energy+power, dropping the 0x0B04 half: voltage and current had no decoder
// and no expose, and power had no live decoder either. The fix adds the
// generic kFzElectricalMeasurement (0x0B04) converter, the voltage/current
// exposes, and the 0x0B04 reporting bind.
//
// Verified here:
//   * power   (haElectricalMeasurement 0x050B ActivePower, s16, signed).
//   * voltage (haElectricalMeasurement 0x0505 RMSVoltage,  u16).
//   * current (haElectricalMeasurement 0x0508 RMSCurrent,  u16).
//   * Regression: energy still decodes via the generic kFzMetering (0x0702),
//     and the light core (state on/off + brightness) still decodes.
//
// z2m-source: repenic_ltd.ts #RD-250ZG + lib/modernExtend.ts genericMeter
//             (electricityMeter, cluster:"both").

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::repenic_ltd {
extern const PreparedDefinition kDef_RD_250ZG;
}  // namespace zhc::devices::repenic_ltd

using namespace zhc;

namespace {

// ── ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then records ─
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
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

constexpr std::uint16_t SE_METERING = 0x0702;
constexpr std::uint16_t HA_ELEC     = 0x0B04;

std::vector<std::uint8_t> u16(std::uint16_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8)};
}
std::vector<std::uint8_t> u48(std::uint64_t x) {
    return {static_cast<std::uint8_t>(x), static_cast<std::uint8_t>(x >> 8),
            static_cast<std::uint8_t>(x >> 16), static_cast<std::uint8_t>(x >> 24),
            static_cast<std::uint8_t>(x >> 32), static_cast<std::uint8_t>(x >> 40)};
}

const Value* decode_one(const PreparedDefinition& def, std::uint16_t cluster,
                        const char* cname, std::uint16_t attr, std::uint8_t type,
                        const std::vector<std::uint8_t>& val, const char* key) {
    auto r = dispatch_zcl(def, cluster, cname, 1, attr_report(attr, type, val));
    assert(r.any_matched);
    return r.merged.find(key);
}

void expect_uint(const PreparedDefinition& def, std::uint16_t cluster,
                 const char* cname, std::uint16_t attr, std::uint8_t type,
                 const std::vector<std::uint8_t>& val, const char* key,
                 std::uint64_t want) {
    assert(def_exposes(def, key));
    const Value* v = decode_one(def, cluster, cname, attr, type, val, key);
    assert(v && v->type == ValueType::Uint && v->u == want);
}

void expect_int(const PreparedDefinition& def, std::uint16_t cluster,
                const char* cname, std::uint16_t attr, std::uint8_t type,
                const std::vector<std::uint8_t>& val, const char* key,
                std::int64_t want) {
    assert(def_exposes(def, key));
    const Value* v = decode_one(def, cluster, cname, attr, type, val, key);
    assert(v && v->type == ValueType::Int && v->i == want);
}

void check_rd_250zg() {
    const auto& def = devices::repenic_ltd::kDef_RD_250ZG;

    // ── 0x0B04 half (the dropped channels) via generic kFzElectricalMeasurement ──
    // ActivePower 0x050B (s16) -> power (signed pass-through).
    expect_int(def, HA_ELEC, "haElectricalMeasurement", 0x050B, 0x29, u16(0xFC18),
               "power", -1000);
    // RMSVoltage 0x0505 (u16) -> voltage.
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0505, 0x21, u16(2301),
                "voltage", 2301);
    // RMSCurrent 0x0508 (u16) -> current.
    expect_uint(def, HA_ELEC, "haElectricalMeasurement", 0x0508, 0x21, u16(1500),
                "current", 1500);

    // ── Regression: energy still decodes via generic kFzMetering (0x0702) ──
    expect_uint(def, SE_METERING, "seMetering", 0x0000, 0x25, u48(987654),
                "energy", 987654);

    // ── Regression: the light core exposes survive ──
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
}

}  // namespace

int main() {
    check_rd_250zg();
    return 0;
}
