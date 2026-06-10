// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the JetHome WS7 3-channel battery discrete-input module.
// Pins the real gaps the auto-generated port carried against z2m:
//
//   * The port wired a phantom genOnOff in/out (kFzOnOff + kTzOnOff +
//     a settable `state` Binary expose + 0x0006 binding) — the device
//     has NO on/off. z2m: fromZigbee=[fz.battery, jetHome.fz.
//     multiStateAction], toZigbee=[]. Replaced with a non-controllable
//     `action` enum + battery.
//   * The entire discrete-input channel was dropped: z2m decodes
//     genMultistateInput.presentValue (attr 0x0055) into release/single/
//     double/triple/hold. Restored with kFzJetHomeMultiStateAction.
//   * The fingerprint dropped the `JetHome` manufacturerName gate.
//   * Per-channel routing: z2m endpoint={in1:1,in2:2,in3:3} +
//     postfixWithEndpointName. Mirrored via endpoint_map + endpoint_action_
//     suffix so action surfaces as action_in1 / action_in2 / action_in3.
//
// z2m-source: zigbee-herdsman-converters/src/devices/jethome.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::jethome {
extern const PreparedDefinition kDef_WS7;
}  // namespace zhc::devices::jethome

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

// Report Attributes (cmd 0x0A) for genMultistateInput presentValue
// (attr 0x0055), data type uint16 (0x21) → 2 value bytes LE.
struct MsFrame { std::array<std::uint8_t, 8> bytes; };

MsFrame ms_present(std::uint16_t value, std::uint8_t tsn = 0x42) {
    MsFrame f{};
    f.bytes = { 0x18, tsn, 0x0A,            // fc, tsn, cmd=Report Attributes
                0x55, 0x00,                 // attr 0x0055
                0x21,                       // type uint16
                static_cast<std::uint8_t>(value & 0xFF),
                static_cast<std::uint8_t>(value >> 8) };
    return f;
}

DispatchResult dispatch_ms(const PreparedDefinition& def, std::uint8_t src_ep,
                           const MsFrame& f) {
    auto raw = make_frame(0x0012, src_ep,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genMultistateInput";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool action_is(const Value* v, const char* expect) {
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, expect) == 0;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

}  // namespace

// ── shape: action enum, battery, voltage; NO phantom on/off ──────────
static void test_shape() {
    const auto& def = devices::jethome::kDef_WS7;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(!def_exposes(def, "state"));   // phantom on/off gone

    // No to_zigbee — the device is uncontrollable.
    assert(def.to_zigbee_count == 0);

    // Manufacturer gate restored.
    assert(def.manufacturer_names_count == 1);
    assert(std::strcmp(def.manufacturer_names[0], "JetHome") == 0);

    // No genOnOff binding; genPowerCfg (battery) present.
    bool binds_onoff = false, binds_power = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].cluster_id == 0x0006) binds_onoff = true;
        if (def.bindings[i].cluster_id == 0x0001) binds_power = true;
    }
    assert(!binds_onoff);
    assert(binds_power);

    // Per-channel suffixing opted in.
    assert(def.endpoint_action_suffix);
    assert(def.endpoint_map_count == 3);
}

// ── discrete-input action per channel (endpoint → action_in<n>) ──────
static void test_action_channels() {
    const auto& def = devices::jethome::kDef_WS7;

    // EP1 single -> action_in1 = "single".
    auto r1 = dispatch_ms(def, 1, ms_present(1));
    assert(r1.any_matched);
    assert(action_is(r1.merged.find("action_in1"), "single"));
    assert(r1.merged.find("action") == nullptr);  // bare key must be suffixed

    // EP2 double -> action_in2 = "double".
    auto r2 = dispatch_ms(def, 2, ms_present(2));
    assert(r2.any_matched);
    assert(action_is(r2.merged.find("action_in2"), "double"));

    // EP3 hold -> action_in3 = "hold".
    auto r3 = dispatch_ms(def, 3, ms_present(4));
    assert(r3.any_matched);
    assert(action_is(r3.merged.find("action_in3"), "hold"));

    // release + triple gestures decode too.
    auto r4 = dispatch_ms(def, 1, ms_present(0));
    assert(action_is(r4.merged.find("action_in1"), "release"));
    auto r5 = dispatch_ms(def, 1, ms_present(3));
    assert(action_is(r5.merged.find("action_in1"), "triple"));
}

// ── high-byte channel offsets fold to the same gesture (z2m lookup) ──
static void test_high_byte_fold() {
    const auto& def = devices::jethome::kDef_WS7;
    // z2m maps 257 (=256+1) and 513 and 1025 all to "single".
    for (std::uint16_t base : {std::uint16_t(256), std::uint16_t(512),
                               std::uint16_t(1024)}) {
        const std::uint16_t value = static_cast<std::uint16_t>(base + 1);
        auto r = dispatch_ms(def, 1, ms_present(value));
        assert(action_is(r.merged.find("action_in1"), "single"));
    }
}

// ── battery still decodes (genPowerCfg) ──────────────────────────────
static void test_battery() {
    const auto& def = devices::jethome::kDef_WS7;
    // Report Attributes: attr 0x0021 (batteryPercentageRemaining=33), u8,
    // value 0xC8 (200 half-percent = 100 %).
    const std::uint8_t kBat[] = {
        0x18, 0x42, 0x0A,
        0x21, 0x00,        // attr 0x0021
        0x20,              // type u8
        0xC8,              // 200 -> 100 %
    };
    auto raw = make_frame(0x0001, 1, kBat);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genPowerCfg";
    RuntimeContext ctx{};
    auto r = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(r.any_matched);
    const Value* b = r.merged.find("battery");
    assert(b && b->type == ValueType::Uint && b->u == 100);
}

int main() {
    test_shape();
    test_action_channels();
    test_high_byte_fold();
    test_battery();
    return 0;
}
