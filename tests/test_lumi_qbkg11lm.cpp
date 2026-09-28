// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Aqara QBKG11LM (lumi.ctrl_ln1, single rocker, neutral).
// The def had been built from the 2-gang label map, so the relay state
// landed under `state_left` and never reached `state`; power, energy,
// action and operation_mode were missing, and battery/voltage were phantom
// exposes on a mains device.
//
// z2m-source: zigbee-herdsman-converters/src/devices/lumi.ts #QBKG11LM
//             (lumi_on_off, lumi_power, lumi_basic tag 0x95,
//              lumi_action_multistate, lumi_operation_mode_basic,
//              lumi_switch_operation_mode_basic).

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefQBKG11LM;
}  // namespace zhc::devices::lumi

using namespace zhc;

namespace {

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster_id,
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

std::vector<std::uint8_t> attr_report(std::uint16_t attr_id, std::uint8_t type,
                                      std::initializer_list<std::uint8_t> value) {
    std::vector<std::uint8_t> v{0x18, 0x42, 0x0A,
                                static_cast<std::uint8_t>(attr_id & 0xFF),
                                static_cast<std::uint8_t>(attr_id >> 8), type};
    v.insert(v.end(), value.begin(), value.end());
    return v;
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0)
            return &def.exposes[i];
    return nullptr;
}

bool enum_has(const Expose* e, const char* label) {
    for (std::uint8_t i = 0; e && i < e->enum_count; ++i)
        if (std::strcmp(e->enum_values[i], label) == 0) return true;
    return false;
}

bool str_is(const Value* v, const char* want) {
    return v && v->type == ValueType::StringRef && std::strcmp(v->str, want) == 0;
}

}  // namespace

