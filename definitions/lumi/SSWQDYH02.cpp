// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara SSWQDYH02 Spotlight V1 (z2m v26.92.0 parity, added v26.81.0).
// z2m-source: lumi.ts #SSWQDYH02 (lumi.light.acn036)
//
// Colour-temperature spotlight on standard ZCL: genOnOff + genLevelCtrl +
// lightingColorCtrl, via factory::LumiLight. Upstream colorTempRange is
// [166, 370] mireds.
//
// DEFERRED (recorded, not overlooked): the two manuSpecificLumi config
// numerics `min_brightness` (attr 0x0515) and `max_brightness` (attr 0x0516),
// both u8 and both requiring the Aqara manufacturer code on the read/write.
// They are settings, not state — the light itself is fully covered. Wiring
// them needs a manufacturer-code-carrying ZclWriteSpec, which is the same
// gap the other 0xFCC0 lumi config features are waiting on.
//
// Upstream folds three white labels into this definition: ZNSD01LM /
// ZNSD02LM / ZNSD03LM.
#include "definitions/lumi/_shared.hpp"
#include "definitions/lumi/factories.hpp"

namespace zhc::devices::lumi {
namespace {

using FX = ::zhc::lumi::factory::LumiLight<
    ::zhc::lumi::factory::LumiLightOpts{
        .level      = true,
        .color_temp = true,
    }
>;

constexpr const char* kZigbeeModels[] = { "lumi.light.acn036" };

constexpr Expose kExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, "Light state",         nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, "Brightness",          nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", "Colour temperature",  nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0000},
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

constexpr WhiteLabel kWhiteLabels[] = {
    {"Aqara", "ZNSD01LM"},
    {"Aqara", "ZNSD02LM"},
    {"Aqara", "ZNSD03LM"},
};

}  // namespace

extern const PreparedDefinition kDef_SSWQDYH02{
    .zigbee_models=kZigbeeModels,.zigbee_models_count=sizeof(kZigbeeModels)/sizeof(kZigbeeModels[0]),
    .model               = "SSWQDYH02",
    .vendor              = "Aqara",
    .meta                = nullptr,
    .exposes             = kExposes,
    .exposes_count       = sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels        = kWhiteLabels,
    .white_labels_count  = sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee         = FX::fz_list,
    .from_zigbee_count   = FX::fz_count,
    .to_zigbee           = FX::tz_list,
    .to_zigbee_count     = FX::tz_count,
    .configure           = nullptr,
    .on_event            = nullptr,
    .bindings            = kBindings,
    .bindings_count      = sizeof(kBindings)/sizeof(kBindings[0]),
};

}  // namespace zhc::devices::lumi
