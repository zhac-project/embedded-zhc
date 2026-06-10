// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Senoro.Win window alarm (TS0601 / _TZE200_ytx9fudw) —
// a Tuya-MCU (0xEF00 manuSpecificTuya) DP-stream device.
//
// Bugs fixed:
//   * The winning auto-port dropped DP 101 `opening_state` (the device's
//     primary channel) and carried two phantom exposes (`state`, `action`)
//     that no DP drives. DP 16 `alarm` was typed Numeric but z2m is e.binary
//     -> Bool. Restored to the z2m surface: opening_state / alarm / battery.
//   * A sibling auto-stub with the SAME fingerprint was misrouted to the
//     generic IAS-zone (ssIasZone 0x0500) emitting bare alarm/tamper/
//     battery_low. z2m has no IAS path here — removed that dup.
//
// z2m-source: senoro.ts #Senoro.Win — tuyaDatapoints
//   [101, opening_state, lookup{open:0,closed:1,tilted:2}]
//   [16,  alarm,         raw]   (e.binary STATE_SET)
//   [2,   battery,       raw]

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/senoro_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> senoro_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::senoro::kSenoroRegistry, devices::senoro::kSenoroRegistryCount);
}

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) {
            return true;
        }
    }
    return false;
}

DecodedMessage make_msg() {
    DecodedMessage msg{};
    msg.family       = FrameFamily::TuyaDp;
    msg.type         = MessageType::Command;
    msg.cluster      = "manuSpecificTuya";
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = 0x02;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

InboundApsFrame make_raw() {
    InboundApsFrame raw{};
    raw.cluster_id   = 0xEF00;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    return raw;
}

DispatchResult dispatch_dp(const PreparedDefinition& def,
                           std::span<const TuyaDpRecord> recs) {
    auto msg = make_msg();
    auto raw = make_raw();
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, recs, def, raw, ctx);
}

const PreparedDefinition* win() {
    const auto* d = find_definition("TS0601", "_TZE200_ytx9fudw", senoro_registry());
    assert(d && "_TZE200_ytx9fudw must resolve to the Senoro.Win def");
    return d;
}

}  // namespace

// The single registry entry is the Tuya-DP def; the misrouted IAS dup is gone.
static void test_registry_single_dp_def() {
    assert(devices::senoro::kSenoroRegistryCount == 1);
    const auto* d = win();
    assert(d->model && std::strcmp(d->model, "Senoro.Win") == 0);
    assert(d->vendor && std::strcmp(d->vendor, "Senoro") == 0);
    assert(d->from_zigbee && d->from_zigbee_count >= 2);  // sync-time + DP map
    assert(d->to_zigbee && d->to_zigbee_count >= 1);
}

// Binds 0xEF00 (Tuya MCU), NOT the phantom ssIasZone 0x0500 the IAS stub had.
static void test_binding_is_tuya_not_ias() {
    const auto* d = win();
    bool has_ef00 = false, has_0500 = false;
    for (std::size_t i = 0; i < d->bindings_count; ++i) {
        if (d->bindings[i].cluster_id == 0xEF00) has_ef00 = true;
        if (d->bindings[i].cluster_id == 0x0500) has_0500 = true;
    }
    assert(has_ef00 && !has_0500);
}

// Exposes match the z2m surface; phantom state/action and the IAS keys are gone.
static void test_exposes_match_z2m() {
    const auto* d = win();
    assert(def_exposes(*d, "opening_state"));  // dropped by the auto port
    assert(def_exposes(*d, "alarm"));
    assert(def_exposes(*d, "battery"));
    // phantom auto-port keys must be gone
    assert(!def_exposes(*d, "state"));
    assert(!def_exposes(*d, "action"));
    // IAS-stub keys must be gone
    assert(!def_exposes(*d, "tamper"));
    assert(!def_exposes(*d, "battery_low"));
    // opening_state is an enum with the three z2m labels.
    for (std::size_t i = 0; i < d->exposes_count; ++i) {
        if (std::strcmp(d->exposes[i].name, "opening_state") == 0) {
            assert(d->exposes[i].type == ExposeType::Enum);
            assert(d->exposes[i].enum_count == 3);
        }
        if (std::strcmp(d->exposes[i].name, "alarm") == 0) {
            assert(d->exposes[i].type == ExposeType::Binary);
            assert(d->exposes[i].access == Access::StateSet);
        }
    }
}

// DP 101 enum: 0->open, 1->closed, 2->tilted (z2m valueConverterBasic.lookup).
static void test_opening_state_enum() {
    const auto* d = win();
    const std::uint8_t kOpen[]   = { 0x00 };
    const std::uint8_t kClosed[] = { 0x01 };
    const std::uint8_t kTilted[] = { 0x02 };

    const TuyaDpRecord ro[] = { { 101, 0x04, std::span<const std::uint8_t>(kOpen, 1) } };
    auto r = dispatch_dp(*d, std::span<const TuyaDpRecord>(ro, 1));
    assert(r.any_matched);
    const Value* os = r.merged.find("opening_state");
    assert(os && os->type == ValueType::StringRef && std::strcmp(os->str, "open") == 0);

    const TuyaDpRecord rc[] = { { 101, 0x04, std::span<const std::uint8_t>(kClosed, 1) } };
    auto rcl = dispatch_dp(*d, std::span<const TuyaDpRecord>(rc, 1));
    const Value* osc = rcl.merged.find("opening_state");
    assert(osc && std::strcmp(osc->str, "closed") == 0);

    const TuyaDpRecord rt[] = { { 101, 0x04, std::span<const std::uint8_t>(kTilted, 1) } };
    auto rti = dispatch_dp(*d, std::span<const TuyaDpRecord>(rt, 1));
    const Value* ost = rti.merged.find("opening_state");
    assert(ost && std::strcmp(ost->str, "tilted") == 0);
}

// DP 16 alarm decodes as Bool (z2m e.binary), DP 2 battery raw pass-through Int.
static void test_alarm_bool_and_battery() {
    const auto* d = win();
    const std::uint8_t kAlarm[] = { 0x01 };               // alarm true
    const std::uint8_t kBat[]   = { 0x00, 0x00, 0x00, 0x5A };  // battery 90 %
    const TuyaDpRecord recs[] = {
        { 16, 0x01, std::span<const std::uint8_t>(kAlarm, 1) },
        { 2,  0x02, std::span<const std::uint8_t>(kBat, 4) },
    };
    auto r = dispatch_dp(*d, std::span<const TuyaDpRecord>(recs, 2));
    assert(r.any_matched);
    const Value* al = r.merged.find("alarm");
    assert(al && al->type == ValueType::Bool && al->b == true);
    const Value* bat = r.merged.find("battery");
    assert(bat && bat->type == ValueType::Int && bat->i == 90);
}

int main() {
    test_registry_single_dp_def();
    test_binding_is_tuya_not_ias();
    test_exposes_match_z2m();
    test_opening_state_enum();
    test_alarm_bool_and_battery();
    return 0;
}
