// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Inovelli VZM36 — uses shared inovelli converters.
// Fan canopy module (light on EP1, fan on EP2)
// z2m-source: inovelli.ts #VZM36.
#include "definitions/_generic/_shared.hpp"
#include "definitions/inovelli/_shared.hpp"

namespace zhc::devices::inovelli {
namespace {
const FzConverter* const kFz_VZM36[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
};
const TzConverter* const kTz_VZM36[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    // Inovelli manuSpec writeable parameters. NOTE: VZM36 uses a
    // *_1 / *_2 suffix convention to disambiguate the light (EP1)
    // and fan (EP2) endpoints. Until ZHC implements the multi-EP
    // parameter routing the shared converters target EP1 only —
    // tracked in `docs/parity/INOVELLI_PARITY.md` § VZM36.
    &kTzInoDimmingSpeedUpRemote,
    &kTzInoRampRateOffToOnRemote,
    &kTzInoDimmingSpeedDownRemote,
    &kTzInoRampRateOnToOffRemote,
    &kTzInoAutoTimerOff,
    &kTzInoDefaultLevelRemote,
    &kTzInoStateAfterPowerRestored,
    &kTzInoMinimumLevel,
    &kTzInoMaximumLevel,
    &kTzInoSmartBulbMode,
};
constexpr const char* kModels_VZM36[] = { "VZM36" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // VZM36 light/fan parameter exposes (EP1-routed subset).
    {"dimmingSpeedUpRemote",    ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rampRateOffToOnRemote",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"dimmingSpeedDownRemote",  ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"rampRateOnToOffRemote",   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"autoTimerOff",            ExposeType::Numeric, Access::StateSet, "s", nullptr, nullptr, 0},
    {"defaultLevelRemote",      ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"stateAfterPowerRestored", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"minimumLevel",            ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"maximumLevel",            ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"smartBulbMode",           ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // light on/off (EP1)
    {1, 0x0008},  // light dim   (EP1)
    {2, 0x0006},  // fan  on/off (EP2)
    {2, 0x0008},  // fan  speed  (EP2)
    {1, 0xFC31},  // manuSpecificInovelli (EP1).
};

extern const PreparedDefinition kDef_VZM36{
    .zigbee_models=kModels_VZM36, .zigbee_models_count=sizeof(kModels_VZM36)/sizeof(kModels_VZM36[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VZM36", .vendor="Inovelli",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_VZM36, .from_zigbee_count=sizeof(kFz_VZM36)/sizeof(kFz_VZM36[0]),
    .to_zigbee=kTz_VZM36, .to_zigbee_count=sizeof(kTz_VZM36)/sizeof(kTz_VZM36[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::inovelli
