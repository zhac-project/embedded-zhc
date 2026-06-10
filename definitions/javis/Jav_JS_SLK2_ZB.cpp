// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Javis JS-SLK2-ZB — intelligent biometric digital lock.
// Intelligent biometric digital lock.
//
// Parity fix (z2m<->embedded-zhc): the auto-port wired this lock as a
// controllable on/off switch — kFzOnOff + kTzOnOff + a StateSet `state`
// expose + a genOnOff (0x0006) bind. z2m's def has an EMPTY toZigbee and
// is NOT controllable: `fromZigbee: [fz.javis_lock_report, fz.battery]`,
// `exposes: [e.battery(), e.action(["unlock"])]`. Dropped the phantom
// on/off control + bind and the dead `state` expose; declared the device's
// real role (battery diagnostic + `action`).
//
// INFRA defer: fz.javis_lock_report parses a custom raw genBasic attribute
// 0x4200 (16896) as a UTF-8 string and arms a 2s timer to re-publish a
// "lock" action — there is no generic converter for that raw-string/timer
// shape, so the `action`/`action_user`/`action_source` decode is not ported
// here. Battery (genPowerCfg) decodes via the generic kFzBattery.
//
// z2m-source: javis.ts #JS-SLK2-ZB; converters/fromZigbee.ts javis_lock_report.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::javis {
namespace {
const FzConverter* const kFz_JS_SLK2_ZB[] = {
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_JS_SLK2_ZB[] = { "JAVISLOCK", "doorlock_5001", "E321V000A03" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};

extern const PreparedDefinition kDef_JS_SLK2_ZB{
    .zigbee_models=kModels_JS_SLK2_ZB, .zigbee_models_count=sizeof(kModels_JS_SLK2_ZB)/sizeof(kModels_JS_SLK2_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="JS-SLK2-ZB", .vendor="Javis",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_JS_SLK2_ZB, .from_zigbee_count=sizeof(kFz_JS_SLK2_ZB)/sizeof(kFz_JS_SLK2_ZB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::javis
