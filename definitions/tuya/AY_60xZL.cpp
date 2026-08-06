// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: AOYAN AY-601ZL / AY-602ZL / AY-603ZL switch modules, no neutral
// wire, 1/2/3 gang (z2m v26.92.0 parity, added v26.79.0).
// z2m-source: tuya.ts #AY-601ZL / #AY-602ZL / #AY-603ZL
//
// These are NOT Tuya-DP devices: on/off rides standard genOnOff (z2m
// `tuya.modernExtend.tuyaOnOff`), one endpoint per gang. Mirrors the
// TS0011/TS0012/TS0013 shape.
//
// Each entry carries a second zigbeeModel — upstream folds the AY-8xxZL
// white-label into the AY-6xxZL definition, so both model ids must match.
//
// z2m `m.forcePowerSource({powerSource: "Mains (single phase)"})` maps to
// power_source_override = 0x01: these modules misreport Basic 0x0007.
//
// DEFERRED (recorded, not silently dropped): the tuyaOnOff options
// `backlightModeOffNormalInverted` and `onOffCountdown` ride the Tuya private
// cluster and have no generic ez converter — the gang on/off, which is what
// the device is for, is fully covered.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"   // kReportsOnOff_1ep / _2ep / _3ep
namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz[] = { &::zhc::generic::kFzOnOff };
const TzConverter* const kTz[] = { &::zhc::generic::kTzOnOff };

constexpr const char* kModels601[] = { "AY-601ZL", "AY-801ZL" };
constexpr const char* kModels602[] = { "AY-602ZL", "AY-802ZL" };
constexpr const char* kModels603[] = { "AY-603ZL", "AY-803ZL" };

constexpr Expose kExp[] = {
    { "state", ExposeType::Binary, ::zhc::Access::StateSet, nullptr, "Switch state", nullptr, 0 },
};

constexpr BindingSpec kBind1[] = { {1, 0x0006} };
constexpr BindingSpec kBind2[] = { {1, 0x0006}, {2, 0x0006} };
constexpr BindingSpec kBind3[] = { {1, 0x0006}, {2, 0x0006}, {3, 0x0006} };

constexpr WhiteLabel kWL601[] = { {"AOYAN", "AY-801ZL"} };
constexpr WhiteLabel kWL602[] = { {"AOYAN", "AY-802ZL"} };
constexpr WhiteLabel kWL603[] = { {"AOYAN", "AY-803ZL"} };
}  // namespace

extern const PreparedDefinition kDef_AY_601ZL{
    .zigbee_models=kModels601,.zigbee_models_count=2,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="AY-601ZL",.vendor="AOYAN",
    .meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL601,.white_labels_count=1,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBind1,.bindings_count=1,
    .reports=::zhc::tuya::kReportsOnOff_1ep,.reports_count=::zhc::tuya::kReportsOnOff_1ep_count,
    .power_source_override=0x01,  // ZCL Basic 0x0007 = Mains (single phase)
};

extern const PreparedDefinition kDef_AY_602ZL{
    .zigbee_models=kModels602,.zigbee_models_count=2,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="AY-602ZL",.vendor="AOYAN",
    .meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL602,.white_labels_count=1,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBind2,.bindings_count=2,
    .reports=::zhc::tuya::kReportsOnOff_2ep,.reports_count=::zhc::tuya::kReportsOnOff_2ep_count,
    .power_source_override=0x01,
};

extern const PreparedDefinition kDef_AY_603ZL{
    .zigbee_models=kModels603,.zigbee_models_count=2,
    .manufacturer_name_prefix=nullptr,.manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="AY-603ZL",.vendor="AOYAN",
    .meta=nullptr,.exposes=kExp,.exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=kWL603,.white_labels_count=1,
    .from_zigbee=kFz,.from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz,.to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr,.on_event=nullptr,
    .bindings=kBind3,.bindings_count=3,
    .reports=::zhc::tuya::kReportsOnOff_3ep,.reports_count=::zhc::tuya::kReportsOnOff_3ep_count,
    .power_source_override=0x01,
};

}  // namespace zhc::devices::tuya
