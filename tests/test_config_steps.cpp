// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Host tests for the declarative config_steps pipeline.
//
// Verifies:
//   - Read / Cmd / Callback / Wait ops fire in array order
//   - Endpoint 0 in the step coerces to 1 inside the walker
//   - First failing step aborts and run_configure returns false
//   - Callback index out of range fails rather than crashing
//   - Missing platform hooks short-circuit (Read/Cmd → false, Wait → skip)

#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"

using namespace zhc;

namespace {

struct Recorded {
    enum class Kind { Read, Cmd, Sleep, Callback };
    Kind          kind;
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
    std::uint8_t  cmd_or_idx;
    std::uint8_t  flags;
    std::vector<std::uint8_t> payload;
    std::uint16_t wait_ms;
    bool          retval;
};

// Mutable test fixture. Test setup writes `s_script` to decide the
// return value for each mock op.
std::vector<Recorded>     s_calls;
bool                      s_next_read_ok  = true;
bool                      s_next_cmd_ok   = true;
bool                      s_next_cb_ok    = true;

bool mock_read(std::uint16_t /*idx*/, std::uint8_t ep,
                std::uint16_t cluster, const std::uint8_t* attrs,
                std::uint8_t attr_count, std::uint16_t /*manu_code*/) {
    Recorded r{Recorded::Kind::Read, ep, cluster, 0, 0,
               std::vector<std::uint8_t>(attrs, attrs + (attr_count * 2)),
               0, s_next_read_ok};
    s_calls.push_back(std::move(r));
    return s_next_read_ok;
}

bool mock_cmd(std::uint16_t /*idx*/, std::uint8_t ep,
               std::uint16_t cluster, std::uint8_t cmd,
               const std::uint8_t* payload, std::uint8_t payload_len,
               std::uint8_t flags) {
    Recorded r{Recorded::Kind::Cmd, ep, cluster, cmd, flags,
               std::vector<std::uint8_t>(payload, payload + payload_len),
               0, s_next_cmd_ok};
    s_calls.push_back(std::move(r));
    return s_next_cmd_ok;
}

void mock_sleep(std::uint16_t wait_ms) {
    Recorded r{Recorded::Kind::Sleep, 0, 0, 0, 0, {}, wait_ms, true};
    s_calls.push_back(std::move(r));
}

bool mock_callback_ok(std::uint16_t /*nwk*/, std::uint8_t ep,
                       RuntimeContext& /*ctx*/) {
    Recorded r{Recorded::Kind::Callback, ep, 0, 0, 0, {}, 0, s_next_cb_ok};
    s_calls.push_back(std::move(r));
    return s_next_cb_ok;
}

RuntimeContext make_ctx(bool wire_hooks = true) {
    RuntimeContext ctx{};
    ctx.device_index = 0;
    ctx.device_nwk   = 0xD049;
    if (wire_hooks) {
        ctx.configure_read  = &mock_read;
        ctx.configure_cmd   = &mock_cmd;
        ctx.configure_sleep = &mock_sleep;
    }
    return ctx;
}

PreparedDefinition make_def(const ConfigStep* steps,
                             std::uint8_t step_count,
                             const ConfigCallbackFn* cbs = nullptr,
                             std::uint8_t cb_count = 0) {
    PreparedDefinition d{};
    d.model  = "TEST";
    d.vendor = "Test";
    d.config_steps            = steps;
    d.config_steps_count      = step_count;
    d.config_callbacks        = cbs;
    d.config_callbacks_count  = cb_count;
    return d;
}

void reset_state() {
    s_calls.clear();
    s_next_read_ok = true;
    s_next_cmd_ok  = true;
    s_next_cb_ok   = true;
}

// ─── test_read_then_cmd_then_wait ────────────────────────────────────

void test_read_then_cmd_then_wait() {
    reset_state();
    static constexpr std::uint8_t attrs[] = { 0x04, 0x00, 0x05, 0x00 };
    static constexpr std::uint8_t body[]  = { 0x01, 0x02, 0x03 };

    ConfigStep steps[] = {
        { ConfigStepOp::Read, 1, 0x0000, 0,    0, attrs, sizeof(attrs), 1000 },
        { ConfigStepOp::Cmd,  1, 0x0006, 0x42, 0, body,  sizeof(body),  1000 },
        { ConfigStepOp::Wait, 0, 0,      0,    0, nullptr, 0,            50   },
    };
    auto def = make_def(steps, 3);
    auto ctx = make_ctx();

    const bool ok = run_configure(def, ctx);
    assert(ok);
    assert(s_calls.size() == 3);
    assert(s_calls[0].kind == Recorded::Kind::Read);
    assert(s_calls[0].endpoint == 1);
    assert(s_calls[0].cluster_id == 0x0000);
    assert(s_calls[0].payload.size() == sizeof(attrs));
    assert(s_calls[1].kind == Recorded::Kind::Cmd);
    assert(s_calls[1].cmd_or_idx == 0x42);
    assert(s_calls[1].cluster_id == 0x0006);
    assert(s_calls[1].payload == std::vector<std::uint8_t>(body, body + sizeof(body)));
    assert(s_calls[2].kind == Recorded::Kind::Sleep);
    assert(s_calls[2].wait_ms == 50);
}

// ─── test_endpoint_zero_coerces_to_1 ─────────────────────────────────

void test_endpoint_zero_coerces_to_1() {
    reset_state();
    ConfigStep steps[] = {
        { ConfigStepOp::Cmd, 0, 0x0006, 0x10, 0, nullptr, 0, 0 },
    };
    auto def = make_def(steps, 1);
    auto ctx = make_ctx();
    const bool ok = run_configure(def, ctx);
    assert(ok);
    assert(s_calls.size() == 1);
    assert(s_calls[0].endpoint == 1);   // coerced from 0
}

// ─── test_first_fail_aborts ──────────────────────────────────────────

void test_first_fail_aborts() {
    reset_state();
    ConfigStep steps[] = {
        { ConfigStepOp::Cmd, 1, 0x0006, 0x10, 0, nullptr, 0, 0 },
        { ConfigStepOp::Cmd, 1, 0x0006, 0x11, 0, nullptr, 0, 0 },
    };
    auto def = make_def(steps, 2);
    auto ctx = make_ctx();
    s_next_cmd_ok = false;

    const bool ok = run_configure(def, ctx);
    assert(!ok);
    assert(s_calls.size() == 1);   // second step never runs
}

// ─── test_callback_dispatch + bad index ─────────────────────────────

void test_callback_ok() {
    reset_state();
    const ConfigCallbackFn cbs[] = { &mock_callback_ok };
    ConfigStep steps[] = {
        { ConfigStepOp::Callback, 3, 0, /*idx=*/0, 0, nullptr, 0, 0 },
    };
    auto def = make_def(steps, 1, cbs, 1);
    auto ctx = make_ctx();
    const bool ok = run_configure(def, ctx);
    assert(ok);
    assert(s_calls.size() == 1);
    assert(s_calls[0].kind == Recorded::Kind::Callback);
    assert(s_calls[0].endpoint == 3);
}

void test_callback_bad_index() {
    reset_state();
    const ConfigCallbackFn cbs[] = { &mock_callback_ok };
    ConfigStep steps[] = {
        { ConfigStepOp::Callback, 1, 0, /*idx=*/2, 0, nullptr, 0, 0 },
    };
    auto def = make_def(steps, 1, cbs, 1);
    auto ctx = make_ctx();
    const bool ok = run_configure(def, ctx);
    assert(!ok);          // out-of-range slot fails gracefully
    assert(s_calls.empty());
}

// ─── test_missing_hook_fails_read_and_cmd ────────────────────────────

void test_missing_hook_fails() {
    reset_state();
    ConfigStep steps[] = {
        { ConfigStepOp::Read, 1, 0x0000, 0, 0, nullptr, 0, 0 },
    };
    auto def = make_def(steps, 1);
    auto ctx = make_ctx(false);   // no hooks wired

    const bool ok = run_configure(def, ctx);
    assert(!ok);
    assert(s_calls.empty());
}

// ─── test_wait_without_sleep_hook_is_skipped ─────────────────────────

void test_wait_without_sleep_hook() {
    reset_state();
    ConfigStep steps[] = {
        { ConfigStepOp::Wait, 0, 0, 0, 0, nullptr, 0, 25 },
    };
    auto def = make_def(steps, 1);
    auto ctx = make_ctx(false);   // no hooks wired

    const bool ok = run_configure(def, ctx);
    assert(ok);                    // Wait without hook = silent skip, still true
    assert(s_calls.empty());
}

}  // namespace

int main() {
    test_read_then_cmd_then_wait();
    test_endpoint_zero_coerces_to_1();
    test_first_fail_aborts();
    test_callback_ok();
    test_callback_bad_index();
    test_missing_hook_fails();
    test_wait_without_sleep_hook();
    return 0;
}
