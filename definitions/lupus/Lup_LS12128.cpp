// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lupus LS12128 — wrong-cluster cover fix 2026-06-10.
// Roller shutter (Climax SCM-family, OTA). z2m drives this through the
// brightness/on-off proxy, NOT the closuresWindowCovering cluster:
//   fz.cover_position_via_brightness (genLevelCtrl 0x0008 currentLevel → position+state)
//   fz.cover_state_via_onoff         (genOnOff   0x0006 onOff → state)
//   tz.cover_via_brightness          (genLevelCtrl 0x0008 moveToLevel, position→level)
// The auto-port wired kFzCoverPosition / kTzCoverPosition (closuresWindowCovering
// 0x0102) + bound 0x0102 — the device never speaks 0x0102, so position decode
// AND control were both dead, and the z2m `cover_position()` `state` expose was
// dropped. Rewired to the generic via-brightness converters + bindings
// 0x0008 / 0x0006, restored the `state` expose. Mirrors the Climax SCM-5ZBS fix.
// z2m-source: lupus.ts #LS12128.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lupus {
namespace {
const FzConverter* const kFz_LS12128[] = {
    &::zhc::generic::kFzCoverPositionViaBrightness,
    &::zhc::generic::kFzCoverStateViaOnOff,
};
const TzConverter* const kTz_LS12128[] = {
    &::zhc::generic::kTzCoverViaBrightness,
};
constexpr const char* kModels_LS12128[] = { "SCM-3-OTA_00.00.03.16TC", "SCM-6-OTA_00.00.03.17TC", "SCM-6-OTA_00.00.03.18TC", "SCM-6-OTA_00.00.03.20TC" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"state",    ExposeType::Binary,  Access::State,    nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0008},
    {1, 0x0006},
};

extern const PreparedDefinition kDef_LS12128{
    .zigbee_models=kModels_LS12128, .zigbee_models_count=sizeof(kModels_LS12128)/sizeof(kModels_LS12128[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LS12128", .vendor="Lupus",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LS12128, .from_zigbee_count=sizeof(kFz_LS12128)/sizeof(kFz_LS12128[0]),
    .to_zigbee=kTz_LS12128, .to_zigbee_count=sizeof(kTz_LS12128)/sizeof(kTz_LS12128[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lupus
