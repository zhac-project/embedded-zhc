// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity fixtures for the Salus Controls vendor.
//
// Headline regression: the RE600 Zigbee router was auto-ported with a
// phantom on/off (state) + battery/voltage bundle and bound
// genOnOff(0x0006)+genPowerCfg(0x0001). z2m declares it with
// fromZigbee:[], toZigbee:[], exposes:[] — a pure mains repeater with no
// reportable state. Graduated to a Tier-2 parent stripping all of it.
//
// Also guards the already-correct vendor devices against regression:
//   - SW600 / OS600 contact sensors: typed `contact` decode (IAS bit 0),
//     not the generic bare `alarm` key.
//   - WLS600 water sensor: typed `water_leak` decode (IAS bit 0).
//   - FC600 fan-coil thermostat: flat climate surface (local_temperature
//     + current_heating_setpoint + system_mode + fan_mode), thermostat
//     0x0201 + fan 0x0202 bound, controllable (toZigbee present).
//   - SP600 / SPE600 / SX885ZB metering plugs: z2m electricityMeter
//     {cluster:"metering"} -> energy+power off seMetering(0x0702) ONLY,
//     no phantom haElectricalMeasurement(0x0B04) current/voltage/power.
//
// z2m-source: salus_controls.ts #RE600 / #SW600 / #OS600 / #WLS600 /
// #FC600 / #SP600 / #SPE600 / #SX885ZB.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::salus_controls {
extern const PreparedDefinition kDef_RE600;    // Zigbee router (no exposes)
extern const PreparedDefinition kDef_SW600;    // contact (IAS bit 0 -> contact)
extern const PreparedDefinition kDef_OS600;    // contact (IAS bit 0 -> contact)
extern const PreparedDefinition kDef_WLS600;   // water leak (IAS bit 0)
extern const PreparedDefinition kDef_FC600;    // fan-coil thermostat
extern const PreparedDefinition kDef_SP600;    // metering plug (0x0702 only)
extern const PreparedDefinition kDef_SPE600;   // metering plug (0x0702 only)
extern const PreparedDefinition kDef_SX885ZB;  // metering plug (0x0702 only)
}  // namespace zhc::devices::salus_controls

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

// 9-byte IAS Zone Status Change Notification:
//   fc(1)=0x09  tsn(1)  cmd(1)=0x00  zoneStatus_le(2)
//   extStatus(1)  zoneId(1)  delay_le(2)
struct IasFrame { std::array<std::uint8_t, 9> bytes; };

IasFrame ias_notif(std::uint16_t zone_status, std::uint8_t tsn = 0x42) {
    IasFrame f{};
    f.bytes = { 0x09, tsn, 0x00,
                static_cast<std::uint8_t>(zone_status & 0xFF),
                static_cast<std::uint8_t>(zone_status >> 8),
                0x00, 0x00, 0x00, 0x00 };
    return f;
}

DispatchResult dispatch_ias(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

}  // namespace

// ── RE600 router: z2m exposes:[] -> nothing phantom ──────────────────
static void test_re600_no_phantom() {
    const auto& def = devices::salus_controls::kDef_RE600;
    assert(def.exposes_count == 0);       // no phantom on/off / battery
    assert(def.from_zigbee_count == 0);   // no decoders
    assert(def.to_zigbee_count == 0);     // not controllable
    assert(def.bindings_count == 0);      // no genOnOff / genPowerCfg bind
    assert(!def_exposes(def, "state"));
    assert(!def_exposes(def, "battery"));
    assert(!def_binds(def, 0x0006));
    assert(!def_binds(def, 0x0001));
}

// ── SW600 / OS600 contact sensors: typed contact decode (IAS bit 0) ──
static void test_contact_typed() {
    for (const auto* dp : { &devices::salus_controls::kDef_SW600,
                            &devices::salus_controls::kDef_OS600 }) {
        const auto& def = *dp;
        assert(def_exposes(def, "contact"));
        assert(def_binds(def, 0x0500));

        auto open = dispatch_ias(def, ias_notif(0x0001));  // bit 0 set
        assert(open.any_matched);
        assert(b_true(open.merged.find("contact")));
        assert(open.merged.find("alarm")   == nullptr);  // not the bare key
        assert(open.merged.find("alarm_1") == nullptr);

        // tamper(bit2)+battery_low(bit3) still decode via the typed converter.
        auto tb = dispatch_ias(def, ias_notif(0x000C));
        assert(tb.any_matched);
        assert(b_true(tb.merged.find("battery_low")));
    }
}

// ── WLS600 water sensor: typed water_leak decode (IAS bit 0) ─────────
static void test_wls600_water_leak() {
    const auto& def = devices::salus_controls::kDef_WLS600;
    assert(def_exposes(def, "water_leak"));

    auto leak = dispatch_ias(def, ias_notif(0x0001));
    assert(leak.any_matched);
    assert(b_true(leak.merged.find("water_leak")));
    assert(leak.merged.find("alarm") == nullptr);  // not the bare key
}

// ── FC600 fan-coil thermostat: flat climate surface, controllable ────
static void test_fc600_thermostat() {
    const auto& def = devices::salus_controls::kDef_FC600;
    // Flat thermostat exposes (no ExposeType::Climate per project rule).
    assert(def_exposes(def, "local_temperature"));
    assert(def_exposes(def, "current_heating_setpoint"));
    assert(def_exposes(def, "system_mode"));
    assert(def_exposes(def, "fan_mode"));
    // Controllable + binds thermostat(0x0201) + fan(0x0202).
    assert(def.to_zigbee_count > 0);
    assert(def_binds(def, 0x0201));
    assert(def_binds(def, 0x0202));
    // Not a phantom light/cover.
    assert(!def_exposes(def, "brightness"));
    assert(!def_exposes(def, "position"));
}

// ── Metering plugs: 0x0702 energy+power only, no phantom 0x0B04 ───────
static void test_metering_plugs() {
    for (const auto* dp : { &devices::salus_controls::kDef_SP600,
                            &devices::salus_controls::kDef_SPE600,
                            &devices::salus_controls::kDef_SX885ZB }) {
        const auto& def = *dp;
        assert(def_exposes(def, "state"));    // on/off control
        assert(def_exposes(def, "energy"));   // seMetering 0x0702
        assert(def_exposes(def, "power"));
        assert(def_binds(def, 0x0006));       // genOnOff
        assert(def_binds(def, 0x0702));       // seMetering
        // z2m {cluster:"metering"} -> NO haElectricalMeasurement half.
        assert(!def_binds(def, 0x0B04));
        assert(!def_exposes(def, "current"));
        assert(!def_exposes(def, "voltage"));
    }
}

int main() {
    test_re600_no_phantom();
    test_contact_typed();
    test_wls600_water_leak();
    test_fc600_thermostat();
    test_metering_plugs();
    return 0;
}
