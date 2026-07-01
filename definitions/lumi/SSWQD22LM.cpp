// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Aqara SSWQD22LM Spotlight T2 Pro dimmable CCT light (z2m v26.76.0 parity).
// z2m-source: lumi.ts #SSWQD22LM
//
// z2m: lumiLight({colorTemp:true, colorTempRange:[166,370]}) on lumi.light.acn040
// plus addManuSpecificLumiCluster(). Ported via factory::LumiLight{level,
// color_temp} — wires on/off + brightness (genOnOff 0x0006 / genLevelCtrl 0x0008)
// and the color-temp encoder/decoder on the standard ZCL clusters. Exposes
// state + brightness (mirrors sibling LGZCDD01LM's expose block).
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
constexpr const char* kZigbeeModels[] = { "lumi.light.acn040" };
}  // namespace

// --- expose block mirrors LGZCDD01LM (LumiLight{level,color_temp}) ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State, "C", nullptr, nullptr, 0},
    {"power_outage_count", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0000},  // genBasic
    {1, 0x0006},  // genOnOff
    {1, 0x0008},  // genLevelCtrl
};

extern const PreparedDefinition kDefSSWQD22LM{
    .zigbee_models=kZigbeeModels,.zigbee_models_count=sizeof(kZigbeeModels)/sizeof(kZigbeeModels[0]),
    .model="SSWQD22LM",.vendor="Xiaomi",
    .meta=nullptr,
    .exposes=kAutoExposes,.exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=FX::fz_list,.from_zigbee_count=FX::fz_count,
    .to_zigbee=FX::tz_list,.to_zigbee_count=FX::tz_count,
    .configure=nullptr,.on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};
}  // namespace zhc::devices::lumi
