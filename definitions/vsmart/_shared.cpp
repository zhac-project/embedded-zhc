// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared VSmart converters.
//
// z2m-source: zigbee-herdsman-converters/src/devices/vsmart.ts.

#include "definitions/vsmart/_shared.hpp"

#include <cctype>
#include <cstdint>
#include <cstring>

namespace zhc::vsmart {

namespace {

// 8 bits ASCII hex char -> 0..15, or 0xFF on failure.
std::uint8_t hex_nibble(char c) {
    if (c >= '0' && c <= '9') return static_cast<std::uint8_t>(c - '0');
    if (c >= 'a' && c <= 'f') return static_cast<std::uint8_t>(10 + c - 'a');
    if (c >= 'A' && c <= 'F') return static_cast<std::uint8_t>(10 + c - 'A');
    return 0xFF;
}

bool parse_hex_color(const char* s, std::uint8_t out[3]) {
    if (!s) return false;
    if (*s == '#') ++s;
    for (int i = 0; i < 3; ++i) {
        const std::uint8_t hi = hex_nibble(s[2 * i]);
        const std::uint8_t lo = hex_nibble(s[2 * i + 1]);
        if (hi == 0xFF || lo == 0xFF) return false;
        out[i] = static_cast<std::uint8_t>((hi << 4) | lo);
    }
    // Tolerate trailing NUL or any sane terminator after 6 hex chars.
    return s[6] == '\0' || s[6] == ' ' || s[6] == '\t';
}

// Coerce a percentage Value into u8 (0..100). Accepts Uint, Int (>=0),
// and Float in 0..100. Returns false if out of range or wrong type.
bool coerce_percent(const Value& in, std::uint32_t& pct) {
    if (in.type == ValueType::Uint) {
        if (in.u > 100) return false;
        pct = static_cast<std::uint32_t>(in.u);
        return true;
    }
    if (in.type == ValueType::Int) {
        if (in.i < 0 || in.i > 100) return false;
        pct = static_cast<std::uint32_t>(in.i);
        return true;
    }
    if (in.type == ValueType::Float) {
        if (in.f < 0.0f || in.f > 100.0f) return false;
        pct = static_cast<std::uint32_t>(in.f + 0.5f);
        return true;
    }
    return false;
}

// percent (0..100) → byte (0..255), z2m-style round-half-up.
std::uint8_t pct_to_byte(std::uint32_t pct) {
    return static_cast<std::uint8_t>((pct * 255 + 50) / 100);
}

// Stamp the manu-specific cluster-specific ZCL header + raw payload.
// fc = 0x15: cluster-specific (bit0), manu-specific (bit2),
// disable-default-response (bit4). Layout matches z2m's
// `entity.command(..., {manufacturerCode, disableDefaultResponse:true})`.
bool emit_frame(std::span<std::uint8_t> out_frame,
                std::uint8_t cmd,
                const std::uint8_t* payload, std::size_t payload_len,
                std::size_t& out_size) {
    const std::size_t total = 1 /*fc*/ + 2 /*manu*/ + 1 /*tsn*/ + 1 /*cmd*/ + payload_len;
    if (out_frame.size() < total) return false;
    std::size_t p = 0;
    out_frame[p++] = 0x15;
    out_frame[p++] = static_cast<std::uint8_t>(kManufacturerCode & 0xFF);
    out_frame[p++] = static_cast<std::uint8_t>((kManufacturerCode >> 8) & 0xFF);
    out_frame[p++] = 0x00;        // tsn placeholder — filled by adapter
    out_frame[p++] = cmd;
    for (std::size_t i = 0; i < payload_len; ++i) out_frame[p++] = payload[i];
    out_size = total;
    return true;
}

}  // namespace

bool tz_vsmart_raw_cmd(std::string_view key,
                       const Value& input,
                       const TzConverter& self,
                       const PreparedDefinition&,
                       RuntimeContext&,
                       std::span<std::uint8_t> out_frame,
                       std::size_t& out_size) {
    out_size = 0;
    const auto* spec = static_cast<const VsmartCmdSpec*>(self.user_config);
    if (!spec || !spec->key) return false;
    if (key != spec->key) return false;

    switch (spec->encoder) {
        case VsmartEncoder::LedColor: {
            if (input.type != ValueType::StringRef || !input.str) return false;
            std::uint8_t rgb[3];
            if (!parse_hex_color(input.str, rgb)) return false;
            const std::uint8_t payload[4] = {rgb[0], rgb[1], rgb[2], spec->extra};
            return emit_frame(out_frame, spec->command_id, payload, 4, out_size);
        }
        case VsmartEncoder::VibrationIntensity: {
            std::uint32_t pct;
            if (!coerce_percent(input, pct)) return false;
            const std::uint8_t payload[1] = {pct_to_byte(pct)};
            return emit_frame(out_frame, spec->command_id, payload, 1, out_size);
        }
        case VsmartEncoder::LedIntensity: {
            std::uint32_t pct;
            if (!coerce_percent(input, pct)) return false;
            const std::uint8_t payload[3] = {pct_to_byte(pct), spec->extra, 0x00};
            return emit_frame(out_frame, spec->command_id, payload, 3, out_size);
        }
    }
    return false;
}

// ── Per-command Tz spec table ───────────────────────────────────────

namespace {

constexpr VsmartCmdSpec kSpec_LedColorOn{
    "led_indicator_color_on", 0xF3, VsmartEncoder::LedColor, 0x01,
};
constexpr VsmartCmdSpec kSpec_LedColorOff{
    "led_indicator_color_off", 0xF3, VsmartEncoder::LedColor, 0x00,
};
constexpr VsmartCmdSpec kSpec_VibrationIntensity{
    "vibration_intensity", 0xF0, VsmartEncoder::VibrationIntensity, 0x00,
};
constexpr VsmartCmdSpec kSpec_MorningLedIntensity{
    "morning_led_intensity", 0xF2, VsmartEncoder::LedIntensity, 0x00,
};
constexpr VsmartCmdSpec kSpec_EveningLedIntensity{
    "evening_led_intensity", 0xF2, VsmartEncoder::LedIntensity, 0x01,
};
constexpr VsmartCmdSpec kSpec_NightLedIntensity{
    "night_led_intensity", 0xF2, VsmartEncoder::LedIntensity, 0x02,
};

}  // namespace

#define ZHC_VSMART_TZ(var, spec_ref, key_str, cmd_byte)              \
    extern const TzConverter var{                                    \
        .key         = key_str,                                      \
        .cluster     = "vsmartSwitchControl",                        \
        .cluster_id  = kClusterId,                                   \
        .command_id  = cmd_byte,                                     \
        .fn          = &::zhc::vsmart::tz_vsmart_raw_cmd,            \
        .user_config = &spec_ref,                                    \
    };

ZHC_VSMART_TZ(kTzLedIndicatorColorOn,  kSpec_LedColorOn,           "led_indicator_color_on",  0xF3)
ZHC_VSMART_TZ(kTzLedIndicatorColorOff, kSpec_LedColorOff,          "led_indicator_color_off", 0xF3)
ZHC_VSMART_TZ(kTzVibrationIntensity,   kSpec_VibrationIntensity,   "vibration_intensity",     0xF0)
ZHC_VSMART_TZ(kTzMorningLedIntensity,  kSpec_MorningLedIntensity,  "morning_led_intensity",   0xF2)
ZHC_VSMART_TZ(kTzEveningLedIntensity,  kSpec_EveningLedIntensity,  "evening_led_intensity",   0xF2)
ZHC_VSMART_TZ(kTzNightLedIntensity,    kSpec_NightLedIntensity,    "night_led_intensity",     0xF2)
#undef ZHC_VSMART_TZ

}  // namespace zhc::vsmart
