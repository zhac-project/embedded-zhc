// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: generic vendor-neutral converters.
//
// z2m-source: zigbee-herdsman-converters/src/converters/fromZigbee.ts

#include "definitions/_generic/_shared.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"   // RuntimeContext::alloc_str

namespace zhc::generic {

// ── fz_on_off ───────────────────────────────────────────────────────

bool fz_on_off(const DecodedMessage& msg,
                const FzConverter&,
                const PreparedDefinition&,
                RuntimeContext&,
                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0000 (onOff) — decimal-string fallback key when the
    // decoder doesn't have an attr_keys table.
    const Value* v = msg.payload.find("0");
    if (!v) return false;

    bool state;
    if (v->type == ValueType::Bool)      state = v->b;
    else if (v->type == ValueType::Uint) state = v->u != 0;
    else return false;

    Value o{}; o.type = ValueType::Bool; o.b = state;
    out.put("state", o);
    return true;
}

extern const FzConverter kFzOnOff{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_on_off },
    .user_config       = nullptr,
};

// ── fz_battery ──────────────────────────────────────────────────────

bool fz_battery(const DecodedMessage& msg,
                 const FzConverter&,
                 const PreparedDefinition&,
                 RuntimeContext&,
                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;

    // attr 0x0020 — batteryVoltage (u8 in units of 100 mV → emit in mV).
    if (const Value* v = msg.payload.find("32")) {
        if (v->type == ValueType::Uint) {
            Value volt{}; volt.type = ValueType::Uint; volt.u = v->u * 100;
            out.put("voltage", volt);
            emitted = true;
        }
    }

    // attr 0x0021 — batteryPercentageRemaining. Zigbee spec: u8 in
    // half-percent units, 0x00 = 0 %, 0xC8 = 100 %. Emit whole percent.
    if (const Value* v = msg.payload.find("33")) {
        if (v->type == ValueType::Uint) {
            Value bat{}; bat.type = ValueType::Uint; bat.u = v->u / 2;
            out.put("battery", bat);
            emitted = true;
        }
    }

    return emitted;
}

extern const FzConverter kFzBattery{
    .family            = FrameFamily::Zcl,
    .cluster           = "genPowerCfg",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_battery },
    .user_config       = nullptr,
};

// ── fz_brightness ───────────────────────────────────────────────────

bool fz_brightness(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 currentLevel
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("brightness", o);
    return true;
}

extern const FzConverter kFzBrightness{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_brightness },
    .user_config       = nullptr,
};

// ── fz_color_temperature ────────────────────────────────────────────

bool fz_color_temperature(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("7");   // attr 0x0007 colorTemperature
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("color_temp", o);
    return true;
}

extern const FzConverter kFzColorTemperature{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_color_temperature },
    .user_config       = nullptr,
};

// ── fz_color (xy) ───────────────────────────────────────────────────

bool fz_color(const DecodedMessage& msg,
               const FzConverter&,
               const PreparedDefinition&,
               RuntimeContext&,
               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // lightingColorCtrl unified report decoder. Walks every attr the
    // ColorControl cluster might ship in a single AttributeReport so
    // we don't need a separate fz per axis.
    bool emitted = false;

    // Attr 0x0000 currentHue (u8 [0,254]).
    if (const Value* v = msg.payload.find("0")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("hue", o);
            emitted = true;
        }
    }
    // Attr 0x0001 currentSaturation (u8 [0,254]).
    if (const Value* v = msg.payload.find("1")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("saturation", o);
            emitted = true;
        }
    }
    // Attr 0x0003 currentX / 0x0004 currentY (u16). z2m encodes CIE 1931
    // x/y as `raw / 65535`; emit float in [0,1].
    if (const Value* v = msg.payload.find("3")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Float;
            o.f = static_cast<float>(v->u) / 65535.0f;
            out.put("color_x", o);
            emitted = true;
        }
    }
    if (const Value* v = msg.payload.find("4")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Float;
            o.f = static_cast<float>(v->u) / 65535.0f;
            out.put("color_y", o);
            emitted = true;
        }
    }
    // Attr 0x0007 colorTemperature (u16 mireds). Same as fz_color_temp
    // — duplicating here so a device can wire just kFzColor and still
    // round-trip color_temp on bulbs that ship all three axes in one
    // report.
    if (const Value* v = msg.payload.find("7")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("color_temp", o);
            emitted = true;
        }
    }
    // Attr 0x0008 (colorMode): 0=HueSat, 1=XY, 2=ColorTemp. Emit as
    // StringRef so UI + rules can see which branch the device is
    // currently representing.
    if (const Value* v = msg.payload.find("8")) {
        if (v->type == ValueType::Uint) {
            const char* mode = nullptr;
            switch (v->u) {
                case 0: mode = "hs";         break;
                case 1: mode = "xy";         break;
                case 2: mode = "color_temp"; break;
            }
            if (mode) {
                Value o{}; o.type = ValueType::StringRef; o.str = mode;
                out.put("color_mode", o);
                emitted = true;
            }
        }
    }
    // Attr 0x4000 enhancedCurrentHue (u16). Optional on legacy bulbs;
    // kept under a separate key so the SPA/rules engine can tell it
    // apart from the 8-bit `hue`.
    if (const Value* v = msg.payload.find("16384")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("enhanced_hue", o);
            emitted = true;
        }
    }
    return emitted;
}

extern const FzConverter kFzColor{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_color },
    .user_config       = nullptr,
};

// ── fz_power_on_behavior (genOnOff 0x4003) ──────────────────────────

bool fz_power_on_behavior(const DecodedMessage& msg,
                            const FzConverter&,
                            const PreparedDefinition&,
                            RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genOnOff attr 0x4003 startUpOnOff (enum8):
    //   0=off, 1=on, 2=toggle, 0xFF=previous.
    const Value* v = msg.payload.find("16387");    // 0x4003 = 16387
    if (!v || v->type != ValueType::Uint) return false;
    const char* label = nullptr;
    switch (v->u) {
        case 0:    label = "off";      break;
        case 1:    label = "on";       break;
        case 2:    label = "toggle";   break;
        case 0xFF: label = "previous"; break;
    }
    if (!label) return false;
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("power_on_behavior", o);
    return true;
}

extern const FzConverter kFzPowerOnBehavior{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_power_on_behavior },
    .user_config       = nullptr,
};

// ── fz_ignore ───────────────────────────────────────────────────────

bool fz_ignore(const DecodedMessage&,
                const FzConverter&,
                const PreparedDefinition&,
                RuntimeContext&,
                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>&) {
    return true;
}

ZHC_FZ_IGNORE(kFzIgnoreOccupancyReport,    "msOccupancySensing");
ZHC_FZ_IGNORE(kFzIgnoreIlluminanceReport,  "msIlluminanceMeasurement");

// ── Cluster-command action converters ───────────────────────────────
//
// Each handler emits `action = "<literal>"`. The string pointer must
// outlive the dispatch call — enforced here by using a static-storage
// string literal in each trampoline. ZCL cluster-specific commands
// travel Client→Server (device is client), so `direction` pins to
// `ClientToServer` to avoid a stray AttributeReport matching.

namespace {

bool emit_action(const char* label,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{};
    v.type = ValueType::StringRef;
    v.str  = label;
    out.put("action", v);
    return true;
}

bool fz_cmd_on(const DecodedMessage&, const FzConverter&,
                const PreparedDefinition&, RuntimeContext&,
                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_action("on", out);
}

bool fz_cmd_off(const DecodedMessage&, const FzConverter&,
                 const PreparedDefinition&, RuntimeContext&,
                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_action("off", out);
}

bool fz_cmd_toggle(const DecodedMessage&, const FzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_action("toggle", out);
}

// ZCL header = 3 bytes (FC, TSN, CmdID) or 5 when manu-specific.
// Payload follows at that offset inside `raw_data`.
std::size_t zcl_hdr_len(const DecodedMessage& msg) {
    return msg.manufacturer_specific ? 5u : 3u;
}

bool fz_cmd_move_to_level(const DecodedMessage& msg, const FzConverter&,
                           const PreparedDefinition&, RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    emit_action("brightness_move_to_level", out);
    // Payload: level u8, transition_time u16 LE.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() >= off + 1) {
        Value v{}; v.type = ValueType::Uint; v.u = msg.raw_data[off];
        out.put("level", v);
    }
    return true;
}

bool fz_cmd_move_to_color_temp(const DecodedMessage& msg, const FzConverter&,
                                 const PreparedDefinition&, RuntimeContext&,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    emit_action("color_temperature_move", out);
    // Payload: color_temp u16 LE (mireds), transition_time u16 LE.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() >= off + 2) {
        const std::uint16_t ct = static_cast<std::uint16_t>(msg.raw_data[off]) |
                                  (static_cast<std::uint16_t>(msg.raw_data[off + 1]) << 8);
        Value v{}; v.type = ValueType::Uint; v.u = ct;
        out.put("color_temperature", v);
    }
    return true;
}

bool fz_cmd_move_to_hue_and_sat(const DecodedMessage& msg, const FzConverter&,
                                  const PreparedDefinition&, RuntimeContext&,
                                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    emit_action("move_to_hue_and_saturation", out);
    // Payload: hue u8, saturation u8, transition_time u16 LE.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() >= off + 2) {
        Value h{}; h.type = ValueType::Uint; h.u = msg.raw_data[off];
        out.put("hue", h);
        Value s{}; s.type = ValueType::Uint; s.u = msg.raw_data[off + 1];
        out.put("saturation", s);
    }
    return true;
}

bool fz_cmd_step(const DecodedMessage& msg, const FzConverter&,
                  const PreparedDefinition&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload: step_mode u8, step_size u8, transition_time u16 LE.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 4) return false;
    const std::uint8_t mode = msg.raw_data[off];
    const std::uint8_t size = msg.raw_data[off + 1];
    const std::uint16_t tt  = static_cast<std::uint16_t>(msg.raw_data[off + 2]) |
                               (static_cast<std::uint16_t>(msg.raw_data[off + 3]) << 8);

    const char* label = (mode == 0) ? "brightness_step_up" :
                         (mode == 1) ? "brightness_step_down" :
                                        nullptr;
    if (!label) return false;   // reserved mode values — drop

    emit_action(label, out);

    Value sz{}; sz.type = ValueType::Uint; sz.u = size;
    out.put("action_step_size", sz);

    Value tf{}; tf.type = ValueType::Float; tf.f = static_cast<float>(tt) / 10.0f;
    out.put("action_transition_time", tf);
    return true;
}

}  // namespace

extern const FzConverter kFzCommandOn{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x01,   // ZCL genOnOff cmd On
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_on },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandOff{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,   // ZCL genOnOff cmd Off
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_off },
    .user_config       = nullptr,
};

// genOnOff cmd 0x40 commandOffWithEffect — payload [effect_id u8,
// variant u8] is ignored; we emit the same `action="off"` as plain
// commandOff so downstream consumers don't need special handling.
extern const FzConverter kFzCommandOffWithEffect{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x40,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_off },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandToggle{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x02,   // ZCL genOnOff cmd Toggle
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_toggle },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandMoveToLevel{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,   // genLevelCtrl cmd MoveToLevel
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_move_to_level },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandMoveToColorTemp{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x0A,   // lightingColorCtrl cmd MoveToColorTemp
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_move_to_color_temp },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandMoveToHueAndSaturation{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x06,   // lightingColorCtrl cmd MoveToHueAndSat
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_move_to_hue_and_sat },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandStep{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x02,   // genLevelCtrl cmd Step
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_step },
    .user_config       = nullptr,
};

namespace {

bool fz_cmd_move(const DecodedMessage& msg, const FzConverter&,
                  const PreparedDefinition&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload: move_mode u8 (0=up, 1=down), rate u8.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 2) return false;
    const std::uint8_t mode = msg.raw_data[off];
    const std::uint8_t rate = msg.raw_data[off + 1];
    const char* label = (mode == 0) ? "brightness_move_up" :
                         (mode == 1) ? "brightness_move_down" :
                                        nullptr;
    if (!label) return false;
    emit_action(label, out);
    Value r{}; r.type = ValueType::Uint; r.u = rate;
    out.put("action_rate", r);
    return true;
}

bool fz_cmd_stop(const DecodedMessage&, const FzConverter&,
                  const PreparedDefinition&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_action("brightness_stop", out);
}

// ColorCtrl move-style commands (z2m fz.command_move_hue,
// fz.command_move_color_temperature). Movemode lookup matches
// z2m: 1 = up/move, 3 = down, 0 (and any other) = stop.

