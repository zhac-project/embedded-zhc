// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Behavioural-parity fixture tests for the Zosung IR runtime as used by
// the Moes UFO-R11 (TS1201) IR remote.
//
// Goal: prove the host's response-builder logic matches z2m's Send/Learn
// state machine for the two cleanly-testable handlers:
//
//   * fz_zosung_send_ir_code_01  (cmd 01 echo during Send) — z2m logs
//     only; we mirror that and verify the converter claims the frame
//     without emitting any cluster commands.
//
//   * fz_zosung_send_ir_code_04  (cmd 04 transfer-complete) — host must
//     reply with cmd 0x05 ack. We capture the emitted frame via the
//     `configure_cmd` hook and verify cluster, command id, and payload.
//
// HONEST GAP: the underlying Zosung handlers carry a TODO for the
// device-payload chunk dispatch (see zosung/_shared.cpp — they use
// stand-in seq=0 / length=Session::kBufCap because the dispatcher does
// not yet expose the raw cluster-command body to converters). We
// therefore validate the response-builder structurally — what bytes it
// emits — rather than full chunk-level round-trip behaviour.
//
// z2m-source: zigbee-herdsman-converters/src/lib/zosung.ts (commit
//             ~2025-Q1, lines 237-294, 380-404).

#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>

#include "definitions/zosung/_shared.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"

using namespace zhc;

namespace {

// Captured outbound cluster command — matches the ConfigureCmdFn tuple.
struct CapturedCmd {
    std::uint16_t device_index;
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
    std::uint8_t  cmd_id;
    std::vector<std::uint8_t> payload;
    std::uint8_t  flags;
};

std::vector<CapturedCmd> g_captured;

bool capture_cmd(std::uint16_t idx, std::uint8_t ep,
                 std::uint16_t cluster, std::uint8_t cmd,
                 const std::uint8_t* payload, std::uint8_t payload_len,
                 std::uint8_t flags) {
    CapturedCmd c{
        idx, ep, cluster, cmd,
        std::vector<std::uint8_t>(payload, payload + payload_len),
        flags
    };
    g_captured.push_back(std::move(c));
    return true;  // simulate transport accept
}

void reset_capture() { g_captured.clear(); }

RuntimeContext make_ctx() {
    RuntimeContext ctx{};
    ctx.device_index  = 7;
    ctx.configure_cmd = &capture_cmd;
    return ctx;
}

DecodedMessage make_zosung_msg(std::uint8_t cmd_id) {
    DecodedMessage msg{};
    msg.family       = FrameFamily::Zcl;
    msg.cluster      = ::zhc::zosung::kZosungIRTransmitName;
    msg.type         = MessageType::Command;
    msg.direction    = Direction::ServerToClient;
    msg.command_id   = cmd_id;
    msg.src_endpoint = 1;
    msg.dst_endpoint = 1;
    return msg;
}

}  // namespace

// fz_zosung_send_ir_code_01 — z2m treats this as debug-log only. The
// converter must claim the frame (return true so the unhandled-frame
// hook stays quiet) but emit zero cluster commands.
//
// TODO(fixture): once raw APS payload access is wired through (see the
// zosung.cpp TODO), tighten this to also assert the seq value the host
// would echo back. For now we assert structural quietness only.
static void test_fz_01_logs_only_no_emit() {
    reset_capture();
    auto ctx = make_ctx();
    auto msg = make_zosung_msg(::zhc::zosung::cmd::kSendIRCode01);
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};

    // Pass dummies for FzConverter + PreparedDefinition since the
    // handler doesn't read them in this branch.
    FzConverter dummy_self{};
    PreparedDefinition dummy_def{};

    const bool claimed = ::zhc::zosung::fz_zosung_send_ir_code_01(
        msg, dummy_self, dummy_def, ctx, out);
    assert(claimed);
    assert(g_captured.empty());
    assert(out.count == 0);
}

