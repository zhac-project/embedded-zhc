// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Owon PB206 panic button (z2m v26.115.1 window).
// z2m m.iasZoneAlarm({zoneType: sos, zoneAttributes: [alarm_1, battery_low]}):
// `sos` is zone-status bit 0 (kFzIasSosAlarm), + m.forcePowerSource(Battery).
// z2m-source: owon.ts #PB206.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasSosAlarm,
};
constexpr Expose kExp[] = {
    {"sos", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "PB206" };
}  // namespace

extern const PreparedDefinition kDef_PB206{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="PB206", .vendor="Owon",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .power_source_override=0x03,
};

}  // namespace zhc::devices::owon
