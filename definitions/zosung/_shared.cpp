// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Zosung IR runtime — chunked-file-transfer state machine for
// Tuya / Moes UFO-R11 (TS1201) and clones.
//
// z2m-source: zigbee-herdsman-converters/src/lib/zosung.ts (commit
// ~2025-Q1). Cross-references in inline comments use line numbers
// from that file.
//
// HARDWARE TESTING REQUIRED. The state machine is faithful to z2m's
// reference but has not been exercised against a real UFO-R11 in this
// session. Treat it as "code lands & compiles, behaviour matches z2m
// on inspection" until somebody learns/sends an IR code on hardware.

#include "definitions/zosung/_shared.hpp"

#include <cstdint>
#include <cstring>
#include <span>

// dispatch.hpp carries the full `RuntimeContext` definition with the
// `configure_*` hook fields. The runtime/definition.hpp pulled in by
// our header only forward-declares it.
#include "zhc/runtime/dispatch.hpp"

namespace zhc::zosung {

namespace {

// Single global session — multi-device IR-blasting unsupported in v1.
// See header comment for trade-off rationale.
Session g_session{};

// Outbound payload chunk size matches z2m's `0x32` (50 bytes) for
// chunks the host sends to the device, and z2m requests up to `0x38`
// (56 bytes) when receiving. We use the same constants verbatim.
constexpr std::uint8_t kChunkSendMax = 0x32;   // bytes/chunk we hand back during Send
constexpr std::uint8_t kChunkRecvMax = 0x38;   // max chunk we request during Learn

// IR-control "study" payloads — JSON literals encoded once at compile
// time. {"study":0} starts learn mode (host → dev); {"study":1} acks
// a learned blob (host → dev). Lengths are explicit because the
// payload is wrapped in an OCTET_STR (length-prefixed) on the wire.
constexpr const char kStudyStartJson[] = "{\"study\":0}";
constexpr const char kStudyAckJson[]   = "{\"study\":1}";

// Common ZCL frame-control byte for our cluster-specific outbound
// commands. Frame type bit 0 set (cluster-specific), no manu-specific,
// direction client→server, disableDefaultResponse = 1 (z2m uses
// `{disableDefaultResponse: true}` everywhere — line 270, 289, 316,
// 327, 357, 367, 397, 440, 455). FC = 0b0001_0001 = 0x11.
constexpr std::uint8_t kFcCmdNoDefRsp = 0x11;

// Clamp a buffer write to the session capacity. Returns the number of
// bytes actually copied; mismatch with `len` signals overflow.
// [[maybe_unused]] until the raw-APS-access TODO lands and these
// helpers move from anticipated-use to in-use (see fz_zosung_send_ir_code_*
// TODOs).
[[maybe_unused]] std::size_t clamped_copy(std::uint8_t* dst, std::size_t dst_cap,
                          const std::uint8_t* src, std::size_t len) {
    if (len > dst_cap) len = dst_cap;
    std::memcpy(dst, src, len);
    return len;
}

// Build a buffalo-style OCTET_STR header (1 byte length prefix) +
// payload. `out` must have room for `1 + payload_len` bytes.
void write_octet_str(std::uint8_t* out, const std::uint8_t* payload,
                     std::size_t payload_len) {
    out[0] = static_cast<std::uint8_t>(payload_len);
    std::memcpy(out + 1, payload, payload_len);
}

// Fire a cluster command via the runtime ctx. Returns false when the
// hook isn't set or the platform's transport rejected it. `payload`
// is the post-header body (no FC, no TSN, no cmd_id — the bridge
// adds the FC and TSN; we still pass cmd_id explicitly). Length cap
// matches the existing `ConfigureCmdFn` contract (uint8 payload_len).
bool send_cmd(RuntimeContext& ctx,
              std::uint16_t cluster_id, std::uint8_t cmd_id,
              const std::uint8_t* payload, std::uint8_t payload_len) {
    if (!ctx.configure_cmd) return false;
    // Endpoint = 1 — UFO-R11 has a single endpoint and z2m issues all
    // frames against `endpoint(1)` (source: device record).
    return ctx.configure_cmd(ctx.device_index, /*ep=*/1,
                              cluster_id, cmd_id,
                              payload, payload_len,
                              /*flags=*/kStepFlagDisableDefaultResponse);
}

// Reads N LE bytes out of a span, bounds-checked. Returns false on OOB.
// [[maybe_unused]] until the raw-APS-access TODO lands.
[[maybe_unused]] bool read_u8 (std::span<const std::uint8_t> in, std::size_t off, std::uint8_t&  v) {
    if (off + 1 > in.size()) return false;
    v = in[off];
    return true;
}
[[maybe_unused]] bool read_u16(std::span<const std::uint8_t> in, std::size_t off, std::uint16_t& v) {
    if (off + 2 > in.size()) return false;
    v = static_cast<std::uint16_t>(in[off]) |
        (static_cast<std::uint16_t>(in[off + 1]) << 8);
    return true;
}
[[maybe_unused]] bool read_u32(std::span<const std::uint8_t> in, std::size_t off, std::uint32_t& v) {
    if (off + 4 > in.size()) return false;
    v = static_cast<std::uint32_t>(in[off]) |
        (static_cast<std::uint32_t>(in[off + 1]) << 8)  |
        (static_cast<std::uint32_t>(in[off + 2]) << 16) |
        (static_cast<std::uint32_t>(in[off + 3]) << 24);
    return true;
}

// Expose-emit helper. Emits `key` → string into `out`. The string is
// a pointer into a static buffer that lives until the next
// dispatch_from_zigbee call — caller's payload pipeline copies bytes
// before reusing the buffer.
void emit_string(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out,
                 const char* key, const char* str) {
    Value v{};
    v.type = ValueType::StringRef;
    v.str  = str;
    out.put(key, v);
}

// Static buffer for `learned_ir_code` base64 strings. 256-byte
// session buf → ~344 base64 chars + NUL. Fits comfortably.
constexpr std::size_t kLearnedB64Cap = 384;
char g_learned_b64[kLearnedB64Cap];

// JSON envelope buffer for the `ir_code_to_send` send path. The
// envelope is `{"key_num":1,"delay":300,"key1":{"num":1,"freq":38000,
// "type":1,"key_code":"<base64>"}}` — about 80 chars overhead + the
// raw IR code string. Cap matches `Session::kBufCap` since that's
// where the bytes ultimately live (we write the JSON straight into
// `Session::buf`).

}  // namespace

Session& session_slot() { return g_session; }

// ── base64 helpers ──────────────────────────────────────────────────

namespace {
constexpr char kB64Alphabet[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64_decode_char(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    if (c == '=') return -2;  // padding
    return -1;
}
}  // namespace

std::size_t base64_encode(const std::uint8_t* in, std::size_t len,
                           char* out, std::size_t out_cap) {
    if (!out || out_cap == 0) return 0;
    const std::size_t need = ((len + 2) / 3) * 4 + 1;  // +1 NUL
    if (need > out_cap) return 0;

    std::size_t pos = 0;
    std::size_t i = 0;
    while (i + 3 <= len) {
        std::uint32_t triple = (static_cast<std::uint32_t>(in[i]) << 16) |
                                (static_cast<std::uint32_t>(in[i + 1]) << 8) |
                                 static_cast<std::uint32_t>(in[i + 2]);
        out[pos++] = kB64Alphabet[(triple >> 18) & 0x3F];
        out[pos++] = kB64Alphabet[(triple >> 12) & 0x3F];
        out[pos++] = kB64Alphabet[(triple >>  6) & 0x3F];
        out[pos++] = kB64Alphabet[ triple        & 0x3F];
        i += 3;
    }
    if (i < len) {
        std::uint32_t triple = static_cast<std::uint32_t>(in[i]) << 16;
        if (i + 1 < len) triple |= static_cast<std::uint32_t>(in[i + 1]) << 8;
        out[pos++] = kB64Alphabet[(triple >> 18) & 0x3F];
        out[pos++] = kB64Alphabet[(triple >> 12) & 0x3F];
        out[pos++] = (i + 1 < len) ? kB64Alphabet[(triple >> 6) & 0x3F] : '=';
        out[pos++] = '=';
    }
    out[pos] = '\0';
    return pos;
}

std::size_t base64_decode(const char* in,
                           std::uint8_t* out, std::size_t out_cap) {
    if (!in || !out) return 0;
    std::size_t pos = 0;
    std::uint32_t buf = 0;
    int bits = 0;
    for (const char* p = in; *p; ++p) {
        char c = *p;
        if (c == '\n' || c == '\r' || c == ' ' || c == '\t') continue;
        int v = b64_decode_char(c);
        if (v == -2) break;       // padding — stop
        if (v < 0)   return 0;    // malformed
        buf = (buf << 6) | static_cast<std::uint32_t>(v);
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            if (pos >= out_cap) return 0;
            out[pos++] = static_cast<std::uint8_t>((buf >> bits) & 0xFF);
        }
    }
    return pos;
}

std::uint8_t calc_byte_sum(const std::uint8_t* data, std::size_t len) {
    std::uint32_t s = 0;
    for (std::size_t i = 0; i < len; ++i) s += data[i];
    return static_cast<std::uint8_t>(s & 0xFFu);
}

// ── fz_zosung_send_ir_code_00 ──────────────────────────────────────
//
// dev → cmd 00: device announces it has IR data to push.
// Payload (from zosung.ts:91-99):
//   seq    u16
//   length u32
//   unk1   u32
//   unk2   u16
//   unk3   u8
//   cmd    u8
//   unk4   u16   (total 16 bytes)
//
// Host: switch session to Learn, allocate `length` bytes, reply with
// cmd 01 (length echo) + cmd 02 (chunk request at position 0).

bool fz_zosung_send_ir_code_00(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    if (msg.command_id != cmd::kSendIRCode00) return false;
    // cmd 00 body (zosung.ts:91-99): seq u16 + length u32 + unk1 u32 +
    // unk2 u16 + unk3 u8 + cmd u8 + unk4 u16 = 16 bytes. Read seq +
    // length from the wire via msg.raw_body (post-ZCL-header span).
    if (msg.raw_body.size() < 16) return true;  // truncated; claim+drop
    const std::uint8_t* p = msg.raw_body.data();
    const std::uint16_t dev_seq =
        static_cast<std::uint16_t>(p[0] | (static_cast<std::uint16_t>(p[1]) << 8));
    const std::uint32_t dev_length =
        static_cast<std::uint32_t>(p[2])
      | (static_cast<std::uint32_t>(p[3]) << 8)
      | (static_cast<std::uint32_t>(p[4]) << 16)
      | (static_cast<std::uint32_t>(p[5]) << 24);
    if (dev_length == 0 || dev_length > Session::kBufCap) return true;

    g_session.mode     = Session::Mode::Learn;
    g_session.seq      = dev_seq;
    g_session.length   = dev_length;
    g_session.position = 0;
    std::memset(g_session.buf, 0, dev_length);

    // cmd 01 echo: leading zero byte + the 16-byte cmd 00 body
    // verbatim. Devices reject seq/length mismatches so we mirror what
    // they sent; z2m does the same (zosung.ts:295-330).
    std::uint8_t pl01[17];
    pl01[0] = 0;
    std::memcpy(&pl01[1], p, 16);
    if (!send_cmd(ctx, kZosungIRTransmitCluster, cmd::kSendIRCode01,
                   pl01, sizeof(pl01))) {
        return true;  // protocol abort, not a parser bug
    }

    // cmd 02 chunk request at position 0 with our seq.
    std::uint8_t pl02[7] = {0};
    pl02[0] = static_cast<std::uint8_t>(g_session.seq & 0xFF);
    pl02[1] = static_cast<std::uint8_t>(g_session.seq >> 8);
    pl02[2] = pl02[3] = pl02[4] = pl02[5] = 0;
    pl02[6] = kChunkRecvMax;
    send_cmd(ctx, kZosungIRTransmitCluster, cmd::kSendIRCode02,
              pl02, sizeof(pl02));
    return true;
}

// ── fz_zosung_send_ir_code_01 ──────────────────────────────────────
//
// dev → cmd 01: ack of host's prior cmd 00 during Send. z2m logs
// only — we mirror that. Returns true to claim the frame so the
// adapter's unhandled-frame hook doesn't fire.
bool fz_zosung_send_ir_code_01(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    if (msg.command_id != cmd::kSendIRCode01) return false;
    return true;  // z2m-ref: zosung.ts:237-248 — debug log only.
}

// ── fz_zosung_send_ir_code_02 ──────────────────────────────────────
//
// dev → cmd 02: chunk request at `position`. Host extracts substring
// of `out_buf`, computes CRC, replies with cmd 03.
// Payload (zosung.ts:115-122):
//   seq      u16
//   position u32
//   maxlen   u8       (total 7 bytes)
//
// Reads the device's requested `position` from the wire so we serve
// the exact chunk it asked for (devices may re-request earlier
// positions on retry).

bool fz_zosung_send_ir_code_02(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    if (msg.command_id != cmd::kSendIRCode02) return false;
    if (g_session.mode != Session::Mode::Send) return true;  // not for us
    if (msg.raw_body.size() < 7) return true;                // truncated

    // cmd 02 body: seq u16 + position u32 + maxlen u8.
    const std::uint8_t* p = msg.raw_body.data();
    const std::uint32_t dev_pos =
        static_cast<std::uint32_t>(p[2])
      | (static_cast<std::uint32_t>(p[3]) << 8)
      | (static_cast<std::uint32_t>(p[4]) << 16)
      | (static_cast<std::uint32_t>(p[5]) << 24);
    const std::uint8_t  dev_maxlen = p[6];
    if (dev_pos >= g_session.length) return true;  // out of range
    g_session.position = dev_pos;
    const std::size_t cap_max =
        dev_maxlen < kChunkSendMax ? dev_maxlen : kChunkSendMax;

    const std::size_t avail  = g_session.length - g_session.position;
    const std::size_t n      = avail < cap_max ? avail : cap_max;

    // cmd 03 reply: zero u8 + seq u16 + position u32 + msgpart octstr
    // (1B len prefix + n bytes) + msgpartcrc u8.
    // Total = 1 + 2 + 4 + 1 + n + 1 = 9 + n.
    std::uint8_t pl03[9 + kChunkSendMax];
    pl03[0] = 0;                                            // zero
    pl03[1] = static_cast<std::uint8_t>(g_session.seq & 0xFF);
    pl03[2] = static_cast<std::uint8_t>(g_session.seq >> 8);
    pl03[3] = static_cast<std::uint8_t>(g_session.position       & 0xFF);
    pl03[4] = static_cast<std::uint8_t>((g_session.position >>  8) & 0xFF);
    pl03[5] = static_cast<std::uint8_t>((g_session.position >> 16) & 0xFF);
    pl03[6] = static_cast<std::uint8_t>((g_session.position >> 24) & 0xFF);
    write_octet_str(&pl03[7], &g_session.buf[g_session.position], n);
    pl03[8 + n] = calc_byte_sum(&g_session.buf[g_session.position], n);

    send_cmd(ctx, kZosungIRTransmitCluster, cmd::kSendIRCode03,
              pl03, static_cast<std::uint8_t>(9 + n));
    g_session.position += n;
    return true;
}

// ── fz_zosung_send_ir_code_03 ──────────────────────────────────────
//
// dev → cmd 03Resp: chunk delivery during Learn. Host validates CRC,
// copies into in-buf, requests next chunk via cmd 02 OR transitions to
// cmd 04 once the buf is full.
// Payload (zosung.ts:124-133):
//   zero       u8
//   seq        u16
//   position   u32
//   msgpart    OCTET_STR (1B len + bytes)
//   msgpartcrc u8
//
// Reads position + msgpart from the wire so we know exactly which
// bytes go where. msgpartcrc is checked against calc_byte_sum; on
// mismatch the chunk is dropped and the same position re-requested
// (device should retransmit on next cycle).

bool fz_zosung_send_ir_code_03(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    if (msg.command_id != cmd::kSendIRCode03) return false;
    if (g_session.mode != Session::Mode::Learn) return true;

    // cmd 03 body: zero u8 + seq u16 + position u32 + msgpart octstr
    // (1B len + bytes) + msgpartcrc u8. Minimum legal length is 9
    // bytes (zero + seq + position + 1B prefix=0 + crc).
    if (msg.raw_body.size() < 9) return true;
    const std::uint8_t* p = msg.raw_body.data();
    const std::uint32_t dev_pos =
        static_cast<std::uint32_t>(p[3])
      | (static_cast<std::uint32_t>(p[4]) << 8)
      | (static_cast<std::uint32_t>(p[5]) << 16)
      | (static_cast<std::uint32_t>(p[6]) << 24);
    const std::uint8_t  msgpart_len = p[7];
    if (msg.raw_body.size() < std::size_t(8) + msgpart_len + 1) return true;
    const std::uint8_t* msgpart = &p[8];
    const std::uint8_t  msgpart_crc = p[8 + msgpart_len];

    // CRC validate; drop chunk on mismatch (re-request same pos).
    if (calc_byte_sum(msgpart, msgpart_len) != msgpart_crc) {
        // Re-request same position; skip the position-advance below.
    } else if (dev_pos < g_session.length && msgpart_len > 0) {
        const std::size_t copy_n =
            (dev_pos + msgpart_len <= g_session.length)
                ? msgpart_len
                : (g_session.length - dev_pos);
        std::memcpy(&g_session.buf[dev_pos], msgpart, copy_n);
        g_session.position = dev_pos + copy_n;
    }

    if (g_session.position < g_session.length) {
        // Request next chunk.
        std::uint8_t pl02[7] = {0};
        pl02[0] = static_cast<std::uint8_t>(g_session.seq & 0xFF);
        pl02[1] = static_cast<std::uint8_t>(g_session.seq >> 8);
        pl02[2] = static_cast<std::uint8_t>(g_session.position       & 0xFF);
        pl02[3] = static_cast<std::uint8_t>((g_session.position >>  8) & 0xFF);
        pl02[4] = static_cast<std::uint8_t>((g_session.position >> 16) & 0xFF);
        pl02[5] = static_cast<std::uint8_t>((g_session.position >> 24) & 0xFF);
        pl02[6] = kChunkRecvMax;
        send_cmd(ctx, kZosungIRTransmitCluster, cmd::kSendIRCode02,
                  pl02, sizeof(pl02));
    } else {
        // Buffer full — emit cmd 04 transfer-complete.
        std::uint8_t pl04[5] = {0};
        pl04[0] = 0;                                   // zero0
        pl04[1] = static_cast<std::uint8_t>(g_session.seq & 0xFF);
        pl04[2] = static_cast<std::uint8_t>(g_session.seq >> 8);
        pl04[3] = 0; pl04[4] = 0;                      // zero1
        send_cmd(ctx, kZosungIRTransmitCluster, cmd::kSendIRCode04,
                  pl04, sizeof(pl04));
    }
    return true;
}

// ── fz_zosung_send_ir_code_04 ──────────────────────────────────────
//
// dev → cmd 04: device signals end of a Send transfer. Reply with cmd
// 05 ack + clear session.
// Payload (zosung.ts:135-142):
//   zero0 u8
//   seq   u16
//   zero1 u16     (5 bytes)

bool fz_zosung_send_ir_code_04(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    if (msg.command_id != cmd::kSendIRCode04) return false;

    std::uint8_t pl05[4] = {0};
    pl05[0] = static_cast<std::uint8_t>(g_session.seq & 0xFF);
    pl05[1] = static_cast<std::uint8_t>(g_session.seq >> 8);
    pl05[2] = 0; pl05[3] = 0;     // zero
    send_cmd(ctx, kZosungIRTransmitCluster, cmd::kSendIRCode05,
              pl05, sizeof(pl05));

    g_session.mode     = Session::Mode::Idle;
    g_session.position = 0;
    g_session.length   = 0;
    return true;
}

// ── fz_zosung_send_ir_code_05 ──────────────────────────────────────
//
// dev → cmd 05Resp: device acks end of Learn transfer. Host
// base64-encodes the buf, emits `learned_ir_code`, fires IRControl
// cmd 00 ({"study":1}) ack, clears session.
// Payload (zosung.ts:144-150):
//   seq  u16
//   zero u16    (4 bytes)

bool fz_zosung_send_ir_code_05(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.command_id != cmd::kSendIRCode05) return false;
    if (g_session.mode != Session::Mode::Learn) return true;

