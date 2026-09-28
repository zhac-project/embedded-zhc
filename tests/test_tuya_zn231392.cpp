// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Tuya ZN231392 (smart water/gas valve). z2m lists it under
// three fingerprint lists (TS011F × 2, TS0001 × 9, TS0011 × 1); ZHAC had no
// definition, so the valve fell to the generic TS0001 / TS011F / TS0011
// switch defs (TS011F added phantom power/energy meters). z2m:
// tuyaOnOff({indicatorMode}) → switch + power_on_behavior (genOnOff 0x8002)
// + indicator_mode (0x8001); configure = magic packet + read
// [onOff, moesStartUpOnOff].
//
// A PreparedDefinition matches models × manufacturers as a cross product,
// so each z2m fingerprint list is its own def (sharing the converter set);
// this pins that no model/manufacturer pair outside z2m's lists matches.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tuya.ts #ZN231392.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/cluster_names.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDefZN231392_TS011F;
extern const PreparedDefinition kDefZN231392_TS0001;
extern const PreparedDefinition kDefZN231392_TS0011;
extern const PreparedDefinition kDefTS0001;
extern const PreparedDefinition kDefTS0011;
extern const PreparedDefinition kDefTS011F;
}  // namespace zhc::devices::tuya

using namespace zhc;
using namespace zhc::devices::tuya;

namespace {

std::span<const PreparedDefinition* const> tuya_reg() {
    return {kTuyaRegistry, kTuyaRegistryCount};
}

const char* decode_enum8(const PreparedDefinition& def, std::uint16_t attr,
                         std::uint8_t value, const char* key) {
    const std::uint8_t bytes[] = {0x18, 0x42, 0x0A,
                                  static_cast<std::uint8_t>(attr & 0xFF),
                                  static_cast<std::uint8_t>(attr >> 8), 0x30, value};
    InboundApsFrame raw{};
    raw.cluster_id   = 0x0006;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x0006);
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    const Value* v = r.merged.find(key);
    return (v && v->type == ValueType::StringRef) ? v->str : nullptr;
}

bool is(const char* got, const char* want) { return got && std::strcmp(got, want) == 0; }

// Encodes `key` = `label`; returns (attr_id << 8) | value of the genOnOff write.
std::uint32_t encode(const PreparedDefinition& def, const char* key, const char* label) {
    RuntimeContext ctx{};
    std::uint8_t frame[32]{};
    Value v{}; v.type = ValueType::StringRef; v.str = label;
    const auto r = dispatch_to_zigbee(def, key, v, ctx, frame);
    assert(r.ok && r.cluster_id == 0x0006 && r.frame_size == 7);
    return (static_cast<std::uint32_t>(frame[3] | (frame[4] << 8)) << 8) | frame[6];
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, key) == 0) return &def.exposes[i];
    return nullptr;
}

void check_def(const PreparedDefinition& def) {
    assert(std::strcmp(def.model, "ZN231392") == 0);

    // Exposes: switch, power_on_behavior, indicator_mode — and no meters.
    assert(find_expose(def, "state"));
    const Expose* pob = find_expose(def, "power_on_behavior");
    assert(pob && pob->type == ExposeType::Enum && pob->enum_count == 3 &&
           pob->access == Access::StateSet);
    const Expose* ind = find_expose(def, "indicator_mode");
    assert(ind && ind->type == ExposeType::Enum && ind->enum_count == 4 &&
           ind->access == Access::StateSet);
    const char* ind_labels[] = {"off", "off/on", "on/off", "on"};
    for (std::uint8_t i = 0; i < 4; ++i) assert(std::strcmp(ind->enum_values[i], ind_labels[i]) == 0);
    for (const char* k : {"power", "voltage", "current", "energy", "child_lock"})
        assert(!find_expose(def, k));

    // Decode + encode, z2m values.
    assert(is(decode_enum8(def, 0x8002, 2, "power_on_behavior"), "previous"));
    for (std::uint8_t raw = 0; raw < 4; ++raw)
        assert(is(decode_enum8(def, 0x8001, raw, "indicator_mode"), ind_labels[raw]));
    assert(encode(def, "power_on_behavior", "previous") == 0x800202);
    assert(encode(def, "indicator_mode", "on/off") == 0x800102);
    {
        RuntimeContext ctx{};
        std::uint8_t frame[32]{};
        Value v{}; v.type = ValueType::Bool; v.b = true;
        assert(dispatch_to_zigbee(def, "state", v, ctx, frame).ok);
    }

    // Configure: magic packet, then read genOnOff [onOff, moesStartUpOnOff].
    assert(def.config_steps_count == 2);
    const ConfigStep& magic = def.config_steps[0];
    const std::uint8_t magic_attrs[] = {0x04, 0x00, 0x00, 0x00, 0x01, 0x00,
                                        0x05, 0x00, 0x07, 0x00, 0xFE, 0xFF};
    assert(magic.op == ConfigStepOp::Read && magic.cluster_id == 0x0000);
    assert(magic.payload_len == sizeof(magic_attrs) &&
           std::memcmp(magic.payload, magic_attrs, sizeof(magic_attrs)) == 0);
    const ConfigStep& rd = def.config_steps[1];
    const std::uint8_t rd_attrs[] = {0x00, 0x00, 0x02, 0x80};
    assert(rd.op == ConfigStepOp::Read && rd.cluster_id == 0x0006);
    assert(rd.payload_len == sizeof(rd_attrs) &&
           std::memcmp(rd.payload, rd_attrs, sizeof(rd_attrs)) == 0);
}

}  // namespace

int main() {
    check_def(kDefZN231392_TS011F);
    check_def(kDefZN231392_TS0001);
    check_def(kDefZN231392_TS0011);

    // Every z2m fingerprint resolves to ZN231392 …
    const char* ts011f[] = {"_TZ3000_rk2yzt0u", "_TZ3000_o4cjetlm"};
    const char* ts0001[] = {"_TZ3000_o4cjetlm", "_TZ3000_iedbgyxt", "_TZ3000_h3noz0a5",
                            "_TYZB01_4tlksk8a", "_TZ3000_5ucujjts", "_TZ3000_h8ngtlxy",
                            "_TZ3000_w0ypwa1f", "_TZ3000_wpueorev", "_TZ3000_cmcjbqup"};
    for (const char* m : ts011f) assert(find_definition("TS011F", m, tuya_reg()) == &kDefZN231392_TS011F);
    for (const char* m : ts0001) assert(find_definition("TS0001", m, tuya_reg()) == &kDefZN231392_TS0001);
    assert(find_definition("TS0011", "_TYZB01_rifa0wlb", tuya_reg()) == &kDefZN231392_TS0011);

    // … and nothing outside them does (no model × manufacturer cross product).
    assert(find_definition("TS011F", "_TZ3000_iedbgyxt", tuya_reg()) == &kDefTS011F);
    assert(find_definition("TS0011", "_TZ3000_o4cjetlm", tuya_reg()) == &kDefTS0011);
    assert(find_definition("TS0001", "_TYZB01_rifa0wlb", tuya_reg()) == &kDefTS0001);
    assert(find_definition("TS0011", "_TZ3000_l8fsgo6p", tuya_reg()) == &kDefTS0011);
    return 0;
}
