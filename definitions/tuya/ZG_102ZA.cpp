// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: HOBEIAN ZG-102ZA door/window sensor (zigbeeModel ZG-102Z). z2m v26.115.1
// splits it out of TS0203 into hobeian.ts: contact from the IAS zone (command
// and zoneStatus attribute report, inverted bit 0), battery percentage and
// voltage; no tamper / battery_low. Not ported: the 3V_1500_2800 voltage curve
// (the percentage comes from batteryPercentageRemaining).
// z2m-source: hobeian.ts #ZG-102ZA.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::tuya {
namespace {
// fz.ias_contact_alarm_1_report: zoneStatus attribute (0x0002) -> contact = !bit0.
bool fz_contact_report(const DecodedMessage& msg, const FzConverter&, const PreparedDefinition&, RuntimeContext&,
                       FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("2");
    if (!v || v->type != ValueType::Uint) return false;
    Value o{}; o.type = ValueType::Bool; o.b = (v->u & 0x1) == 0;
    return out.put("contact", o);
}
constexpr FzConverter kFzContactReport{
    .family = FrameFamily::Zcl, .cluster = "ssIasZone",
    .type_mask = type_bit(MessageType::AttributeReport) | type_bit(MessageType::ReadResponse),
    .command_id = WILDCARD_CMD_ID, .attr_id = WILDCARD_ATTR_ID, .endpoint = WILDCARD_ENDPOINT,
    .frame_flags_mask = 0, .frame_flags_value = 0, .direction = Direction::ServerToClient,
    .fn = { .zcl_fn = fz_contact_report }, .user_config = nullptr,
};
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasContactAlarmOnly,
    &kFzContactReport,
    &::zhc::generic::kFzBattery,
};
constexpr Expose kExp[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "ZG-102Z" };
constexpr BindingSpec kBind[] = {
    {1, 0x0001},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 0, 0},
    {1, 0x0001, 0x0020, 0x20, 3600, 65000, 0, 0},
};
}  // namespace

extern const PreparedDefinition kDef_ZG_102ZA{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZG-102ZA", .vendor="HOBEIAN",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::tuya
