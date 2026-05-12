// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tech VNTH-T2_v2 — neutered.
// The richer DP-mapped port lives at Tec__TZE204_p1qrtljn.cpp; this
// minimal-port stub from generate_simple_vendor_ports.py decoded the
// thermostatic radiator valve as battery + on/off only and would
// shadow the proper port. Header `// Tier 2` keeps the simple
// vendor generator from regenerating it.
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::tech {
namespace {
constexpr const char* kModels_VNTH_T2_v2[] = { "__NEUTERED__TZE204_p1qrtljn" };
}  // namespace

extern const PreparedDefinition kDef_VNTH_T2_v2{
    .zigbee_models=kModels_VNTH_T2_v2,
    .zigbee_models_count=sizeof(kModels_VNTH_T2_v2)/sizeof(kModels_VNTH_T2_v2[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="VNTH-T2_v2", .vendor="Tech",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::tech
