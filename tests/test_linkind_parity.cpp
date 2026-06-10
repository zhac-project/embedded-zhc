// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Linkind family. Pins the real gap the auto-ported
// defs carried against the z2m wire shape:
//
//   * ZS230002 (5-key smart bulb dimmer remote) wired the wrong color
//     converters: kFzCommandStepColorTemp (lightingColorCtrl cmd 0x4C →
//     "color_temperature_step_*", a converter z2m never uses here) and
//     kFzCommandMoveToHueAndSaturation (cmd 0x06 → phantom
//     "move_to_hue_and_saturation"). z2m's actual fz are
//     fz.command_move_color_temperature (cmd 0x4B → "color_temperature_move_*")
//     and fz.command_move_to_color (cmd 0x07 → "color_move"). Both generic
//     converters already exist; the def now wires them so the discrete
//     color-temp move + color_move actions decode and the phantom is gone.
//
// Also pins the already-correct surfaces as regression guards: the IAS
// sensors emit the right typed keys (bit 0), and the senders expose
// `action` with NO settable `state`.
//
// z2m-source: zigbee-herdsman-converters/src/devices/linkind.ts
//             + converters/fromZigbee.ts command_* / ias_*_alarm_1.

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

namespace zhc::devices::linkind {
extern const PreparedDefinition kDef_ZS230002;             // 5-key remote
extern const PreparedDefinition kDef_ZS232000178;          // 1-key remote
extern const PreparedDefinition kDef_ZS1100400_IN_V1A02;   // PIR motion
extern const PreparedDefinition kDef_ZS110050078;          // door/window
extern const PreparedDefinition kDef_LS21001;              // water leak
}  // namespace zhc::devices::linkind

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

DispatchResult dispatch_zcl(const PreparedDefinition& def,
                            std::uint16_t cluster_id, std::uint8_t src_ep,
                            const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster_id, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

// Cluster-specific command frame: fc=0x01 (C->S, cluster-specific), tsn, cmd,
// then the command payload.
std::vector<std::uint8_t> cmd_frame(std::uint8_t cmd,
                                    std::span<const std::uint8_t> payload = {}) {
    std::vector<std::uint8_t> v{0x01, 0x42, cmd};
    for (auto b : payload) v.push_back(b);
    return v;
}

// ssIasZone commandStatusChangeNotification (cmd 0x00): fc=0x09, tsn, cmd,
// zoneStatus u16le + extStatus u8 + zoneId u8 + delay u16.
std::vector<std::uint8_t> ias_status(std::uint16_t zone_status) {
    return {0x09, 0x42, 0x00,
            static_cast<std::uint8_t>(zone_status & 0xFF),
            static_cast<std::uint8_t>(zone_status >> 8),
            0x00, 0x00, 0x00, 0x00};
}

bool action_is(const DispatchResult& r, const char* expected) {
    const Value* v = r.merged.find("action");
    if (!v || v->type != ValueType::StringRef || !v->str) return false;
    return std::strcmp(v->str, expected) == 0;
}

bool bool_is(const DispatchResult& r, const char* key, bool expected) {
    const Value* v = r.merged.find(key);
    if (!v || v->type != ValueType::Bool) return false;
    return v->b == expected;
}

bool def_lists_action(const PreparedDefinition& def, const char* action) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const Expose& e = def.exposes[i];
        if (!e.name || std::strcmp(e.name, "action") != 0) continue;
        for (std::size_t j = 0; j < e.enum_count; ++j)
            if (e.enum_values[j] && std::strcmp(e.enum_values[j], action) == 0) return true;
    }
    return false;
}

}  // namespace

