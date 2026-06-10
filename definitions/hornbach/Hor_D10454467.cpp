// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Hornbach 10454467 — graduated to shared ColorCTLight bundle (added color + color_temp / 0x0300).
// FLAIR Viyu smart LED candle RGB E14
// The auto-generated def wired only on/off + brightness and dropped the
// lightingColorCtrl (0x0300) axis that z2m's m.light(...) exposes.
// z2m-source: hornbach.ts #10454467.
#include "definitions/_generic/_shared.hpp"
#include "definitions/hornbach/_shared.hpp"

namespace zhc::devices::hornbach {
namespace {
constexpr const char* kModels_D10454467[] = { "VIYU_C35_470_RGBW_10454467" };
}  // namespace

extern const PreparedDefinition kDef_D10454467{
    .zigbee_models=kModels_D10454467, .zigbee_models_count=sizeof(kModels_D10454467)/sizeof(kModels_D10454467[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="10454467", .vendor="Hornbach",
    .meta=nullptr, .exposes=kExposesHornbachColorCTLight, .exposes_count=kExposesHornbachColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzHornbachColorCTLight, .from_zigbee_count=kFzHornbachColorCTLightCount,
    .to_zigbee=kTzHornbachColorCTLight, .to_zigbee_count=kTzHornbachColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsHornbachColorCTLight, .bindings_count=kBindingsHornbachColorCTLightCount,
};

}  // namespace zhc::devices::hornbach
