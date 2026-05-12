// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya GDC311ZBQ1 — auto-generated.
// LoraTap garage door opener with wireless sensor
// z2m-source: tuya.ts #GDC311ZBQ1.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_GDC311ZBQ1[] = { "TS0601" };
constexpr const char* kManus_GDC311ZBQ1[] = { "_TZE200_wfxuhoea", "_TZE204_wfxuhoea" };
}  // namespace

namespace {
const FzConverter* const kFz_min_GDC311ZBQ1_Tuy_GDC311ZBQ1[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_GDC311ZBQ1_Tuy_GDC311ZBQ1[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_GDC311ZBQ1_Tuy_GDC311ZBQ1[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_GDC311ZBQ1{
    .zigbee_models=kModels_GDC311ZBQ1, .zigbee_models_count=sizeof(kModels_GDC311ZBQ1)/sizeof(kModels_GDC311ZBQ1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_GDC311ZBQ1, .manufacturer_names_count=sizeof(kManus_GDC311ZBQ1)/sizeof(kManus_GDC311ZBQ1[0]),
    .model="GDC311ZBQ1", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_GDC311ZBQ1_Tuy_GDC311ZBQ1, .exposes_count=sizeof(kExp_min_GDC311ZBQ1_Tuy_GDC311ZBQ1)/sizeof(kExp_min_GDC311ZBQ1_Tuy_GDC311ZBQ1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_GDC311ZBQ1_Tuy_GDC311ZBQ1, .from_zigbee_count=sizeof(kFz_min_GDC311ZBQ1_Tuy_GDC311ZBQ1)/sizeof(kFz_min_GDC311ZBQ1_Tuy_GDC311ZBQ1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
