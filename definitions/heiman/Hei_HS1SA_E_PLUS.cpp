// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman HS1SA_E_PLUS — hand-maintained parity override.
//
// Graduated from generated/Hei_HS1SA_E_PLUS.cpp: the generated def lowered the
// generic kFzIasZone (which emits the bare key "alarm") while the
// expose declares the semantic key "smoke". With no rename layer the
// primary sensor state never reached the shadow. z2m decodes this via
// fz.ias_smoke_alarm_1 (zoneStatus bit 0), so the typed kFzIasSmokeAlarm converter — which emits
// "smoke" directly — is at parity. Mirrors the already-correct
// HS1CA-M / HS2WD-E heiman defs and the trust / smartthings IAS ports.
// Smoke detector
// Tier 2: uses shared heiman converters (heimanClusterSpecial 0xFC90).
// z2m-source: heiman.ts #HS1SA-E-PLUS.
#include "definitions/_generic/_shared.hpp"
#include "definitions/heiman/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS1SA_E_PLUS[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasSmokeAlarm,
    &::zhc::heiman::kFzHeimanSmokeSpecial,
};
const TzConverter* const kTz_HS1SA_E_PLUS[] = {
    &::zhc::generic::kTzWarning,
    &::zhc::heiman::kTzHeimanIndicatorLight,
    &::zhc::heiman::kTzHeimanTemporaryMute,
    &::zhc::heiman::kTzHeimanInitiateTestMode,
};
constexpr const char* kModels_HS1SA_E_PLUS[] = { "HS1SA-EF-3.0", "HS1SA-E-PLUS" };

}  // namespace


constexpr Expose kExposes_HS1SA_E_PLUS[] = {
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

extern const PreparedDefinition kDef_HS1SA_E_PLUS{
    .zigbee_models=kModels_HS1SA_E_PLUS, .zigbee_models_count=sizeof(kModels_HS1SA_E_PLUS)/sizeof(kModels_HS1SA_E_PLUS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS1SA-E-PLUS", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS1SA_E_PLUS, .exposes_count=sizeof(kExposes_HS1SA_E_PLUS)/sizeof(kExposes_HS1SA_E_PLUS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS1SA_E_PLUS, .from_zigbee_count=sizeof(kFz_HS1SA_E_PLUS)/sizeof(kFz_HS1SA_E_PLUS[0]),
    .to_zigbee=kTz_HS1SA_E_PLUS, .to_zigbee_count=sizeof(kTz_HS1SA_E_PLUS)/sizeof(kTz_HS1SA_E_PLUS[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::heiman
