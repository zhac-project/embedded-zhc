// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Develco EMIZB-132 — hand-ported (z2m v26.125.0).
// Wattle AMS HAN power-meter sensor
// The meter sits on EP2 (z2m adds haElectricalMeasurement there with
// m.quirkAddEndpointCluster), so binds, reports and writes target EP2.
// Decode follows develco.fz.metering_emizb132 / electrical_measurement_emizb132:
//   seMetering 0x0000 energy /1000 (0 and 0xFFFFFFFFFFFF are dropped),
//   0x0001 produced_energy /1000, 0x0302 interface_mode (enum16, mfg 0x1015);
//   haElectricalMeasurement 0x0304 power / 0x0305 power_reactive (W / VAr),
//   voltage /10 and current /10 on all three phases — current /1000 once the
//   meter reports interface_mode kaifa_and_kamstrup (cached per device in
//   DeviceRuntimeState::scratch[0]; before any report it is norwegian_han,
//   as upstream defaults).
// Not ported: the cluster divisor attributes (fixed /10 as upstream's
// electricityMeter config) and readGenBasicPrimaryVersions.
// z2m-source: develco.ts #EMIZB-132.
#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"

namespace zhc::devices::develco {
namespace {

constexpr std::uint16_t kDevelcoManu = 0x1015;
constexpr std::size_t   kModeSlot    = 0;   // DeviceRuntimeState::scratch index
constexpr std::uint8_t  kModeKaifa   = 0x03;   // interface mode low byte

constexpr ::zhc::generic::ZclWriteLookup kInterfaceModes[] = {
    {"norwegian_han", 0x0200}, {"norwegian_han_extra_load", 0x0201},
    {"aidon_meter", 0x0202}, {"kaifa_and_kamstrup", 0x0203} };

bool as_num(const Value* v, double& out) {
    if (!v) return false;
    if (v->type == ValueType::Uint) { out = static_cast<double>(v->u); return true; }
    if (v->type == ValueType::Int)  { out = static_cast<double>(v->i); return true; }
    return false;
}
void put_float(FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out, const char* key, double f) {
    Value o{}; o.type = ValueType::Float; o.f = static_cast<float>(f);
    out.put(key, o);
}

bool fz_emizb_metering(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                       RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool any = false;
    double v = 0;
    if (as_num(msg.payload.find("0"), v) && v != 0 && v != 281474976710655.0) {   // 0xFFFFFFFFFFFF
        put_float(out, "energy", v / 1000.0); any = true;
    }
    if (as_num(msg.payload.find("1"), v)) { put_float(out, "produced_energy", v / 1000.0); any = true; }
    if (as_num(msg.payload.find("770"), v)) {                                       // 0x0302
        for (const auto& m : kInterfaceModes) {
            if (m.value != static_cast<std::uint32_t>(v)) continue;
            if (auto* st = ctx.device_state()) st->scratch[kModeSlot] = static_cast<std::uint8_t>(m.value & 0xFF);
            Value o{}; o.type = ValueType::StringRef; o.str = m.label;
            any |= out.put("interface_mode", o);
        }
    }
    return any;
}

bool fz_emizb_electrical(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&,
                         RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto* st = ctx.device_state();
    const double current_div = (st && st->scratch[kModeSlot] == kModeKaifa) ? 1000.0 : 10.0;
    struct Row { const char* attr; const char* key; double div; };
    const Row rows[] = {
        {"772", "power", 1}, {"773", "power_reactive", 1},                          // 0x0304 / 0x0305
        {"1285", "voltage", 10}, {"2309", "voltage_phase_b", 10}, {"2565", "voltage_phase_c", 10},
        {"1288", "current", current_div}, {"2312", "current_phase_b", current_div},
        {"2568", "current_phase_c", current_div},
    };
    bool any = false;
    double v = 0;
    for (const Row& r : rows) {
        if (!as_num(msg.payload.find(r.attr), v)) continue;
        put_float(out, r.key, v / r.div); any = true;
    }
    return any;
}

constexpr FzConverter zcl_fz(const char* cluster, FromZigbeeFn fn) {
    return FzConverter{
        .family            = FrameFamily::Zcl,
        .cluster           = cluster,
        .type_mask         = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = fn },
        .user_config       = nullptr,
    };
}
constexpr FzConverter kFzMetering = zcl_fz("seMetering", fz_emizb_metering);
constexpr FzConverter kFzElectrical = zcl_fz("haElectricalMeasurement", fz_emizb_electrical);

constexpr ::zhc::generic::ZclWriteSpec kInterfaceModeSpec{
    "interface_mode", 0x0302, 0x31, kDevelcoManu, kInterfaceModes, 4 };
constexpr TzConverter kTzInterfaceMode = ::zhc::generic::zcl_write_tz("seMetering", 0x0702, &kInterfaceModeSpec, 2);

const FzConverter* const kFz_EMIZB_132[] = { &kFzMetering, &kFzElectrical };
const TzConverter* const kTz_EMIZB_132[] = { &kTzInterfaceMode };
constexpr const char* kModels_EMIZB_132[] = { "EMIZB-132" };

constexpr const char* kInterfaceModeOpts[] = {
    "norwegian_han", "norwegian_han_extra_load", "aidon_meter", "kaifa_and_kamstrup" };
constexpr Expose kExposes[] = {
    {"power", ExposeType::Numeric, Access::State, "W", "Total active power", nullptr, 0},
    {"power_reactive", ExposeType::Numeric, Access::State, "VAr", "Total reactive power", nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"produced_energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_phase_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"voltage_phase_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_phase_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"current_phase_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"interface_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "Specifies the configuration of the external HAN port interface.", kInterfaceModeOpts, 4,
     ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {2, 0x0702},
    {2, 0x0B04},
};

// power / power_reactive per the m.numeric reporting {min 5, max 1 h, change
// 1}; energy / voltage / current per m.electricityMeter defaults.
constexpr ReportingSpec kReports[] = {
    {2, 0x0B04, 0x0304, 0x2B, 5, 3600, 1, 0},
    {2, 0x0B04, 0x0305, 0x2B, 5, 3600, 1, 0},
    {2, 0x0702, 0x0000, 0x25, 10, 65000, 100, 0},
    {2, 0x0702, 0x0001, 0x25, 10, 65000, 100, 0, ::zhc::kReportFlagOptional},
    {2, 0x0B04, 0x0505, 0x21, 10, 65000, 50, 0},
    {2, 0x0B04, 0x0905, 0x21, 10, 65000, 50, 0, ::zhc::kReportFlagOptional},
    {2, 0x0B04, 0x0A05, 0x21, 10, 65000, 50, 0, ::zhc::kReportFlagOptional},
    {2, 0x0B04, 0x0508, 0x21, 10, 65000, 1, 0},
    {2, 0x0B04, 0x0908, 0x21, 10, 65000, 1, 0, ::zhc::kReportFlagOptional},
    {2, 0x0B04, 0x0A08, 0x21, 10, 65000, 1, 0, ::zhc::kReportFlagOptional},
};

// interface_mode has no reporting: read it so the current divisor is right.
constexpr std::uint8_t kReadInterfaceMode[] = { 0x02, 0x03 };
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 2, 0x0702, 0x00, 0, kReadInterfaceMode, sizeof(kReadInterfaceMode), 0, kDevelcoManu },
};

}  // namespace

extern const PreparedDefinition kDef_EMIZB_132{
    .zigbee_models=kModels_EMIZB_132, .zigbee_models_count=sizeof(kModels_EMIZB_132)/sizeof(kModels_EMIZB_132[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EMIZB-132", .vendor="Develco",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_EMIZB_132, .from_zigbee_count=sizeof(kFz_EMIZB_132)/sizeof(kFz_EMIZB_132[0]),
    .to_zigbee=kTz_EMIZB_132, .to_zigbee_count=sizeof(kTz_EMIZB_132)/sizeof(kTz_EMIZB_132[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=kReports, .reports_count=sizeof(kReports)/sizeof(kReports[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::develco
