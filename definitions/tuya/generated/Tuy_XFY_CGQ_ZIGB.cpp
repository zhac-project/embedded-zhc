// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya XFY-CGQ-ZIGB — auto-generated.
// Illuminance sensor
// z2m-source: tuya.ts #XFY-CGQ-ZIGB.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_XFY_CGQ_ZIGB[] = { "TS0601" };
constexpr const char* kManus_XFY_CGQ_ZIGB[] = { "_TZE200_yi4jtqq1", "_TZE200_khx7nnka", "_TZE204_khx7nnka" };
}  // namespace

namespace {
const FzConverter* const kFz_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_XFY_CGQ_ZIGB{
    .zigbee_models=kModels_XFY_CGQ_ZIGB, .zigbee_models_count=sizeof(kModels_XFY_CGQ_ZIGB)/sizeof(kModels_XFY_CGQ_ZIGB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_XFY_CGQ_ZIGB, .manufacturer_names_count=sizeof(kManus_XFY_CGQ_ZIGB)/sizeof(kManus_XFY_CGQ_ZIGB[0]),
    .model="XFY-CGQ-ZIGB", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB, .exposes_count=sizeof(kExp_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB)/sizeof(kExp_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB, .from_zigbee_count=sizeof(kFz_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB)/sizeof(kFz_min_XFY_CGQ_ZIGB_Tuy_XFY_CGQ_ZIGB[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
