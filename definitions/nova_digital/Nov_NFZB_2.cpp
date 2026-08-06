// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Nova Digital NFZB-2 2-gang switch with backlight, countdown and
// inching (z2m v26.92.0 parity, added v26.82.0).
// z2m-source: nova_digital.ts #NFZB-2  (fingerprint TS0002 / _TZ3210_5ksufhqi)
//
// One of the three devices the family-modelID coverage bug hid: it shares the
// TS0002 modelID with existing defs but is a distinct device by manufacturer.
//
// On/off rides standard genOnOff on two endpoints (l1 = EP1, l2 = EP2).
// DEFERRED: the tuyaOnOff options (powerOutageMemory, backlightModeOffOn,
// indicatorMode, onOffCountdown, inchingSwitch) all ride the Tuya private
// cluster and have no generic converter -- the gangs themselves are covered.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::nova_digital {
namespace {
const FzConverter* const kFz[] = { &::zhc::generic::kFzOnOff };
const TzConverter* const kTz[] = { &::zhc::generic::kTzOnOff };
constexpr const char* kModels[] = { "TS0002" };
constexpr const char* kManus[]  = { "_TZ3210_5ksufhqi" };

constexpr Expose kExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, "Switch state", nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
};
}  // namespace

extern const PreparedDefinition kDef_NFZB_2{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="NFZB-2", .vendor="Nova Digital",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};

}  // namespace zhc::devices::nova_digital
