// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Visonic (PowerMax/Tyco) IAS-Zone sensor family.
// z2m-source: visonic.ts — the per-zone-type `fz.ias_<type>_alarm_1`
// decoders (occupancy / contact / vibration) plus `fz.temperature`.
//
// Bugs fixed: every generated Visonic def lowered the generic kFzIasZone
// converter — which emits the bare key "alarm" — while the expose declared
// the *semantic* key (occupancy / contact / vibration). With no rename
// layer the primary sensor state never reached the shadow. Each def was
// graduated to a Tier-2 parent override that swaps in the typed
// kFzIas<Type>Alarm converter (emitting the semantic key directly).
//
// Additionally, the five temperature-capable models (MP-840, MCT-350 SMA,
// MCT-340 E, MCT-340 SMA, MCT-302 SMA) carry `fz.temperature` /
// `m.temperature()` in z2m but the port dropped both the decoder and the
// `temperature` expose. The kFzTemperature converter (msTemperatureMeasurement
// 0x0402, /100) + the expose + the 0x0402 binding were restored.
//
// These tests pin, on real wire shapes, that each representative device
// decodes its semantic key (and NOT the bare "alarm" key), and that the
// temperature path decodes from msTemperatureMeasurement.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::visonic {
extern const PreparedDefinition kDef_MP_840;      // occupancy + temperature + battery
extern const PreparedDefinition kDef_MP_841;      // occupancy
extern const PreparedDefinition kDef_MCT_370_SMA; // contact
extern const PreparedDefinition kDef_MCT_350_SMA; // contact + temperature
extern const PreparedDefinition kDef_MCT_340_E;   // contact + temperature + battery
extern const PreparedDefinition kDef_MCT_340_SMA; // contact + temperature + battery
extern const PreparedDefinition kDef_GB_540;      // vibration (glass break)
extern const PreparedDefinition kDef_MCT_302_SMA; // contact + temperature + battery
}  // namespace zhc::devices::visonic

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
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

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

// Assert: alarm_1 (bit 0) asserted -> semantic key true, "alarm" absent,
// tamper/battery_low reflect bits 2/3. The semantic key must also be a
// declared expose (regression guard against generic kFzIasZone, whose
// "alarm" key never matched the expose).
void check_alarm1(const PreparedDefinition& def, const char* sem) {
    assert(def_exposes(def, sem));

    auto on = dispatch_ias(def, ias_notif(0x0001));   // alarm_1 only
    assert(on.any_matched);
    assert(b_true(on.merged.find(sem)));
    assert(on.merged.find("alarm") == nullptr);       // bare key must be gone
    assert(b_false(on.merged.find("tamper")));
    assert(b_false(on.merged.find("battery_low")));

    auto off = dispatch_ias(def, ias_notif(0x0000));  // clear
    assert(off.any_matched);
    assert(b_false(off.merged.find(sem)));

    auto tb = dispatch_ias(def, ias_notif(0x000C));   // tamper(bit2)+battery_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find(sem)));
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

// Assert the restored temperature channel: kFzTemperature decodes
// msTemperatureMeasurement (0x0402) attr 0x0000 (measuredValue, s16, /100),
// the `temperature` expose is declared, and the device binds 0x0402.
void check_temperature(const PreparedDefinition& def) {
    assert(def_exposes(def, "temperature"));
    assert(def_binds(def, 0x0402));

    // msTemperatureMeasurement Report Attributes: attr 0x0000 (int16) = 2105
    // -> 21.05 °C.
    const std::uint8_t kTempReport[] = {
        0x18, 0x42, 0x0A,        // fc, tsn, cmd=Report Attributes
        0x00, 0x00,              // attr 0x0000 (measuredValue)
        0x29,                    // type int16
        0x39, 0x08,              // 0x0839 = 2105 (LE)
    };
    auto raw = make_frame(0x0402, 1, kTempReport);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* t = r.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(t->f > 21.0f && t->f < 21.1f);
}

}  // namespace

// ── per-type alarm_1 (bit 0) decoders ────────────────────────────────
static void test_mp840_occupancy()  { check_alarm1(devices::visonic::kDef_MP_840,      "occupancy"); }
static void test_mp841_occupancy()  { check_alarm1(devices::visonic::kDef_MP_841,      "occupancy"); }
static void test_mct370_contact()   { check_alarm1(devices::visonic::kDef_MCT_370_SMA, "contact"); }
static void test_mct350_contact()   { check_alarm1(devices::visonic::kDef_MCT_350_SMA, "contact"); }
static void test_mct340e_contact()  { check_alarm1(devices::visonic::kDef_MCT_340_E,   "contact"); }
static void test_mct340sma_contact(){ check_alarm1(devices::visonic::kDef_MCT_340_SMA, "contact"); }
static void test_gb540_vibration()  { check_alarm1(devices::visonic::kDef_GB_540,      "vibration"); }
static void test_mct302_contact()   { check_alarm1(devices::visonic::kDef_MCT_302_SMA, "contact"); }

// ── restored temperature channels ────────────────────────────────────
static void test_mp840_temp()    { check_temperature(devices::visonic::kDef_MP_840); }
static void test_mct350_temp()   { check_temperature(devices::visonic::kDef_MCT_350_SMA); }
static void test_mct340e_temp()  { check_temperature(devices::visonic::kDef_MCT_340_E); }
static void test_mct340sma_temp(){ check_temperature(devices::visonic::kDef_MCT_340_SMA); }
static void test_mct302_temp()   { check_temperature(devices::visonic::kDef_MCT_302_SMA); }

int main() {
    test_mp840_occupancy();
    test_mp841_occupancy();
    test_mct370_contact();
    test_mct350_contact();
    test_mct340e_contact();
    test_mct340sma_contact();
    test_gb540_vibration();
    test_mct302_contact();

    test_mp840_temp();
    test_mct350_temp();
    test_mct340e_temp();
    test_mct340sma_temp();
    test_mct302_temp();
    return 0;
}
