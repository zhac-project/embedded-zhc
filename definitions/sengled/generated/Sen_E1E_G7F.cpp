// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Sengled E1E-G7F — hand-rewritten 2026-04-28.
// Smart switch (battery-powered remote, NOT a light)
// z2m-source: sengled.ts #E1E-G7F.
// Mapping: sengledSwitchAction()
//          -> custom raw action decoder over manufacturer cluster 0xfc10 (64528).
//          The 8-action lookup ("on" / "up" / "down" / "off" / "on_double" /
//          "on_long" / "off_double" / "off_long") is BLOCKED in ZHC: the
//          static-memory runtime does not yet ship a generic raw-frame
//          action FZ converter for vendor cluster 0xfc10 with the exact byte
//          decoding sengledSwitchAction performs (data[5] -> action,
//          data[7] === 2 -> long-press suffix).
//          Until that converter lands the device is registered with no
//          handlers — REST/MQTT will still surface it as a Sengled E1E-G7F.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::sengled {
namespace {
constexpr const char* kModels_E1E_G7F[] = { "E1E-G7F" };

constexpr Expose kExposes_E1E_G7F[] = {
    { "action", ::zhc::ExposeType::Enum, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
}  // namespace

extern const PreparedDefinition kDef_E1E_G7F{
    .zigbee_models=kModels_E1E_G7F, .zigbee_models_count=sizeof(kModels_E1E_G7F)/sizeof(kModels_E1E_G7F[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E1E-G7F", .vendor="Sengled",
    .meta=nullptr, .exposes=kExposes_E1E_G7F, .exposes_count=sizeof(kExposes_E1E_G7F)/sizeof(kExposes_E1E_G7F[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::sengled