bool fz_cmd_move_hue(const DecodedMessage& msg, const FzConverter&,
                      const PreparedDefinition&, RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload: move_mode u8, rate u8.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 2) return false;
    const std::uint8_t mode = msg.raw_data[off];
    const std::uint8_t rate = msg.raw_data[off + 1];
    const char* sub = (mode == 1) ? "move" : (mode == 3) ? "down" : "stop";
    char buf[16];                                // "hue_<8>"
    std::size_t i = 0;
    const char* p = "hue_";
    while (*p && i + 1 < sizeof(buf)) buf[i++] = *p++;
    p = sub;
    while (*p && i + 1 < sizeof(buf)) buf[i++] = *p++;
    buf[i] = '\0';
    static char ring[3][16];
    static unsigned ring_idx = 0;
    auto& slot = ring[ring_idx++ % 3];
    std::memcpy(slot, buf, i + 1);
    Value a{}; a.type = ValueType::StringRef; a.str = slot;
    out.put("action", a);
    Value r{}; r.type = ValueType::Uint; r.u = rate;
    out.put("action_rate", r);
    return true;
}

bool fz_cmd_move_to_color(const DecodedMessage& msg, const FzConverter&,
                           const PreparedDefinition&, RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload: colorx u16le, colory u16le, transition_time u16le.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 4) return false;
    emit_action("color_move", out);
    const std::uint16_t cx = static_cast<std::uint16_t>(msg.raw_data[off]) |
                              (static_cast<std::uint16_t>(msg.raw_data[off + 1]) << 8);
    const std::uint16_t cy = static_cast<std::uint16_t>(msg.raw_data[off + 2]) |
                              (static_cast<std::uint16_t>(msg.raw_data[off + 3]) << 8);
    Value vx{}; vx.type = ValueType::Uint; vx.u = cx;
    out.put("action_color_x", vx);
    Value vy{}; vy.type = ValueType::Uint; vy.u = cy;
    out.put("action_color_y", vy);
    if (msg.raw_data.size() >= off + 6) {
        const std::uint16_t tt = static_cast<std::uint16_t>(msg.raw_data[off + 4]) |
                                  (static_cast<std::uint16_t>(msg.raw_data[off + 5]) << 8);
        Value vt{}; vt.type = ValueType::Float; vt.f = static_cast<float>(tt) / 10.0f;
        out.put("action_transition_time", vt);
    }
    return true;
}

bool fz_cmd_move_color_temperature(const DecodedMessage& msg, const FzConverter&,
                                     const PreparedDefinition&, RuntimeContext&,
                                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload: move_mode u8, rate u16le, min u16le, max u16le.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 1) return false;
    const std::uint8_t mode = msg.raw_data[off];
    const char* sub = (mode == 1) ? "up" : (mode == 3) ? "down" : "stop";
    char buf[40];                                // "color_temperature_move_<down>"
    const char* prefix = "color_temperature_move_";
    std::size_t i = 0;
    while (prefix[i] != '\0' && i + 1 < sizeof(buf)) { buf[i] = prefix[i]; ++i; }
    std::size_t k = 0;
    while (sub[k] != '\0' && i + 1 < sizeof(buf)) buf[i++] = sub[k++];
    buf[i] = '\0';
    static char ct_ring[3][40];
    static unsigned ct_ring_idx = 0;
    auto& slot = ct_ring[ct_ring_idx++ % 3];
    std::memcpy(slot, buf, i + 1);
    Value a{}; a.type = ValueType::StringRef; a.str = slot;
    out.put("action", a);
    if (msg.raw_data.size() >= off + 3) {
        const std::uint16_t rate = static_cast<std::uint16_t>(msg.raw_data[off + 1]) |
                                    (static_cast<std::uint16_t>(msg.raw_data[off + 2]) << 8);
        Value r{}; r.type = ValueType::Uint; r.u = rate;
        out.put("action_rate", r);
    }
    if (msg.raw_data.size() >= off + 5) {
        const std::uint16_t min = static_cast<std::uint16_t>(msg.raw_data[off + 3]) |
                                   (static_cast<std::uint16_t>(msg.raw_data[off + 4]) << 8);
        Value m{}; m.type = ValueType::Uint; m.u = min;
        out.put("action_minimum", m);
    }
    if (msg.raw_data.size() >= off + 7) {
        const std::uint16_t max = static_cast<std::uint16_t>(msg.raw_data[off + 5]) |
                                   (static_cast<std::uint16_t>(msg.raw_data[off + 6]) << 8);
        Value m{}; m.type = ValueType::Uint; m.u = max;
        out.put("action_maximum", m);
    }
    return true;
}

}  // namespace

extern const FzConverter kFzCommandMoveHue{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x01,   // lightingColorCtrl cmd MoveHue
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_move_hue },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandMoveToColor{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x07,   // lightingColorCtrl cmd MoveToColor
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_move_to_color },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandMoveColorTemperature{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x4B,   // lightingColorCtrl cmd MoveColorTemp
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_move_color_temperature },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandMove{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x01,   // genLevelCtrl cmd Move
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_move },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandStop{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x03,   // genLevelCtrl cmd Stop
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_stop },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandStopWithOnOff{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x07,   // genLevelCtrl cmd StopWithOnOff
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_stop },
    .user_config       = nullptr,
};

namespace {
bool fz_cmd_cover_open(const DecodedMessage&, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_action("open", out);
}
bool fz_cmd_cover_close(const DecodedMessage&, const FzConverter&,
                         const PreparedDefinition&, RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_action("close", out);
}
bool fz_cmd_cover_stop(const DecodedMessage&, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return emit_action("stop", out);
}
}  // namespace

extern const FzConverter kFzCommandCoverOpen{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_cover_open },
    .user_config       = nullptr,
};
extern const FzConverter kFzCommandCoverClose{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x01,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_cover_close },
    .user_config       = nullptr,
};
extern const FzConverter kFzCommandCoverStop{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x02,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_cover_stop },
    .user_config       = nullptr,
};

// ── Temperature / humidity / pressure / illuminance ──────────────────

namespace {

bool fz_temperature(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 measuredValue
    if (!v) return false;
    std::int32_t raw = 0;
    if (v->type == ValueType::Int)       raw = static_cast<std::int32_t>(v->i);
    else if (v->type == ValueType::Uint) raw = static_cast<std::int32_t>(v->u);
    else return false;
    Value o{}; o.type = ValueType::Float;
    o.f = static_cast<float>(raw) / 100.0f;
    out.put("temperature", o);
    return true;
}

// genDeviceTempCfg (0x0002) attr 0x0000 currentTemperature (s16, whole °C).
// z2m `fz.device_temperature` returns the raw value unscaled.
bool fz_device_temperature(const DecodedMessage& msg, const FzConverter&,
                           const PreparedDefinition&, RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 currentTemperature
    if (!v) return false;
    std::int32_t raw = 0;
    if (v->type == ValueType::Int)       raw = static_cast<std::int32_t>(v->i);
    else if (v->type == ValueType::Uint) raw = static_cast<std::int32_t>(v->u);
    else return false;
    Value o{}; o.type = ValueType::Int; o.i = raw;
    out.put("device_temperature", o);
    return true;
}

bool fz_humidity(const DecodedMessage& msg, const FzConverter&,
                   const PreparedDefinition&, RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Float;
    o.f = static_cast<float>(v->u) / 100.0f;
    out.put("humidity", o);
    return true;
}

// msSoilMoisture (0x0408) measuredValue (u16, 0.01 %). Mirrors fz_humidity.
bool fz_soil_moisture(const DecodedMessage& msg, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v) return false;
    std::int32_t raw = 0;
    if (v->type == ValueType::Uint)     raw = static_cast<std::int32_t>(v->u);
    else if (v->type == ValueType::Int) raw = static_cast<std::int32_t>(v->i);
    else return false;
    Value o{}; o.type = ValueType::Float;
    o.f = static_cast<float>(raw) / 100.0f;
    out.put("soil_moisture", o);
    return true;
}

bool fz_pressure(const DecodedMessage& msg, const FzConverter&,
                  const PreparedDefinition&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v) return false;
    std::int32_t raw = 0;
    if (v->type == ValueType::Int)       raw = static_cast<std::int32_t>(v->i);
    else if (v->type == ValueType::Uint) raw = static_cast<std::int32_t>(v->u);
    else return false;
    Value o{}; o.type = ValueType::Int; o.i = raw;
    out.put("pressure", o);
    return true;
}

bool fz_illuminance(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("illuminance", o);
    return true;
}

bool fz_ias_zone_status_change(const DecodedMessage& msg, const FzConverter&,
                                const PreparedDefinition&, RuntimeContext&,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload (after ZCL header): zoneStatus:u16 LE, extendedStatus:u8,
    // zoneId:u8, delay:u16.  We only need zoneStatus.
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    const std::uint16_t status =
        static_cast<std::uint16_t>(msg.raw_data[hdr]) |
        (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);

    Value v{}; v.type = ValueType::Bool;
    v.b = (status & 0x0001) != 0; out.put("alarm_1",     v);
    v.b = (status & 0x0002) != 0; out.put("alarm_2",     v);
    v.b = (status & 0x0004) != 0; out.put("tamper",      v);
    v.b = (status & 0x0008) != 0; out.put("battery_low", v);
    return true;
}

}  // namespace

extern const FzConverter kFzTemperature{
    .family            = FrameFamily::Zcl,
    .cluster           = "msTemperatureMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_temperature },
    .user_config       = nullptr,
};

extern const FzConverter kFzDeviceTemperature{
    .family            = FrameFamily::Zcl,
    .cluster           = "genDeviceTempCfg",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_device_temperature },
    .user_config       = nullptr,
};

extern const FzConverter kFzHumidity{
    .family            = FrameFamily::Zcl,
    .cluster           = "msRelativeHumidity",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_humidity },
    .user_config       = nullptr,
};

extern const FzConverter kFzPressure{
    .family            = FrameFamily::Zcl,
    .cluster           = "msPressureMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_pressure },
    .user_config       = nullptr,
};

extern const FzConverter kFzIlluminance{
    .family            = FrameFamily::Zcl,
    .cluster           = "msIlluminanceMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_illuminance },
    .user_config       = nullptr,
};

extern const FzConverter kFzSoilMoisture{
    .family            = FrameFamily::Zcl,
    .cluster           = "msSoilMoisture",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_soil_moisture },
    .user_config       = nullptr,
};

extern const FzConverter kFzIasZoneStatusChange{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,   // commandStatusChangeNotification
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    // ZoneStatusChangeNotification flows server (device) → client
    // (coord); FC bit 3 = 1 in the inbound frame.
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ias_zone_status_change },
    .user_config       = nullptr,
};

// ── zone-type-specific IAS converters ────────────────────────────────

namespace {

// user_config points at a static label + bit selector. Most z2m
// `fz.ias_*_alarm` decoders read bit 0 (alarm_1); a few
// (`fz.ias_occupancy_alarm_2` and friends) read bit 1 instead.
struct IasAlarmLabel { const char* alarm_1; std::uint8_t bit = 0; };

bool fz_ias_typed(const DecodedMessage& msg, const FzConverter& self,
                   const PreparedDefinition&, RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::size_t hdr = msg.manufacturer_specific ? 5 : 3;
    if (msg.raw_data.size() < hdr + 2) return false;
    const std::uint16_t status =
        static_cast<std::uint16_t>(msg.raw_data[hdr]) |
        (static_cast<std::uint16_t>(msg.raw_data[hdr + 1]) << 8);

    const auto* cfg = reinterpret_cast<const IasAlarmLabel*>(self.user_config);
    if (!cfg || !cfg->alarm_1) return false;

    Value v{}; v.type = ValueType::Bool;
    v.b = (status & (1u << cfg->bit)) != 0; out.put(cfg->alarm_1,  v);
    v.b = (status & 0x0004) != 0; out.put("tamper",      v);
    v.b = (status & 0x0008) != 0; out.put("battery_low", v);
    return true;
}

constexpr IasAlarmLabel kLbl_Contact     { "contact",         0 };
constexpr IasAlarmLabel kLbl_Motion      { "occupancy",       0 };
constexpr IasAlarmLabel kLbl_Motion2     { "occupancy",       1 };  // z2m fz.ias_occupancy_alarm_2
constexpr IasAlarmLabel kLbl_WaterLeak   { "water_leak",      0 };
constexpr IasAlarmLabel kLbl_Smoke       { "smoke",           0 };
constexpr IasAlarmLabel kLbl_Co          { "carbon_monoxide", 0 };
constexpr IasAlarmLabel kLbl_Vibration   { "vibration",       0 };
constexpr IasAlarmLabel kLbl_Vibration2  { "vibration",       1 };  // z2m fz.ias_occupancy_alarm_2 (Bitron AV2010/33)
constexpr IasAlarmLabel kLbl_Rain        { "rain",            0 };  // eWeLink WS01 rainfall
constexpr IasAlarmLabel kLbl_Gas         { "gas",             0 };  // z2m fz.ias_gas_alarm_1
constexpr IasAlarmLabel kLbl_Gas2        { "gas",             1 };  // z2m fz.ias_gas_alarm_2
// z2m m.iasZoneAlarm({zoneType: "alarm"}) publishes the single key `alarm`
// (both alarm_1/alarm_2 collapse onto it). Used by generic sirens (adeo
// LDSENK06). bit 0 → `alarm`.
constexpr IasAlarmLabel kLbl_Alarm       { "alarm",           0 };

}  // namespace

