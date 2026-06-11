// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the Spacetronik ZB-DG02 gas leakage sensor — a TS0601 /
// _TZE204_uc0iv1hb Tuya-MCU (0xEF00 manuSpecificTuya) DP-stream device.
//
// Bug fixed: the device was auto-ported as a generic IAS-zone (ssIasZone
// 0x0500) emitting bare alarm/tamper/battery_low. The device speaks NONE of
// that — z2m decodes it via tuya.modernExtend.tuyaBase({dp:true}) + a single
// datapoint [1, "gas", tuya.valueConverter.trueFalse0] on the 0xEF00 DP stream.
// The IAS-vs-DP misroute left the only real channel (gas) dead. Graduated to a
// Tier-2 override wiring fz_tuya_datapoints with the DP map.
//
// trueFalse0 reports gas==true when the wire DP value is 0 (dp::binary_inv).
//
// z2m-source: spacetronik.ts #ZB-DG02 (tuya.valueConverter.trueFalse0).

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/spacetronik_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> spacetronik_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::spacetronik::kSpacetronikRegistry,
        devices::spacetronik::kSpacetronikRegistryCount);
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

const PreparedDefinition* dg02() {
    const auto* d = find_definition("TS0601", "_TZE204_uc0iv1hb",
                                    spacetronik_registry());
    assert(d && "_TZE204_uc0iv1hb must resolve to the Spacetronik ZB-DG02 def");
    return d;
}

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

}  // namespace

// The fingerprint resolves to a full Tuya-DP def (sync-time + DP map), NOT a
// generic IAS-zone stub. Exposes the semantic `gas` key; the phantom IAS
// surface (alarm/tamper/battery_low) is gone.
static void test_resolves_to_dp_def() {
    const auto* d = dg02();
    assert(d->model && std::strcmp(d->model, "ZB-DG02") == 0);
    assert(d->vendor && std::strcmp(d->vendor, "Spacetronik") == 0);
    assert(d->from_zigbee && d->from_zigbee_count >= 2);  // sync-time + DP map

    assert(def_exposes(*d, "gas"));
    assert(!def_exposes(*d, "alarm"));
    assert(!def_exposes(*d, "tamper"));
    assert(!def_exposes(*d, "battery_low"));
}

// Binds 0xEF00 (the Tuya MCU cluster), NOT the phantom ssIasZone 0x0500 the
// auto stub wired.
static void test_binds_tuya_not_ias() {
    const auto* d = dg02();
    bool has_ef00 = false, has_0500 = false;
    for (std::size_t i = 0; i < d->bindings_count; ++i) {
        if (d->bindings[i].cluster_id == 0xEF00) has_ef00 = true;
        if (d->bindings[i].cluster_id == 0x0500) has_0500 = true;
    }
    assert(has_ef00 && !has_0500);
}

// DP1 decodes to `gas` with z2m trueFalse0 polarity: wire 0 -> gas true
// (detected), wire 1 -> gas false (clear). And it does NOT emit the bare IAS
// "alarm" key.
static void test_gas_dp_inverted_polarity() {
    const auto* d = dg02();

    // wire value 0 -> gas detected (true).
    const std::uint8_t kDetected[] = { 0x00 };
    const TuyaDpRecord recsOn[] = {
        { 1, 0x01, std::span<const std::uint8_t>(kDetected, 1) },
    };
    const auto on = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsOn, 1));
    assert(on.any_matched);
    assert(b_true(on.merged.find("gas")));
    assert(on.merged.find("alarm") == nullptr);

    // wire value 1 -> gas clear (false).
    const std::uint8_t kClear[] = { 0x01 };
    const TuyaDpRecord recsOff[] = {
        { 1, 0x01, std::span<const std::uint8_t>(kClear, 1) },
    };
    const auto off = dispatch_dp(*d, std::span<const TuyaDpRecord>(recsOff, 1));
    assert(off.any_matched);
    assert(b_false(off.merged.find("gas")));
}

int main() {
    test_resolves_to_dp_def();
    test_binds_tuya_not_ias();
    test_gas_dp_inverted_polarity();
    return 0;
}