// fz_zosung_send_ir_code_00 — device announces an inbound IR push.
// Host MUST queue cmd 01 (length echo) AND cmd 02 (chunk request) on the
// IRTransmit cluster. Verify the response-builder structurally AND that
// it honours the seq + length values read from the wire body.
static void test_fz_00_emits_cmd_01_and_02() {
    reset_capture();
    // Reset the static session — fz_00 mutates it; tests must isolate.
    auto& sess = ::zhc::zosung::session_slot();
    sess.mode     = ::zhc::zosung::Session::Mode::Idle;
    sess.position = 0;
    sess.length   = 0;

    auto ctx = make_ctx();
    auto msg = make_zosung_msg(::zhc::zosung::cmd::kSendIRCode00);

    // Cmd 00 body (16 B): seq u16 + length u32 + unk1 u32 + unk2 u16
    // + unk3 u8 + cmd u8 + unk4 u16. We pick seq=0x1234, length=128.
    static const std::uint8_t kCmd00Body[16] = {
        0x34, 0x12,                          // seq = 0x1234
        0x80, 0x00, 0x00, 0x00,              // length = 128
        0x00, 0x00, 0x00, 0x00,              // unk1
        0x04, 0xE0,                          // unk2 = 0xE004
        0x01,                                // unk3
        0x02,                                // cmd
        0x00, 0x00,                          // unk4
    };
    msg.raw_body = std::span<const std::uint8_t>(kCmd00Body, 16);

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    FzConverter dummy_self{};
    PreparedDefinition dummy_def{};

    const bool claimed = ::zhc::zosung::fz_zosung_send_ir_code_00(
        msg, dummy_self, dummy_def, ctx, out);
    assert(claimed);
    assert(g_captured.size() == 2);

    // Frame 0: cmd 01 echo, IRTransmit cluster, 17-byte payload
    // (leading zero + 16 B verbatim copy of cmd 00 body).
    assert(g_captured[0].cluster_id == ::zhc::zosung::kZosungIRTransmitCluster);
    assert(g_captured[0].cmd_id     == ::zhc::zosung::cmd::kSendIRCode01);
    assert(g_captured[0].payload.size() == 17);
    assert(g_captured[0].endpoint == 1);
    assert(g_captured[0].payload[0] == 0x00);
    assert(std::memcmp(&g_captured[0].payload[1], kCmd00Body, 16) == 0);

    // Frame 1: cmd 02 chunk request, IRTransmit cluster, 7-byte payload.
    // seq u16 + position u32 + maxlen u8. Seq must echo the wire seq.
    assert(g_captured[1].cluster_id == ::zhc::zosung::kZosungIRTransmitCluster);
    assert(g_captured[1].cmd_id     == ::zhc::zosung::cmd::kSendIRCode02);
    assert(g_captured[1].payload.size() == 7);
    assert(g_captured[1].payload[0] == 0x34);
    assert(g_captured[1].payload[1] == 0x12);
    // position = 0
    assert(g_captured[1].payload[2] == 0x00);
    assert(g_captured[1].payload[3] == 0x00);
    assert(g_captured[1].payload[4] == 0x00);
    assert(g_captured[1].payload[5] == 0x00);

    // Session must have transitioned to Learn with wire seq + length.
    assert(sess.mode == ::zhc::zosung::Session::Mode::Learn);
    assert(sess.seq    == 0x1234);
    assert(sess.length == 128);
}

