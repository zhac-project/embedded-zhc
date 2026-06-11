// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the TIS Control IAS sensor family.
// z2m-source: tis_control.ts
//   - BEE PIR 1 (model ED6XX): extend [m.battery(),
//       m.iasZoneAlarm({zoneType:"occupancy",
//                       zoneAttributes:["alarm_1","tamper","battery_low"]})]
//     exposes: battery, voltage, occupancy, tamper, battery_low.
//
// Bug fixed:
//  (c) IAS dead-key — the auto-port lowered the generic kFzIasZone converter,
//      which emits the bare key "alarm", while z2m's iasZoneAlarm with zoneType
//      "occupancy" (bothAlarms=false) publishes the SEMANTIC key "occupancy"
//      (zoneStatus bit0). The expose was also named "alarm". With no rename
//      layer the motion state never reached the shadow. Graduated to a Tier-2
//      override swapping in kFzIasMotionAlarm (bare "occupancy" from bit0 +
//      tamper bit2 + battery_low bit3) and renaming the expose to "occupancy".
//
// This test pins, on a real IAS Zone Status Change Notification (cmd 0x00) wire
// shape, that BEE PIR 1 decodes the semantic "occupancy" key (NOT the dead bare
// "alarm" key), plus tamper and battery_low.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::tis_control {
extern const PreparedDefinition kDef_BEE_PIR_1;  // occupancy + battery
}  // namespace zhc::devices::tis_control

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

// 9-byte IAS Zone Status Change Notification (command 0x00):
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

DispatchResult dispatch_ias_notif(const PreparedDefinition& def, const IasFrame& f) {
    auto raw = make_frame(0x0500, 1,
                          std::span<const std::uint8_t>(f.bytes.data(), f.bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// ── BEE PIR 1 occupancy sensor ───────────────────────────────────────
// kFzIasMotionAlarm: bit0 -> occupancy, bit2 -> tamper, bit3 -> battery_low.
// The dead bare "alarm" key must NOT appear.
static void test_bee_pir_occupancy() {
    const auto& def = devices::tis_control::kDef_BEE_PIR_1;
    assert(def_exposes(def, "occupancy"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(!def_exposes(def, "alarm"));   // dead key removed from expose list
    assert(def_binds(def, 0x0500));       // ssIasZone
    assert(def_binds(def, 0x0001));       // genPowerCfg (battery)

    auto motion = dispatch_ias_notif(def, ias_notif(0x0001));  // bit0 set -> occupancy
    assert(motion.any_matched);
    assert(b_true(motion.merged.find("occupancy")));
    assert(motion.merged.find("alarm") == nullptr);            // dead-key gone
    assert(b_false(motion.merged.find("tamper")));
    assert(b_false(motion.merged.find("battery_low")));

    auto clear = dispatch_ias_notif(def, ias_notif(0x0000));   // bit0 clear -> no motion
    assert(clear.any_matched);
    assert(b_false(clear.merged.find("occupancy")));

    auto tb = dispatch_ias_notif(def, ias_notif(0x000C));      // tamper(bit2)+batt_low(bit3)
    assert(tb.any_matched);
    assert(b_false(tb.merged.find("occupancy")));              // bit0 clear
    assert(b_true(tb.merged.find("tamper")));
    assert(b_true(tb.merged.find("battery_low")));
}

int main() {
    test_bee_pir_occupancy();
    return 0;
}
