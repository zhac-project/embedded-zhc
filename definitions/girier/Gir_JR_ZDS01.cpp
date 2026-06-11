// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Girier JR-ZDS01 — 1 gang mini switch.
// z2m-source: girier.ts #JR-ZDS01 — tuya.modernExtend.tuyaOnOff({switchType: true}).
//
// The generated Tier-1 def carried ONLY the on/off `state`. z2m's
// tuyaOnOff() always adds `power_on_behavior` (default branch:
// tuyaFz.power_on_behavior_1 — genOnOff attr 0x8002 moesStartUpOnOff,
// lookup {0:off,1:on,2:previous}) and — because {switchType:true} — adds
// `switch_type` (tuyaFz.switch_type — manuSpecificTuya3/0xE001 attr 0xD030
// switchType, lookup {0:toggle,1:state,2:momentary}). Both attribute
// decoders + their write paths are wired here via the existing
// definitions/tuya/_shared converters (kFzTuyaPowerOnBehavior /
// kFzTuyaSwitchType and their kTz* twins). This is NOT a TS0601 0xEF00
// Tuya-DP device: state rides genOnOff (0x0006), the config attrs ride
// standard ZCL / Tuya-manuSpecific attribute reports.
//
// NOTE: the shared kFzTuyaPowerOnBehavior table maps value 2->"toggle",
// 3->"previous" (Tuya superset). z2m's power_on_behavior_1 maps 2->"previous"
// (no separate toggle). Values 0/1 agree exactly; only the value-2 label
// diverges from z2m for this attribute. The expose value list below mirrors
// z2m ([off,on,previous]); fixing the shared table is cross-vendor infra and
// out of scope for this single-vendor pass.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::girier {
namespace {
const FzConverter* const kFz_JR_ZDS01[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaPowerOnBehavior,   // genOnOff 0x8002 moesStartUpOnOff
    &::zhc::tuya::kFzTuyaSwitchType,        // manuSpecificTuya2 (0xE001) 0xD030 switchType
};
const TzConverter* const kTz_JR_ZDS01[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::tuya::kTzTuyaPowerOnBehavior,
    &::zhc::tuya::kTzTuyaSwitchType,
};
constexpr const char* kModels_JR_ZDS01[] = { "TS0001" };
constexpr const char* kManus_JR_ZDS01[] = { "_TZ3000_majwnphg", "_TZ3000_6axxqqi2", "_TZ3000_zw7yf6yk" };
}  // namespace


// Expose enum value lists (mirror z2m girier.ts #JR-ZDS01 exposes).
constexpr const char* kPowerOnBehaviorValues_JR_ZDS01[] = { "off", "on", "previous" };
constexpr const char* kSwitchTypeValues_JR_ZDS01[]      = { "toggle", "state", "momentary" };

constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kPowerOnBehaviorValues_JR_ZDS01, sizeof(kPowerOnBehaviorValues_JR_ZDS01)/sizeof(kPowerOnBehaviorValues_JR_ZDS01[0])},
    {"switch_type", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSwitchTypeValues_JR_ZDS01, sizeof(kSwitchTypeValues_JR_ZDS01)/sizeof(kSwitchTypeValues_JR_ZDS01[0])},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_JR_ZDS01{
    .zigbee_models=kModels_JR_ZDS01, .zigbee_models_count=sizeof(kModels_JR_ZDS01)/sizeof(kModels_JR_ZDS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_JR_ZDS01, .manufacturer_names_count=sizeof(kManus_JR_ZDS01)/sizeof(kManus_JR_ZDS01[0]),
    .model="JR-ZDS01", .vendor="Girier",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_JR_ZDS01, .from_zigbee_count=sizeof(kFz_JR_ZDS01)/sizeof(kFz_JR_ZDS01[0]),
    .to_zigbee=kTz_JR_ZDS01, .to_zigbee_count=sizeof(kTz_JR_ZDS01)/sizeof(kTz_JR_ZDS01[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::girier
