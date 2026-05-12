// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus NLG-RGBW_light — hand-rewritten to use shared ColorCTLight bundle.
// Various RGBW lights (e.g. 100.110.39)
// z2m-source: paul_neuhaus.ts #NLG-RGBW_light.
// z2m extend: m.light({colorTemp: {range: undefined}, color: true})
// z2m endpoint: () => ({default: 2})  — multi-endpoint default 2.
//
// PARTIAL: shared bundle binds endpoint 1 by default. The default-2
// override is not yet expressed via PreparedDefinition (no
// `default_endpoint` field on the runtime). Brightness/color/CT will
// still decode on EP2 because the generic Fz/Tz converters don't
// hard-code an EP, but binding will land on the wrong endpoint until a
// per-port binding override is wired.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
constexpr const char* kModels_NLG_RGBW_light[] = { "NLG-RGBW light " };

// Per-EP binding override: ep=2 instead of the shared bundle's ep=1.
constexpr ::zhc::BindingSpec kBindings_NLG_RGBW_light[] = {
    { 2, 0x0006 }, { 2, 0x0008 }, { 2, 0x0300 },
};
}  // namespace

extern const PreparedDefinition kDef_NLG_RGBW_light{
    .zigbee_models=kModels_NLG_RGBW_light, .zigbee_models_count=sizeof(kModels_NLG_RGBW_light)/sizeof(kModels_NLG_RGBW_light[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NLG-RGBW_light", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposesPaulNeuhausColorCTLight, .exposes_count=kExposesPaulNeuhausColorCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulNeuhausColorCTLight, .from_zigbee_count=kFzPaulNeuhausColorCTLightCount,
    .to_zigbee=kTzPaulNeuhausColorCTLight, .to_zigbee_count=kTzPaulNeuhausColorCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_NLG_RGBW_light, .bindings_count=sizeof(kBindings_NLG_RGBW_light)/sizeof(kBindings_NLG_RGBW_light[0]),
    .default_endpoint = 2,
};

}  // namespace zhc::devices::paul_neuhaus
