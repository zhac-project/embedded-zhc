// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Philips 929003535301 Hue Festavia gradient light string, graduated
// from four generated copies. z2m v26.115.1 folded 9290036744 (LCX015),
// 9290036745 (LCX016) and 929003674601 (LCX017) into this definition and
// lists the newer LXC015 / LXC016 / LXC017 model ids as their white labels.
// Same surface as before: the shared Philips colour + CT light bundle (the
// gradient segments are not ported).
// z2m-source: philips.ts #929003535301.
#include "definitions/_generic/_shared.hpp"
#include "definitions/philips/_shared.hpp"

namespace zhc::devices::philips {
namespace {

constexpr const char* kModels[] = {
    "LCX012", "LCX015", "LCX016", "LCX017", "LXC015", "LXC016", "LXC017",
};
constexpr WhiteLabel kWL[] = {
    {"Philips", "9290036744"},
    {"Philips", "9290036745"},
    {"Philips", "929003674601"},
};

}  // namespace

extern const PreparedDefinition kDef_D929003535301_festavia{
    .zigbee_models           = kModels,
    .zigbee_models_count     = sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix= nullptr,
    .manufacturer_names      = nullptr,
    .manufacturer_names_count= 0,
    .model                   = "929003535301",
    .vendor                  = "Philips",
    .meta                    = nullptr,
    .exposes                 = kExposesPhilipsColorCTLight,
    .exposes_count           = kExposesPhilipsColorCTLightCount,
    .white_labels            = kWL,
    .white_labels_count      = sizeof(kWL)/sizeof(kWL[0]),
    .from_zigbee             = kFzPhilipsColorCTLight,
    .from_zigbee_count       = kFzPhilipsColorCTLightCount,
    .to_zigbee               = kTzPhilipsColorCTLight,
    .to_zigbee_count         = kTzPhilipsColorCTLightCount,
    .configure               = nullptr,
    .on_event                = nullptr,
    .bindings                = kBindingsPhilipsCTLight,
    .bindings_count          = kBindingsPhilipsCTLightCount,
};

}  // namespace zhc::devices::philips
