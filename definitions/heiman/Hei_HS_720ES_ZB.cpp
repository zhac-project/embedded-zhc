// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Heiman HS-720ES-ZB smart carbon monoxide alarm (zigbeeModel
// JM720ES-EF-3.0), new in z2m v26.126.0..v26.133.0. m.battery +
// m.iasZoneAlarm carbon_monoxide (alarm_1, tamper, battery_low) + m.iasWarning
// (tz.warning). Mirrors HS1CA-E. Moes ZC-HM lists the same string as a
// manufacturerName on TS0601; that fingerprint is Tuya-styled and still wins for
// TS0601, while this plain zigbeeModel wins for JM720ES-EF-3.0.
// Not ported: identify, battery-percentage reporting, the warning composite expose.
// z2m-source: heiman.ts #HS-720ES-ZB
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::heiman {
namespace {
const FzConverter* const kFz_HS_720ES_ZB[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzIasCoAlarm,
};
const TzConverter* const kTz_HS_720ES_ZB[] = {
    &::zhc::generic::kTzWarning,
};
constexpr const char* kModels_HS_720ES_ZB[] = { "JM720ES-EF-3.0" };
constexpr Expose kExposes_HS_720ES_ZB[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"carbon_monoxide", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};
constexpr BindingSpec kBindings_HS_720ES_ZB[] = {
    {1, 0x0001},
};
}  // namespace

extern const PreparedDefinition kDef_HS_720ES_ZB{
    .zigbee_models=kModels_HS_720ES_ZB, .zigbee_models_count=sizeof(kModels_HS_720ES_ZB)/sizeof(kModels_HS_720ES_ZB[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="HS-720ES-ZB", .vendor="Heiman",
    .meta=nullptr, .exposes=kExposes_HS_720ES_ZB, .exposes_count=sizeof(kExposes_HS_720ES_ZB)/sizeof(kExposes_HS_720ES_ZB[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_HS_720ES_ZB, .from_zigbee_count=sizeof(kFz_HS_720ES_ZB)/sizeof(kFz_HS_720ES_ZB[0]),
    .to_zigbee=kTz_HS_720ES_ZB, .to_zigbee_count=sizeof(kTz_HS_720ES_ZB)/sizeof(kTz_HS_720ES_ZB[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_HS_720ES_ZB, .bindings_count=sizeof(kBindings_HS_720ES_ZB)/sizeof(kBindings_HS_720ES_ZB[0]),
};

}  // namespace zhc::devices::heiman
