// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Purmo/Radson Yali Parada Plus — graduated from generated/ 2026-06-10.
// Electric oil-filled radiator (PUMM01102, manufacturerName "computime").
// z2m-source: purmo.ts #Yali Parada Plus.
//
// z2m wires `fromZigbee: [fz.thermostat]` over a single hvacThermostat
// endpoint and exposes a full climate surface:
//   e.climate()
//     .withSetpoint("occupied_heating_setpoint", 5, 30, 0.5)
//     .withSetpoint("unoccupied_heating_setpoint", 5, 30, 0.5)
//     .withSystemMode(["heat", "off"])
//     .withLocalTemperature()
//     .withLocalTemperatureCalibration()
//     .withRunningState(["idle", "heat"])
//     .withRunningMode(["off", "heat"])
//   e.max_heat_setpoint_limit(5, 30, 0.5)
//
// Parity fix over the Tier-1 auto-port:
//   * The Tier-1 port carried only 3 exposes (local_temperature,
//     current_heating_setpoint, system_mode-as-Binary) and the bare
//     generic kFzThermostat, which decodes ONLY 0x0000 / 0x0012 / 0x001C.
//     So unoccupied_heating_setpoint, local_temperature_calibration,
//     running_mode, running_state, max_heat_setpoint_limit and occupancy
//     were all exposed-but-dead (z2m's fz.thermostat decodes every one).
//   * system_mode was typed Binary; z2m exposes it as an enum (heat/off).
//   * The full expose surface + the write path (unoccupied setpoint,
//     max-heat limit, local-temperature calibration, occupancy) are now
//     wired, matching the device's tz.thermostat_* converter list.
//
// fz.thermostat attr → key (decimal payload key) → out key (z2m scale):
//   0x0000 localTemp                 "0"  → local_temperature              (/100)
//   0x0002 occupancy                 "2"  → occupancy (bit0)               (bool)
//   0x0010 localTemperatureCalibration "16" → local_temperature_calibration (/10)
//   0x0012 occupiedHeatingSetpoint   "18" → current_heating_setpoint       (/100)
//   0x0014 unoccupiedHeatingSetpoint "20" → unoccupied_heating_setpoint    (/100)
//   0x0016 maxHeatSetpointLimit      "22" → max_heat_setpoint_limit        (/100)
//   0x001C systemMode                "28" → system_mode                    (enum)
//   0x001E runningMode               "30" → running_mode  (0 off,3 cool,4 heat)
//   0x0029 runningState              "41" → running_state (bit0 heat, else idle)
// local_temperature, current_heating_setpoint and system_mode flow through
// the generic decoder; the rest are added by kFzPurmoThermostatExtras.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::purmo {
namespace {

constexpr const char* kModels_Yali_Parada_Plus[] = { "PUMM01102" };
constexpr const char* kMfg_Yali_Parada_Plus[]    = { "computime" };

// ── Vendor FZ: thermostat extras ─────────────────────────────────────
// Delegates to the generic thermostat decoder (local_temperature,
// current_heating_setpoint, system_mode), then adds the attrs the generic
// decoder does not cover but z2m's fz.thermostat does. Enum attrs emit the
// z2m string form (matching getFromLookup); numeric attrs emit the raw int
// (downstream applies the per-key scale, same contract as
// local_temperature in the generic decoder).
bool fz_purmo_thermostat_extras(const ::zhc::DecodedMessage& msg,
                                 const ::zhc::FzConverter& self,
                                 const ::zhc::PreparedDefinition& def,
                                 ::zhc::RuntimeContext& ctx,
                                 ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = ::zhc::generic::kFzThermostat.fn.zcl_fn(msg, self, def, ctx, out);

    // 0x0002 occupancy (bitmap8) — z2m: occupancy % 2 > 0.
    if (const ::zhc::Value* v = msg.payload.find("2")) {
        if (v->type == ::zhc::ValueType::Uint || v->type == ::zhc::ValueType::Int) {
            const std::uint64_t raw = v->type == ::zhc::ValueType::Uint
                ? v->u : static_cast<std::uint64_t>(v->i);
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Bool;
            o.b = (raw % 2) > 0;
            out.put("occupancy", o);
            emitted = true;
        }
    }

    // 0x0010 localTemperatureCalibration (s8, ×10 °C) — raw int read-back.
    if (const ::zhc::Value* v = msg.payload.find("16")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int
                      ? v->i : static_cast<std::int64_t>(v->u);
            out.put("local_temperature_calibration", o);
            emitted = true;
        }
    }