#define ZHC_IAS_TYPED_CVT(name, cfg_ptr)                                   \
    extern const FzConverter name{                                         \
        .family            = FrameFamily::Zcl,                             \
        .cluster           = "ssIasZone",                                  \
        .type_mask         = type_bit(MessageType::Command),               \
        .command_id        = 0x00,                                         \
        .attr_id           = WILDCARD_ATTR_ID,                             \
        .endpoint          = WILDCARD_ENDPOINT,                            \
        .frame_flags_mask  = 0,                                            \
        .frame_flags_value = 0,                                            \
        .direction         = Direction::ServerToClient,                   \
        .fn                = { .zcl_fn = fz_ias_typed },                   \
        .user_config       = cfg_ptr,                                      \
    }

ZHC_IAS_TYPED_CVT(kFzIasContactAlarm,   &kLbl_Contact);
ZHC_IAS_TYPED_CVT(kFzIasMotionAlarm,    &kLbl_Motion);
ZHC_IAS_TYPED_CVT(kFzIasMotionAlarm2,   &kLbl_Motion2);
ZHC_IAS_TYPED_CVT(kFzIasWaterLeakAlarm, &kLbl_WaterLeak);
ZHC_IAS_TYPED_CVT(kFzIasSmokeAlarm,     &kLbl_Smoke);
ZHC_IAS_TYPED_CVT(kFzIasCoAlarm,        &kLbl_Co);
ZHC_IAS_TYPED_CVT(kFzIasVibrationAlarm, &kLbl_Vibration);
ZHC_IAS_TYPED_CVT(kFzIasVibrationAlarm2, &kLbl_Vibration2);
ZHC_IAS_TYPED_CVT(kFzIasRainAlarm,      &kLbl_Rain);
ZHC_IAS_TYPED_CVT(kFzIasGasAlarm,       &kLbl_Gas);
ZHC_IAS_TYPED_CVT(kFzIasGasAlarm2,      &kLbl_Gas2);
ZHC_IAS_TYPED_CVT(kFzIasGenericAlarm,   &kLbl_Alarm);

// ── ssIasAce arm / panic command decoders ───────────────────────────
//
// Closes z2m fz.command_arm + fz.command_panic. Used by security
// keypads + key fobs (linkind ZS130000078, ZS130000178).
//
// commandArm   (cmd 0x00): u8 arm_mode, octstr code (u8 len + bytes),
//                          u8 zone_id. Emit `action` (lookup),
//                          `action_code`, `action_zone`.
// commandPanic (cmd 0x04): no body. Emit `action = "panic"`.

namespace {

constexpr const char* kArmModeLut[] = {
    "disarm",          // 0
    "arm_day_zones",   // 1
    "arm_night_zones", // 2
    "arm_all_zones",   // 3
    "exit_delay",      // 4
    "entry_delay",     // 5
    "not_ready",       // 6
    "in_alarm",        // 7
    "arming_stay",     // 8
    "arming_night",    // 9
    "arming_away",     // 10
};

bool fz_ias_ace_arm(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.size() < 2) return false;
    const std::uint8_t arm_mode = msg.raw_body[0];
    const std::uint8_t code_len = msg.raw_body[1];
    if (msg.raw_body.size() < 2u + code_len + 1u) return false;

    Value a{}; a.type = ValueType::StringRef;
    a.str = arm_mode < (sizeof(kArmModeLut) / sizeof(kArmModeLut[0]))
            ? kArmModeLut[arm_mode] : "unknown";
    out.put("action", a);

    // PIN code: copy into the per-dispatch arena so the StringRef
    // remains valid through the merge. Cap at 16 chars to match the
    // longest realistic ZCL ssIasAce code length; failure to allocate
    // (arena full) drops the field rather than dangling. The previous
    // 3-slot static ring could be clobbered by a 4th panel within the
    // same dispatch.
    const std::size_t kCodeCap = 15;   // +1 NUL fits in alloc_str
    const std::size_t n = code_len < kCodeCap ? code_len : kCodeCap;
    if (const char* s =
            ctx.alloc_str(reinterpret_cast<const char*>(msg.raw_body.data() + 2), n)) {
        Value c{}; c.type = ValueType::StringRef; c.str = s;
        out.put("action_code", c);
    }

    Value z{}; z.type = ValueType::Uint;
    z.u = msg.raw_body[2 + code_len];
    out.put("action_zone", z);
    return true;
}

bool fz_ias_ace_panic(const DecodedMessage&, const FzConverter&,
                       const PreparedDefinition&, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value a{}; a.type = ValueType::StringRef; a.str = "panic";
    out.put("action", a);
    return true;
}

}  // namespace

extern const FzConverter kFzIasAceArm{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasAce",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ias_ace_arm },
    .user_config       = nullptr,
};

extern const FzConverter kFzIasAcePanic{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasAce",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x04,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ias_ace_panic },
    .user_config       = nullptr,
};

// ssIasAce commandEmergency (cmd 0x02). z2m fz.command_emergency
// equivalent — emits action="emergency". No body.
namespace {
bool fz_ias_ace_emergency(const DecodedMessage&, const FzConverter&,
                            const PreparedDefinition&, RuntimeContext&,
                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value a{}; a.type = ValueType::StringRef; a.str = "emergency";
    out.put("action", a);
    return true;
}
}  // namespace

extern const FzConverter kFzIasAceEmergency{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasAce",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x02,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ias_ace_emergency },
    .user_config       = nullptr,
};

// ssIasWd commandStartWarning shorthand (cmd 0x00) — `kTzWarningSimple`.
// z2m tz.warning_simple writes a single u16 `warningmode` (0=stop /
// 1=burglar / 2=fire / 3=emergency / 4=police_panic / 5=fire_panic /
// 6=emergency_panic) plus zero strobe + zero duration. Accepts a
// StringRef matching the labels above OR a raw uint mode.
namespace {
constexpr struct { const char* label; std::uint8_t mode; } kWarnModeLut[] = {
    {"stop",            0}, {"OFF",             0},
    {"burglar",         1},
    {"fire",            2},
    {"emergency",       3}, {"alarm",           3}, {"ON",              3},
    {"police_panic",    4},
    {"fire_panic",      5},
    {"emergency_panic", 6},
};

bool tz_ias_wd_warning_simple(std::string_view key, const Value& input,
                                const TzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                std::span<std::uint8_t> out, std::size_t& n) {
    if (key != "alarm" && key != "warning_simple") return false;
    std::uint8_t mode = 0;
    if (input.type == ValueType::Bool) mode = input.b ? 3 : 0;
    else if (input.type == ValueType::Uint) mode = static_cast<std::uint8_t>(input.u);
    else if (input.type == ValueType::Int)  mode = static_cast<std::uint8_t>(input.i);
    else if (input.type == ValueType::StringRef && input.str) {
        bool found = false;
        for (const auto& e : kWarnModeLut) {
            if (std::strcmp(e.label, input.str) == 0) {
                mode = e.mode; found = true; break;
            }
        }
        if (!found) return false;
    } else return false;

    // Build startWarning frame. Body per ZCL spec:
    //   warningmode_strobe_sirenLevel (u8: bits 0-3=mode, 4=strobe, 6-7=siren_level)
    //   warningduration u16 LE (seconds; 0 = device default)
    //   strobeDutyCycle u8 (0-100) ; strobeLevel u8 (0-3)
    if (out.size() < 8) return false;
    out[0] = 0x01;                          // fc cluster-specific
    out[1] = 0x00;                          // tsn
    out[2] = 0x00;                          // cmd startWarning
    // info byte: mode in low nibble, level very_high (3) in high bits
    const std::uint8_t info = mode | (mode != 0 ? (3u << 6) | (1u << 4) : 0u);
    out[3] = info;
    out[4] = 0x00; out[5] = 0x00;           // duration u16
    out[6] = 0x00;                          // strobe duty
    out[7] = 0x00;                          // strobe level
    n = 8;
    return true;
}

bool tz_ias_wd_squawk(std::string_view key, const Value& input,
                       const TzConverter&,
                       const PreparedDefinition&,
                       RuntimeContext&,
                       std::span<std::uint8_t> out, std::size_t& n) {
    if (key != "squawk") return false;
    // Pragmatic: accept Bool (true=armed-squawk default high) or
    // StringRef "system_is_armed"/"system_is_disarmed". Squawk
    // info: state(0=armed/1=disarmed) | strobe<<4 | level<<6.
    std::uint8_t state = 0;
    if (input.type == ValueType::Bool) state = input.b ? 0 : 1;
    else if (input.type == ValueType::StringRef && input.str) {
        if (std::strcmp(input.str, "system_is_armed") == 0)    state = 0;
        else if (std::strcmp(input.str, "system_is_disarmed") == 0) state = 1;
        else return false;
    } else if (input.type == ValueType::Uint) state = static_cast<std::uint8_t>(input.u);
    else return false;

    if (out.size() < 4) return false;
    out[0] = 0x01;                          // fc cluster-specific
    out[1] = 0x00;                          // tsn
    out[2] = 0x02;                          // cmd squawk
    // info: state | (strobe=1)<<4 | (level=very_high=3)<<6
    out[3] = state | (1u << 4) | (3u << 6);
    n = 4;
    return true;
}
}  // namespace

extern const TzConverter kTzIasWdWarningSimple{
    .key         = "alarm",
    .cluster     = "ssIasWd",
    .cluster_id  = 0x0502,
    .command_id  = 0x00,
    .fn          = tz_ias_wd_warning_simple,
    .user_config = nullptr,
};

extern const TzConverter kTzIasWdSquawk{
    .key         = "squawk",
    .cluster     = "ssIasWd",
    .cluster_id  = 0x0502,
    .command_id  = 0x02,
    .fn          = tz_ias_wd_squawk,
    .user_config = nullptr,
};

// ── ColorCtrl extras: enhanced_move_to_hue_and_saturation, color_loop_set ─

namespace {

bool fz_cmd_enhanced_move_to_hue_and_sat(const DecodedMessage& msg,
                                            const FzConverter&,
                                            const PreparedDefinition&,
                                            RuntimeContext&,
                                            FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Body: enhancehue u16le + saturation u8 + transtime u16le.
    const std::size_t off = msg.manufacturer_specific ? 5u : 3u;
    if (msg.raw_data.size() < off + 5) return false;
    const std::uint16_t enh =
        static_cast<std::uint16_t>(msg.raw_data[off]) |
        (static_cast<std::uint16_t>(msg.raw_data[off + 1]) << 8);
    const std::uint8_t sat = msg.raw_data[off + 2];
    const std::uint16_t tt =
        static_cast<std::uint16_t>(msg.raw_data[off + 3]) |
        (static_cast<std::uint16_t>(msg.raw_data[off + 4]) << 8);

    Value a{}; a.type = ValueType::StringRef;
    a.str = "enhanced_move_to_hue_and_saturation";
    out.put("action", a);

    Value e{}; e.type = ValueType::Uint; e.u = enh;
    out.put("action_enhanced_hue", e);

    Value h{}; h.type = ValueType::Uint;
    h.u = static_cast<std::uint32_t>(
        (static_cast<std::uint64_t>(enh) * 360u) / 65535u);
    out.put("action_hue", h);

    Value s{}; s.type = ValueType::Uint; s.u = sat;
    out.put("action_saturation", s);

    Value t{}; t.type = ValueType::Float;
    t.f = static_cast<float>(tt) / 10.0f;
    out.put("action_transition_time", t);
    return true;
}

constexpr const char* kColorLoopSetActionLut[] = {
    "deactivate",                                  // 0
    "activate_from_color_loop_start_enhanced_hue", // 1
    "activate_from_enhanced_current_hue",          // 2
};

bool fz_cmd_color_loop_set(const DecodedMessage& msg, const FzConverter&,
                             const PreparedDefinition&, RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Body: updateflags u8 + action u8 + direction u8 + time u16le +
    //       startHue u16le.
    const std::size_t off = msg.manufacturer_specific ? 5u : 3u;
    if (msg.raw_data.size() < off + 1) return false;
    const std::uint8_t flags = msg.raw_data[off];
    Value f{}; f.type = ValueType::Uint; f.u = flags;
    out.put("action_update_flags", f);

    if (msg.raw_data.size() >= off + 2) {
        const std::uint8_t act = msg.raw_data[off + 1];
        Value a{}; a.type = ValueType::StringRef;
        a.str = act < (sizeof(kColorLoopSetActionLut)/sizeof(kColorLoopSetActionLut[0]))
                ? kColorLoopSetActionLut[act] : "color_loop_set";
        out.put("action", a);
    }
    return true;
}

bool fz_tint_scene(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext& ctx,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genBasic.write of attr 0x4005 — z2m emits action = "scene_<n>".
    const Value* v = msg.payload.find("16389");
    if (!v || v->type != ValueType::Uint) return false;
    char tmp[24];
    const int n = std::snprintf(tmp, sizeof(tmp), "scene_%u",
                                  static_cast<unsigned>(v->u));
    if (n <= 0) return false;
    const std::size_t slen =
        static_cast<std::size_t>(n) >= sizeof(tmp) ? sizeof(tmp) - 1
                                                    : static_cast<std::size_t>(n);
    const char* s = ctx.alloc_str(tmp, slen);
    if (!s) return false;
    Value a{}; a.type = ValueType::StringRef; a.str = s;
    out.put("action", a);
    return true;
}

}  // namespace

