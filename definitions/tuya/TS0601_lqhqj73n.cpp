// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya TS0601_lqhqj73n outdoor roller blind motor (`_TZE284_lqhqj73n`).
// DP1 coverAction; DP2/DP3 coverPositionInverted (position = 100 - raw, and
// the state derived from it: 0 -> CLOSE, else OPEN); DP5 motor_direction;
// DP7 motor_state {opening, closing}; DP12 motor_fault = bit 0; DP13 battery;
// DP104 obstacle_sensitivity. Writes of `position` hit DP2.
// z2m-source: tuya.ts #TS0601_lqhqj73n.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

using ::zhc::tuya::TuyaDpMapEntry;
using ::zhc::tuya::TuyaEnumEntry;
using Out = FixedPayload<ZHC_FIXED_PAYLOAD_CAP>;

// z2m coverPositionInverted.from: position = 100 - v, plus publish({state}).
// Clamped to [0,100] like the shared kTuyaDpFlagInvertPosition decode.
bool position_inv_state(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&, Out& out) {
    if (raw.type != ValueType::Int) return false;
    std::int64_t v = raw.i < 0 ? 0 : (raw.i > 100 ? 100 : raw.i);
    v = 100 - v;
    Value p{}; p.type = ValueType::Int; p.i = v;
    out.put(e.out_key, p);
    Value s{}; s.type = ValueType::StringRef; s.str = v == 0 ? "CLOSE" : "OPEN";
    out.put("state", s);
    return true;
}

// z2m `(Number(v) & 1) === 1`.
bool bit0(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&, Out& out) {
    std::uint64_t v = 0;
    if (raw.type == ValueType::Uint)      v = raw.u;
    else if (raw.type == ValueType::Int)  v = static_cast<std::uint64_t>(raw.i);
    else if (raw.type == ValueType::Bool) v = raw.b ? 1 : 0;
    else return false;
    Value b{}; b.type = ValueType::Bool; b.b = (v & 1) != 0;
    out.put(e.out_key, b);
    return true;
}

constexpr TuyaEnumEntry kSt[]    = {{0, "OPEN"}, {1, "STOP"}, {2, "CLOSE"}, {3, "CONTINUE"}};
constexpr TuyaEnumEntry kDir[]   = {{0, "normal"}, {1, "reversed"}};
constexpr TuyaEnumEntry kMotor[] = {{0, "opening"}, {1, "closing"}};
constexpr TuyaEnumEntry kObst[]  = {{0, "off"}, {1, "high"}, {2, "medium"}, {3, "low"}};

struct cfg {
    static constexpr TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::enum_lookup(1, "state", kSt, 4),
        { 2, "position", TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition, 0.0f,
          &position_inv_state, nullptr },
        { 3, "position", TuyaDpType::Numeric, 1, nullptr, 0, ::zhc::tuya::kTuyaDpFlagInvertPosition, 0.0f,
          &position_inv_state, nullptr },
        ::zhc::tuya::dp::enum_lookup(5, "motor_direction", kDir, 2),
        ::zhc::tuya::dp::enum_lookup(7, "motor_state", kMotor, 2),
        { 12, "motor_fault", TuyaDpType::Bitmap, 1, nullptr, 0, 0, 0.0f, &bit0, nullptr },
        ::zhc::tuya::dp::numeric(13, "battery"),
        ::zhc::tuya::dp::enum_lookup(104, "obstacle_sensitivity", kObst, 4),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kStOpts[]    = {"OPEN", "CLOSE", "STOP"};
constexpr const char* kDirOpts[]   = {"normal", "reversed"};
constexpr const char* kMotorOpts[] = {"opening", "closing"};
constexpr const char* kObstOpts[]  = {"off", "high", "medium", "low"};
constexpr Expose kExposes[] = {
    {"state",                ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts, 3},
    {"position",             ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0,
     ExposeCategory::State, 0, 100, 0},
    {"motor_direction",      ExposeType::Enum,    Access::StateSet, nullptr, "Motor rotation direction", kDirOpts, 2,
     ExposeCategory::Config},
    {"motor_state",          ExposeType::Enum,    Access::State,    nullptr, "Current motor activity", kMotorOpts, 2},
    {"motor_fault",          ExposeType::Binary,  Access::State,    nullptr, "Motor fault", nullptr, 0},
    {"battery",              ExposeType::Numeric, Access::State,    "%",     nullptr, nullptr, 0},
    {"obstacle_sensitivity", ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kObstOpts, 4},
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE284_lqhqj73n" };

}  // namespace

extern const PreparedDefinition kDef_TS0601_lqhqj73n{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="TS0601_lqhqj73n", .vendor="Tuya",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::tuya