int main() {
    const auto& def = devices::lumi::kDefQBKG11LM;

    // ── exposes: z2m switch, power, device_temperature, energy, action, operation_mode
    assert(find_expose(def, "state"));
    assert(find_expose(def, "power"));
    assert(find_expose(def, "energy"));
    assert(find_expose(def, "device_temperature"));
    const Expose* action = find_expose(def, "action");
    assert(action && action->type == ExposeType::Enum && action->enum_count == 4);
    for (const char* a : {"single", "double", "release", "hold"}) assert(enum_has(action, a));
    const Expose* opmode = find_expose(def, "operation_mode");
    assert(opmode && opmode->type == ExposeType::Enum && opmode->access == Access::StateSet);
    assert(enum_has(opmode, "control_relay") && enum_has(opmode, "decoupled"));
    // Mains device: no battery / voltage phantoms.
    assert(!find_expose(def, "battery"));
    assert(!find_expose(def, "voltage"));
    // z2m m.forcePowerSource({powerSource: "Mains (single phase)"}).
    assert(def.power_source_override == 0x01);

    // ── genOnOff ep1 onOff=1 → `state` (was `state_left`)
    {
        auto r = dispatch_zcl(def, 0x0006, 1, attr_report(0x0000, 0x10, {0x01}));
        const Value* s = r.merged.find("state");
        assert(s && s->type == ValueType::Bool && s->b);
        assert(!r.merged.find("state_left"));
    }
    // ── Decoupled rocker press: genOnOff report on EP4, no 0xF000. Not relay state
    //    (z2m lumi_on_off skips EPs 4-6) but z2m lumi_action's press, {0, 1} → single.
    {
        auto r = dispatch_zcl(def, 0x0006, 4, attr_report(0x0000, 0x10, {0x00}));
        assert(!r.merged.find("state"));
        assert(str_is(r.merged.find("action"), "single"));
        auto r1 = dispatch_zcl(def, 0x0006, 4, attr_report(0x0000, 0x10, {0x01}));
        assert(str_is(r1.merged.find("action"), "single"));
    }
    // ── The relay's own report carries 0xF000 (61440): state, no press.
    {
        const std::uint8_t f[] = {0x18, 0x43, 0x0A,
                                  0x00, 0x00, 0x10, 0x01,                      // onOff = 1
                                  0x00, 0xF0, 0x23, 0xCB, 0x00, 0x00, 0x07};   // 0xF000 u32
        auto r = dispatch_zcl(def, 0x0006, 1, f);
        const Value* s = r.merged.find("state");
        assert(s && s->type == ValueType::Bool && s->b);
        assert(!r.merged.find("action"));
        // z2m tests 0xF000 for truthiness, so a zero value counts as absent.
        const std::uint8_t f0[] = {0x18, 0x44, 0x0A,
                                   0x00, 0x00, 0x10, 0x01,
                                   0x00, 0xF0, 0x23, 0x00, 0x00, 0x00, 0x00};
        assert(str_is(dispatch_zcl(def, 0x0006, 1, f0).merged.find("action"), "single"));
    }
    // ── lumi_action takes attribute reports only: reading onOff back is no press.
    {
        const std::uint8_t rsp[] = {0x18, 0x45, 0x01, 0x00, 0x00, 0x00, 0x10, 0x01};
        auto r = dispatch_zcl(def, 0x0006, 1, rsp);
        assert(r.merged.find("state"));
        assert(!r.merged.find("action"));
    }

    // ── genAnalogInput presentValue (float 0x39, 12.5 W) → power
    {
        auto r = dispatch_zcl(def, 0x000C, 2, attr_report(0x0055, 0x39, {0x00, 0x00, 0x48, 0x41}));
        const Value* p = r.merged.find("power");
        assert(p && p->type == ValueType::Float && std::fabs(p->f - 12.5f) < 0.001f);
    }

    // ── genMultistateInput presentValue → action (0 hold, 1 single, 2 double, 255 release)
    {
        auto r1 = dispatch_zcl(def, 0x0012, 5, attr_report(0x0055, 0x21, {0x01, 0x00}));
        assert(str_is(r1.merged.find("action"), "single"));
        auto r2 = dispatch_zcl(def, 0x0012, 5, attr_report(0x0055, 0x21, {0xFF, 0x00}));
        assert(str_is(r2.merged.find("action"), "release"));
    }

    // ── genBasic 0xFF22 → operation_mode (0x12 control_relay, 0xFE decoupled)
    {
        auto r = dispatch_zcl(def, 0x0000, 1, attr_report(0xFF22, 0x20, {0xFE}));
        assert(str_is(r.merged.find("operation_mode"), "decoupled"));
        auto r2 = dispatch_zcl(def, 0x0000, 1, attr_report(0xFF22, 0x20, {0x12}));
        assert(str_is(r2.merged.find("operation_mode"), "control_relay"));
    }

    // ── genBasic 0xFF01 MI-struct: tag 0x03 device_temperature, tag 0x95 float energy
    {
        auto f = attr_report(0xFF01, 0x42, {0x09,
                                            0x03, 0x28, 0x21,                     // tag 3 i8 33 °C
                                            0x95, 0x39, 0x00, 0x00, 0x20, 0x40}); // tag 0x95 float 2.5
        auto r = dispatch_zcl(def, 0x0000, 1, f);
        const Value* e = r.merged.find("energy");
        assert(e && e->type == ValueType::Float && std::fabs(e->f - 2.5f) < 0.001f);
        const Value* t = r.merged.find("device_temperature");
        assert(t && t->type == ValueType::Int && t->i == 33);
    }

    // ── tz operation_mode → genBasic write 0xFF22 u8, Lumi manufacturer code 0x115F
    {
        RuntimeContext ctx{};
        std::uint8_t frame[32]{};
        Value v{}; v.type = ValueType::StringRef; v.str = "decoupled";
        auto r = dispatch_to_zigbee(def, "operation_mode", v, ctx, frame);
        assert(r.ok && r.cluster_id == 0x0000);
        const std::uint8_t want[] = {0x14, 0x5F, 0x11, 0x00, 0x02, 0x22, 0xFF, 0x20, 0xFE};
        assert(r.frame_size == sizeof(want) && std::memcmp(frame, want, sizeof(want)) == 0);

        v.str = "control_relay";
        r = dispatch_to_zigbee(def, "operation_mode", v, ctx, frame);
        assert(r.ok && frame[8] == 0x12);
    }

    return 0;
}
