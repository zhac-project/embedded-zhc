// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: SMLIGHT ZREL Zigbee-Ethernet router, new in z2m v26.126.0..v26.133.0.
// A router with nothing to read or set. m.linkQuality({reporting: true}):
// genBasic zclVersion (0x0000, u8) reported every 1-4 h so the router keeps
// sending frames (linkquality comes from the radio layer); endpoint 1 genBasic
// bound for it, as setupAttributes does upstream.
// Not ported: identify.
// z2m-source: smlight.ts #ZREL.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smlight {
namespace {

constexpr const char* kModels_ZREL[] = { "ZREL" };

constexpr BindingSpec kBindings_ZREL[] = {
    {1, 0x0000},
};
constexpr ReportingSpec kReports_ZREL[] = {
    {1, 0x0000, 0x0000, 0x20, 3600, 14400, 0, 0},
};

}  // namespace

extern const PreparedDefinition kDef_ZREL{
    .zigbee_models=kModels_ZREL, .zigbee_models_count=sizeof(kModels_ZREL)/sizeof(kModels_ZREL[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZREL", .vendor="Smlight",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZREL, .bindings_count=sizeof(kBindings_ZREL)/sizeof(kBindings_ZREL[0]),
    .reports=kReports_ZREL, .reports_count=sizeof(kReports_ZREL)/sizeof(kReports_ZREL[0]),
};

}  // namespace zhc::devices::smlight