// ── ZS230002: 5-key remote — the corrected color converters. ─────────
static void test_zs230002_color() {
    using devices::linkind::kDef_ZS230002;

    // Sender: action expose, no settable state, no toolbox.
    assert(def_exposes(kDef_ZS230002, "action"));
    assert(!def_exposes(kDef_ZS230002, "state"));
    assert(kDef_ZS230002.to_zigbee_count == 0);

    // genOnOff On (cmd 0x01) → "on"; Off (cmd 0x00) → "off".
    assert(action_is(dispatch_zcl(kDef_ZS230002, 0x0006, 1, "genOnOff", cmd_frame(0x01)), "on"));
    assert(action_is(dispatch_zcl(kDef_ZS230002, 0x0006, 1, "genOnOff", cmd_frame(0x00)), "off"));

    // genLevelCtrl Move up (cmd 0x01, move_mode=0, rate=50) → "brightness_move_up".
    {
        const std::uint8_t p[] = {0x00, 0x32};
        auto r = dispatch_zcl(kDef_ZS230002, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x01, std::span<const std::uint8_t>(p, sizeof(p))));
        assert(action_is(r, "brightness_move_up"));
    }

    // lightingColorCtrl MoveToColorTemp (cmd 0x0A) → "color_temperature_move".
    {
        const std::uint8_t p[] = {0x9A, 0x01, 0x00, 0x00};  // colortemp + transtime
        auto r = dispatch_zcl(kDef_ZS230002, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x0A, std::span<const std::uint8_t>(p, sizeof(p))));
        assert(action_is(r, "color_temperature_move"));
    }

    // FIX: lightingColorCtrl MoveColorTemp (cmd 0x4B) move_mode=1 (up) →
    // "color_temperature_move_up". Was mis-wired to kFzCommandStepColorTemp.
    {
        const std::uint8_t up[] = {0x01, 0x0A, 0x00, 0x00, 0x00, 0xFF, 0xFF};  // mode, rate, min, max
        auto r = dispatch_zcl(kDef_ZS230002, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x4B, std::span<const std::uint8_t>(up, sizeof(up))));
        assert(action_is(r, "color_temperature_move_up"));
        // Regression: the phantom step action must never appear.
        assert(!action_is(r, "color_temperature_step_up"));
    }
    {
        const std::uint8_t down[] = {0x03, 0x0A, 0x00, 0x00, 0x00, 0xFF, 0xFF};
        auto r = dispatch_zcl(kDef_ZS230002, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x4B, std::span<const std::uint8_t>(down, sizeof(down))));
        assert(action_is(r, "color_temperature_move_down"));
    }
    {
        const std::uint8_t stop[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF};
        auto r = dispatch_zcl(kDef_ZS230002, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x4B, std::span<const std::uint8_t>(stop, sizeof(stop))));
        assert(action_is(r, "color_temperature_move_stop"));
    }

    // FIX: lightingColorCtrl MoveToColor (cmd 0x07) → "color_move".
    // Was mis-wired to kFzCommandMoveToHueAndSaturation (cmd 0x06 → phantom).
    {
        const std::uint8_t p[] = {0x00, 0x80, 0x00, 0x40, 0x00, 0x00};  // colorx, colory, transtime
        auto r = dispatch_zcl(kDef_ZS230002, 0x0300, 1, "lightingColorCtrl",
                              cmd_frame(0x07, std::span<const std::uint8_t>(p, sizeof(p))));
        assert(action_is(r, "color_move"));
    }

    // The phantom action must be gone from the expose enum; the real ones present.
    assert(!def_lists_action(kDef_ZS230002, "move_to_hue_and_saturation"));
    assert(def_lists_action(kDef_ZS230002, "color_move"));
    assert(def_lists_action(kDef_ZS230002, "color_temperature_move_up"));
    assert(def_lists_action(kDef_ZS230002, "color_temperature_move_stop"));
}

// ── ZS232000178: 1-key remote — sender invariants + level move/stop. ─
static void test_zs232000178() {
    using devices::linkind::kDef_ZS232000178;
    assert(def_exposes(kDef_ZS232000178, "action"));
    assert(!def_exposes(kDef_ZS232000178, "state"));
    assert(kDef_ZS232000178.to_zigbee_count == 0);

    assert(action_is(dispatch_zcl(kDef_ZS232000178, 0x0006, 1, "genOnOff", cmd_frame(0x01)), "on"));
    {
        const std::uint8_t p[] = {0x01, 0x32};  // move_mode=down
        auto r = dispatch_zcl(kDef_ZS232000178, 0x0008, 1, "genLevelCtrl",
                              cmd_frame(0x01, std::span<const std::uint8_t>(p, sizeof(p))));
        assert(action_is(r, "brightness_move_down"));
    }
    assert(action_is(dispatch_zcl(kDef_ZS232000178, 0x0008, 1, "genLevelCtrl", cmd_frame(0x03)),
                     "brightness_stop"));
}

// ── IAS sensors: typed keys on bit 0, no bare alarm. ─────────────────
static void test_ias_sensors() {
    using namespace devices::linkind;

    // Motion: occupancy on bit 0, tamper bit 2, battery_low bit 3.
    {
        auto r = dispatch_zcl(kDef_ZS1100400_IN_V1A02, 0x0500, 1, "ssIasZone", ias_status(0x0001));
        assert(bool_is(r, "occupancy", true));
        assert(r.merged.find("alarm") == nullptr);
        auto t = dispatch_zcl(kDef_ZS1100400_IN_V1A02, 0x0500, 1, "ssIasZone", ias_status(0x0004));
        assert(bool_is(t, "tamper", true));
        assert(bool_is(t, "occupancy", false));
    }
    // Contact: contact on bit 0.
    {
        auto r = dispatch_zcl(kDef_ZS110050078, 0x0500, 1, "ssIasZone", ias_status(0x0001));
        assert(bool_is(r, "contact", true));
        assert(r.merged.find("alarm") == nullptr);
    }
    // Water leak: water_leak on bit 0.
    {
        auto r = dispatch_zcl(kDef_LS21001, 0x0500, 1, "ssIasZone", ias_status(0x0001));
        assert(bool_is(r, "water_leak", true));
        assert(r.merged.find("alarm") == nullptr);
    }
}

int main() {
    test_zs230002_color();
    test_zs232000178();
    test_ias_sensors();
    return 0;
}
