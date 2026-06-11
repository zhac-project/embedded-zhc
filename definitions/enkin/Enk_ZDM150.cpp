// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Enkin ZDM150 — 150W dimmer module.
//
// Gap fixed (WRONG class / phantom bundle): z2m drives this device with
// extend=[light({powerOnBehavior:false, effect:false}), forcePowerSource(...)].
// `m.light()` (lib/modernExtend.ts) gives on/off + brightness (genLevelCtrl
// 0x0008) plus the matching tz writers. The auto-port mis-classified it as a
// battery-only on/off device: it wired kFzBattery + a phantom battery/voltage
// expose + a genPowerCfg (0x0001) binding, and DROPPED brightness entirely —
// the core dimmer feature. A mains-powered dimmer has no battery (z2m
// forcePowerSource = "Mains (single phase)"). Fix: kFzOnOff + kFzBrightness,
// kTzOnOff + kTzBrightness, state + brightness exposes, bindings on genOnOff
// (0x0006) + genLevelCtrl (0x0008), and power_source_override = Mains.
//
// z2m-source: enkin.ts #ZDM150; lib/modernExtend.ts light() + forcePowerSource.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::enkin {
namespace {


constexpr const char* kModels_ZDM150[] = { "ZDM150" };

}  // namespace


namespace {
const FzConverter* const kFz_ZDM150[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_ZDM150[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
};
constexpr Expose kExp_ZDM150[] = {
    { "state",      ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness", ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_ZDM150[] = {
    { 1, 0x0006 }, { 1, 0x0008 },
};
}
extern const PreparedDefinition kDef_ZDM150{
    .zigbee_models=kModels_ZDM150, .zigbee_models_count=sizeof(kModels_ZDM150)/sizeof(kModels_ZDM150[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZDM150", .vendor="Enkin",
    .meta=nullptr, .exposes=kExp_ZDM150, .exposes_count=sizeof(kExp_ZDM150)/sizeof(kExp_ZDM150[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZDM150, .from_zigbee_count=sizeof(kFz_ZDM150)/sizeof(kFz_ZDM150[0]),
    .to_zigbee=kTz_ZDM150, .to_zigbee_count=sizeof(kTz_ZDM150)/sizeof(kTz_ZDM150[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_ZDM150,
    .bindings_count           = sizeof(kBind_ZDM150)/sizeof(kBind_ZDM150[0]),
    .power_source_override     = 0x01,  // ZCL Basic 0x0007 = Mains (single phase)
};

}  // namespace zhc::devices::enkin
