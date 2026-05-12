// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Perenio PEHWE20 — hand-rewritten (vendor parity sweep).
// Two-channel single-wire mini-relay (l1=ep1, l2=ep2, diag=ep10).
// z2m-source: perenio.ts #PEHWE20.
//
// Adds power_on_behavior + switch_type + diagnostic LQI/RSSI relative
// to the previous auto-generated stub. Multi-endpoint exposes follow
// z2m's `withEndpoint("l1"/"l2")` shape; v1 dispatch keeps the keys
// flat and the platform layer handles per-endpoint demux.
#include "definitions/_generic/_shared.hpp"
#include "definitions/perenio/_shared.hpp"

namespace zhc::devices::perenio {
namespace {
const FzConverter* const kFz_PEHWE20[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzPowerOnBehavior1,
    &kFzPerenioDiagnostic,
    &kFzPerenioSwitchType,
};
const TzConverter* const kTz_PEHWE20[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzPowerOnBehavior1,
    &kTzPerenioSwitchType,
};
constexpr const char* kModels_PEHWE20[] = { "PEHWE20", "PEHWE2X" };

constexpr const char* kSwitchTypeEnum[] = {
    "maintained_state", "maintained_toggle",
    "momentary_state",  "momentary_press", "momentary_release",
};
constexpr const char* kPowerOnBehaviorEnum[] = {
    "off", "on", "toggle", "previous",
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state_l1",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior_l1", ExposeType::Enum,    Access::StateSet, nullptr, nullptr,
        kPowerOnBehaviorEnum, sizeof(kPowerOnBehaviorEnum)/sizeof(kPowerOnBehaviorEnum[0])},
    {"switch_type_l1",       ExposeType::Enum,    Access::StateSet, nullptr, nullptr,
        kSwitchTypeEnum, sizeof(kSwitchTypeEnum)/sizeof(kSwitchTypeEnum[0])},
    {"state_l2",             ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power_on_behavior_l2", ExposeType::Enum,    Access::StateSet, nullptr, nullptr,
        kPowerOnBehaviorEnum, sizeof(kPowerOnBehaviorEnum)/sizeof(kPowerOnBehaviorEnum[0])},
    {"switch_type_l2",       ExposeType::Enum,    Access::StateSet, nullptr, nullptr,
        kSwitchTypeEnum, sizeof(kSwitchTypeEnum)/sizeof(kSwitchTypeEnum[0])},
    {"last_message_lqi",     ExposeType::Numeric, Access::State,    "lqi", nullptr, nullptr, 0},
    {"last_message_rssi",    ExposeType::Numeric, Access::State,    "dB",  nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1,  0x0006},   // genOnOff (l1)
    {2,  0x0006},   // genOnOff (l2)
    {10, 0x0B05},   // haDiagnostic
};

extern const PreparedDefinition kDef_PEHWE20{
    .zigbee_models=kModels_PEHWE20, .zigbee_models_count=sizeof(kModels_PEHWE20)/sizeof(kModels_PEHWE20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PEHWE20", .vendor="Perenio",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_PEHWE20, .from_zigbee_count=sizeof(kFz_PEHWE20)/sizeof(kFz_PEHWE20[0]),
    .to_zigbee=kTz_PEHWE20, .to_zigbee_count=sizeof(kTz_PEHWE20)/sizeof(kTz_PEHWE20[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::perenio
