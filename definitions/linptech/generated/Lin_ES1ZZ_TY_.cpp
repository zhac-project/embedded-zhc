// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Linptech ES1ZZ(TY) (TS0225) — mmWave Presence sensor.
// Consolidated port covering all 3 manufacturer fingerprints
// (_TZ3218_awarhusb, _TZ3218_t9ynfz4x, _TZ3218_ewrxirng) + Momax SL12S
// whitelabel. The corresponding Lin__TZ3218_*.cpp stubs are NEUTERED.
// z2m-source: linptech.ts #ES1ZZ(TY).
//
// Wires:
//  - kFzIasMotionAlarm   → fz.ias_occupancy_alarm_1 (occupancy/tamper/battery_low)
//  - fz_tuya_datapoints  → meta.tuyaDatapoints DP 101 fading_time
//  - tuya_base_configure → genTime + tuya magic packet handshake
//
// Tier 3 wired 2026-04-29c:
//  - manuSpecificTuya2 attr decoder for distance / sensitivity / led /
//    presence_keep_time / target_distance attrs.
//  - tzLocal.TS0225 sensitivity / led_indicator / motion_detection_distance
//    writes (attrs 57348 / 57349 / 57353 / 57355).
//
// Still partial:
//  - msIlluminanceMeasurement raw decode (custom byte 6/7 lux formula)
//    requires float `pow` — z2m's `10^((raw-1)/10000)` exponential. ZHC
//    emits the raw u16 measuredValue as `illuminance_raw` instead.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

#include <cstdio>
#include <cstring>

namespace zhc::devices::linptech {
namespace {

// fzLocal.TS0225 — manuSpecificTuya2 (cluster 0xE001) attribute decoder.
// Attr ids: 57345 presence_keep_time, 57348 motion_detection_sensitivity,
//           57349 static_detection_sensitivity, 57353 led_indicator (Bool),
//           57354 target_distance, 57355 motion_detection_distance.
bool fz_es1zz_tuya2(const ::zhc::DecodedMessage& msg,
                     const ::zhc::FzConverter&,
                     const ::zhc::PreparedDefinition&,
                     ::zhc::RuntimeContext&,
                     ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    auto put_uint = [&](const char* key, std::uint16_t attr_id) {
        char buf[8];
        std::snprintf(buf, sizeof(buf), "%u", attr_id);
        const ::zhc::Value* v = msg.payload.find(buf);
        if (!v || v->type != ::zhc::ValueType::Uint) return;
        ::zhc::Value o{}; o.type = ::zhc::ValueType::Uint; o.u = v->u;
        out.put(key, o);
        emitted = true;
    };
    put_uint("presence_keep_time",            57345);
    put_uint("motion_detection_sensitivity",  57348);
    put_uint("static_detection_sensitivity",  57349);
    put_uint("target_distance",               57354);
    put_uint("motion_detection_distance",     57355);
    if (const ::zhc::Value* v = msg.payload.find("57353"); v && v->type == ::zhc::ValueType::Uint) {
        ::zhc::Value o{}; o.type = ::zhc::ValueType::Bool; o.b = v->u != 0;
        out.put("led_indicator", o);
        emitted = true;
    }
    return emitted;
}

constexpr ::zhc::FzConverter kFzES1ZZTuya2{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "manuSpecificTuya2",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_es1zz_tuya2 },
    .user_config       = nullptr,
};

// fzLocal.TS0225_illuminance — emit the raw u16 measuredValue from
// msIlluminanceMeasurement raw frames at byte offset 6/7 of the body.
// z2m converts via `round(10^((raw-1)/10000))`; ZHC keeps raw and lets
// the caller map (avoids float `pow` on-target).
bool fz_es1zz_illuminance(const ::zhc::DecodedMessage& msg,
                            const ::zhc::FzConverter&,
                            const ::zhc::PreparedDefinition&,
                            ::zhc::RuntimeContext&,
                            ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.size() < 8) return false;
    const std::uint16_t raw =
        static_cast<std::uint16_t>(msg.raw_body[6]) |
        (static_cast<std::uint16_t>(msg.raw_body[7]) << 8);
    ::zhc::Value v{}; v.type = ::zhc::ValueType::Uint; v.u = raw;
    out.put("illuminance_raw", v);
    return true;
}

constexpr ::zhc::FzConverter kFzES1ZZIlluminance{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "msIlluminanceMeasurement",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_es1zz_illuminance },
    .user_config       = nullptr,
};

