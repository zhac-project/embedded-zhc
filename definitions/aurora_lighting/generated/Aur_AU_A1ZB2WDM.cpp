// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: AuroraLighting AU-A1ZB2WDM — plain m.light + custom backlight_led.
// AOne 250W smart rotary dimmer module
// z2m-source: aurora_lighting.ts #AU-A1ZB2WDM
//
// Uses the shared plain-light bundle for state/brightness; in addition
// z2m exposes a vendor-specific `backlight_led` boolean (cluster 0x0008
// attribute 0x00f3, manuSpec 0x113c). The custom Tz `tzLocal.aOneBacklight`
// is not yet ported — generic kFz/kTzOnOff handle the on/off, and the
// expose row is kept here so the SPA shows the toggle. Wiring the manuSpec
// write-attribute path is a follow-up TODO.
#include "definitions/aurora_lighting/_shared.hpp"

namespace zhc::devices::aurora_lighting {
namespace {
constexpr const char* kModels_AU_A1ZB2WDM[] = { "WallDimmerMaster" };

constexpr ::zhc::Expose kExp_AU_A1ZB2WDM[] = {
    { "state",         ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness",    ::zhc::ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "backlight_led", ::zhc::ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
};
}  // namespace

extern const PreparedDefinition kDef_AU_A1ZB2WDM{
    .zigbee_models=kModels_AU_A1ZB2WDM,
    .zigbee_models_count=sizeof(kModels_AU_A1ZB2WDM)/sizeof(kModels_AU_A1ZB2WDM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AU-A1ZB2WDM", .vendor="AuroraLighting",
    .meta=nullptr,
    .exposes=kExp_AU_A1ZB2WDM, .exposes_count=sizeof(kExp_AU_A1ZB2WDM)/sizeof(kExp_AU_A1ZB2WDM[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzAuroraLight, .from_zigbee_count=kFzAuroraLightCount,
    .to_zigbee=kTzAuroraLight,   .to_zigbee_count=kTzAuroraLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsAuroraLight, .bindings_count=kBindingsAuroraLightCount,
};

}  // namespace zhc::devices::aurora_lighting
