// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Ensystec Leak-Protect-Zigbee — new-vendor scaffold (z2m v26.77.0 parity).
// z2m-source: ensystec.ts #Ensystec-Leak-Protect-Zigbee — m.iasZoneAlarm(
//   {zoneType:"water_leak", zoneAttributes:["alarm_1"]}); no genPowerCfg battery,
//   battery_low is a custom EnsystecLeakProtect-cluster attr (no generic decoder) — IAS bit 3 co-decodes it.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ensystec {
namespace {
const FzConverter* const kFz_Ensystec_Leak_Protect_Zigbee[] = {
    &::zhc::generic::kFzIasWaterLeakAlarm,
};

constexpr const char* kModels_Ensystec_Leak_Protect_Zigbee[] = { "Ensystec.ELPC" };

}  // namespace


// water_leak (zoneStatus bit 0) via kFzIasWaterLeakAlarm; tamper (bit 2) +
// battery_low (bit 3) co-decode from the same IAS zone-status word.
constexpr Expose kAutoExposes[] = {
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0500},
};

extern const PreparedDefinition kDef_Ensystec_Leak_Protect_Zigbee{
    .zigbee_models=kModels_Ensystec_Leak_Protect_Zigbee, .zigbee_models_count=sizeof(kModels_Ensystec_Leak_Protect_Zigbee)/sizeof(kModels_Ensystec_Leak_Protect_Zigbee[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Ensystec-Leak-Protect-Zigbee", .vendor="Ensystec",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Ensystec_Leak_Protect_Zigbee, .from_zigbee_count=sizeof(kFz_Ensystec_Leak_Protect_Zigbee)/sizeof(kFz_Ensystec_Leak_Protect_Zigbee[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ensystec
