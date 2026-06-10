// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Netica FRZ1 — graduated from generated/ 2026-06-10.
// FreezBee, a smart thermostat designed to operate with Frisquet boilers.
// z2m-source: netica.ts #FRZ1.
//
// z2m wires `m.temperature() + m.humidity() + m.thermostat({...}) +
//            3 manuSpec hvacThermostat attrs`. NO battery / genPowerCfg.
//
// Parity fixes over the earlier hand-rewrite:
//   * Dropped the PHANTOM battery: z2m has no m.battery() and no
//     genPowerCfg — battery/voltage exposes + kFzBattery + the 0x0001
//     binding were invented. Removed.
//   * running_state (0x0029) now decodes. z2m m.thermostat({runningState})
//     exposes `running_state` and decodes it via fz.thermostat, but the
//     generic kFzThermostat only handles 0x0000/0x0012/0x001C — so the
//     state was missing entirely. Added via kFzNeticaThermostatExtras.
//   * control_sequence_of_operation (0x001B) likewise decodes now
//     (z2m withControlSequenceOfOperation, value "heating_only").
//   * remote_temperature (0x4000) + target_water_temperature (0x4002)
//     manuSpec attrs now decode (read-back). z2m m.numeric exposes them
//     with a read path (remote_temperature access ALL,
//     target_water_temperature STATE_GET) but the port only wired the
//     write side — they were dead STATE_GET keys.
//
// z2m manuSpec hvacThermostat attrs (mfg = ManufacturerCode.RESERVED_10
// = 0xFFFF):
//   0x4000 INT16 remoteTemperature      (×100 °C)  — read + write
//   0x4001 BOOL  useRemoteTemperature                — write
//   0x4002 INT16 targetWaterTemperature (×100 °C)  — read only (STATE_GET)
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::netica {
namespace {

constexpr const char* kModels_FRZ1[] = { "FRZ1" };

constexpr std::uint16_t kNeticaMfgCode = 0xFFFF;  // ManufacturerCode.RESERVED_10

// ── Vendor FZ: thermostat extras ─────────────────────────────────────
// Delegates to the generic thermostat decoder (local_temperature,
// current_heating_setpoint, system_mode), then adds the attrs the
// generic decoder does not cover but z2m's fz.thermostat / m.numeric do:
//   0x0029 runningState   (key "41")    → running_state  (0→idle, 1→heat)
//   0x001B ctrlSeqeOfOper (key "27")    → control_sequence_of_operation
//   0x4000 remoteTemperature   (key "16384") → remote_temperature   (raw ×100)
//   0x4002 targetWaterTemperature (key "16386") → target_water_temperature (raw ×100)
//
// running_state / control_sequence_of_operation emit z2m's string form
// (matching fz.thermostat's getFromLookup); the numeric manuSpec attrs
// emit the raw int (downstream applies the ×100 scale, same contract as
// local_temperature).
bool fz_netica_thermostat_extras(const ::zhc::DecodedMessage& msg,
                                  const ::zhc::FzConverter& self,
                                  const ::zhc::PreparedDefinition& def,
                                  ::zhc::RuntimeContext& ctx,
                                  ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = ::zhc::generic::kFzThermostat.fn.zcl_fn(msg, self, def, ctx, out);

    // 0x0029 runningState — z2m thermostatRunningStates: 0=idle, 1=heat
    // (this device exposes only {idle, heat}).
    if (const ::zhc::Value* v = msg.payload.find("41")) {
        if (v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = (v->u == 0) ? "idle" : "heat";
            out.put("running_state", o);
            emitted = true;
        }
    }

    // 0x001B ctrlSeqeOfOper — z2m thermostatControlSequenceOfOperations.
    // This device's only declared value is heating_only (2).
    if (const ::zhc::Value* v = msg.payload.find("27")) {
        if (v->type == ::zhc::ValueType::Uint) {
            const char* s = nullptr;
            switch (v->u) {
                case 0: s = "cooling_only"; break;
                case 1: s = "cooling_with_reheat"; break;
                case 2: s = "heating_only"; break;
                case 3: s = "heating_with_reheat"; break;
                case 4: s = "cooling_and_heating_4-pipes"; break;
                case 5: s = "cooling_and_heating_4-pipes_with_reheat"; break;
                default: break;
            }
            if (s) {
                ::zhc::Value o{};
                o.type = ::zhc::ValueType::StringRef;
                o.str  = s;
                out.put("control_sequence_of_operation", o);
                emitted = true;
            }
        }
    }

    // 0x4000 remoteTemperature (INT16, ×100 °C) — raw int read-back.
    if (const ::zhc::Value* v = msg.payload.find("16384")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int
                      ? v->i
                      : static_cast<std::int64_t>(v->u);
            out.put("remote_temperature", o);
            emitted = true;
        }
    }

    // 0x4002 targetWaterTemperature (INT16, ×100 °C) — raw int read-back.
    if (const ::zhc::Value* v = msg.payload.find("16386")) {
        if (v->type == ::zhc::ValueType::Int || v->type == ::zhc::ValueType::Uint) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::Int;
            o.i = v->type == ::zhc::ValueType::Int
                      ? v->i
                      : static_cast<std::int64_t>(v->u);
            out.put("target_water_temperature", o);
            emitted = true;
        }
    }

    return emitted;
}

