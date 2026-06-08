// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HOCH DIN-rail energy meter (_TZE200_hkdl5fmv, legacy hoch_din).
// CUSTOM converter: voltage/current/power arrive as PACKED multi-byte buffers
// (e.g. voltage = bytes[0:2] big-endian / 10) that the declarative DP map
// cannot express, so this is a hand-written fz that byte-slices them.
// Core readings only — state, voltage, current, power, temperature, energy,
// child_lock, countdown_timer. Threshold/leakage/fault config DPs (110-112,
// 109, 26) are skipped on the read side (complex multi-field, low value).
//
// WRITE path (z2m `legacy.toZigbee.hoch_din`): the four round-trippable
// scalar DPs are wired through the generic Tuya-DP writer
// (`tz_tuya_datapoints`, manuSpecificTuya 0xEF00 cmd setData):
//   state(DP1, bool)  child_lock(DP29, bool)
//   countdown_timer(DP9, value)  power_on_behavior(DP27, enum off/on/previous)
// z2m's write-only command DPs `trip` (DP116 hochLocking) and
// `clear_device_data` (DP115 hochClearEnergy) are momentary "do it now"
// pulses with no expose state to round-trip and are intentionally omitted.
// BEST-EFFORT, not hardware-verified.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"
#include <cstdint>
#include <span>
namespace zhc::devices::tuya {
namespace {

bool fz_hoch_din(std::span<const TuyaDpRecord> dps,
                 const DecodedMessage&,
                 const FzConverter&,
                 const PreparedDefinition&,
                 RuntimeContext&,
                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool any = false;
    auto be = [](std::span<const std::uint8_t> b) -> std::int64_t {
        std::int64_t v = 0; for (auto x : b) v = (v << 8) | x; return v; };
    auto putF = [&](const char* k, float f) {
        Value v{}; v.type = ValueType::Float; v.f = f; out.put(k, v); any = true; };
    auto putI = [&](const char* k, std::int64_t i) {
        Value v{}; v.type = ValueType::Int; v.i = i; out.put(k, v); any = true; };
    auto putB = [&](const char* k, bool b) {
        Value v{}; v.type = ValueType::Bool; v.b = b; out.put(k, v); any = true; };
    for (const auto& r : dps) {
        const auto b = r.value;
        switch (r.dp_id) {
            case 1:   if (!b.empty()) putB("state", b[b.size()-1] != 0); break;        // relay
            case 9:   putI("countdown_timer", be(b)); break;                            // DP9
            case 29:  if (!b.empty()) putB("child_lock", b[b.size()-1] != 0); break;    // DP29
            case 101: if (b.size() >= 2) putF("voltage", static_cast<float>((b[1] | (b[0] << 8))) / 10.0f); break;
            case 102: if (b.size() >= 3) putF("current", static_cast<float>((b[2] | (b[1] << 8))) / 1000.0f); break;
            case 103: if (b.size() >= 3) putF("power",   static_cast<float>((b[2] | (b[1] << 8))) / 10.0f); break;
            case 105: putI("temperature", be(b)); break;                                // DP105
            case 113: putF("energy", static_cast<float>(be(b)) / 100.0f); break;        // DP113 /100 kWh
            default: break;
        }
    }
    return any;
}

constexpr FzConverter kFzHoch{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &fz_hoch_din },
    .user_config       = nullptr,
};
const FzConverter* const kFz[]={ &::zhc::tuya::kFzTuyaMcuSyncTime, &kFzHoch };

// ── Write path ─────────────────────────────────────────────────────
// z2m `lib/legacy.ts` `dataPoints.hochRelayStatus = 27` →
// power_on_behavior with lookup {off:0, on:1, previous:2}.
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehaviorLut[] = {
    { 0, "off" }, { 1, "on" }, { 2, "previous" },
};
constexpr ::zhc::tuya::TuyaDpMapEntry kWriteEntries[] = {
    ::zhc::tuya::dp::binary(1, "state"),                 // sendDataPointBool DP1
    ::zhc::tuya::dp::numeric(9, "countdown_timer"),      // sendDataPointValue DP9
    ::zhc::tuya::dp::binary(29, "child_lock"),           // sendDataPointBool DP29
    ::zhc::tuya::dp::enum_lookup(27, "power_on_behavior",
                                 kPowerOnBehaviorLut, 3),// sendDataPointEnum DP27
};
constexpr ::zhc::tuya::TuyaDatapointMap kWriteMap{ kWriteEntries,
    sizeof(kWriteEntries) / sizeof(kWriteEntries[0]) };

// manuSpecificTuya (0xEF00) cmd 0x00 setData. key=nullptr → claims any
// key the map lists. Shares the generic encoder with every other
// DP-stream Tuya device.
constexpr TzConverter kTzHoch{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kWriteMap,
};
const TzConverter* const kTz[] = { &kTzHoch };

constexpr const char* kM[]={"TS0601"};
constexpr const char* kN[]={"_TZE200_hkdl5fmv"};
}  // namespace
extern const PreparedDefinition kDefHochDin{
    .zigbee_models=kM,.zigbee_models_count=1,.manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN,.manufacturer_names_count=1,.model="TS0601_din_hoch",
    .vendor="HOCH",.meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=2,
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),.on_event=nullptr };
}  // namespace zhc::devices::tuya
