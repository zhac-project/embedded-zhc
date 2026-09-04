// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Siemens RDZ101ZB Connected Home wireless room thermostat
// (zigbeeModel "RDZ101"; z2m v26.104.0, #13084). New vendor.
//
// Upstream: m.battery() + m.thermostat({occupiedHeatingSetpoint 5..35 step
// 0.5, systemMode off/heat, runningState idle/heat}) + a genPowerCfg bind.
// The generic thermostat decoder covers local_temperature, the setpoint and
// system_mode; running_state (attr 0x0029, bitmap16) is layered on here the
// way ecozy does it -- bit 0 is "heat", anything else idle for this device's
// two-value enum. Setpoint writes go through the generic thermostat encoder
// (s16 x100); the 0.5 degree step is upstream UI metadata.
// z2m-source: siemens.ts #RDZ101ZB.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::siemens {
namespace {

constexpr const char* kModels[] = { "RDZ101" };

bool fz_siemens_thermostat(const ::zhc::DecodedMessage& msg,
                           const ::zhc::FzConverter& self,
                           const ::zhc::PreparedDefinition& def,
                           ::zhc::RuntimeContext& ctx,
                           ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = ::zhc::generic::kFzThermostat.fn.zcl_fn(msg, self, def, ctx, out);
    // 0x0029 thermostatRunningState: bit 0 = heat demand.
    if (const ::zhc::Value* v = msg.payload.find("41")) {
        std::uint64_t st = 0;
        bool ok = false;
        if (v->type == ::zhc::ValueType::Uint) { st = v->u; ok = true; }
        else if (v->type == ::zhc::ValueType::Int && v->i >= 0) { st = static_cast<std::uint64_t>(v->i); ok = true; }
        if (ok) {
            ::zhc::Value o{};
            o.type = ::zhc::ValueType::StringRef;
            o.str  = (st & 0x1) ? "heat" : "idle";
            if (out.put("running_state", o)) emitted = true;
        }
    }
    return emitted;
}

constexpr ::zhc::FzConverter kFzSiemensThermostat{
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
    .fn                = { .zcl_fn = fz_siemens_thermostat },
    .user_config       = nullptr,
};

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzBattery,
    &kFzSiemensThermostat,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzThermostat,
};

constexpr const char* kSystemModeOpts[]   = { "off", "heat" };
constexpr const char* kRunningStateOpts[] = { "idle", "heat" };
constexpr Expose kExposes[] = {
    {"battery",                  ExposeType::Numeric, Access::State,    "%",  nullptr, nullptr, 0},
    {"voltage",                  ExposeType::Numeric, Access::State,    "mV", nullptr, nullptr, 0},
    {"local_temperature",        ExposeType::Numeric, Access::State,    "\xC2\xB0""C", nullptr, nullptr, 0},
    {"current_heating_setpoint", ExposeType::Numeric, Access::StateSet, "\xC2\xB0""C", "Occupied heating setpoint (5..35, 0.5 steps)", nullptr, 0, ExposeCategory::State, 5, 35, 0},
    {"system_mode",              ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kSystemModeOpts, 2},
    {"running_state",            ExposeType::Enum,    Access::State,    nullptr, nullptr, kRunningStateOpts, 2},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0001},   // genPowerCfg
    {1, 0x0201},   // hvacThermostat
};

// Same attribute set m.thermostat() reports on: local temperature,
// heating setpoint, system mode, running state; battery percentage.
constexpr ReportingSpec kReports[] = {
    {1, 0x0201, 0x0000, 0x29, 10, 3600, 10, 0},
    {1, 0x0201, 0x0012, 0x29, 10, 3600, 10, 0},
    {1, 0x0201, 0x001C, 0x30, 10, 3600, 1,  0},
    {1, 0x0201, 0x0029, 0x19, 10, 3600, 1,  0},
    {1, 0x0001, 0x0021, 0x20, 3600, 62000, 1, 0},
};
}  // namespace

extern const PreparedDefinition kDef_RDZ101ZB{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="RDZ101ZB",.vendor="Siemens",
    .meta=nullptr,.exposes=kExposes,.exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports,.reports_count=sizeof(kReports)/sizeof(kReports[0]),
};

}  // namespace zhc::devices::siemens
