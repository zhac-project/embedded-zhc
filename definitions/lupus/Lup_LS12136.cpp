// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lupus LS12136 2-gang outdoor mains socket, no power meter
// (z2m v26.133.0). TS011F `_TZ3000_9dm33lp8`; before this def it fell to the
// model-only metering plug in tuya/TS011F.cpp.
//
// z2m tuyaBase() + tuyaOnOff({powerOutageMemory, indicatorMode, childLock,
// endpoints: [left, right]}), endpoint {left: 1, right: 2}:
//   state_left / state_right  genOnOff 0x0000 on EP1 / EP2
//   power_outage_memory       genOnOff 0x8002 enum {0 off, 1 on, 2 restore}
//                             (powerOutageMemory replaces power_on_behavior)
//   indicator_mode            genOnOff 0x8001 enum off / off-on / on-off / on
//   child_lock                genOnOff 0x8000 bool
// Unsuffixed keys are written to EP1. Reports from EP1 come out with the
// `_left` suffix (the dispatcher suffixes every key of a mapped endpoint).
// Configure = magic packet + bind genOnOff on EP1 only, as upstream.
//
// z2m-source: lupus.ts #LS12136.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::lupus {
namespace {
using ::zhc::generic::ZclAttrMap;
using ::zhc::generic::ZclAttrRow;
using ::zhc::generic::ZclWriteLookup;
using ::zhc::generic::ZclWriteSpec;

// z2m tuyaFz.power_outage_memory + tuyaFz.child_lock / tuyaTz.power_on_behavior_1.
constexpr ZclWriteLookup kPomLut[] = { {"off", 0}, {"on", 1}, {"restore", 2} };
constexpr ZclAttrRow kRows[] = {
    { 0x8002, "power_outage_memory", 1, kPomLut, 3 },
    { 0x8000, "child_lock", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool },
};
constexpr ZclAttrMap kMap{ kRows, 2 };
constexpr FzConverter kFzExtras = ::zhc::generic::zcl_attr_fz("genOnOff", &kMap);
constexpr ZclWriteSpec kSpecPom{ "power_outage_memory", 0x8002, 0x30, 0, kPomLut, 3 };
constexpr TzConverter kTzPom = ::zhc::generic::zcl_write_tz("genOnOff", 0x0006, &kSpecPom, 1);

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaIndicatorMode,
    &kFzExtras,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &kTzPom,
    &::zhc::tuya::kTzTuyaIndicatorMode,
    &::zhc::tuya::kTzTuyaChildLock,
};

constexpr const char* kPomOpts[]       = { "on", "off", "restore" };
constexpr const char* kIndicatorOpts[] = { "off", "off/on", "on/off", "on" };
constexpr Expose kExposes[] = {
    {"state_left",  ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_right", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_outage_memory", ExposeType::Enum, Access::StateSet, nullptr,
     "Recover state after power outage", kPomOpts, 3, ExposeCategory::Config},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "LED indicator mode", kIndicatorOpts, 4, ExposeCategory::Config},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr,
     "Enables/disables physical input on the device", nullptr, 0},
};

constexpr const char* kModels[] = { "TS011F" };
constexpr const char* kManus[]  = { "_TZ3000_9dm33lp8" };
constexpr BindingSpec kBindings[] = { {1, 0x0006} };
constexpr ::zhc::EndpointLabel kEndpoints[] = { {"left", 1}, {"right", 2} };
}  // namespace

extern const PreparedDefinition kDef_LS12136{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="LS12136", .vendor="Lupus",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::lupus
