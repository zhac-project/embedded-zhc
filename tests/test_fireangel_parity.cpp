// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the FireAngel CO family.
// z2m-source: fireangel.ts.
//
// Bugs fixed (generator misses), pinned here on real wire shapes:
//
//  * W2-Module (zigbeeModel "Alarm_SD_Device") — z2m wires
//    fz.W2_module_carbon_monoxide + fz.battery and exposes carbon_monoxide().
//    The generated def dropped the CO channel entirely (battery-only). The
//    module reports CO on ssIasZone zoneStatus *bit 8* (not the standard
//    bit 0), so the generic kFzIasCoAlarm would never fire. Re-added the
//    dedicated kFzW2ModuleCarbonMonoxide + carbon_monoxide expose + 0x0500
//    binding.
//
//  * ZBCO-AE-10X-EUR (fingerprint "Alarm_SD_Device" + manufacturerName
//    "Fireangel") — missing from the registry entirely. z2m decodes via
//    fz.ias_carbon_monoxide_alarm_1 (carbon_monoxide=bit0, tamper=bit2,
//    battery_low=bit3 → generic kFzIasCoAlarm) + fzLocal.fireangel_co_test
//    (test = bit5 OR bit9 → kFzFireangelCoTest). Authored the def.
//
//  * find_definition coexistence: both defs carry model "Alarm_SD_Device".
//    The ZBCO def is manufacturer-gated ("Fireangel") so it wins Pass 1 for
//    FireAngel hardware; the model-only W2-Module is the fallback for any
//    other manufacturer.

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::fireangel {
extern const PreparedDefinition kDef_W2_Module;
extern const PreparedDefinition kDef_ZBCO_AE_10X_EUR;
}  // namespace zhc::devices::fireangel

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

DispatchResult dispatch_zcl(const PreparedDefinition& def, std::uint16_t cluster,
                            std::uint8_t src_ep, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    auto raw = make_frame(cluster, src_ep, bytes);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
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

bool def_binds(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i)
        if (def.bindings[i].cluster_id == cluster_id) return true;
    return false;
}

// 9-byte IAS Zone Status Change Notification (cmd 0x00): fc, tsn, cmd,
// zoneStatus(u16 LE), extStatus, zoneId, delay(u16).
std::array<std::uint8_t, 9> ias_notif(std::uint16_t zone_status) {
    return { 0x09, 0x42, 0x00,
             static_cast<std::uint8_t>(zone_status & 0xFF),
             static_cast<std::uint8_t>(zone_status >> 8),
             0x00, 0x00, 0x00, 0x00 };
}

}  // namespace

// ── W2-Module — carbon_monoxide on zoneStatus bit 8 ───────────────────
static void test_w2_module_carbon_monoxide() {
    const auto& def = devices::fireangel::kDef_W2_Module;
    assert(def_exposes(def, "carbon_monoxide"));
    assert(def_exposes(def, "battery"));
    assert(def_binds(def, 0x0500));   // ssIasZone re-added
    assert(def_binds(def, 0x0001));   // genPowerCfg (battery)

    // bit 8 set → CO detected.
    auto on = ias_notif(1u << 8);
    auto ron = dispatch_zcl(def, 0x0500, 1, "ssIasZone", {on.data(), on.size()});
    assert(ron.any_matched);
    assert(b_true(ron.merged.find("carbon_monoxide")));

    // bit 0 set (standard CO bit) must NOT trigger this device's bit-8 decode.
    auto b0 = ias_notif(1u << 0);
    auto rb0 = dispatch_zcl(def, 0x0500, 1, "ssIasZone", {b0.data(), b0.size()});
    assert(b_false(rb0.merged.find("carbon_monoxide")));

    // all-clear.
    auto off = ias_notif(0x0000);
    auto roff = dispatch_zcl(def, 0x0500, 1, "ssIasZone", {off.data(), off.size()});
    assert(b_false(roff.merged.find("carbon_monoxide")));
}

// ── ZBCO-AE-10X-EUR — standard IAS CO + tamper + battery_low + test ──
static void test_zbco_co_alarm() {
    const auto& def = devices::fireangel::kDef_ZBCO_AE_10X_EUR;
    assert(def_exposes(def, "carbon_monoxide"));
    assert(def_exposes(def, "tamper"));
    assert(def_exposes(def, "battery_low"));
    assert(def_exposes(def, "test"));
    assert(def_binds(def, 0x0500));

    // bit0 = carbon_monoxide, bit2 = tamper, bit3 = battery_low.
    auto alarm = ias_notif(0x0001 | (1u << 2) | (1u << 3));
    auto ra = dispatch_zcl(def, 0x0500, 1, "ssIasZone", {alarm.data(), alarm.size()});
    assert(ra.any_matched);
    assert(b_true(ra.merged.find("carbon_monoxide")));
    assert(b_true(ra.merged.find("tamper")));
    assert(b_true(ra.merged.find("battery_low")));
    // No CO test bits set → test=false (decoder still fires).
    assert(b_false(ra.merged.find("test")));

    // all-clear.
    auto clear = ias_notif(0x0000);
    auto rc = dispatch_zcl(def, 0x0500, 1, "ssIasZone", {clear.data(), clear.size()});
    assert(b_false(rc.merged.find("carbon_monoxide")));
    assert(b_false(rc.merged.find("tamper")));
    assert(b_false(rc.merged.find("battery_low")));
    assert(b_false(rc.merged.find("test")));

    // test bit 5.
    auto t5 = ias_notif(1u << 5);
    auto rt5 = dispatch_zcl(def, 0x0500, 1, "ssIasZone", {t5.data(), t5.size()});
    assert(b_true(rt5.merged.find("test")));

    // test bit 9.
    auto t9 = ias_notif(1u << 9);
    auto rt9 = dispatch_zcl(def, 0x0500, 1, "ssIasZone", {t9.data(), t9.size()});
    assert(b_true(rt9.merged.find("test")));
}

// ── find_definition: mfr-gated ZBCO wins; W2 is the model-only fallback ─
static void test_definition_routing() {
    const std::array<const PreparedDefinition*, 2> reg = {
        &devices::fireangel::kDef_ZBCO_AE_10X_EUR,
        &devices::fireangel::kDef_W2_Module,
    };
    std::span<const PreparedDefinition* const> sp{reg.data(), reg.size()};

    // FireAngel CO alarm → manufacturer-gated ZBCO def.
    const auto* zbco = find_definition("Alarm_SD_Device", "Fireangel", sp);
    assert(zbco == &devices::fireangel::kDef_ZBCO_AE_10X_EUR);

    // Any other manufacturer reporting the same model → W2-Module fallback.
    const auto* w2 = find_definition("Alarm_SD_Device", "SomeoneElse", sp);
    assert(w2 == &devices::fireangel::kDef_W2_Module);
}

int main() {
    test_w2_module_carbon_monoxide();
    test_zbco_co_alarm();
    test_definition_routing();
    return 0;
}
