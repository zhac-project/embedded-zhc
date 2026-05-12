// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Ctm mTouch_Astro — auto-generated.
// mTouch Astro OP, astro clock
// z2m-source: ctm.ts #mTouch_Astro.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ctm/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
// mTouch Astro z2m: fz.on_off + tz.on_off + fz.command_{on,off} +
// fz.ctm_{device_mode,device_enabled,child_lock,group_config}. The
// custom genOnOff manuSpec attrs (deviceMode 0x4001, deviceEnabled
// 0x4002, childLock 0x4003) decode through the standard ZCL parser
// today — keys land as raw attribute IDs until ZHC gains a vendor
// fz_ctm_genonoff decoder. We surface them as hidden state and wire
// the canonical writers via the shared ctm helpers.
const FzConverter* const kFz_mTouch_Astro[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
};
const TzConverter* const kTz_mTouch_Astro[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::ctm::kTzCtmDeviceEnabled,
};
constexpr const char* kModels_mTouch_Astro[] = { "mTouch Astro" };

}  // namespace


// Manual: add device_mode (read-only enum, raw decode TODO),
// device_enabled (bool RW via kTzCtmDeviceEnabled), child_lock
// (read-only TODO), action (from command_*). group_id BLOCKED on
// ctmGroupConfig 0xFEA7 manuSpec.
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"device_enabled", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"device_mode", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"child_lock", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_mTouch_Astro{
    .zigbee_models=kModels_mTouch_Astro, .zigbee_models_count=sizeof(kModels_mTouch_Astro)/sizeof(kModels_mTouch_Astro[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="mTouch_Astro", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_mTouch_Astro, .from_zigbee_count=sizeof(kFz_mTouch_Astro)/sizeof(kFz_mTouch_Astro[0]),
    .to_zigbee=kTz_mTouch_Astro, .to_zigbee_count=sizeof(kTz_mTouch_Astro)/sizeof(kTz_mTouch_Astro[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
