// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Eglo 901471 (z2m v26.76.0 parity).
// z2m-source: eglo.ts #901471
//
// EGLO ROVITO-Z ceiling light (EBF-RGB-ZMB-CLB): a CCT panel on EP1 and an
// RGB ring on EP3. z2m v26.125.0 splits it with m.deviceEndpoints({white: 1,
// rgb: 3}) and two m.light()s: colour temperature (153-370) on "white",
// xy/hs colour on "rgb". The endpoint map suffixes keys `_white` / `_rgb` and
// routes writes to the matching endpoint.
// Deferred: m.commandsOnOff/LevelCtrl/ColorCtrl (`action` from the device's
// controller endpoint), as on the 300686 sibling.
#include "definitions/eglo/_shared.hpp"

namespace zhc::devices::eglo {
namespace {
constexpr const char* kModels_D901471[] = { "EBF-RGB-ZMB-CLB" };
constexpr ::zhc::EndpointLabel kEndpoints_D901471[] = { {"white", 1}, {"rgb", 3} };

constexpr Expose kExposes_D901471[] = {
    {"state_white",      ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_white", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp_white", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0,
     ExposeCategory::State, 153, 370, 1},
    {"state_rgb",        ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness_rgb",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_x_rgb",      ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_y_rgb",      ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"hue_rgb",          ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"saturation_rgb",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_D901471[] = {
    {1, 0x0006}, {1, 0x0008}, {1, 0x0300},
    {3, 0x0006}, {3, 0x0008}, {3, 0x0300},
};
}  // namespace

extern const PreparedDefinition kDef_D901471{
    .zigbee_models=kModels_D901471, .zigbee_models_count=sizeof(kModels_D901471)/sizeof(kModels_D901471[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="901471", .vendor="Eglo",
    .meta=nullptr, .exposes=kExposes_D901471, .exposes_count=sizeof(kExposes_D901471)/sizeof(kExposes_D901471[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzEgloColorCTLight, .from_zigbee_count=kFzEgloColorCTLightCount,
    .to_zigbee=kTzEgloColorCTLight, .to_zigbee_count=kTzEgloColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D901471, .bindings_count=sizeof(kBindings_D901471)/sizeof(kBindings_D901471[0]),
    .endpoint_map       = kEndpoints_D901471,
    .endpoint_map_count = sizeof(kEndpoints_D901471)/sizeof(kEndpoints_D901471[0]),
};

}  // namespace zhc::devices::eglo