extern const FzConverter kFzCommandEnhancedMoveToHueAndSat{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x43,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_enhanced_move_to_hue_and_sat },
    .user_config       = nullptr,
};

extern const FzConverter kFzCommandColorLoopSet{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x44,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_color_loop_set },
    .user_config       = nullptr,
};

// genBasic write of attr 0x4005 (16389) — Tint scene picker. z2m
// `fz.tint_scene` equivalent. Emits action = "scene_<n>".
extern const FzConverter kFzTintScene{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = type_bit(MessageType::Write),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_tint_scene },
    .user_config       = nullptr,
};

#undef ZHC_IAS_TYPED_CVT

// ── power_on_behavior (genOnOff attr 0x4003) ─────────────────────────

namespace {

bool fz_pob1(const DecodedMessage& msg, const FzConverter&,
              const PreparedDefinition&, RuntimeContext&,
              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Attr id 0x4003 = 16387 — payload key formatted as decimal.
    const Value* v = msg.payload.find("16387");
    if (!v || v->type != ValueType::Uint) return false;
    const char* label = nullptr;
    switch (v->u) {
        case 0:   label = "off";      break;
        case 1:   label = "on";       break;
        case 2:   label = "toggle";   break;
        case 255: label = "previous"; break;
        default:  return false;
    }
    Value o{}; o.type = ValueType::StringRef; o.str = label;
    out.put("power_on_behavior", o);
    return true;
}

bool tz_pob1(std::string_view key, const Value& input, const TzConverter&,
              const PreparedDefinition&, RuntimeContext&,
              std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    if (key != "power_on_behavior") return false;

    std::uint8_t val = 0xFF;
    if (input.type == ValueType::StringRef && input.str) {
        if (std::strcmp(input.str, "off")      == 0) val = 0;
        else if (std::strcmp(input.str, "on")  == 0) val = 1;
        else if (std::strcmp(input.str, "toggle")   == 0) val = 2;
        else if (std::strcmp(input.str, "previous") == 0) val = 0xFF;
        else return false;
    } else if (input.type == ValueType::Uint) {
        // SPA's device.attr.set path forwards an int32 — accept 0/1/2/255.
        val = static_cast<std::uint8_t>(input.u == 0xFF || input.u <= 2 ? input.u : 0xFF);
        if (val != 0 && val != 1 && val != 2 && val != 0xFF) return false;
    } else {
        return false;
    }

    if (out_frame.size() < 8) return false;
    // ZCL writeAttributes: FC=0x10 (profile-wide, disable-default-response
    // off). TSN fill-in by adapter. CmdID=0x02.
    // Body: attrId LE (2), dataType u8, value u8.
    out_frame[0] = 0x10; out_frame[1] = 0; out_frame[2] = 0x02;
    out_frame[3] = 0x03; out_frame[4] = 0x40;     // attr 0x4003 LE
    out_frame[5] = 0x30;                          // dataType enum8
    out_frame[6] = val;
    out_size = 7;
    return true;
}

}  // namespace

extern const FzConverter kFzPowerOnBehavior1{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_pob1 },
    .user_config       = nullptr,
};

extern const TzConverter kTzPowerOnBehavior1{
    .key         = "power_on_behavior",
    .cluster     = "genOnOff",
    .cluster_id  = 0x0006,
    .command_id  = 0x02,        // Write Attributes
    .fn          = tz_pob1,
    .user_config = nullptr,
};

// ── genOnOff onWithTimedOff (cmd 0x42) — countdown setter ────────────
//
// z2m-source: lib/tuya.ts tuyaTz.on_off_countdown.
// Body per ZCL spec: ctrlbits(1) + ontime(2 LE) + offwaittime(2 LE).
// z2m mirrors `ontime` into `offwaittime` per Tuya doc.

namespace {

bool tz_on_with_timed_off(std::string_view key, const Value& input,
                            const TzConverter&,
                            const PreparedDefinition&, RuntimeContext&,
                            std::span<std::uint8_t> out_frame,
                            std::size_t& out_size) {
    out_size = 0;
    if (key != "countdown") return false;

    // Range 0..43200s (12h). 0 cancels a running countdown.
    std::uint32_t seconds = 0;
    if (input.type == ValueType::Uint) {
        seconds = static_cast<std::uint32_t>(input.u);
    } else if (input.type == ValueType::Int) {
        if (input.i < 0) return false;
        seconds = static_cast<std::uint32_t>(input.i);
    } else {
        return false;
    }
    if (seconds > 43200) return false;

    if (out_frame.size() < 8) return false;
    // FC=0x11 (cluster-specific cmd + disable-default-response). TSN
    // patched by adapter. CmdID=0x42.
    const std::uint16_t t = static_cast<std::uint16_t>(seconds);
    out_frame[0] = 0x11;
    out_frame[1] = 0;
    out_frame[2] = 0x42;
    out_frame[3] = 0x00;                                   // ctrlbits
    out_frame[4] = static_cast<std::uint8_t>(t & 0xFF);    // ontime LE
    out_frame[5] = static_cast<std::uint8_t>((t >> 8) & 0xFF);
    out_frame[6] = static_cast<std::uint8_t>(t & 0xFF);    // offwaittime LE
    out_frame[7] = static_cast<std::uint8_t>((t >> 8) & 0xFF);
    out_size = 8;
    return true;
}

}  // namespace

extern const TzConverter kTzOnWithTimedOff{
    .key         = "countdown",
    .cluster     = "genOnOff",
    .cluster_id  = 0x0006,
    .command_id  = 0x42,        // onWithTimedOff
    .fn          = tz_on_with_timed_off,
    .user_config = nullptr,
};

// ── eWeLink action (SNZB-01 family) ──────────────────────────────────

namespace {

bool fz_ewelink_action(const DecodedMessage& msg, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const char* label = nullptr;
    switch (msg.command_id) {
        case 0x00: label = "double"; break;
        case 0x01: label = "single"; break;
        case 0x02: label = "long";   break;
        default:   return false;
    }
    return emit_action(label, out);
}

}  // namespace

extern const FzConverter kFzEwelinkAction{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ewelink_action },
    .user_config       = nullptr,
};

// ── IKEA TRADFRI arrow buttons (genScenes custom cmds 0x07/0x08/0x09) ─

namespace {

bool fz_ikea_arrow_click(const DecodedMessage& msg, const FzConverter&,
                           const PreparedDefinition&, RuntimeContext&,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 2) return false;
    const std::uint16_t v = static_cast<std::uint16_t>(msg.raw_data[off]) |
                             (static_cast<std::uint16_t>(msg.raw_data[off + 1]) << 8);
    if (v == 2) return false;   // spurious on toggle hold
    const char* label = (v == 257) ? "arrow_left_click" : "arrow_right_click";
    return emit_action(label, out);
}

bool fz_ikea_arrow_hold(const DecodedMessage& msg, const FzConverter&,
                          const PreparedDefinition&, RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 2) return false;
    const std::uint16_t v = static_cast<std::uint16_t>(msg.raw_data[off]) |
                             (static_cast<std::uint16_t>(msg.raw_data[off + 1]) << 8);
    const char* label = (v == 3329) ? "arrow_left_hold" : "arrow_right_hold";
    return emit_action(label, out);
}

bool fz_ikea_arrow_release(const DecodedMessage& msg, const FzConverter&,
                             const PreparedDefinition&, RuntimeContext&,
                             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    emit_action("arrow_release", out);
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() >= off + 2) {
        const std::uint16_t ms = static_cast<std::uint16_t>(msg.raw_data[off]) |
                                  (static_cast<std::uint16_t>(msg.raw_data[off + 1]) << 8);
        Value dur{}; dur.type = ValueType::Float; dur.f = static_cast<float>(ms) / 1000.0f;
        out.put("action_duration", dur);
    }
    return true;
}

}  // namespace

extern const FzConverter kFzIkeaArrowClick{
    .family            = FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x07,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ikea_arrow_click },
    .user_config       = nullptr,
};

extern const FzConverter kFzIkeaArrowHold{
    .family            = FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x08,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ikea_arrow_hold },
    .user_config       = nullptr,
};

extern const FzConverter kFzIkeaArrowRelease{
    .family            = FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x09,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_ikea_arrow_release },
    .user_config       = nullptr,
};

// ── lightingColorCtrl StepColorTemperature (cmd 0x4C) ───────────────

namespace {

bool fz_cmd_step_color_temp(const DecodedMessage& msg, const FzConverter&,
                              const PreparedDefinition&, RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload: step_mode u8, step_size u16 LE, transition_time u16 LE,
    // (then min/max mireds we ignore). Some firmware omits the min/max
    // tail — guard accordingly.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 5) return false;
    const std::uint8_t  mode = msg.raw_data[off];
    const std::uint16_t size = static_cast<std::uint16_t>(msg.raw_data[off + 1]) |
                                (static_cast<std::uint16_t>(msg.raw_data[off + 2]) << 8);
    const std::uint16_t tt   = static_cast<std::uint16_t>(msg.raw_data[off + 3]) |
                                (static_cast<std::uint16_t>(msg.raw_data[off + 4]) << 8);

    const char* label = (mode == 0x01) ? "color_temperature_step_up"
                       : (mode == 0x03) ? "color_temperature_step_down"
                                          : nullptr;
    if (!label) return false;

    emit_action(label, out);

    Value sz{}; sz.type = ValueType::Uint; sz.u = size;
    out.put("action_step_size", sz);

    Value tf{}; tf.type = ValueType::Float; tf.f = static_cast<float>(tt) / 10.0f;
    out.put("action_transition_time", tf);
    return true;
}

}  // namespace

extern const FzConverter kFzCommandStepColorTemp{
    .family            = FrameFamily::Zcl,
    .cluster           = "lightingColorCtrl",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x4C,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_step_color_temp },
    .user_config       = nullptr,
};

// ── Philips Hue dimmer (manuSpecificPhilips 0xFC00 cmd 0x00) ──────────

namespace {

bool fz_hue_dimmer_notification(const DecodedMessage& msg, const FzConverter&,
                                   const PreparedDefinition&, RuntimeContext&,
                                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload after 5-byte manu-specific ZCL header:
    //   [0]      button  u8  (1 on, 2 up, 3 down, 4 off)
    //   [1..3]   unknown u24 (skipped)
    //   [4]      type    u8  (0 press, 1 hold, 2 press_release,
    //                          3 hold_release)
    //   [5]      unknown u8, [6] time u8, [7] unknown u8
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 5) return false;
    const std::uint8_t button = msg.raw_data[off];
    const std::uint8_t type   = msg.raw_data[off + 4];
    if (button < 1 || button > 4 || type > 3) return false;

    // 4 buttons × 4 types = 16 stable string literals.
    static constexpr const char* kLabels[4][4] = {
        { "on_press",   "on_hold",   "on_press_release",   "on_hold_release"   },
        { "up_press",   "up_hold",   "up_press_release",   "up_hold_release"   },
        { "down_press", "down_hold", "down_press_release", "down_hold_release" },
        { "off_press",  "off_hold",  "off_press_release",  "off_hold_release"  },
    };
    return emit_action(kLabels[button - 1][type], out);
}

}  // namespace

extern const FzConverter kFzHueDimmerNotification{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificPhilips",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x00,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_hue_dimmer_notification },
    .user_config       = nullptr,
};

// ── genScenes Recall (cmd 0x05) ──────────────────────────────────────

namespace {

bool fz_cmd_recall(const DecodedMessage& msg, const FzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Payload: group u16 LE, scene u8.
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 3) return false;
    const std::uint8_t scene = msg.raw_data[off + 2];
    // Stable literals for 0..15 (typical scene ID range). Out-of-band
    // scenes fall through without emitting — this is rare in practice.
    static constexpr const char* kLabels[16] = {
        "recall_0",  "recall_1",  "recall_2",  "recall_3",
        "recall_4",  "recall_5",  "recall_6",  "recall_7",
        "recall_8",  "recall_9",  "recall_10", "recall_11",
        "recall_12", "recall_13", "recall_14", "recall_15",
    };
    if (scene >= 16) return false;
    return emit_action(kLabels[scene], out);
}

}  // namespace

extern const FzConverter kFzCommandRecall{
    .family            = FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x05,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_recall },
    .user_config       = nullptr,
};

