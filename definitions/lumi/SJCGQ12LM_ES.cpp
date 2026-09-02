// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Aqara SJCGQ12LM-ES water-leak sensor T1, engineering-test build.
// New upstream in z2m v26.103.0 (#13071).
//
// The build reports the modelID "lumi.flood.agl02\tF\x01" — the production
// string followed by a TAB, an 'F' and a 0x01 byte. The firmware copies the
// Basic modelID byte-for-byte (no sanitising) and the matcher compares with
// strcmp, so the literal below carries exactly those three bytes; the
// production SJCGQ12LM keeps matching its own clean string.
//
// Upstream gives this build the IAS zone alarm only (alarm_1 + battery_low),
// no lumi 0xFCC0 battery / telemetry, and notes it is not Aqara-Home
// compatible.
//
// z2m-source: lumi.ts #SJCGQ12LM-ES (lumi.flood.agl02\tF\x01).
#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzIasWaterLeakAlarm,
};
constexpr const char* kModels[] = { "lumi.flood.agl02\tF\x01" };
constexpr Expose kExposes[] = {
    {"water_leak",  ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0, ExposeCategory::Diagnostic},
};
constexpr BindingSpec kBindings[] = {
    {1, 0x0500},
};
}  // namespace

extern const PreparedDefinition kDefSJCGQ12LM_ES{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .model = "SJCGQ12LM-ES", .vendor = "Aqara",
    .meta = nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels = nullptr, .white_labels_count = 0,
    .from_zigbee = kFz, .from_zigbee_count = sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee = nullptr, .to_zigbee_count = 0,
    .configure = nullptr, .on_event = nullptr,
    .bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};
}  // namespace zhc::devices::lumi
