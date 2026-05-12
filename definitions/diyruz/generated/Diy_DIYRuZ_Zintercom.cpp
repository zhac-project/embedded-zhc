// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Diyruz DIYRuZ_Zintercom — hand-extended 2026-04-28.
// Matrix intercom auto opener. Uses closuresDoorLock (0x0101) for state
// + manuSpec attrs decoded by fz.diyruz_zintercom_config / written via
// tz.diyruz_zintercom_config.
// z2m-source: diyruz.ts #DIYRuZ_Zintercom.
// PARTIAL: battery wired via generic. fz/tz.diyruz_zintercom_config are
// BLOCKED (custom cluster). Exposes declared so the UI mirrors z2m;
// runtime reports drop until a diyruz/_shared.cpp lands.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::diyruz {
namespace {
const FzConverter* const kFz_DIYRuZ_Zintercom[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_DIYRuZ_Zintercom[] = { "DIY_Zintercom" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",   ExposeType::Numeric, Access::State,    "%",   nullptr, nullptr, 0},
    {"voltage",   ExposeType::Numeric, Access::State,    "mV",  nullptr, nullptr, 0},
    {"state",     ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"mode",      ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sound",     ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"time_ring", ExposeType::Numeric, Access::StateSet, "sec", nullptr, nullptr, 0},
    {"time_talk", ExposeType::Numeric, Access::StateSet, "sec", nullptr, nullptr, 0},
    {"time_open", ExposeType::Numeric, Access::StateSet, "sec", nullptr, nullptr, 0},
    {"time_bell", ExposeType::Numeric, Access::StateSet, "sec", nullptr, nullptr, 0},
    {"time_mode", ExposeType::Numeric, Access::StateSet, "sec", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg
    {1, 0x0101},  // closuresDoorLock
};

extern const PreparedDefinition kDef_DIYRuZ_Zintercom{
    .zigbee_models=kModels_DIYRuZ_Zintercom, .zigbee_models_count=sizeof(kModels_DIYRuZ_Zintercom)/sizeof(kModels_DIYRuZ_Zintercom[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_Zintercom", .vendor="Diyruz",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIYRuZ_Zintercom, .from_zigbee_count=sizeof(kFz_DIYRuZ_Zintercom)/sizeof(kFz_DIYRuZ_Zintercom[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::diyruz
