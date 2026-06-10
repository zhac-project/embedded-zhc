// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Hornbach 10297665 — graduated to shared CTLight bundle (added color_temp / 0x0300).
// FLAIR Viyu smart LED bulb CCT E27 filament
// The auto-generated def wired only on/off + brightness and dropped the
// lightingColorCtrl (0x0300) axis that z2m's m.light(...) exposes.
// z2m-source: hornbach.ts #10297665.
#include "definitions/_generic/_shared.hpp"
#include "definitions/hornbach/_shared.hpp"

namespace zhc::devices::hornbach {
namespace {
constexpr const char* kModels_D10297665[] = { "VIYU_A60_470_FI_D_CCT_10297665" };
}  // namespace

extern const PreparedDefinition kDef_D10297665{
    .zigbee_models=kModels_D10297665, .zigbee_models_count=sizeof(kModels_D10297665)/sizeof(kModels_D10297665[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="10297665", .vendor="Hornbach",
    .meta=nullptr, .exposes=kExposesHornbachCTLight, .exposes_count=kExposesHornbachCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzHornbachCTLight, .from_zigbee_count=kFzHornbachCTLightCount,
    .to_zigbee=kTzHornbachCTLight, .to_zigbee_count=kTzHornbachCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsHornbachCTLight, .bindings_count=kBindingsHornbachCTLightCount,
};

}  // namespace zhc::devices::hornbach
