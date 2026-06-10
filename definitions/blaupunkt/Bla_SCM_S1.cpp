// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Blaupunkt SCM-S1 — wrong-cluster cover fix 2026-06-11.
// Roller shutter. z2m drives this through the brightness/on-off proxy, NOT
// the closuresWindowCovering cluster:
//   fz.cover_position_via_brightness (genLevelCtrl 0x0008 currentLevel → position+state)
//   fz.cover_state_via_onoff         (genOnOff   0x0006 onOff → state)
//   tz.cover_via_brightness          (genLevelCtrl 0x0008 moveToLevel, position→level)
// The auto-port wired kFzCoverPosition / kTzCoverPosition (closuresWindowCovering
// 0x0102) + bound 0x0102 — the device emits nothing on 0x0102, so position
// decode AND control were both dead, and the `state` channel (z2m exposes
// cover_position().setAccess("state", ea.ALL)) was dropped entirely.
// Rewired to the generic via-brightness converters + bindings 0x0008 / 0x0006.
// Mirrors the Climax SCM-5ZBS fix (same SCM roller-shutter family).
// z2m-source: blaupunkt.ts #SCM-S1.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::blaupunkt {
namespace {
const FzConverter* const kFz_SCM_S1[] = {
    &::zhc::generic::kFzCoverPositionViaBrightness,
    &::zhc::generic::kFzCoverStateViaOnOff,
};
const TzConverter* const kTz_SCM_S1[] = {
    &::zhc::generic::kTzCoverViaBrightness,
};
constexpr const char* kModels_SCM_S1[] = { "SCM-2_00.00.03.15", "SCM-R_00.00.03.15TC", "SCM_00.00.03.14TC", "SCM_00.00.03.05TC" };

constexpr Expose kExp_SCM_S1[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"state",    ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_SCM_S1[] = {
    {1, 0x0008},
    {1, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_SCM_S1{
    .zigbee_models=kModels_SCM_S1, .zigbee_models_count=sizeof(kModels_SCM_S1)/sizeof(kModels_SCM_S1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SCM-S1", .vendor="Blaupunkt",
    .meta=nullptr, .exposes=kExp_SCM_S1, .exposes_count=sizeof(kExp_SCM_S1)/sizeof(kExp_SCM_S1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SCM_S1, .from_zigbee_count=sizeof(kFz_SCM_S1)/sizeof(kFz_SCM_S1[0]),
    .to_zigbee=kTz_SCM_S1, .to_zigbee_count=sizeof(kTz_SCM_S1)/sizeof(kTz_SCM_S1[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_SCM_S1, .bindings_count=sizeof(kBind_SCM_S1)/sizeof(kBind_SCM_S1[0]),
};

}  // namespace zhc::devices::blaupunkt
