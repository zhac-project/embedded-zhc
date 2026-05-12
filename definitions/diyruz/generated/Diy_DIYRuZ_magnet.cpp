// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Diyruz DIYRuZ_magnet — hand-rewritten 2026-04-28.
// DIYRuZ contact sensor.
// z2m-source: diyruz.ts #DIYRuZ_magnet.
// PARTIAL: battery wired via generic. fz.diyruz_contact is a custom
// attribute on a non-IAS cluster (genOnOff manu) and BLOCKED — the prior
// kFzIasZone wiring was a wrong-bundle assumption (z2m does NOT use IAS
// here). Contact expose is declared so the UI shows the device shape;
// runtime updates would need a diyruz/_shared.cpp converter.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::diyruz {
namespace {
const FzConverter* const kFz_DIYRuZ_magnet[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_DIYRuZ_magnet[] = { "DIYRuZ_magnet" };

constexpr Expose kExp_DIYRuZ_magnet[] = {
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"contact", ExposeType::Binary,  Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBind_DIYRuZ_magnet[] = {
    {1, 0x0001},  // genPowerCfg
};

}  // namespace

extern const PreparedDefinition kDef_DIYRuZ_magnet{
    .zigbee_models=kModels_DIYRuZ_magnet, .zigbee_models_count=sizeof(kModels_DIYRuZ_magnet)/sizeof(kModels_DIYRuZ_magnet[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_magnet", .vendor="Diyruz",
    .meta=nullptr, .exposes=kExp_DIYRuZ_magnet, .exposes_count=sizeof(kExp_DIYRuZ_magnet)/sizeof(kExp_DIYRuZ_magnet[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIYRuZ_magnet, .from_zigbee_count=sizeof(kFz_DIYRuZ_magnet)/sizeof(kFz_DIYRuZ_magnet[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_DIYRuZ_magnet,
    .bindings_count=sizeof(kBind_DIYRuZ_magnet)/sizeof(kBind_DIYRuZ_magnet[0]),
};

}  // namespace zhc::devices::diyruz
