// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yokis MTR1300E-UP — parity-extended 2026-04-28.
// Remote power switch with timer 1300W (mains, no battery).
// z2m-source: yokis.ts #MTR1300E-UP. White-label: MTR1300EB-UP (screw terminals).
//
// z2m extend mirrors MTR500E-UP. Runtime gap: same manuSpec clusters.
// Note: previous port mistakenly carried genPowerCfg/battery — removed (z2m has none).
// TODO 2026-04-28: wire manuSpecificYokis* converters from
//   definitions/yokis/_shared.hpp (kFzYokis{Input,Entry,Channel,LightControl,
//   Dimmer,SubSystem,PilotWire} + matching kTz*) — see Yok_MTR500E_UP.cpp /
//   Yok_TLC2_UP.cpp / Yok_MFP_UP.cpp for canonical wiring patterns.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yokis {
namespace {
const FzConverter* const kFz_MTR1300E_UP[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_MTR1300E_UP[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_MTR1300E_UP[] = { "MTR1300E-UP", "MTR1300EB-UP" };

constexpr Expose kExp_MTR1300E_UP[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_MTR1300E_UP[] = {
    {1, 0x0006},
};

constexpr WhiteLabel kWhiteLabels_MTR1300E_UP[] = {
    {"YOKIS", "MTR1300EB-UP"},
};
}  // namespace

extern const PreparedDefinition kDef_MTR1300E_UP{
    .zigbee_models=kModels_MTR1300E_UP, .zigbee_models_count=sizeof(kModels_MTR1300E_UP)/sizeof(kModels_MTR1300E_UP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MTR1300E-UP", .vendor="Yokis",
    .meta=nullptr, .exposes=kExp_MTR1300E_UP, .exposes_count=sizeof(kExp_MTR1300E_UP)/sizeof(kExp_MTR1300E_UP[0]),
    .white_labels=kWhiteLabels_MTR1300E_UP, .white_labels_count=sizeof(kWhiteLabels_MTR1300E_UP)/sizeof(kWhiteLabels_MTR1300E_UP[0]),
    .from_zigbee=kFz_MTR1300E_UP, .from_zigbee_count=sizeof(kFz_MTR1300E_UP)/sizeof(kFz_MTR1300E_UP[0]),
    .to_zigbee=kTz_MTR1300E_UP, .to_zigbee_count=sizeof(kTz_MTR1300E_UP)/sizeof(kTz_MTR1300E_UP[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_MTR1300E_UP, .bindings_count=sizeof(kBind_MTR1300E_UP)/sizeof(kBind_MTR1300E_UP[0]),
};

}  // namespace zhc::devices::yokis
