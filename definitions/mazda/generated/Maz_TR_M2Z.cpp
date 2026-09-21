// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Mazda TR-M2Z — neutered.
// The richer DP-mapped ports live at Maz__TZE204_k6rdmisz.cpp and
// Maz__TZE284_k6rdmisz.cpp; this minimal-port stub from
// generate_simple_vendor_ports.py decoded the thermostatic radiator
// valve as battery + on/off only and would shadow the proper ports
// during find_definition's Pass 1. Header `// Tier 2` keeps the
// simple vendor generator from regenerating it.
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::mazda {
namespace {
constexpr const char* kModels_TR_M2Z[] = { "__NEUTERED_TR_M2Z" };
// --- auto-generated exposes by zhac-tools/emit/emit_exposes.py (26.105.0) ---
constexpr const char* kAutoOpts_TR_M2Z_10[] = {"off", "heat"};
constexpr const char* kAutoOpts_TR_M2Z_13[] = {"schedule", "eco", "comfort", "frost_protection", "holiday"};
constexpr const char* kAutoOpts_TR_M2Z_14[] = {"idle", "heat"};
constexpr Expose kAutoExposes_TR_M2Z[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0, ExposeCategory::Diagnostic, 0, 100, 0},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"window_detection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0, ExposeCategory::Config},
    {"window_open", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
    {"frost_protection", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alarm_switch", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"comfort_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, 5, 35, 0},
    {"eco_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, 5, 35, 0},
    {"holiday_temperature", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::Config, 5, 35, 0},
    {"temperature_sensitivity", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 0, 5, 0},
    {"system_mode", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts_TR_M2Z_10, 2},
    {"local_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"preset", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kAutoOpts_TR_M2Z_13, 5},
    {"running_state", ExposeType::Enum, Access::State, nullptr, nullptr, kAutoOpts_TR_M2Z_14, 2},
    {"local_temperature_calibration", ExposeType::Numeric, Access::StateSet, "°C", nullptr, nullptr, 0, ExposeCategory::State, -9, 15, 0},
    {"schedule_monday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_tuesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_wednesday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_thursday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_friday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_saturday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"schedule_sunday", ExposeType::String, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
// --- end auto-generated exposes ---
}  // namespace

extern const PreparedDefinition kDef_TR_M2Z{
    .zigbee_models=kModels_TR_M2Z,
    .zigbee_models_count=sizeof(kModels_TR_M2Z)/sizeof(kModels_TR_M2Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TR-M2Z", .vendor="Mazda",
    .meta=nullptr, .exposes=kAutoExposes_TR_M2Z,.exposes_count=sizeof(kAutoExposes_TR_M2Z)/sizeof(kAutoExposes_TR_M2Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::mazda
