// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Zosung IR runtime — host-side state machine for the chunked
// IR-file-transfer protocol used by Tuya / Moes UFO-R11 (TS1201) and
// Zosung-clone IR transmitter / learner devices.
//
// Wire-level reference: zigbee-herdsman-converters/src/lib/zosung.ts
// (commit ~2025-Q1). Device record: src/devices/moes.ts #UFO-R11.
//
// Two custom clusters are involved:
//
//   * zosungIRTransmit (0xED00) — bulk-transfer pipe. 6 commands
//     (00..05) form the rx/tx state machine documented below.
//   * zosungIRControl  (0xE004) — single-cmd cluster (00) carrying a
//     JSON BUFFER. {"study":0} starts learn mode; {"study":1} is the
//     ack we send back after we've collected a learned blob.
//
// State-machine summary (host = ZHAC, dev = UFO-R11):
//
//   Send IR code  (host → dev):
//      tz `ir_code_to_send` → host emits cmd 00 (seq, length).
//      dev → cmd 01 (length echo).         host: log only.
//      dev → cmd 02 (seq, position, max).  host: cmd 03 (chunk + crc).
//      dev → cmd 02 again until done.
//      dev → cmd 04 (transfer-complete).   host: cmd 05 ack +
//                                          messagesClear.
//
//   Learn IR code (dev → host):
//      tz `learn_ir_code` → host emits IRControl cmd 00 ({"study":0}).
//      dev → IRTransmit cmd 00 (seq, length). host: cmd 01 echo +
//                                              cmd 02 (start chunks).
//      dev → cmd 03Resp (chunks).          host: append to buf,
//                                          cmd 02 again until full.
//      dev → cmd 05Resp (transfer done).   host: emit
//                                          `learned_ir_code` (base64)
//                                          + IRControl cmd 00
//                                          ({"study":1}) ack.
//
// Limitations (see docs/MOES_PARITY.md):
//   * Single global session — only one in-flight IR transfer per
//     coordinator; multi-device IR-blasting is not supported in v1.
//   * Buffer cap: kSessionBufCap (256 B). Tuya UFO-R11 learned codes
//     run ~120-180 B base64; the JSON we *send* is ~80 B. Overrun
//     aborts the session and `learned_ir_code` is not emitted.
//   * No retransmit / timeout — if a chunk is dropped mid-stream the
//     session stalls until the next `tz` write reuses the slot.

#include <cstddef>
#include <cstdint>
#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::zosung {

// Cluster IDs (numeric — convenient when wiring BindingSpec).
inline constexpr std::uint16_t kZosungIRTransmitCluster = 0xED00;
inline constexpr std::uint16_t kZosungIRControlCluster  = 0xE004;

// Cluster names (string — used by FzConverter selectors).
inline constexpr const char* kZosungIRTransmitName = "zosungIRTransmit";
inline constexpr const char* kZosungIRControlName  = "zosungIRControl";

// Cluster command IDs (mirrors zosung.ts addZosungIRTransmitCluster).
namespace cmd {
inline constexpr std::uint8_t kSendIRCode00 = 0x00;
inline constexpr std::uint8_t kSendIRCode01 = 0x01;
inline constexpr std::uint8_t kSendIRCode02 = 0x02;
inline constexpr std::uint8_t kSendIRCode03 = 0x03;
inline constexpr std::uint8_t kSendIRCode04 = 0x04;
inline constexpr std::uint8_t kSendIRCode05 = 0x05;
inline constexpr std::uint8_t kControlCmd00 = 0x00;
}  // namespace cmd

// ── Per-session state ───────────────────────────────────────────────
//
// Single global slot — see file-header limitation. Lives in static
// storage because ZHC bans heap. Mutated from ANY ZCL inbound or the
// adapter's outbound TZ path; the adapter serialises both onto a
// single zigbee work queue so no extra locking is needed at this
// layer.
//
// `Mode::Idle`   — no in-flight transfer.
// `Mode::Send`   — host is feeding `out_buf` chunks to the device
//                  (driven by `ir_code_to_send` tz).
// `Mode::Learn`  — host is collecting `in_buf` chunks from the device
//                  (driven by either `learn_ir_code` tz or an
//                  unsolicited cmd 00 the device emits when it has
//                  IR data to push without prior learn enable).
struct Session {
    enum class Mode : std::uint8_t { Idle, Send, Learn };

    static constexpr std::size_t kBufCap = 256;

    Mode      mode{Mode::Idle};
    std::uint16_t seq{0};
    std::uint32_t length{0};      // expected total bytes
    std::uint32_t position{0};    // bytes consumed (Send) / written (Learn)
    std::uint8_t  buf[kBufCap]{};
};

