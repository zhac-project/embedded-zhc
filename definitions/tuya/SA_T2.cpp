// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Nova Digital SA-T2 Safira smart socket, 2 gang (z2m v26.133.0).
// TS011F `_TZ3210_z1kba38n`; before this def it fell to the model-only
// metering plug in tuya/TS011F.cpp.
//
// z2m tuyaBase() + deviceEndpoints {l1: 1, l2: 2} + tuyaOnOff({
// powerOutageMemory, childLock, onOffCountdown, endpoints: [l1, l2]}) + a local
// indicator_mode extend:
//   state_l1 / state_l2          genOnOff 0x0000 on EP1 / EP2
//   countdown_l1 / countdown_l2  genOnOff onTime 0x4001 (offWaitTime 0x4002),
//                                set via onWithTimedOff (cmd 0x42)
//   power_outage_memory          genOnOff 0x8002 enum {0 off, 1 on, 2 restore}
//   child_lock                   genOnOff 0x8000 bool
//   indicator_mode               genOnOff 0x8001 enum off / off-on / on-off / on
// Upstream exposes indicator_mode only when EP1 carries seMetering (the other
// hardware revision answers UNSUPPORTED_ATTRIBUTE); a static def cannot look at
// the endpoint, so it is always exposed here. No metering is exposed (that
// revision reports 0 for V/I/P). Unsuffixed keys are written to EP1; reports
// from EP1 come out `_l1`-suffixed. Configure = magic packet only (upstream
// has no configure of its own).
//
// z2m-source: tuya.ts #SA-T2.
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
    &::zhc::tuya::kFzTuyaIndicatorMode,
    &kFzExtras,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzOnWithTimedOff,
    &kTzPom,
    &::zhc::tuya::kTzTuyaChildLock,
    &::zhc::tuya::kTzTuyaIndicatorMode,
};

constexpr const char* kPomOpts[]       = { "on", "off", "restore" };
constexpr const char* kIndicatorOpts[] = { "off", "off/on", "on/off", "on" };
// z2m tuyaExposes.countdown(): 0-43200 s.
constexpr Expose countdown(const char* name) {
    return {name, ExposeType::Numeric, Access::StateSet, "s",
            "Toggle the device after a set duration (one time action)", nullptr, 0,
            ExposeCategory::State, 0, 43200, 1};
}
constexpr Expose kExposes[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    countdown("countdown_l1"),
    countdown("countdown_l2"),
    {"power_outage_memory", ExposeType::Enum, Access::StateSet, nullptr,
     "Recover state after power outage", kPomOpts, 3, ExposeCategory::Config},
    {"child_lock", ExposeType::Binary, Access::StateSet, nullptr,
     "Enables/disables physical input on the device", nullptr, 0},
    {"indicator_mode", ExposeType::Enum, Access::StateSet, nullptr,
     "LED indicator mode", kIndicatorOpts, 4, ExposeCategory::Config},
};

constexpr const char* kModels[] = { "TS011F" };
constexpr const char* kManus[]  = { "_TZ3210_z1kba38n" };
constexpr ::zhc::EndpointLabel kEndpoints[] = { {"l1", 1}, {"l2", 2} };
}  // namespace

extern const PreparedDefinition kDef_SA_T2{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="SA-T2", .vendor="Nova Digital",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .config_steps=::zhc::tuya::kConfigStepsTuyaMagicPacket,
    .config_steps_count=::zhc::tuya::kConfigStepsTuyaMagicPacketCount,
    .endpoint_map=kEndpoints, .endpoint_map_count=sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::tuya
