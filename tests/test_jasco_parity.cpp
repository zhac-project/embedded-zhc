// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Jasco / GE relay + metering family.
//
//   * 43095 (smart plug-in switch w/ metering): z2m fromZigbee
//       [fz.command_on_state, fz.command_off_state] +
//       extend [m.onOff(), m.electricityMeter({cluster: "metering"})].
//     The device reports its physical button presses as genOnOff *commands*
//     (commandOn 0x01 / commandOff 0x00), which z2m folds into the `state`
//     property ("ON"/"OFF") — NOT into `action`. The auto-port carried only
//     kFzOnOff (attribute 0x0000) + kFzMetering, so every physical toggle was
//     dropped. Fixed by graduating the def and wiring the jasco
//     kFzCommandOnState / kFzCommandOffState command-to-state decoders.
//
//   * 43132 (smart outlet): z2m [m.onOff(), m.electricityMeter({cluster:
//     "metering"})] — cluster "metering" = seMetering 0x0702 only (NO
//     0x0B04 electrical-measurement half). Regression-guard: the metering
//     half binds 0x0702 and decodes energy/power, and there is NO 0x0B04
//     binding / current/voltage expose.
//
//   * ZB3102 (plug-in dimmer): z2m m.light() — onOff + brightness. Unaffected
//     by the 43095 fix; sanity-checked here.
//
// z2m-source: zigbee-herdsman-converters/src/devices/jasco.ts +
//             converters/fromZigbee.ts command_on_state / command_off_state.

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

namespace zhc::devices::jasco {
extern const PreparedDefinition kDef_D43095;
extern const PreparedDefinition kDef_D43132;
extern const PreparedDefinition kDef_ZB3102;
}  // namespace zhc::devices::jasco

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

DispatchResult dispatch_zcl(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, 1, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd) {
    return {0x01, 0x42, cmd};
}

// ZCL attribute report (fc=0x18, tsn, cmd=0x0A, one record).
std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::span<const std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A};
    v.push_back(static_cast<std::uint8_t>(attr_id & 0xFF));
    v.push_back(static_cast<std::uint8_t>(attr_id >> 8));
    v.push_back(type);
    for (auto b : value) v.push_back(b);
    return v;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

// ── 43095: genOnOff commands → state; attribute + metering still work. ─────
void check_43095(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(!def_exposes(def, "action"));   // z2m folds commands into state

    // genOnOff commandOn (0x01) → state = true. THE FIX: previously dropped.
    {
        RuntimeContext ctx{};
        auto r = dispatch_zcl(ctx, def, 0x0006, "genOnOff", cmd_frame(0x01));
        assert(r.any_matched);
        assert(b_true(r.merged.find("state")));
        assert(r.merged.find("action") == nullptr);   // never into action
    }
    // genOnOff commandOff (0x00) → state = false.
    {
        RuntimeContext ctx{};
        auto r = dispatch_zcl(ctx, def, 0x0006, "genOnOff", cmd_frame(0x00));
        assert(r.any_matched);
        assert(b_false(r.merged.find("state")));
        assert(r.merged.find("action") == nullptr);
    }
    // Attribute path still decodes: onOff attr 0x0000 (bool) → state.
    {
        RuntimeContext ctx{};
        const std::array<std::uint8_t, 1> on = {0x01};
        auto r = dispatch_zcl(ctx, def, 0x0006, "genOnOff",
                              attr_report(0x0000, 0x10,
                                          std::span<const std::uint8_t>(on.data(), on.size())));
        assert(r.any_matched);
        assert(b_true(r.merged.find("state")));
    }
    // seMetering instantaneousDemand (attr 0x0400, s24) → power.
    {
        RuntimeContext ctx{};
        const std::array<std::uint8_t, 3> pw = {0x64, 0x00, 0x00};   // 100
        auto r = dispatch_zcl(ctx, def, 0x0702, "seMetering",
                              attr_report(0x0400, 0x2A,
                                          std::span<const std::uint8_t>(pw.data(), pw.size())));
        assert(r.any_matched);
        const Value* p = r.merged.find("power");
        assert(p && (p->type == ValueType::Int || p->type == ValueType::Uint));
    }

    // Binds genOnOff + seMetering, NOT haElectricalMeasurement (cluster
    // "metering" = 0x0702-only).
    assert(def_binds(def, 0x0006));
    assert(def_binds(def, 0x0702));
    assert(!def_binds(def, 0x0B04));
    assert(!def_exposes(def, "current"));
    assert(!def_exposes(def, "voltage"));
}

// ── 43132: metering = 0x0702-only outlet; no 0x0B04 half. ─────────────────
void check_43132(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "energy"));
    assert(def_exposes(def, "power"));
    assert(def_binds(def, 0x0702));
    assert(!def_binds(def, 0x0B04));
    assert(!def_exposes(def, "current"));
    assert(!def_exposes(def, "voltage"));

    // seMetering currentSummDelivered (attr 0x0000, u48) → energy.
    RuntimeContext ctx{};
    const std::array<std::uint8_t, 6> en = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00};
    auto r = dispatch_zcl(ctx, def, 0x0702, "seMetering",
                          attr_report(0x0000, 0x25,
                                      std::span<const std::uint8_t>(en.data(), en.size())));
    assert(r.any_matched);
    assert(r.merged.find("energy") != nullptr);
}

// ── ZB3102: plain dimmer (onOff + brightness); unaffected. ────────────────
void check_ZB3102(const PreparedDefinition& def) {
    assert(def_exposes(def, "state"));
    assert(def_exposes(def, "brightness"));
    assert(!def_exposes(def, "energy"));

    // genLevelCtrl currentLevel attr 0x0000 (u8) → brightness.
    RuntimeContext ctx{};
    const std::array<std::uint8_t, 1> lv = {0x80};
    auto r = dispatch_zcl(ctx, def, 0x0008, "genLevelCtrl",
                          attr_report(0x0000, 0x20,
                                      std::span<const std::uint8_t>(lv.data(), lv.size())));
    assert(r.any_matched);
    const Value* b = r.merged.find("brightness");
    assert(b && b->type == ValueType::Uint && b->u == 128);
}

}  // namespace

int main() {
    using namespace zhc::devices::jasco;
    check_43095(kDef_D43095);
    check_43132(kDef_D43132);
    check_ZB3102(kDef_ZB3102);
    return 0;
}