// fz_zosung_send_ir_code_03 — device delivers a chunk during Learn.
// Host must copy the bytes into the session buffer at the wire-given
// position, then request the next chunk via cmd 02.
static void test_fz_03_copies_chunk_into_session_buffer() {
    reset_capture();
    auto& sess = ::zhc::zosung::session_slot();
    sess.mode     = ::zhc::zosung::Session::Mode::Learn;
    sess.seq      = 0x1234;
    sess.length   = 16;
    sess.position = 0;
    std::memset(sess.buf, 0, ::zhc::zosung::Session::kBufCap);

    auto ctx = make_ctx();
    auto msg = make_zosung_msg(::zhc::zosung::cmd::kSendIRCode03);

    // cmd 03 body: zero u8 + seq u16 + position u32 + msgpart octstr
    // (1B len + bytes) + crc u8.
    // 4 bytes of payload "ABCD" at position 0; crc = sum & 0xFF.
    constexpr std::uint8_t kPart[4] = { 'A', 'B', 'C', 'D' };  // 0x41+0x42+0x43+0x44 = 0x10A → crc=0x0A
    const std::uint8_t kCrc = 0x0A;
    const std::uint8_t kBody[14] = {
        0x00,                                   // zero
        0x34, 0x12,                             // seq
        0x00, 0x00, 0x00, 0x00,                 // position = 0
        0x04,                                   // msgpart len
        kPart[0], kPart[1], kPart[2], kPart[3], // 4 B payload
        kCrc,                                   // crc
        0x00,                                   // trailing pad (ignored)
    };
    msg.raw_body = std::span<const std::uint8_t>(kBody, 13);

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    FzConverter dummy_self{};
    PreparedDefinition dummy_def{};

    const bool claimed = ::zhc::zosung::fz_zosung_send_ir_code_03(
        msg, dummy_self, dummy_def, ctx, out);
    assert(claimed);

    // Buffer must carry the 4 wire bytes at the correct offset.
    assert(std::memcmp(sess.buf, kPart, 4) == 0);
    assert(sess.position == 4);

    // Host must request the next chunk via cmd 02 (since position < length).
    assert(g_captured.size() == 1);
    assert(g_captured[0].cmd_id == ::zhc::zosung::cmd::kSendIRCode02);
}

// fz_zosung_send_ir_code_04 — device signals end-of-Send. Host replies
// with cmd 05 ack and clears the session. We assert: exactly one
// outbound command, correct cluster, correct cmd id, 4-byte payload
// (seq u16 + zero u16), session reset to Idle.
static void test_fz_04_emits_cmd_05_ack() {
    reset_capture();
    auto& sess = ::zhc::zosung::session_slot();
    sess.mode     = ::zhc::zosung::Session::Mode::Send;
    sess.seq      = 0x4243;
    sess.length   = 64;
    sess.position = 64;

    auto ctx = make_ctx();
    auto msg = make_zosung_msg(::zhc::zosung::cmd::kSendIRCode04);
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    FzConverter dummy_self{};
    PreparedDefinition dummy_def{};

    const bool claimed = ::zhc::zosung::fz_zosung_send_ir_code_04(
        msg, dummy_self, dummy_def, ctx, out);
    assert(claimed);
    assert(g_captured.size() == 1);

    const auto& f = g_captured[0];
    assert(f.cluster_id == ::zhc::zosung::kZosungIRTransmitCluster);
    assert(f.cmd_id     == ::zhc::zosung::cmd::kSendIRCode05);
    assert(f.payload.size() == 4);
    // Seq echoed back LE.
    assert(f.payload[0] == 0x43);
    assert(f.payload[1] == 0x42);
    assert(f.payload[2] == 0x00);
    assert(f.payload[3] == 0x00);

    // Session must be cleared.
    assert(sess.mode == ::zhc::zosung::Session::Mode::Idle);
    assert(sess.length   == 0);
    assert(sess.position == 0);
}

// fz_zosung_send_ir_code_04 with wrong command_id should not match.
// Defensive — the dispatcher already filters by command_id, but we
// belt-and-brace the inner check.
static void test_fz_04_rejects_wrong_cmd_id() {
    reset_capture();
    auto ctx = make_ctx();
    auto msg = make_zosung_msg(0x99);  // not 0x04
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    FzConverter dummy_self{};
    PreparedDefinition dummy_def{};

    const bool claimed = ::zhc::zosung::fz_zosung_send_ir_code_04(
        msg, dummy_self, dummy_def, ctx, out);
    assert(!claimed);
    assert(g_captured.empty());
}

int main() {
    test_fz_01_logs_only_no_emit();
    test_fz_00_emits_cmd_01_and_02();
    test_fz_03_copies_chunk_into_session_buffer();
    test_fz_04_emits_cmd_05_ack();
    test_fz_04_rejects_wrong_cmd_id();
    return 0;
}
