// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Brun Holding "Fire Fence" (zigbeeModel "Power Control Unit"),
// a dual-relay stove guard with power metering, ambient temperature, and
// battery. Pins the real gaps the auto-generated port carried against the z2m
// wire shape (brun_holding.ts):
//
//   * endpoint_map mapped short_override to EP1 instead of EP2 (z2m
//     m.deviceEndpoints({main_switch:1, short_override:2})). Both relays then
//     collided on the bare `state` key. Restored EP2 so EP1 -> state_main_switch
//     and EP2 -> state_short_override.
//
//   * m.temperature() (msTemperatureMeasurement 0x0402) was dropped entirely —
//     no kFzTemperature, no expose, no bind. Re-added; EP1 report suffixes to
//     `temperature_main_switch` (multi-EP convention).
//
//   * Phantom `voltage` expose: z2m m.electricityMeter sets voltage:false +
//     current:false, so only power + energy stream from seMetering 0x0702. The
//     generic kFzMetering never emits voltage, so the expose was dead. Removed.
//     z2m tags metering to main_switch -> power_main_switch / energy_main_switch.
//
// z2m-source: zigbee-herdsman-converters/src/devices/brun_holding.ts.

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

namespace zhc::devices::brun_holding {
extern const PreparedDefinition kDef_Fire_Fence;
}  // namespace zhc::devices::brun_holding

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

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

// Keep `ctx` alive while the result is read: multi-endpoint suffixing allocates
// the rewritten keys in RuntimeContext scratch, so pointers alias `ctx`.
DispatchResult dispatch_zcl_ep(RuntimeContext& ctx, const PreparedDefinition& def,
                               std::uint16_t cluster_id, std::uint8_t src_ep,
                               const char* cluster_name,
                               std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// ZCL attribute report builder (fc=0x18 S->C, tsn, cmd=0x0A, one record).
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

}  // namespace

// ── Shape: z2m endpoint-tagged keys; no phantom voltage; no bare state. ─────
static void test_shape() {
    using namespace zhc::devices::brun_holding;
    const auto& def = kDef_Fire_Fence;

    // Per-relay state (suffixed), per-EP metering, ambient temp, battery.
    assert(def_exposes(def, "state_main_switch"));
    assert(def_exposes(def, "state_short_override"));
    assert(def_exposes(def, "power_main_switch"));
    assert(def_exposes(def, "energy_main_switch"));
    assert(def_exposes(def, "temperature_main_switch"));
    assert(def_exposes(def, "battery"));

    // Dead / phantom keys must be gone.
    assert(!def_exposes(def, "voltage"));   // z2m voltage:false
    assert(!def_exposes(def, "current"));   // z2m current:false
    assert(!def_exposes(def, "state"));     // collided bare key removed

    // endpoint_map: main_switch=1, short_override=2 (z2m deviceEndpoints).
    assert(def.endpoint_map && def.endpoint_map_count == 2);
    std::uint8_t main_ep = 0, ovr_ep = 0;
    for (std::size_t i = 0; i < def.endpoint_map_count; ++i) {
        if (std::strcmp(def.endpoint_map[i].label, "main_switch") == 0)
            main_ep = def.endpoint_map[i].endpoint;
        if (std::strcmp(def.endpoint_map[i].label, "short_override") == 0)
            ovr_ep = def.endpoint_map[i].endpoint;
    }
    assert(main_ep == 1);
    assert(ovr_ep == 2);

    // Bindings: onOff per relay EP, battery/temp/metering on EP1.
    assert(def_binds(def, 1, 0x0006));   // main_switch onOff
    assert(def_binds(def, 2, 0x0006));   // short_override onOff
    assert(def_binds(def, 1, 0x0001));   // battery
    assert(def_binds(def, 1, 0x0402));   // temperature
    assert(def_binds(def, 1, 0x0702));   // metering
}

