// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: AOYAN AY301Z-2CH 2-gang wall switch module
// (z2m v26.95.0 parity, added v26.95.0).
// z2m-source: tuya.ts #AY301Z-2CH
//
// Not a Tuya-DP device: on/off rides standard genOnOff, one endpoint per gang
// (z2m m.deviceEndpoints {l1: 1, l2: 2} + tuya.modernExtend.tuyaOnOff).
// Same shape as the AY-6xxZL modules.
//
// PARITY FIX (v26.99.0 sweep): the def bound BOTH gang endpoints and even
// configured reporting on both, but declared a single bare `state` expose and
// NO endpoint_map. Without the map the dispatcher never suffixes the inbound
// key, so gang 2's report overwrote gang 1's state, and an outbound write had
// no endpoint to route to — the second gang was effectively uncontrollable.
// z2m exposes `state_l1` / `state_l2`; now so does this.
//
// DEFERRED: the tuyaOnOff options `switchType` and `onOffCountdown` ride the
// Tuya private cluster and have no generic converter. The gangs themselves,
// which is what the module is for, are fully covered.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"   // kReportsOnOff_2ep

namespace zhc::devices::tuya {
namespace {
const FzConverter* const kFz_AY301Z_2CH[] = { &::zhc::generic::kFzOnOff };
const TzConverter* const kTz_AY301Z_2CH[] = { &::zhc::generic::kTzOnOff };
constexpr const char* kModels_AY301Z_2CH[] = { "AY301Z-2CH" };
constexpr const char* kManus_AY301Z_2CH[]  = { "AOYAN" };

constexpr Expose kExposes_AY301Z_2CH[] = {
    {"state_l1", ExposeType::Binary, Access::StateSet, nullptr, "Switch state", nullptr, 0},
    {"state_l2", ExposeType::Binary, Access::StateSet, nullptr, "Switch state", nullptr, 0},
};

constexpr ::zhc::EndpointLabel kEndpoints_AY301Z_2CH[] = {
    {"l1", 1}, {"l2", 2},
};
constexpr BindingSpec kBindings_AY301Z_2CH[] = { {1, 0x0006}, {2, 0x0006} };
}  // namespace

extern const PreparedDefinition kDef_AY301Z_2CH{
    .zigbee_models=kModels_AY301Z_2CH, .zigbee_models_count=sizeof(kModels_AY301Z_2CH)/sizeof(kModels_AY301Z_2CH[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_AY301Z_2CH, .manufacturer_names_count=sizeof(kManus_AY301Z_2CH)/sizeof(kManus_AY301Z_2CH[0]),
    .model="AY301Z-2CH", .vendor="AOYAN",
    .meta=nullptr, .exposes=kExposes_AY301Z_2CH, .exposes_count=sizeof(kExposes_AY301Z_2CH)/sizeof(kExposes_AY301Z_2CH[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AY301Z_2CH, .from_zigbee_count=sizeof(kFz_AY301Z_2CH)/sizeof(kFz_AY301Z_2CH[0]),
    .to_zigbee=kTz_AY301Z_2CH, .to_zigbee_count=sizeof(kTz_AY301Z_2CH)/sizeof(kTz_AY301Z_2CH[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_AY301Z_2CH, .bindings_count=sizeof(kBindings_AY301Z_2CH)/sizeof(kBindings_AY301Z_2CH[0]),
    .reports=::zhc::tuya::kReportsOnOff_2ep, .reports_count=::zhc::tuya::kReportsOnOff_2ep_count,
    .endpoint_map       = kEndpoints_AY301Z_2CH,
    .endpoint_map_count = sizeof(kEndpoints_AY301Z_2CH)/sizeof(kEndpoints_AY301Z_2CH[0]),
};

}  // namespace zhc::devices::tuya
