// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Hive SLB2 — hand-tuned.
// Signal booster — z2m: only fz.linkquality_from_basic + a polling extend
// that periodically reads `genBasic.zclVersion`. No on/off cluster, no
// user-visible exposes. Earlier auto-port wrongly listed kFzOnOff and
// 0x0006 binding; corrected here.
// z2m-source: hive.ts #SLB2.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::hive {
namespace {
constexpr const char* kModels_SLB2[] = { "SLB2" };

}  // namespace


constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0000},  // genBasic — for linkquality polling
};

extern const PreparedDefinition kDef_SLB2{
    .zigbee_models=kModels_SLB2, .zigbee_models_count=sizeof(kModels_SLB2)/sizeof(kModels_SLB2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="SLB2", .vendor="Hive",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::hive
