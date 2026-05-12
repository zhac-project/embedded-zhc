// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Aubess ZXZIR-02 — auto-generated.
// Universal smart IR remote control
// z2m-source: aubess.ts #ZXZIR-02.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aubess {
namespace {


constexpr const char* kModels_ZXZIR_02[] = { "TS1201" };
constexpr const char* kManus_ZXZIR_02[] = { "_TZ3290_acv1iuslxi3shaaj" };
}  // namespace


namespace {
const FzConverter* const kFz_min_ZXZIR_02[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZXZIR_02[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZXZIR_02[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_ZXZIR_02{
    .zigbee_models=kModels_ZXZIR_02, .zigbee_models_count=sizeof(kModels_ZXZIR_02)/sizeof(kModels_ZXZIR_02[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZXZIR_02, .manufacturer_names_count=sizeof(kManus_ZXZIR_02)/sizeof(kManus_ZXZIR_02[0]),
    .model="ZXZIR-02", .vendor="Aubess",
    .meta=nullptr, .exposes=kExp_min_ZXZIR_02, .exposes_count=sizeof(kExp_min_ZXZIR_02)/sizeof(kExp_min_ZXZIR_02[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZXZIR_02, .from_zigbee_count=sizeof(kFz_min_ZXZIR_02)/sizeof(kFz_min_ZXZIR_02[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_ZXZIR_02,
    .bindings_count           = sizeof(kBind_min_ZXZIR_02)/sizeof(kBind_min_ZXZIR_02[0]),
};

}  // namespace zhc::devices::aubess
