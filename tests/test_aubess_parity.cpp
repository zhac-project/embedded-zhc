// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for the Aubess ZXZIR-02 universal smart IR remote control.
// z2m-source: aubess.ts #ZXZIR-02 (zosung.zosungExtend + fzZosung +
// tzZosung + presetsZosung).
//
// Bug fixed: the auto-generated Tier-1 port mis-classified this pure
// Zosung IR blaster/learner as a bare on/off + battery device — phantom
// kFzOnOff + kFzBattery, dead `state`/`battery`/`voltage` exposes, and
// bindings to genOnOff (0x0006) + genPowerCfg (0x0001). The z2m record
// emits NONE of those clusters: it is built entirely on the Zosung IR
// runtime (clusters 0xED00 IRTransmit + 0xE004 IRControl) and exposes
// only learn_ir_code / learned_ir_code / ir_code_to_send.
//
// Graduated to a Tier-2 parent wiring the existing Zosung converters
// (definitions/zosung/_shared), matching the Moes UFO-R11 sibling but
// WITHOUT battery (z2m's Aubess def exposes none).
//
// These tests pin the corrected wiring: the IR exposes replace the
// phantom bundle, the six Zosung fz decoders + two tz writers are wired,
// the bindings target the two Zosung clusters (not genOnOff/genPowerCfg),
// and the learn_ir_code tz path emits an IRControl cmd-00 {"study":0}
// frame.

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <span>
#include <string_view>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

#include "definitions/zosung/_shared.hpp"

namespace zhc::devices::aubess {
extern const PreparedDefinition kDef_ZXZIR_02;
}  // namespace zhc::devices::aubess

using namespace zhc;

namespace {

bool has_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (std::strcmp(def.exposes[i].name, name) == 0) return true;
    }
    return false;
}

bool has_fz(const PreparedDefinition& def, const FzConverter* conv) {
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i) {
        if (def.from_zigbee[i] == conv) return true;
    }
    return false;
}

bool has_tz_key(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i) {
        if (def.to_zigbee[i]->key && std::strcmp(def.to_zigbee[i]->key, key) == 0)
            return true;
    }
    return false;
}

bool has_binding(const PreparedDefinition& def, std::uint16_t cluster) {
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].cluster_id == cluster) return true;
    }
    return false;
}

// The corrected def must expose exactly the three Zosung IR primitives —
// and NONE of the phantom on/off + battery keys from the broken port.
void test_exposes() {
    const auto& d = zhc::devices::aubess::kDef_ZXZIR_02;
    assert(has_expose(d, "learn_ir_code"));
    assert(has_expose(d, "learned_ir_code"));
    assert(has_expose(d, "ir_code_to_send"));
    // Phantom bundle gone.
    assert(!has_expose(d, "state"));
    assert(!has_expose(d, "battery"));
    assert(!has_expose(d, "voltage"));
    assert(d.exposes_count == 3);
}

// The six Zosung IRTransmit decoders must be wired; the generic on/off +
// battery decoders must be gone.
void test_from_zigbee() {
    const auto& d = zhc::devices::aubess::kDef_ZXZIR_02;
    assert(has_fz(d, &::zhc::zosung::kFzZosungSendIrCode_00));
    assert(has_fz(d, &::zhc::zosung::kFzZosungSendIrCode_01));
    assert(has_fz(d, &::zhc::zosung::kFzZosungSendIrCode_02));
    assert(has_fz(d, &::zhc::zosung::kFzZosungSendIrCode_03));
    assert(has_fz(d, &::zhc::zosung::kFzZosungSendIrCode_04));
    assert(has_fz(d, &::zhc::zosung::kFzZosungSendIrCode_05));
    assert(d.from_zigbee_count == 6);
}

// Both Zosung tz writers must be wired by their z2m keys.
void test_to_zigbee() {
    const auto& d = zhc::devices::aubess::kDef_ZXZIR_02;
    assert(has_tz_key(d, "ir_code_to_send"));
    assert(has_tz_key(d, "learn_ir_code"));
    assert(d.to_zigbee_count == 2);
}

// Bindings target the two Zosung clusters — NOT the phantom genOnOff /
// genPowerCfg the broken port bound.
void test_bindings() {
    const auto& d = zhc::devices::aubess::kDef_ZXZIR_02;
    assert(has_binding(d, ::zhc::zosung::kZosungIRTransmitCluster));  // 0xED00
    assert(has_binding(d, ::zhc::zosung::kZosungIRControlCluster));   // 0xE004
    assert(!has_binding(d, 0x0006));  // genOnOff — phantom, gone
    assert(!has_binding(d, 0x0001));  // genPowerCfg — phantom, gone
    assert(d.bindings_count == 2);
}

// learn_ir_code tz: writing it must encode an IRControl cmd-00 frame whose
// OCTET_STR payload is the z2m learn-mode JSON {"study":0}.
void test_learn_ir_tz() {
    const auto& d = zhc::devices::aubess::kDef_ZXZIR_02;
    std::uint8_t frame[64] = {};
    RuntimeContext ctx{};
    Value v{};
    v.type = ValueType::StringRef;
    v.str = "ON";  // z2m's UI sends "ON"; converter accepts any input.
    auto r = dispatch_to_zigbee(d, "learn_ir_code", v, ctx,
                                std::span<std::uint8_t>(frame, sizeof(frame)));
    assert(r.ok);
    assert(r.cluster_id == ::zhc::zosung::kZosungIRControlCluster);  // 0xE004
    assert(r.command_id == 0x00);
    // FC | TSN | cmd(0x00) | len | "{\"study\":0}"
    static const char kJson[] = "{\"study\":0}";
    const std::size_t json_len = sizeof(kJson) - 1;
    assert(r.frame_size == 3 + 1 + json_len);
    assert(frame[2] == 0x00);                 // command id
    assert(frame[3] == json_len);             // OCTET_STR length prefix
    assert(std::memcmp(&frame[4], kJson, json_len) == 0);
}

}  // namespace

int main() {
    test_exposes();
    test_from_zigbee();
    test_to_zigbee();
    test_bindings();
    test_learn_ir_tz();
    std::printf("aubess parity tests passed\n");
    return 0;
}
