// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: PaulNeuhaus NLG-TW light — hand-rewritten to use shared CTLight bundle.
// Various tunable white lights (e.g. 8195-55)
// z2m-source: paul_neuhaus.ts #NLG-TW light.
// z2m extend: m.light({colorTemp: {range: [153, 370]}})
// Note: shared CTLight expose row carries no per-device range. The
// 153/370 mired clamp lives on z2m's render layer; ZHC uses the device's
// reported color_temp_min/max instead.
#include "definitions/_generic/_shared.hpp"
#include "definitions/paul_neuhaus/_shared.hpp"

namespace zhc::devices::paul_neuhaus {
namespace {
constexpr const char* kModels_NLG_TW_light[] = { "Neuhaus NLG-TW light", "NLG-TW light" };
}  // namespace

extern const PreparedDefinition kDef_NLG_TW_light{
    .zigbee_models=kModels_NLG_TW_light, .zigbee_models_count=sizeof(kModels_NLG_TW_light)/sizeof(kModels_NLG_TW_light[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="NLG-TW light", .vendor="PaulNeuhaus",
    .meta=nullptr, .exposes=kExposesPaulNeuhausCTLight, .exposes_count=kExposesPaulNeuhausCTLightCount,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPaulNeuhausCTLight, .from_zigbee_count=kFzPaulNeuhausCTLightCount,
    .to_zigbee=kTzPaulNeuhausCTLight, .to_zigbee_count=kTzPaulNeuhausCTLightCount,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindingsPaulNeuhausCTLight, .bindings_count=kBindingsPaulNeuhausCTLightCount,
};

}  // namespace zhc::devices::paul_neuhaus
