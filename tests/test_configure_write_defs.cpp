// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Host tests: real device definitions that carry a `ConfigStepOp::Write`
// configure step actually route the write through `ctx.configure_write`
// with the exact (endpoint, cluster, attr_id, attr_type, value, manu_code).
//
// Covers the two defs wired in feat/wire-configure-write:
//   - Tuya TS004F knob (kDef_TS004F_knob): genOnOff (0x0006) attr 0x8004
//     (tuyaOperationMode), enum8 0x30, value {0x01} ("event"), manu 0.
//     z2m: tuya.ts ERS-10TZBVK-AA `endpoint.write("genOnOff",
//     {tuyaOperationMode: 1})`.
//   - Aqara WXCJKG11LM (kDefWXCJKG11LM): manuSpecificLumi (0xFCC0) attr
//     0x0009 (mode), uint8 0x20, value {0x01} ("event"), manu 0x115F.
//     z2m: lumi.ts `endpoint.write("manuSpecificLumi", {mode: 1},
//     {manufacturerCode: 0x115f})`.
//
// Plus the five lumi event-mode defs wired in feat/wire-lumi-mode (same
// manuSpecificLumi mode-write as WXCJKG11LM): WXCJKG12LM, WXCJKG13LM,
// WXKG15LM (parent def for the Aqara WRS-R02 whiteLabel), WXKG21LM,
// WXKG22LM.
//
// These defs also carry non-Write steps (Wait / Read). The walker fails a
// Read with no hook, so this fixture wires harmless success stubs for the
// read / cmd / sleep hooks and only *captures* the Write op — exactly how
// the firmware adapter will behave once it registers configure_write.

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"

namespace zhc::devices::tuya {
extern const PreparedDefinition kDef_TS004F_knob;
}
namespace zhc::devices::lumi {
extern const PreparedDefinition kDefWXCJKG11LM;
extern const PreparedDefinition kDefWXCJKG12LM;
extern const PreparedDefinition kDefWXCJKG13LM;
extern const PreparedDefinition kDefWXKG15LM;   // also covers WRS-R02 whiteLabel
extern const PreparedDefinition kDefWXKG21LM;
extern const PreparedDefinition kDefWXKG22LM;
}

using namespace zhc;

