// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Hornbach 10454471 — graduated to shared ColorCTLight bundle (added color + color_temp / 0x0300).
// FLAIR Viyu smart LED bulb RGBW E27
// The auto-generated def wired only on/off + brightness and dropped the
// lightingColorCtrl (0x0300) axis that z2m's m.light(...) exposes.
// z2m-source: hornbach.ts #10454471.
#include "definitions/_generic/_shared.hpp"
#include "definitions/hornbach/_shared.hpp"

namespace zhc::devices::hornbach {
namespace {
constexpr const char* kModels_D10454471[] = { "VIYU_A60_806_RGBW_10454471" };
}  // namespace

extern const PreparedDefinition kDef_D10454471{
    .zigbee_models=kModels_D10454471, .zigbee_models_count=sizeof(kModels_D10454471)/sizeof(kModels_D10454471[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="10454471", .vendor="Hornbach",
    .meta=nullptr, .exposes=kExposesHornbachColorCTLight, .exposes_count=kExposesHornbachColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzHornbachColorCTLight, .from_zigbee_count=kFzHornbachColorCTLightCount,
    .to_zigbee=kTzHornbachColorCTLight, .to_zigbee_count=kTzHornbachColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsHornbachColorCTLight, .bindings_count=kBindingsHornbachColorCTLightCount,
};

}  // namespace zhc::devices::hornbach