// genScenes commandStore (cmd 0x04). z2m fz.command_store equivalent —
// emits action="store_<scene>" from u16 group + u8 scene_id body.
namespace {

bool fz_cmd_store(const DecodedMessage& msg, const FzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const std::size_t off = zcl_hdr_len(msg);
    if (msg.raw_data.size() < off + 3) return false;
    const std::uint8_t scene = msg.raw_data[off + 2];
    static constexpr const char* kLabels[16] = {
        "store_0",  "store_1",  "store_2",  "store_3",
        "store_4",  "store_5",  "store_6",  "store_7",
        "store_8",  "store_9",  "store_10", "store_11",
        "store_12", "store_13", "store_14", "store_15",
    };
    if (scene >= 16) return false;
    return emit_action(kLabels[scene], out);
}

}  // namespace

extern const FzConverter kFzCommandStore{
    .family            = FrameFamily::Zcl,
    .cluster           = "genScenes",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x04,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_cmd_store },
    .user_config       = nullptr,
};

// ── window covering (closuresWindowCovering 0x0102) ──────────────────

namespace {

bool tz_cover_state(std::string_view key, const Value& input,
                     const TzConverter&, const PreparedDefinition&,
                     RuntimeContext&, std::span<std::uint8_t> out_frame,
                     std::size_t& out_size) {
    if (key != "state") return false;

    std::uint8_t cmd = 0xFF;
    if (input.type == ValueType::StringRef && input.str) {
        // Case-insensitive match — z2m sends uppercase OPEN/CLOSE/STOP.
        auto ieq = [](const char* s, const char* target) {
            while (*s && *target) {
                char a = *s, b = *target;
                if (a >= 'a' && a <= 'z') a -= 32;
                if (b >= 'a' && b <= 'z') b -= 32;
                if (a != b) return false;
                ++s; ++target;
            }
            return *s == 0 && *target == 0;
        };
        if      (ieq(input.str, "OPEN"))  cmd = 0x00;
        else if (ieq(input.str, "CLOSE")) cmd = 0x01;
        else if (ieq(input.str, "STOP"))  cmd = 0x02;
        else return false;
    } else if (input.type == ValueType::Uint) {
        // SPA path: 0=OPEN, 1=CLOSE, 2=STOP — matches the underlying
        // ZCL cluster-specific command IDs.
        if      (input.u == 0) cmd = 0x00;
        else if (input.u == 1) cmd = 0x01;
        else if (input.u == 2) cmd = 0x02;
        else return false;
    } else {
        return false;
    }

    // FC = 0x11 (cluster-specific + disable default response), TSN
    // placeholder, cmd, no payload.
    if (out_frame.size() < 3) return false;
    out_frame[0] = 0x11; out_frame[1] = 0; out_frame[2] = cmd;
    out_size = 3;
    return true;
}

bool tz_cover_lift(std::string_view key, const Value& input,
                    const TzConverter&, const PreparedDefinition&,
                    RuntimeContext&, std::span<std::uint8_t> out_frame,
                    std::size_t& out_size) {
    if (key != "position") return false;
    if (input.type != ValueType::Uint && input.type != ValueType::Int) return false;
    std::int64_t pct = (input.type == ValueType::Uint)
                        ? static_cast<std::int64_t>(input.u)
                        : input.i;
    if (pct < 0 || pct > 100) return false;

    if (out_frame.size() < 4) return false;
    out_frame[0] = 0x11; out_frame[1] = 0; out_frame[2] = 0x05;
    out_frame[3] = static_cast<std::uint8_t>(pct);
    out_size = 4;
    return true;
}

// closuresWindowCovering cmd 0x08 (goToTiltPercentage). z2m
// tz.cover_position_tilt equivalent — accepts `tilt` (Uint/Int 0..100).
bool tz_cover_tilt(std::string_view key, const Value& input,
                    const TzConverter&, const PreparedDefinition&,
                    RuntimeContext&, std::span<std::uint8_t> out_frame,
                    std::size_t& out_size) {
    if (key != "tilt") return false;
    if (input.type != ValueType::Uint && input.type != ValueType::Int) return false;
    std::int64_t pct = (input.type == ValueType::Uint)
                        ? static_cast<std::int64_t>(input.u)
                        : input.i;
    if (pct < 0 || pct > 100) return false;

    if (out_frame.size() < 4) return false;
    out_frame[0] = 0x11; out_frame[1] = 0; out_frame[2] = 0x08;
    out_frame[3] = static_cast<std::uint8_t>(pct);
    out_size = 4;
    return true;
}

}  // namespace

extern const TzConverter kTzCoverState{
    .key         = "state",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x00,        // runtime-selected (open/close/stop)
    .fn          = tz_cover_state,
    .user_config = nullptr,
};

extern const TzConverter kTzCoverPositionLift{
    .key         = "position",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x05,        // goToLiftPercentage
    .fn          = tz_cover_lift,
    .user_config = nullptr,
};

extern const TzConverter kTzCoverPositionTilt{
    .key         = "tilt",
    .cluster     = "closuresWindowCovering",
    .cluster_id  = 0x0102,
    .command_id  = 0x08,        // goToTiltPercentage
    .fn          = tz_cover_tilt,
    .user_config = nullptr,
};

// ── Outbound encoders ───────────────────────────────────────────────

namespace {

// ZCL frame-control byte for a cluster-specific c→s command with the
// default-response bit suppressed. Matches z2m's convention.
constexpr std::uint8_t kCmdFc = 0x11;

bool write_header(std::span<std::uint8_t> out, std::uint8_t cmd_id,
                   std::size_t payload_len, std::size_t& out_size) {
    const std::size_t total = 3 + payload_len;
    if (out.size() < total) return false;
    out[0] = kCmdFc;
    out[1] = 0x00;         // TSN — platform adapter patches.
    out[2] = cmd_id;
    out_size = total;
    return true;
}

}  // namespace

bool tz_on_off(std::string_view key, const Value& input,
                const TzConverter&,
                const PreparedDefinition&, RuntimeContext&,
                std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "state") return false;

    // Accept Bool, Uint (0/1/2), Int (same), and StringRef ("ON"/"OFF"/
    // "TOGGLE"). The UI's `device.attr.set` path ultimately arrives as
    // Uint (`attr.val` is int32_t cast to uint64), so narrowing to Bool
    // only was silently rejecting every toggle from the SPA.
    std::uint8_t cmd;
    if (input.type == ValueType::Bool) {
        cmd = input.b ? 0x01 : 0x00;
    } else if (input.type == ValueType::Uint) {
        cmd = (input.u == 2) ? 0x02 : (input.u ? 0x01 : 0x00);
    } else if (input.type == ValueType::Int) {
        cmd = (input.i == 2) ? 0x02 : (input.i != 0 ? 0x01 : 0x00);
    } else if (input.type == ValueType::StringRef && input.str) {
        const char* s = input.str;
        if      (std::strcmp(s, "ON")     == 0) cmd = 0x01;
        else if (std::strcmp(s, "OFF")    == 0) cmd = 0x00;
        else if (std::strcmp(s, "TOGGLE") == 0) cmd = 0x02;
        else return false;
    } else {
        return false;
    }
    return write_header(out_frame, cmd, /*payload_len=*/0, out_size);
}

extern const TzConverter kTzOnOff{
    .key          = "state",
    .cluster      = "genOnOff",
    .cluster_id   = 0x0006,
    .command_id   = 0x00,    // encoded per-request (on/off/toggle)
    .fn           = tz_on_off,
    .user_config  = nullptr,
};

bool tz_brightness(std::string_view key, const Value& input,
                    const TzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "brightness") return false;
    if (input.type != ValueType::Uint) return false;
    if (input.u > 254) return false;

    // moveToLevelWithOnOff (0x04) — payload: level u8, transtime u16 LE.
    if (!write_header(out_frame, 0x04, /*payload_len=*/3, out_size)) {
        return false;
    }
    out_frame[3] = static_cast<std::uint8_t>(input.u);
    out_frame[4] = 0x00;     // transition time LSB
    out_frame[5] = 0x00;     // transition time MSB
    return true;
}

extern const TzConverter kTzBrightness{
    .key          = "brightness",
    .cluster      = "genLevelCtrl",
    .cluster_id   = 0x0008,
    .command_id   = 0x04,
    .fn           = tz_brightness,
    .user_config  = nullptr,
};

bool tz_color_temp(std::string_view key, const Value& input,
                    const TzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;
    if (key != "color_temp") return false;
    if (input.type != ValueType::Uint) return false;
    if (input.u > 0xFFFF) return false;

    // moveToColorTemp (0x0A) — payload: colortemp u16 LE, transtime u16 LE.
    if (!write_header(out_frame, 0x0A, /*payload_len=*/4, out_size)) {
        return false;
    }
    const std::uint16_t mireds = static_cast<std::uint16_t>(input.u);
    out_frame[3] = static_cast<std::uint8_t>(mireds & 0xFF);
    out_frame[4] = static_cast<std::uint8_t>((mireds >> 8) & 0xFF);
    out_frame[5] = 0x00;
    out_frame[6] = 0x00;
    return true;
}

extern const TzConverter kTzColorTemp{
    .key          = "color_temp",
    .cluster      = "lightingColorCtrl",
    .cluster_id   = 0x0300,
    .command_id   = 0x0A,
    .fn           = tz_color_temp,
    .user_config  = nullptr,
};

// ── tz_color (lightingColorCtrl 0x0300, multi-key) ───────────────────
//
// Single wildcard TzConverter that claims `color_x` / `color_y` /
// `hue` / `saturation`. Each key encodes a different ZCL command on
// the same cluster. See header docs for input shape contract.
namespace {

// Convert a Value-shaped color axis to a u16 ZCL CIE x/y. Accepts
// Float in [0,1] (z2m's normalised shape) or Uint already in [0,
// 65535]. Returns false on type mismatch / out-of-range.
bool color_axis_to_u16(const Value& v, std::uint16_t& out) {
    if (v.type == ValueType::Float) {
        if (v.f < 0.0f || v.f > 1.0f) return false;
        out = static_cast<std::uint16_t>(v.f * 65535.0f + 0.5f);
        return true;
    }
    if (v.type == ValueType::Uint) {
        if (v.u > 0xFFFF) return false;
        out = static_cast<std::uint16_t>(v.u);
        return true;
    }
    return false;
}

bool color_u8_arg(const Value& v, std::uint8_t& out) {
    // Accept Uint in [0, 254]. Float is also tolerated since the SPA
    // sometimes ships hue/saturation as fractions; clamp + scale.
    if (v.type == ValueType::Uint) {
        if (v.u > 254) return false;
        out = static_cast<std::uint8_t>(v.u);
        return true;
    }
    if (v.type == ValueType::Float) {
        if (v.f < 0.0f || v.f > 254.0f) return false;
        out = static_cast<std::uint8_t>(v.f + 0.5f);
        return true;
    }
    return false;
}

}  // namespace

bool tz_color(std::string_view key, const Value& input,
               const TzConverter&,
               const PreparedDefinition&, RuntimeContext&,
               std::span<std::uint8_t> out_frame, std::size_t& out_size) {
    out_size = 0;

    if (key == "color_x" || key == "color_y") {
        std::uint16_t axis = 0;
        if (!color_axis_to_u16(input, axis)) return false;
        // moveToColor (0x07): x u16 LE, y u16 LE, transition u16 LE.
        if (!write_header(out_frame, 0x07, /*payload_len=*/6, out_size)) {
            return false;
        }
        // The other axis isn't carried in the same write — default
        // to 0x8000 (centre of the [0,1] range). Real clients should
        // either send both axes back-to-back or use a vendor-specific
        // composite encoder.
        const std::uint16_t kCentre = 0x8000;
        const std::uint16_t x = (key == "color_x") ? axis : kCentre;
        const std::uint16_t y = (key == "color_y") ? axis : kCentre;
        out_frame[3] = static_cast<std::uint8_t>(x & 0xFF);
        out_frame[4] = static_cast<std::uint8_t>((x >> 8) & 0xFF);
        out_frame[5] = static_cast<std::uint8_t>(y & 0xFF);
        out_frame[6] = static_cast<std::uint8_t>((y >> 8) & 0xFF);
        out_frame[7] = 0x00;
        out_frame[8] = 0x00;
        return true;
    }

    if (key == "hue") {
        std::uint8_t hue = 0;
        if (!color_u8_arg(input, hue)) return false;
        // moveToHue (0x00): hue u8, direction u8 (0=shortest), transition u16 LE.
        if (!write_header(out_frame, 0x00, /*payload_len=*/4, out_size)) {
            return false;
        }
        out_frame[3] = hue;
        out_frame[4] = 0x00;
        out_frame[5] = 0x00;
        out_frame[6] = 0x00;
        return true;
    }

    if (key == "saturation") {
        std::uint8_t sat = 0;
        if (!color_u8_arg(input, sat)) return false;
        // moveToSaturation (0x03): sat u8, transition u16 LE.
        if (!write_header(out_frame, 0x03, /*payload_len=*/3, out_size)) {
            return false;
        }
        out_frame[3] = sat;
        out_frame[4] = 0x00;
        out_frame[5] = 0x00;
        return true;
    }

    return false;
}

