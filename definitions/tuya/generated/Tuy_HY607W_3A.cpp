// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Tuya HY607W-3A thermostat for gas boiler (`_TZE200_khah2lkr`).
// GRADUATED from the generated ZCL-thermostat stub (no DP map, no time sync)
// to upstream's Tuya DP def. tuyaBase timeStart "1970" (#13676), so the
// time-sync reply counts from 1970 (.tuya_time_start = 1).
// DP16 local_temperature /10; DP50 occupied_heating_setpoint /10; DP125 state;
// DP128 mode_state = first byte of a raw DP {0 manual, 1 auto, 3 temp_override};
// DP102 running_state (true -> heat, else idle).
// z2m-source: tuya.ts #HY607W-3A.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::tuya {
namespace {

using ::zhc::tuya::TuyaDpMapEntry;
using ::zhc::tuya::TuyaEnumEntry;

// z2m extractFirstNumericByte + lookup: raw buffer -> first byte, or a plain number.
bool first_byte_lookup(const TuyaDpMapEntry& e, const Value& raw, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint64_t b = 0;
    if (raw.type == ValueType::BytesRef && !raw.bytes.empty()) b = raw.bytes[0];
    else if (raw.type == ValueType::Uint)                      b = raw.u;
    else if (raw.type == ValueType::Int && raw.i >= 0)         b = static_cast<std::uint64_t>(raw.i);
    else return false;
    for (std::uint8_t i = 0; i < e.enum_count; ++i) {
        if (e.enum_table[i].value == b) {
            Value v{}; v.type = ValueType::StringRef; v.str = e.enum_table[i].label;
            out.put(e.out_key, v);
            return true;
        }
    }
    return false;  // unmapped, as upstream (undefined)
}

constexpr TuyaEnumEntry kMode[]    = {{0, "manual"}, {1, "auto"}, {3, "temp_override"}};
constexpr TuyaEnumEntry kRunning[] = {{0, "idle"}, {1, "heat"}};

struct cfg {
    static constexpr TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(16, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(50, "occupied_heating_setpoint", 10),
        ::zhc::tuya::dp::binary(125, "state"),
        { 128, "mode_state", TuyaDpType::Raw, 1, kMode, 3, 0, 0.0f, &first_byte_lookup, nullptr },
        { 102, "running_state", TuyaDpType::Bool, 1, kRunning, 2, ::zhc::tuya::kTuyaDpFlagBoolEnum },
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg>;

constexpr const char* kModeOpts[]    = {"manual", "auto", "temp_override"};
constexpr const char* kRunningOpts[] = {"idle", "heat"};
constexpr Expose kExposes[] = {
    {"local_temperature",         ExposeType::Numeric, Access::State,    "°C", nullptr, nullptr, 0},
    {"occupied_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0,
     ExposeCategory::State, 5, 35, 0},
    // Read-only here: upstream writes DP128 as a raw [n] payload, which
    // tz_tuya_datapoints cannot send yet.
    {"mode_state",    ExposeType::Enum,   Access::State,    nullptr, "Thermostat mode: manual, auto, or temp_override",
     kModeOpts, 3},
    {"state",         ExposeType::Binary, Access::StateSet, nullptr, "ON/OFF thermostat", nullptr, 0},
    {"running_state", ExposeType::Enum,   Access::State,    nullptr, "State of heating", kRunningOpts, 2},
};

constexpr const char* kModels_HY607W_3A[] = { "TS0601" };
constexpr const char* kManus_HY607W_3A[]  = { "_TZE200_khah2lkr" };

}  // namespace

extern const PreparedDefinition kDef_HY607W_3A{
    .zigbee_models=kModels_HY607W_3A, .zigbee_models_count=sizeof(kModels_HY607W_3A)/sizeof(kModels_HY607W_3A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_HY607W_3A, .manufacturer_names_count=sizeof(kManus_HY607W_3A)/sizeof(kManus_HY607W_3A[0]),
    .model="HY607W-3A", .vendor="Tuya",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .tuya_time_start=1 };

}  // namespace zhc::devices::tuya
