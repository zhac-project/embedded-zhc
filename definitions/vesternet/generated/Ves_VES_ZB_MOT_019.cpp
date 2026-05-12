// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Vesternet VES-ZB-MOT-019 — hand-curated.
// Zigbee motor controller (closuresWindowCovering).
// NOTE: z2m sets `meta: {coverInverted: true}` — the generic
// `kTzCoverPosition` does NOT honour invertCover yet, so reported
// position values may be inverted vs the z2m UI. Tracked in
// docs/VESTERNET_PARITY.md.
// z2m-source: vesternet.ts #VES-ZB-MOT-019.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::vesternet {
namespace {
const FzConverter* const kFz_VES_ZB_MOT_019[] = {
    &::zhc::generic::kFzCoverPosition,
};
const TzConverter* const kTz_VES_ZB_MOT_019[] = {
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kModels_VES_ZB_MOT_019[] = { "HK-ZCC-A" };

}  // namespace


// --- hand-curated exposes / bindings ---
constexpr Expose kAutoExposes[] = {
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},
};
// --- end ---

extern const PreparedDefinition kDef_VES_ZB_MOT_019{
    .zigbee_models=kModels_VES_ZB_MOT_019, .zigbee_models_count=sizeof(kModels_VES_ZB_MOT_019)/sizeof(kModels_VES_ZB_MOT_019[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VES-ZB-MOT-019", .vendor="Vesternet",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VES_ZB_MOT_019, .from_zigbee_count=sizeof(kFz_VES_ZB_MOT_019)/sizeof(kFz_VES_ZB_MOT_019[0]),
    .to_zigbee=kTz_VES_ZB_MOT_019, .to_zigbee_count=sizeof(kTz_VES_ZB_MOT_019)/sizeof(kTz_VES_ZB_MOT_019[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::vesternet
