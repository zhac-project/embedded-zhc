// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: EDFLab ZIGBEE_ERL, Linky meter TIC interface (new vendor in z2m
// v26.126.0..v26.133.0). Partial port.
// Ported: m.onOff (no power-on behaviour) and every plain numeric attribute,
// published raw as upstream (m.numeric without scale, STATE_GET, no reporting):
//   seMeterIdentification 0x0B01  meter type / data quality ids, available power,
//                                 power threshold (VA)
//   seMetering 0x0702             summations delivered / received, tiers 1..6 (Wh),
//                                 reactive Q1..Q4 (varh), instantaneous demand and
//                                 current / previous day max demands (W), status,
//                                 tier / mode / disconnect-reason enums
//   haElectricalMeasurement       measurement type, total and per-phase (A/B/C)
//                                 active / apparent power, rms current / voltage
//   genTime 0x000A                time_status, time_zone, dst_shift (s)
// The dailySchedule cluster (0x070D) has no name in zhc/cluster_names.hpp, so its
// frames reach every converter of the def ("fail open"): its tier labels
// (attrs 0..7, octet strings) and 0x0100..0x0106 would land in the seMetering
// rows. gated() keeps every attribute map off unnamed frames.
// Not ported: the text attributes (pod, company_name, serial numbers, meter_model,
// tariff / tier / aux-switch labels); the dailySchedule numerics
// (current_delivered_tier, linky_peak_period_status,
// current_auxiliary_load_switch_state: unnamed cluster) and peakPeriodStatus
// (day colours, peak notices); the timestamp attributes and their *_text twins;
// extended_status (hex text); writeTimeDailyUtc (genTime poll); the commands
// (profile, snapshot, sampled data, schedule, day profile, messages) and their
// multi-frame reassembly; identify.
// z2m-source: edflab.ts #ZIGBEE_ERL.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::edflab {
namespace {

namespace g = ::zhc::generic;

// A named wrapper skips frames of unnamed clusters; an unnamed one (cluster
// nullptr) takes only those, non-manufacturer-specific.
// ponytail: copied per def (also heatzy, hzc_electric, repenic_ltd); hoist into
// _generic if more defs need it.
bool fz_gated(const DecodedMessage& msg, const FzConverter& self, const PreparedDefinition& def,
              RuntimeContext& ctx, FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const bool named = msg.cluster != nullptr;
    if (self.cluster ? !named : (named || msg.manufacturer_specific)) return false;
    const auto* inner = static_cast<const FzConverter*>(self.user_config);
    return inner->fn.zcl_fn(msg, *inner, def, ctx, out);
}
constexpr FzConverter gated(const char* cluster, const FzConverter* inner) {
    FzConverter c = g::zcl_attr_fz(cluster, nullptr);
    c.fn.zcl_fn = fz_gated;
    c.user_config = inner;
    return c;
}

constexpr g::ZclAttrRow kIdentRows[] = {
    { 0x0001, "meter_type_id" },
    { 0x0004, "data_quality_id" },
    { 0x000D, "available_power" },
    { 0x000E, "power_threshold" },
};
constexpr g::ZclAttrMap kIdentMap{ kIdentRows, sizeof(kIdentRows)/sizeof(kIdentRows[0]) };
constexpr FzConverter kFzIdentRaw = g::zcl_attr_fz("haMeterIdentification", &kIdentMap);

constexpr g::ZclAttrRow kMeteringRows[] = {
    { 0x0000, "current_summ_delivered" },
    { 0x0001, "current_summ_received" },
    { 0x0020, "active_register_tier_delivered" },
    { 0x0023, "number_of_tiers_in_use" },
    { 0x0100, "current_tier1_summ_delivered" },
    { 0x0102, "current_tier2_summ_delivered" },
    { 0x0104, "current_tier3_summ_delivered" },
    { 0x0106, "current_tier4_summ_delivered" },
    { 0x0108, "current_tier5_summ_delivered" },
    { 0x010A, "current_tier6_summ_delivered" },
    { 0x0200, "status" },
    { 0x0208, "service_disconnect_reason" },
    { 0x0209, "linky_mode_of_operation" },
    { 0x0400, "instantaneous_demand" },
    { 0x045D, "current_day_max_demand_delivered" },
    { 0x045F, "current_day_max_demand_received" },
    { 0x0461, "previous_day_max_demand_delivered" },
    { 0x0463, "previous_day_max_demand_received" },
    { 0x0D05, "current_reactive_summation_q1" },
    { 0x0D06, "current_reactive_summation_q2" },
    { 0x0D07, "current_reactive_summation_q3" },
    { 0x0D08, "current_reactive_summation_q4" },
};
constexpr g::ZclAttrMap kMeteringMap{ kMeteringRows, sizeof(kMeteringRows)/sizeof(kMeteringRows[0]) };
constexpr FzConverter kFzMeteringRaw = g::zcl_attr_fz("seMetering", &kMeteringMap);

constexpr g::ZclAttrRow kElectricalRows[] = {
    { 0x0000, "measurement_type" },
    { 0x0304, "total_active_power" },
    { 0x0306, "total_apparent_power" },
    { 0x050B, "active_power" },
    { 0x050F, "apparent_power" },
    { 0x0508, "rms_current" },
    { 0x0505, "rms_voltage" },
    { 0x090B, "active_power_ph_b" },
    { 0x090F, "apparent_power_ph_b" },
    { 0x0908, "rms_current_ph_b" },
    { 0x0905, "rms_voltage_ph_b" },
    { 0x0A0B, "active_power_ph_c" },
    { 0x0A0F, "apparent_power_ph_c" },
    { 0x0A08, "rms_current_ph_c" },
    { 0x0A05, "rms_voltage_ph_c" },
};
constexpr g::ZclAttrMap kElectricalMap{ kElectricalRows, sizeof(kElectricalRows)/sizeof(kElectricalRows[0]) };
constexpr FzConverter kFzElectricalRaw = g::zcl_attr_fz("haElectricalMeasurement", &kElectricalMap);

constexpr g::ZclAttrRow kTimeRows[] = {
    { 0x0001, "time_status" },
    { 0x0002, "time_zone" },
    { 0x0005, "dst_shift" },
};
constexpr g::ZclAttrMap kTimeMap{ kTimeRows, sizeof(kTimeRows)/sizeof(kTimeRows[0]) };
constexpr FzConverter kFzTimeRaw = g::zcl_attr_fz("genTime", &kTimeMap);

constexpr FzConverter kFzIdent = gated("haMeterIdentification", &kFzIdentRaw);
constexpr FzConverter kFzMetering = gated("seMetering", &kFzMeteringRaw);
constexpr FzConverter kFzElectrical = gated("haElectricalMeasurement", &kFzElectricalRaw);
constexpr FzConverter kFzTime = gated("genTime", &kFzTimeRaw);

const FzConverter* const kFz[] = {
    &g::kFzOnOff,
    &kFzIdent,
    &kFzMetering,
    &kFzElectrical,
    &kFzTime,
};
const TzConverter* const kTz[] = {
    &g::kTzOnOff,
};

constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"meter_type_id", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"data_quality_id", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"available_power", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"power_threshold", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"status", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"service_disconnect_reason", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"measurement_type", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"linky_mode_of_operation", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"active_register_tier_delivered", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"number_of_tiers_in_use", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"current_summ_delivered", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_summ_received", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_tier1_summ_delivered", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_tier2_summ_delivered", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_tier3_summ_delivered", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_tier4_summ_delivered", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_tier5_summ_delivered", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_tier6_summ_delivered", ExposeType::Numeric, Access::State, "Wh", nullptr, nullptr, 0},
    {"current_reactive_summation_q1", ExposeType::Numeric, Access::State, "varh", nullptr, nullptr, 0},
    {"current_reactive_summation_q2", ExposeType::Numeric, Access::State, "varh", nullptr, nullptr, 0},
    {"current_reactive_summation_q3", ExposeType::Numeric, Access::State, "varh", nullptr, nullptr, 0},
    {"current_reactive_summation_q4", ExposeType::Numeric, Access::State, "varh", nullptr, nullptr, 0},
    {"instantaneous_demand", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_day_max_demand_delivered", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"current_day_max_demand_received", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"previous_day_max_demand_delivered", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"previous_day_max_demand_received", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"total_active_power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"total_apparent_power", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"active_power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"apparent_power", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"rms_current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"rms_voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"active_power_ph_b", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"apparent_power_ph_b", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"rms_current_ph_b", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"rms_voltage_ph_b", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"active_power_ph_c", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"apparent_power_ph_c", ExposeType::Numeric, Access::State, "VA", nullptr, nullptr, 0},
    {"rms_current_ph_c", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"rms_voltage_ph_c", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"time_status", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"time_zone", ExposeType::Numeric, Access::State, "s", nullptr, nullptr, 0},
    {"dst_shift", ExposeType::Numeric, Access::State, "s", nullptr, nullptr, 0},
};

constexpr const char* kM[] = { "ZIGBEE_ERL" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},   // onOff MIN..MAX
};
}  // namespace

extern const PreparedDefinition kDef_ZIGBEE_ERL{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZIGBEE_ERL", .vendor="EDFLab",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::edflab
