// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: shared develco / frient converters.
//
// z2m-source: zigbee-herdsman-converters/src/lib/develco.ts
//             zigbee-herdsman-converters/src/devices/develco.ts

#include "definitions/develco/_shared.hpp"
#include "definitions/_generic/_shared.hpp"   // ZclWriteSpec / tz_zcl_write_attr

#include <cstdint>
#include <cstring>

#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/foundation.hpp"

namespace zhc::develco {

namespace {

// ── attr / cluster ids per z2m develcoModernExtend ────────────────
//
// genBasic           develcoLedControl     ID 0x8100 BITMAP8
// ssIasZone          develcoAlarmOffDelay  ID 0x8001 UINT16
// manuSpecDevelcoAQ  measuredValue (VOC)   ID 0x0000 UINT16, cluster 0xFC03
//
// Note: header comment block cites genBasic 0x8001 for LED control; the
// authoritative z2m source (lib/develco.ts) says 0x8100. We follow z2m.
constexpr std::uint16_t kAttrLedControl       = 0x8100;
constexpr std::uint16_t kAttrAlarmOffDelay    = 0x8001;

// VOC ppb thresholds — see header doc.
constexpr std::uint64_t kVocExcellent = 65;
constexpr std::uint64_t kVocGood      = 220;
constexpr std::uint64_t kVocModerate  = 660;
constexpr std::uint64_t kVocPoor      = 2200;
constexpr std::uint64_t kVocUnhealthy = 5500;

const char* voc_to_air_quality(std::uint64_t ppb) {
    if (ppb <= kVocExcellent) return "excellent";
    if (ppb <= kVocGood)      return "good";
    if (ppb <= kVocModerate)  return "moderate";
    if (ppb <= kVocPoor)      return "poor";
    if (ppb <= kVocUnhealthy) return "unhealthy";
    return "out_of_range";
}

}  // namespace

// ── fz_develco_voc ────────────────────────────────────────────────
bool fz_develco_voc(const DecodedMessage& msg,
                     const FzConverter&,
                     const PreparedDefinition&,
                     RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // attr 0x0000 measuredValue — payload key is "0".
    const Value* v = msg.payload.find("0");
    if (!v || v->type != ValueType::Uint) return false;
    const std::uint64_t ppb = v->u;

    // VOC in µg/m³ — Sensirion SGP3x note: 1 ppb TVOC ≈ 4.5 µg/m³.
    Value voc{}; voc.type = ValueType::Float;
    voc.f = static_cast<float>(ppb) * 4.5f;
    out.put("voc", voc);

    Value aq{}; aq.type = ValueType::StringRef;
    aq.str = voc_to_air_quality(ppb);
    out.put("air_quality", aq);
    return true;
}

extern const FzConverter kFzDevelcoVoc{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificDevelcoAirQuality",
    .type_mask         = type_bit(MessageType::AttributeReport)
                       | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_develco_voc },
    .user_config       = nullptr,
};

// ── fz_develco_led_control ────────────────────────────────────────
bool fz_develco_led_control(const DecodedMessage& msg,
                              const FzConverter&,
                              const PreparedDefinition&,
                              RuntimeContext&,
                              FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // genBasic attr 0x8100 — payload key "33024".
    const Value* v = msg.payload.find("33024");
    if (!v || v->type != ValueType::Uint) return false;

    Value o{}; o.type = ValueType::StringRef;
    switch (v->u) {
        case 0:   o.str = "off";          break;
        case 1:   o.str = "fault_only";   break;
        case 2:   o.str = "motion_only";  break;
        case 255: o.str = "both";         break;
        default:  return false;
    }
    out.put("led_control", o);
    return true;
}

extern const FzConverter kFzDevelcoLedControl{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = type_bit(MessageType::AttributeReport)
                       | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_develco_led_control },
    .user_config       = nullptr,
};

// ── fz_develco_occupancy_timeout ──────────────────────────────────
bool fz_develco_occupancy_timeout(const DecodedMessage& msg,
                                    const FzConverter&,
                                    const PreparedDefinition&,
                                    RuntimeContext&,
                                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // ssIasZone attr 0x8001 — payload key "32769".
    const Value* v = msg.payload.find("32769");
    if (!v || v->type != ValueType::Uint) return false;

    Value o{}; o.type = ValueType::Uint; o.u = v->u;
    out.put("occupancy_timeout", o);
    return true;
}

extern const FzConverter kFzDevelcoOccupancyTimeout{
    .family            = FrameFamily::Zcl,
    .cluster           = "ssIasZone",
    .type_mask         = type_bit(MessageType::AttributeReport)
                       | type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &fz_develco_occupancy_timeout },
    .user_config       = nullptr,
};

// ── Canonical TZ writes ──────────────────────────────────────────
namespace {

// LED control lookup — matches develcoLedControlMap in z2m source.
constexpr ::zhc::generic::ZclWriteLookup kLookupLedControl[] = {
    {"off",         0},
    {"fault_only",  1},
    {"motion_only", 2},
    {"both",        255},
};

// genBasic attr 0x8100 BITMAP8 (ZCL type 0x18), mfg 0x1015.
constexpr ::zhc::generic::ZclWriteSpec kSpecLedControl{
    "led_control",
    kAttrLedControl,
    0x18,                     // BITMAP8
    kManuCodeDevelco,
    kLookupLedControl,
    sizeof(kLookupLedControl) / sizeof(kLookupLedControl[0]),
};

// ssIasZone attr 0x8001 UINT16 (ZCL type 0x21), mfg 0x1015.
// z2m enforces a 5-second floor on the write — adapter / SPA can
// re-enforce; we pass values straight through here.
constexpr ::zhc::generic::ZclWriteSpec kSpecOccupancyTimeout{
    "occupancy_timeout",
    kAttrAlarmOffDelay,
    0x21,                     // UINT16
    kManuCodeDevelco,
    nullptr,
    0,
};

}  // namespace

#define ZHC_DEVELCO_TZ(var, spec_ref, key_str, cluster_str, cluster_id_v) \
    extern const TzConverter var{                                         \
        .key         = key_str,                                           \
        .cluster     = cluster_str,                                       \
        .cluster_id  = cluster_id_v,                                      \
        .command_id  = 0x02,                                              \
        .fn          = &::zhc::generic::tz_zcl_write_attr,                \
        .user_config = &spec_ref,                                         \
    };

ZHC_DEVELCO_TZ(kTzDevelcoLedControl,
                kSpecLedControl,
                "led_control",
                "genBasic",
                0x0000)
ZHC_DEVELCO_TZ(kTzDevelcoOccupancyTimeout,
                kSpecOccupancyTimeout,
                "occupancy_timeout",
                "ssIasZone",
                0x0500)

#undef ZHC_DEVELCO_TZ

}  // namespace zhc::develco
