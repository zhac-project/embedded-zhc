// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: hand-tuned. Avatto LZWSM16-1 — 1 gang switch module (no neutral).
// z2m-source: avatto.ts #LZWSM16-1.
//
// z2m lowers `tuyaOnOff({switchType: true, onOffCountdown: true})` into:
//   - genOnOff state + bind/report
//   - switch_type (enum toggle/state/momentary) on manuSpecificTuya3 0xD030
//   - countdown numeric via genOnOff onWithTimedOff (cmd 0x42)
// Plus `m.forcePowerSource({powerSource: "Mains (single phase)"})` — pinned
// power_source override. Will be wired through Phase C `power_source_override`
// field once that lands; for now the device's ZCL Basic 0x0007 read still
// fills the field.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/extend.hpp"

namespace zhc::devices::avatto {
namespace {

const FzConverter* const kFz_LZWSM16_1[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaSwitchType,
};
const TzConverter* const kTz_LZWSM16_1[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::tuya::kTzTuyaSwitchType,
    &::zhc::generic::kTzOnWithTimedOff,
};
constexpr const char* kModels_LZWSM16_1[] = { "TS0011" };
constexpr const char* kManus_LZWSM16_1[] = { "_TZ3000_hbxsdd6k" };

constexpr const char* kSwitchTypeValues[] = { "toggle", "state", "momentary" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"state",       ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"switch_type", ExposeType::Enum,    Access::StateSet, nullptr,
        "External switch wiring style",
        kSwitchTypeValues, sizeof(kSwitchTypeValues)/sizeof(kSwitchTypeValues[0]),
        ExposeCategory::Config},
    {"countdown",   ExposeType::Numeric, Access::StateSet, "s",
        "Auto-off countdown via onWithTimedOff (0..43200s, 0 cancels)",
        nullptr, 0,
        ExposeCategory::Config},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},   // genOnOff
    {1, 0xE001},   // manuSpecificTuya3 (switch_type)
};

extern const PreparedDefinition kDef_LZWSM16_1{
    .zigbee_models=kModels_LZWSM16_1, .zigbee_models_count=sizeof(kModels_LZWSM16_1)/sizeof(kModels_LZWSM16_1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_LZWSM16_1, .manufacturer_names_count=sizeof(kManus_LZWSM16_1)/sizeof(kManus_LZWSM16_1[0]),
    .model="LZWSM16-1", .vendor="Avatto",
    .meta=&::zhc::tuya::extend::kTuyaMagicPacket_readAttrs,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LZWSM16_1, .from_zigbee_count=sizeof(kFz_LZWSM16_1)/sizeof(kFz_LZWSM16_1[0]),
    .to_zigbee=kTz_LZWSM16_1, .to_zigbee_count=sizeof(kTz_LZWSM16_1)/sizeof(kTz_LZWSM16_1[0]),
    .configure=::zhc::tuya::extend::magic_packet(),
    .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=nullptr, .reports_count=0,
    .config_steps=nullptr, .config_steps_count=0,
    .config_callbacks=nullptr, .config_callbacks_count=0,
    .endpoint_map=nullptr, .endpoint_map_count=0,
    .endpoint_action_suffix=false,
    .default_endpoint=0,
    // z2m source: m.forcePowerSource({powerSource: "Mains (single phase)"}).
    .power_source_override=0x01,  // ZCL Basic 0x0007 = Mains (single phase)
};

}  // namespace zhc::devices::avatto
