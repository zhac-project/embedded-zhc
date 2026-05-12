// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <cstddef>
#include <cstdint>

#include "zhc/platform.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/runtime/timer.hpp"
#include "zhc/types.hpp"

namespace zhc {

// Type-erased runtime context passed to every converter and hook.
// Wall-clock comes from `now_ms` — null-safe: converters fall back to 0
// if the platform didn't supply one, making pure-functional tests easy.
//
// Tuya-specific: `tuya_sync_time` is called when the decoder sees a
// `manuSpecificTuya` (0xEF00) cmd 0x24 (`mcuSyncTime`) request. Some
// Tuya devices hang on join until they receive a reply carrying Unix
// epoch seconds. The adapter populates the hook; the core fires it
// via the generic `fz_tuya_mcu_sync_time` converter. Leave null on
// platforms that don't care.
using TuyaSyncTimeReplyFn = void (*)(std::uint16_t device_index,
                                      std::uint8_t trans_seq);

// Platform hook for ZDO_BIND_REQ. `coord_ieee` / `device_ieee` /
// `device_nwk` live platform-side; the zhc core only passes back the
// endpoint + cluster it wants bound. Returns false on transport
// failure — callers treat that as "retry at next join".
using ConfigureBindFn = bool (*)(std::uint16_t device_index,
                                  std::uint8_t  endpoint,
                                  std::uint16_t cluster_id);

// Platform hook for configureReporting. One attribute per call — the
// radio layer coalesces if needed.
using ConfigureReportFn = bool (*)(std::uint16_t device_index,
                                    std::uint8_t  endpoint,
                                    std::uint16_t cluster_id,
                                    std::uint16_t attr_id,
                                    std::uint8_t  attr_type,
                                    std::uint16_t min_interval,
                                    std::uint16_t max_interval,
                                    std::uint32_t reportable_change,
                                    std::uint16_t manufacturer_code);

// Platform hook for cluster-specific commands issued during configure.
// `flags` carries `kStepFlag*` bits from the step; the platform uses
// them to set the ZCL FC byte. `payload` points at the command body
// that follows the CmdID — it's opaque to zhc and written verbatim.
using ConfigureCmdFn = bool (*)(std::uint16_t device_index,
                                 std::uint8_t  endpoint,
                                 std::uint16_t cluster_id,
                                 std::uint8_t  cmd_id,
                                 const std::uint8_t* payload,
                                 std::uint8_t  payload_len,
                                 std::uint8_t  flags);

// Platform hook for ZCL Read Attributes issued during configure.
// `attr_ids` is packed LE (two bytes per id). `manu_code` 0 = profile-
// wide; non-zero = manufacturer-specific frame.
using ConfigureReadFn = bool (*)(std::uint16_t device_index,
                                  std::uint8_t  endpoint,
                                  std::uint16_t cluster_id,
                                  const std::uint8_t* attr_ids_le,
                                  std::uint8_t  attr_id_count,
                                  std::uint16_t manu_code);

// Platform sleep hook used by `ConfigStepOp::Wait`. Null → steps of
// that op type are silently skipped (no-op on tiny hosts without a
// scheduler).
using ConfigureSleepFn = void (*)(std::uint16_t wait_ms);

struct RuntimeContext {
    NowMsFn              now_ms;
    LogFn                log;
    UnhandledFrameFn     unhandled;
    void*                store;
    StoreGetFn           store_get;
    TimerScheduler*      timers;          // optional — nullable on platforms without timers
    TuyaSyncTimeReplyFn  tuya_sync_time;    // optional — Tuya-only
    ConfigureBindFn      configure_bind;    // optional — join-time bind
    ConfigureReportFn    configure_report;  // optional — join-time report setup
    ConfigureCmdFn       configure_cmd;     // optional — join-time ZCL command
    ConfigureReadFn      configure_read;    // optional — join-time ZCL read
    ConfigureSleepFn     configure_sleep;   // optional — Wait-op hook
    std::uint16_t        device_nwk;        // populated at run_configure time for Callback op
    std::uint16_t        device_index;

    // Multi-endpoint suffix arena. Used by `dispatch_from_zigbee` to
    // build per-EP-suffixed keys (e.g. `state_l2`) for devices whose
    // PreparedDefinition opts in via `endpoint_map`. The arena keeps
    // emitted-key strings alive for the lifetime of the returned
    // `DispatchResult`. `ep_scratch_used` is reset implicitly because
    // RuntimeContext is constructed fresh per inbound dispatch.
    char                  ep_scratch[256]{};
    std::uint16_t         ep_scratch_used = 0;

    DeviceRuntimeState* device_state() {
        return (store && store_get) ? store_get(store, device_index) : nullptr;
    }

    std::uint32_t now() const { return now_ms ? now_ms() : 0; }
};

struct DispatchResult {
    bool any_matched{false};
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> merged{};
};

// Walk every `FzConverter` in `def.from_zigbee` in array order. On match,
// invoke the converter; merge its output into `result.merged` in call
// order (last-writer-wins on key collision). If zero converters match
// and `ctx.unhandled` is set, fire the unhandled-frame hook.
//
// `tuya_dps` is consulted only when `msg.family == FrameFamily::TuyaDp`;
// callers that never see Tuya frames can pass an empty span.
DispatchResult dispatch_from_zigbee(const DecodedMessage& msg,
                                     std::span<const TuyaDpRecord> tuya_dps,
                                     const PreparedDefinition& def,
                                     const InboundApsFrame& raw_frame,
                                     RuntimeContext& ctx);

// Result of encoding a write-path value into an outbound APS frame.
struct TzDispatchResult {
    bool          ok{false};
    std::uint16_t cluster_id{0};
    std::uint16_t command_id{0};
    std::size_t   frame_size{0};
};

// Walk `def.to_zigbee` looking for a TzConverter that claims `key`.
// On match, invoke the converter's encoder which writes a ZCL header +
// payload into `out_frame`. Returns the cluster/command ids caller uses
// to build the APS wrapper. Returns `ok=false` if no converter claims
// the key or the encoder fails.
TzDispatchResult dispatch_to_zigbee(const PreparedDefinition& def,
                                     std::string_view key,
                                     const Value& input,
                                     RuntimeContext& ctx,
                                     std::span<std::uint8_t> out_frame);

// Walk `def.bindings[]` + `def.reports[]` and fire the matching
// platform hooks. Invoked by the adapter at device join. Stops at
// first transport failure so subsequent attempts don't pile up.
// Returns true when every spec was accepted.
bool run_configure(const PreparedDefinition& def, RuntimeContext& ctx);

}  // namespace zhc
