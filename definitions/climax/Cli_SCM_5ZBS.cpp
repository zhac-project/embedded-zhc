// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Climax SCM-5ZBS — wrong-cluster cover fix 2026-06-10.
// Roller shutter. z2m drives this through the brightness/on-off proxy, NOT
// the closuresWindowCovering cluster:
//   fz.cover_position_via_brightness (genLevelCtrl 0x0008 currentLevel → position+state)
//   fz.cover_state_via_onoff         (genOnOff   0x0006 onOff → state)
//   tz.cover_via_brightness          (genLevelCtrl 0x0008 moveToLevel, position→level)
// The auto-port wired kFzCoverPosition / kTzCoverPosition (closuresWindowCovering
// 0x0102) + bound 0x0102 — the device emits nothing on 0x0102, so position
// decode AND control were both dead. Rewired to the generic via-brightness
// converters + bindings 0x0008 / 0x0006.
// z2m-source: climax.ts #SCM-5ZBS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::climax {
namespace {
const FzConverter* const kFz_SCM_5ZBS[] = {
    &::zhc::generic::kFzCoverPositionViaBrightness,
    &::zhc::generic::kFzCoverStateViaOnOff,
};
const TzConverter* const kTz_SCM_5ZBS[] = {
    &::zhc::generic::kTzCoverViaBrightness,
};
constexpr const char* kModels_SCM_5ZBS[] = { "SCM-3_00.00.03.15" };

constexpr Expose kExp_SCM_5ZBS[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"state",    ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_SCM_5ZBS[] = {
    {1, 0x0008},
    {1, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_SCM_5ZBS{
    .zigbee_models=kModels_SCM_5ZBS, .zigbee_models_count=sizeof(kModels_SCM_5ZBS)/sizeof(kModels_SCM_5ZBS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SCM-5ZBS", .vendor="Climax",
    .meta=nullptr, .exposes=kExp_SCM_5ZBS, .exposes_count=sizeof(kExp_SCM_5ZBS)/sizeof(kExp_SCM_5ZBS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SCM_5ZBS, .from_zigbee_count=sizeof(kFz_SCM_5ZBS)/sizeof(kFz_SCM_5ZBS[0]),
    .to_zigbee=kTz_SCM_5ZBS, .to_zigbee_count=sizeof(kTz_SCM_5ZBS)/sizeof(kTz_SCM_5ZBS[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SCM_5ZBS, .bindings_count=sizeof(kBind_SCM_5ZBS)/sizeof(kBind_SCM_5ZBS[0]),
};

}  // namespace zhc::devices::climax