    // Base64-encode the now-populated session buffer and emit
    // `learned_ir_code`. fz_03 fills `g_session.buf` chunk-by-chunk
    // using the wire `position` + `msgpart` fields; by the time we
    // reach cmd 05 the buffer holds the full IR blob.
    base64_encode(g_session.buf, g_session.length,
                  g_learned_b64, kLearnedB64Cap);
    emit_string(out, "learned_ir_code", g_learned_b64);

    // IRControl cmd 00 ack: OCTET_STR-wrapped JSON {"study":1}.
    constexpr std::size_t kAckLen = sizeof(kStudyAckJson) - 1;
    std::uint8_t pl_ack[1 + kAckLen];
    pl_ack[0] = static_cast<std::uint8_t>(kAckLen);
    std::memcpy(&pl_ack[1], kStudyAckJson, kAckLen);
    send_cmd(ctx, kZosungIRControlCluster, cmd::kControlCmd00,
              pl_ack, sizeof(pl_ack));

    g_session.mode     = Session::Mode::Idle;
    g_session.position = 0;
    g_session.length   = 0;
    return true;
}

// ── tz_zosung_ir_code_to_send ──────────────────────────────────────
//
// Inbound `ir_code_to_send` write. Wraps the value (assumed base64 IR
// code string) in the Tuya JSON envelope and emits cmd 00 to start
// the Send flow.
// z2m-ref: zosung.ts:407-444.

