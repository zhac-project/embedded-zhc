// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aqara ZNYB01LM Smart bathroom heater T1
// (z2m v26.92.0 parity, added v26.83.0).
// z2m-source: lumi.ts #ZNYB01LM (lumi.bhf_light.acn001)
//
// PARTIAL PORT — the light is covered, the heater is DEFERRED.
//
// Upstream builds this device from `m.light({colorTemp: {range: [153, 370]}})`
// plus `lumiBathroomHeaterT1()`. The light half is ordinary ZCL and is wired
// here via factory::LumiLight.
//
// The heater half is not portable with today's converter set. It is the "YUBA"
// block that lib/lumi.ts grew in this window (+483 lines): a manufacturer-
// specific family on manuSpecificLumi where the whole operating state is
// carried in ONE packed attribute, 0x024f, that upstream decodes through a
// BigInt (`getYubaHeartbeatPackedState` / `serializeYubaBuffer`) into mode,
// fan speed and clock, with four more attributes alongside it:
//
//   0x024f packed state          0x0256 mute prompt tone
//   0x0257 mute prompt time      0x02be constant-temperature mode
//   0x0518 night light
//
// and four interlocking lookups (mode {warm:0, dry:3, fan_only:4, exhaust:5},
// system_mode, fan {low:0, medium:1, high:2}, fan_state {0xfffd, 0xfffe,
// 0xffff}). A ZHC converter maps one attribute to one value; there is no
// bitfield/struct decoder to split a packed 64-bit state, and no way to derive
// system_mode from operating_mode. Emitting a partial or guessed decode of a
// heater's mode is worse than emitting none.
//
// To finish this device: add a packed-attribute decoder to lumi/_shared, then
// wire the five attributes above. Tracked with the wider 0xFCC0 lumi config
// gap.
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

constexpr const char* kZigbeeModels[] = { "lumi.bhf_light.acn001" };

constexpr Expose kExposes[] = {
    {"state",      ExposeType::Binary,  Access::StateSet, nullptr, "Light state",        nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, "Brightness",         nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", "Colour temperature", nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0000},
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
};

}  // namespace

extern const PreparedDefinition kDef_ZNYB01LM{
    .zigbee_models=kZigbeeModels,.zigbee_models_count=sizeof(kZigbeeModels)/sizeof(kZigbeeModels[0]),
    .model               = "ZNYB01LM",
    .vendor              = "Aqara",
    .meta                = nullptr,
    .exposes             = kExposes,
    .exposes_count       = sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels        = nullptr,
    .white_labels_count  = 0,
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
