// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Securifi (Almond / Peanut) family. Pins the gap
// the auto-generated port carried against the z2m wire shape:
//
//   * Almond Click (B01M7Y8BP9 / ZB2-BU01) is a button: z2m wires
//     `fromZigbee:[fz.almond_click]`, `exposes:[e.action(["single",
//     "double","long"])]`, NO toZigbee. The auto-port mis-ported it as
//     a controllable on/off switch (kFzOnOff + kTzOnOff + bare `state`)
//     — a dead control: the device never speaks genOnOff, it reports
//     clicks on ssIasAce `commandArm`, reusing the `armmode` byte as a
//     click-type code with a DEVICE-SPECIFIC lookup (NOT the standard
//     IAS-ACE arm-mode names): 3→single, 0→double, 2→long. Fixed with
//     the vendor `kFzAlmondClick` decoder + an `action` expose.
//
//   * Peanut Smart Plug (PP-WHT-US) — verified clean: z2m wires
//     `[fz.on_off, fz.electrical_measurement]` (0x0B04 power/voltage/
//     current; NO 0x0702 energy) → ez kFzOnOff + kFzElectricalMeasurement
//     already matches. No change.
//
// z2m-source: zigbee-herdsman-converters/src/devices/securifi.ts.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/securifi_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> securifi_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::securifi::kSecurifiRegistry,
        devices::securifi::kSecurifiRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

const PreparedDefinition* almond_click() {
    const auto* d = find_definition("ZB2-BU01", nullptr, securifi_registry());
    assert(d && "ZB2-BU01 must resolve to the Almond Click def");
    return d;
}

const PreparedDefinition* peanut_plug() {
    const auto* d = find_definition("PP-WHT-US", nullptr, securifi_registry());
    assert(d && "PP-WHT-US must resolve to the Peanut plug def");
    return d;
}

// ssIasAce commandArm body: [armmode]. The decoder only reads the
// first byte; we keep the minimal body the wire carries for a click.
DispatchResult dispatch_arm(const PreparedDefinition& def, std::uint8_t armmode) {
    std::uint8_t body[] = { armmode };
    DecodedMessage msg{};
    msg.family       = FrameFamily::Zcl;
    msg.type         = MessageType::Command;
    msg.cluster      = "ssIasAce";
    msg.direction    = Direction::ClientToServer;
    msg.command_id   = 0x00;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    msg.raw_body     = std::span<const std::uint8_t>(body, sizeof(body));
    InboundApsFrame raw{};
    raw.cluster_id   = 0x0501;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

bool action_is(const DispatchResult& r, const char* want) {
    const Value* v = r.merged.find("action");
    return v && v->type == ValueType::StringRef && v->str &&
           std::strcmp(v->str, want) == 0;
}

}  // namespace

// ── Almond Click: button, not a switch ──────────────────────────────
static void test_almond_click_is_button() {
    const auto* d = almond_click();
    // Button surfaces clicks via `action`; the phantom on/off control
    // (state expose + writeable path) must be gone.
    assert(def_exposes(*d, "action"));
    assert(!def_exposes(*d, "state"));
    assert(d->to_zigbee_count == 0);   // z2m toZigbee:[]
}

// ── Almond Click: device-specific armmode → click lookup ────────────
static void test_almond_click_lookup() {
    const auto* d = almond_click();

    auto single = dispatch_arm(*d, 3);
    assert(single.any_matched);
    assert(action_is(single, "single"));

    auto dbl = dispatch_arm(*d, 0);
    assert(dbl.any_matched);
    assert(action_is(dbl, "double"));

    auto lng = dispatch_arm(*d, 2);
    assert(lng.any_matched);
    assert(action_is(lng, "long"));

    // Unknown arm modes are dropped (z2m lookup miss → no publish), and
    // must NOT be mislabelled as a standard IAS-ACE arm-mode name.
    auto unknown = dispatch_arm(*d, 1);
    assert(unknown.merged.find("action") == nullptr);
}

// ── Peanut plug: on/off + electrical_measurement, no energy ─────────
static void test_peanut_plug_metering() {
    const auto* d = peanut_plug();
    assert(def_exposes(*d, "state"));
    assert(def_exposes(*d, "power"));
    assert(def_exposes(*d, "voltage"));
    assert(def_exposes(*d, "current"));
    // z2m uses fz.electrical_measurement (0x0B04) only — no 0x0702
    // energy channel, so no `energy` expose.
    assert(!def_exposes(*d, "energy"));
}

int main() {
    test_almond_click_is_button();
    test_almond_click_lookup();
    test_peanut_plug_metering();
    return 0;
}
