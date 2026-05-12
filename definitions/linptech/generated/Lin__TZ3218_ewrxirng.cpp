// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: NEUTERED — TS0225 / _TZ3218_ewrxirng is covered by the consolidated
// kDef_ES1ZZ_TY_ port (manufacturer_names list). Stub kept as link-time stub
// so registry.cpp's extern still resolves.
#include "zhc/runtime/definition.hpp"
namespace zhc::devices::linptech {
namespace {
constexpr const char* kM__TZ3218_ewrxirng[] = { "__NEUTERED__TZ3218_ewrxirng" };
}  // namespace

extern const PreparedDefinition kDefLin__TZ3218_ewrxirng{
    .zigbee_models=kM__TZ3218_ewrxirng,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr,.manufacturer_names_count=0,
    .model="TS0225__TZ3218_ewrxirng",.vendor="Linptech",
    .meta=nullptr,.exposes=nullptr,.exposes_count=0,
    .white_labels=nullptr,.white_labels_count=0,
    .from_zigbee=nullptr,.from_zigbee_count=0,
    .to_zigbee=nullptr,.to_zigbee_count=0,
    .configure=nullptr,.on_event=nullptr,
    .bindings=nullptr,.bindings_count=0,
};
}  // namespace zhc::devices::linptech
