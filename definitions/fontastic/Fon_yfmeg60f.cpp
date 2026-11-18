// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Fontastic _TYST11_yfmeg60f — Tuya-MCU radiator valve (TRV), new upstream
// vendor in z2m v26.133.0. Old-style _TYST11_ fingerprint: modelID
// "fmeg60f\0" (stored NUL-stripped, as Alecto SMART-HEAT10).
//
// DP map (z2m fontastic.ts):
//   8   window_detection               onOff        -> "ON"/"OFF"
//   10  frost_protection               onOff
//   27  local_temperature_calibration  raw (signed s32)
//   40  child_lock                     lockUnlock   -> "LOCK"/"UNLOCK"
//   101 system_mode                    bool, true=heat / false=off
//   102 local_temperature              /10
//   103 current_heating_setpoint       /10
//   105 battery_low                    bitmap, bit 0
//   106 away_mode, 108 schedule_mode   raw bool
//   130 anti_scaling                   onOff
// Not ported: DP 123-129 weekly schedule (read-only day strings) — deferred
// with the other schedule codecs. DPs 110-122 are unmapped upstream too.
//
// z2m-source: fontastic.ts #_TYST11_yfmeg60f.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
#include "definitions/tuya/factories.hpp"

namespace zhc::devices::fontastic {
namespace {

// z2m: {from: (v) => (v & 1) === 1} — bit 0 of a bitmap (any wire type).
bool expand_battery_low(const ::zhc::tuya::TuyaDpMapEntry& e, const Value& raw,
                        RuntimeContext&, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    std::uint64_t v = 0;
    if      (raw.type == ValueType::Uint) v = raw.u;
    else if (raw.type == ValueType::Int)  v = static_cast<std::uint64_t>(raw.i);
    else if (raw.type == ValueType::Bool) v = raw.b ? 1u : 0u;
    else return false;
    Value o{}; o.type = ValueType::Bool; o.b = (v & 1u) != 0;
    out.put(e.out_key, o);
    return true;
}

constexpr ::zhc::tuya::TuyaEnumEntry kFonOnOff[]      = { {0, "OFF"}, {1, "ON"} };
constexpr ::zhc::tuya::TuyaEnumEntry kFonLock[]       = { {0, "UNLOCK"}, {1, "LOCK"} };
constexpr ::zhc::tuya::TuyaEnumEntry kFonSystemMode[] = { {0, "off"}, {1, "heat"} };

constexpr std::uint16_t kBoolEnum = ::zhc::tuya::kTuyaDpFlagBoolEnum;

struct cfg_yfmeg60f {
    static constexpr ::zhc::tuya::TuyaDpMapEntry e[] = {
        { 8,   "window_detection", ::zhc::TuyaDpType::Bool, 1, kFonOnOff, 2, kBoolEnum },
        { 10,  "frost_protection", ::zhc::TuyaDpType::Bool, 1, kFonOnOff, 2, kBoolEnum },
        ::zhc::tuya::dp::numeric(27, "local_temperature_calibration", 1),
        { 40,  "child_lock",       ::zhc::TuyaDpType::Bool, 1, kFonLock, 2, kBoolEnum },
        { 101, "system_mode",      ::zhc::TuyaDpType::Bool, 1, kFonSystemMode, 2, kBoolEnum },
        ::zhc::tuya::dp::numeric(102, "local_temperature", 10),
        ::zhc::tuya::dp::numeric(103, "current_heating_setpoint", 10),
        { 105, "battery_low", ::zhc::TuyaDpType::Bitmap, 1, nullptr, 0, 0, 0.0f, &expand_battery_low, nullptr },
        ::zhc::tuya::dp::binary(106, "away_mode"),
        ::zhc::tuya::dp::binary(108, "schedule_mode"),
        { 130, "anti_scaling",     ::zhc::TuyaDpType::Bool, 1, kFonOnOff, 2, kBoolEnum },
    };
    static constexpr ::zhc::tuya::TuyaDatapointMap dp_map{ e, sizeof(e) / sizeof(e[0]) };
};
using FXFon = ::zhc::tuya::factory::TuyaRw<cfg_yfmeg60f>;

constexpr const char* kFonSystemModeOpts[] = { "off", "heat" };

// Flat climate exposes (project rule). The 0.5 °C setpoint step is not
// expressible in the integer value_step, left unspecified.
constexpr Expose kExp_yfmeg60f[] = {
    { "battery_low",      ExposeType::Binary, ::zhc::Access::State,    nullptr,
      "Indicates if the battery of this device is almost empty", nullptr, 0, ExposeCategory::Diagnostic },
    { "anti_scaling",     ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Enables/disables bi-weekly anti-scaling feature", nullptr, 0, ExposeCategory::Config },
    { "child_lock",       ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Enables/disables physical input on the device", nullptr, 0 },
    { "away_mode",        ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Temperature drops to 16 °C when activated and restores when off", nullptr, 0 },
    { "schedule_mode",    ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Enable/disable the heating schedule", nullptr, 0 },
    { "window_detection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "Enables/disables window detection", nullptr, 0, ExposeCategory::Config },
    { "local_temperature_calibration", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",
      "Offset to add/subtract to the local temperature", nullptr, 0, ExposeCategory::State, -6, 6, 1 },
    { "local_temperature", ExposeType::Numeric, ::zhc::Access::State, "°C",
      "Current temperature measured on the device", nullptr, 0 },
    { "current_heating_setpoint", ExposeType::Numeric, ::zhc::Access::StateSet, "°C",
      "Temperature setpoint", nullptr, 0, ExposeCategory::State, 5, 30, 0 },
    { "system_mode",      ExposeType::Enum,   ::zhc::Access::StateSet, nullptr,
      "Mode of this device", kFonSystemModeOpts, 2 },
    { "frost_protection", ExposeType::Binary, ::zhc::Access::StateSet, nullptr,
      "When anti-freezing is active, the temperature is kept at 8 °C and the display shows \"AF\". "
      "Press the pair button to cancel.", nullptr, 0 },
};

// z2m tuyaBase({bindBasicOnConfigure: true}) -> bind genBasic on EP1.
constexpr BindingSpec kBind_yfmeg60f[] = { { 1, 0x0000 } };

constexpr const char* kM_yfmeg60f[] = { "fmeg60f" };
constexpr const char* kN_yfmeg60f[] = { "_TYST11_yfmeg60f" };

}  // namespace

extern const PreparedDefinition kDef_yfmeg60f{
    .zigbee_models=kM_yfmeg60f, .zigbee_models_count=sizeof(kM_yfmeg60f)/sizeof(kM_yfmeg60f[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN_yfmeg60f, .manufacturer_names_count=sizeof(kN_yfmeg60f)/sizeof(kN_yfmeg60f[0]),
    .model="_TYST11_yfmeg60f", .vendor="Fontastic",
    .meta=nullptr,
    .exposes=kExp_yfmeg60f, .exposes_count=sizeof(kExp_yfmeg60f)/sizeof(kExp_yfmeg60f[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=FXFon::fz_list, .from_zigbee_count=FXFon::fz_count,
    .to_zigbee=FXFon::tz_list, .to_zigbee_count=FXFon::tz_count,
    .configure=::zhc::tuya::extend::tuya_base_configure(), .on_event=nullptr,
    .bindings=kBind_yfmeg60f, .bindings_count=sizeof(kBind_yfmeg60f)/sizeof(kBind_yfmeg60f[0]),
    .tuya_time_start=2 };

}  // namespace zhc::devices::fontastic
