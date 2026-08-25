// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: MultIR HE300_ZB human presence sensor. New upstream in
// z2m v26.97.0.
//
// A radar occupancy sensor whose occupancy attribute is NOT the plain
// IAS/occupancy boolean: msOccupancySensing 0x0000 carries a bitmap
// where bit0 = active motion and bit1 = static presence, which z2m's
// `fzhe300Local.HE300` lowers into two keys —
//   occupancy          bool   (either bit set)
//   human_motion_state enum   none / active / static
// Generic kFzOccupancy would emit occupancy=true for value 2 as well,
// but would drop the active-vs-static distinction the sensor exists to
// report, so a device codec replaces it.
//
// Alongside sit three vendor config attributes on the same cluster,
// decoded here and exposed under the Config category:
//   0xA205 u8   occupancy_distance  motion range, 2..6 m
//   0xA206 u16  unmanned_duration   occupied->unoccupied delay, 0..65535 s
//   0xA203 e8   sensitivity         low / medium / high
//
// DEFERRED: writing those three back. z2m's m.numeric/m.enumLookup
// generate a write-attribute tz per config item; ZHC has no generic
// attribute-write tz to point at, and each would need its own frame
// builder. They are declared State-only until a generic attr-write tz
// lands — reading them is what surfaces the device's current tuning.
//
// z2m-source: multir.ts #HE300_ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::multir {
namespace {

// msOccupancySensing attribute ids in the decimal-string form the
// parser uses for non-standard ids.
constexpr const char* kOccupancyKey  = "0";       // 0x0000
constexpr const char* kSensitivity   = "41475";   // 0xA203
constexpr const char* kDistance      = "41477";   // 0xA205
constexpr const char* kUnmannedDelay = "41478";   // 0xA206

bool as_int(const Value* v, std::int64_t& out) {
    if (!v) return false;
    if      (v->type == ValueType::Uint) out = static_cast<std::int64_t>(v->u);
    else if (v->type == ValueType::Int)  out = v->i;
    else if (v->type == ValueType::Bool) out = v->b ? 1 : 0;
    else return false;
    return true;
}

bool fz_he300(const DecodedMessage& msg,
               const FzConverter&,
               const PreparedDefinition&,
               RuntimeContext&,
               FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    std::int64_t raw = 0;

    if (as_int(msg.payload.find(kOccupancyKey), raw)) {
        // z2m special-cases 0/1/2 then falls through to the same bit
        // test for anything else, so the bit test alone is equivalent.
        const bool active = (raw & 0x01) != 0;
        const bool statik = (raw & 0x02) != 0;

        Value occ{}; occ.type = ValueType::Bool;
        occ.b = active || statik;
        out.put("occupancy", occ);

        Value state{}; state.type = ValueType::StringRef;
        state.str = statik ? "static" : (active ? "active" : "none");
        out.put("human_motion_state", state);
        emitted = true;
    }

    if (as_int(msg.payload.find(kDistance), raw)) {
        Value d{}; d.type = ValueType::Uint;
        d.u = static_cast<std::uint64_t>(raw);
        out.put("occupancy_distance", d);
        emitted = true;
    }

    if (as_int(msg.payload.find(kUnmannedDelay), raw)) {
        Value d{}; d.type = ValueType::Uint;
        d.u = static_cast<std::uint64_t>(raw);
        out.put("unmanned_duration", d);
        emitted = true;
    }

    if (as_int(msg.payload.find(kSensitivity), raw)) {
        const char* label = nullptr;
        switch (raw) {
            case 0: label = "low";    break;
            case 1: label = "medium"; break;
            case 2: label = "high";   break;
            default: label = nullptr; break;
        }
        if (label) {
            Value s{}; s.type = ValueType::StringRef;
            s.str = label;
            out.put("sensitivity", s);
            emitted = true;
        }
    }

    return emitted;
}

const FzConverter kFzHe300{
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
    .fn                = { .zcl_fn = fz_he300 },
    .user_config       = nullptr,
};

const FzConverter* const kFz_HE300_ZB[] = {
    &kFzHe300,
    &::zhc::generic::kFzIlluminance,
};

constexpr const char* kModels_HE300_ZB[] = { "HE300_ZB" };

constexpr const char* kMotionStateValues[] = { "none", "active", "static" };
constexpr const char* kSensitivityValues[] = { "low", "medium", "high" };

constexpr Expose kExposes_HE300_ZB[] = {
    {"occupancy",   ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
    {"illuminance", ExposeType::Numeric, Access::State, "lx",    nullptr, nullptr, 0},
    {"human_motion_state", ExposeType::Enum, Access::State, nullptr,
     "Human Motion State", kMotionStateValues,
     sizeof(kMotionStateValues)/sizeof(kMotionStateValues[0])},
    {"occupancy_distance", ExposeType::Numeric, Access::State, "m",
     "Motion Range Detection (meter)", nullptr, 0,
     ExposeCategory::Config, 2, 6, 1},
    {"unmanned_duration", ExposeType::Numeric, Access::State, "s",
     "Ultrasonic occupied to unoccupied delay (seconds)", nullptr, 0,
     ExposeCategory::Config, 0, 65535, 1},
    {"sensitivity", ExposeType::Enum, Access::State, nullptr,
     "Sensitivity of human presence detection", kSensitivityValues,
     sizeof(kSensitivityValues)/sizeof(kSensitivityValues[0]),
     ExposeCategory::Config},
};

constexpr BindingSpec kBindings_HE300_ZB[] = {
    {1, 0x0400},   // msIlluminanceMeasurement
    {1, 0x0406},   // msOccupancySensing
};

}  // namespace

extern const PreparedDefinition kDef_HE300_ZB{
    .zigbee_models=kModels_HE300_ZB,
    .zigbee_models_count=sizeof(kModels_HE300_ZB)/sizeof(kModels_HE300_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HE300_ZB", .vendor="MultIR",
    .meta=nullptr,
    .exposes=kExposes_HE300_ZB,
    .exposes_count=sizeof(kExposes_HE300_ZB)/sizeof(kExposes_HE300_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HE300_ZB, .from_zigbee_count=sizeof(kFz_HE300_ZB)/sizeof(kFz_HE300_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HE300_ZB,
    .bindings_count=sizeof(kBindings_HE300_ZB)/sizeof(kBindings_HE300_ZB[0]),
};

}  // namespace zhc::devices::multir