constexpr ::zhc::FzConverter kFzNeticaThermostatExtras{
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
    .fn                = { .zcl_fn = fz_netica_thermostat_extras },
    .user_config       = nullptr,
};

// ── Manu-specific write specs (mfg 0xFFFF) ───────────────────────────
constexpr ::zhc::generic::ZclWriteSpec kSpecRemoteTemperature{
    "remote_temperature",      0x4000, 0x29 /* INT16 */, kNeticaMfgCode, nullptr, 0,
};
constexpr ::zhc::generic::ZclWriteSpec kSpecUseRemoteTemperature{
    "use_remote_temperature",  0x4001, 0x10 /* BOOL */,  kNeticaMfgCode, nullptr, 0,
};

constexpr ::zhc::TzConverter kTzRemoteTemperature{
    .key="remote_temperature", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecRemoteTemperature,
};
constexpr ::zhc::TzConverter kTzUseRemoteTemperature{
    .key="use_remote_temperature", .cluster="hvacThermostat",
    .cluster_id=0x0201, .command_id=0x02,
    .fn=&::zhc::generic::tz_zcl_write_attr,
    .user_config=&kSpecUseRemoteTemperature,
};
// NOTE: targetWaterTemperature (0x4002) is read-only in z2m (STATE_GET)
// — no write converter.

const FzConverter* const kFz_FRZ1[] = {
    &kFzNeticaThermostatExtras,   // thermostat + running_state + ctrlSeq + manuSpec read-back
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
const TzConverter* const kTz_FRZ1[] = {
    &::zhc::generic::kTzThermostat,
    &::zhc::generic::kTzMinHeatSetpointLimit,
    &::zhc::generic::kTzMaxHeatSetpointLimit,
    &::zhc::generic::kTzSetpointRaiseLower,
    &kTzRemoteTemperature,
    &kTzUseRemoteTemperature,
};

constexpr Expose kExp_FRZ1[] = {
    { "local_temperature",                ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "current_heating_setpoint",         ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "system_mode",                      ExposeType::Enum,    ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "running_state",                    ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "control_sequence_of_operation",    ExposeType::Enum,    ::zhc::Access::State,    nullptr, nullptr, nullptr, 0 },
    { "min_heat_setpoint_limit",          ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "max_heat_setpoint_limit",          ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0 },
    { "temperature",                      ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0 },
    { "humidity",                         ExposeType::Numeric, ::zhc::Access::State,    "%", nullptr, nullptr, 0 },
    { "remote_temperature",               ExposeType::Numeric, ::zhc::Access::StateSet, "C", nullptr, nullptr, 0,
        ExposeCategory::Config },
    { "use_remote_temperature",           ExposeType::Binary,  ::zhc::Access::Set,      nullptr, nullptr, nullptr, 0,
        ExposeCategory::Config },
    { "target_water_temperature",         ExposeType::Numeric, ::zhc::Access::State,    "C", nullptr, nullptr, 0,
        ExposeCategory::Diagnostic },
};

constexpr BindingSpec kBind_FRZ1[] = {
    { 1, 0x0201 },   // hvacThermostat
    { 1, 0x0402 },   // msTemperatureMeasurement
    { 1, 0x0405 },   // msRelativeHumidity
};

}  // namespace

extern const PreparedDefinition kDef_FRZ1{
    .zigbee_models=kModels_FRZ1, .zigbee_models_count=sizeof(kModels_FRZ1)/sizeof(kModels_FRZ1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FRZ1", .vendor="Netica",
    .meta=nullptr,
    .exposes=kExp_FRZ1, .exposes_count=sizeof(kExp_FRZ1)/sizeof(kExp_FRZ1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_FRZ1, .from_zigbee_count=sizeof(kFz_FRZ1)/sizeof(kFz_FRZ1[0]),
    .to_zigbee=kTz_FRZ1,   .to_zigbee_count=sizeof(kTz_FRZ1)/sizeof(kTz_FRZ1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_FRZ1, .bindings_count=sizeof(kBind_FRZ1)/sizeof(kBind_FRZ1[0]),
};

}  // namespace zhc::devices::netica
