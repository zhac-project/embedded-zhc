// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Zemismart KES-606US-L1 1-gang light switch (z2m v26.115.1 window).
// TS0001 `_TZ3000_w5s3mbyn`: on/off + Tuya power_on_behavior (genOnOff 0x8002)
// + indicator_mode (0x8001); configure = magic packet + onOff bind.
// z2m-source: zemismart.ts #KES-606US-L1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::zemismart {
namespace {
constexpr std::uint8_t kReadOnOffAttrs[] = { 0x00, 0x00, 0x02, 0x80 };   // onOff, 0x8002
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaPowerOnBehavior,
    &::zhc::tuya::kFzTuyaIndicatorMode,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::tuya::kTzTuyaPowerOnBehavior,
    &::zhc::tuya::kTzTuyaIndicatorMode,
};
constexpr const char* kOpts1[] = { "off", "previous", "on" };
constexpr const char* kOpts2[] = { "off", "off/on", "on/off", "on" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, "Controls the behavior when the device is powered on after power loss", kOpts1, 3, ExposeCategory::Config},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr, "LED indicator mode", kOpts2, 4, ExposeCategory::Config},
};
constexpr const char* kM[] = { "TS0001" };
constexpr const char* kN[] = { "_TZ3000_w5s3mbyn" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0006, 0x0000, 0x10, 0, 65000, 1, 0},
};
constexpr ConfigStep kSteps[] = {
    { ConfigStepOp::Read, 1, 0x0000, 0x00, 0, ::zhc::tuya::kTuyaMagicPacketAttrs, sizeof(::zhc::tuya::kTuyaMagicPacketAttrs), 0 },
    { ConfigStepOp::Read, 1, 0x0006, 0x00, 0, kReadOnOffAttrs, sizeof(kReadOnOffAttrs), 0 },
};
}  // namespace

extern const PreparedDefinition kDef_KES_606US_L1{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="KES-606US-L1", .vendor="Zemismart",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .config_steps=kSteps, .config_steps_count=sizeof(kSteps)/sizeof(kSteps[0]),
};

}  // namespace zhc::devices::zemismart
