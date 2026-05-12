// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Adurosmart 81809/81813 — hand-rewritten to use Adu_ColorCTLight bundle.
// ERIA colors and white shades smart light bulb A19/BR30.
// z2m extend: m.light({colorTemp: {range: undefined}, color: {applyRedFix: true}}).
// NOTE: z2m sets endpoint default=2 for outbound traffic; ZHC has no per-port default-endpoint field, so outbound routing relies on firmware/runtime fall-back. See ADUROSMART_PARITY.md.
// z2m-source: adurosmart.ts #81809/81813.
#include "definitions/_generic/_shared.hpp"
#include "definitions/adurosmart/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81809_81813[] = { "ZLL-ExtendedColo", "ZLL-ExtendedColor" };

}  // namespace

extern const PreparedDefinition kDef_D81809_81813{
    .zigbee_models=kModels_D81809_81813,
    .zigbee_models_count=sizeof(kModels_D81809_81813)/sizeof(kModels_D81809_81813[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="81809/81813", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kExposesAduColorCTLight, .exposes_count=kExposesAduColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAduColorCTLight, .from_zigbee_count=kFzAduColorCTLightCount,
    .to_zigbee=kTzAduColorCTLight, .to_zigbee_count=kTzAduColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAduColorCTLight, .bindings_count=kBindingsAduColorCTLightCount,
    .default_endpoint = 2,
};

}  // namespace zhc::devices::adurosmart
