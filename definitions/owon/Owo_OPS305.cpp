// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Owon OPS305 ceiling-mounted presence sensor (z2m v26.115.1 window).
// z2m m.occupancy(): msOccupancySensing occupancy, reported up to hourly.
// z2m-source: owon.ts #OPS305.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOccupancy,
};
constexpr Expose kExp[] = {
    {"occupancy", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "OCP305", "OCP_305" };
constexpr BindingSpec kBind[] = {
    {1, 0x0406},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0406, 0x0000, 0x18, 0, 3600, 0, 0},
};
}  // namespace

extern const PreparedDefinition kDef_OPS305{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="OPS305", .vendor="Owon",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::owon