// `key=nullptr` makes this a wildcard — `tz_color` itself decides
// which keys to honour. `command_id` is left at moveToColor (0x07)
// for the descriptor; the actual command is encoded per-call.
extern const TzConverter kTzColor{
    .key          = nullptr,
    .cluster      = "lightingColorCtrl",
    .cluster_id   = 0x0300,
    .command_id   = 0x07,
    .fn           = tz_color,
    .user_config  = nullptr,
};

// ── tz_lock (closuresDoorLock 0x0101) ───────────────────────────────
//
// Accepts "state" StringRef "LOCK" / "UNLOCK" → cmd 0x00 / 0x01.
bool tz_lock(std::string_view key, const Value& input,
              const TzConverter&,
              const PreparedDefinition&, RuntimeContext&,
              std::span<std::uint8_t> out_frame,
              std::size_t& out_size) {
    out_size = 0;
    if (key != "state") return false;
    std::uint8_t cmd;
    if (input.type == ValueType::StringRef && input.str) {
        if      (std::strcmp(input.str, "LOCK")   == 0) cmd = 0x00;
        else if (std::strcmp(input.str, "UNLOCK") == 0) cmd = 0x01;
        else return false;
    } else if (input.type == ValueType::Bool) {
        cmd = input.b ? 0x00 : 0x01;   // true = LOCK
    } else if (input.type == ValueType::Uint) {
        cmd = input.u ? 0x00 : 0x01;   // 1 = LOCK, 0 = UNLOCK
    } else {
        return false;
    }
    return write_header(out_frame, cmd, /*payload_len=*/0, out_size);
}

extern const TzConverter kTzLock{
    .key          = "state",
    .cluster      = "closuresDoorLock",
    .cluster_id   = 0x0101,
    .command_id   = 0x00,
    .fn           = tz_lock,
    .user_config  = nullptr,
};

// ── tz_thermostat (hvacThermostat 0x0201 writeAttributes) ──────────
//
// Accepts the two most common thermostat keys and encodes a standard
// ZCL writeAttributes (cmd 0x02) frame:
//   "occupied_heating_setpoint" / "current_heating_setpoint"
//        → attr 0x0012, int16, wire is 0.01 °C units.
//        UNIT CONTRACT: caller supplies degrees Celsius (Float OR Int).
//          - Float 21.5 → wire 2150 (21.50 °C)
//          - Int   21   → wire 2100 (21.00 °C)
//          - Uint  21   → wire 2100 (21.00 °C)
//        This DIFFERS from `kTzMinHeatSetpointLimit` and friends, which
//        accept already-scaled deci-°C int (1500 = 15.00 °C) via the
//        generic `tz_zcl_write_attr` path. Watch the unit when wiring
//        a Lua rule against either family.
//   "system_mode"   → attr 0x001C, enum8 (off=0/auto=1/cool=3/heat=4)
bool tz_thermostat(std::string_view key, const Value& input,
                    const TzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    std::span<std::uint8_t> out_frame,
                    std::size_t& out_size) {
    out_size = 0;
    std::uint16_t attr_id;
    std::uint8_t  attr_type;
    std::uint32_t val;
    std::size_t   vlen;

    if (key == "occupied_heating_setpoint" ||
        key == "current_heating_setpoint") {
        attr_id = 0x0012;
        attr_type = 0x29;  // int16
        vlen = 2;
        // x100 scaling lives here only (caller supplies °C, wire is
        // deci-°C). See unit-contract header comment above.
        std::int32_t raw;
        if (input.type == ValueType::Int)   raw = static_cast<std::int32_t>(input.i * 100);
        else if (input.type == ValueType::Uint)  raw = static_cast<std::int32_t>(input.u * 100);
        else if (input.type == ValueType::Float) raw = static_cast<std::int32_t>(input.f * 100);
        else return false;
        val = static_cast<std::uint32_t>(raw) & 0xFFFF;
    } else if (key == "system_mode") {
        attr_id = 0x001C;
        attr_type = 0x30;  // enum8
        vlen = 1;
        if (input.type == ValueType::StringRef && input.str) {
            const char* s = input.str;
            if      (std::strcmp(s, "off")  == 0) val = 0;
            else if (std::strcmp(s, "auto") == 0) val = 1;
            else if (std::strcmp(s, "cool") == 0) val = 3;
            else if (std::strcmp(s, "heat") == 0) val = 4;
            else return false;
        } else if (input.type == ValueType::Uint) {
            val = static_cast<std::uint32_t>(input.u);
        } else return false;
    } else {
        return false;
    }

    // writeAttributes frame: fc + tsn + cmd(0x02) + attrId LE(2)
    //   + attrType(1) + value(vlen)
    const std::size_t payload = 2 + 1 + vlen;
    if (!write_header(out_frame, 0x02, payload, out_size)) return false;
    out_frame[3] = static_cast<std::uint8_t>(attr_id & 0xFF);
    out_frame[4] = static_cast<std::uint8_t>((attr_id >> 8) & 0xFF);
    out_frame[5] = attr_type;
    for (std::size_t i = 0; i < vlen; ++i) {
        out_frame[6 + i] = static_cast<std::uint8_t>((val >> (i * 8)) & 0xFF);
    }
    return true;
}

extern const TzConverter kTzThermostat{
    .key          = nullptr,
    .cluster      = "hvacThermostat",
    .cluster_id   = 0x0201,
    .command_id   = 0x02,
    .fn           = tz_thermostat,
    .user_config  = nullptr,
};

// ── tz_warning (ssIasWd 0x0502) ─────────────────────────────────────
//
// Minimal startWarning (cmd 0x00) encoder. Payload is 1-byte warning-
// mode/strobe/siren-level bitmap + 2-byte duration (big-endian). We
// accept:
//   input Uint        — packed duration (seconds); mode defaults to
//                       "burglar" (0x13), strobe on, siren max.
//   input Bool true   — same as above, duration 10s.
//   input Bool false  — stop: mode 0x00, duration 0.
bool tz_warning(std::string_view key, const Value& input,
                 const TzConverter&,
                 const PreparedDefinition&, RuntimeContext&,
                 std::span<std::uint8_t> out_frame,
                 std::size_t& out_size) {
    out_size = 0;
    if (key != "warning") return false;
    std::uint8_t  mode = 0x13;   // burglar + strobe + level-3
    std::uint16_t dur  = 10;
    std::uint8_t  strobe_dc = 0;
    std::uint8_t  strobe_lv = 0;
    if (input.type == ValueType::Bool) {
        if (!input.b) { mode = 0x00; dur = 0; }
    } else if (input.type == ValueType::Uint) {
        dur = static_cast<std::uint16_t>(input.u > 0xFFFF ? 0xFFFF : input.u);
        if (dur == 0) mode = 0x00;
    } else return false;
    if (!write_header(out_frame, 0x00, /*payload_len=*/5, out_size)) {
        return false;
    }
    out_frame[3] = mode;
    out_frame[4] = static_cast<std::uint8_t>(dur & 0xFF);
    out_frame[5] = static_cast<std::uint8_t>((dur >> 8) & 0xFF);
    out_frame[6] = strobe_dc;
    out_frame[7] = strobe_lv;
    return true;
}

extern const TzConverter kTzWarning{
    .key          = "warning",
    .cluster      = "ssIasWd",
    .cluster_id   = 0x0502,
    .command_id   = 0x00,
    .fn           = tz_warning,
    .user_config  = nullptr,
};

// ── tz_cover_position (closuresWindowCovering 0x0102) ───────────────
//
// Accepts:
//   "position" (Uint 0–100)          → goToLiftPercentage (0x05)
//   "state"    (StringRef "OPEN"|    → upOpen (0x00) / downClose (0x01)
//               "CLOSE"|"STOP")         / stop (0x02)
bool tz_cover_position(std::string_view key, const Value& input,
                        const TzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        std::span<std::uint8_t> out_frame,
                        std::size_t& out_size) {
    out_size = 0;
    if (key == "position") {
        if (input.type != ValueType::Uint || input.u > 100) return false;
        if (!write_header(out_frame, 0x05, /*payload_len=*/1, out_size)) {
            return false;
        }
        out_frame[3] = static_cast<std::uint8_t>(input.u);
        return true;
    }
    if (key == "state") {
        std::uint8_t cmd;
        if (input.type == ValueType::StringRef && input.str) {
            if      (std::strcmp(input.str, "OPEN")  == 0) cmd = 0x00;
            else if (std::strcmp(input.str, "CLOSE") == 0) cmd = 0x01;
            else if (std::strcmp(input.str, "STOP")  == 0) cmd = 0x02;
            else return false;
        } else if (input.type == ValueType::Uint) {
            // SPA path: 0=OPEN, 1=CLOSE, 2=STOP — matches ZCL cmd IDs.
            if      (input.u == 0) cmd = 0x00;
            else if (input.u == 1) cmd = 0x01;
            else if (input.u == 2) cmd = 0x02;
            else return false;
        } else {
            return false;
        }
        return write_header(out_frame, cmd, /*payload_len=*/0, out_size);
    }
    return false;
}

extern const TzConverter kTzCoverPosition{
    .key          = nullptr,  // claims "position" + "state"
    .cluster      = "closuresWindowCovering",
    .cluster_id   = 0x0102,
    .command_id   = 0x05,
    .fn           = tz_cover_position,
    .user_config  = nullptr,
};

// ── tz_zcl_write_attr — generic writeAttributes encoder ──────────

namespace {

std::size_t zcl_type_len(std::uint8_t t) {
    switch (t) {
        case 0x10: return 1;   // boolean
        case 0x18: return 1;   // 8-bit bitmap
        case 0x19: return 2;   // 16-bit bitmap
        case 0x20: return 1;   // u8
        case 0x21: return 2;   // u16
        case 0x22: return 3;   // u24
        case 0x23: return 4;   // u32
        case 0x28: return 1;   // s8
        case 0x29: return 2;   // s16
        case 0x30: return 1;   // enum8
        default:   return 0;
    }
}

bool zcl_coerce(const Value& in, const ZclWriteSpec& s,
                 std::uint32_t& out) {
    if (in.type == ValueType::Bool) { out = in.b ? 1 : 0; return true; }
    if (in.type == ValueType::Uint) { out = static_cast<std::uint32_t>(in.u); return true; }
    if (in.type == ValueType::Int)  { out = static_cast<std::uint32_t>(in.i); return true; }
    if (in.type == ValueType::StringRef && in.str && s.lookup) {
        for (std::uint8_t i = 0; i < s.lookup_count; ++i) {
            if (s.lookup[i].label &&
                std::strcmp(s.lookup[i].label, in.str) == 0) {
                out = s.lookup[i].value; return true;
            }
        }
    }
    return false;
}

}  // namespace

bool tz_zcl_write_attr(std::string_view key,
                        const Value& input,
                        const TzConverter& self,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        std::span<std::uint8_t> out_frame,
                        std::size_t& out_size) {
    out_size = 0;
    const auto* spec = static_cast<const ZclWriteSpec*>(self.user_config);
    if (!spec) return false;
    if (spec->key && key != spec->key) return false;

    std::uint32_t v = 0;
    if (!zcl_coerce(input, *spec, v)) return false;
    if ((spec->flags & kZclWriteFlagInvertBool) &&
        input.type == ValueType::Bool) {
        v = v ? 0 : 1;
    }
    const std::size_t vlen = zcl_type_len(spec->attr_type);
    if (vlen == 0) return false;

    const bool manu = spec->manufacturer_code != 0;
    // fc: bit 2 = manu-specific, bit 4 = disable-default-response.
    const std::uint8_t fc = manu ? 0x14 : 0x10;
    const std::size_t total = 1 + (manu ? 2 : 0) + 1 + 1 + 2 + 1 + vlen;
    if (out_frame.size() < total) return false;

    std::size_t p = 0;
    out_frame[p++] = fc;
    if (manu) {
        out_frame[p++] = static_cast<std::uint8_t>(spec->manufacturer_code & 0xFF);
        out_frame[p++] = static_cast<std::uint8_t>((spec->manufacturer_code >> 8) & 0xFF);
    }
    out_frame[p++] = 0x00;                  // tsn placeholder
    out_frame[p++] = 0x02;                  // cmd = writeAttributes
    out_frame[p++] = static_cast<std::uint8_t>(spec->attr_id & 0xFF);
    out_frame[p++] = static_cast<std::uint8_t>((spec->attr_id >> 8) & 0xFF);
    out_frame[p++] = spec->attr_type;
    for (std::size_t i = 0; i < vlen; ++i) {
        out_frame[p++] = static_cast<std::uint8_t>((v >> (i * 8)) & 0xFF);
    }
    out_size = total;
    return true;
}

// ── fz_lock (closuresDoorLock 0x0101) ───────────────────────────────
//
// Emits "lock_state" from attribute 0x0000 (LockState, enum8):
//   0 = not fully locked, 1 = locked, 2 = unlocked.

