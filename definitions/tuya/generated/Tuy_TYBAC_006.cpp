// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Tuya TYBAC-006 — auto-generated.
// Wall-mount thermostat for 2-pipe fan-coil unit
// z2m-source: tuya.ts #TYBAC-006.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {


constexpr const char* kModels_TYBAC_006[] = { "TS0601" };
constexpr const char* kManus_TYBAC_006[] = { "_TZE204_mpbki2zm" };
}  // namespace

namespace {
const FzConverter* const kFz_min_TYBAC_006_Tuy_TYBAC_006[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_TYBAC_006_Tuy_TYBAC_006[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_TYBAC_006_Tuy_TYBAC_006[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_TYBAC_006{
    .zigbee_models=kModels_TYBAC_006, .zigbee_models_count=sizeof(kModels_TYBAC_006)/sizeof(kModels_TYBAC_006[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_TYBAC_006, .manufacturer_names_count=sizeof(kManus_TYBAC_006)/sizeof(kManus_TYBAC_006[0]),
    .model="TYBAC-006", .vendor="Tuya",
    .meta=nullptr, .exposes=kExp_min_TYBAC_006_Tuy_TYBAC_006, .exposes_count=sizeof(kExp_min_TYBAC_006_Tuy_TYBAC_006)/sizeof(kExp_min_TYBAC_006_Tuy_TYBAC_006[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_TYBAC_006_Tuy_TYBAC_006, .from_zigbee_count=sizeof(kFz_min_TYBAC_006_Tuy_TYBAC_006)/sizeof(kFz_min_TYBAC_006_Tuy_TYBAC_006[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tuya
