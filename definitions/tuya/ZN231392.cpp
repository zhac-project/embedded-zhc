// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya ZN231392 smart water/gas valve.
//
// z2m tuyaOnOff({indicatorMode}): switch + power_on_behavior (genOnOff
// 0x8002, off/on/previous) + indicator_mode (0x8001, off / off/on /
// on/off / on). Configure = magic packet + read [onOff, moesStartUpOnOff].
// Without this def the valve fell to the generic TS0001 / TS011F / TS0011
// switch defs (TS011F with phantom power/voltage/current/energy).
//
// z2m has three fingerprint lists (TS011F, TS0001, TS0011). A def matches
// models × manufacturers as a cross product, so each list is its own def
// over the same converter set.
//
// Kept from the generic switch defs, beyond z2m: genOnOff bind + onOff
// reporting (z2m's ZN231392 configure only reads), so manual valve
// changes keep reaching the hub.
//
// z2m-source: tuya.ts #ZN231392.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
namespace zhc::devices::tuya {
namespace {
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

constexpr const char* kPowerOnValues[]   = { "off", "previous", "on" };
constexpr const char* kIndicatorValues[] = { "off", "off/on", "on/off", "on" };
constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr,
     "Controls the behavior when the device is powered on after power loss",
     kPowerOnValues, 3, ExposeCategory::Config},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr, "LED indicator mode",
     kIndicatorValues, 4, ExposeCategory::Config},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
};

constexpr std::uint8_t kReadOnOffAttrs[] = {
    0x00, 0x00,   // onOff
    0x02, 0x80,   // moesStartUpOnOff (0x8002)
};
constexpr ConfigStep kConfigSteps[] = {
    { ConfigStepOp::Read, 1, 0x0000, 0x00, 0,
      ::zhc::tuya::kTuyaMagicPacketAttrs, sizeof(::zhc::tuya::kTuyaMagicPacketAttrs), 0 },
    { ConfigStepOp::Read, 1, 0x0006, 0x00, 0, kReadOnOffAttrs, sizeof(kReadOnOffAttrs), 0 },
};

constexpr const char* kModelsTS011F[] = { "TS011F" };
constexpr const char* kManusTS011F[]  = { "_TZ3000_rk2yzt0u", "_TZ3000_o4cjetlm" };
constexpr const char* kModelsTS0001[] = { "TS0001" };
constexpr const char* kManusTS0001[]  = {
    "_TZ3000_o4cjetlm", "_TZ3000_iedbgyxt", "_TZ3000_h3noz0a5",
    "_TYZB01_4tlksk8a", "_TZ3000_5ucujjts", "_TZ3000_h8ngtlxy",
    "_TZ3000_w0ypwa1f", "_TZ3000_wpueorev", "_TZ3000_cmcjbqup",
};
constexpr const char* kModelsTS0011[] = { "TS0011" };
constexpr const char* kManusTS0011[]  = { "_TYZB01_rifa0wlb" };
}  // namespace

#define ZHC_ZN231392_DEF(var, models, manus)                                              \
    extern const PreparedDefinition var{                                                  \
        .zigbee_models=models, .zigbee_models_count=sizeof(models)/sizeof(models[0]),     \
        .manufacturer_name_prefix=nullptr,                                                \
        .manufacturer_names=manus,                                                        \
        .manufacturer_names_count=sizeof(manus)/sizeof(manus[0]),                         \
        .model="ZN231392", .vendor="Tuya",                                                \
        .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]), \
        .white_labels=nullptr, .white_labels_count=0,                                     \
        .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),                  \
        .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),                      \
        .configure=nullptr, .on_event=nullptr,                                            \
        .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),      \
        .reports=::zhc::tuya::kReportsOnOff_1ep,                                          \
        .reports_count=::zhc::tuya::kReportsOnOff_1ep_count,                              \
        .config_steps=kConfigSteps,                                                       \
        .config_steps_count=sizeof(kConfigSteps)/sizeof(kConfigSteps[0]),                 \
    };
ZHC_ZN231392_DEF(kDefZN231392_TS011F, kModelsTS011F, kManusTS011F)
ZHC_ZN231392_DEF(kDefZN231392_TS0001, kModelsTS0001, kManusTS0001)
ZHC_ZN231392_DEF(kDefZN231392_TS0011, kModelsTS0011, kManusTS0011)
#undef ZHC_ZN231392_DEF

}  // namespace zhc::devices::tuya
