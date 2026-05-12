// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Mazda TR-M2Z — neutered.
// The richer DP-mapped ports live at Maz__TZE204_k6rdmisz.cpp and
// Maz__TZE284_k6rdmisz.cpp; this minimal-port stub from
// generate_simple_vendor_ports.py decoded the thermostatic radiator
// valve as battery + on/off only and would shadow the proper ports
// during find_definition's Pass 1. Header `// Tier 2` keeps the
// simple vendor generator from regenerating it.
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::mazda {
namespace {
constexpr const char* kModels_TR_M2Z[] = { "__NEUTERED_TR_M2Z" };
}  // namespace

extern const PreparedDefinition kDef_TR_M2Z{
    .zigbee_models=kModels_TR_M2Z,
    .zigbee_models_count=sizeof(kModels_TR_M2Z)/sizeof(kModels_TR_M2Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TR-M2Z", .vendor="Mazda",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::mazda
