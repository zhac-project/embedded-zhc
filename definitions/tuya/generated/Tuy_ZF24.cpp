// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya ZF24 — auto-generated.
// Human presence sensor (millimeter wave radar)
// z2m-source: tuya.ts #ZF24.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_ZF24[] = { "TS0601" };
constexpr const char* kManus_ZF24[] = { "_TZE284_pzm3wab5", "_TZE284_twybxdzl", "_TZE284_hgeqeyuv" };
}  // namespace

namespace {
const FzConverter* const kFz_min_ZF24_Tuy_ZF24[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_ZF24_Tuy_ZF24[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_ZF24_Tuy_ZF24[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_ZF24{
    .zigbee_models=kModels_ZF24, .zigbee_models_count=sizeof(kModels_ZF24)/sizeof(kModels_ZF24[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_ZF24, .manufacturer_names_count=sizeof(kManus_ZF24)/sizeof(kManus_ZF24[0]),
    .model="ZF24", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_ZF24_Tuy_ZF24, .exposes_count=sizeof(kExp_min_ZF24_Tuy_ZF24)/sizeof(kExp_min_ZF24_Tuy_ZF24[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_ZF24_Tuy_ZF24, .from_zigbee_count=sizeof(kFz_min_ZF24_Tuy_ZF24)/sizeof(kFz_min_ZF24_Tuy_ZF24[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