// tzLocal.TS0225 — write 4 attrs on manuSpecificTuya2 (cluster 0xE001).
constexpr ::zhc::generic::ZclWriteSpec kSpecMotionDetectionDistance{
    "motion_detection_distance",     57355, 0x21 /* UINT16 */, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecMotionDetectionSensitivity{
    "motion_detection_sensitivity",  57348, 0x20 /* UINT8  */, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecStaticDetectionSensitivity{
    "static_detection_sensitivity",  57349, 0x20 /* UINT8  */, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLedIndicator{
    "led_indicator",                 57353, 0x10 /* BOOL   */, 0, nullptr, 0,
};

#define ZHC_ES1ZZ_TZ(NAME, KEY, SPEC)                                        \
    constexpr ::zhc::TzConverter NAME{                                       \
        .key         = KEY,                                                  \
        .cluster     = "manuSpecificTuya2",                                  \
        .cluster_id  = 0xE001,                                               \
        .command_id  = 0x02,                                                 \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                   \
        .user_config = &SPEC,                                                \
    }

ZHC_ES1ZZ_TZ(kTzES1ZZMotionDistance,    "motion_detection_distance",
              kSpecMotionDetectionDistance);
ZHC_ES1ZZ_TZ(kTzES1ZZMotionSensitivity, "motion_detection_sensitivity",
              kSpecMotionDetectionSensitivity);
ZHC_ES1ZZ_TZ(kTzES1ZZStaticSensitivity, "static_detection_sensitivity",
              kSpecStaticDetectionSensitivity);
ZHC_ES1ZZ_TZ(kTzES1ZZLedIndicator,      "led_indicator",
              kSpecLedIndicator);

#undef ZHC_ES1ZZ_TZ

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries_ES1ZZ_TY_[] = {
    { 101, "fading_time", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap_ES1ZZ_TY_{
    kEntries_ES1ZZ_TY_,
    sizeof(kEntries_ES1ZZ_TY_)/sizeof(kEntries_ES1ZZ_TY_[0]),
};

constexpr FzConverter kFzDp_ES1ZZ_TY_{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap_ES1ZZ_TY_,
};

constexpr TzConverter kTzDp_ES1ZZ_TY_{
    .key         = nullptr,
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap_ES1ZZ_TY_,
};

const FzConverter* const kFz_ES1ZZ_TY_[] = {
    &::zhc::generic::kFzIasMotionAlarm,
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp_ES1ZZ_TY_,
    &kFzES1ZZTuya2,
    &kFzES1ZZIlluminance,
};
const TzConverter* const kTz_ES1ZZ_TY_[] = {
    &kTzDp_ES1ZZ_TY_,
    &kTzES1ZZMotionDistance,
    &kTzES1ZZMotionSensitivity,
    &kTzES1ZZStaticSensitivity,
    &kTzES1ZZLedIndicator,
};

constexpr const char* kModels_ES1ZZ_TY_[] = { "TS0225" };
constexpr const char* kManus_ES1ZZ_TY_[] = { "_TZ3218_awarhusb", "_TZ3218_t9ynfz4x", "_TZ3218_ewrxirng" };

constexpr WhiteLabel kWhiteLabels_ES1ZZ_TY_[] = {
    {"Momax", "SL12S"},
};

constexpr Expose kAutoExposes[] = {
    {"occupancy",                     ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"tamper",                        ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"battery_low",                   ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
    {"fading_time",                   ExposeType::Numeric, Access::StateSet, "s",     nullptr, nullptr, 0},
    {"target_distance",               ExposeType::Numeric, Access::State,    "cm",    nullptr, nullptr, 0},
    {"motion_detection_distance",     ExposeType::Numeric, Access::StateSet, "cm",    nullptr, nullptr, 0},
    {"motion_detection_sensitivity",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"static_detection_sensitivity",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"presence_keep_time",            ExposeType::Numeric, Access::State,    "min",   nullptr, nullptr, 0},
    {"led_indicator",                 ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"illuminance_raw",               ExposeType::Numeric, Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},   // ssIasZone
    {1, 0xEF00},   // manuSpecificTuya (DP fading_time)
    {1, 0xE001},   // manuSpecificTuya2 (sensitivity / distance / led)
    {1, 0x0400},   // msIlluminanceMeasurement (raw lux register)
};
}  // namespace

extern const PreparedDefinition kDef_ES1ZZ_TY_{
    .zigbee_models=kModels_ES1ZZ_TY_, .zigbee_models_count=sizeof(kModels_ES1ZZ_TY_)/sizeof(kModels_ES1ZZ_TY_[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ES1ZZ_TY_, .manufacturer_names_count=sizeof(kManus_ES1ZZ_TY_)/sizeof(kManus_ES1ZZ_TY_[0]),
    .model="ES1ZZ(TY)", .vendor="Linptech",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=kWhiteLabels_ES1ZZ_TY_, .white_labels_count=sizeof(kWhiteLabels_ES1ZZ_TY_)/sizeof(kWhiteLabels_ES1ZZ_TY_[0]),
    .from_zigbee=kFz_ES1ZZ_TY_, .from_zigbee_count=sizeof(kFz_ES1ZZ_TY_)/sizeof(kFz_ES1ZZ_TY_[0]),
    .to_zigbee=kTz_ES1ZZ_TY_,   .to_zigbee_count=sizeof(kTz_ES1ZZ_TY_)/sizeof(kTz_ES1ZZ_TY_[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::linptech
