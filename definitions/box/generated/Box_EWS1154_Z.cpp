// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// NEUTERED 2026-04-28p — superseded by Box__TZE200_yp5tsi3y.cpp (full Tuya-DP port).
// This minimal stub had the same (TS0601, _TZE200_yp5tsi3y) fingerprint as the full port,
// causing duplicate-match conflicts. Neutered per Recipe B (zigbeeModel=__NEUTERED_*,
// manufacturer_names cleared). Kept for git history; not reachable by the matcher.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::box {
namespace {


constexpr const char* kModels_EWS1154_Z[] = { "__NEUTERED__TZE200_yp5tsi3y" };
}  // namespace


namespace {
const FzConverter* const kFz_min_EWS1154_Z[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_EWS1154_Z[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_EWS1154_Z[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_EWS1154_Z{
    .zigbee_models=kModels_EWS1154_Z, .zigbee_models_count=sizeof(kModels_EWS1154_Z)/sizeof(kModels_EWS1154_Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="EWS1154-Z", .vendor="BOX",
    .meta=nullptr, .exposes=kExp_min_EWS1154_Z, .exposes_count=sizeof(kExp_min_EWS1154_Z)/sizeof(kExp_min_EWS1154_Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_EWS1154_Z, .from_zigbee_count=sizeof(kFz_min_EWS1154_Z)/sizeof(kFz_min_EWS1154_Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_EWS1154_Z,
    .bindings_count           = sizeof(kBind_min_EWS1154_Z)/sizeof(kBind_min_EWS1154_Z[0]),
};

}  // namespace zhc::devices::box
