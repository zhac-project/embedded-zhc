// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: SONOFF SNZT-04P smart door/window sensor (z2m v26.115.1 window).
// z2m: m.iasZoneAlarm({zoneType: contact, zoneAttributes: [alarm_1]}) — contact
// only, so the zone's tamper bit is not published (kFzIasContactAlarmOnly) —
// tamper from eWeLink 0xFC11 attribute 0x2000 (u8, read with the Coolkit
// manufacturer code 0x1286) and battery % reported 1..2 h, change 2.
// z2m-source: sonoff.ts #SNZT-04P.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sonoff {
namespace {
constexpr ::zhc::generic::ZclAttrRow kTamperRows[] = {
    { 0x2000, "tamper", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
};
constexpr ::zhc::generic::ZclAttrMap kTamperMap{ kTamperRows, 1 };
constexpr FzConverter kFzTamper = ::zhc::generic::zcl_attr_fz("manuSpecificWoolley", &kTamperMap);   // 0xFC11
constexpr std::uint8_t kReadTamper[] = { 0x00, 0x20 };
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasContactAlarmOnly,
    &kFzTamper,
    &::zhc::generic::kFzBattery,
};
constexpr Expose kExp[] = {
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "SNZT-04P" };
constexpr BindingSpec kBind[] = {
    {1, 0x0001},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0001, 0x0021, 0x20, 3600, 7200, 2, 0},
};
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0xFC11, 0x00, 0, kReadTamper, sizeof(kReadTamper), 0, 0x1286 },
};
}  // namespace

extern const PreparedDefinition kDef_SNZT_04P{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SNZT-04P", .vendor="SONOFF",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::sonoff