bool tz_zosung_ir_code_to_send(std::string_view key,
                                const Value& input,
                                const TzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                std::span<std::uint8_t> out_frame,
                                std::size_t& out_size) {
    out_size = 0;
    if (key != "ir_code_to_send") return false;
    if (input.type != ValueType::StringRef || !input.str) return false;

    // Build the JSON envelope directly into the session buffer:
    //   {"key_num":1,"delay":300,"key1":{"num":1,"freq":38000,
    //   "type":1,"key_code":"<base64>"}}
    const char* code = input.str;
    const std::size_t code_len = std::strlen(code);

    char* out  = reinterpret_cast<char*>(g_session.buf);
    char* end  = out + Session::kBufCap;
    auto put = [&out, end](const char* s) {
        while (*s && out < end) *out++ = *s++;
    };
    put("{\"key_num\":1,\"delay\":300,\"key1\":{\"num\":1,\"freq\":38000,\"type\":1,\"key_code\":\"");
    if (out + code_len + 3 >= end) return false;  // overflow ("}}", ", NUL)
    std::memcpy(out, code, code_len);
    out += code_len;
    put("\"}}");
    if (out >= end) return false;
    *out = '\0';
    const std::size_t total = static_cast<std::size_t>(
        out - reinterpret_cast<char*>(g_session.buf));

    g_session.mode     = Session::Mode::Send;
    g_session.seq      = static_cast<std::uint16_t>((g_session.seq + 1) & 0xFFFF);
    g_session.length   = static_cast<std::uint32_t>(total);
    g_session.position = 0;

    // Encode cmd 00 into the outbound frame so the existing TZ pipe
    // (dispatch_to_zigbee → g_af_send) ships it. Header + payload:
    //
    //   FC(0x11) TSN(00) cmdID(0x00) seq(u16) length(u32) unk1(u32)
    //   unk2(u16) unk3(u8) cmd(u8) unk4(u16) = 3 + 16 = 19 bytes
    //
    // z2m-ref: zosung.ts:428-441 — sets unk1=0x00000000, unk2=0xe004,
    // unk3=0x01, cmd=0x02, unk4=0x0000.
    if (out_frame.size() < 19) return false;
    out_frame[0] = kFcCmdNoDefRsp;
    out_frame[1] = 0x00;   // TSN — patched by send bridge
    out_frame[2] = cmd::kSendIRCode00;
    out_frame[3] = static_cast<std::uint8_t>(g_session.seq & 0xFF);
    out_frame[4] = static_cast<std::uint8_t>(g_session.seq >> 8);
    out_frame[5] = static_cast<std::uint8_t>(g_session.length        & 0xFF);
    out_frame[6] = static_cast<std::uint8_t>((g_session.length >>  8) & 0xFF);
    out_frame[7] = static_cast<std::uint8_t>((g_session.length >> 16) & 0xFF);
    out_frame[8] = static_cast<std::uint8_t>((g_session.length >> 24) & 0xFF);
    out_frame[9]  = 0x00; out_frame[10] = 0x00;
    out_frame[11] = 0x00; out_frame[12] = 0x00;             // unk1 = 0
    out_frame[13] = 0x04; out_frame[14] = 0xE0;             // unk2 = 0xE004 LE
    out_frame[15] = 0x01;                                   // unk3
    out_frame[16] = 0x02;                                   // cmd
    out_frame[17] = 0x00; out_frame[18] = 0x00;             // unk4
    out_size = 19;
    return true;
}

