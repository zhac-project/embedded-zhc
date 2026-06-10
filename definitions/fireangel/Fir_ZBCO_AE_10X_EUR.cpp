// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Fireangel ZBCO-AE-10X-EUR — hand-authored parity port.
// CO alarm
//
// Missing from the generated registry entirely (the generator emitted only
// the W2-Module entry). z2m fingerprints this on modelID "Alarm_SD_Device" +
// manufacturerName "Fireangel" and decodes via fz.ias_carbon_monoxide_alarm_1
// (standard IAS bits: carbon_monoxide=bit0, tamper=bit2, battery_low=bit3 →
// generic kFzIasCoAlarm) plus fzLocal.fireangel_co_test (test = bit5 OR bit9 →
// vendor kFzFireangelCoTest). z2m's extra `alarm` binary expose has no
// decoder (no key ever reaches the shadow) so it is intentionally omitted;
// the decoded keys carbon_monoxide/tamper/battery_low/test are exposed.
//
// Coexists with kDef_W2_Module (same model "Alarm_SD_Device"): this def
// carries manufacturer_names so it is matched via the manufacturer-gated
// pass (find_definition Pass 1), winning for "Fireangel" devices; the
// model-only W2-Module def is reached only when the manufacturer differs.
// z2m-source: fireangel.ts #ZBCO-AE-10X-EUR.
#include "definitions/_generic/_shared.hpp"
#include "definitions/fireangel/_shared.hpp"

namespace zhc::devices::fireangel {
namespace {
const FzConverter* const kFz_ZBCO_AE_10X_EUR[] = {
    &::zhc::generic::kFzIasCoAlarm,        // carbon_monoxide(bit0)+tamper+battery_low
    &::zhc::fireangel::kFzFireangelCoTest, // test = bit5 || bit9
};

constexpr const char* kModels_ZBCO_AE_10X_EUR[] = { "Alarm_SD_Device" };
constexpr const char* kMfrs_ZBCO_AE_10X_EUR[]   = { "Fireangel" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"carbon_monoxide", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"test", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},   // ssIasZone
};

extern const PreparedDefinition kDef_ZBCO_AE_10X_EUR{
    .zigbee_models=kModels_ZBCO_AE_10X_EUR, .zigbee_models_count=sizeof(kModels_ZBCO_AE_10X_EUR)/sizeof(kModels_ZBCO_AE_10X_EUR[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kMfrs_ZBCO_AE_10X_EUR, .manufacturer_names_count=sizeof(kMfrs_ZBCO_AE_10X_EUR)/sizeof(kMfrs_ZBCO_AE_10X_EUR[0]),
    .model="ZBCO-AE-10X-EUR", .vendor="FireAngel",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZBCO_AE_10X_EUR, .from_zigbee_count=sizeof(kFz_ZBCO_AE_10X_EUR)/sizeof(kFz_ZBCO_AE_10X_EUR[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::fireangel
