// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Yandex YNDX_00526 — graduated from generated/ (vendor-parity sweep).
// Contact sensor. z2m uses `m.iasZoneAlarm({zoneType:"contact"})` which emits
// the semantic key `contact` (zoneStatus bit 0). The generated def lowered the
// generic kFzIasZone, which emits the bare key "alarm" — that never matched the
// declared expose, so the primary sensor state never reached the shadow. Swap
// in the typed kFzIasContactAlarm (emits "contact" directly).
// z2m-source: yandex.ts #YNDX_00526 (m.iasZoneAlarm zoneType:"contact").
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yandex {
namespace {
const FzConverter* const kFz_YNDX_00526[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasContactAlarm,
};

constexpr const char* kModels_YNDX_00526[] = { "YNDX-00526" };

}  // namespace


// --- exposes / bindings (graduated; semantic IAS key) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_YNDX_00526{
    .zigbee_models=kModels_YNDX_00526, .zigbee_models_count=sizeof(kModels_YNDX_00526)/sizeof(kModels_YNDX_00526[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="YNDX_00526", .vendor="Yandex",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_YNDX_00526, .from_zigbee_count=sizeof(kFz_YNDX_00526)/sizeof(kFz_YNDX_00526[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::yandex
