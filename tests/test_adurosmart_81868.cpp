// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// AduroSmart 81868 (ERIA Plug-in Siren) follows z2m `adurosmart.extend.siren()`:
// m.iasZoneAlarm({zoneType: "alarm", zoneAttributes: ["alarm_1"]}) + m.iasWarning().
// It matches all three ERIA manufacturer names, publishes only `alarm` (zoneStatus
// bit 0, from a status change or an attribute report) and takes `warning` as its
// one command. z2m gives the plug-in siren no battery, tamper, squawk or
// max_duration, and configures nothing.
//
// z2m-source: adurosmart.ts #81868 (v26.133.0).

#include <cassert>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <span>
#include <vector>

#include "zhc/devices/adurosmart_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"
#include "test_ctx.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> registry() {
    return {devices::adurosmart::kAdurosmartRegistry, devices::adurosmart::kAdurosmartRegistryCount};
}

const Expose* expose(const PreparedDefinition& d, const char* name) {
    for (std::size_t i = 0; i < d.exposes_count; ++i)
        if (std::strcmp(d.exposes[i].name, name) == 0) return &d.exposes[i];
    return nullptr;
}

// ssIasZone (0x0500) frame from endpoint 1; `bytes` must outlive the result.
DispatchResult dispatch(const PreparedDefinition& def, const std::vector<std::uint8_t>& bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = 0x0500;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = std::span<const std::uint8_t>(bytes.data(), bytes.size());
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    return dispatch_from_zigbee(msg, {}, def, raw, test_ctx());
}

bool bool_is(const DispatchResult& r, const char* key, bool want) {
    const Value* v = r.merged.find(key);
    return v && v->type == ValueType::Bool && v->b == want;
}

struct Encoded { bool ok = false; std::uint16_t cluster = 0; std::vector<std::uint8_t> frame; };
Encoded encode(const PreparedDefinition& def, const char* key, const Value& v) {
    Encoded e{};
    RuntimeContext ctx{};
    std::uint8_t buf[96] = {};
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        const TzConverter* c = def.to_zigbee[i];
        if (!c || (c->key && std::strcmp(c->key, key) != 0)) continue;   // null key = wildcard
        std::size_t n = 0;
        if (c->fn(key, v, *c, def, ctx, std::span<std::uint8_t>(buf, sizeof(buf)), n) && n) {
            e.ok = true; e.cluster = c->cluster_id;
            e.frame.assign(buf, buf + n);
            return e;
        }
    }
    return e;
}

Value boolean(bool b) { Value v{}; v.type = ValueType::Bool; v.b = b; return v; }

}  // namespace

int main() {
    // z2m fingerprints modelID "Smart Siren" with three manufacturer names.
    const PreparedDefinition* def = nullptr;
    for (const char* manu : {"AduroSmart ERIA", "ERIA", "AduroSmart Eria"}) {
        def = find_definition("Smart Siren", manu, registry());
        assert(def && std::strcmp(def->model, "81868") == 0);
    }
    const PreparedDefinition& d = *def;

    // Exposes: the `alarm` state and the `warning` command, nothing else.
    assert(d.exposes_count == 2);
    const Expose* alarm = expose(d, "alarm");
    assert(alarm && alarm->type == ExposeType::Binary && alarm->access == Access::State);
    const Expose* warning = expose(d, "warning");
    assert(warning && warning->type == ExposeType::Binary && warning->access == Access::Set);
    assert(d.bindings_count == 0);

    // Zone Status Change Notification, zoneStatus 0x0005 (alarm_1 + tamper):
    // `alarm` only, as zoneAttributes ["alarm_1"] leaves the tamper bit out.
    const std::vector<std::uint8_t> on{0x09, 0x01, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x00};
    DispatchResult r = dispatch(d, on);
    assert(bool_is(r, "alarm", true));
    assert(!r.merged.find("tamper") && !r.merged.find("battery_low"));
    const std::vector<std::uint8_t> off{0x09, 0x02, 0x00, 0x04, 0x00, 0x00, 0x01, 0x00, 0x00};
    r = dispatch(d, off);
    assert(bool_is(r, "alarm", false));

    // zoneStatus attribute report (attr 0x0002, bitmap16 0x0009 = alarm_1 + battery_low).
    const std::vector<std::uint8_t> rep{0x18, 0x03, 0x0A, 0x02, 0x00, 0x19, 0x09, 0x00};
    r = dispatch(d, rep);
    assert(bool_is(r, "alarm", true));
    assert(!r.merged.find("tamper") && !r.merged.find("battery_low"));

    // warning → ssIasWd startWarning (cmd 0x00); false stops it (mode 0, duration 0).
    Encoded w = encode(d, "warning", boolean(true));
    assert(w.ok && w.cluster == 0x0502 && w.frame.size() == 8 && w.frame[2] == 0x00 && w.frame[3] != 0x00);
    w = encode(d, "warning", boolean(false));
    assert(w.ok && w.frame.size() == 8 && w.frame[3] == 0x00 && w.frame[4] == 0x00 && w.frame[5] == 0x00);

    // `alarm` is a state, not a second siren command; no squawk, no max_duration.
    assert(!encode(d, "alarm", boolean(true)).ok);
    assert(!encode(d, "squawk", boolean(true)).ok);
    Value secs{}; secs.type = ValueType::Uint; secs.u = 30;
    assert(!encode(d, "max_duration", secs).ok);
    return 0;
}
