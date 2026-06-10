// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Zunzunbee SSWZ8T "Slate switch" (8-button battery
// scene controller). Pins the real gap the auto-generated port carried
// against the z2m wire shape:
//
//   * The auto-port collapsed the device into a settable on/off `state`
//     (kFzOnOff + kTzOnOff), a dead control: z2m has NO controllable
//     on/off — the device only reports button presses, packing them into
//     the ssIasZone zoneStatus word. fzLocal.fzZunzunbeeSlateSwitchIAS:
//       bit 0     → press type (0 = short_press, 1 = long_press)
//       bits 1..8 → button number one-hot (2→1 … 256→8)
//     surfaced as a single composite `action = button_<n>_<short|long>
//     _press`. Fixed with the vendor kFzZunzunbeeSlateSwitch + an `action`
//     enum expose, dropping `state`/to_zigbee. Battery + temperature
//     channels (m.battery() + m.temperature()) are also wired.
//
// z2m-source: zigbee-herdsman-converters/src/devices/zunzunbee.ts.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::zunzunbee {
extern const PreparedDefinition kDef_SSWZ8T;
}  // namespace zhc::devices::zunzunbee

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

const char* action_of(const DispatchResult& r) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

// zoneStatus for button n (1..8), press type short/long.
std::uint16_t zone_for(int button, bool long_press) {
    const std::uint16_t one_hot = static_cast<std::uint16_t>(1u << button);  // n=1 → 0x0002 …
    return static_cast<std::uint16_t>(one_hot | (long_press ? 0x0001 : 0x0000));
}

}  // namespace

// ── exposes shape: action enum present, dead on/off gone ─────────────
static void test_exposes() {
    const auto& def = devices::zunzunbee::kDef_SSWZ8T;
    assert(def_exposes(def, "action"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "temperature"));
    assert(!def_exposes(def, "state"));     // dead settable on/off must be gone

    // No to_zigbee — device is uncontrollable.
    assert(def.to_zigbee_count == 0);

    // manufacturer-gated like the z2m fingerprint.
    assert(def.manufacturer_names_count == 1);
    assert(std::strcmp(def.manufacturer_names[0], "zunzunbee") == 0);

    // genOnOff (0x0006) bind must be gone; battery + temperature bound.
    bool binds_onoff = false, binds_batt = false, binds_temp = false;
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].cluster_id == 0x0006) binds_onoff = true;
        if (def.bindings[i].cluster_id == 0x0001) binds_batt  = true;
        if (def.bindings[i].cluster_id == 0x0402) binds_temp  = true;
    }
    assert(!binds_onoff);
    assert(binds_batt);
    assert(binds_temp);
}

// ── all 8 buttons, both press types, decode to the right action ──────
static void test_all_buttons() {
    const auto& def = devices::zunzunbee::kDef_SSWZ8T;
    const char* expect_short[8] = {
        "button_1_short_press", "button_2_short_press", "button_3_short_press",
        "button_4_short_press", "button_5_short_press", "button_6_short_press",
        "button_7_short_press", "button_8_short_press",
    };
    const char* expect_long[8] = {
        "button_1_long_press", "button_2_long_press", "button_3_long_press",
        "button_4_long_press", "button_5_long_press", "button_6_long_press",
        "button_7_long_press", "button_8_long_press",
    };
    for (int n = 1; n <= 8; ++n) {
        auto sp = dispatch_ias(def, ias_notif(zone_for(n, false)));
        assert(sp.any_matched);
        const char* a_s = action_of(sp);
        assert(a_s && std::strcmp(a_s, expect_short[n - 1]) == 0);

        auto lp = dispatch_ias(def, ias_notif(zone_for(n, true)));
        assert(lp.any_matched);
        const char* a_l = action_of(lp);
        assert(a_l && std::strcmp(a_l, expect_long[n - 1]) == 0);
    }
}

// ── regression guard: never emit the generic IAS alarm bitfield ──────
static void test_no_generic_alarm() {
    const auto& def = devices::zunzunbee::kDef_SSWZ8T;
    // Button 1 short press = zoneStatus 0x0002. The generic
    // kFzIasZoneStatusChange would read this as alarm_2=true; ours must
    // NOT, and must emit only `action`.
    auto r = dispatch_ias(def, ias_notif(0x0002));
    assert(r.any_matched);
    assert(r.merged.find("alarm_1") == nullptr);
    assert(r.merged.find("alarm_2") == nullptr);
    assert(r.merged.find("tamper")  == nullptr);
    const char* a = action_of(r);
    assert(a && std::strcmp(a, "button_1_short_press") == 0);

    // An all-zero / unmapped mask produces no action (no spurious key).
    auto z = dispatch_ias(def, ias_notif(0x0000));
    assert(z.merged.find("action") == nullptr);
}

int main() {
    test_exposes();
    test_all_buttons();
    test_no_generic_alarm();
    return 0;
}
