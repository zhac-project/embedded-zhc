// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Moes two-endpoint on/off definitions that the Moes
// registry-count fix made reachable as generated stubs with one bare
// `state` (the second gang overwrote the first, and had no control).
//   MS-104BZ (TS011F _TZ3000_pmz6mjyu / _TZ3000_iv6ph5tr): tuyaBase +
//     tuyaOnOff({endpoints: [l1, l2]}) — state_l1 / state_l2 and one
//     power_on_behavior on genOnOff 0x8002 {off, on, previous};
//     configure = magic packet, bind + reporting.onOff on EP1 and EP2.
//
// z2m-source: zigbee-herdsman-converters/src/devices/moes.ts #MS-104BZ,
//             lib/tuya.ts tuyaBase / tuyaOnOff, lib/reporting.ts onOff.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <span>
#include <vector>

#include "zhc/cluster_names.hpp"
#include "zhc/devices/moes_registry.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::moes {
extern const PreparedDefinition kDef_MS_104BZ;
}  // namespace zhc::devices::moes

using namespace zhc;

namespace {

// One genOnOff Report Attributes frame from `ep`; returns what was emitted
// under `key`. Suffixed keys live in the RuntimeContext, so read them here.
struct Got { bool found; Value v; };
Got report(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t attr,
           std::uint8_t type, std::uint8_t value, const char* key) {
    const std::uint8_t bytes[] = {0x18, 0x42, 0x0A,
                                  static_cast<std::uint8_t>(attr & 0xFF),
                                  static_cast<std::uint8_t>(attr >> 8), type, value};
    InboundApsFrame raw{};
    raw.cluster_id   = 0x0006;
    raw.src_endpoint = ep;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_id_to_name(0x0006);
    RuntimeContext ctx{};
    const auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    const Value* v = r.merged.find(key);
    return {v != nullptr, v ? *v : Value{}};
}

bool is_bool(const Got& g, bool want) {
    return g.found && g.v.type == ValueType::Bool && g.v.b == want;
}
bool is_str(const Got& g, const char* want) {
    return g.found && g.v.type == ValueType::StringRef && g.v.str && std::strcmp(g.v.str, want) == 0;
}

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

void expect_switch(const PreparedDefinition& def, const char* key) {
    const Expose* e = find_expose(def, key);
    assert(e && e->type == ExposeType::Binary && e->access == Access::StateSet);
}

void expect_enum(const PreparedDefinition& def, const char* key,
                 std::initializer_list<const char*> values) {
    const Expose* e = find_expose(def, key);
    assert(e && e->type == ExposeType::Enum && e->access == Access::StateSet &&
           e->category == ExposeCategory::Config);
    assert(e->enum_count == values.size());
    std::size_t i = 0;
    for (const char* v : values) assert(std::strcmp(e->enum_values[i++], v) == 0);
}

void expect_l1_l2(const PreparedDefinition& def) {
    assert(def.endpoint_map_count == 2);
    assert(std::strcmp(def.endpoint_map[0].label, "l1") == 0 && def.endpoint_map[0].endpoint == 1);
    assert(std::strcmp(def.endpoint_map[1].label, "l2") == 0 && def.endpoint_map[1].endpoint == 2);
}

bool binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// onOff (bool) reporting on `ep`: min 0, `max_s`, `change`.
bool reports_onoff(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t max_s,
                   std::uint32_t change) {
    for (std::size_t i = 0; i < def.reports_count; ++i) {
        const ReportingSpec& r = def.reports[i];
        if (r.endpoint == ep && r.cluster_id == 0x0006 && r.attr_id == 0x0000)
            return r.attr_type == 0x10 && r.min_interval == 0 && r.max_interval == max_s &&
                   r.reportable_change == change;
    }
    return false;
}

// A Read step on `ep`/`cluster` with exactly these attribute bytes (LE ids).
bool reads(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster,
           std::initializer_list<std::uint8_t> attrs) {
    const std::vector<std::uint8_t> want(attrs);
    for (std::size_t i = 0; i < def.config_steps_count; ++i) {
        const ConfigStep& s = def.config_steps[i];
        if (s.op == ConfigStepOp::Read && s.endpoint == ep && s.cluster_id == cluster &&
            s.payload_len == want.size() && std::memcmp(s.payload, want.data(), want.size()) == 0)
            return true;
    }
    return false;
}

// The tuya + moes registries, in adapter order.
const PreparedDefinition* match(const char* model, const char* manu) {
    std::vector<const PreparedDefinition*> merged(devices::tuya::kTuyaRegistry,
        devices::tuya::kTuyaRegistry + devices::tuya::kTuyaRegistryCount);
    merged.insert(merged.end(), devices::moes::kMoesRegistry,
                  devices::moes::kMoesRegistry + devices::moes::kMoesRegistryCount);
    return find_definition(model, manu,
                           std::span<const PreparedDefinition* const>(merged.data(), merged.size()));
}

}  // namespace

int main() {
    // ── Moes MS-104BZ
    {
        const auto& def = devices::moes::kDef_MS_104BZ;
        expect_l1_l2(def);
        expect_switch(def, "state_l1");
        expect_switch(def, "state_l2");
        assert(!find_expose(def, "state"));
        // tuyaOnOff's default power-on branch: one setting, e.power_on_behavior().
        expect_enum(def, "power_on_behavior", {"off", "previous", "on"});

        assert(is_bool(report(def, 1, 0x0000, 0x10, 0x01, "state_l1"), true));
        assert(is_bool(report(def, 2, 0x0000, 0x10, 0x00, "state_l2"), false));
        // moesStartUpOnOff; z2m's postfixWithEndpointName names the report the same way.
        assert(is_str(report(def, 1, 0x8002, 0x30, 0x02, "power_on_behavior_l1"), "previous"));
        assert(encode(def, "power_on_behavior", "previous") == 0x800202);
        assert(encode(def, "power_on_behavior", "off")      == 0x800200);

        // configure: bind + reporting.onOff (0 / 3600 / 0) on both gangs; tuyaBase
        // magic packet (genBasic read of 4, 0, 1, 5, 7, 0xFFFE).
        assert(binds(def, 1, 0x0006) && binds(def, 2, 0x0006));
        assert(reports_onoff(def, 1, 3600, 0) && reports_onoff(def, 2, 3600, 0));
        assert(reads(def, 1, 0x0000, {0x04, 0x00, 0x00, 0x00, 0x01, 0x00,
                                      0x05, 0x00, 0x07, 0x00, 0xFE, 0xFF}));

        // Picked ahead of the generic TS011F plug by manufacturer name.
        assert(match("TS011F", "_TZ3000_pmz6mjyu") == &def);
        assert(match("TS011F", "_TZ3000_iv6ph5tr") == &def);
    }
    return 0;
}
