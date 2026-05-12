// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Profalux MAI-ZTM20C — newer cover remote on closuresWindow-
// Covering open/close/stop, with battery on EP2. z2m emits action up/down/
// stop. Removed bogus "MAI-ZTS" alias (collided with NB102) and switched
// fz=on/off → fz=command_cover_{open,close,stop}.
// z2m-source: profalux.ts #MAI-ZTM20C.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::profalux {
namespace {
const FzConverter* const kFz_MAI_ZTM20C[] = {
    &::zhc::generic::kFzCommandCoverOpen,
    &::zhc::generic::kFzCommandCoverClose,
    &::zhc::generic::kFzCommandCoverStop,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_MAI_ZTM20C[] = { "MAI-ZTM20C" };

constexpr const char* kActionEnum[] = { "up", "down", "stop" };
constexpr Expose kExp_MAI_ZTM20C[] = {
    {"action",  ExposeType::Enum,    ::zhc::Access::State, nullptr, nullptr, kActionEnum, 3},
    {"battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0},
};

// z2m's configure: bind genLevelCtrl on EP1, then (genPowerCfg,
// closuresWindowCovering) on EP2 — battery and cover commands originate
// from EP2.
constexpr BindingSpec kBind_MAI_ZTM20C[] = {
    {1, 0x0008},   // genLevelCtrl on EP1
    {2, 0x0001},   // genPowerCfg  on EP2
    {2, 0x0102},   // closuresWindowCovering on EP2
};
}  // namespace

extern const PreparedDefinition kDef_MAI_ZTM20C{
    .zigbee_models=kModels_MAI_ZTM20C, .zigbee_models_count=sizeof(kModels_MAI_ZTM20C)/sizeof(kModels_MAI_ZTM20C[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MAI-ZTM20C", .vendor="Profalux",
    .meta=nullptr, .exposes=kExp_MAI_ZTM20C, .exposes_count=sizeof(kExp_MAI_ZTM20C)/sizeof(kExp_MAI_ZTM20C[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MAI_ZTM20C, .from_zigbee_count=sizeof(kFz_MAI_ZTM20C)/sizeof(kFz_MAI_ZTM20C[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_MAI_ZTM20C,
    .bindings_count=sizeof(kBind_MAI_ZTM20C)/sizeof(kBind_MAI_ZTM20C[0]),
};

}  // namespace zhc::devices::profalux
