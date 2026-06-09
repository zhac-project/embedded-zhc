// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ctm AX_Water_Sensor — graduated from generated/ for an
// IAS-zone bit-mapping fix.
// AX Water Sensor, water leakage detector.
//
// Parity fix: z2m's `fzLocal.ctm_water_leak_alarm` reads zoneStatus
// bit 0 = "active_water_leak", bit 1 = "water_leak", bit 3 =
// "battery_low" — it does NOT emit "tamper". The auto-port wired the
// generic `kFzIasWaterLeakAlarm`, which reads the leak state from
// bit 0 (so it reported the *active* bit as the leak) and declared a
// spurious `tamper`. Re-wired onto the CTM-specific `kFzCtmWaterLeak`.
// z2m-source: ctm.ts #AX_Water_Sensor.
#include "definitions/_generic/_shared.hpp"
#include "definitions/ctm/_shared.hpp"

namespace zhc::devices::ctm {
namespace {
const FzConverter* const kFz_AX_Water_Sensor[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::ctm::kFzCtmWaterLeak,
};

constexpr const char* kModels_AX_Water_Sensor[] = { "AX Water Sensor" };

}  // namespace


// Manual: leak on bit 1 (water_leak) + bit 0 (active_water_leak),
// battery_low on bit 3 — matches z2m `ctm_water_leak_alarm`. No tamper.
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"active_water_leak", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x0500},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_AX_Water_Sensor{
    .zigbee_models=kModels_AX_Water_Sensor, .zigbee_models_count=sizeof(kModels_AX_Water_Sensor)/sizeof(kModels_AX_Water_Sensor[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="AX_Water_Sensor", .vendor="Ctm",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_AX_Water_Sensor, .from_zigbee_count=sizeof(kFz_AX_Water_Sensor)/sizeof(kFz_AX_Water_Sensor[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::ctm