// ── tz_zosung_learn_ir_code ────────────────────────────────────────
//
// Inbound `learn_ir_code` write. Sends IRControl cmd 00 with payload
// `{"study":0}` to put the device into learn mode.
// z2m-ref: zosung.ts:445-459.

bool tz_zosung_learn_ir_code(std::string_view key,
                              const Value& input,
                              const TzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              std::span<std::uint8_t> out_frame,
                              std::size_t& out_size) {
    out_size = 0;
    if (key != "learn_ir_code") return false;
    // z2m doesn't gate on the value — any write triggers learn mode
    // (UI sends "ON"). We accept any input shape.
    (void)input;

    // Frame: FC(0x11) TSN(00) cmdID(0x00) + OCTET_STR("{\"study\":0}")
    constexpr std::size_t kJsonLen = sizeof(kStudyStartJson) - 1;
    if (out_frame.size() < 3 + 1 + kJsonLen) return false;
    out_frame[0] = kFcCmdNoDefRsp;
    out_frame[1] = 0x00;
    out_frame[2] = cmd::kControlCmd00;
    out_frame[3] = static_cast<std::uint8_t>(kJsonLen);
    std::memcpy(&out_frame[4], kStudyStartJson, kJsonLen);
    out_size = 3 + 1 + kJsonLen;
    return true;
}

