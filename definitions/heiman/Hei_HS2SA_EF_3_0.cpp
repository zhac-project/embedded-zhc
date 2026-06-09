// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman HS2SA_EF_3_0 — hand-maintained parity override.
//
// Graduated from generated/Hei_HS2SA_EF_3_0.cpp: the generated def lowered the
// generic kFzIasZone (which emits the bare key "alarm") while the
// expose declares the semantic key "smoke". With no rename layer the
// primary sensor state never reached the shadow. z2m decodes this via
// fz.ias_smoke_alarm_1 (zoneStatus bit 0), so the typed kFzIasSmokeAlarm converter — which emits
// "smoke" directly — is at parity. Mirrors the already-correct
// HS1CA-M / HS2WD-E heiman defs and the trust / smartthings IAS ports.
// Smoke detector
// z2m-source: heiman.ts #HS2SA-EF-3.0.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS2SA_EF_3_0[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
};

constexpr const char* kModels_HS2SA_EF_3_0[] = { "HS2SA-EF-3.0" };

}  // namespace


constexpr Expose kExposes_HS2SA_EF_3_0[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};

extern const PreparedDefinition kDef_HS2SA_EF_3_0{
    .zigbee_models=kModels_HS2SA_EF_3_0, .zigbee_models_count=sizeof(kModels_HS2SA_EF_3_0)/sizeof(kModels_HS2SA_EF_3_0[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS2SA-EF-3.0", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS2SA_EF_3_0, .exposes_count=sizeof(kExposes_HS2SA_EF_3_0)/sizeof(kExposes_HS2SA_EF_3_0[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS2SA_EF_3_0, .from_zigbee_count=sizeof(kFz_HS2SA_EF_3_0)/sizeof(kFz_HS2SA_EF_3_0[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