// Returns the global session slot. Internal helper — exposed only so
// host tests can poke it.
Session& session_slot();

// ── Converters ──────────────────────────────────────────────────────
// Six fz handlers (commands 00..05) + two tz handlers (ir_code_to_send,
// learn_ir_code).

// dev → cmd 00: device announces it has IR data to push (learn or
// proactive). Host accepts by switching session to `Learn`, allocates
// `length` byte slot, emits cmd 01 (length echo) + cmd 02 (request
// first chunk). z2m-ref: zosung.ts:295-330.
bool fz_zosung_send_ir_code_00(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// dev → cmd 01: ack of host's prior cmd 00 (during Send). z2m treats
// this as a debug log only. We mirror that — no state mutation,
// returns true so dispatch knows the frame is claimed.
// z2m-ref: zosung.ts:237-248.
bool fz_zosung_send_ir_code_01(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// dev → cmd 02: device requests next chunk at `position` (max
// `maxlen`). Host extracts substring of `out_buf`, computes byte-sum
// CRC, replies with cmd 03. Active during `Send`. z2m-ref:
// zosung.ts:249-275.
bool fz_zosung_send_ir_code_02(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// dev → cmd 03Resp: device delivers a chunk during `Learn`. Host
// validates CRC, copies into `in_buf`, requests next chunk via cmd 02
// (or transitions to cmd 04 once buf is full). z2m-ref:
// zosung.ts:332-379.
bool fz_zosung_send_ir_code_03(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// dev → cmd 04: device signals the end of a `Send` transfer. Host
// replies with cmd 05 ack and clears the session. z2m-ref:
// zosung.ts:276-294.
bool fz_zosung_send_ir_code_04(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// dev → cmd 05Resp: device acknowledges the end of a `Learn`
// transfer. Host base64-encodes `in_buf`, emits `learned_ir_code`,
// fires IRControl cmd 00 ({"study":1}) ack, clears session.
// z2m-ref: zosung.ts:380-404.
bool fz_zosung_send_ir_code_05(const DecodedMessage& msg,
                                const FzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out);

// `ir_code_to_send` — inbound user write. Wraps the value (base64
// IR code) in the Tuya JSON envelope {key_num,delay,key1{...}} and
// kicks off the Send flow with cmd 00. z2m-ref: zosung.ts:407-444.
bool tz_zosung_ir_code_to_send(std::string_view key,
                                const Value& input,
                                const TzConverter& self,
                                const PreparedDefinition& def,
                                RuntimeContext& ctx,
                                std::span<std::uint8_t> out_frame,
                                std::size_t& out_size);

// `learn_ir_code` — inbound user write. Sends IRControl cmd 00 with
// payload `{"study":0}` to put the device into learn mode. The
// device replies (asynchronously, after the user presses a remote
// button) with IRTransmit cmd 00 — handled by fz_zosung_send_ir_code_00.
// z2m-ref: zosung.ts:445-459.
bool tz_zosung_learn_ir_code(std::string_view key,
                              const Value& input,
                              const TzConverter& self,
                              const PreparedDefinition& def,
                              RuntimeContext& ctx,
                              std::span<std::uint8_t> out_frame,
                              std::size_t& out_size);

// Pre-stamped converter descriptors. Devices include this header and
// reference these by pointer in their from_zigbee[] / to_zigbee[]
// arrays.
extern const FzConverter kFzZosungSendIrCode_00;
extern const FzConverter kFzZosungSendIrCode_01;
extern const FzConverter kFzZosungSendIrCode_02;
extern const FzConverter kFzZosungSendIrCode_03;
extern const FzConverter kFzZosungSendIrCode_04;
extern const FzConverter kFzZosungSendIrCode_05;

extern const TzConverter kTzZosungIrCodeToSend;
extern const TzConverter kTzZosungLearnIrCode;

// ── Helpers (exposed for testability) ───────────────────────────────

// Base64 encode `len` bytes of `in` into NUL-terminated `out` of
// capacity `out_cap`. Returns bytes written excluding NUL, or 0 on
// overflow. RFC 4648 alphabet, '=' padding, no line breaks.
std::size_t base64_encode(const std::uint8_t* in, std::size_t len,
                          char* out, std::size_t out_cap);

// Base64 decode NUL-terminated `in` into `out` (capacity `out_cap`).
// Returns bytes written, or 0 on malformed input / overflow.
std::size_t base64_decode(const char* in,
                          std::uint8_t* out, std::size_t out_cap);

// Byte-sum CRC mod 256 (matches z2m's calcArrayCrc / calcStringCrc).
std::uint8_t calc_byte_sum(const std::uint8_t* data, std::size_t len);

}  // namespace zhc::zosung
