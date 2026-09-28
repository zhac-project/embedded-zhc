// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for two plain wall switches that lacked power_on_behavior:
//   Tuya TS0011 — tuyaOnOff({backlightModeOffNormalInverted}): genOnOff
//     0x8002 power_on_behavior {off, on, previous} + 0x8001 backlight_mode
//     {off, normal, inverted}; configure = Tuya magic packet + bind genOnOff,
//     powerSource forced to Mains (single phase).
//   eWeLink SWITCH-ZR03-1 — m.onOff(): standard genOnOff 0x4003 startUpOnOff
//     {0 off, 1 on, 2 toggle, 255 previous}; configure = onOff reporting.
//
// z2m-source: zigbee-herdsman-converters/src/devices/tuya.ts #TS0011,
//             src/devices/ewelink.ts #SWITCH-ZR03-1, lib/tuya.ts tuyaOnOff,
//             lib/modernExtend.ts onOff.

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
extern const PreparedDefinition kDefTS0011;
}  // namespace zhc::devices::tuya
namespace zhc::devices::ewelink {
extern const PreparedDefinition kDef_SWITCH_ZR03_1;
}  // namespace zhc::devices::ewelink

using namespace zhc;

namespace {

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
    assert(frame[2] == 0x02 && frame[5] == 0x30);   // writeAttributes, enum8
    return (static_cast<std::uint32_t>(frame[3] | (frame[4] << 8)) << 8) | frame[6];
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, key) == 0) return &def.exposes[i];
    return nullptr;
}

void expect_enum(const PreparedDefinition& def, const char* key,
                 std::initializer_list<const char*> values) {
    const Expose* e = find_expose(def, key);
    assert(e && e->type == ExposeType::Enum && e->access == Access::StateSet);
    assert(e->enum_count == values.size());
    std::size_t i = 0;
    for (const char* v : values) assert(std::strcmp(e->enum_values[i++], v) == 0);
}

}  // namespace

int main() {
    // ── Tuya TS0011
    {
        const auto& def = devices::tuya::kDefTS0011;
        expect_enum(def, "power_on_behavior", {"off", "previous", "on"});
        expect_enum(def, "backlight_mode", {"off", "normal", "inverted"});

        assert(is(decode_enum8(def, 0x8002, 2, "power_on_behavior"), "previous"));
        assert(is(decode_enum8(def, 0x8001, 0, "backlight_mode"), "off"));
        assert(is(decode_enum8(def, 0x8001, 1, "backlight_mode"), "normal"));
        assert(is(decode_enum8(def, 0x8001, 2, "backlight_mode"), "inverted"));
        assert(!decode_enum8(def, 0x8001, 2, "indicator_mode"));

        assert(encode(def, "power_on_behavior", "previous") == 0x800202);
        assert(encode(def, "backlight_mode", "off")      == 0x800100);
        assert(encode(def, "backlight_mode", "normal")   == 0x800101);
        assert(encode(def, "backlight_mode", "inverted") == 0x800102);

        // Magic packet: genBasic read of 4, 0, 1, 5, 7, 0xFFFE.
        bool magic = false;
        for (std::size_t i = 0; i < def.config_steps_count; ++i) {
            const ConfigStep& s = def.config_steps[i];
            if (s.op != ConfigStepOp::Read || s.cluster_id != 0x0000) continue;
            const std::uint8_t want[] = {0x04, 0x00, 0x00, 0x00, 0x01, 0x00,
                                         0x05, 0x00, 0x07, 0x00, 0xFE, 0xFF};
            magic = s.payload_len == sizeof(want) &&
                    std::memcmp(s.payload, want, sizeof(want)) == 0;
        }
        assert(magic);
        assert(def.power_source_override == 0x01);   // Mains (single phase)

        // z2m v26.105.0: zigbeeModel ["TS0011", "ZG-302Z1"] (any manufacturer
        // without its own definition), tuyaBase() → the magic packet above.
        const std::span<const PreparedDefinition* const> reg(devices::tuya::kTuyaRegistry,
                                                             devices::tuya::kTuyaRegistryCount);
        assert(find_definition("ZG-302Z1", "_TZ3000_zzzzzzzz", reg) == &def);
        assert(find_definition("TS0011", "_TZ3000_l8fsgo6p", reg) == &def);
    }

    // ── eWeLink SWITCH-ZR03-1
    {
        const auto& def = devices::ewelink::kDef_SWITCH_ZR03_1;
        expect_enum(def, "power_on_behavior", {"off", "on", "toggle", "previous"});
        assert(is(decode_enum8(def, 0x4003, 0, "power_on_behavior"), "off"));
        assert(is(decode_enum8(def, 0x4003, 2, "power_on_behavior"), "toggle"));
        assert(is(decode_enum8(def, 0x4003, 0xFF, "power_on_behavior"), "previous"));
        assert(encode(def, "power_on_behavior", "previous") == 0x4003FF);
        assert(encode(def, "power_on_behavior", "toggle")   == 0x400302);

        bool onoff_report = false;
        for (std::size_t i = 0; i < def.reports_count; ++i)
            onoff_report |= def.reports[i].cluster_id == 0x0006 && def.reports[i].attr_id == 0x0000;
        assert(onoff_report);
    }
    return 0;
}
