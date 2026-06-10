// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for the D-Link DCH-B112 (door/window sensor with vibration).
//
// Bug fixed:
//   The auto port wired the generic kFzIasZone, which decodes the ssIasZone
//   *attribute* into a bare `alarm` (zoneStatus bit0, NOT inverted) and never
//   emits the `contact` / `vibration` keys the device actually reports.
//   z2m's fzLocal.DCH_B112 decodes the ZoneStatusChangeNotification command
//   (cmd 0x00) into FOUR semantic keys off one zoneStatus word:
//     contact     = !(bit0)   (inverted)
//     vibration   =  bit1
//     tamper      =  bit2
//     battery_low =  bit3
//   Graduated to a Tier-2 override wiring a dedicated decoder + restoring the
//   contact/vibration exposes (the phantom `alarm` key is gone).
//
// z2m-source: dlink.ts #DCH-B112 (fzLocal.DCH_B112 + fz.battery).

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::dlink {
extern const PreparedDefinition kDef_DCH_B112;
}  // namespace zhc::devices::dlink

using namespace zhc;

namespace {

bool b_true(const Value* v)  { return v && v->type == ValueType::Bool && v->b; }
bool b_false(const Value* v) { return v && v->type == ValueType::Bool && !v->b; }

bool def_exposes(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (def.exposes[i].name && std::strcmp(def.exposes[i].name, key) == 0) return true;
    return false;
}

bool def_binds(const PreparedDefinition& def, std::uint8_t ep, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].endpoint == ep && def.bindings[i].cluster_id == cluster) return true;
    return false;
}

InboundApsFrame make_frame(std::uint16_t cluster, std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// IAS Zone Status Change Notification (ssIasZone cmd 0x00): fc=0x09, tsn, cmd,
// zonestatus u16 LE, ext-status u8, zoneid u8, delay u16 LE.
std::array<std::uint8_t, 9> ias_notif(std::uint16_t zone_status) {
    return { 0x09, 0x42, 0x00,
             static_cast<std::uint8_t>(zone_status & 0xFF),
             static_cast<std::uint8_t>(zone_status >> 8),
             0x00, 0x00, 0x00, 0x00 };
}

DispatchResult dispatch_ias(RuntimeContext& ctx, const PreparedDefinition& def,
                            std::uint16_t zone_status) {
    auto bytes = ias_notif(zone_status);
    auto raw = make_frame(0x0500, std::span<const std::uint8_t>(bytes.data(), bytes.size()));
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "ssIasZone";
    return dispatch_from_zigbee(msg, {}, def, raw, ctx);
}

}  // namespace

// The graduated def declares the restored contact + vibration channels and the
// phantom bare-`alarm` key is gone; it still binds genPowerCfg + ssIasZone.
static void test_exposes_and_bindings() {
    const auto& def = zhc::devices::dlink::kDef_DCH_B112;
    assert(def.model && std::strcmp(def.model, "DCH-B112") == 0);
    assert(def_exposes(def, "contact"));      // restored
    assert(def_exposes(def, "vibration"));    // restored
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "battery"));
    assert(def_exposes(def, "voltage"));
    assert(!def_exposes(def, "alarm"));       // phantom gone
    assert(def_binds(def, 1, 0x0001));        // genPowerCfg
    assert(def_binds(def, 1, 0x0500));        // ssIasZone
}

// zoneStatus = 0x0000 -> door closed (magnet present): contact:true (inverted
// bit0), and every other flag clear.
static void test_zone_all_clear_contact_true() {
    const auto& def = zhc::devices::dlink::kDef_DCH_B112;
    RuntimeContext ctx{};
    auto r = dispatch_ias(ctx, def, 0x0000);
    assert(r.any_matched);
    assert(b_true(r.merged.find("contact")));       // !(bit0) == true
    assert(b_false(r.merged.find("vibration")));
    assert(b_false(r.merged.find("tamper")));
    assert(b_false(r.merged.find("battery_low")));
    // No bare `alarm` key should ever be produced.
    assert(r.merged.find("alarm") == nullptr);
}

// bit0 set -> door open -> contact:false (inverted). Confirms the inversion.
static void test_zone_bit0_contact_false() {
    const auto& def = zhc::devices::dlink::kDef_DCH_B112;
    RuntimeContext ctx{};
    auto r = dispatch_ias(ctx, def, 0x0001);
    assert(r.any_matched);
    assert(b_false(r.merged.find("contact")));
}

// bit1 -> vibration:true (this is the channel the generic IAS converter
// dropped entirely).
static void test_zone_bit1_vibration() {
    const auto& def = zhc::devices::dlink::kDef_DCH_B112;
    RuntimeContext ctx{};
    auto r = dispatch_ias(ctx, def, 0x0002);
    assert(r.any_matched);
    assert(b_true(r.merged.find("vibration")));
    assert(b_true(r.merged.find("contact")));  // bit0 clear -> contact still true
}

// bit2 -> tamper, bit3 -> battery_low (and all four set together).
static void test_zone_tamper_and_battery_low() {
    const auto& def = zhc::devices::dlink::kDef_DCH_B112;
    RuntimeContext ctx{};
    auto r = dispatch_ias(ctx, def, 0x000F);  // bits 0..3 all set
    assert(r.any_matched);
    assert(b_false(r.merged.find("contact")));     // bit0 set -> open
    assert(b_true(r.merged.find("vibration")));     // bit1
    assert(b_true(r.merged.find("tamper")));        // bit2
    assert(b_true(r.merged.find("battery_low")));   // bit3
}

int main() {
    test_exposes_and_bindings();
    test_zone_all_clear_contact_true();
    test_zone_bit0_contact_false();
    test_zone_bit1_vibration();
    test_zone_tamper_and_battery_low();
    return 0;
}