// ── Dual relay: EP1 onOff -> state_main_switch, EP2 -> state_short_override. ─
static void test_relays() {
    using namespace zhc::devices::brun_holding;
    const auto& def = kDef_Fire_Fence;

    // EP1 genOnOff = ON (bool true, type 0x10).
    RuntimeContext c1{};
    const std::array<std::uint8_t, 1> on = {0x01};
    auto r1 = dispatch_zcl_ep(c1, def, 0x0006, 1, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(on.data(), on.size())));
    assert(r1.any_matched);
    assert(b_true(r1.merged.find("state_main_switch")));
    assert(r1.merged.find("state") == nullptr);             // never bare
    assert(r1.merged.find("state_short_override") == nullptr);

    // EP2 genOnOff = OFF (bool false).
    RuntimeContext c2{};
    const std::array<std::uint8_t, 1> off = {0x00};
    auto r2 = dispatch_zcl_ep(c2, def, 0x0006, 2, "genOnOff",
                              attr_report(0x0000, 0x10, std::span<const std::uint8_t>(off.data(), off.size())));
    assert(r2.any_matched);
    assert(b_false(r2.merged.find("state_short_override")));
    assert(r2.merged.find("state") == nullptr);
    assert(r2.merged.find("state_main_switch") == nullptr);
}

// ── Metering: EP1 seMetering -> power_main_switch / energy_main_switch. ──────
static void test_metering() {
    using namespace zhc::devices::brun_holding;
    const auto& def = kDef_Fire_Fence;

    // InstantaneousDemand (attr 0x0400, s24 type 0x2A) = 1500 -> power.
    RuntimeContext c1{};
    const std::array<std::uint8_t, 3> p = {0xDC, 0x05, 0x00};   // 1500 LE (24-bit)
    auto pr = dispatch_zcl_ep(c1, def, 0x0702, 1, "seMetering",
                              attr_report(0x0400, 0x2A, std::span<const std::uint8_t>(p.data(), p.size())));
    assert(pr.any_matched);
    assert(pr.merged.find("power_main_switch") != nullptr);
    assert(pr.merged.find("power") == nullptr);   // per-EP, never bare

    // CurrentSummDelivered (attr 0x0000, u48 type 0x25) = 12345 -> energy.
    RuntimeContext c2{};
    const std::array<std::uint8_t, 6> e = {0x39, 0x30, 0x00, 0x00, 0x00, 0x00};  // 12345 LE
    auto er = dispatch_zcl_ep(c2, def, 0x0702, 1, "seMetering",
                              attr_report(0x0000, 0x25, std::span<const std::uint8_t>(e.data(), e.size())));
    assert(er.any_matched);
    assert(er.merged.find("energy_main_switch") != nullptr);
    assert(er.merged.find("energy") == nullptr);
}

// ── Temperature regained: EP1 msTemperatureMeasurement -> temperature_main_switch. ─
static void test_temperature() {
    using namespace zhc::devices::brun_holding;
    const auto& def = kDef_Fire_Fence;

    // 21.50 °C -> 2150 (s16, type 0x29) on measuredValue 0x0000.
    RuntimeContext c{};
    const std::array<std::uint8_t, 2> t = {0x66, 0x08};   // 2150 LE
    auto tr = dispatch_zcl_ep(c, def, 0x0402, 1, "msTemperatureMeasurement",
                              attr_report(0x0000, 0x29, std::span<const std::uint8_t>(t.data(), t.size())));
    assert(tr.any_matched);
    const Value* tv = tr.merged.find("temperature_main_switch");
    assert(tv && tv->type == ValueType::Float);
    assert(tr.merged.find("temperature") == nullptr);   // suffixed, never bare
}

// ── Battery: device-global key stays bare even with the endpoint_map. ────────
static void test_battery() {
    using namespace zhc::devices::brun_holding;
    const auto& def = kDef_Fire_Fence;

    // batteryPercentageRemaining (attr 0x0021/33, u8 type 0x20) = 200 -> 100%.
    RuntimeContext c{};
    const std::array<std::uint8_t, 1> b = {200};
    auto br = dispatch_zcl_ep(c, def, 0x0001, 1, "genPowerCfg",
                              attr_report(0x0021, 0x20, std::span<const std::uint8_t>(b.data(), b.size())));
    assert(br.any_matched);
    assert(br.merged.find("battery") != nullptr);           // global, stays bare
    assert(br.merged.find("battery_main_switch") == nullptr);
}

int main() {
    test_shape();
    test_relays();
    test_metering();
    test_temperature();
    test_battery();
    return 0;
}