bool fz_lock(const DecodedMessage& msg,
              const FzConverter&,
              const PreparedDefinition&,
              RuntimeContext&,
              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("lock_state", o);
    return true;
}

extern const FzConverter kFzLock{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresDoorLock",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lock },
    .user_config       = nullptr,
};

// ── fz_ias_zone ─────────────────────────────────────────────────────
//
// ssIasZone (0x0500). Decodes ZoneStatus (attr 0x0002, u16):
//   bit 0 — alarm1       → "alarm" (Bool)
//   bit 2 — tamper       → "tamper"
//   bit 3 — battery_low  → "battery_low"
// Semantic key ("water_leak", "contact", "smoke", …) is device-
// specific; the generic emits "alarm" and callers rename via an
// outer layer (per-device z2m key lives in the `exposes[]`).

bool fz_ias_zone(const DecodedMessage& msg,
                  const FzConverter&,
                  const PreparedDefinition&,
                  RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0002 — ZoneStatus (u16).
    const Value* v = msg.payload.find("2");
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint32_t s = static_cast<std::uint32_t>(v->u);
    Value a{};  a.type  = ValueType::Bool; a.b  = (s & 0x0001) != 0;
    Value tp{}; tp.type = ValueType::Bool; tp.b = (s & 0x0004) != 0;
    Value bl{}; bl.type = ValueType::Bool; bl.b = (s & 0x0008) != 0;
    out.put("alarm", a);
    out.put("tamper", tp);
    out.put("battery_low", bl);
    return true;
}

extern const FzConverter kFzIasZone{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse) |
                         type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ias_zone },
    .user_config       = nullptr,
};

// ── fz_ias_zone_config (sensitivity + keep_time) ────────────────────
//
// Tuya PIR family (ZG-204Z / IH012-RT01 / ZMS-102 / …) configures
// sensitivity + PIR keep-time via two ssIasZone attributes:
//
//   attr 0x0013 currentZoneSensitivityLevel (ENUM8): 0=low / 1=medium / 2=high
//   attr 0xF001 (61441, manuSpec, ENUM8):            0=30s / 1=60s / 2=120s
//
// Mirrors z2m `fz.ZM35HQ_attr`. The decoder runs the dispatcher with an
// empty AttrKeyEntry table so attr ids surface as decimal-string keys
// in `msg.payload` (`"19"` and `"61441"`). Both keys are independent;
// either or both may be present in a single AttributeReport / ReadResponse.
//
// Output uses StringRef so the SPA's enum picker can render the values
// directly; the matching TZ uses `tz_zcl_write_attr` with a string
// lookup table for symmetry.

bool fz_ias_zone_config(const DecodedMessage& msg,
                         const FzConverter&,
                         const PreparedDefinition&,
                         RuntimeContext&,
                         FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    if (const Value* v = msg.payload.find("19");
        v && v->type == ValueType::Uint && v->u < 3) {
        static const char* const kSens[3] = { "low", "medium", "high" };
        Value o{}; o.type = ValueType::StringRef; o.str = kSens[v->u];
        out.put("sensitivity", o);
        emitted = true;
    }
    if (const Value* v = msg.payload.find("61441");
        v && v->type == ValueType::Uint && v->u < 3) {
        static const char* const kKeep[3] = { "30", "60", "120" };
        Value o{}; o.type = ValueType::StringRef; o.str = kKeep[v->u];
        out.put("keep_time", o);
        emitted = true;
    }
    return emitted;
}

extern const FzConverter kFzIasZoneConfig{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ias_zone_config },
    .user_config       = nullptr,
};

// ── fz_fan_mode (hvacFanCtrl 0x0202) ────────────────────────────────
//
// Emits "fan_mode" (u8 enum) from attribute 0x0000 (FanMode).

bool fz_fan_mode(const DecodedMessage& msg,
                  const FzConverter&,
                  const PreparedDefinition&,
                  RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("fan_mode", o);
    return true;
}

extern const FzConverter kFzFanMode{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacFanCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_fan_mode },
    .user_config       = nullptr,
};

// ── fz_cover_position ───────────────────────────────────────────────
//
// closuresWindowCovering (0x0102). Emits "position" from attribute
// 0x0008 (CurrentPositionLiftPercentage, u8). No tilt variant yet.
//
// z2m-source: fromZigbee.ts `cover_position_tilt` (lift half).

bool fz_cover_position(const DecodedMessage& msg,
                        const FzConverter&,
                        const PreparedDefinition&,
                        RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("8");
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("position", o);
    return true;
}

extern const FzConverter kFzCoverPosition{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresWindowCovering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_cover_position },
    .user_config       = nullptr,
};

// ── fz_metering (seMetering 0x0702) ────────────────────────────────
//
// Emits "energy" (kWh, from CurrentSummDelivered /1000 via Int) and
// "power" (W, from InstantaneousDemand) as raw Int pass-through.
// Device-specific divisors live in the meta-aware path.

bool fz_metering(const DecodedMessage& msg,
                  const FzConverter&,
                  const PreparedDefinition&,
                  RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    // attr 0x0000 — CurrentSummDelivered (u48).
    if (const Value* v = msg.payload.find("0")) {
        if (v->type == ValueType::Uint) {
            Value e{}; e.type = ValueType::Uint; e.u = v->u;
            out.put("energy", e); emitted = true;
        }
    }
    // attr 0x0400 — InstantaneousDemand (s24).
    if (const Value* v = msg.payload.find("1024")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value p{}; p.type = ValueType::Int;
            p.i = v->type == ValueType::Int ? v->i
                                              : static_cast<std::int64_t>(v->u);
            out.put("power", p); emitted = true;
        }
    }
    return emitted;
}

extern const FzConverter kFzMetering{
    .family            = FrameFamily::Zcl,
    .cluster           = "seMetering",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_metering },
    .user_config       = nullptr,
};

// ── fz_electrical_measurement (haElectricalMeasurement 0x0B04) ──────
//
// Emits "power" (W), "voltage" (V), "current" (A) from their
// RMS attrs as raw Int pass-through.

bool fz_electrical_measurement(const DecodedMessage& msg,
                                 const FzConverter&,
                                 const PreparedDefinition&,
                                 RuntimeContext&,
                                 FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    // attr 0x050B — ActivePower (s16).
    if (const Value* v = msg.payload.find("1291")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value p{}; p.type = ValueType::Int;
            p.i = v->type == ValueType::Int ? v->i
                                              : static_cast<std::int64_t>(v->u);
            out.put("power", p); emitted = true;
        }
    }
    // attr 0x0505 — RMSVoltage (u16).
    if (const Value* v = msg.payload.find("1285")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("voltage", o); emitted = true;
        }
    }
    // attr 0x0508 — RMSCurrent (u16).
    if (const Value* v = msg.payload.find("1288")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("current", o); emitted = true;
        }
    }
    return emitted;
}

extern const FzConverter kFzElectricalMeasurement{
    .family            = FrameFamily::Zcl,
    .cluster           = "haElectricalMeasurement",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_electrical_measurement },
    .user_config       = nullptr,
};

// ── fz_thermostat (hvacThermostat 0x0201) ───────────────────────────
//
// Emits "local_temperature" (s16, 1/100 °C raw), "occupied_heating_
// setpoint" (s16), "system_mode" (u8 enum raw).

bool fz_thermostat(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    // attr 0x0000 — LocalTemperature (s16).
    if (const Value* v = msg.payload.find("0")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = v->type == ValueType::Int ? v->i
                                              : static_cast<std::int64_t>(v->u);
            out.put("local_temperature", o); emitted = true;
        }
    }
    // attr 0x0012 — OccupiedHeatingSetpoint (s16).
    if (const Value* v = msg.payload.find("18")) {
        if (v->type == ValueType::Int || v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Int;
            o.i = v->type == ValueType::Int ? v->i
                                              : static_cast<std::int64_t>(v->u);
            out.put("current_heating_setpoint", o); emitted = true;
        }
    }
    // attr 0x001C — SystemMode (u8 enum).
    if (const Value* v = msg.payload.find("28")) {
        if (v->type == ValueType::Uint) {
            Value o{}; o.type = ValueType::Uint; o.u = v->u;
            out.put("system_mode", o); emitted = true;
        }
    }
    return emitted;
}

extern const FzConverter kFzThermostat{
    .family            = FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_thermostat },
    .user_config       = nullptr,
};

// ── Lock programming/operation event decoders + PIN encoder ──────
//
// Closes z2m fz.lock_programming_event / fz.lock_operation_event /
// tz.pincode_lock. Used cross-vendor by datek 0402946, onesti
// easyCodeTouch_v1 / Nimly, and any future closuresDoorLock device.

namespace {

constexpr const char* kProgEventCodeLut[] = {
    "unknown",              "master_code_changed",
    "pin_code_added",       "pin_code_deleted",
    "pin_code_changed",     "rfid_code_added",
    "rfid_code_deleted",
};

constexpr const char* kOpEventCodeLut[] = {
    "unknown",                              "lock",
    "unlock",                               "lock_failure_invalid_pin_or_id",
    "lock_failure_invalid_schedule",        "unlock_failure_invalid_pin_or_id",
    "unlock_failure_invalid_schedule",      "one_touch_lock",
    "key_lock",                             "key_unlock",
    "auto_lock",                            "schedule_lock",
    "schedule_unlock",                      "manual_lock",
    "manual_unlock",                        "non_access_user_event",
};

constexpr const char* kLockSourceLut[] = {
    "keypad", "rf", "manual", "rfid", "indeterminate",
};

bool fz_lock_event_common(const DecodedMessage& msg,
                           const char* const* code_lut,
                           std::size_t code_lut_n,
                           FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // Body: src:u8 code:u8 user_id:u16le ...  (rest unused).
    if (msg.raw_body.size() < 4) return false;
    const std::uint8_t  src  = msg.raw_body[0];
    const std::uint8_t  code = msg.raw_body[1];
    const std::uint16_t user_id =
        static_cast<std::uint16_t>(msg.raw_body[2]) |
        (static_cast<std::uint16_t>(msg.raw_body[3]) << 8);
    Value a{}; a.type = ValueType::StringRef;
    a.str = code < code_lut_n ? code_lut[code] : "unknown";
    out.put("action", a);
    Value u{}; u.type = ValueType::Uint; u.u = user_id;
    out.put("action_user", u);
    Value sn{}; sn.type = ValueType::Uint; sn.u = src;
    out.put("action_source", sn);
    if (src < sizeof(kLockSourceLut)/sizeof(kLockSourceLut[0])) {
        Value s{}; s.type = ValueType::StringRef;
        s.str = kLockSourceLut[src];
        out.put("action_source_name", s);
    }
    return true;
}

bool fz_lock_programming_event(const DecodedMessage& msg,
                                const FzConverter&,
                                const PreparedDefinition&,
                                RuntimeContext&,
                                FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return fz_lock_event_common(msg, kProgEventCodeLut,
        sizeof(kProgEventCodeLut)/sizeof(kProgEventCodeLut[0]), out);
}

bool fz_lock_operation_event(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    return fz_lock_event_common(msg, kOpEventCodeLut,
        sizeof(kOpEventCodeLut)/sizeof(kOpEventCodeLut[0]), out);
}

// closuresDoorLock setPinCode (cmd 0x05) / clearPinCode (cmd 0x07).
// Input: StringRef "<user_id>:<pin_digits>" — empty pin clears slot.
// user_status defaults to enabled (1), user_type unrestricted (0).
bool tz_lock_pin_code(std::string_view key, const Value& input,
                       const TzConverter&,
                       const PreparedDefinition&,
                       RuntimeContext&,
                       std::span<std::uint8_t> out, std::size_t& n) {
    if (key != "pin_code") return false;
    if (input.type != ValueType::StringRef || !input.str) return false;
    const char* s = input.str;
    std::uint16_t user = 0;
    bool have_digit = false;
    while (*s >= '0' && *s <= '9') {
        user = static_cast<std::uint16_t>(user * 10 + (*s - '0'));
        ++s; have_digit = true;
    }
    if (!have_digit || *s != ':') return false;
    ++s;
    const char* pin = s;
    std::size_t pin_len = 0;
    while (pin[pin_len] != '\0' && pin_len < 32) ++pin_len;
    if (pin[pin_len] != '\0') return false;
    if (pin_len == 0) {
        if (out.size() < 5) return false;
        out[0] = 0x01;  out[1] = 0x00;  out[2] = 0x07;  // clearPinCode
        out[3] = static_cast<std::uint8_t>(user & 0xFF);
        out[4] = static_cast<std::uint8_t>((user >> 8) & 0xFF);
        n = 5; return true;
    }
    const std::size_t total = 3 + 2 + 1 + 1 + 1 + pin_len;
    if (out.size() < total) return false;
    std::size_t p = 0;
    out[p++] = 0x01;  out[p++] = 0x00;  out[p++] = 0x05;  // setPinCode
    out[p++] = static_cast<std::uint8_t>(user & 0xFF);
    out[p++] = static_cast<std::uint8_t>((user >> 8) & 0xFF);
    out[p++] = 0x01;   // user_status: enabled
    out[p++] = 0x00;   // user_type:   unrestricted
    out[p++] = static_cast<std::uint8_t>(pin_len);
    for (std::size_t i = 0; i < pin_len; ++i) {
        out[p++] = static_cast<std::uint8_t>(pin[i]);
    }
    n = p; return true;
}

// closuresDoorLock soundVolume (attr 0x0024, ENUM8). z2m lookup.
constexpr ZclWriteLookup kLockSoundVolumeLut[] = {
    {"silent", 0}, {"low", 1}, {"high", 2},
};
constexpr ZclWriteSpec kLockSoundVolumeSpec{
    "sound_volume", 0x0024, 0x30, 0,
    kLockSoundVolumeLut,
    sizeof(kLockSoundVolumeLut)/sizeof(kLockSoundVolumeLut[0]),
};

}  // namespace

