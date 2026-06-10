// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity test for Xenon (single z2m def: X7726 smart Zigbee curtain motor,
// TS0601 / _TZE284_hbjwgkdh — a Tuya-MCU 0xEF00 datapoint device).
//
// The parity pass found a dup-fingerprint misroute: the vendor dir carried TWO
// generated defs for the SAME fingerprint —
//   * Xen__TZE284_hbjwgkdh.cpp — the correct full Tuya-DP port, AND
//   * Xen_X7726.cpp — a stub that misrouted the same fingerprint to
//     genWindowCovering (0x0102) via kFzCoverPosition and dropped
//     state/calibration/temperature.
// The firmware only emits this device's state over 0xEF00 DPs, so the 0x0102
// route was dead. The stub was removed and the full DP def graduated to
// definitions/xenon/X7726.cpp, renamed to the z2m identity (X7726 / Xenon Smart)
// and with the phantom `action` expose dropped.
//
// These tests decode real 0xEF00 (manuSpecificTuya) DP frames through the
// dispatcher and assert the device resolves to the DP port and decodes every
// z2m datapoint (state enum, position, calibration enum, temperature).
//
// z2m-source: zigbee-herdsman-converters/src/devices/xenon.ts.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/devices/xenon_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

std::span<const PreparedDefinition* const> xenon_registry() {
    return std::span<const PreparedDefinition* const>(
        devices::xenon::kXenonRegistry,
        devices::xenon::kXenonRegistryCount);
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

const PreparedDefinition& resolve() {
    const auto* d = find_definition("TS0601", "_TZE284_hbjwgkdh", xenon_registry());
    assert(d && "xenon _TZE284_hbjwgkdh must resolve to a def");
    assert(d->model && std::strcmp(d->model, "X7726") == 0);
    return *d;
}

const char* decode_enum(const PreparedDefinition& def, std::uint8_t dp,
                        std::uint8_t wire, const char* out_key) {
    const std::uint8_t b[] = { wire };
    const TuyaDpRecord recs[] = {
        { dp, 0x04, std::span<const std::uint8_t>(b, 1) },
    };
    const auto result = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find(out_key);
    if (!v || v->type != ValueType::StringRef) return nullptr;
    return v->str;
}

double decode_numeric(const PreparedDefinition& def, std::uint8_t dp,
                      std::uint32_t wire, const char* out_key) {
    const std::uint8_t b[] = {
        static_cast<std::uint8_t>((wire >> 24) & 0xFF),
        static_cast<std::uint8_t>((wire >> 16) & 0xFF),
        static_cast<std::uint8_t>((wire >> 8) & 0xFF),
        static_cast<std::uint8_t>(wire & 0xFF),
    };
    const TuyaDpRecord recs[] = {
        { dp, 0x02, std::span<const std::uint8_t>(b, 4) },
    };
    const auto result = dispatch_dp(def, std::span<const TuyaDpRecord>(recs, 1));
    assert(result.any_matched);
    const Value* v = result.merged.find(out_key);
    assert(v && "numeric DP must surface its key");
    return (v->type == ValueType::Int) ? double(v->i) : double(v->f);
}

const Expose* find_expose(const PreparedDefinition& def, const char* key) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        if (std::strcmp(def.exposes[i].name, key) == 0) return &def.exposes[i];
    }
    return nullptr;
}

}  // namespace

// --- The single def must be the Tuya-DP port, not a genWindowCovering stub ---
// Exactly one def survives; it carries the z2m identity and routes 0xEF00 DPs.
static void test_single_dp_def_resolves() {
    assert(devices::xenon::kXenonRegistryCount == 1 &&
           "the misrouted dup-fingerprint stub must be gone");
    const auto& d = resolve();
    assert(std::strcmp(d.vendor, "Xenon Smart") == 0);
    // It is a Tuya-DP device: a from_zigbee converter must claim 0xEF00 DPs.
    assert(d.from_zigbee_count >= 1);
}

// --- No phantom exposes; types match z2m ------------------------------------
static void test_exposes_shape() {
    const auto& d = resolve();
    // z2m exposes exactly: cover_position(position), calibration, temperature
    // (+ internal state enum). The phantom `action` expose must be gone.
    assert(find_expose(d, "action") == nullptr && "phantom action expose removed");

    const Expose* state = find_expose(d, "state");
    assert(state && state->type == ExposeType::Enum);
    const Expose* pos = find_expose(d, "position");
    assert(pos && pos->type == ExposeType::Numeric);
    const Expose* cal = find_expose(d, "calibration");
    assert(cal && cal->type == ExposeType::Enum);
    const Expose* temp = find_expose(d, "temperature");
    assert(temp && temp->type == ExposeType::Numeric);
}

// --- DP1 state enum {OPEN:0, STOP:1, CLOSE:2} -------------------------------
static void test_state_enum_decode() {
    const auto& d = resolve();
    const char* s0 = decode_enum(d, 1, 0, "state");
    assert(s0 && std::strcmp(s0, "OPEN") == 0);
    const char* s1 = decode_enum(d, 1, 1, "state");
    assert(s1 && std::strcmp(s1, "STOP") == 0);
    const char* s2 = decode_enum(d, 1, 2, "state");
    assert(s2 && std::strcmp(s2, "CLOSE") == 0);
}

// --- DP2/DP3 position: 1:1 percent (z2m coverPosition identity) -------------
static void test_position_decode() {
    const auto& d = resolve();
    assert(decode_numeric(d, 2, 75, "position") == 75.0);
    assert(decode_numeric(d, 3, 40, "position") == 40.0);
}

// --- DP102 calibration enum {start:0, finish:1} -----------------------------
static void test_calibration_enum_decode() {
    const auto& d = resolve();
    const char* c0 = decode_enum(d, 102, 0, "calibration");
    assert(c0 && std::strcmp(c0, "start") == 0);
    const char* c1 = decode_enum(d, 102, 1, "calibration");
    assert(c1 && std::strcmp(c1, "finish") == 0);
}

// --- DP103 temperature: raw 1:1 (z2m valueConverter.raw) --------------------
static void test_temperature_decode() {
    const auto& d = resolve();
    assert(decode_numeric(d, 103, 23, "temperature") == 23.0);
}

int main() {
    test_single_dp_def_resolves();
    test_exposes_shape();
    test_state_enum_decode();
    test_position_decode();
    test_calibration_enum_decode();
    test_temperature_decode();
    return 0;
}
