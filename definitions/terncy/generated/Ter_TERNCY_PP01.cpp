// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Terncy TERNCY-PP01 — uses shared terncy converters.
// Awareness switch — emits action (single/double/triple/quadruple),
// occupancy via the manuSpecificClusterAduroSmart raw frame +
// motion-side payload, plus temperature (scale 10), illuminance, and
// battery on stock clusters. No outbound writes (toZigbee: []).
// z2m v26.125.0->v26.133.0 (R030): fzLocal.terncy_pp01_motion reads the same
// raw cmd-4 motion frame as terncy_raw: illuminance = uint16 LE at z2m
// data[5..6] (raw_body[0..1], lux) and a PIR bitmap at data[7] (raw_body[2]);
// right = interrupt 0x04 / detected 0x01, left = interrupt 0x20 / detected
// 0x08. A side is published only when its interrupt bit is set. z2m clears a
// side `occupancy_timeout` (90 s) after it went true; the hub's timer only
// clears `occupancy`, so a side stays true until the device reports it clear.
// m.illuminance({scale: 1}) now publishes msIlluminanceMeasurement unscaled.
// action / action_side are Enums (were Numeric).
// z2m-source: terncy.ts #TERNCY-PP01.
#include "definitions/_generic/_shared.hpp"
#include "definitions/terncy/_shared.hpp"

namespace zhc::devices::terncy {
namespace {

bool fz_pp01_motion(const DecodedMessage& msg, const FzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // z2m: data[4] !== 4 || data.length < 8 -> 5-byte manu header + 3 body bytes.
    if (msg.command_id != 4 || msg.raw_body.size() < 3) return false;
    Value lux{}; lux.type = ValueType::Uint;
    lux.u = static_cast<std::uint64_t>(msg.raw_body[0]) |
            (static_cast<std::uint64_t>(msg.raw_body[1]) << 8);
    out.put("illuminance", lux);
    const std::uint8_t pir = msg.raw_body[2];
    struct Side { const char* key; std::uint8_t interrupt; std::uint8_t detected; };
    constexpr Side kSides[] = { {"occupancy_right", 0x04, 0x01}, {"occupancy_left", 0x20, 0x08} };
    for (const Side& side : kSides) {
        if (!(pir & side.interrupt)) continue;
        Value b{}; b.type = ValueType::Bool; b.b = (pir & side.detected) != 0;
        out.put(side.key, b);
    }
    return true;
}

// Same selector as kFzTerncyRawActionMotion (terncy/_shared.cpp).
constexpr FzConverter kFzPp01Motion{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificClusterAduroSmart",
    .type_mask         = type_bit(MessageType::Command) | type_bit(MessageType::Raw),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_pp01_motion },
    .user_config       = nullptr,
};

const FzConverter* const kFz_TERNCY_PP01[] = {
    &::zhc::generic::kFzOccupancy,                 // msOccupancySensing 0x0406
    &::zhc::terncy::kFzTerncyRawActionMotion,      // raw button + motion side
    &kFzPp01Motion,                                // raw motion: left/right + lux
    &::zhc::terncy::kFzTerncyTempScale10,          // 0.1 °C scale
    &::zhc::generic::kFzIlluminanceRaw,            // 0x0400 unscaled (m.illuminance scale 1)
    &::zhc::generic::kFzBatteryNoDivide,           // 0x0001 voltage + battery% (dontDividePercentage, R10 R030)
};

constexpr const char* kActions_TERNCY_PP01[] = { "single", "double", "triple", "quadruple" };
constexpr const char* kSides_TERNCY_PP01[] = { "left", "right" };

constexpr const char* kModels_TERNCY_PP01[] = { "TERNCY-PP01" };

constexpr Expose kAutoExposes[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"occupancy_left",  ExposeType::Binary, Access::State, nullptr, "Motion detected by the left sensor", nullptr, 0},
    {"occupancy_right", ExposeType::Binary, Access::State, nullptr, "Motion detected by the right sensor", nullptr, 0},
    {"action_side", ExposeType::Enum, Access::State, nullptr, "Side of the most recent motion detection",
     kSides_TERNCY_PP01, sizeof(kSides_TERNCY_PP01)/sizeof(kSides_TERNCY_PP01[0])},
    {"action",      ExposeType::Enum, Access::State, nullptr, nullptr,
     kActions_TERNCY_PP01, sizeof(kActions_TERNCY_PP01)/sizeof(kActions_TERNCY_PP01[0])},
    {"temperature", ExposeType::Numeric, Access::State, "°C",    nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr, 0},
    {"battery",     ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0402},   // msTemperatureMeasurement
    {1, 0x0400},   // msIlluminanceMeasurement
    {1, 0x0406},   // msOccupancySensing
    {1, 0xFCCC},   // manuSpecificClusterAduroSmart
};

}  // namespace

extern const PreparedDefinition kDef_TERNCY_PP01{
    .zigbee_models=kModels_TERNCY_PP01,
    .zigbee_models_count=sizeof(kModels_TERNCY_PP01)/sizeof(kModels_TERNCY_PP01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TERNCY-PP01", .vendor="Terncy",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TERNCY_PP01, .from_zigbee_count=sizeof(kFz_TERNCY_PP01)/sizeof(kFz_TERNCY_PP01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    // z2m fz.occupancy_with_timeout: the hub clears occupancy 90 s after the last motion.
    .occupancy_timeout = 90,
};

}  // namespace zhc::devices::terncy