    // 0x0014 unoccupiedHeatingSetpoint (s16, ×100 °C) — raw int read-back.
    if (const ::zhc::Value* v = msg.payload.find("20")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int
                      ? v->i : static_cast<std::int64_t>(v->u);
            out.put("unoccupied_heating_setpoint", o);
            emitted = true;
        }
    }

    // 0x0016 maxHeatSetpointLimit (s16, ×100 °C) — raw int read-back.
    if (const ::zhc::Value* v = msg.payload.find("22")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int
                      ? v->i : static_cast<std::int64_t>(v->u);
            out.put("max_heat_setpoint_limit", o);
            emitted = true;
        }
    }

    // 0x001E runningMode (u8 enum) — z2m thermostatRunningMode:
    // 0=off, 3=cool, 4=heat. This device exposes only {off, heat}.
    if (const ::zhc::Value* v = msg.payload.find("30")) {
        if (v->type == ::zhc::ValueType::Uint) {
            const char* s = nullptr;
            switch (v->u) {
                case 0: s = "off";  break;
                case 3: s = "cool"; break;
                case 4: s = "heat"; break;
                default: break;
            }
            if (s) {
                ::zhc::Value o{};
                o.type = ::zhc::ValueType::StringRef;
                o.str  = s;
                out.put("running_mode", o);
                emitted = true;
            }
        }
    }

    // 0x0029 runningState (bitmap16) — z2m thermostatRunningStates. This
    // device exposes only {idle, heat}: bit0 (heat) set → heat, else idle.
    if (const ::zhc::Value* v = msg.payload.find("41")) {
        if (v->type == ::zhc::ValueType::Uint || v->type == ::zhc::ValueType::Int) {
            const std::uint64_t raw = v->type == ::zhc::ValueType::Uint
                ? v->u : static_cast<std::uint64_t>(v->i);
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = (raw & 0x1) ? "heat" : "idle";
            out.put("running_state", o);
            emitted = true;
        }
    }

    return emitted;
}

constexpr ::zhc::FzConverter kFzPurmoThermostatExtras{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "hvacThermostat",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_purmo_thermostat_extras },
    .user_config       = nullptr,
};

// ── Write specs for attrs not covered by the generic kTzThermostat ───
// kTzThermostat already covers occupied_heating_setpoint (0x0012, ×100)
// and system_mode (0x001C). kTzMaxHeatSetpointLimit covers 0x0016.
// The remaining tz.thermostat_* converters from the z2m def:
//   tz.thermostat_unoccupied_heating_setpoint   0x0014 INT16 (pre-scaled ×100)
//   tz.thermostat_local_temperature_calibration 0x0010 INT8  (pre-scaled ×10)
//   tz.thermostat_occupancy                      0x0002 BITMAP8
constexpr ::zhc::generic::ZclWriteSpec kSpecUnoccupiedHeatingSetpoint{
    "unoccupied_heating_setpoint", 0x0014, 0x29 /* INT16 */, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecLocalTemperatureCalibration{
    "local_temperature_calibration", 0x0010, 0x28 /* INT8 */, 0, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecOccupancy{
    "occupancy", 0x0002, 0x18 /* BITMAP8 */, 0, nullptr, 0,
};

constexpr ::zhc::TzConverter kTzUnoccupiedHeatingSetpoint{
    .key="unoccupied_heating_setpoint", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecUnoccupiedHeatingSetpoint,
};
constexpr ::zhc::TzConverter kTzLocalTemperatureCalibration{
    .key="local_temperature_calibration", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecLocalTemperatureCalibration,
};
constexpr ::zhc::TzConverter kTzOccupancy{
    .key="occupancy", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecOccupancy,
};

const FzConverter* const kFz_Yali_Parada_Plus[] = {
    &kFzPurmoThermostatExtras,
};
const TzConverter* const kTz_Yali_Parada_Plus[] = {
    &::zhc::generic::kTzThermostat,            // occupied_heating_setpoint + system_mode
    &kTzUnoccupiedHeatingSetpoint,
    &::zhc::generic::kTzMaxHeatSetpointLimit,  // max_heat_setpoint_limit (0x0016)
    &kTzLocalTemperatureCalibration,
    &kTzOccupancy,
};

// Thermostats ship FLAT exposes (no ExposeType::Climate). Setpoint
// ranges 5–30 °C step 0.5 per z2m; system_mode/running_mode/running_state
// are enums.
constexpr Expose kExp_Yali_Parada_Plus[] = {
    { "local_temperature",              ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "current_heating_setpoint",       ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "unoccupied_heating_setpoint",    ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "system_mode",                    ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "local_temperature_calibration",  ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0,
        ExposeCategory::Config },
    { "running_state",                  ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "running_mode",                   ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "max_heat_setpoint_limit",        ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_Yali_Parada_Plus[] = {
    { 1, 0x0201 },   // hvacThermostat
};

}  // namespace

extern const PreparedDefinition kDef_Yali_Parada_Plus{
    .zigbee_models=kModels_Yali_Parada_Plus, .zigbee_models_count=sizeof(kModels_Yali_Parada_Plus)/sizeof(kModels_Yali_Parada_Plus[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kMfg_Yali_Parada_Plus, .manufacturer_names_count=sizeof(kMfg_Yali_Parada_Plus)/sizeof(kMfg_Yali_Parada_Plus[0]),
    .model="Yali Parada Plus", .vendor="Purmo",
    .meta=nullptr,
    .exposes=kExp_Yali_Parada_Plus, .exposes_count=sizeof(kExp_Yali_Parada_Plus)/sizeof(kExp_Yali_Parada_Plus[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Yali_Parada_Plus, .from_zigbee_count=sizeof(kFz_Yali_Parada_Plus)/sizeof(kFz_Yali_Parada_Plus[0]),
    .to_zigbee=kTz_Yali_Parada_Plus,   .to_zigbee_count=sizeof(kTz_Yali_Parada_Plus)/sizeof(kTz_Yali_Parada_Plus[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_Yali_Parada_Plus, .bindings_count=sizeof(kBind_Yali_Parada_Plus)/sizeof(kBind_Yali_Parada_Plus[0]),
};

}  // namespace zhc::devices::purmo
