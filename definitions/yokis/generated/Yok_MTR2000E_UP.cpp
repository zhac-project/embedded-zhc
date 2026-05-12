// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Yokis MTR2000E-UP — parity-extended 2026-04-28.
// Remote power switch with timer 2000W (mains, no battery).
// z2m-source: yokis.ts #MTR2000E-UP.
//
// z2m extend mirrors MTR500E-UP. Runtime gap: same manuSpec clusters.
// Note: previous port mistakenly carried genPowerCfg/battery — removed.
// TODO 2026-04-28: wire manuSpecificYokis* converters from
//   definitions/yokis/_shared.hpp (kFzYokis{Input,Entry,Channel,LightControl,
//   Dimmer,SubSystem,PilotWire} + matching kTz*) — see Yok_MTR500E_UP.cpp /
//   Yok_TLC2_UP.cpp / Yok_MFP_UP.cpp for canonical wiring patterns.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::yokis {
namespace {
const FzConverter* const kFz_MTR2000E_UP[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_MTR2000E_UP[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_MTR2000E_UP[] = { "MTR2000E-UP" };

constexpr Expose kExp_MTR2000E_UP[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr BindingSpec kBind_MTR2000E_UP[] = {
    {1, 0x0006},
};
}  // namespace

extern const PreparedDefinition kDef_MTR2000E_UP{
    .zigbee_models=kModels_MTR2000E_UP, .zigbee_models_count=sizeof(kModels_MTR2000E_UP)/sizeof(kModels_MTR2000E_UP[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MTR2000E-UP", .vendor="Yokis",
    .meta=nullptr, .exposes=kExp_MTR2000E_UP, .exposes_count=sizeof(kExp_MTR2000E_UP)/sizeof(kExp_MTR2000E_UP[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MTR2000E_UP, .from_zigbee_count=sizeof(kFz_MTR2000E_UP)/sizeof(kFz_MTR2000E_UP[0]),
    .to_zigbee=kTz_MTR2000E_UP, .to_zigbee_count=sizeof(kTz_MTR2000E_UP)/sizeof(kTz_MTR2000E_UP[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_MTR2000E_UP, .bindings_count=sizeof(kBind_MTR2000E_UP)/sizeof(kBind_MTR2000E_UP[0]),
};

}  // namespace zhc::devices::yokis