// ── Pre-stamped converter descriptors ──────────────────────────────

extern const FzConverter kFzZosungSendIrCode_00{
    .family            = FrameFamily::Zcl,
    .cluster           = kZosungIRTransmitName,
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = cmd::kSendIRCode00,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zosung_send_ir_code_00 },
    .user_config       = nullptr,
};

extern const FzConverter kFzZosungSendIrCode_01{
    .family            = FrameFamily::Zcl,
    .cluster           = kZosungIRTransmitName,
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = cmd::kSendIRCode01,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zosung_send_ir_code_01 },
    .user_config       = nullptr,
};

extern const FzConverter kFzZosungSendIrCode_02{
    .family            = FrameFamily::Zcl,
    .cluster           = kZosungIRTransmitName,
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = cmd::kSendIRCode02,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zosung_send_ir_code_02 },
    .user_config       = nullptr,
};

extern const FzConverter kFzZosungSendIrCode_03{
    .family            = FrameFamily::Zcl,
    .cluster           = kZosungIRTransmitName,
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = cmd::kSendIRCode03,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zosung_send_ir_code_03 },
    .user_config       = nullptr,
};

extern const FzConverter kFzZosungSendIrCode_04{
    .family            = FrameFamily::Zcl,
    .cluster           = kZosungIRTransmitName,
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = cmd::kSendIRCode04,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zosung_send_ir_code_04 },
    .user_config       = nullptr,
};

extern const FzConverter kFzZosungSendIrCode_05{
    .family            = FrameFamily::Zcl,
    .cluster           = kZosungIRTransmitName,
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = cmd::kSendIRCode05,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_zosung_send_ir_code_05 },
    .user_config       = nullptr,
};

extern const TzConverter kTzZosungIrCodeToSend{
    .key          = "ir_code_to_send",
    .cluster      = kZosungIRTransmitName,
    .cluster_id   = kZosungIRTransmitCluster,
    .command_id   = cmd::kSendIRCode00,
    .fn           = tz_zosung_ir_code_to_send,
    .user_config  = nullptr,
};

extern const TzConverter kTzZosungLearnIrCode{
    .key          = "learn_ir_code",
    .cluster      = kZosungIRControlName,
    .cluster_id   = kZosungIRControlCluster,
    .command_id   = cmd::kControlCmd00,
    .fn           = tz_zosung_learn_ir_code,
    .user_config  = nullptr,
};

}  // namespace zhc::zosung
