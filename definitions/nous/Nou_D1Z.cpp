// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nous D1Z DIN-rail smart switch with power monitoring
// (`_TZE204_vfnf7zhq`), new upstream in z2m v26.133.0 (#13698).
//
//   dp6       phaseVariant4 — be16 voltage /10, 24-bit current /1000, 24-bit
//             power. Rides phase_variant2_with_phase: the same 8-byte layout;
//             its negative-power correction only applies at >= 0x800000 W.
//   dp9       circuitBreakerFaults (bit 14 stays "magnetism", unlike the
//             circuitBreakerFaults1 on D4Z / D4Z-M), joined into one string.
//   dp12      reset — publishes "idle" whatever arrives; writing "RESET"
//             sends Bool false, as upstream.
//   dp17/18/126 threshold_4/5/6 — packed [id, enabled, value_be16] records.
//             threshold_5 divides over_current_threshold by 10 and threshold_6
//             lost_flow_threshold by 10, so 18 and 126 use a local expander.
//             Upstream nests them under alarm_set_1/2/3 composites; ZHC has
//             no composite, so the record keys are published flat. READ-ONLY:
//             upstream writes are a read-modify-write over the whole blob (see
//             the D4Z-M header for why that is not done here).
//
// NOT PORTED: dp106 cycle_schedule (schedule codec) and dp109 inching
// (inchingSwitch2 composite) — no codec for either in this tree.
//
// z2m-source: nous.ts #D1Z.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::nous {
namespace {

// z2m `circuitBreakerFaultList`, indexed by bit position.
constexpr const char* kFaultNames[] = {
    "short_circuit", "surge", "overload", "leakage_current",
    "temperature", "fire", "high_power", "self_test",
    "over_current", "unbalance", "over_voltage", "under_voltage",
    "miss_phase", "outage", "magnetism", "credit",
    "no_balance",
};
constexpr ::zhc::tuya::TuyaFaultTable kFaultTable{
    kFaultNames, sizeof(kFaultNames)/sizeof(kFaultNames[0]) };

// dp17 — z2m `alarmSet1ThresholdDefinitions` (threshold_4, no scaling).
constexpr ::zhc::tuya::TuyaThresholdDef kAlarmSet1Defs[] = {
    { 4, "leakage_current_alarm",    "leakage_current_threshold"    },
    { 5, "device_temperature_alarm", "device_temperature_threshold" },
};
constexpr ::zhc::tuya::TuyaThresholdTable kAlarmSet1{
    kAlarmSet1Defs, sizeof(kAlarmSet1Defs)/sizeof(kAlarmSet1Defs[0]) };

// threshold_5 / threshold_6: parseThresholds plus one value divided by 10.
struct ScaledThreshold {
    std::uint8_t id;
    const char*  enabled_key;
    const char*  value_key;   // null for a flag-only record
    std::uint8_t divisor;     // 1 = raw Int, 10 = Float /10
};
struct ScaledThresholdTable {
    const ScaledThreshold* defs;
    std::uint8_t           count;
};

bool expand_scaled_thresholds(const ::zhc::tuya::TuyaDpMapEntry& e, const Value& raw,
                              RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (raw.type != ValueType::BytesRef) return false;
    const auto* t = static_cast<const ScaledThresholdTable*>(e.expand_cfg);
    const auto b = raw.bytes;
    bool emitted = false;
    for (std::size_t i = 0; i + 4 <= b.size(); i += 4) {
        for (std::uint8_t j = 0; j < t->count; ++j) {
            const ScaledThreshold& d = t->defs[j];
            if (d.id != b[i]) continue;       // unknown record id — skip, as upstream
            Value flag{}; flag.type = ValueType::Bool; flag.b = b[i + 1] != 0;
            out.put(d.enabled_key, flag);
            if (d.value_key) {
                const int v = (b[i + 2] << 8) | b[i + 3];
                Value x{};
                if (d.divisor > 1) { x.type = ValueType::Float; x.f = static_cast<float>(v) / d.divisor; }
                else               { x.type = ValueType::Int;   x.i = v; }
                out.put(d.value_key, x);
            }
            emitted = true;
            break;
        }
    }
    return emitted;
}

// dp18 — z2m `alarmSet2ThresholdDefinitions` (threshold_5). Ids 5 and 7 are
// flag-only; upstream exposes only ids 1, 3 and 4 but decodes them all.
constexpr ScaledThreshold kAlarmSet2Defs[] = {
    { 1, "over_current_alarm",             "over_current_threshold",    10 },
    { 2, "unbalanced_load_alarm",          "unbalanced_load_threshold", 1  },
    { 3, "over_voltage_alarm",             "over_voltage_threshold",    1  },
    { 4, "under_voltage_alarm",            "under_voltage_threshold",   1  },
    { 5, "phase_loss_alarm",               nullptr,                     1  },
    { 7, "negative_active_power_alarm",    nullptr,                     1  },
    { 8, "custom_data_reporting_interval", "data_reporting_interval",   1  },
    { 9, "device_locating",                "device_locating_threshold", 1  },
};
constexpr ScaledThresholdTable kAlarmSet2{
    kAlarmSet2Defs, sizeof(kAlarmSet2Defs)/sizeof(kAlarmSet2Defs[0]) };

// dp126 — z2m `alarmSet3ThresholdDefinitions` (threshold_6).
constexpr ScaledThreshold kAlarmSet3Defs[] = {
    { 3, "lost_flow_alarm", "lost_flow_threshold", 10 },
};
constexpr ScaledThresholdTable kAlarmSet3{ kAlarmSet3Defs, 1 };

// z2m tuya.valueConverter.reset.from: always "idle".
bool static_label(const ::zhc::tuya::TuyaDpMapEntry& e, const Value&, RuntimeContext&,
                  FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    Value v{}; v.type = ValueType::StringRef; v.str = static_cast<const char*>(e.expand_cfg);
    out.put(e.out_key, v);
    return true;
}

// z2m tuya.valueConverter.reset.to: "RESET" -> Bool false.
constexpr ::zhc::tuya::TuyaEnumEntry kResetWords[]      = { {0, "RESET"} };
constexpr ::zhc::tuya::TuyaEnumEntry kStatus[]          = { {0, "off"}, {1, "consumption"}, {2, "production"} };
constexpr ::zhc::tuya::TuyaEnumEntry kPowerOnBehavior[] = { {0, "off"}, {1, "on"}, {2, "previous"} };

struct cfg_d1z {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        ::zhc::tuya::dp::numeric(1, "energy", 100),
        ::zhc::tuya::dp::phase_variant2_with_phase(6, &::zhc::tuya::kTuyaPhaseKeysPlain),
        ::zhc::tuya::dp::fault_bitmap(9, "faults", &kFaultTable),
        ::zhc::tuya::dp::binary(11, "prepayment"),
        { 12, "energy_balance_reset", ::zhc::TuyaDpType::Bool, 1, kResetWords, 1,
          ::zhc::tuya::kTuyaDpFlagBoolEnum, 0.0f, &static_label, "idle" },
        ::zhc::tuya::dp::numeric(13, "energy_balance", 100),
        ::zhc::tuya::dp::numeric(14, "energy_balance_add", 100),
        ::zhc::tuya::dp::binary(16, "state"),
        ::zhc::tuya::dp::thresholds(17, &kAlarmSet1),
        { 18, nullptr, ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0, 0.0f, &expand_scaled_thresholds, &kAlarmSet2 },
        ::zhc::tuya::dp::ignored(101),   // deprecated in the device schema
        ::zhc::tuya::dp::numeric(102, "reclosing_count", 1),
        ::zhc::tuya::dp::numeric(103, "device_temperature", 1),
        ::zhc::tuya::dp::binary(104, "reclosing"),
        ::zhc::tuya::dp::numeric(105, "countdown", 1),
        ::zhc::tuya::dp::numeric(107, "reclosing_delay", 1),
        ::zhc::tuya::dp::numeric(108, "random_timing", 1),
        ::zhc::tuya::dp::numeric(119, "power_on_delay", 1),
        { 126, nullptr, ::zhc::TuyaDpType::Raw, 1, nullptr, 0, 0, 0.0f, &expand_scaled_thresholds, &kAlarmSet3 },
        ::zhc::tuya::dp::enum_lookup(127, "status", kStatus, 3),
        ::zhc::tuya::dp::enum_lookup(134, "power_on_behavior", kPowerOnBehavior, 3),
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FX = ::zhc::tuya::factory::TuyaRw<cfg_d1z>;

constexpr const char* kPobOpts[]    = { "off", "on", "previous" };
constexpr const char* kStatusOpts[] = { "off", "consumption", "production" };
constexpr const char* kResetOpts[]  = { "RESET" };

constexpr Expose kExposes[] = {
    { "state",     ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, "On/off state of the switch", nullptr, 0 },
    { "countdown", ExposeType::Numeric, ::zhc::Access::StateSet, "s",
      "Toggle the device after a set duration (one time action)", nullptr, 0, ExposeCategory::State, 0, 86400, 1 },
    { "power_on_behavior", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Power on behavior state",
      kPobOpts, 3, ExposeCategory::Config },
    { "status", ExposeType::Enum, ::zhc::Access::State, nullptr, "Current operating status",
      kStatusOpts, 3, ExposeCategory::Diagnostic },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "V",   nullptr, nullptr, 0 },
    { "current", ExposeType::Numeric, ::zhc::Access::State, "A",   nullptr, nullptr, 0 },
    { "power",   ExposeType::Numeric, ::zhc::Access::State, "W",   nullptr, nullptr, 0 },
    { "energy",  ExposeType::Numeric, ::zhc::Access::State, "kWh", nullptr, nullptr, 0 },
    { "device_temperature", ExposeType::Numeric, ::zhc::Access::State, "°C", "Temperature of the device",
      nullptr, 0, ExposeCategory::Diagnostic },
    { "faults", ExposeType::String, ::zhc::Access::State, nullptr,
      "Active faults, comma separated; empty when none", nullptr, 0, ExposeCategory::Diagnostic },
    { "reclosing", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Automatically attempt switching ON the circuit after it was turned OFF by a detected fault",
      nullptr, 0, ExposeCategory::Config },
    { "reclosing_delay", ExposeType::Numeric, ::zhc::Access::StateSet, "s",
      "Time to wait after the fault is cleared, before attempting reclose", nullptr, 0, ExposeCategory::Config, 1, 99, 1 },
    { "reclosing_count", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr,
      "Number of allowed reclosing attempts per fault", nullptr, 0, ExposeCategory::Config, 1, 99, 1 },
    { "prepayment", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Automatically switch OFF the circuit when the energy balance reaches zero", nullptr, 0, ExposeCategory::Config },
    { "energy_balance", ExposeType::Numeric, ::zhc::Access::State, "kWh",
      "Amount of energy allowed for consumption (Decreases when Prepayment is enabled)", nullptr, 0, ExposeCategory::Diagnostic },
    { "energy_balance_add", ExposeType::Numeric, ::zhc::Access::StateSet, "kWh",
      "Add an amount of energy to the balance", nullptr, 0, ExposeCategory::Config, 0, 9999, 0 },
    { "energy_balance_reset", ExposeType::Enum, ::zhc::Access::StateSet, nullptr, "Set the energy balance to zero",
      kResetOpts, 1, ExposeCategory::Config },
    // alarm_set_1/2/3 features, flat and read-only — see the header note.
    { "leakage_current_alarm",        ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "leakage_current_threshold",    ExposeType::Numeric, ::zhc::Access::State, "mA", nullptr, nullptr, 0, ExposeCategory::Config, 1, 99, 1 },
    { "device_temperature_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "device_temperature_threshold", ExposeType::Numeric, ::zhc::Access::State, "°C", nullptr, nullptr, 0, ExposeCategory::Config, 10, 85, 1 },
    { "over_current_alarm",      ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_current_threshold",  ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0, ExposeCategory::Config, 1, 80, 0 },
    { "over_voltage_alarm",      ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "over_voltage_threshold",  ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config, 120, 300, 1 },
    { "under_voltage_alarm",     ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "under_voltage_threshold", ExposeType::Numeric, ::zhc::Access::State, "V", nullptr, nullptr, 0, ExposeCategory::Config, 80, 210, 1 },
    { "lost_flow_alarm",         ExposeType::Binary,  ::zhc::Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Config },
    { "lost_flow_threshold",     ExposeType::Numeric, ::zhc::Access::State, "A", nullptr, nullptr, 0, ExposeCategory::Config, 1, 100, 0 },
};

constexpr const char* kModels[] = { "TS0601" };
constexpr const char* kManus[]  = { "_TZE204_vfnf7zhq" };

}  // namespace

extern const PreparedDefinition kDef_D1Z{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="D1Z", .vendor="Nous",
    .meta=nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FX::fz_list, .from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list, .to_zigbee_count=FX::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr };

}  // namespace zhc::devices::nous