extern const FzConverter kFzLockProgrammingEvent{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresDoorLock",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x20,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lock_programming_event },
    .user_config       = nullptr,
};

extern const FzConverter kFzLockOperationEvent{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresDoorLock",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x21,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lock_operation_event },
    .user_config       = nullptr,
};

extern const TzConverter kTzLockPinCode{
    .key         = "pin_code",
    .cluster     = "closuresDoorLock",
    .cluster_id  = 0x0101,
    .command_id  = 0x05,
    .fn          = tz_lock_pin_code,
    .user_config = nullptr,
};

extern const TzConverter kTzLockSoundVolume{
    .key         = "sound_volume",
    .cluster     = "closuresDoorLock",
    .cluster_id  = 0x0101,
    .command_id  = 0x02,
    .fn          = tz_zcl_write_attr,
    .user_config = &kLockSoundVolumeSpec,
};

// closuresDoorLock autoRelockTime (attr 0x0023, UINT32 seconds).
// z2m tz.lock_auto_relock_time + tz.easycode_auto_relock both write
// this attribute (the latter as bool→0/1; tz_zcl_write_attr's Bool
// coercion handles that automatically).
namespace {
constexpr ZclWriteSpec kLockAutoRelockTimeSpec{
    "auto_relock_time", 0x0023, 0x23 /* UINT32 */, 0, nullptr, 0,
};
}  // namespace

extern const TzConverter kTzLockAutoRelockTime{
    .key         = "auto_relock_time",
    .cluster     = "closuresDoorLock",
    .cluster_id  = 0x0101,
    .command_id  = 0x02,
    .fn          = tz_zcl_write_attr,
    .user_config = &kLockAutoRelockTimeSpec,
};

// closuresDoorLock commandGetUserStatusRsp (cmd 0x05 response).
// z2m fz.lock_user_status_response decodes via constants.lockUserStatus
// lookup. Body per ZCL spec:
//   userid     u16 LE
//   userstatus u8  enum
namespace {

bool fz_lock_user_status_response(const DecodedMessage& msg,
                                    const FzConverter&,
                                    const PreparedDefinition&,
                                    RuntimeContext&,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.size() < 3) return false;
    const std::uint16_t uid =
        static_cast<std::uint16_t>(msg.raw_body[0]) |
        (static_cast<std::uint16_t>(msg.raw_body[1]) << 8);
    const std::uint8_t status_raw = msg.raw_body[2];

    Value u{}; u.type = ValueType::Uint; u.u = uid;
    out.put("user_id", u);

    Value s{}; s.type = ValueType::Uint; s.u = status_raw;
    out.put("user_status_raw", s);

    // Common z2m statuses → friendly labels. Anything else stays
    // exposed via `user_status_raw` for the caller to interpret.
    const char* label = nullptr;
    if (status_raw == 0) label = "available";
    else if (status_raw == 1) label = "enabled";
    else if (status_raw == 3) label = "disabled";
    if (label) {
        Value sl{}; sl.type = ValueType::StringRef; sl.str = label;
        out.put("user_status", sl);
    }
    return true;
}

}  // namespace

extern const FzConverter kFzLockUserStatusResponse{
    .family            = FrameFamily::Zcl,
    .cluster           = "closuresDoorLock",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = 0x05,   // commandGetUserStatusRsp
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lock_user_status_response },
    .user_config       = nullptr,
};

// ssIasWd `maxDuration` (cluster 0x0502 attr 0x0000, UINT16 seconds).
// z2m tz.ias_max_duration / tz.ts0216_duration both write here.
// Also wire a read-side decoder for the same attribute.
namespace {
constexpr ZclWriteSpec kIasWdMaxDurationSpec{
    "max_duration", 0x0000, 0x21 /* UINT16 */, 0, nullptr, 0,
};

bool fz_ias_wd_max_duration(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("max_duration", o);
    return true;
}
}  // namespace

extern const TzConverter kTzIasWdMaxDuration{
    .key         = "max_duration",
    .cluster     = "ssIasWd",
    .cluster_id  = 0x0502,
    .command_id  = 0x02,   // writeAttributes
    .fn          = tz_zcl_write_attr,
    .user_config = &kIasWdMaxDurationSpec,
};

extern const FzConverter kFzIasWdMaxDuration{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasWd",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_ias_wd_max_duration },
    .user_config       = nullptr,
};

// ── msOccupancySensing decoder ──────────────────────────────────────
//
// Closes z2m fz.occupancy. Attribute 0x0000 is `occupancy` (bitmap8).
// Bit 0 is the "currently occupied" flag; z2m emits
// `occupancy: (raw & 1) > 0`. Used cross-vendor on PIR / mmWave
// sensors (Shelly S4SN_0U61X, Phi_D9290030675, Owon PCT504,
// Bacchus Presence_Sensor_v2*, Wirenboard WB-MSW-ZIGBEE-v3/v4,
// imhotepcreation E-Ctrl).

namespace {

bool fz_occupancy(const DecodedMessage& msg,
                   const FzConverter&,
                   const PreparedDefinition&,
                   RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Bool; o.b = (v->u & 0x01u) != 0;
    out.put("occupancy", o);
    return true;
}

}  // namespace

extern const FzConverter kFzOccupancy{
    .family            = FrameFamily::Zcl,
    .cluster           = "msOccupancySensing",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_occupancy },
    .user_config       = nullptr,
};

// ── msCO2 decoder ───────────────────────────────────────────────────
//
// Closes z2m fz.co2. Attribute 0x0000 (MeasuredValue) on the standard
// msCO2 cluster (0x040D) is a single-precision float holding the CO2
// mole fraction (0.0004 == 400 ppm); z2m emits
// `co2: Math.floor(measuredValue * 1000000)` → ppm. We mirror that:
// read the Float, scale by 1e6, floor, emit `co2` as Uint ppm.
// A few firmwares report the value as an already-scaled integer ppm on
// the same attr — pass those through unscaled so we never report a
// wildly wrong 1e6× value. Used by Wirenboard WB-MSW-ZIGBEE-v3/v4 and
// any other vendor binding the standard msCO2 cluster.

namespace {

bool fz_co2(const DecodedMessage& msg,
             const FzConverter&,
             const PreparedDefinition&,
             RuntimeContext&,
             FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 MeasuredValue
    if (!v) return false;
    Value o{}; o.type = ValueType::Uint;
    if (v->type == ValueType::Float) {
        if (v->f < 0.0f) return false;        // invalid / unmeasured
        o.u = static_cast<std::uint64_t>(v->f * 1000000.0f);
    } else if (v->type == ValueType::Uint) {
        o.u = v->u;                           // already ppm
    } else if (v->type == ValueType::Int) {
        if (v->i < 0) return false;
        o.u = static_cast<std::uint64_t>(v->i);
    } else {
        return false;
    }
    out.put("co2", o);
    return true;
}

}  // namespace

extern const FzConverter kFzCO2{
    .family            = FrameFamily::Zcl,
    .cluster           = "msCO2",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_co2 },
    .user_config       = nullptr,
};

// ── hvacThermostat setpoint-limit attribute writes ──────────────────
//
// Closes z2m tz.thermostat_min_heat_setpoint_limit /
// max_heat_setpoint_limit / min_cool_setpoint_limit /
// max_cool_setpoint_limit. Attributes are INT16 in 0.01 °C units;
// UNIT CONTRACT: caller passes the already-scaled int (e.g. 1500 =
// 15.00 °C). DIFFERENT from `tz_thermostat` which takes °C and scales
// internally — see the unit-contract block on tz_thermostat for the
// rationale (limits go through the generic write-attr path).

namespace {
constexpr ZclWriteSpec kSpecMinHeatSetpointLimit{
    "min_heat_setpoint_limit", 0x0015, 0x29, 0, nullptr, 0,
};
constexpr ZclWriteSpec kSpecMaxHeatSetpointLimit{
    "max_heat_setpoint_limit", 0x0016, 0x29, 0, nullptr, 0,
};
constexpr ZclWriteSpec kSpecMinCoolSetpointLimit{
    "min_cool_setpoint_limit", 0x0017, 0x29, 0, nullptr, 0,
};
constexpr ZclWriteSpec kSpecMaxCoolSetpointLimit{
    "max_cool_setpoint_limit", 0x0018, 0x29, 0, nullptr, 0,
};
}  // namespace

extern const TzConverter kTzMinHeatSetpointLimit{
    .key = "min_heat_setpoint_limit", .cluster = "hvacThermostat",
    .cluster_id = 0x0201, .command_id = 0x02,
    .fn = tz_zcl_write_attr, .user_config = &kSpecMinHeatSetpointLimit,
};
extern const TzConverter kTzMaxHeatSetpointLimit{
    .key = "max_heat_setpoint_limit", .cluster = "hvacThermostat",
    .cluster_id = 0x0201, .command_id = 0x02,
    .fn = tz_zcl_write_attr, .user_config = &kSpecMaxHeatSetpointLimit,
};
extern const TzConverter kTzMinCoolSetpointLimit{
    .key = "min_cool_setpoint_limit", .cluster = "hvacThermostat",
    .cluster_id = 0x0201, .command_id = 0x02,
    .fn = tz_zcl_write_attr, .user_config = &kSpecMinCoolSetpointLimit,
};
extern const TzConverter kTzMaxCoolSetpointLimit{
    .key = "max_cool_setpoint_limit", .cluster = "hvacThermostat",
    .cluster_id = 0x0201, .command_id = 0x02,
    .fn = tz_zcl_write_attr, .user_config = &kSpecMaxCoolSetpointLimit,
};

// ── hvacThermostat setpointRaiseLower (cluster cmd 0x00) ────────────
//
// Closes z2m tz.thermostat_setpoint_raise_lower. Body: u8 mode +
// s8 amount-in-0.1°C-steps. mode lookup heat=0 / cool=1 / both=2.
// Input formats:
//   - Int / Uint: amount in 0.1°C steps, mode defaults to both (2)
//   - StringRef "heat:5" / "cool:-3" / "both:10" / "5" — explicit
//     mode prefix optional.

namespace {

bool tz_setpoint_raise_lower(std::string_view key, const Value& input,
                              const TzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              std::span<std::uint8_t> out, std::size_t& n) {
    if (key != "setpoint_raise_lower") return false;
    std::uint8_t mode = 2;       // both
    int amount        = 0;

    if (input.type == ValueType::Int) {
        amount = static_cast<int>(input.i);
    } else if (input.type == ValueType::Uint) {
        amount = static_cast<int>(input.u);
    } else if (input.type == ValueType::StringRef && input.str) {
        const char* s = input.str;
        if (std::strncmp(s, "heat:", 5) == 0) { mode = 0; s += 5; }
        else if (std::strncmp(s, "cool:", 5) == 0) { mode = 1; s += 5; }
        else if (std::strncmp(s, "both:", 5) == 0) { mode = 2; s += 5; }
        bool neg = false;
        if (*s == '-') { neg = true; ++s; }
        else if (*s == '+') { ++s; }
        int v = 0;
        bool have_digit = false;
        while (*s >= '0' && *s <= '9') {
            v = v * 10 + (*s - '0');
            ++s; have_digit = true;
        }
        if (!have_digit) return false;
        amount = neg ? -v : v;
    } else {
        return false;
    }
    if (amount < -128 || amount > 127) return false;
    if (out.size() < 5) return false;
    out[0] = 0x01;        // fc: cluster-specific
    out[1] = 0x00;        // tsn placeholder
    out[2] = 0x00;        // cmd = setpointRaiseLower
    out[3] = mode;
    out[4] = static_cast<std::uint8_t>(static_cast<std::int8_t>(amount));
    n = 5;
    return true;
}

}  // namespace

extern const TzConverter kTzSetpointRaiseLower{
    .key         = "setpoint_raise_lower",
    .cluster     = "hvacThermostat",
    .cluster_id  = 0x0201,
    .command_id  = 0x00,
    .fn          = tz_setpoint_raise_lower,
    .user_config = nullptr,
};

}  // namespace zhc::generic
