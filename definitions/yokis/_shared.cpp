// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Yokis manuSpecificYokis* runtime: one Fz decoder reused
// across the seven Yokis clusters, plus per-attribute Tz writers
// that delegate to the generic ZclWriteSpec encoder (which already
// stamps fc=0x14 + manufacturer-code bytes when manufacturer_code
// is non-zero).
//
// z2m-source: zigbee-herdsman-converters/src/devices/yokis.ts.

#include "definitions/yokis/_shared.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string_view>

namespace zhc::yokis {

// ── fz_yokis_manu_specific ──────────────────────────────────────
//
// Generic attribute-report decoder. The cluster identity is taken
// from `self.cluster` so the same fn serves every YokisXxx cluster.
// Attr lookup is keyed by stringified attribute id (the runtime's
// payload format already keys ZCL attrs by their decimal id).

bool fz_yokis_manu_specific(const DecodedMessage& msg,
                             const FzConverter& self,
                             const PreparedDefinition&,
                             RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* map = static_cast<const YokisAttrMap*>(self.user_config);
    if (!map || !map->entries || map->count == 0) return false;

    bool emitted = false;
    for (std::uint8_t i = 0; i < map->count; ++i) {
        const auto& e = map->entries[i];
        char keybuf[8];
        std::snprintf(keybuf, sizeof(keybuf), "%u",
                      static_cast<unsigned>(e.attr_id));
        const Value* v = msg.payload.find(keybuf);
        if (!v) continue;

        Value o{};
        switch (e.type) {
            case YokisAttrType::Raw:
                o = *v;
                break;
            case YokisAttrType::Bool: {
                bool b = false;
                if      (v->type == ValueType::Bool) b = v->b;
                else if (v->type == ValueType::Uint) b = v->u != 0;
                else if (v->type == ValueType::Int)  b = v->i != 0;
                else continue;
                o.type = ValueType::Bool; o.b = b;
                break;
            }
            case YokisAttrType::Enum: {
                if (v->type != ValueType::Uint || !e.enum_table) continue;
                const char* label = nullptr;
                for (std::uint8_t j = 0; j < e.enum_count; ++j) {
                    if (e.enum_table[j].value == v->u) {
                        label = e.enum_table[j].label; break;
                    }
                }
                if (!label) continue;
                o.type = ValueType::StringRef; o.str = label;
                break;
            }
        }
        out.put(e.out_key, o);
        emitted = true;
    }
    return emitted;
}

// ── Per-cluster YokisAttrMap definitions ────────────────────────

namespace {

// LightControl (0xFC06) — every reportable knob the SPA cares about.
constexpr YokisAttrEnumEntry kStateAfterBlinkLut[] = {
    { 0x00, "previous" }, { 0x01, "off" }, { 0x02, "on" }, { 0x03, "infinite" },
};
constexpr YokisAttrEntry kLightControlEntries[] = {
    { 0x0000, "yokis_on_off",         YokisAttrType::Bool, nullptr, 0 },
    { 0x0001, "yokis_prev_state",     YokisAttrType::Bool, nullptr, 0 },
    { 0x0002, "on_timer",             YokisAttrType::Raw,  nullptr, 0 },
    { 0x0003, "e_on_timer",           YokisAttrType::Bool, nullptr, 0 },
    { 0x0004, "pre_on_delay",         YokisAttrType::Raw,  nullptr, 0 },
    { 0x0005, "e_pre_on_delay",       YokisAttrType::Bool, nullptr, 0 },
    { 0x0008, "pre_off_delay",        YokisAttrType::Raw,  nullptr, 0 },
    { 0x0009, "e_pre_off_delay",      YokisAttrType::Bool, nullptr, 0 },
    { 0x000A, "pulse_duration",       YokisAttrType::Raw,  nullptr, 0 },
    { 0x000C, "long_on_duration",     YokisAttrType::Raw,  nullptr, 0 },
    { 0x0014, "e_stop_announce",      YokisAttrType::Bool, nullptr, 0 },
    { 0x0015, "e_deaf",               YokisAttrType::Bool, nullptr, 0 },
    { 0x0016, "e_blink",              YokisAttrType::Bool, nullptr, 0 },
    { 0x0017, "blink_amount",         YokisAttrType::Raw,  nullptr, 0 },
    { 0x0018, "blink_on_time",        YokisAttrType::Raw,  nullptr, 0 },
    { 0x0019, "blink_off_time",       YokisAttrType::Raw,  nullptr, 0 },
    { 0x001C, "state_after_blink",    YokisAttrType::Enum,
      kStateAfterBlinkLut, 4 },
    { 0x001D, "e_nc_command",         YokisAttrType::Bool, nullptr, 0 },
};

// Dimmer (0xFC07).
constexpr YokisAttrEntry kDimmerEntries[] = {
    { 0x0000, "yokis_brightness",     YokisAttrType::Raw,  nullptr, 0 },
    { 0x0001, "memory_position",      YokisAttrType::Raw,  nullptr, 0 },
    { 0x0002, "e_ramp_up",            YokisAttrType::Bool, nullptr, 0 },
    { 0x0003, "ramp_up",              YokisAttrType::Raw,  nullptr, 0 },
    { 0x0004, "e_ramp_down",          YokisAttrType::Bool, nullptr, 0 },
    { 0x0005, "ramp_down",            YokisAttrType::Raw,  nullptr, 0 },
    { 0x0007, "step_up",              YokisAttrType::Raw,  nullptr, 0 },
    { 0x0008, "low_dim_limit",        YokisAttrType::Raw,  nullptr, 0 },
    { 0x0009, "high_dim_limit",       YokisAttrType::Raw,  nullptr, 0 },
    { 0x0016, "step_down",            YokisAttrType::Raw,  nullptr, 0 },
};

// PilotWire (0xFC0A) — actualOrder is the live heating mode.
constexpr YokisAttrEnumEntry kPilotWireOrderLut[] = {
    { 0x00, "stop" },     { 0x01, "frost_off" },
    { 0x02, "eco" },      { 0x03, "confort_2" },
    { 0x04, "confort_1" },{ 0x05, "confort" },
};
constexpr YokisAttrEntry kPilotWireEntries[] = {
    { 0x0000, "actual_order",         YokisAttrType::Enum,
      kPilotWireOrderLut, 6 },
    { 0x0001, "order_timer",          YokisAttrType::Raw,  nullptr, 0 },
    { 0x0002, "pre_order_timer",      YokisAttrType::Raw,  nullptr, 0 },
    { 0x0004, "led_mode",             YokisAttrType::Raw,  nullptr, 0 },
    { 0x0008, "fallback_order",       YokisAttrType::Enum,
      kPilotWireOrderLut, 6 },
};

// Input (0xFC02).
constexpr YokisAttrEnumEntry kInputModeLut[] = {
    { 0x00, "unknown" }, { 0x01, "push_button" }, { 0x02, "switch" },
    { 0x03, "relay" },   { 0x04, "fp_in" },
};
constexpr YokisAttrEntry kInputEntries[] = {
    { 0x0000, "input_mode",           YokisAttrType::Enum,
      kInputModeLut, 5 },
    { 0x0001, "contact_mode",         YokisAttrType::Bool, nullptr, 0 },
    { 0x0002, "last_local_state",     YokisAttrType::Bool, nullptr, 0 },
    { 0x0003, "last_bp_connect",      YokisAttrType::Bool, nullptr, 0 },
    { 0x0004, "backlight_intensity",  YokisAttrType::Raw,  nullptr, 0 },
};

// Entry / Configurator (0xFC03).
constexpr YokisAttrEntry kEntryEntries[] = {
    { 0x0001, "e_short_press",        YokisAttrType::Bool, nullptr, 0 },
    { 0x0002, "e_long_press",         YokisAttrType::Bool, nullptr, 0 },
    { 0x0003, "long_press_duration",  YokisAttrType::Raw,  nullptr, 0 },
    { 0x0004, "time_between_press",   YokisAttrType::Raw,  nullptr, 0 },
    { 0x0005, "e_r12m_long_press",    YokisAttrType::Bool, nullptr, 0 },
    { 0x0006, "e_local_config_lock",  YokisAttrType::Bool, nullptr, 0 },
};

// Channel (0xFC09) — routing config; expose the most-used few.
constexpr YokisAttrEntry kChannelEntries[] = {
    { 0x0000, "on_off_cluster_mode",  YokisAttrType::Raw,  nullptr, 0 },
    { 0x0001, "level_cluster_mode",   YokisAttrType::Raw,  nullptr, 0 },
    { 0x0002, "wcov_cluster_mode",    YokisAttrType::Raw,  nullptr, 0 },
    { 0x0003, "cluster_to_be_used",   YokisAttrType::Raw,  nullptr, 0 },
    { 0x0004, "sending_mode",         YokisAttrType::Raw,  nullptr, 0 },
    { 0x0007, "yokis_group_id",       YokisAttrType::Raw,  nullptr, 0 },
};

// SubSystem (0xFC04).
constexpr YokisAttrEnumEntry kPowerFailureLut[] = {
    { 0x00, "off" }, { 0x01, "on" }, { 0x02, "previous" },
};
constexpr YokisAttrEntry kSubSystemEntries[] = {
    { 0x0001, "power_failure_mode",   YokisAttrType::Enum,
      kPowerFailureLut, 3 },
};

}  // namespace

extern const YokisAttrMap kYokisLightControlMap{
    kLightControlEntries,
    sizeof(kLightControlEntries) / sizeof(kLightControlEntries[0]),
};
extern const YokisAttrMap kYokisDimmerMap{
    kDimmerEntries,
    sizeof(kDimmerEntries) / sizeof(kDimmerEntries[0]),
};
extern const YokisAttrMap kYokisPilotWireMap{
    kPilotWireEntries,
    sizeof(kPilotWireEntries) / sizeof(kPilotWireEntries[0]),
};
extern const YokisAttrMap kYokisInputMap{
    kInputEntries,
    sizeof(kInputEntries) / sizeof(kInputEntries[0]),
};
extern const YokisAttrMap kYokisEntryMap{
    kEntryEntries,
    sizeof(kEntryEntries) / sizeof(kEntryEntries[0]),
};
extern const YokisAttrMap kYokisChannelMap{
    kChannelEntries,
    sizeof(kChannelEntries) / sizeof(kChannelEntries[0]),
};
extern const YokisAttrMap kYokisSubSystemMap{
    kSubSystemEntries,
    sizeof(kSubSystemEntries) / sizeof(kSubSystemEntries[0]),
};

// Macro stamping out one FzConverter per cluster, all pointing at
// the same decoder with their own map + cluster name.
#define ZHC_YOKIS_FZ(var, cluster_str, map_ref)                       \
    extern const FzConverter var{                                     \
        .family            = FrameFamily::Zcl,                        \
        .cluster           = cluster_str,                             \
        .type_mask         = type_bit(MessageType::AttributeReport) | \
                             type_bit(MessageType::ReadResponse),     \
        .command_id        = WILDCARD_CMD_ID,                         \
        .attr_id           = WILDCARD_ATTR_ID,                        \
        .endpoint          = WILDCARD_ENDPOINT,                       \
        .frame_flags_mask  = 0, .frame_flags_value = 0,               \
        .direction         = Direction::ServerToClient,               \
        .fn                = { .zcl_fn = &fz_yokis_manu_specific },   \
        .user_config       = &map_ref,                                \
    };
ZHC_YOKIS_FZ(kFzYokisLightControl, "manuSpecificYokisLightControl",
             kYokisLightControlMap)
ZHC_YOKIS_FZ(kFzYokisDimmer,       "manuSpecificYokisDimmer",
             kYokisDimmerMap)
ZHC_YOKIS_FZ(kFzYokisPilotWire,    "manuSpecificYokisPilotWire",
             kYokisPilotWireMap)
ZHC_YOKIS_FZ(kFzYokisInput,        "manuSpecificYokisInput",
             kYokisInputMap)
ZHC_YOKIS_FZ(kFzYokisEntry,        "manuSpecificYokisEntryConfigurator",
             kYokisEntryMap)
ZHC_YOKIS_FZ(kFzYokisChannel,      "manuSpecificYokisChannel",
             kYokisChannelMap)
ZHC_YOKIS_FZ(kFzYokisSubSystem,    "manuSpecificYokisSubSystem",
             kYokisSubSystemMap)
#undef ZHC_YOKIS_FZ

// ── Tz: ZclWriteSpec instances + ZHC_YOKIS_TZ macro ─────────────

namespace {

using ::zhc::generic::ZclWriteSpec;
using ::zhc::generic::ZclWriteLookup;

// LightControl writes (cluster 0xFC06).
constexpr ZclWriteSpec kSpecOnTimer        { "on_timer",        0x0002, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecEOnTimer       { "e_on_timer",      0x0003, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecPreOnDelay     { "pre_on_delay",    0x0004, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecEPreOnDelay    { "e_pre_on_delay",  0x0005, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecPreOffDelay    { "pre_off_delay",   0x0008, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecEPreOffDelay   { "e_pre_off_delay", 0x0009, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecPulseDuration  { "pulse_duration",  0x000A, 0x21, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecLongOnDuration { "long_on_duration",0x000C, 0x23, kYokisManufacturerCode, nullptr, 0 };

constexpr ZclWriteLookup kOperatingModeLut[] = {
    { "timer",      0x00 }, { "staircase", 0x01 },
    { "pulse",      0x02 }, { "switch",    0x03 },
};
constexpr ZclWriteSpec kSpecOperatingMode  { "operating_mode",  0x000D, 0x30, kYokisManufacturerCode, kOperatingModeLut, 4 };

constexpr ZclWriteSpec kSpecEStopAnnounce  { "e_stop_announce", 0x0014, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecEDeaf          { "e_deaf",          0x0015, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecEBlink         { "e_blink",         0x0016, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecBlinkAmount    { "blink_amount",    0x0017, 0x20, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecBlinkOnTime    { "blink_on_time",   0x0018, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecBlinkOffTime   { "blink_off_time",  0x0019, 0x23, kYokisManufacturerCode, nullptr, 0 };

constexpr ZclWriteLookup kStateAfterBlinkWrLut[] = {
    { "previous", 0x00 }, { "off", 0x01 }, { "on", 0x02 }, { "infinite", 0x03 },
};
constexpr ZclWriteSpec kSpecStateAfterBlink{ "state_after_blink", 0x001C, 0x30, kYokisManufacturerCode, kStateAfterBlinkWrLut, 4 };
constexpr ZclWriteSpec kSpecENcCommand     { "e_nc_command",    0x001D, 0x10, kYokisManufacturerCode, nullptr, 0 };

// Dimmer writes (cluster 0xFC07).
constexpr ZclWriteSpec kSpecMemoryPosition { "memory_position", 0x0001, 0x20, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecERampUp        { "e_ramp_up",       0x0002, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecRampUp         { "ramp_up",         0x0003, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecERampDown      { "e_ramp_down",     0x0004, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecRampDown       { "ramp_down",       0x0005, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecStepUp         { "step_up",         0x0007, 0x20, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecStepDown       { "step_down",       0x0016, 0x20, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecLowDimLimit    { "low_dim_limit",   0x0008, 0x20, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecHighDimLimit   { "high_dim_limit",  0x0009, 0x20, kYokisManufacturerCode, nullptr, 0 };

// PilotWire writes (cluster 0xFC0A) — attr writes only; setOrder
// command lives in the custom Tz further down.
constexpr ZclWriteSpec kSpecOrderTimer     { "order_timer",     0x0001, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecPreOrderTimer  { "pre_order_timer", 0x0002, 0x23, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecLedMode        { "led_mode",        0x0004, 0x20, kYokisManufacturerCode, nullptr, 0 };

constexpr ZclWriteLookup kFallbackOrderLut[] = {
    { "stop", 0 }, { "frost_off", 1 }, { "eco", 2 },
    { "confort_2", 3 }, { "confort_1", 4 }, { "confort", 5 },
};
constexpr ZclWriteSpec kSpecFallbackOrder  { "fallback_order",  0x0008, 0x20, kYokisManufacturerCode, kFallbackOrderLut, 6 };

// SubSystem writes (cluster 0xFC04).
constexpr ZclWriteLookup kPowerFailureWrLut[] = {
    { "off", 0 }, { "on", 1 }, { "previous", 2 },
};
constexpr ZclWriteSpec kSpecPowerFailureMode { "power_failure_mode", 0x0001, 0x30, kYokisManufacturerCode, kPowerFailureWrLut, 3 };

// Input writes (cluster 0xFC02).
constexpr ZclWriteLookup kInputModeWrLut[] = {
    { "unknown", 0 }, { "push_button", 1 }, { "switch", 2 },
    { "relay", 3 },   { "fp_in", 4 },
};
constexpr ZclWriteSpec kSpecInputMode      { "input_mode",      0x0000, 0x30, kYokisManufacturerCode, kInputModeWrLut, 5 };
constexpr ZclWriteSpec kSpecContactMode    { "contact_mode",    0x0001, 0x10, kYokisManufacturerCode, nullptr, 0 };

// Entry writes (cluster 0xFC03).
constexpr ZclWriteSpec kSpecEShortPress      { "e_short_press",       0x0001, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecELongPress       { "e_long_press",        0x0002, 0x10, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecLongPressDuration{ "long_press_duration", 0x0003, 0x21, kYokisManufacturerCode, nullptr, 0 };
constexpr ZclWriteSpec kSpecTimeBetweenPress { "time_between_press",  0x0004, 0x21, kYokisManufacturerCode, nullptr, 0 };

}  // namespace

// Macro stamping out a TzConverter that delegates to the generic
// writeAttributes encoder. All Yokis manuSpecific writes use cmd
// 0x02 (writeAttributes) — the generic encoder builds the rest.
#define ZHC_YOKIS_TZ(var, cluster_str, cluster_id_lit, spec_ref, key_str) \
    extern const TzConverter var{                                         \
        .key         = key_str,                                           \
        .cluster     = cluster_str,                                       \
        .cluster_id  = cluster_id_lit,                                    \
        .command_id  = 0x02,                                              \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                \
        .user_config = &spec_ref,                                         \
    };

// LightControl (0xFC06)
ZHC_YOKIS_TZ(kTzYokisOnTimer,         "manuSpecificYokisLightControl", 0xFC06, kSpecOnTimer,        "on_timer")
ZHC_YOKIS_TZ(kTzYokisEOnTimer,        "manuSpecificYokisLightControl", 0xFC06, kSpecEOnTimer,       "e_on_timer")
ZHC_YOKIS_TZ(kTzYokisPreOnDelay,      "manuSpecificYokisLightControl", 0xFC06, kSpecPreOnDelay,     "pre_on_delay")
ZHC_YOKIS_TZ(kTzYokisEPreOnDelay,     "manuSpecificYokisLightControl", 0xFC06, kSpecEPreOnDelay,    "e_pre_on_delay")
ZHC_YOKIS_TZ(kTzYokisPreOffDelay,     "manuSpecificYokisLightControl", 0xFC06, kSpecPreOffDelay,    "pre_off_delay")
ZHC_YOKIS_TZ(kTzYokisEPreOffDelay,    "manuSpecificYokisLightControl", 0xFC06, kSpecEPreOffDelay,   "e_pre_off_delay")
ZHC_YOKIS_TZ(kTzYokisPulseDuration,   "manuSpecificYokisLightControl", 0xFC06, kSpecPulseDuration,  "pulse_duration")
ZHC_YOKIS_TZ(kTzYokisLongOnDuration,  "manuSpecificYokisLightControl", 0xFC06, kSpecLongOnDuration, "long_on_duration")
ZHC_YOKIS_TZ(kTzYokisOperatingMode,   "manuSpecificYokisLightControl", 0xFC06, kSpecOperatingMode,  "operating_mode")
ZHC_YOKIS_TZ(kTzYokisEStopAnnounce,   "manuSpecificYokisLightControl", 0xFC06, kSpecEStopAnnounce,  "e_stop_announce")
ZHC_YOKIS_TZ(kTzYokisEDeaf,           "manuSpecificYokisLightControl", 0xFC06, kSpecEDeaf,          "e_deaf")
ZHC_YOKIS_TZ(kTzYokisEBlink,          "manuSpecificYokisLightControl", 0xFC06, kSpecEBlink,         "e_blink")
ZHC_YOKIS_TZ(kTzYokisBlinkAmount,     "manuSpecificYokisLightControl", 0xFC06, kSpecBlinkAmount,    "blink_amount")
ZHC_YOKIS_TZ(kTzYokisBlinkOnTime,     "manuSpecificYokisLightControl", 0xFC06, kSpecBlinkOnTime,    "blink_on_time")
ZHC_YOKIS_TZ(kTzYokisBlinkOffTime,    "manuSpecificYokisLightControl", 0xFC06, kSpecBlinkOffTime,   "blink_off_time")
ZHC_YOKIS_TZ(kTzYokisStateAfterBlink, "manuSpecificYokisLightControl", 0xFC06, kSpecStateAfterBlink,"state_after_blink")
ZHC_YOKIS_TZ(kTzYokisENcCommand,      "manuSpecificYokisLightControl", 0xFC06, kSpecENcCommand,     "e_nc_command")

// Dimmer (0xFC07)
ZHC_YOKIS_TZ(kTzYokisMemoryPosition,  "manuSpecificYokisDimmer", 0xFC07, kSpecMemoryPosition, "memory_position")
ZHC_YOKIS_TZ(kTzYokisERampUp,         "manuSpecificYokisDimmer", 0xFC07, kSpecERampUp,        "e_ramp_up")
ZHC_YOKIS_TZ(kTzYokisRampUp,          "manuSpecificYokisDimmer", 0xFC07, kSpecRampUp,         "ramp_up")
ZHC_YOKIS_TZ(kTzYokisERampDown,       "manuSpecificYokisDimmer", 0xFC07, kSpecERampDown,      "e_ramp_down")
ZHC_YOKIS_TZ(kTzYokisRampDown,        "manuSpecificYokisDimmer", 0xFC07, kSpecRampDown,       "ramp_down")
ZHC_YOKIS_TZ(kTzYokisStepUp,          "manuSpecificYokisDimmer", 0xFC07, kSpecStepUp,         "step_up")
ZHC_YOKIS_TZ(kTzYokisStepDown,        "manuSpecificYokisDimmer", 0xFC07, kSpecStepDown,       "step_down")
ZHC_YOKIS_TZ(kTzYokisLowDimLimit,     "manuSpecificYokisDimmer", 0xFC07, kSpecLowDimLimit,    "low_dim_limit")
ZHC_YOKIS_TZ(kTzYokisHighDimLimit,    "manuSpecificYokisDimmer", 0xFC07, kSpecHighDimLimit,   "high_dim_limit")

// PilotWire (0xFC0A)
ZHC_YOKIS_TZ(kTzYokisOrderTimer,      "manuSpecificYokisPilotWire", 0xFC0A, kSpecOrderTimer,    "order_timer")
ZHC_YOKIS_TZ(kTzYokisPreOrderTimer,   "manuSpecificYokisPilotWire", 0xFC0A, kSpecPreOrderTimer, "pre_order_timer")
ZHC_YOKIS_TZ(kTzYokisLedMode,         "manuSpecificYokisPilotWire", 0xFC0A, kSpecLedMode,       "led_mode")
ZHC_YOKIS_TZ(kTzYokisFallbackOrder,   "manuSpecificYokisPilotWire", 0xFC0A, kSpecFallbackOrder, "fallback_order")

// SubSystem (0xFC04)
ZHC_YOKIS_TZ(kTzYokisPowerFailureMode, "manuSpecificYokisSubSystem", 0xFC04, kSpecPowerFailureMode, "power_failure_mode")

// Input (0xFC02)
ZHC_YOKIS_TZ(kTzYokisInputMode,   "manuSpecificYokisInput", 0xFC02, kSpecInputMode,   "input_mode")
ZHC_YOKIS_TZ(kTzYokisContactMode, "manuSpecificYokisInput", 0xFC02, kSpecContactMode, "contact_mode")

// Entry / Configurator (0xFC03)
ZHC_YOKIS_TZ(kTzYokisEShortPress,       "manuSpecificYokisEntryConfigurator", 0xFC03, kSpecEShortPress,       "e_short_press")
ZHC_YOKIS_TZ(kTzYokisELongPress,        "manuSpecificYokisEntryConfigurator", 0xFC03, kSpecELongPress,        "e_long_press")
ZHC_YOKIS_TZ(kTzYokisLongPressDuration, "manuSpecificYokisEntryConfigurator", 0xFC03, kSpecLongPressDuration, "long_press_duration")
ZHC_YOKIS_TZ(kTzYokisTimeBetweenPress,  "manuSpecificYokisEntryConfigurator", 0xFC03, kSpecTimeBetweenPress,  "time_between_press")

#undef ZHC_YOKIS_TZ

// ── Tz: PilotWire setOrder cluster command ──────────────────────
//
// `setOrder` (cluster 0xFC0A, cmd 0x00) is a manu-specific cluster
// command, NOT writeAttributes — so we cannot use tz_zcl_write_attr.
// Wire format (mirrors how the generic `fz_cmd_*` handlers expect
// to be reversed):
//   fc:1   = 0x15  (cluster-specific + manu-specific + DDR + c→s)
//   manu:2 = 0x132D LE
//   tsn:1  = 0x00 placeholder (filled by adapter)
//   cmd:1  = 0x00
//   payload:1 = order (u8)
//
// Input key: "pilotwire_order"; accepts uint or one of the labels in
// the pilotwireOrderEnun lookup.

namespace {

bool tz_yokis_pilotwire_set_order(std::string_view key,
                                    const Value& input,
                                    const TzConverter&,
                                    const PreparedDefinition&,
                                    RuntimeContext&,
                                    std::span<std::uint8_t> out_frame,
                                    std::size_t& out_size) {
    out_size = 0;
    if (key != "pilotwire_order") return false;

    std::uint8_t order = 0xFF;
    if (input.type == ValueType::Uint) {
        if (input.u > 5) return false;
        order = static_cast<std::uint8_t>(input.u);
    } else if (input.type == ValueType::Int) {
        if (input.i < 0 || input.i > 5) return false;
        order = static_cast<std::uint8_t>(input.i);
    } else if (input.type == ValueType::StringRef && input.str) {
        for (const auto& l : kFallbackOrderLut) {
            if (std::strcmp(l.label, input.str) == 0) {
                order = static_cast<std::uint8_t>(l.value); break;
            }
        }
        if (order == 0xFF) return false;
    } else {
        return false;
    }

    // 1 (fc) + 2 (manu) + 1 (tsn) + 1 (cmd) + 1 (payload) = 6 bytes.
    if (out_frame.size() < 6) return false;
    out_frame[0] = 0x15;
    out_frame[1] = static_cast<std::uint8_t>(kYokisManufacturerCode & 0xFF);
    out_frame[2] = static_cast<std::uint8_t>((kYokisManufacturerCode >> 8) & 0xFF);
    out_frame[3] = 0x00;        // tsn placeholder
    out_frame[4] = 0x00;        // cmd setOrder
    out_frame[5] = order;
    out_size = 6;
    return true;
}

}  // namespace

extern const TzConverter kTzYokisPilotWireSetOrder{
    .key         = "pilotwire_order",
    .cluster     = "manuSpecificYokisPilotWire",
    .cluster_id  = 0xFC0A,
    .command_id  = 0x00,
    .fn          = &tz_yokis_pilotwire_set_order,
    .user_config = nullptr,
};

}  // namespace zhc::yokis
