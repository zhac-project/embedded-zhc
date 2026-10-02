// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Candeo C-ZB-SSFS smart switched fused spur (z2m v26.115.1 window).
// z2m m.onOff + m.electricityMeter with fixed divisors (power W, voltage V,
// current /1000 A, energy /100 kWh) + Candeo's genOnOff attributes:
// 0x8002 power_on_behavior (enum8 off/on/previous), 0x8000 child_lock (bool).
// child_lock publishes a boolean (z2m: LOCK/UNLOCK); writes take either.
// Not ported: z2m's enforce_child_lock option on on/off writes.
// z2m-source: candeo.ts #C-ZB-SSFS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::candeo {
namespace {
constexpr ::zhc::generic::ZclWriteLookup kPob[] = { {"off", 0}, {"on", 1}, {"previous", 2} };
constexpr ::zhc::generic::ZclWriteLookup kLock[] = { {"LOCK", 1}, {"UNLOCK", 0}, {"lock", 1}, {"unlock", 0} };
constexpr ::zhc::generic::ZclAttrRow kOnOffRows[] = {
    { 0x8002, "power_on_behavior", 1, kPob, 3 },
    { 0x8000, "child_lock", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
};
constexpr ::zhc::generic::ZclAttrMap kOnOffMap{ kOnOffRows, 2 };
constexpr ::zhc::generic::ZclAttrRow kElecRows[] = {
    { 0x050B, "power" }, { 0x0505, "voltage" }, { 0x0508, "current", 1000 },
};
constexpr ::zhc::generic::ZclAttrMap kElecMap{ kElecRows, 3 };
constexpr ::zhc::generic::ZclAttrRow kMeterRows[] = { { 0x0000, "energy", 100 } };
constexpr ::zhc::generic::ZclAttrMap kMeterMap{ kMeterRows, 1 };
constexpr FzConverter kFzOnOffExtras = ::zhc::generic::zcl_attr_fz("genOnOff", &kOnOffMap);
constexpr FzConverter kFzElec = ::zhc::generic::zcl_attr_fz("haElectricalMeasurement", &kElecMap);
constexpr FzConverter kFzMeter = ::zhc::generic::zcl_attr_fz("seMetering", &kMeterMap);
constexpr ::zhc::generic::ZclWriteSpec kPobSpec{ "power_on_behavior", 0x8002, 0x30, 0, kPob, 3 };
constexpr ::zhc::generic::ZclWriteSpec kLockSpec{ "child_lock", 0x8000, 0x10, 0, kLock, 4 };
constexpr TzConverter kTzPob = ::zhc::generic::zcl_write_tz("genOnOff", 0x0006, &kPobSpec);
constexpr TzConverter kTzLock = ::zhc::generic::zcl_write_tz("genOnOff", 0x0006, &kLockSpec);
constexpr std::uint8_t kReadOnOffExtras[] = { 0x00, 0x80, 0x02, 0x80 };   // 0x8000, 0x8002
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &kFzOnOffExtras,
    &kFzElec,
    &kFzMeter,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &kTzPob,
    &kTzLock,
};
constexpr const char* kOpts1[] = { "off", "on", "previous" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Controls the behavior when the device is powered on after power loss", kOpts1, 3, ExposeCategory::Config},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr, "Temporarily enables / disables physical input on the device until the next on command", nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
    {"energy", ExposeType::Numeric, Access::State, "kWh", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "C-ZB-SSFS" };
constexpr const char* kN[] = { "Candeo" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
    {1, 0x0B04},
    {1, 0x0702},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
    {1, 0x0B04, 0x050B, 0x29, 5, 300, 10, 0},
    {1, 0x0B04, 0x0505, 0x21, 5, 600, 5, 0},
    {1, 0x0B04, 0x0508, 0x21, 5, 900, 10, 0},
    {1, 0x0702, 0x0000, 0x25, 5, 1800, 50, 0},
};
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0x0006, 0x00, 0, kReadOnOffExtras, sizeof(kReadOnOffExtras), 0 },
};
}  // namespace

extern const PreparedDefinition kDef_C_ZB_SSFS{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="C-ZB-SSFS", .vendor="Candeo",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::candeo
