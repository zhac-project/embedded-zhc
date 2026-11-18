// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS011F_5 power strip, 5 gang (z2m v26.133.0, #13611 power
// outage memory). Before this def the five manus fell to the model-only
// single-endpoint metering plug in tuya/TS011F.cpp, so l2–l5 were
// uncontrollable (the generated Gen__TZ3000_* copies are neutered).
//
// z2m tuyaBase() + tuyaOnOff({powerOutageMemory, childLock, onOffCountdown,
// endpoints: [l1 … l5]}), endpoint {l1: 1 … l5: 5},
// multiEndpointSkip [power_outage_memory]:
//   state_l1 … state_l5          genOnOff 0x0000 on EP1 … EP5
//   countdown_l1 … countdown_l5  genOnOff onTime 0x4001 / onWithTimedOff
//   power_outage_memory          genOnOff 0x8002 {0 off, 1 on, 2 restore}, EP1
//                                only (tzLocal.ts011f_5_power_outage_memory:
//                                the strip answers UNSUPPORTED_ATTRIBUTE but
//                                applies the write — a fire-and-forget write
//                                here)
//   child_lock                   genOnOff 0x8000 bool
// Reports from EP1 come out `_l1`-suffixed (the dispatcher has no per-def
// multiEndpointSkip). Configure = magic packet + bind genOnOff on EP1 … EP5.
//
// z2m-source: tuya.ts #TS011F_5.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
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
    &::zhc::generic::kFzOnOffCountdown,
    &kFzExtras,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzOnWithTimedOff,
    &kTzPom,
    &::zhc::tuya::kTzTuyaChildLock,
};

// z2m tuyaExposes.countdown(): 0-43200 s.
constexpr Expose countdown(const char* name) {
    return {name, ExposeType::Numeric, Access::StateSet, "s",
            "Toggle the device after a set duration (one time action)", nullptr, 0,
            ExposeCategory::State, 0, 43200, 1};
}
constexpr Expose state(const char* name) {
    return {name, ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0};
}
constexpr const char* kPomOpts[] = { "on", "off", "restore" };
constexpr Expose kExposes[] = {
    state("state_l1"), state("state_l2"), state("state_l3"), state("state_l4"), state("state_l5"),
    countdown("countdown_l1"), countdown("countdown_l2"), countdown("countdown_l3"),
    countdown("countdown_l4"), countdown("countdown_l5"),
    {"power_outage_memory", ExposeType::Enum, Access::StateSet, nullptr,
     "Recover state after power outage", kPomOpts, 3, ExposeCategory::Config},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr,
     "Enables/disables physical input on the device", nullptr, 0},
};

constexpr const char* kModels[] = { "TS011F" };
constexpr const char* kManus[]  = {
    "_TZ3000_cfnprab5", "_TZ3000_o005nuxx", "_TZ3000_gdyjfvgm", "_TZ3000_pl5v1yyy", "_TZ3000_djgzdba9",
};
constexpr WhiteLabel kWhiteLabels[] = {
    {"UseeLink", "SM-0306E-2W"},   // _TZ3000_cfnprab5
    {"UseeLink", "SM-O301-AZ"},    // _TZ3000_o005nuxx
    {"Lotus", "Ref 2117"},         // _TZ3000_djgzdba9
};
constexpr BindingSpec kBindings[] = {
    {1, 0x0006}, {2, 0x0006}, {3, 0x0006}, {4, 0x0006}, {5, 0x0006},
};
constexpr ::zhc::EndpointLabel kEndpoints[] = {
    {"l1", 1}, {"l2", 2}, {"l3", 3}, {"l4", 4}, {"l5", 5},
};
}  // namespace

extern const PreparedDefinition kDef_TS011F_5{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="TS011F_5", .vendor="Tuya",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=kWhiteLabels, .white_labels_count=sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::tuya