namespace {

struct WriteCall {
    std::uint8_t              endpoint;
    std::uint16_t             cluster_id;
    std::uint16_t             attr_id;
    std::uint8_t              attr_type;
    std::uint16_t             manu_code;
    std::vector<std::uint8_t> value;
};

std::vector<WriteCall> s_writes;

bool capture_write(std::uint16_t /*idx*/, std::uint8_t ep,
                   std::uint16_t cluster, std::uint16_t attr_id,
                   std::uint8_t attr_type, const std::uint8_t* value,
                   std::size_t len, std::uint16_t manu_code) {
    s_writes.push_back(WriteCall{
        ep, cluster, attr_id, attr_type, manu_code,
        std::vector<std::uint8_t>(value, value + len)});
    return true;
}

// Harmless success stubs so the non-Write steps (Wait / Read) in the real
// defs do not abort the configure walk before the Write fires. Signatures
// must match the ConfigureReadFn / ConfigureCmdFn / ConfigureSleepFn
// typedefs in dispatch.hpp exactly.
bool ok_read(std::uint16_t, std::uint8_t, std::uint16_t,
             const std::uint8_t*, std::uint8_t, std::uint16_t) {
    return true;
}
bool ok_cmd(std::uint16_t, std::uint8_t, std::uint16_t, std::uint8_t,
            const std::uint8_t*, std::uint8_t, std::uint8_t) {
    return true;
}
void ok_sleep(std::uint16_t) {}

RuntimeContext make_ctx() {
    RuntimeContext ctx{};
    ctx.device_index    = 0;
    ctx.device_nwk      = 0xD049;
    ctx.configure_read  = &ok_read;
    ctx.configure_cmd   = &ok_cmd;
    ctx.configure_write = &capture_write;
    ctx.configure_sleep = &ok_sleep;
    return ctx;
}

// Find the single captured Write to (cluster, attr_id). Asserts exactly one.
const WriteCall& only_write_to(std::uint16_t cluster, std::uint16_t attr_id) {
    const WriteCall* found = nullptr;
    int count = 0;
    for (const auto& w : s_writes) {
        if (w.cluster_id == cluster && w.attr_id == attr_id) {
            found = &w;
            ++count;
        }
    }
    assert(count == 1 && "expected exactly one matching Write");
    return *found;
}

// ─── Tuya TS004F knob — operation_mode force-write ───────────────────
void test_ts004f_knob_operation_mode_write() {
    s_writes.clear();
    auto ctx = make_ctx();

    const bool ok = run_configure(devices::tuya::kDef_TS004F_knob, ctx);
    assert(ok);
    assert(s_writes.size() == 1);          // exactly one Write op in the def

    const WriteCall& w = only_write_to(0x0006, 0x8004);
    assert(w.endpoint  == 1);
    assert(w.cluster_id == 0x0006);        // genOnOff
    assert(w.attr_id   == 0x8004);         // tuyaOperationMode
    assert(w.attr_type == 0x30);           // enum8
    assert(w.manu_code == 0x0000);         // profile-wide
    assert(w.value == std::vector<std::uint8_t>{0x01});   // "event"
}

// ─── Aqara WXCJKG11LM — manuSpecificLumi mode event-write ────────────
void test_wxcjkg11lm_mode_write() {
    s_writes.clear();
    auto ctx = make_ctx();

    const bool ok = run_configure(devices::lumi::kDefWXCJKG11LM, ctx);
    assert(ok);
    assert(s_writes.size() == 1);          // exactly one Write op in the def

    const WriteCall& w = only_write_to(0xFCC0, 0x0009);
    assert(w.endpoint  == 1);
    assert(w.cluster_id == 0xFCC0);        // manuSpecificLumi
    assert(w.attr_id   == 0x0009);         // mode
    assert(w.attr_type == 0x20);           // uint8
    assert(w.manu_code == 0x115F);         // Lumi manufacturer code
    assert(w.value == std::vector<std::uint8_t>{0x01});   // "event"
}

// Shared check for the lumi event-mode defs wired in feat/wire-lumi-mode:
// every one writes manuSpecificLumi (0xFCC0) attr mode (0x0009, uint8) = 1
// with manufacturerCode 0x115F on endpoint 1, and nothing else.
void assert_lumi_mode_write(const PreparedDefinition& def) {
    s_writes.clear();
    auto ctx = make_ctx();

    const bool ok = run_configure(def, ctx);
    assert(ok);
    assert(s_writes.size() == 1);          // exactly one Write op in the def

    const WriteCall& w = only_write_to(0xFCC0, 0x0009);
    assert(w.endpoint  == 1);
    assert(w.cluster_id == 0xFCC0);        // manuSpecificLumi
    assert(w.attr_id   == 0x0009);         // mode
    assert(w.attr_type == 0x20);           // uint8
    assert(w.manu_code == 0x115F);         // Lumi manufacturer code
    assert(w.value == std::vector<std::uint8_t>{0x01});   // "event"
}

void test_wxcjkg12lm_mode_write() {
    assert_lumi_mode_write(devices::lumi::kDefWXCJKG12LM);
}
void test_wxcjkg13lm_mode_write() {
    assert_lumi_mode_write(devices::lumi::kDefWXCJKG13LM);
}
// WXKG15LM is the parent def for the Aqara WRS-R02 whiteLabel.
void test_wxkg15lm_mode_write() {
    assert_lumi_mode_write(devices::lumi::kDefWXKG15LM);
}
void test_wxkg21lm_mode_write() {
    assert_lumi_mode_write(devices::lumi::kDefWXKG21LM);
}
void test_wxkg22lm_mode_write() {
    assert_lumi_mode_write(devices::lumi::kDefWXKG22LM);
}

}  // namespace

int main() {
    test_ts004f_knob_operation_mode_write();
    test_wxcjkg11lm_mode_write();
    test_wxcjkg12lm_mode_write();
    test_wxcjkg13lm_mode_write();
    test_wxkg15lm_mode_write();
    test_wxkg21lm_mode_write();
    test_wxkg22lm_mode_write();
    return 0;
}
