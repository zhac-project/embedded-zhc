// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the VAV (NodOn-derived) modules.
//   * VAV-256215-MOD1 "Pilot wire heating module" — z2m extends with
//     m.onOff({powerOnBehavior:true}) + m.electricityMeter({cluster:
//     "metering"}) + ...nodonPilotWire(true). The Tier-1 auto-port
//     dropped BOTH the power_on_behavior decode (genOnOff 0x4003) and
//     the entire NodOn pilot-wire cluster (0xFC00 attr 0x0000 → mode).
//   * VAV-256215-MOD2 "Multifunction relay switch with metering" —
//     same onOff+metering; the Tier-1 port dropped power_on_behavior.
//
// metering uses z2m cluster:"metering" = seMetering 0x0702 ONLY (energy
// + power), so there is intentionally NO current/voltage 0x0B04 surface
// — this test pins that the metering exposes stay energy+power and no
// phantom current/voltage was added.
//
// z2m-source: zigbee-herdsman-converters/src/devices/vav.ts +
//             lib/nodon.ts nodonPilotWire + lib/modernExtend.ts onOff.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::vav {
extern const PreparedDefinition kDef_VAV_256215_MOD1;
extern const PreparedDefinition kDef_VAV_256215_MOD2;
}  // namespace zhc::devices::vav

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

// ZCL attribute report builder: fc=0x18, tsn, cmd=0x0A, then one record.
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool str_is(const Value* v, const char* s) {
    return v && v->type == ValueType::StringRef && v->str && std::strcmp(v->str, s) == 0;
}

}  // namespace

int main() {
    using namespace zhc::devices::vav;

    // ── VAV-256215-MOD1 (pilot wire heating module) ──────────────────
    {
        const PreparedDefinition& def = kDef_VAV_256215_MOD1;
        assert(def_exposes(def, "state"));
        assert(def_exposes(def, "power_on_behavior"));
        assert(def_exposes(def, "pilot_wire_mode"));
        assert(def_exposes(def, "energy"));
        assert(def_exposes(def, "power"));
        // metering = cluster:"metering" (0x0702 only) — NO 0x0B04 surface.
        assert(!def_exposes(def, "current"));
        assert(!def_exposes(def, "voltage"));

        // power_on_behavior (genOnOff 0x4003 enum8): 0xFF → previous.
        {
            const std::uint8_t b[] = {0xFF};
            auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                                  attr_report(0x4003, 0x30, b));
            assert(r.any_matched);
            assert(str_is(r.merged.find("power_on_behavior"), "previous"));
        }
        // pilot_wire_mode: NodOn 0xFC00 attr 0x0000 (mode, uint8).
        // 1 → comfort.
        {
            const std::uint8_t b[] = {0x01};
            auto r = dispatch_zcl(def, 0xFC00, "manuSpecificPhilips", 1,
                                  attr_report(0x0000, 0x20, b));
            assert(r.any_matched);
            assert(str_is(r.merged.find("pilot_wire_mode"), "comfort"));
        }
        // 3 → frost_protection.
        {
            const std::uint8_t b[] = {0x03};
            auto r = dispatch_zcl(def, 0xFC00, "manuSpecificPhilips", 1,
                                  attr_report(0x0000, 0x20, b));
            assert(str_is(r.merged.find("pilot_wire_mode"), "frost_protection"));
        }
        // 5 → comfort_-2 (NodOn-specific high value).
        {
            const std::uint8_t b[] = {0x05};
            auto r = dispatch_zcl(def, 0xFC00, "manuSpecificPhilips", 1,
                                  attr_report(0x0000, 0x20, b));
            assert(str_is(r.merged.find("pilot_wire_mode"), "comfort_-2"));
        }
        // A genOnOff state report (attr 0x0000 bool) must NOT mis-fire
        // the pilot-wire decoder — it lives on a different cluster.
        {
            const std::uint8_t b[] = {0x01};
            auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                                  attr_report(0x0000, 0x10, b));
            assert(r.any_matched);
            assert(r.merged.find("state") != nullptr);
            assert(r.merged.find("pilot_wire_mode") == nullptr);
        }
    }

    // ── VAV-256215-MOD2 (relay switch with metering) ─────────────────
    {
        const PreparedDefinition& def = kDef_VAV_256215_MOD2;
        assert(def_exposes(def, "state"));
        assert(def_exposes(def, "power_on_behavior"));
        assert(def_exposes(def, "energy"));
        assert(def_exposes(def, "power"));
        // No pilot wire on MOD2, and metering=0x0702 only.
        assert(!def_exposes(def, "pilot_wire_mode"));
        assert(!def_exposes(def, "current"));
        assert(!def_exposes(def, "voltage"));

        // power_on_behavior: 0x00 → off.
        {
            const std::uint8_t b[] = {0x00};
            auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                                  attr_report(0x4003, 0x30, b));
            assert(r.any_matched);
            assert(str_is(r.merged.find("power_on_behavior"), "off"));
        }
        // 0x01 → on.
        {
            const std::uint8_t b[] = {0x01};
            auto r = dispatch_zcl(def, 0x0006, "genOnOff", 1,
                                  attr_report(0x4003, 0x30, b));
            assert(str_is(r.merged.find("power_on_behavior"), "on"));
        }
    }

    return 0;
}
