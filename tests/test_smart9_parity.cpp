// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Smart9 family. Pins the gap the auto-generated
// port carried against the z2m wire shape:
//
//   * S9ZGBRC01 (TS0215) is a "Smart remote controller" — a battery key
//     fob / panic remote. z2m wires `fromZigbee:[fz.command_arm,
//     fz.command_emergency, fz.battery]`, `exposes:[e.battery(),
//     e.action(["disarm","arm_day_zones","arm_night_zones",
//     "arm_all_zones","exit_delay","emergency"])]`, NO toZigbee, and
//     binds genPowerCfg only. The auto-port mis-ported it as a
//     controllable on/off switch (kFzOnOff + kTzOnOff + a phantom
//     `state` Binary StateSet + a spurious genOnOff 0x0006 bind) — a
//     dead control: the fob never sinks genOnOff, it SENDS ssIasAce
//     arm/emergency commands. Fixed by wiring the generic
//     kFzIasAceArm (commandArm 0x00 → action=<armMode>) +
//     kFzIasAceEmergency (commandEmergency 0x02 → action="emergency")
//     + kFzBattery, and replacing `state` with an `action` expose.
//
//     z2m uses fz.command_emergency (commandEmergency, cmd 0x02), NOT
//     fz.command_panic (cmd 0x04) — so kFzIasAceEmergency, not
//     kFzIasAcePanic.
//
// z2m-source: zigbee-herdsman-converters/src/devices/smart9.ts.

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/smart9_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> smart9_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::smart9::kSmart9Registry,
        devices::smart9::kSmart9RegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

const PreparedDefinition* s9_remote() {
    const auto* d = find_definition("TS0215", nullptr, smart9_registry());
    assert(d && "TS0215 must resolve to the Smart9 S9ZGBRC01 def");
    return d;
}

// ssIasAce commandArm body: [arm_mode, code_len, code..., zone_id].
// With an empty PIN code: [arm_mode, 0x00, zone_id].
DispatchResult dispatch_arm(const PreparedDefinition& def, std::uint8_t arm_mode,
                            std::uint8_t zone_id = 0) {
    std::uint8_t body[] = { arm_mode, 0x00, zone_id };
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

// ssIasAce commandEmergency (cmd 0x02): no body.
DispatchResult dispatch_emergency(const PreparedDefinition& def) {
    DecodedMessage msg{};
    msg.family       = FrameFamily::Zcl;
    msg.type         = MessageType::Command;
    msg.cluster      = "ssIasAce";
    msg.direction    = Direction::ClientToServer;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    msg.raw_body     = std::span<const std::uint8_t>();
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

// ── S9ZGBRC01: key fob, not a switch ────────────────────────────────
static void test_s9_is_action_remote() {
    const auto* d = s9_remote();
    // Action surface present; the phantom on/off control must be gone.
    assert(def_exposes(*d, "action"));
    assert(def_exposes(*d, "battery"));
    assert(!def_exposes(*d, "state"));
    assert(d->to_zigbee_count == 0);   // z2m toZigbee:[]
}

// ── arm command: armMode → action name + action_zone ────────────────
static void test_s9_arm_actions() {
    const auto* d = s9_remote();

    auto disarm = dispatch_arm(*d, 0);
    assert(disarm.any_matched);
    assert(action_is(disarm, "disarm"));

    auto day = dispatch_arm(*d, 1);
    assert(action_is(day, "arm_day_zones"));

    auto night = dispatch_arm(*d, 2);
    assert(action_is(night, "arm_night_zones"));

    auto all = dispatch_arm(*d, 3, /*zone_id=*/7);
    assert(action_is(all, "arm_all_zones"));
    const Value* z = all.merged.find("action_zone");
    assert(z && z->type == ValueType::Uint && z->u == 7);

    auto exit = dispatch_arm(*d, 4);
    assert(action_is(exit, "exit_delay"));
}

// ── emergency command (cmd 0x02) → action="emergency" ───────────────
static void test_s9_emergency() {
    const auto* d = s9_remote();
    auto em = dispatch_emergency(*d);
    assert(em.any_matched);
    assert(action_is(em, "emergency"));
    // Must be the emergency converter, not panic.
    assert(!action_is(em, "panic"));
}

int main() {
    test_s9_is_action_remote();
    test_s9_arm_actions();
    test_s9_emergency();
    return 0;
}
